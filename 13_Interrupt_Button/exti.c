#include "exti.h"

void GPIOC_EXTI13_Init( void )
{
    /* disable global interrupts */
    __disable_irq();

    /* enable SYSCFG clock access */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* map EXTI13 to GPIOC */
    SYSCFG->EXTICR[ 3 ] &= ~SYSCFG_EXTICR4_EXTI13_2;
    SYSCFG->EXTICR[ 3 ] |=  SYSCFG_EXTICR4_EXTI13_1;
    SYSCFG->EXTICR[ 3 ] &= ~SYSCFG_EXTICR4_EXTI13_0;

    /* unmask/enable interrupt request on line 13 */
    EXTI->IMR |= EXTI_IMR_IM13;

    /* unmask/enable event request on line 13 */
    EXTI->EMR |= EXTI_EMR_EM13;

    /* rising edge trigger enabled on line 13 */
    EXTI->RTSR |= EXTI_RTSR_RT13;

    /* falling edge trigger enabled on line 13 */
    EXTI->FTSR |= EXTI_FTSR_FT13;

    /* enable interrupt 7 (EXTI4_15).
       Another method to do this is by using:
    NVIC_EnableIRQ( EXTI4_15_IRQn ) */
    NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_7;

    /* set interrupt EXTI4_15 (N=7) priority to zero (highest priority).
       ISPRx. With x = N / 4 = 7 / 4, then x = 1. Therefore ISPRx = ISPR1.
       Byte offset = N MOD 4 = 7 MOD 4, therefore Byte offset = 3.
       NOTE: processor implements only bits 7 and 6 of each field. */
    NVIC->IP[ 2 ] &= ~NVIC_IPR_PRI7_7;
    NVIC->IP[ 2 ] &= ~NVIC_IPR_PRI7_6;

    /* enable global interrupts */
    __enable_irq();
}
