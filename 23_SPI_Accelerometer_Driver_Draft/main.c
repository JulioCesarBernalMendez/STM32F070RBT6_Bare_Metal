/* NOTE: This is a mess of a file and is meant to be used for documentation and debugging purposes regarding
         the ADXL345 initialization as well as for the writting and reading processes.
         A proper driver for the ADXL345 accelerometer will be developed in the future. */

#include "main.h"
#include "math.h"

/* I2C to LCD
   PB8 - SCK
   PB9 - SDA */

/* SPI1 to ADXL345
   PA4 - CS
   PA5 - SCK
   PA6 - MISO
   PA7 - MOSI
   PA8 - INT1 (not implemented)
   PA9 - INT2 (not implemented) */

/* REFER TO THE "OFFSET CALIBRATION" SECTION IN THE ADXL345 DATASHEET FOR A BETTER UNDERSTANDING OF
   THE CALIBRATION METHODS, IN THE CASE OF SOFTWARE CALIBRATION THE NECESSARY EQUATIONS ARE SHOWN THERE
   AS WELL AS THE OFFSET CALIBRATION BY WRITING TO THE OFFSET REGISTERS */

extern void initialise_monitor_handles();

/* - uncomment the following line if offset calibration for X, Y and Z axes will be performed by software.
   - leave the line commented if write to the offset registers is preferred.
   NOTE:  calculations are performed automatically and no extra action is needed. This is only for debugging purposes
   NOTE2: after testing, results obtained with offset registers are not accurate. Calibration by software is recommended */
#define CALIB_BY_SOFTWARE

int16_t x0g, y0g, z0g; /* values measured at 0g for all axes (this will be used for offset calibration regardless
                          the method used) */
int16_t xmeas, ymeas, zmeas; /* raw data read from X0, X1, Y0, Y1, Z0 and Z1 registers */

#ifdef CALIB_BY_SOFTWARE
/* variables used for offset calibration by software */
int16_t xactual, yactual, zactual; /* data read from X, Y and Z registers after offset being applied */
#else
/* variables used for offset calibration by writing to the offset registers */
uint8_t offset[ 3 ]; /* values to be written to the offset registers, X, Y and Z respectively */
#endif

//float xscal, yscal, zscal;          /* scaled values according to selected g range */
//float xmax = 0, ymax = 0, zmax = 0; /* debug variables used to know if max range of accelerometer is correct */
//float xmin = 0, ymin = 0, zmin = 0; /* debug variables used to know if min range of accelerometer is correct */

/* using ints instead of floats for now, due to semihosting's printf does not print floating point values (ST linker script alignment issue?) */
int16_t xscal, yscal, zscal;          /* scaled values according to selected g range */
int16_t xmax = 0, ymax = 0, zmax = 0; /* debug variables used to know if max range of accelerometer is correct */
int16_t xmin = 0, ymin = 0, zmin = 0; /* debug variables used to know if min range of accelerometer is correct */

