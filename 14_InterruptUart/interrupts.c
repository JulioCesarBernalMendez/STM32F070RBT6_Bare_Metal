#include "interrupts.h"

void USART2_IRQHandler( void )
{   
    /* if read data register is not empty
       (data is received and ready to be read) */
    if ( ( USART2->ISR & USART_ISR_RXNE ) == USART_ISR_RXNE )
    {   
        /* clear received data flag */
        USART2->RQR |= USART_RQR_RXFRQ;

        /* execute USART2 RX callback */
        USART2_Rx_Ready_Callback();
    }

    /* if transmission is complete */
    if ( ( USART2->ISR & USART_ISR_TC ) == USART_ISR_TC )
    {   
        /* clear transmission complete flag */
        USART2->ICR |= USART_ICR_TCCF;

        /* execute USART2 TX callback */
        USART2_Tx_Complete_Callback();
    }
}

void USART2_Rx_Ready_Callback( void )
{   
    char message[ 47 ];

    /* store received data on UART2 (from PC to Board) */
    char data = ( char ) USART2->RDR;

    /* print received data over UART2 (from Board to PC) */
    sprintf( message,  "Data ready to be read. Data received is: '%c'\n\r", data );
    printf( message );
}

void USART2_Tx_Complete_Callback( void )
{   
    /* acknowledge complete transmission (from Board to PC) over UART2 */
    printf( "Transmission complete!!!\n" );
}
