#include "main.h"

/********************************* PINOUT *********************************
   I2C1 <-> LCD
   -----------
   PB8   -  SCK
   PB9   -  SDA
  
   SPI1 <-> ADXL345
   ----------------
   PA4   -  CS
   PA5   -  SCK
   PA6   -  MISO
   PA7   -  MOSI
   PC0   -  INT1 (ADXL345 DATA_READY interrupt)
   PC1   -  INT2 (ADXL345 user selected interrupt. Functionality varies) */

/* Main global variables */
LCD_HandleTypeDef   LCD_Handler;   /* Structure containing LCD configuration parameters */
ACCEL_HandleTypeDef ACCEL_Handler; /* Structure containing ACCELEROMETER configuration parameters */
uint8_t int2Flag = 0U;             /* Flag used to determine when an interrupt on ADXL345 INT2 has occurred */
int16_t x0g, y0g, z0g;             /* Initial axes DATA values used in manual calibration */
int16_t xmeas, ymeas, zmeas;       /* Axes DATA values read from the ADXL345 whenever INT2 is triggered */
float *scaled_axes;                /* Axes scaled DATA values according to the selected format and g range */

int main( void )
{  
   uint8_t  state = IDLE;          /* Current state */
   uint32_t stateClock;            /* State machine timer */
   uint32_t int2Clock;             /* INT2 interrupt timer for printing on LCD */
   uint8_t  intsource;             /* Interrupt source to be handled */
   uint8_t  tapaxis;               /* Responsible axis for single and double tap events */
   uint8_t  activityaxis;          /* Responsible axis for activity event */
   uint8_t  entries;               /* Data values stored in FIFO */
   uint8_t  interrupts;            /* Stores initial interrupts to be enabled */

   /* Initialize Systick (timebase for the state machine) */
   SysTick_Init();

   /* Configure GPIO input interrupts for handling ADXL345 INT interrupt pins */
   GPIO_Interrupt_Init();

   /**************************** LCD module initialization ****************************/
   LCD_Handler.lcdaddress        = LCD_ADDRESS;
   LCD_Handler.backlight         = BACKLIGHT_ENABLED;
   LCD_Handler.datalenght        = INTERFACE_DATA_4_BIT;
   LCD_Handler.displayline       = DISPLAY_2_LINES;
   LCD_Handler.font              = DISPLAY_FONT_TYPE_5x8;
   LCD_Handler.display           = DISPLAY_ON;
   LCD_Handler.cursor            = CURSOR_OFF;
   LCD_Handler.blink             = CURSOR_BLINK_ON;
   LCD_Handler.incrementddram    = ID_DDRAM_ADDRESS_RIGHT;
   LCD_Handler.shiftdisplay      = SHIFT_DISPLAY_OFF;
   LCD_Init( &LCD_Handler );

   /************************ ADXL345 accelerometer initialization **********************/
   ACCEL_Handler.dataformat      = SELF_TEST_DISABLED | SPI_4WIRE_MODE | INT_INVERT_ACTIVE_HIGH | FULL_RES_10BIT_MODE | JUSTIFY_RIGHT | RANGE_2G;
   ACCEL_Handler.powerctl        = LINK_INACT_ACT_CONCURRENT | AUTO_SLEEP_DISABLED | MEASURE_ENABLED | SLEEP_DISABLED | WAKEUP_8HZ;
   ACCEL_Handler.bwrate          = LOW_POWER_DISABLED | RATE_6_25_HZ; /* Output data rate = 1/6.25Hz = 160ms */
   ACCEL_Handler.fifoctl         = FIFO_BYPASS_MODE;        /* No samples stored in FIFO */
   ACCEL_Handler.ofsx            = 0x00U;                   /* No offset on X axis (during initialization) */
   ACCEL_Handler.ofsy            = 0x00U;                   /* No offset on Y axis (during initialization) */
   ACCEL_Handler.ofsz            = 0x00U;                   /* No offset on Z axis (during initialization) */
   /* ADXL345 DATA_READY interrupt (triggered every 160ms and corresponds to the output data rate set in ACCEL_Handler.bwrate) */
   interrupts                    = DATA_READY_INT_ENABLED;  /* Initial interrupts to be enabled, used to evaluate which interrupt is triggered.
                                                               (INT_SOURCE does not work for this purpose and intenable structure member is 
                                                               updated automatically by the driver, therefore when interrupts are disabled
                                                               intenable is read as 0 and no interrupt would be correctly handled) */
   ACCEL_Handler.intenable       = interrupts;              /* DATA_READY interrupt enabled */
   ACCEL_Handler.intmap          = DATA_READY_INT_MAP_INT1; /* DATA_READY interrupt output mapped to ADXL345 INT1 pin */
   #if defined (ADXL345_SINGLE_TAP_INTERRUPT)
      interrupts                |= SINGLE_TAP_INT_ENABLED;
      ACCEL_Handler.intenable    = interrupts;              /* DATA_READY + SINGLE_TAP interrupts enabled */
      ACCEL_Handler.intmap      |= SINGLE_TAP_INT_MAP_INT2; /* SINGLE_TAP interrupt output mapped to ADXL345 INT2 pin */
      ACCEL_Handler.threshtap    = THRESH_TAP_BIT_3;        /* 0x08 =>   8 x 62.5mg = 0.500g */
      ACCEL_Handler.dur          = 0xFFU;                   /* 0xFF => 255 x 625us  = 159.37ms */
      ACCEL_Handler.tapaxes      = TAP_Y_ENABLED;           /* Enable Y axis for single tap detection */
   #elif defined (ADXL345_DOUBLE_TAP_INTERRUPT)
      interrupts                |= DOUBLE_TAP_INT_ENABLED;
      ACCEL_Handler.intenable    = interrupts;              /* DATA_READY + DOUBLE_TAP interrupts enabled */
      ACCEL_Handler.intmap      |= DOUBLE_TAP_INT_MAP_INT2; /* DOUBLE_TAP interrupt output mapped to ADXL345 INT2 pin */
      ACCEL_Handler.threshtap    = 0x20U;                   /* 0x20 =>  32 x 62.5mg = 2g */
      ACCEL_Handler.dur          = 0x30U;                   /* 0x30 =>  48 x 625us  = 30ms */
      ACCEL_Handler.latent       = 0x01U;                   /* 0x01 =>   1 x 1.25ms = 1.25ms */
      ACCEL_Handler.window       = 0x90U;                   /* 0x90 => 144 x 1.25ms = 180ms */
      ACCEL_Handler.tapaxes      = SUPPRESS_ENABLED | TAP_X_ENABLED; /* Invalidate 2nd tap detection during the latency time
                                                                        and enable X axis for double tap detection */
   #elif defined (ADXL345_ACTIVITY_INTERRUPT)
      interrupts                |= ACTIVITY_INT_ENABLED;
      ACCEL_Handler.intenable    = interrupts;              /* DATA_READY + ACTIVITY interrupts enabled */
      ACCEL_Handler.intmap      |= ACTIVITY_INT_MAP_INT2;   /* ACTIVITY interrupt output mapped to ADXL345 INT2 pin */
      ACCEL_Handler.threshact    = 0x0CU;                   /* 0x0C => 12 x 62.5mg = 0.750g */
      ACCEL_Handler.actinactctl  = ACT_DC_COUPLED | ACT_X_ENABLED | ACT_Y_ENABLED; /* DC coupled mode, X & Y axes enabled for activity detection */
   #elif defined (ADXL345_INACTIVITY_INTERRUPT)
      interrupts                |= INACTIVITY_INT_ENABLED;
      ACCEL_Handler.intenable    = interrupts;              /* DATA_READY + INACTIVITY interrupts enabled */
      ACCEL_Handler.intmap      |= INACTIVITY_INT_MAP_INT2; /* INACTIVITY interrupt output mapped to ADXL345 INT2 pin */
      ACCEL_Handler.threshinact  = 0x0AU;                   /* 0x0A => 10 x 62.5mg = 0.625g */
      ACCEL_Handler.timeinact    = 0x05U;                   /* 0x05 =>  5 x 1sec   = 5 seconds */
      ACCEL_Handler.actinactctl  = INACT_DC_COUPLED | INACT_Z_ENABLED; /* DC coupled mode and Z axis enabled for inactivity detection */
   #elif defined (ADXL345_FREE_FALL_INTERRUPT)
      interrupts                |= FREE_FALL_INT_ENABLED;
      ACCEL_Handler.intenable    = interrupts;              /* DATA_READY + FREE_FALL interrupts enabled */
      ACCEL_Handler.intmap      |= FREE_FALL_INT_MAP_INT2;  /* FREE_FALL interrupt output mapped to ADXL345 INT2 pin */
      ACCEL_Handler.threshff     = 0x06U;                   /* 0x06 => 6 x 62.5mg = 0.375g */
      ACCEL_Handler.timeff       = 0x03U;                   /* 0x03 => 3 x 5ms    = 15ms */
   #elif defined (ADXL345_WATERMARK_INTERRUPT)
      interrupts                 = WATERMARK_INT_ENABLED;
      ACCEL_Handler.intenable    = interrupts;              /* WATERMARK interrupt enabled (DATA_READY interrupt disabled) */
      ACCEL_Handler.intmap      |= WATERMARK_INT_MAP_INT2;  /* Interrupt output mapped to ADXL345 INT2 pin */
      ACCEL_Handler.fifoctl      = FIFO_FIFO_MODE | 0x05U;  /* FIFO MODE and 5 samples for the WATERMARK interrupt to be triggered */
      ACCEL_Handler.bwrate       = LOW_POWER_DISABLED | RATE_3200_HZ; /* output data rate = 1 / 3200Hz = 312us */
   #elif defined (ADXL345_OVERRUN_INTERRUPT)
      interrupts                |= OVERRUN_INT_ENABLED;
      ACCEL_Handler.intenable    = interrupts;              /* Both OVERRUN and DATA_READY interrupts are enabled, but data will not be read
                                                               whenever DATA_READY is triggered (since the purpose is to replace unread data in
                                                               order to trigger the OVERRUN). Refer to interrupts.c file */
      ACCEL_Handler.intmap      |= OVERRUN_INT_MAP_INT2;    /* interrupt output mapped to ADXL345 INT2 pin */
      ACCEL_Handler.bwrate       = LOW_POWER_DISABLED | RATE_0_20_HZ; /* output data rate = 1 / 0.2 = 5 secs */
      #undef MANUAL_CALIBRATION /* NOTE: for the sake of this program when OVERRUN interrupt is enabled, manual calibration must be avoided
                                         since this yields to the values read for X, Y and Z axes to be incremented/decremented endlessly
                                         (in the MANUAL_OFFSET state) due to no values are read when the DATA_READ interrupt is triggered
                                         (DATA_READY does not read the DATA registers despite being enabled in this case. Refer to interrupts.c) */
   #endif
   ADXL345_Init( &ACCEL_Handler );

   /* If no calibration is enabled */
   #if defined (NO_CALIBRATION) 
      /* Set to zero the offset registers */
      ADXL345_Write( OFSX_REG, 0x00U );
      ADXL345_Write( OFSY_REG, 0x00U );
      ADXL345_Write( OFSZ_REG, 0x00U );
   /* If manual calibration is enabled */
   #elif defined (MANUAL_CALIBRATION)
      /* Get the initial values of the 3 axes (needed for manual calibration) */
      ADXL345_API_Manual_OffsetCal( &ACCEL_Handler, &x0g, &y0g, &z0g );
   /* If automatic calibration is enabled */
   #else
      /* Configure the ADXL345 offset registers */
      ADXL345_API_Auto_OffsetCal( &ACCEL_Handler );
   #endif

   /* Example on how to read the Device ID (ACCEL_Handler.devid has to be read as 0xE5 after the execution of this function)
   ADXL345_API_Read_Devid( &ACCEL_Handler ); */

   /* Example on how to enable the low power mode for a 400Hz output data rate
   ADXL345_API_Low_Power_Mode( &ACCEL_Handler, RATE_400_HZ ); */

   /* Example on how to manually set the ADXL345 to sleep mode. In sleep mode, DATA_READY is suppressed,
      data transmission to FIFO is stopped and the sampling rate switches to the specified in the WAKEUP bits.
      In this mode, only the ACTIVITY function can be used
   ADXL345_API_Sleep_Mode( &ACCEL_Handler, WAKEUP_4HZ ); */
   /* if ADXL345 was manually set to sleep mode, the next line can be used to wake it up
   ADXL345_API_Wake_Up( &ACCEL_Handler ); */
   
   /* Example for setting the ADXL345 to autosleep mode with a reading frequency of 1Hz in sleep mode.
      NOTE: inactivity interrupt must be enabled and configured properly, activity interrupt is optional
            - if activity is enabled the device will wake up automatically
            - if activity is disabled then the ADXL345 must be awakened manually
   ADXL345_API_Auto_Sleep_Mode( &ACCEL_Handler, WAKEUP_1HZ ); */

   /* Example on how to enable link mode with autosleep mode and a reading frequency of 2Hz in sleep mode.
      NOTE: both activity and inactivity interrupts must be enabled and properly configured in order for the link
            mode to work accordingly (these interrupts must be handled and cleared by reading INT_SOURCE,
            otherwise the ADXL345 cannot go into autosleep mode)
   ADXL345_API_Link_Mode( &ACCEL_Handler, AUTO_SLEEP_ENABLED, WAKEUP_2HZ ); */

   /* Example on how to set FIFO to Bypass mode
   ADXL345_API_Fifo_Bypass_Mode( &ACCEL_Handler ); */

   /* Example on how to set FIFO to Fifo mode
   ADXL345_API_Fifo_Fifo_Mode( &ACCEL_Handler ); */

   /* Example on how to set FIFO to Stream mode
   ADXL345_API_Fifo_Stream_Mode( &ACCEL_Handler ); */

   /* Example on how to set FIFO to Trigger mode, SINGLE_TAP and INACTIVITY trigger events are linked to INT2 pin
      and a value of 10 samples are retained before the trigger event
   ADXL345_API_Fifo_Trigger_Mode( &ACCEL_Handler, SINGLE_TAP_INT_ENABLED | INACTIVITY_INT_ENABLED, TRIGGER_EVENT_INT2, 0x0A ); */

   /* Example on how to use the SELF_TEST feature:
      - The self test LSBs values must be compared to the table explained in the next point 
      - Max and Min values of the "Self-Test Output in LSB for Full Resolution" table in the ADXL345 datasheet have to be scaled 
        according the "Self-Test Output Scale Factors for Different Supply Voltages, VS" table
      - Test passes if the self test values are within the range of the Mix and Min values
      - Bare in mind that using this function reconfigures some of the ADXL345 registers. See the function for more details
   int16_t xst, yst, zst; self test values in LSBs returned by the SELF_TEST function
   ADXL345_API_Self_Test( &ACCEL_Handler, &xst, &yst, &zst ); */

   /* Initialize state machine timer */
   stateClock = Get_SysTick();

   while( 1 )
   {  
      /* If 40ms have passed */
      if ( ( Get_SysTick() - stateClock ) >= 40U )
      {
         /* Update state machine clock */
         stateClock = Get_SysTick();

         switch( state )
         {
            case IDLE:
               idle( &state );
               break;

            case MANUAL_OFFSET:
               manual_offset( &state );
               break;

            case SCALE_AXES:
               scale_axes( &state );
               break;

            case PRINT_AXES:
               print_axes( &state );
               break;

            case PRINT_SINGLE_TAP:
               print_single_tap( &state, &int2Clock, &tapaxis );
               break;

            case PRINT_DOUBLE_TAP:
               print_double_tap( &state, &int2Clock, &tapaxis );
               break;

            case PRINT_ACTIVITY:
               print_activity( &state, &int2Clock, &activityaxis );
               break;

            case PRINT_INACTIVITY:
               print_inactivity( &state, &int2Clock );
               break;

            case PRINT_FREE_FALL:
               print_free_fall( &state, &int2Clock );
               break;

            case PRINT_WATERMARK:
               print_watermark( &state, &entries );
               break;

            case PRINT_OVERRUN:
               print_overrun( &state );
               break;

            case DISABLE_INTERRUPTS:
               disable_interrupts( &state );
               break;

            case ENABLE_INTERRUPTS:
               enable_interrupts( &state, &interrupts );
               break;

            case CLEAR_INT2_FLAGS:
               clear_int2_flags( &state, &int2Clock );
               break;

            case EVALUATE_INT2:
               evaluate_int2( &state, &interrupts, &intsource );
               break;

            case GET_TAP_AXIS:
               get_tap_axis( &state, &tapaxis, &intsource );
               break;

            case GET_ACTIVITY_AXIS:
               get_activity_axis( &state, &activityaxis );
               break;

            case READ_FIFO_ENTRIES:
               read_fifo_entries( &state, &entries );
               break;

            case READ_INT2_PIN:
               read_int2_pin( &state );
               break;

            case SET_STANDBY_MODE:
               set_standby_mode( &state );
               break;

            case SET_MEASUREMENT_MODE:
               set_measurement_mode( &state );
               break;

            default:
               state = IDLE;
               break;
         }
      }
   }

   return 0U;
}

