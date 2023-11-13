#include "usart.h"

void USART2_Init( void )
{
    /* enable GPIOA (GPIOA2 = USART2TX, GPIOA3 = USART2RX) clock access */
    GPIOA_CLK_ENBL();

    /* enable USART2 clock access */
    USART2_CLK_ENBL();

    /* GPIOA2 as alternate function */
    GPIOA->MODER |=  GPIO_MODER_MODER2_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER2_0;

    /* GPIOA2 alternate function 1 (USART2 Tx) */
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL2_3;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL2_2;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL2_1;
    GPIOA->AFR[ 0 ] |=  GPIO_AFRL_AFSEL2_0;

    /* GPIOA2 with pull-up resistor */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR2_1;
    GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR2_0;

    /* GPIOA3 as alternate function */
    GPIOA->MODER |=  GPIO_MODER_MODER3_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER3_0; 

    /* USART2 1 start bit and 8 data bits */
    USART2->CR1 &= ~USART_CR1_M1;
    USART2->CR1 &= ~USART_CR1_M0;

    /* USART2 1 stop bit */
    USART2->CR2 &= ~USART_CR2_STOP_1;
    USART2->CR2 &= ~USART_CR2_STOP_0;

    /* USART2 no parity control */
    USART2->CR1 &= ~USART_CR1_PCE;

    /* USART2 oversampling by 16 */
    USART2->CR1 &= ~USART_CR1_OVER8;

    /* USART2 least significant bit first */
    USART2->CR2 &= ~USART_CR2_MSBFIRST;

    /* USART2 9600 baud rate.
       When oversampling by 16 (OVER8 = 1):
       BRR = USARTDIV, with USARTDIV = fck / baud.
       Therefore USARTDIV = 8MHz (default value upon init.) / 9600 */
    USART2->BRR = GET_USARTDIV();

    /* USART2 transmitter enabled */
    USART2->CR1 |= USART_CR1_TE;

    /* USART2 enabled */
    USART2->CR1 |= USART_CR1_UE;
}
