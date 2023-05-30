#include "main.h"

/* SPI1 Pins:
   PA4 - CS
   PA5 - SCK
   PA6 - MISO
   PA7 - MOSI */

int main( void )
{  
   uint8_t data[ 6 ] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F, '\0' };

   /* initialize SPI1 peripheral */
   SPI1_Init();

   /* send data array through SPI1 */
   SPI1_Write( data, 6U );

   while( 1 )
   {  
      /* do nothing */
   }

   return 0U;
}