void idle( uint8_t *curr_state )
{
   /* If INT2 was triggered on the ADXL345 (GPIOC1 input) and OVERRUN interrupt is disabled */
   if ( ( int2Flag == 1U ) && ( ( ACCEL_Handler.intenable & OVERRUN_INT_ENABLED ) != OVERRUN_INT_ENABLED ) )
   {
      *curr_state = DISABLE_INTERRUPTS;
   }
   /* If INT2 was triggered on the ADXL345 (GPIOC1 input) and OVERRUN interrupt is enabled */
   else if ( ( int2Flag == 1U ) && ( ( ACCEL_Handler.intenable & OVERRUN_INT_ENABLED ) == OVERRUN_INT_ENABLED ) )
   {
      /* Clear screen */
      LCD_Send_Instruction( &LCD_Handler, CLEAR_DISPLAY_REG );

      /* OVERRUN interrupt is handled differently in this program to any other interrupt, reason is due to the
         ADXL345 interrupts will not be disabled (INT1 and INT2 pins will not be forced to LOW) in order to know
         when DATA is available on the DATA registers (INT1) and when OVERRUN interrupt is triggered (INT2) */
      *curr_state = PRINT_OVERRUN;
   }
   /* If INT2 was not triggered */
   else
   {
      #if defined (MANUAL_CALIBRATION)
         /* Offset for ADXL345 DATA registers must be applied manually */
         *curr_state = MANUAL_OFFSET;
      #else
         /* Offset for ADXL345 DATA registers is applied automatically (no extra action required) */
         *curr_state = SCALE_AXES;
      #endif
   }
}

