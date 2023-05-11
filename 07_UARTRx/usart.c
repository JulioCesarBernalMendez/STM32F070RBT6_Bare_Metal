#include "stm32f0xx.h"
#include "usart.h"

void USART2_Init( void )
{
    GPIOA_CLK_ENBL();
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

    /* GPIOA3 alternate function 1 (USART2 Rx) */
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL3_3;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL3_2;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL3_1;
    GPIOA->AFR[ 0 ] |=  GPIO_AFRL_AFSEL3_0;

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

    /* USART2 transmitter and receiver enabled */
    USART2->CR1 |= ( USART_CR1_TE | USART_CR1_RE );

    /* USART2 enabled */
    USART2->CR1 |= USART_CR1_UE;
}


void USART2_Transmit_Character( uint8_t data )
{   
    /* wait for the transmit data register to be empty */
    while ( !( USART2->ISR & USART_ISR_TXE ) )
    {
        /* do nothing */
    }

    /* transmit data */
    USART2->TDR = data;
}

void USART2_Transmit_Array( const uint8_t *str )
{
    uint8_t i;

    /* loop through the character array */
    for ( i = 0; str[ i ] != '\0'; i++ )
    {   
        /* transmit one character at a time */
        USART2_Transmit_Character( str[ i ] );
    }
}

uint8_t USART2_Receive_Character( void )
{   
    /* wait for received data to be ready to be read */
    while ( !( USART2->ISR & USART_ISR_RXNE ) )
    {
        /* do nothing */
    }

    /* return received data */
    return ( USART2->RDR & 0xFF );
}