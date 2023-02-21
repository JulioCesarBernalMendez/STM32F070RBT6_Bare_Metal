#include "interrupts.h"

void EXTI4_15_IRQHandler( void )
{
    /* determine if EXTI13 has occurred */
    if ( ( EXTI->PR & EXTI_PR_PIF13 ) == EXTI_PR_PIF13 )
    {
        /* clear EXTI13 pending bit */
        EXTI->PR |= EXTI_PR_PIF13;

        /* execute EXTI4_15_Callback */
        EXTI4_15_Callback();
    }
}

void EXTI4_15_Callback( void )
{
    /* if BOARD BUTTON is pressed */
    if ( ( GPIOC->IDR & GPIO_IDR_13 ) == 0U )
    {
        /* turn on BOARD LED */
        GPIOA->ODR |= GPIO_ODR_5;
    }
    else
    {
        /* turn off BOARD LED */
        GPIOA->ODR &= ~GPIO_ODR_5;
    }
}
