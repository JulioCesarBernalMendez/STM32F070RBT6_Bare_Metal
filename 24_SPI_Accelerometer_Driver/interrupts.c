#include "interrupts.h"

/* Global variables from main.c */
extern ACCEL_HandleTypeDef ACCEL_Handler;
extern uint8_t int2Flag;
extern int16_t x0g, y0g, z0g;
extern int16_t xmeas, ymeas, zmeas;

/* Global variables from systick.c */
extern uint32_t tick;

void SysTick_Handler( void )
{
   /* Execute SysTick callback */
   SysTick_Callback();
}

void EXTI0_1_IRQHandler( void )
{  
   /* If pending request is on EXTI1 (= GPIOC1 = INT2 of ADXL345) */
   if ( ( EXTI->PR & EXTI_PR_PIF1 ) == EXTI_PR_PIF1 )
   {
      /* Clear EXTI1 pending request bit */
      EXTI->PR |= EXTI_PR_PIF1;

      /* Execute EXTI1 callback */
      EXTI1_Callback();
   }

   /* If pending request is on EXTI0 (= GPIOC0 = INT1 of ADXL345) */
   if ( ( EXTI->PR & EXTI_PR_PIF0 ) == EXTI_PR_PIF0 )
   {
      /* Clear EXTI0 pending request bit */
      EXTI->PR |= EXTI_PR_PIF0;
      
      /* If OVERRUN_INTERRUPT is enabled, do not execute the next block of code
         (do not read DATA registers from the ADXL345, so that FIFO replaces previous data stored
         and overrun interrupt will be triggered).
         if OVERRUN_INTERRUPT is not enabled, read the ADXL345 DATA registers instead */
      #if !defined (ADXL345_OVERRUN_INTERRUPT)
         /* Execute EXTI0 callback */
         EXTI0_Callback();
      #endif
   }
}

void EXTI4_15_IRQHandler( void )
{
   /* If pending request is on GPIOC13 (board button) */
   if ( ( EXTI->PR & EXTI_PR_PIF13 ) == EXTI_PR_PIF13 )
   {
      /* Clear EXTI13 peding request bit */
      EXTI->PR |= EXTI_PR_PIF13;

      /* Execute EXTI13 callback */
      EXTI13_Callback();
   }
}

void SysTick_Callback( void )
{   
   /* Every time the SysTick exception occurs (1ms)
      the SysTick counter will be increased,
      this counter can be used by the user as a timebase
      to execute periodic tasks */
   tick++;
}

void EXTI0_Callback( void )
{
   /* If a previous SPI1 transmission is in progress when this interrupt was triggered */
   if ( ( GPIOA->ODR & GPIO_ODR_4 ) == 0U ) /* ADXL345 CS pin LOW = slave selected */
   {  
      /* Set ADXL345 CS pin to HIGH (IDLE state = slave not selected).
         This avoids overlapping bits for the current and next SPI frames) */
      SPI1_CS_Disable();
   }

   /* Read acceleration raw data registers */
   ADXL345_API_Read_Axes( &ACCEL_Handler, &xmeas, &ymeas, &zmeas );
}

void EXTI1_Callback( void )
{
   /* Raise ADXL345 INT2 interrupt status flag */
   int2Flag = 1U;
}

void EXTI13_Callback( void )
{
   /* Read acceleration raw data registers (the number of entries in FIFO will decrease
      by one if FIFO is set to a mode other than bypass) */
   ADXL345_API_Read_Axes( &ACCEL_Handler, &xmeas, &ymeas, &zmeas );
}