void manual_offset( uint8_t *curr_state )
{
   /* Apply offset to the data read (xmeas, ymeas and zmeas are the raw data values
      obtained from the ADXL345 whenever the DATA_READY interrupt is triggered on GPIOC0) */
   xmeas -= x0g;
   ymeas -= y0g;
   zmeas -= z0g;

   *curr_state = SCALE_AXES;
}

void scale_axes( uint8_t *curr_state )
{
   /* Scale data according to the selected g range (at this point an offset has been
      already applied on xmeas, ymeas and zmeas either manually or automatically) */
   scaled_axes = ADXL345_API_Scale_Axes( &ACCEL_Handler, xmeas, ymeas, zmeas );

   *curr_state = PRINT_AXES;
}

void print_axes( uint8_t *curr_state )
{
   /* Print read acceleration (one sample is obtained whenever INT1 is triggered) */
   lcd_print_all_axes_info( &LCD_Handler, scaled_axes[ 0 ], scaled_axes[ 1 ], scaled_axes[ 2 ] );

   *curr_state = IDLE;
}

void print_single_tap( uint8_t *curr_state, uint32_t *int2_clk, uint8_t *tap_axis )
{
   lcd_print_single_tap( &LCD_Handler, &ACCEL_Handler, *tap_axis, ACCEL_Handler.threshtap * 0.0625F );

   /* Start timer to stop printing the INT2 event on the LCD */
   *int2_clk = Get_SysTick();

   *curr_state = CLEAR_INT2_FLAGS;
}

