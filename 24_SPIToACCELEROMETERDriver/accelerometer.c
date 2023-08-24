#include "accelerometer.h"

void ADXL345_Write( uint8_t reg_addr, uint8_t data )
{
    uint8_t spi1_write[ 2 ];

    /* Write bit (0) + multiple bit (0) + register address */
    spi1_write[ 0 ] = SPI_WRITE | MULTIPLE_BYTE_DISABLED | ( reg_addr & REG_ADDRESS_MASK );
    /* Data to be written into reg_addr */
    spi1_write[ 1 ] = data;

    /* Send instruction over SPI1 to the ADXL345 */
    SPI1_CS_Enable();
    SPI1_Write( spi1_write, 2U );
    SPI1_CS_Disable();
}

uint8_t ADXL345_Read( uint8_t reg_addr )
{
    uint8_t spi1_write;
    uint8_t spi1_read;

    /* Read bit (0) + multiple bit (0) + register address */
    spi1_write = SPI_READ | MULTIPLE_BYTE_DISABLED | ( reg_addr & REG_ADDRESS_MASK );

    /* Request reading of specified register and store data read into local variable */
    SPI1_CS_Enable();
    SPI1_Write( &spi1_write, 1U );
    SPI1_Read( &spi1_read, 1U );
    SPI1_CS_Disable();

    return spi1_read;
}

void ADXL345_Init( ACCEL_HandleTypeDef *haccel )
{
    /* Initialize SPI1 peripheral */
    SPI1_Init();

    /* Set ADXL345 to standby mode */
    ADXL345_Write( POWER_CTL_REG, MEASURE_STANDBY );

    /* Configure data format: self test, SPI 3/4 bit mode, interrupt active high/low, 
       full/10bit resolution, right/left justified, 2/4/8/16 g range */
    ADXL345_Write( DATA_FORMAT_REG, haccel->dataformat );

    /* Configure power mode and output data rate */
    ADXL345_Write( BW_RATE_REG, haccel->bwrate );

    /* Configure X, Y and Z axis offset values */
    ADXL345_Write( OFSX_REG, haccel->ofsx );
    ADXL345_Write( OFSY_REG, haccel->ofsy );
    ADXL345_Write( OFSZ_REG, haccel->ofsz );

    /* Map and enable user selected interrupts */
    ADXL345_API_Enable_Int( haccel, haccel->intenable, haccel->intmap );

    /* If single tap or double tap interrupts are enabled */
    if ( ( ( haccel->intenable & SINGLE_TAP_INT_ENABLED ) == SINGLE_TAP_INT_ENABLED ) || 
         ( ( haccel->intenable & DOUBLE_TAP_INT_ENABLED ) == DOUBLE_TAP_INT_ENABLED ) )
    {
        /* Configure tap threshold */
        ADXL345_Write( THRESH_TAP_REG, haccel->threshtap );

        /* Configure tap duration */
        ADXL345_Write( DUR_REG, haccel->dur );

        /* Enable participating axes in tap detection (and double tap supressing if double tap int. enabled) */
        ADXL345_Write( TAP_AXES_REG, haccel->tapaxes );
    }
    
    /* Configure latent and window for double tap detection (if enabled) */
    if ( ( haccel->intenable & DOUBLE_TAP_INT_ENABLED ) == DOUBLE_TAP_INT_ENABLED )
    {
        ADXL345_Write( LATENT_REG, haccel->latent );
        ADXL345_Write( WINDOW_REG, haccel->window );
    }

    /* If activity interrupt is enabled */
    if ( ( haccel->intenable & ACTIVITY_INT_ENABLED ) == ACTIVITY_INT_ENABLED )
    {
        /* Configure threshold for activity */
        ADXL345_Write( THRESH_ACT_REG, haccel->threshact );

        /* Enable participating axes in activity detection */
        ADXL345_Write( ACT_INACT_CTL_REG, haccel->actinactctl );
    }

    /* If inactivity interrupt is enabled */
    if ( ( haccel->intenable & INACTIVITY_INT_ENABLED ) == INACTIVITY_INT_ENABLED )
    {
        /* Configure threshold for inactivity */
        ADXL345_Write( THRESH_INACT_REG, haccel->threshinact );
        
        /* Configure time for inactivity */
        ADXL345_Write( TIME_INACT_REG, haccel->timeinact );

        /* Enable participating axes in inactivity detection */
        ADXL345_Write( ACT_INACT_CTL_REG, haccel->actinactctl );
    }

    /* If free-fall interrupt is enabled */
    if ( ( ( haccel->intenable & FREE_FALL_INT_ENABLED ) == FREE_FALL_INT_ENABLED ) )
    {
        /* Configure freefall threshold */
        ADXL345_Write( THRESH_FF_REG, haccel->threshff );

        /* Configure freefall time */
        ADXL345_Write( TIME_FF_REG, haccel->timeff );
    }

    /* Configure FIFO mode (and number of samples for the watermark interrupt, if enabled) */
    ADXL345_Write( FIFO_CTL_REG, haccel->fifoctl );
    
    /* Configure power mode (if measure bit is set, ADXL345 enters normal mode) */
    ADXL345_Write( POWER_CTL_REG, haccel->powerctl );
}

