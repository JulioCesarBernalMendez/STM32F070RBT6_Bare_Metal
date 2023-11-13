#include "main.h"

int main( void )
{   
    uint8_t dma_msg[ 48 ] = "This a message from DMA transferred to USART2\n\r";

    /* initialize UART2 peripheral in TX mode */
    USART2_Init();

    /* enable:
       - DMA CH4 (USART2_TX) in memory (array) to peripheral (USART2) transfer mode
       - DMA CH4 transfer complete interrupt */
    DMA_USART2_Tx_Init( ( uint32_t ) dma_msg, ( uint32_t ) ( &( USART2->TDR ) ), sizeof( dma_msg ) );

    /* initialize GPIOA5 (BOARD LED) as output */
    GPIOA_Init(); 
    
    while( 1 )
    {
        /* do nothing,
           NOTE: DMA is enabled in circular mode, therefore the USART
                 transmission is issued automatically one after another */
    }

    return 0;
}

void toggle_led( void )
{
    GPIOA->ODR ^= LED_PIN;
}