void print_double_tap( uint8_t *curr_state, uint32_t *int2_clk, uint8_t *tap_axis )
{
   lcd_print_double_tap( &LCD_Handler, &ACCEL_Handler, *tap_axis, ACCEL_Handler.threshtap * 0.0625F );

   /* Start timer to stop printing the INT2 event on the LCD */
   *int2_clk = Get_SysTick();

   *curr_state = CLEAR_INT2_FLAGS;
}

void print_activity( uint8_t *curr_state, uint32_t *int2_clk, uint8_t *activity_axis )
{
   lcd_print_activity( &LCD_Handler, &ACCEL_Handler, *activity_axis, ACCEL_Handler.threshact * 0.0625F );

   /* Start timer to stop printing the INT2 event on the LCD */
   *int2_clk = Get_SysTick();

   *curr_state = CLEAR_INT2_FLAGS;
}

void print_inactivity( uint8_t *curr_state, uint32_t *int2_clk )
{
   lcd_print_inactivity( &LCD_Handler, &ACCEL_Handler, ACCEL_Handler.actinactctl, ACCEL_Handler.threshinact * 0.0625F );

   /* Start timer to stop printing the INT2 event on the LCD */
   *int2_clk = Get_SysTick();

   *curr_state = CLEAR_INT2_FLAGS;
}