void ADXL345_API_Read_Devid( ACCEL_HandleTypeDef *haccel )
{
    /* Read ADXL345 DEVICE ID, 0xE5H should be returned */
    haccel->devid = ADXL345_Read( DEVID_REG );
}

void ADXL345_API_Standby_Mode( ACCEL_HandleTypeDef *haccel )
{
    /* Set ADXL345 to standby mode (keep the other parameters of POWER_CTL) */
    haccel->powerctl &= ~MEASURE_ENABLED;
    ADXL345_Write( POWER_CTL_REG, haccel->powerctl );
}

void ADXL345_API_Measurement_Mode( ACCEL_HandleTypeDef *haccel )
{
    /* Set ADXL345 to measurement mode (keep the other parameters of POWER_CTL) */
    haccel->powerctl |= MEASURE_ENABLED;
    ADXL345_Write( POWER_CTL_REG, haccel->powerctl );
}

void ADXL345_API_Low_Power_Mode( ACCEL_HandleTypeDef *haccel, uint8_t output_rate )
{
    /* Enable reduced power operation (at the expense of higher noise) for the specified
       output data rate (only a range from 12.5Hz to 400Hz offers lower current level) */
    haccel->bwrate = LOW_POWER_ENABLED | ( output_rate & RATE_BITS_MASK );
    ADXL345_Write( BW_RATE_REG, haccel->bwrate );
}

void ADXL345_API_Normal_Power_Mode( ACCEL_HandleTypeDef *haccel, uint8_t output_rate )
{
    /* Enable normal power operation for the specified output data rate */
    haccel->bwrate &= ~LOW_POWER_ENABLED;
    haccel->bwrate |= ( output_rate & RATE_BITS_MASK );
    ADXL345_Write( BW_RATE_REG, haccel->bwrate );
}

void ADXL345_API_Sleep_Mode( ACCEL_HandleTypeDef *haccel, uint8_t wakeup_rate )
{   
    /* Configure frequency of readings in sleep mode and enable sleep mode */
    haccel->powerctl = MEASURE_ENABLED | SLEEP_ENABLED | ( wakeup_rate & WAKEUP_BITS_MASK );
    ADXL345_Write( POWER_CTL_REG, haccel->powerctl );
}

void ADXL345_API_Wake_Up( ACCEL_HandleTypeDef *haccel )
{
    /* Disable sleep mode and set device to standby mode */
    haccel->powerctl &= ~SLEEP_ENABLED;
    ADXL345_API_Standby_Mode( haccel );

    /* Set device to measurement mode */
    ADXL345_API_Measurement_Mode( haccel );
}

/* For this feature to work as intended INACTIVITY interrupt must be enabled and configured, ACTIVITY is optional.
   - If ACTIVITY is enabled the device will wake up automatically,
   - If ACTIVITY is disabled, manual wake up has to be used to get the device out of sleep mode */
