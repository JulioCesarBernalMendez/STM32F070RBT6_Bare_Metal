#include <stdint.h>
#include <stdio.h>
#include "stm32f0xx.h"
#include "main.h"
#include "gpio.h"
#include "timer.h"

int main( void )
{
    GPIOA_Init();
    TIM3_Init();

    while( 1 )
    {   
        toggle_led();
        TIM3_Delay_1s( 1U );
    }

    return 0;
}

void toggle_led( void )
{
    GPIOA->ODR ^= LED_PIN;
}
