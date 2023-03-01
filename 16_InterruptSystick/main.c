#include "main.h"

int main( void )
{   
    uint32_t ledTicks = 0;

    /* enable GPIOA5 (BOARD LED) as
       digital output */
    GPIOA_Init();

    /* enable:
       - SysTick timer
       - SysTick count down to zero exception */
    SysTick_Init();

    /* initialize ledTicks counter */
    ledTicks = Get_SysTick();

    while( 1 )
    {   
        /* if 500 miliseconds have passed */
        if ( ( Get_SysTick() - ledTicks ) >= 500 )
        {   
            /* toggle board led */
            Toggle_Led();

            /* update ledTicks counter */
            ledTicks = Get_SysTick();
        }
    }

    return 0;
}

void Toggle_Led( void )
{   
    GPIOA->ODR ^= LED_PIN;
}