void ADXL345_API_Auto_Sleep_Mode( ACCEL_HandleTypeDef *haccel, uint8_t wakeup_rate )
{
    /* Enable auto_sleep mode and configure frequency of readings in sleep mode */
    haccel->powerctl |= AUTO_SLEEP_ENABLED | ( wakeup_rate & WAKEUP_BITS_MASK );
    ADXL345_Write( POWER_CTL_REG, haccel->powerctl );
}

/* For this feature to work accordingly, both INACTIVITY and ACTIVITY interrupts must be enabled and configured previously */
void ADXL345_API_Link_Mode( ACCEL_HandleTypeDef *haccel, uint8_t autosleep_mode, uint8_t wakeup_rate )
{
    /* If auto_sleep mode is enabled */
    if ( ( autosleep_mode & AUTO_SLEEP_ENABLED ) == AUTO_SLEEP_ENABLED )
    {
        /* Enable auto_sleep mode and configure frequency of readings in sleep mode */
        haccel->powerctl |= AUTO_SLEEP_ENABLED | ( wakeup_rate & WAKEUP_BITS_MASK );
    }

    /* Set ADXL345 to link mode + extra parameters if auto sleep is enabled */
    haccel->powerctl |= LINK_DELAY_ACT_UNTIL_INACT;
    ADXL345_Write( POWER_CTL_REG, haccel->powerctl );
}

void ADXL345_API_Manual_OffsetCal( ACCEL_HandleTypeDef *haccel, int16_t *x0g, int16_t *y0g, int16_t *z0g )
{
    uint8_t spi1_read;

    /* Read current measured values for X, Y and Z axis.
       NOTE: ADXL345 has to be oriented such that Z is in the 1g field whereas X and Y are in a 0g gravity field */
    ADXL345_API_Read_Axes( haccel, x0g , y0g, z0g );

    /* Read DATA_FORMAT register */
    haccel->dataformat = ADXL345_Read( DATA_FORMAT_REG );

    /* Obtain FULL_RES (bit 3) and RANGE (bits 1 and 0) from DATA_FORMAT */
    spi1_read = haccel->dataformat & ( FULL_RES_ENABLED | RANGE_BIT_1 | RANGE_BIT_0 );

    /* If full resolution is enabled */
    if ( ( spi1_read & FULL_RES_ENABLED ) == FULL_RES_ENABLED )
    {
        /* Subtract typ. sensitivity for full resolution mode at 1g in order to get Z for the 0g field */
        *z0g -= 256;
    }
    /* If 10-bit resolution is used */
    else
    {
        /* Get Z0g by subtracting typical sensitivity at 1g depending on the selected g range in DATA_FORMAT */
        switch ( spi1_read )
        {
            case RANGE_2G:
                *z0g -= 256;
                break;

            case RANGE_4G:
                *z0g -= 128;
                break;

            case RANGE_8G:
                *z0g -= 64;
                break;

            case RANGE_16G:
                *z0g -= 32;
                break;

            default:
                /* Do nothing */
                break;
        }
    }

    /* After this the values obtained for x0g, y0g and z0g must be subtracted from the X, Y and Z data registers
       for each new read in order to get the real values (take into consideration that scaling still needs to be
       done for a correct reading) */
}

