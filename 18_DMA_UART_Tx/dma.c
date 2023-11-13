#include "dma.h"

void DMA_USART2_Tx_Init( uint32_t mem_src, uint32_t periph_dest, uint16_t items )
{
    /* enable access to DMA clock */
    RCC->AHBENR |= RCC_AHBENR_DMAEN;

    /* NOTE: data will be transferred from MEMORY (a character array)
             to PERIPHERAL (UART transmit data register) */

    /* set SOURCE buffer address (memory address) */
    DMA1_Channel4->CMAR = mem_src;

    /* set DESTINATION buffer address (peripheral address) */
    DMA1_Channel4->CPAR = periph_dest;

    /* set DMA CH 4 (USART2_TX) memory size (source) to 8 bits */
    DMA1_Channel4->CCR &= ~DMA_CCR_MSIZE_1;
    DMA1_Channel4->CCR &= ~DMA_CCR_MSIZE_0;

    /* set DMA CH 4 (USART2_TX) peripheral size (destination) to 8 bits */
    DMA1_Channel4->CCR &= ~DMA_CCR_PSIZE_1;
    DMA1_Channel4->CCR &= ~DMA_CCR_PSIZE_0;

    /* set number of data items to transfer from memory to peripheral */
    DMA1_Channel4->CNDTR = items;

    /* enable DMA CH4 (USART2_TX) memory address increment mode */
    DMA1_Channel4->CCR |= DMA_CCR_MINC;

    /* disable DMA CH4 (USART2_TX) peripheral address increment mode (fixed address) */
    DMA1_Channel4->CCR &= ~DMA_CCR_PINC;

    /* enable DMA CH4 (USART2_TX) circular mode */
    DMA1_Channel4->CCR |= DMA_CCR_CIRC;

    /* disable DMA CH4 (USART2_TX) memory to memory mode */
    DMA1_Channel4->CCR &= ~DMA_CCR_MEM2MEM;

    /* set DMA CH4 (USART2_TX) transfer direction: read from memory ...
       therefore read from memory and write to peripheral */
    DMA1_Channel4->CCR |= DMA_CCR_DIR;

    /* set DMA CH 4 (USART2_TX) to highest priority */
    DMA1_Channel4->CCR |= DMA_CCR_PL_1;
    DMA1_Channel4->CCR |= DMA_CCR_PL_0;

    /* enable DMA CH4 transfer complete interrupt */
    DMA_USART2_Tx_Interrupt_Init();

    /* enable DMA CH4 transmission mode on USART2 */
    USART2->CR3 |= USART_CR3_DMAT;

    /* enable DMA CH 4 (USART2_TX) */
    DMA1_Channel4->CCR |= DMA_CCR_EN;
}

void DMA_USART2_Tx_Interrupt_Init( void )
{
    /* disable global interrupts */
    __disable_irq();

    /* enable DMA CH4 (USART2_TX) transfer complete interrupt */
    DMA1_Channel4->CCR |= DMA_CCR_TCIE;

    /* enable interrupt 11 (DMA CH4 interrupt).
       Another method to do this is by using:
       NVIC_EnableIRQ( DMA1_Channel4_5_IRQn ) */
    NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_11;

    /* set DMA CH4 interrupt (N=11) priority to 64 (second highest priority).
       ISPRx. With x = N / 4 = 11 / 4, then 2 = 7. Therefore ISPRx = ISPR2.
       Byte offset = N MOD 4 = 11 MOD 4, therefore Byte offset = 3.
       NOTE: processor implements only bits 7 and 6 of each field. */
    NVIC->IP[ 2 ] &= ~NVIC_IPR_PRI11_7;
    NVIC->IP[ 2 ] &= ~NVIC_IPR_PRI11_6;

    /* enable global interrupts */
    __enable_irq();
}
