#include "interrupts.h"

void DMA1_Channel4_5_IRQHandler( void )
{
    /* check if DMA CH4 (USART2_TX) transfer complete triggered the interrupt */
    if ( ( DMA1->ISR & DMA_ISR_TCIF4 ) == DMA_ISR_TCIF4 )
    {   
        /* clear DMA CH4 (USART2_TX) transfer complete flag */
        DMA1->IFCR |= DMA_IFCR_CTCIF4;

        /* execute DMA CH4 transfer complete callback */
        DMA_USART2_Tx_Complete_Callback();
    }
}

void DMA_USART2_Tx_Complete_Callback( void )
{
    /* toggle board led */
    toggle_led();
}