void ADXL345_API_Auto_OffsetCal( ACCEL_HandleTypeDef *haccel )
{
    uint8_t  spi1_read;
    uint16_t ztyp;
    int16_t  xoff, yoff, zoff;
    double   scale_off;

    /* Read current measured values for X, Y and Z axis.
       NOTE: ADXL345 has to be oriented such that Z is in the 1g field whereas X and Y are in a 0g gravity field */
    ADXL345_API_Read_Axes( haccel, &xoff , &yoff, &zoff );

    /* Read DATA_FORMAT register */
    haccel->dataformat = ADXL345_Read( DATA_FORMAT_REG );

    /* Obtain FULL_RES (bit 3) and RANGE (bits 1 and 0) from DATA_FORMAT */
    spi1_read = haccel->dataformat & ( FULL_RES_ENABLED | RANGE_BIT_1 | RANGE_BIT_0 );

    /* If full resolution is enabled */
    if ( ( spi1_read & FULL_RES_ENABLED ) == FULL_RES_ENABLED )
    {
        /* Get the scale factor of the offset registers for the full resolution mode:
           - Offset regs. scale f. = 15.6 mg/LSB, Data regs. (full res.) scale f. = 3.9 mg/LSB */
        scale_off = 15.6 / 3.9;

        /* Z axis typ. sensitivity for full resolution at 1g */
        ztyp = 256U;
    }
    /* If 10-bit resolution is used */
    else
    {
        /* Get the scale factor of the offset registers depending on the selected g range */
        switch ( spi1_read )
        {
            case RANGE_2G:
                /* Offset regs. scale f. = 15.6 mg/LSB, Data regs. (2g) scale f. = 3.9 mg/LSB */
                scale_off = 15.6 / 3.9;

                /* Z axis typ. sensitivity for 2g range at 1g */
                ztyp = 256U;
                break;

            case RANGE_4G:
                /* Offset regs. scale f. = 15.6 mg/LSB, Data regs. (4g) scale f. = 7.8 mg/LSB */
                scale_off = 15.6 / 7.8;

                /* Z axis typ. sensitivity for 4g range at 1g */
                ztyp = 128U;
                break;

            case RANGE_8G:
                /* Offset regs. scale f. = 15.6 mg/LSB, Data regs. (8g) scale f. = 15.6 mg/LSB */
                scale_off = 15.6 / 15.6;

                /* Z axis typ. sensitivity for 8g range at 1g */
                ztyp = 64U;
                break;

            case RANGE_16G:
                /* Offset regs. scale f. = 15.6 mg/LSB, Data regs. (16g) scale f. = 31.2 mg/LSB */
                scale_off = 15.6 / 31.2;

                /* Z axis typ. sensitivity for 16g range at 1g */
                ztyp = 32U;
                break;

            default:
                /* Do nothing */
                break;
        }
    }

    /* Obtain the offset values that must be programmed into each offset register */
    xoff = ( int16_t )( -round( xoff / scale_off ) );
    yoff = ( int16_t )( -round( yoff / scale_off ) );
    zoff = ( int16_t )( -round( ( zoff - ztyp ) / scale_off ) );

    /* Write (burst mode) the offset values into the offset registers */
    SPI1_CS_Enable();
    spi1_read = SPI_WRITE | MULTIPLE_BYTE_ENABLED | OFSX_REG; /* this variable is reused for writing */
    SPI1_Write( &spi1_read, 1U );            /* request write in multiple byte mode to the offset registers */
    SPI1_Write( ( uint8_t* )( &xoff ), 1U ); /* write xoffset value into OFSX */
    SPI1_Write( ( uint8_t* )( &yoff ), 1U ); /* write yoffset value into OFSY */
    SPI1_Write( ( uint8_t* )( &zoff ), 1U ); /* write zoffset value into OFSZ */
    SPI1_CS_Disable();

    /* After this for each new read of the data registers X, Y and Z the programmed offset will be applied
       automatically (take into consideration that scaling still needs to be done for a correct reading) */
}

void ADXL345_API_Fifo_Bypass_Mode( ACCEL_HandleTypeDef *haccel )
{
    /* Set FIFO to bypass mode (no entries are stored).
       NOTE: Trigger and Samples bits of the FIFO_CTL register are set to 0 as they
             have no effect when bypass mode is implemented */
    haccel->fifoctl = FIFO_BYPASS_MODE;
    ADXL345_Write( FIFO_CTL_REG, haccel->fifoctl );
}

