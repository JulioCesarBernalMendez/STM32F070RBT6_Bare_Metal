#include "exti.h"

void SysTick_Interrupt_Init( void )
{
   /* enable SysTick exception request
      (counting down to zero asserts the exception request) */
   SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}
