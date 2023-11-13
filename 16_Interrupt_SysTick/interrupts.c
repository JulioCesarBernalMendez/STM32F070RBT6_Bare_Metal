#include "interrupts.h"

void SysTick_Handler( void )
{
    /* execute SysTick callback */
    SysTick_Callback();
}

void SysTick_Callback( void )
{   
    /* every time the SysTick exception occurs (1ms)
       the SysTick counter will be increased,
       this counter can be used by the user as a timebase
       to execute periodic tasks */
    tick++;
}