void ADXL345_API_Fifo_Fifo_Mode( ACCEL_HandleTypeDef *haccel )
{
    /* Set FIFO to fifo mode.
       NOTE: Trigger of the FIFO_CTL register is set to 0 as it has no effect when
             fifo mode is implemented */
    haccel->fifoctl  = haccel->fifoctl & SAMPLES_BITS_MASK;
    haccel->fifoctl |= FIFO_FIFO_MODE;
    ADXL345_Write( FIFO_CTL_REG, haccel->fifoctl );
}

void ADXL345_API_Fifo_Stream_Mode( ACCEL_HandleTypeDef *haccel )
{
    /* Set FIFO to stream mode.
       NOTE: Trigger of the FIFO_CTL register is set to 0 as it has no effect when
             stream mode is implemented */
    haccel->fifoctl  = haccel->fifoctl & SAMPLES_BITS_MASK;
    haccel->fifoctl |= FIFO_STREAM_MODE;
    ADXL345_Write( FIFO_CTL_REG, haccel->fifoctl );
}

void ADXL345_API_Fifo_Trigger_Mode( ACCEL_HandleTypeDef *haccel, uint8_t interrupts, uint8_t trigger_pin, uint8_t samples )
{
    uint8_t bit;

    /* Mask trigger bit to ensure a correct value to be written to FIFO_CTL */
    trigger_pin &= TRIGGER_EVENT_INT2;

    /* Read INT_MAP register */
    haccel->intmap = ADXL345_Read( INT_MAP_REG );

    /* If trigger_pin is linked to INT1 pin */
    if ( trigger_pin == TRIGGER_EVENT_INT1 )
    {
        /* Map the specified interrupts to INT1 pin as well */
        haccel->intmap &= ~interrupts;

        /* Search for any other interrupt also mapped to INT1.
           This will deactivate previous interrupts mapped to INT1, activate new interrupts linked to
           the trigger event on INT1 and any other interrupt mapped to INT2 remains the same */
        for ( bit = 0; bit <= 7; bit++ )
        {
            /* if interrupt is mapped to INT1 */
            if ( ( haccel->intmap & ( 0x01U << bit ) ) == 0x00U )
            {
                /* deselect interrupt in INT_ENABLE */
                haccel->intenable &= ~( 0x01 << bit );
            }
        }
    }
    /* If trigger_pin is linked to INT2 pin */
    else
    {
        /* Map the specified interrupts to the INT2 pin as well */
        haccel->intmap |= interrupts;

        /* Search for any other interrupt also mapped to INT2.
           This will deactivate previous interrupts mapped to INT2, activate new interrupts linked to the
           trigger event on INT2 and any other interrupt mapped to INT1 remains the same */
        for ( bit = 0; bit <= 7; bit++ )
        {
            /* If interrupt is mapped to INT2 */
            if ( ( haccel->intmap & ( 0x01U << bit ) ) != 0x00U )
            {
                /* Deselect interrupt in INT_ENABLE */
                haccel->intenable &= ~( 0x01 << bit );
            }
        }
    }

    /* Select interrupts to be used as trigger events on the specified INTx */
    haccel->intenable |= interrupts;

    /* Enable and map specified interrupts */
    ADXL345_API_Enable_Int( haccel, haccel->intenable, haccel->intmap );

    /* Set FIFO to trigger mode, link the trigger event to the specified INTx pin
       and define number of samples to be retained before the trigger event */
    haccel->fifoctl = 0x00U;
    haccel->fifoctl = FIFO_TRIGGER_MODE | trigger_pin | ( samples & SAMPLES_BITS_MASK );
    ADXL345_Write( FIFO_CTL_REG, haccel->fifoctl );
}

