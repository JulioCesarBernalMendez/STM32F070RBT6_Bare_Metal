#include "print_lcd.h"

uint8_t *uint16_t_to_string( uint16_t value, uint8_t *dest_str )
{
   uint8_t item = 0U;
    
   /* Store the '0' character when decimal value is 0 */
   if ( value == 0U )
   {
      dest_str[ item ] = '0';
        
      item++;
   }
    
   /* Split the digits of the integer and store each into the string.
      NOTE: string is stored backwards */
   while ( value > 0U )
   {
      dest_str[ item ] = ( value % 10 ) + '0';
      value /= 10U;

      item++;
   }

   /* Add termination null character '\0' */
   dest_str[ item ] = '\0';
    
   ( void ) reverse_string( dest_str );
    
   return dest_str;
}

uint8_t *reverse_string( uint8_t *src_string )
{
   uint8_t start = 0U;
   uint8_t end;
   uint8_t temp;
    
   /* Find the end of the string */
   for ( end = start; src_string[ end ] != '\0'; end++ )
   {
      /* Do nothing */ 
   }
    
   end--;
    
   /* Switch start and end elements successively */
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

   /* Get the integer part of the x axis data */
   integer = ( int8_t ) axis;

   /* If x axis value is negative */
   if ( axis < 0 )
   {
      /* Convert the integer part of the x axis data to its unsiged form 
         by reverting 2's and 1's complement */
      integer = ~( integer - 1 );

      /* Print negative sign */
      LCD_API_Print_Character( hlcd, '-' );
   }
   /* If x axis value is negative */
   else
   {
      /* Print positive sign */
      LCD_API_Print_Character( hlcd, '+' );
   }

   /* Convert the integer part of the x axis into a string */
   ( void ) uint16_t_to_string( integer, str );

   /* If integer part has only 1 digit */
   if ( ( integer / 10 ) < 1 )
   {
      /* Append a '0' character to the left */
      LCD_API_Print_Character( hlcd, '0' );
   }

   /* Print the integer part of the x axis */
   LCD_API_Print_String( hlcd, str, 2 );

   /* Print decimal point character */
   LCD_API_Print_Character( hlcd, '.' );

   /* Get the fractional part of the x axis data by subtracting the integer portion */
   axis = ( axis >= 0 ) ? ( axis - integer ) : ( axis + integer );

   /* Convert the fractional part of the x axis data to unsigned integer format,
      4 digits to the right of the decimal point are obtained when multiplying by 10000 */
   fractional = ( axis >= 0 ) ? ( axis * 10000 ) : ( axis * -10000 );

   /* Obtain the number of digits in the fractional portion, this indicates the number
      of zeros that must be introduced to the right of the decimal point */
   if ( ( fractional / 10 ) < 1 )
   {
      LCD_API_Print_String( hlcd, ( uint8_t* ) "000", 3 );

      extradigs = 0U;
   }
   else if ( ( fractional / 100 ) < 1 )
   {
      LCD_API_Print_String( hlcd, ( uint8_t* ) "00", 2 );

      /* Indicate number of extra digits to print to the right of the decimal point,
         this does not include the 2 previous zeros appended, i.e: 3 digs in total */
      extradigs = 1U;
   }
   else if ( ( fractional / 1000 ) < 1 )
   {
      LCD_API_Print_Character( hlcd, '0' );
      
      /* Indicate number of extra digits to print to the right of the decimal point,
         this does not include the previous zero appended, i.e: 3 digs in total */
      extradigs = 2U;
   }

   /* Convert the fractional part to a string */
   ( void ) uint16_t_to_string( fractional, str );

   /* Print the fractional part */
   LCD_API_Print_String( hlcd, str, extradigs );
}

void lcd_print_all_axes_info( LCD_HandleTypeDef *hlcd, float x, float y, float z )
{
   /* Move cursor to first row and first column on the LCD */
   LCD_API_Move_Cursor( hlcd, 0U, 0U );

   /* Print x axis data */
   lcd_print_one_axis_info( hlcd, x );

   /* Print 'X' character to indicate previous data belongs to the x axis  */
   LCD_API_Print_Character( hlcd, 'X' );

   /* Move cursor to first row and ninth column on the LCD */
   LCD_API_Move_Cursor( hlcd, 0U, 8U );

   /* Print y axis data */
   lcd_print_one_axis_info( hlcd, y );

   /* Print 'Y' character to indicate previous data belongs to the x axis  */
   LCD_API_Print_Character( hlcd, 'Y' );

   /* Move cursor to second row and fifth column on the LCD */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );

   /* Print z axis data */
   lcd_print_one_axis_info( hlcd, z );

   /* Print 'Z' character to indicate previous data belongs to the x axis. 
      NOTE: 2 extra spaces are for clearing possible axes printed due to inactivity event */
   LCD_API_Print_String( hlcd, ( uint8_t* ) "Z  ", 3U );
}

void lcd_print_single_tap( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t tap_source, float thresh_tap )
{

   /* Move cursor and print "SINGLE TAP >" */
   LCD_API_Move_Cursor( hlcd, 0U, 2U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "SINGLE TAP >", 12U );
	
   /* Print single tap threshold value */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   lcd_print_one_axis_info( hlcd, thresh_tap );
   
   /* Print 'X', 'Y' or 'Z' character depending on which axis generated the single tap */
   switch ( tap_source )
   {
      case TAP_X_SOURCE:
         LCD_API_Print_Character( hlcd, 'X' );
         break;

      case TAP_Y_SOURCE:
         LCD_API_Print_Character( hlcd, 'Y' );
         break;

      case TAP_Z_SOURCE:
         LCD_API_Print_Character( hlcd, 'Z' );
         break;

      default:
         /* Do nothing */
         break;
   }

   /* "Unprint" the +/- sign as single tap can be detected for both positive and negative magnitudes */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   LCD_API_Print_Character( hlcd, ' ' );
}

