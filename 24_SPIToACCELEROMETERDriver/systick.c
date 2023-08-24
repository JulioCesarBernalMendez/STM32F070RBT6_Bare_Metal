#include "systick.h"

/* Systick global variable updated every time the Systick exception takes place (1ms). Refer to interrupts.c */
uint32_t tick = 0U;

void SysTick_Init( void )
{
   /* Processor clock (HCLK = 20MHz) as SysTick clocksource */
   SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;

   /* SysTick reload value set to 1ms.
      RELOAD = 1ms / ( 1 / HCLK )
      with HCLK = 20MHz */
   SysTick->LOAD = 0x4E20U;

   /* Enable SysTick exception request */
   SysTick_Exception_Init();

   /* SysTick current value set to 0 */
   SysTick->VAL = 0x00U;

   /* Enable SysTick */
   SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Exception_Init( void )
{
   /* Enable SysTick exception request
      (counting down to zero asserts the exception request) */
   SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

uint32_t Get_SysTick( void )
{
   /* Increment by 1 the Systick global variable */
   return tick;
}

void Systick_Delay_Ms( uint32_t delay )
{
   uint32_t wait;

   /* SysTick current value set to 0 */
   SysTick->VAL = 0x0U;

   /* Enable SysTick */
   SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

   /* Count 'delay' ms */
   for ( wait = 0; wait < delay; wait++ )
   {   
      /* Wait for SysTick value to be 0 (1ms) */
      while ( ( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) == 0U )
      {
         /* Do nothing */
      }
   }

   /* Disable SysTick
   SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; */
}