void print_free_fall( uint8_t *curr_state, uint32_t *int2_clk )
{
   lcd_print_freefall( &LCD_Handler, &ACCEL_Handler, ACCEL_Handler.threshff * 0.0625F );

   /* Start timer to stop printing the INT2 event on the LCD */
   *int2_clk = Get_SysTick();

   *curr_state = CLEAR_INT2_FLAGS;
}

void print_overrun( uint8_t *curr_state )
{
   lcd_print_overrun( &LCD_Handler, &ACCEL_Handler );

   *curr_state = READ_INT2_PIN;
}

void print_watermark( uint8_t *curr_state, uint8_t *entries )
{
   lcd_print_watermark( &LCD_Handler, &ACCEL_Handler, ACCEL_Handler.fifoctl & SAMPLES_BITS_MASK, *entries );

   *curr_state = READ_FIFO_ENTRIES;
}

void disable_interrupts( uint8_t *curr_state )
{
   /* This allows handling the current interrupt properly and no other
      interrupt will be triggered in the meantime */

   /* Disable interrupts on the NUCLEO board
   __disable_irq(); (do not do this as it disables SysTick interrupt leading to an INFINITE LOOP) */

   /* Disable INT1 = DATA_READY (GPIOC0) and INT2 = mapped interrupt (GPIOC1) interrupts on the nucleo board 
      (NVIC interrupt 5 = EXTI0_1 interrupt) */
   NVIC->ICER[ 0 ] = NVIC_ICER_CLRENA_5;

   /* Disable interrupts on the ADXL345 */
   ADXL345_API_Disable_Int( &ACCEL_Handler, ALL_INTERRUPTS_ENABLED );

   /* Clear screen */
   LCD_Send_Instruction( &LCD_Handler, CLEAR_DISPLAY_REG );

   *curr_state = EVALUATE_INT2;
}

