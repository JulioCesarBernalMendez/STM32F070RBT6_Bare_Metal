#include "main.h"

int main( void )
{   
    uint8_t data[ 6 ] = "Hello";

    /* inititalize I2C1 */
    I2C1_Init();

    /* send data over I2C1 */
    I2C1_Write( SLAVE_ADDRESS, data, sizeof( data ) );

    while( 1 )
    {   
        /* do nothing */
    }

    return 0;
}
