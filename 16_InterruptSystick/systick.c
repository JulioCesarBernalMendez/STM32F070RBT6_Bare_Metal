#include "systick.h"

uint32_t tick = 0;

void SysTick_Init( void )
{
    /* Processor clock (HCLK = 8MHz) as SysTick clocksource */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;

    /* SysTick exception request disabled */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

    /* SysTick reload value set to 1ms.
       RELOAD = 1ms / ( 1 / 8MHz ) */
    SysTick->LOAD = 0x1F40U;

    /* enable SysTick exception request */
    SysTick_Interrupt_Init();

    /* SysTick current value set to 0 */
    SysTick->VAL = 0x0U;

    /* Enable SysTick */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint32_t Get_SysTick( void )
{
    return tick;
}