void ADXL345_API_Read_Axes( ACCEL_HandleTypeDef *haccel, int16_t *x_read, int16_t *y_read, int16_t *z_read )
{
    uint8_t spi1_write;
    uint8_t spi1_read[ 6 ];

    /* Read bit (0) + multiple bit (1) + DATAX0 register address */
    spi1_write = SPI_READ | MULTIPLE_BYTE_ENABLED | DATAX0_REG;

    /* Request burst reading of the data registers (DATAX0, DATAX1, DATAY0, DATAY1, DATAZ0 and DATAZ1)
       and store the data read into spi_read[0], spi_read[1] ... spi_read[5] respectively */
    SPI1_CS_Enable();
    SPI1_Write( &spi1_write, 1U );
    SPI1_Read( spi1_read, 6U );
    SPI1_CS_Disable();

    /* Align data for each axis and store it into destination variables */
    *x_read = ( int16_t )( ( spi1_read[ 1 ] << 8 ) + spi1_read[ 0 ] );
    *y_read = ( int16_t )( ( spi1_read[ 3 ] << 8 ) + spi1_read[ 2 ] );
    *z_read = ( int16_t )( ( spi1_read[ 5 ] << 8 ) + spi1_read[ 4 ] );

    /* Read DATA_FORMAT register */
    haccel->dataformat = ADXL345_Read( DATA_FORMAT_REG );

    /* Obtain JUSTIFY (bit 2) from DATA_FORMAT */
    spi1_read[ 1 ] = haccel->dataformat & JUSTIFY_LEFT;

    /* Obtain FULL_RES (bit 3) from DATA_FORMAT */
    spi1_read[ 2 ] = haccel->dataformat & FULL_RES_ENABLED;

    /* Obtain RANGE (bits 1 and 0) from DATA_FORMAT */
    spi1_read[ 3 ] = haccel->dataformat & ( RANGE_BIT_1 | RANGE_BIT_0 );

    /* If data is left justified and full resolution is enabled */
    if ( ( spi1_read[ 1 ] == JUSTIFY_LEFT ) && ( spi1_read[ 2 ] == FULL_RES_ENABLED ) )
    {   
        /* Get proper data depending on selected g range.
           NOTE: when right shifting, the C standard appends 0s or 1s on the left 
                 based on the sign of the data shifted, i.e. C handles the sign and
                 no extra operations are needed */
        switch( spi1_read[ 3 ] )
        {
            case RANGE_2G:
                /* 2g full res. mode only uses the 10 MSBs of the data register */ 
                *x_read >>= 6;
                *y_read >>= 6;
                *z_read >>= 6;
                break;

            case RANGE_4G:
                /* 4g full res. mode only uses the 11 MSBs of the data register */ 
                *x_read >>= 5;
                *y_read >>= 5;
                *z_read >>= 5;
                break;

            case RANGE_8G:
                /* 8g full res. mode only uses the 12 MSBs of the data register */ 
                *x_read >>= 4;
                *y_read >>= 4;
                *z_read >>= 4;
                break;

            case RANGE_16G:
                /* 16g full res. mode only uses the 13 MSBs of the data register */ 
                *x_read >>= 3;
                *y_read >>= 3;
                *z_read >>= 3;
                break;

            default:
                /* Do nothing */
                break;
        }
    }
    /* If data is left justified and 10 bit resolution */
    else if( ( spi1_read[ 1 ] == JUSTIFY_LEFT ) && ( spi1_read[ 2 ] == FULL_RES_10BIT_MODE ) )
    {   
        /* Left justified 10 bit mode only uses the 10 MSBs of the data registers */
        *x_read >>= 6;
        *y_read >>= 6;
        *z_read >>= 6;
    }

    /* NOTE: Right justified data uses the LSBs from the DATAX, Y and Z registers. Since the ADXL345
             fills the not used MSBs for all the g ranges with a proper value for negative and positive
             data, therefore there is no need for masking out such bits or perform any extra conversion
             as those bits are necessary in order to know the sign of said data */

    /* After this the values obtained for x, y and z must be
       scaled according to the selected resolution and g range */
}

