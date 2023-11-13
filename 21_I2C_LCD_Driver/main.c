#include "main.h"

int main( void )
{
   LCD_HandleTypeDef LCD_Handler;

   /* initialize LCD module */
   LCD_Handler.lcdaddress     = LCD_ADDRESS;
   LCD_Handler.backlight      = BACKLIGHT_ENABLED;
   LCD_Handler.datalenght     = INTERFACE_DATA_4_BIT;
   LCD_Handler.displayline    = DISPLAY_2_LINES;
   LCD_Handler.font           = DISPLAY_FONT_TYPE_5x8;
   LCD_Handler.display        = DISPLAY_ON;
   LCD_Handler.cursor         = CURSOR_ON;
   LCD_Handler.blink          = CURSOR_BLINK_ON;
   LCD_Handler.incrementddram = ID_DDRAM_ADDRESS_RIGHT;
   LCD_Handler.shiftdisplay   = SHIFT_DISPLAY_OFF;
   LCD_Init( &LCD_Handler );
   
   /* move cursor to first row, third column */
   LCD_API_Move_Cursor( &LCD_Handler, 0U, 2U );
   LCD_API_Print_String( &LCD_Handler, ( uint8_t* ) "LCD DRIVER", 10U );

   /* move cursor to second row, sixth column */
   LCD_API_Move_Cursor( &LCD_Handler, 1U, 5U );
   LCD_API_Print_String( &LCD_Handler, ( uint8_t* ) "TEST", 4U );

   while( 1 )
   {  
      /* shift text on the LCD left and right every 800 ms */
      LCD_API_Shift_Screen_Left( &LCD_Handler, 2U, 800U );
      LCD_API_Shift_Screen_Right( &LCD_Handler, 6U, 800U );
      LCD_API_Shift_Screen_Left( &LCD_Handler, 4U, 800U );
   }

   return 0;
}
