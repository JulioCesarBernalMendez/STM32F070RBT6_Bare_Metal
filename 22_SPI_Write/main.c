#include "main.h"

/* SPI1 Pins:
   PA4 - CS
   PA5 - SCK
   PA6 - MISO
   PA7 - MOSI */

int main( void )
{  
   uint8_t data[ 6 ] = { 0x48U, 0x65U, 0x6CU, 0x6CU, 0x6FU, '\0' };

   /* initialize SPI1 peripheral */
   SPI1_Init();

   /* send data array through SPI1 */
   SPI1_CS_Enable();
   SPI1_Write( data, 6U );
   SPI1_CS_Disable();

   while( 1 )
   {  
      /* do nothing */
   }

   return 0U;
}
