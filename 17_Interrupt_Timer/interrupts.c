#include "interrupts.h"

void TIM3_IRQHandler( void )
{   
    /* if TIM3 update event (overflow) has occurred */
    if ( ( TIM3->SR & TIM_SR_UIF ) == TIM_SR_UIF )
    {   
        /* clear TIM3 update event flag */
        TIM3->SR &= ~TIM_SR_UIF;

        /* execute TIM3 callback */
        TIM3_Callback();

        /* clear TIM3 counter */
        //TIM3->CNT = 0x0U;
    }
}

void TIM3_Callback( void )
{   
    /* toggle GPIOA5 (BOARD LED) */
    toggle_led();
}

void toggle_led( void )
{
    GPIOA->ODR ^= LED_PIN;
}
