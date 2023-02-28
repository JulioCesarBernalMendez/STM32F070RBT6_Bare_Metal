#include "exti.h"

void ADC1_Interrupt_Init( void )
{
   /* disable global interrupts */
   __disable_irq();

   /* enable ADC1 end of conversion interrupt */
   ADC1->IER |= ADC_IER_EOCIE;

   /* enable interrupt 12 (ADC1 global interrupt) */
   NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_12;

   /* set ADC1 interrupt (N=12) priority to zero (highest priority).
      ISPRx. With x = N / 4 = 12 / 4, then x = 3. Therefore ISPRx = ISPR3.
      Byte offset = N MOD 4 = 12 MOD 4, therefore Byte offset = 0.
      NOTE: processor implements only bits 7 and 6 of each field. */
   NVIC->IP[ 3 ] &= ~NVIC_IPR_PRI12_7;
   NVIC->IP[ 3 ] &= ~NVIC_IPR_PRI12_6;

   /* enable global interrupts */
    __enable_irq();
}