int main( void )
{  
   uint8_t spi1_write[ 2 ]; /* SPI data to be written to the ADXL345 */
   uint8_t spi1_read;       /* SPI data read from ADXL345 */
   
   uint8_t read_axes[ 6 ];  /* Values read from X0, X1, Y0, Y1, Z0 and Z1 registers respectively */

   initialise_monitor_handles();

   /* initialize Systick */
   SysTick_Init();

   /* initialize SPI1 peripheral */
   SPI1_Init();

   /* read from DEVID (0x00), this should return the device ID 0xE5 */
   spi1_write[ 0 ] = 0x80U; /* 1 = read + 0 = no multiple bit + (0x00) 6 bits devid register */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 1U );
   SPI1_Read( &spi1_read, 1U );
   SPI1_CS_Disable();

   /* write to DATA_FORMAT (0x31) */
   spi1_write[ 0 ] = 0x31U; /* 0 = write + 0 = no multiple bit + (0x31) 6 bits data_format register */
   spi1_write[ 1 ] = 0x00U; /* data to write: NO SELF TEST, SPI 4-Wire, ints active high, right-justified, 2g range */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 2U );
   SPI1_CS_Disable();

   /* read from DATA_FORMAT (0x31) in order to verify if previous data was correctly written */
   spi1_write[ 0 ] = 0xB1U; /* 1 = read + 0 = no multiple bit + (0x31) 6 bits data_format register */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 1U );
   SPI1_Read( &spi1_read, 1U );
   SPI1_CS_Disable();

   /* set ADXL345 to normal mode by setting measurement bit in POWER_CTL (0x2D) */
   spi1_write[ 0 ] = 0x2DU; /* 0 = write + 0 = no multiple bit + (0x2D) 6 bits power_ctl register */
   spi1_write[ 1 ] = 0x08U; /* data to write: link = 0, no auto sleep-mode, measurement mode, sleep = 0 */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 2U );
   SPI1_CS_Disable();

   /* read from POWER_CTL (0x2D) in order to verify if previous data was correctly written */
   spi1_write[ 0 ] = 0xADU; /* 1 = read + 0 = no multiple bit + (0x2D) 6 bits power_ctl register */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 1U );
   SPI1_Read( &spi1_read, 1U );
   SPI1_CS_Disable();

   /* ************************************ ADXL345 CALIBRATION ************************************************** */

   /* Read from DATAX0 (0x32), DATAX1 (0x33), DATAY0 (0x34), DATAY1 (0x35) in multiple-byte mode
      If a single-byte read operation is performed, the remaining bytes of data for the current FIFO 
      sample are lost. Therefore, all axes of interest should be read in a burst (or multiple-byte) read operation. */
   spi1_write[ 0 ] = 0xF2U; /* 1 = read + 1 = multiple bit + (0x32) 6 bits DATAX0 register */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 1U );
   SPI1_Read( read_axes, 6U ); /* get measured values for X, Y and Z axes in 0g, 0g and +1g respectively */
   SPI1_CS_Disable();

   /* copy read data (X0g, Y0g and Z+1g) into variables */
   x0g = ( read_axes[ 1 ] << 8 ) + read_axes[ 0 ]; /* data read from X registers */
   y0g = ( read_axes[ 3 ] << 8 ) + read_axes[ 2 ]; /* data read from Y registers */
   z0g = ( ( read_axes[ 5 ] << 8 ) + read_axes[ 4 ] ) - 256; /* data read from Z registers minus ideal value when 1g applied in 2g range mode.
                                                                256 is the typ. sensitivity in 2g range mode, substraction is needed due to
                                                                Z axis is not being measured at 0g but at +1g instead (unlike X and Y axes) */
      
   #ifndef CALIB_BY_SOFTWARE
   /* get offset values for each axis to be written into the offset registers */
   offset[ 0 ] = ( uint8_t ) ( -1 * round( x0g / ( 15.6 / 3.9 ) ) ); /* 15.6 is the scale factor of the offset reg, 3.9 is the typ. scale factor for 2g range */
   offset[ 1 ] = ( uint8_t ) ( -1 * round( y0g / ( 15.6 / 3.9 ) ) ); /* 15.6 is the scale factor of the offset reg, 3.9 is the typ. scale factor for 2g range */
   offset[ 2 ] = ( uint8_t ) ( -1 * round( z0g / ( 15.6 / 3.9 ) ) ); /* 15.6 is the scale factor of the offset reg, 3.9 is the typ. scale factor for 2g range */
   
   /* write offsets to OFSX (0x1E), OFSY (0x1F) and OFSZ (0x20) in multi-byte mode */
   spi1_write[ 0 ] = 0x5EU; /* 0 = write + 1 = multiple bit + (0x1E) 6 bits ofsx register */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 1U ); /* request write to OFSX in multiple-byte mode, a new write will shift to the next registers (OFSY and OFSZ) */
   SPI1_Write( offset, 3U );     /* each byte will be written to OFSX, OFSY and OFSZ registers respectively */
   SPI1_CS_Disable();

   /* read from OFSX (0x1E), OFSY (0x1F) and OFSZ (0x20) in multiple byte mode in order to verify if previous data was correctly written */
   spi1_write[ 0 ] = 0xDEU; /* 1 = read + 1 = multiple bit + (0x1E) 6 bits ofsx register */
   SPI1_CS_Enable();
   SPI1_Write( spi1_write, 1U );
   SPI1_Read( read_axes, 3U );
   SPI1_CS_Disable();
   #endif

   /* ****************************************** LCD ************************************************* */
   LCD_HandleTypeDef LCD_Handler;

   /* initialize LCD module */
   LCD_Handler.lcdaddress     = LCD_ADDRESS;
   LCD_Handler.backlight      = BACKLIGHT_ENABLED;
   LCD_Handler.datalenght     = INTERFACE_DATA_4_BIT;
   LCD_Handler.displayline    = DISPLAY_2_LINES;
   LCD_Handler.font           = DISPLAY_FONT_TYPE_5x8;
   LCD_Handler.display        = DISPLAY_ON;
   LCD_Handler.cursor         = CURSOR_OFF;
   LCD_Handler.blink          = CURSOR_BLINK_OFF;
   LCD_Handler.incrementddram = ID_DDRAM_ADDRESS_RIGHT;
   LCD_Handler.shiftdisplay   = SHIFT_DISPLAY_OFF;

   /* initialize LCD module */
   LCD_Init( &LCD_Handler ); /******** NOTE: TIM3 delay function not working properly (longer) due to uC is not set at 48MHz.
                                             This does not affect the LCD operation at all *********/

   while( 1 )
   {  
      /* Read from DATAX0 (0x32), DATAX1 (0x33), DATAY0 (0x34), DATAY1 (0x35) in multiple-byte mode
         If a single-byte read operation is performed, the remaining bytes of data for the current FIFO 
         sample are lost. Therefore, all axes of interest should be read in a burst (or multiple-byte) read operation. */
      spi1_write[ 0 ] = 0xF2U; /* 1 = read + 1 = multiple bit + (0x32) 6 bits DATAX0 register */
      SPI1_CS_Enable();
      SPI1_Write( spi1_write, 1U );
      SPI1_Read( read_axes, 6U );
      SPI1_CS_Disable();
            
      /* copy current data read into variables (if calibration is done by software ...
         these readings have error due to offset not being applied yet. If offset registers are used then this data should be correct) */
      xmeas = ( read_axes[ 1 ] << 8 ) + read_axes[ 0 ];
      ymeas = ( read_axes[ 3 ] << 8 ) + read_axes[ 2 ];
      zmeas = ( read_axes[ 5 ] << 8 ) + read_axes[ 4 ];
            
      #ifdef CALIB_BY_SOFTWARE
      /* apply offset by software to previous data read (this lecture should be correct after offset being applied) */
      xactual = xmeas - x0g;
      yactual = ymeas - y0g;
      zactual = zmeas - z0g;

      /* scale data for 2g range (see range bits in DATA_FORMAT), this gives us the data in mg */
      //xscal = xactual * 0.0039; /* 0.0039 is the typ. scale factor for the 2g range */
      //yscal = yactual * 0.0039; /* 0.0039 is the typ. scale factor for the 2g range */
      //zscal = zactual * 0.0039; /* 0.0039 is the typ. scale factor for the 2g range */
      /* using ints instead of floats for now, due to printf does not print floating point values (ST linker script alignment issue?) */
      xscal = xactual * 39;
      yscal = yactual * 39;
      zscal = zactual * 39;
      #else
      /* scale data for 2g range (see range bits in DATA_FORMAT), this gives us the data in mg */
      //xscal = xmeas * 0.0039; /* 0.0039 is the typ. scale factor for the 2g range */
      //yscal = ymeas * 0.0039; /* 0.0039 is the typ. scale factor for the 2g range */
      //zscal = zmeas * 0.0039; /* 0.0039 is the typ. scale factor for the 2g range */
      /* using ints instead of floats for now, due to printf does not print floating point values (ST linker script alignment issue?) */
      xscal = xmeas * 39;
      yscal = ymeas * 39;
      zscal = zmeas * 39;
      #endif

      /* store min and max values for each axis (this helps to verify if the range of the data is OK) */
      if ( xscal > xmax )
      {
         xmax = xscal;
      }

      if ( xscal < xmin )
      {
         xmin = xscal;
      }

      if ( yscal > ymax )
      {
         ymax = yscal;
      }

      if ( yscal < ymin )
      {
         ymin = yscal;
      }

      if ( zscal > zmax )
      {
         zmax = zscal;
      }

      if ( zscal < zmin )
      {
         zmin = zscal;
      }

      /* NOTE: under normal conditions (i.e. acceleration applied does not exceed the 1g gravity force) 
         the max and minimum values for X, Y and Z are within +/- 1g */

      /* debug purposes */
      printf( "Xact = %d\n", xscal );
      printf( "Yact = %d\n", yscal );
      printf( "Zact = %d\n\n", zscal );

      /* print data from the 3 axis on the LCD.
         - 39 is the value used for scaling each axis data (a few lines above it's mentioned a problem in the usage of the float data
           type in relation to printf and semihosting)
         - Actually the data for the 2g range has to be scaled by a factor of 0.0039 mg (that's why float was used at first)
         - Then, lcd_print_all_axes_info receives the floating values for each axis
         - Because int data type is used (for semihosting purposes) instead of floating (real data interpretation), the correct float
           data is obtained by dividing and multiplying accordingly as originally intended */
      lcd_print_all_axes_info( &LCD_Handler, ( xscal / 39.0 ) * 0.0039, ( yscal / 39.0 ) * 0.0039, ( zscal / 39.0 ) * 0.0039 );

      /* wait 100 ms */
      Systick_Delay_Ms( 100 );
   }

   return 0U;
}

