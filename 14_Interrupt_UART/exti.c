#include "exti.h"

void USART2_Interrupt_Init( void )
{
   /* disable global interrupts */
   __disable_irq();

   /* enable USART2 Receive Data Register
      not empty interrupt (data ready to be read) */
   USART2->CR1 |= USART_CR1_RXNEIE;

   /* enable USART2 Tranmision Complete interrupt */
   USART2->CR1 |= USART_CR1_TCIE;

   /* enable interrupt 28 (USART global interrupt).
      Another method to do this is by using:
    NVIC_EnableIRQ( USART2_IRQn ) */
   NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_28;

   /* set USART2 interrupt (N=28) priority to zero (highest priority).
      ISPRx. With x = N / 4 = 28 / 4, then x = 7. Therefore ISPRx = ISPR7.
      Byte offset = N MOD 4 = 28 MOD 4, therefore Byte offset = 0.
      NOTE: processor implements only bits 7 and 6 of each field. */
   NVIC->IP[ 7 ] &= ~NVIC_IPR_PRI28_7;
   NVIC->IP[ 7 ] &= ~NVIC_IPR_PRI28_6;

   /* clear transmission complete flag,
      this is a workaround due to the transmission complete interrupt
      is triggered upon initialization for unknown reasons */
   USART2->ICR |= USART_ICR_TCCF;

   /* enable global interrupts */
    __enable_irq();
}