void lcd_print_double_tap( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t tap_source, float thresh_tap )
{
   /* Move cursor and print "SINGLE TAP >" */
   LCD_API_Move_Cursor( hlcd, 0U, 2U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "DOUBLE TAP >", 12U );
	
   /* Print double tap threshold value */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   lcd_print_one_axis_info( hlcd, thresh_tap );
   
   /* Print 'X', 'Y' or 'Z' character depending on which axis generated the single tap */
   switch ( tap_source )
   {
      case TAP_X_SOURCE:
         LCD_API_Print_Character( hlcd, 'X' );
         break;

      case TAP_Y_SOURCE:
         LCD_API_Print_Character( hlcd, 'Y' );
         break;

      case TAP_Z_SOURCE:
         LCD_API_Print_Character( hlcd, 'Z' );
         break;

      default:
         /* do nothing */
         break;
   }

   /* "Unprint" the +/- sign as single tap can be detected for both positive and negative magnitudes */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   LCD_API_Print_Character( hlcd, ' ' );
}

void lcd_print_activity( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t act_source, float thresh_act )
{
   /* Move cursor and print "ACTIVITY > " */
   LCD_API_Move_Cursor( hlcd, 0U, 3U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "ACTIVITY >", 10U );

   /* Print activity threshold */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   lcd_print_one_axis_info( hlcd, thresh_act );

   /* Print 'X', 'Y' or 'Z' character depending on the axis where activity was first detected */
   switch ( act_source )
   {
      case ACT_X_SOURCE:
         LCD_API_Print_Character( hlcd, 'X' );
         break;

      case ACT_Y_SOURCE:
         LCD_API_Print_Character( hlcd, 'Y' );
         break;

      case ACT_Z_SOURCE:
         LCD_API_Print_Character( hlcd, 'Z' );
         break;

      default:
         /* Do nothing */
         break;
   }

   /* "Unprint" the +/- sign as activity is detected regardless if it is positive or negative */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   LCD_API_Print_Character( hlcd, ' ' );
}

void lcd_print_inactivity( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t inact_axes, float thresh_inact )
{
   /* Move cursor and print "INACTIVITY <" */
   LCD_API_Move_Cursor( hlcd, 0U, 2U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "INACTIVITY <", 12U );

   /* Print inactivity threshold */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   lcd_print_one_axis_info( hlcd, thresh_inact );

   /* Print 'X', 'Y' or 'Z' character depending on the axis involved in the inactivity event */
   switch ( inact_axes )
   {
      case INACT_X_ENABLED:
         LCD_API_Print_Character( hlcd, ( uint8_t ) 'X' );
         break;

      case INACT_Y_ENABLED:
         LCD_API_Print_Character( hlcd, ( uint8_t ) 'Y' );
         break;

      case INACT_Z_ENABLED:
         LCD_API_Print_Character( hlcd, ( uint8_t ) 'Z' );
         break;

      default:
         /* Do nothing */
         break;
   }

   /* "Unprint" the +/- sign as inactivity is detected regardless if it is positive or negative */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   LCD_API_Print_Character( hlcd, ' ' );
}

void lcd_print_freefall( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, float thresh_ff )
{
   /* Clear screen */
   LCD_Send_Instruction( hlcd, CLEAR_DISPLAY_REG );

   /* Move cursor and print "FREE-FALL <" */
   LCD_API_Move_Cursor( hlcd, 0U, 2U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "FREE-FALL <", 11U );

   /* Print free-fall threshold */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   lcd_print_one_axis_info( hlcd, thresh_ff );

   /* "Unprint" the +/- sign as free-fall is detected regardless if it is positive or negative */
   LCD_API_Move_Cursor( hlcd, 1U, 4U );
   LCD_API_Print_Character( hlcd, ' ' );
}

void lcd_print_watermark( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t watermark_samples, uint8_t fifo_entries )
{
   /* Move cursor and print "WATERMARK >=" */
   LCD_API_Move_Cursor( hlcd, 0U, 1U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "WATERMARK >=", 12U );

   /* Print number of samples for the watermark to be triggered */
   LCD_API_Print_Character( hlcd, ' ' );
   LCD_API_Print_Character( hlcd, ( watermark_samples / 10U ) + '0' ); /* 1st digit (tens)  of samples in FIFO_CTL */
   LCD_API_Print_Character( hlcd, ( watermark_samples % 10U ) + '0' ); /* 2nd digit (units) of samples in FIFO_CTL */

   /* Print number of entries stored in FIFO */
   LCD_API_Move_Cursor( hlcd, 1U, 2U );
   LCD_API_Print_Character( hlcd, '(' );
   LCD_API_Print_Character( hlcd, ( fifo_entries / 10U ) + '0' ); /* 1st digit (tens)  of entries */
   LCD_API_Print_Character( hlcd, ( fifo_entries % 10U ) + '0' ); /* 2nd digit (units) of entries */
   LCD_API_Print_String( hlcd, ( uint8_t* ) " samples)", 9U );
}

void lcd_print_overrun( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel )
{  
   /* Move cursor and print "OVERRUN," */
   LCD_API_Move_Cursor( hlcd, 0U, 4U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "OVERRUN,", 7U );

   /* Move cursor and print "PRESS BUTTON" */
   LCD_API_Move_Cursor( hlcd, 1U, 2U );
   LCD_API_Print_String( hlcd, ( uint8_t* ) "PRESS BUTTON", 12U );
}