uint8_t *uint16_t_to_string( uint16_t value, uint8_t *dest_str )
{
   uint8_t item = 0U;
    
   /* store the '0' character when decimal value is 0 */
   if ( value == 0U )
   {
      dest_str[ item ] = '0';
        
      item++;
   }
    
   /* split the digits of the integer and store each into the string.
      NOTE: string is stored backwards */
   while ( value > 0U )
   {
      dest_str[ item ] = ( value % 10 ) + '0';
      value /= 10U;
        
      item++;
   }

   /* add termination null character '\0' */
   dest_str[ item ] = '\0';
    
   ( void ) reverse_string( dest_str );
    
   return dest_str;
}

uint8_t *reverse_string( uint8_t *src_string )
{
   uint8_t start = 0U;
   uint8_t end;
   uint8_t temp;
    
   /* find the end of the string */
   for ( end = start; src_string[ end ] != '\0'; end++ )
   {
      /* do nothing */ 
   }
    
   end--;
    
   /* switch start and end elements successively */
   while ( start < end )
   {
      temp = src_string[ start ];
        
      src_string[ start ] = src_string[ end ];
      src_string[ end ] = temp;
        
      start++;
      end--;
   }
    
   return src_string;
}

void lcd_print_one_axis_info( LCD_HandleTypeDef *hlcd, float axis )
{
   uint8_t str[ 4 ];
   int8_t integer;
   int16_t fractional;
   uint8_t extradigs = 3U;

   /* get the integer part of the x axis data */
   integer = ( int8_t ) axis;

   /* if x axis value is negative */
   if ( axis < 0 )
   {
      /* convert the integer part of the x axis data to its unsiged form 
         by reverting 2's and 1's complement */
      integer = ~( integer - 1 );

      /* print negative sign */
      LCD_API_Print_Character( hlcd, '-' );
   }
   /* if x axis value is negative */
   else
   {
      /* print positive sign */
      LCD_API_Print_Character( hlcd, '+' );
   }

   /* convert the integer part of the x axis into a string */
   ( void ) uint16_t_to_string( integer, str );

   /* if integer part has only 1 digit */
   if ( ( integer / 10 ) < 1 )
   {
      /* append a '0' character to the left */
      LCD_API_Print_Character( hlcd, '0' );
   }

   /* print the integer part of the x axis */
   LCD_API_Print_String( hlcd, str, 2 );

   /* print decimal point character */
   LCD_API_Print_Character( hlcd, '.' );

   /* get the fractional part of the x axis data by subtracting the integer portion */
   axis = ( axis >= 0 ) ? ( axis - integer ) : ( axis + integer );

   /* convert the fractional part of the x axis data to unsigned integer format,
      4 digits to the right of the decimal point are obtained when multiplying by 10000 */
   fractional = ( axis >= 0 ) ? ( axis * 10000 ) : ( axis * -10000 );

   /* obtain the number of digits in the fractional portion, this indicates the number
      of zeros that must be introduced to the right of the decimal point */
   if ( ( fractional / 10 ) < 1 )
   {
      LCD_API_Print_String( hlcd, ( uint8_t* ) "000", 3 );

      extradigs = 0U;
   }
   else if ( ( fractional / 100 ) < 1 )
   {
      LCD_API_Print_String( hlcd, ( uint8_t* ) "00", 2 );

      /* indicate number of extra digits to print to the right of the decimal point,
         this does not include the 2 previous zeros appended, i.e: 3 digs in total */
      extradigs = 1U;
   }
   else if ( ( fractional / 1000 ) < 1 )
   {
      LCD_API_Print_Character( hlcd, '0' );
      
      /* indicate number of extra digits to print to the right of the decimal point,
         this does not include the previous zero appended, i.e: 3 digs in total */
      extradigs = 2U;
   }

   /* convert the fractional part to a string */
   ( void ) uint16_t_to_string( fractional, str );

   /* print the fractional part */
   LCD_API_Print_String( hlcd, str, extradigs );
}

void lcd_print_all_axes_info( LCD_HandleTypeDef *hlcd, float x, float y, float z )
{
   /* move cursor to first row and first column on the LCD */
   LCD_API_Move_Cursor( hlcd, 0U, 0U );

   /* print x axis data */
   lcd_print_one_axis_info( hlcd, x );

   /* print 'X' character to indicate previous data belongs to the x axis  */
   LCD_API_Print_Character( hlcd, 'X' );

   /* move cursor to first row and ninth column on the LCD */
   LCD_API_Move_Cursor( hlcd, 0U, 8U );

   /* print y axis data */
   lcd_print_one_axis_info( hlcd, y );

   /* print 'Y' character to indicate previous data belongs to the x axis  */
   LCD_API_Print_Character( hlcd, 'Y' );

   /* move cursor to second row and fifth column on the LCD */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );

   /* print z axis data */
   lcd_print_one_axis_info( hlcd, z );

   /* print 'Z' character to indicate previous data belongs to the x axis  */
   LCD_API_Print_Character( hlcd, 'Z' );
}
