#include "main.h"

int main( void )
{  
   /* initialize LCD module */
   LCD_Init( LCD_ADDRESS );

   /* move cursor to first row, third column */
   LCD_API_Move_Cursor( LCD_ADDRESS, 0U, 2U );
   LCD_API_Print_String( LCD_ADDRESS, ( uint8_t* ) "LCD DRIVER", 10U );

   /* move cursor to second row, sixth column */
   LCD_API_Move_Cursor( LCD_ADDRESS, 1U, 5U );
   LCD_API_Print_String( LCD_ADDRESS, ( uint8_t* ) "TEST", 4U );

   while( 1 )
   {  
      /* shift text on the LCD left and right every 800 ms */
      LCD_API_Shift_Screen_Left( LCD_ADDRESS, 2U, 800U );
      LCD_API_Shift_Screen_Right( LCD_ADDRESS, 6U, 800U );
      LCD_API_Shift_Screen_Left( LCD_ADDRESS, 4U, 800U );
   }

   return 0;
}