float *ADXL345_API_Scale_Axes( ACCEL_HandleTypeDef *haccel, int16_t x_raw, int16_t y_raw, int16_t z_raw )
{
    uint8_t spi1_read;
    static float axes[ 3 ]; /* axes[0] = scaled x, axes[1] = scaled y, axes[2] = scaled z */

    /* Read DATA_FORMAT register */
    haccel->dataformat = ADXL345_Read( DATA_FORMAT_REG );

    /* Obtain FULL_RES (bit 3) and RANGE (bits 1 and 0) from DATA_FORMAT */
    spi1_read = haccel->dataformat & ( FULL_RES_ENABLED | RANGE_BIT_1 | RANGE_BIT_0 );

    /* If full resolution is configured */
    if ( ( spi1_read & FULL_RES_ENABLED ) == FULL_RES_ENABLED )
    {   
        /* Scale axes raw data for the full resolution mode */
        axes[ 0 ] = x_raw * 0.0039F;
        axes[ 1 ] = y_raw * 0.0039F;
        axes[ 2 ] = z_raw * 0.0039F;
    }
    /* If 10-bit mode is enabled */
    else
    {
        /* Scale axes raw data for the selected g range */
        switch ( spi1_read )
        {
            case RANGE_2G:
                axes[ 0 ] = x_raw * 0.0039F;
                axes[ 1 ] = y_raw * 0.0039F;
                axes[ 2 ] = z_raw * 0.0039F;
                break;

            case RANGE_4G:
                axes[ 0 ] = x_raw * 0.0078F;
                axes[ 1 ] = y_raw * 0.0078F;
                axes[ 2 ] = z_raw * 0.0078F;
                break;

            case RANGE_8G:
                axes[ 0 ] = x_raw * 0.0156F;
                axes[ 1 ] = y_raw * 0.0156F;
                axes[ 2 ] = z_raw * 0.0156F;
                break;

            case RANGE_16G:
                axes[ 0 ] = x_raw * 0.0312F;
                axes[ 1 ] = y_raw * 0.0312F;
                axes[ 2 ] = z_raw * 0.0312F;
                break;

            default:
                /* Do nothing */
                break;
        }
    }

    /* Return address of array containing the scaled data */
    return axes;
}

/* NOTE: this function reconfigures the POWER_CTL, DATA_FORMAT, BW_RATE, INT_ENABLE and FIFO_CTL
         registers in order to meet the necessary conditions for the SELF_TEST feature */
void ADXL345_API_Self_Test( ACCEL_HandleTypeDef *haccel, int16_t *x_st, int16_t *y_st, int16_t *z_st )
{
    int16_t axes_st_off[ 3 ];
    int16_t axes_st_on[ 3 ];

    /* Initialize TIM3 timer */
    TIM3_Init();

    /* Set ADXL345 to standby mode (no further data acquired) */
    ADXL345_API_Standby_Mode( haccel );

    /* Disable ADXL345 interrupts */
    ADXL345_API_Disable_Int( haccel, ALL_INTERRUPTS_ENABLED );

    /* Disable self test and force output format to be 16g right-justified and full resolution  */
    haccel->dataformat &= ~SELF_TEST_ENABLED;
    haccel->dataformat |= ( FULL_RES_ENABLED | JUSTIFY_RIGHT | RANGE_16G );
    ADXL345_Write( DATA_FORMAT_REG, haccel->dataformat );

    /* Enable normal power mode and force a 3200Hz output data rate */
    ADXL345_API_Normal_Power_Mode( haccel, RATE_3200_HZ );
    
    /* FIFO in bypass mode (FIFO = disabled) */
    ADXL345_API_Fifo_Bypass_Mode( haccel );

    /* ADXL345 in measurement mode */
    ADXL345_API_Measurement_Mode( haccel );

    /* Wait for proper data to be obtained in the DATA registers */
    TIM3_Delay_ms( 2U );

    /* Store samples of X, Y and Z axes for the SELF_TEST disabled case */
    ADXL345_API_Read_Axes( haccel, &axes_st_off[ 0 ], &axes_st_off[ 1 ], &axes_st_off[ 2 ] );

    /* Self test enabled */
    haccel->dataformat |= SELF_TEST_ENABLED;
    ADXL345_Write( DATA_FORMAT_REG, haccel->dataformat );

    /* Wait >4 times the configured output data rate (4 x 3200Hz) for the output to settle */
    TIM3_Delay_ms( 2U );
    
    /* Store samples of X, Y and Z axes for the SELF_TEST enabled case */
    ADXL345_API_Read_Axes( haccel, &axes_st_on[ 0 ], &axes_st_on[ 1 ], &axes_st_on[ 2 ] );

    /* Compute XST, YST and ZST in LSBs */
    *x_st = axes_st_on[ 0 ] - axes_st_off[ 0 ]; /* X_ST = X_ST_ON - X_ST_OFF */
    *y_st = axes_st_on[ 1 ] - axes_st_off[ 1 ]; /* Y_ST = Y_ST_ON - Y_ST_OFF */
    *z_st = axes_st_on[ 2 ] - axes_st_off[ 2 ]; /* Z_ST = Z_ST_ON - Z_ST_OFF */

    /* These results have to be compared with the "Self-Test Output in LSB for Full Resolution" table 
       in the datasheet of the ADXL345 (max and min values of this should be scaled according to the
       "Self-Test Output Scale Factors for Different Supply Voltages, VS" table if necessary) */
}