void enable_interrupts( uint8_t *curr_state, uint8_t *interrupts )
{
   /* Re-enable interrupts on the NUCLEO board
   __enable_irq(); */

   /* Re-enable INT1 = DATA READY (GPIOC0) and INT2 = user mapped (GPIOC1) interrupts on the nucleo board */
   NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_5;

   /* Ee-enable intial interrupts on the ADXL345 */
   ADXL345_API_Enable_Int( &ACCEL_Handler, *interrupts, ACCEL_Handler.intmap );

   *curr_state = IDLE;
}

void clear_int2_flags( uint8_t *curr_state, uint32_t *int2_clk )
{
   /* If 2 seconds have passed since the INT2 event was printed on the LCD */
   if ( ( Get_SysTick() - *int2_clk ) >= 2000U )
   {
      /* Clear INT2 flag */
      int2Flag = 0U;

      /* Read INT_SOURCE to clear interrupt flags on the ADXL345 (set both INT1 and INT2 to LOW) */
      ( void ) ADXL345_Read( INT_SOURCE_REG );

      /* Clear screen */
      LCD_Send_Instruction( &LCD_Handler, CLEAR_DISPLAY_REG );

      *curr_state = ENABLE_INTERRUPTS;
   }
}

void evaluate_int2( uint8_t *curr_state, uint8_t *interrupts, uint8_t *int_source )
{
   /* There is no way to know which interrupt was triggered, not even by reading INT_STATUS due to
      "DATA_READY, WATERMARK, and OVERRUN bits are always set if the corresponding events occur,
      regardless of the INT_ENABLE register settings" as the ADXL345 datasheet states ...
      ... therefore taking into account that only 1 interrupt is allowed to be enabled (apart from
      DATA_READY) in this program, the original value of the member intenable of the structure will be used */

   /* Get interrupt "triggered" based on previous configured value (excluding DATA_READY)*/
   *int_source = *interrupts & ~DATA_READY_INT_ENABLED;

   /* Act according to the interrupt triggered */
   switch ( *int_source )
   {
      case SINGLE_TAP_INT_ENABLED:
      case DOUBLE_TAP_INT_ENABLED:
         *curr_state = GET_TAP_AXIS;
         break;

      case ACTIVITY_INT_ENABLED:
         *curr_state = GET_ACTIVITY_AXIS;
         break;

      case INACTIVITY_INT_ENABLED:
         *curr_state = PRINT_INACTIVITY;
         break;

      case FREE_FALL_INT_ENABLED:
         *curr_state = PRINT_FREE_FALL;
         break;

      case WATERMARK_INT_ENABLED:
         *curr_state =  SET_STANDBY_MODE;
         break;

      case OVERRUN_INT_ENABLED:
         *curr_state = PRINT_OVERRUN;
         break;

      default:
         *curr_state = CLEAR_INT2_FLAGS;
         break;
   }
}

