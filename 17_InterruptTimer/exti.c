#include "exti.h"

void TIM3_Interrupt_Init( void )
{  
   /* disable global interrupts */
   __disable_irq();

   /* only TIM3 counter overflow/underflow generates interrupt */
   TIM3->CR1 |= TIM_CR1_URS;

   /* TIM3 update event interrupt enabled */
   TIM3->DIER |= TIM_DIER_UIE;

   /* enable TIM3 global interrupt */
   NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_16;

   /* set TIM3 interrupt (N=16) priority to zero (highest priority).
      ISPRx. With x = N / 4 = 16 / 4, then x = 4. Therefore ISPRx = ISPR4.
      Byte offset = N MOD 4 = 16 MOD 4, therefore Byte offset = 0.
      NOTE: processor implements only bits 7 and 6 of each field. */
   NVIC->IP[ 4 ] &= ~NVIC_IPR_PRI16_7;
   NVIC->IP[ 4 ] &= ~NVIC_IPR_PRI16_6;

   /* enable global interrupts */
    __enable_irq();
}