void ADXL345_API_Enable_Int( ACCEL_HandleTypeDef *haccel, uint8_t int_enable, uint8_t int_map )
{
    /* Disable all interrupts */
    haccel->intenable = 0x00;
    ADXL345_Write( INT_ENABLE_REG, haccel->intenable );

    /* Map interrupts to the specified INTx pin */
    haccel->intmap = int_map;
    ADXL345_Write( INT_MAP_REG, haccel->intmap );

    /* Enable user selected interrupts */
    haccel->intenable = int_enable;
    ADXL345_Write( INT_ENABLE_REG, haccel->intenable );

    /* NOTE: interrupts can be set active high or active low in INT_INVERT bit from DATA_FORMAT register */
}

void ADXL345_API_Disable_Int( ACCEL_HandleTypeDef *haccel, uint8_t interrupts )
{
    /* Disable selected interrupts */
    haccel->intenable &= ~interrupts;
    ADXL345_Write( INT_ENABLE_REG, haccel->intenable );
}

uint8_t ADXL345_API_Read_Int_Source( ACCEL_HandleTypeDef *haccel )
{   
    uint8_t source;
    source = ADXL345_Read( INT_SOURCE_REG );
    return source;
}

uint8_t ADXL345_API_Read_Tap_Source( ACCEL_HandleTypeDef *haccel )
{
    uint8_t axis_tap_source;

    /* Read ACT_TAP_STATUS register through SPI in order to know which axis exceeded the THRESH_TAP */
    axis_tap_source = ADXL345_Read( ACT_TAP_STATUS_REG );

    /* Get bits regarding single tap generation axes */
    axis_tap_source &= ( TAP_X_SOURCE | TAP_Y_ENABLED | TAP_Z_SOURCE );

    /* Return axis responsible for single tap interrupt */
    return axis_tap_source;
}

uint8_t ADXL345_API_Read_Activity_Source( ACCEL_HandleTypeDef *haccel )
{
    uint8_t axis_act_source;

    /* Read ACT_TAP_STATUS register through SPI in order to know which axis first detected activity */
    axis_act_source = ADXL345_Read( ACT_TAP_STATUS_REG );

    /* Get bits regarding activity axes */
    axis_act_source &= ( ACT_X_SOURCE | ACT_Y_SOURCE | ACT_Y_SOURCE );

    /* Return axis responsible for activity interrupt */
    return axis_act_source;
}

uint8_t ADXL345_API_Read_FIFO_Entries( ACCEL_HandleTypeDef *haccel )
{
    uint8_t entries;

    /* Read FIFO_STATUS register */
    entries = ADXL345_Read( FIFO_STATUS_REG );

    /* Keep ENTRIES bits in the FIFO_STATUS register */
    entries &= ENTRIES_BITS_MASK;

    /* Return number of entries stored in FIFO */
    return entries;
}