void get_tap_axis( uint8_t *curr_state, uint8_t *tap_axis, uint8_t *int_source )
{
   /* Read ADXL345 to determine which axis generated the single/double tap */
   *tap_axis = ADXL345_API_Read_Tap_Source( &ACCEL_Handler );

   /* If single tap triggered the interrupt */
   if ( *int_source == SINGLE_TAP_INT_ENABLED )
   {
      *curr_state = PRINT_SINGLE_TAP;
   }
   /* If double tap triggered the interrupt */
   else
   {
      *curr_state = PRINT_DOUBLE_TAP;
   }
}

void get_activity_axis( uint8_t *curr_state, uint8_t *activity_axis )
{
   /* Read ADXL345 to determine which axis detected activity */
   *activity_axis = ADXL345_API_Read_Activity_Source( &ACCEL_Handler );

   *curr_state = PRINT_ACTIVITY;
}

void read_fifo_entries( uint8_t *curr_state, uint8_t *entries )
{
   /* Read number of samples stored in FIFO */
   *entries = ADXL345_API_Read_FIFO_Entries( &ACCEL_Handler );

   /* If number of entries in FIFO is equal or greater than the number of samples configured
      in FIFO_CTL for the WATERMARK interrupt to be triggered */
   if ( *entries >= ( ACCEL_Handler.fifoctl & SAMPLES_BITS_MASK ) )
   {
      *curr_state = PRINT_WATERMARK;
   }
   /* If number of entries in FIFO is less than samples in FIFO_CTL */
   else
   {
      *curr_state = SET_MEASUREMENT_MODE;
   }
}

void read_int2_pin( uint8_t *curr_state )
{
   /* If ADXL345 INT2 pin (GPIOC1) is HIGH (= OVERRUN is still present, unread data has been replaced) */
   if ( ( GPIOC->IDR & GPIO_IDR_1 ) == GPIO_IDR_1 )
   {
      *curr_state = PRINT_OVERRUN;
   }
   /* If ADXL345 INT2 pin (GPIOC1) is LOW (data has already been read from FIFO) */
   else 
   {
      *curr_state = CLEAR_INT2_FLAGS;
   }
}

void set_standby_mode( uint8_t *curr_state )
{
   /* Set ADXL345 to standby mode */
   ADXL345_API_Standby_Mode( &ACCEL_Handler );

   *curr_state = READ_FIFO_ENTRIES;
}

void set_measurement_mode( uint8_t *curr_state )
{
   /* Set ADXL345 to measurement mode */
   ADXL345_API_Measurement_Mode( &ACCEL_Handler );

   *curr_state = CLEAR_INT2_FLAGS;
}
