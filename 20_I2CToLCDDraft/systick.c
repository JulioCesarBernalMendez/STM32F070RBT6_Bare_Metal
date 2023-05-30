#include "systick.h"

void SysTick_Init( void )
{
    /* Processor clock (HCLK = 8MHz) as SysTick clocksource */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;

    /* SysTick exception request disabled */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

    /* SysTick reload value set to 1ms.
       RELOAD = 1ms / ( 1 / 8MHz ) */
    SysTick->LOAD = 0x1F40U;

    /* Enable SysTick */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void Systick_Delay_Ms( uint32_t delay )
{
    uint32_t wait;

    /* SysTick current value set to 0 */
    SysTick->VAL = 0x0U;

    /* Enable SysTick */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    /* count 'delay' ms */
    for ( wait = 0; wait < delay; wait++ )
    {   
        /* wait for SysTick value to be 0 */
        while ( ( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) == 0U )
        {
            /* do nothing */
        }
    }

    /* Disable SysTick */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
