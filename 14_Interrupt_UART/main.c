#include "main.h"

int main( void )
{   
    initialise_monitor_handles();

    /* initialize UART2 peripheral */
    USART2_Init();

    /* initialize SYSTICK timer */
    SysTick_Init();
    
    while( 1 )
    {
        /* NOTE: a UART2 RX interrupt can occur at any time */

        /* wait half a second */
        Systick_Delay_Ms( 500 );

        /* transmit over UART2, this will trigger the transmission complete interrupt */
        USART2_Transmit_Array( ( uint8_t * ) "This a message sent over UART2\n\r" );
    }

    return 0;
}
