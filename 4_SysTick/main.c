#include <stdint.h>
#include "stm32f0xx.h"
#include "main.h"
#include "gpio.h"
#include "systick.h"

int main( void )
{
    GPIOA_Init();
    SysTick_Init();

    while( 1 )
    {   
        Toggle_Led();
        Systick_Delay_Ms( 1000 );
    }

    return 0;
}

void Toggle_Led( void )
{
    GPIOA->ODR ^= LED_PIN;
}
