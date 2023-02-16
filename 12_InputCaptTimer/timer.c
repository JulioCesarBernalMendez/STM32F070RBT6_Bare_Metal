#include <stdint.h>
#include "stm32f0xx.h"
#include "timer.h"
#include "gpio.h"

void TIM3_Init( void )
{   
    /* enable TIM3 clock */
    TIM3_CLK_ENBL();

    /* TIM3 as upcounter */
    TIM3->CR1 &= ~TIM_CR1_DIR;

    /* TIM3 UEV (update event) generation enabled */
    TIM3->CR1 &= ~TIM_CR1_UDIS;

    /* TIM3 auto-reload preload enabled */
    TIM3->CR1 |= TIM_CR1_ARPE;
    
    /* TIM3 prescaler.
       In order to generate a TIM3_period = 1ms:
       PSC = (1ms x fck_psc) - 1
       with fck_psc = 8MHz */
    TIM3->PSC = 0x1F3FU;

    /* enable TIM3 */
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_Delay_1s( uint32_t seconds )
{
    uint32_t i;

    /* TIM3 auto-reload register.
       In order to generate a timebase = 1 second:
       AUTORELOAD = (1s / TIM3_period) - 1
       with TIM3_period = 1ms */
    TIM3->ARR = 0x03E7U;

    /* wait entered seconds */
    for ( i = 0; i < seconds; i++ )
    {
        /* clear TIM3 counter */
        TIM3->CNT = 0x0U;

        /* wait for update flag (1 second) */
        while ( ( TIM3->SR & TIM_SR_UIF ) == 0U )
        {
            /* do nothing */
        }

        /* clear update flag */
        TIM3->SR &= ~TIM_SR_UIF;
    }
}

void TIM15_Init( void )
{   
    /* enable GPIOB clock */
    GPIOB_CLK_ENBL();

    /* GPIOB14 (TIM15_CH1) in alternate mode */
    GPIOB->MODER |=  GPIO_MODER_MODER14_1;
    GPIOB->MODER &= ~GPIO_MODER_MODER14_0;

    /* GPIOB14 as push-pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_14;

    /* GPIOB14 with pull-down */
    GPIOB->PUPDR |=  GPIO_PUPDR_PUPDR14_1;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR14_0;

    /* GPIOB14 as AF01 (TIM15_CH1) */
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRH_AFSEL14_3;
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRH_AFSEL14_2;
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRH_AFSEL14_1;
    GPIOB->AFR[ 1 ] |=  GPIO_AFRH_AFSEL14_0;

    /* enable TIM15 clock access */
    TIM15_CLK_ENBL();

    /* NOTE: TIM15 is an upcounter */

    /* TIM15 channel 1 configured as output compare */
    TIM15->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM15->CCMR1 &= ~TIM_CCMR1_CC1S_0;

    /* TIM15 output compare in toggle mode  */
    TIM15->CCMR1 &= ~TIM_CCMR1_OC1M_2;
    TIM15->CCMR1 |=  TIM_CCMR1_OC1M_1;
    TIM15->CCMR1 |=  TIM_CCMR1_OC1M_0;

    /* TIM15 compare channel 1 active high */
    TIM15->CCER &= ~TIM_CCER_CC1P;

    /* TIM15 UEV (update event) generation enabled */
    TIM15->CR1 &= ~TIM_CR1_UDIS;

    /* TIM15 auto-reload preload enabled */
    TIM15->CR1 |= TIM_CR1_ARPE;

    /* TIM15 compare preloaded control enabled.
       CC1E, CC1NE (not implemented) and OC1M bits are not preloaded */
    TIM15->CR2 &= ~TIM_CR2_CCPC;

    /* TIM15 repetition counter set to 0.
       Each time this counter reaches zero an update event (UEV) is generated.
       This counter decrements by one every time a timebase of TIM15 has passed */
    TIM15->RCR = 0x00U;

    /* TIM15 prescaler.
       In order to generate a TIM15_period = 1ms:
       PSC = (1ms x fck_psc) - 1
       with fck_psc = 8MHz */
    TIM15->PSC = 0x1F3FU;

    /* TIM15 auto-reload register.
       In order to generate a timebase = 1 second:
       AUTORELOAD = (1s / TIM15_period) - 1
       with TIM15_period = 1ms */
    TIM15->ARR = 0x03E7U;

    /* TIM15 counter cleared */
    TIM15->CNT = 0x00U;

    /* TIM15 compare output channel 1 enabled */
    TIM15->CCER |= TIM_CCER_CC1E;

    /* TIM15 output compare enabled */
    TIM15->BDTR |= TIM_BDTR_MOE;

    /* enable TIM15 */
    TIM15->CR1 |= TIM_CR1_CEN;
}

void TIM16_Init( void )
{
    /* enable GPIOA clock */
    GPIOA_CLK_ENBL();

    /* GPIOA6 (TIM16_CH1) in alternate mode */
    GPIOA->MODER |=  GPIO_MODER_MODER6_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER6_0;

    /* GPIOA6 as push-pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;

    /* GPIOA6 with pull-down */
    GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR6_1;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6_0;

    /* GPIOA6 as AF05 (TIM16_CH1) */
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL6_3;
    GPIOA->AFR[ 0 ] |=  GPIO_AFRL_AFSEL6_2;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL6_1;
    GPIOA->AFR[ 0 ] |=  GPIO_AFRL_AFSEL6_0;

    /* enable TIM16 clock access */
    TIM16_CLK_ENBL();
    
    /* NOTE: TIM16 is an upcounter */
    
    /* TIM16 channel 1 configured as input capture and mapped on TI1 */
    TIM16->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM16->CCMR1 |=  TIM_CCMR1_CC1S_0;
    
    /* TIM16 capture operation on both edges of not inverted filtered 
       timer input 1 (TI1FP1)  */
    TIM16->CCER |= TIM_CCER_CC1NP;
    TIM16->CCER |= TIM_CCER_CC1P;
    
    /* TIM16 UEV (update event) generation enabled */
    TIM16->CR1 &= ~TIM_CR1_UDIS;

    /* TIM16 auto-reload preload enabled */
    TIM16->CR1 |= TIM_CR1_ARPE;
    
    /* TIM16 compare preloaded control enabled.
       CC1E, CC1NE (not implemented) and OC1M bits are not preloaded */
    TIM16->CR2 &= ~TIM_CR2_CCPC;

    /* no prescaler on input capture 1.
       Capture is done each time an edge is detected */
    TIM16->CCMR1 &= ~TIM_CCMR1_IC1PSC_1;
    TIM16->CCMR1 &= ~TIM_CCMR1_IC1PSC_0;
     
    /* TIM16 repetition counter set to 0.
       Each time this counter reaches zero an update event (UEV) is generated.
       This counter decrements by one every time a timebase of TIM16 has passed */
    TIM16->RCR = 0x00U;

    /* TIM16 prescaler.
       In order to generate a TIM16_period = 1ms:
       PSC = (1ms x fck_psc) - 1
       with fck_psc = 8MHz */
    TIM16->PSC = 0x1F3FU;

    /* TIM16 auto-reload register.
       In order to generate a timebase = 1 second:
       AUTORELOAD = (1s / TIM16_period) - 1
       with TIM16_period = 1ms */
    TIM16->ARR = 0x03E7U;

    /* TIM16 capture input channel 1 enabled */
    TIM16->CCER |= TIM_CCER_CC1E;

    /* TIM16 output compare enabled */
    TIM16->BDTR |= TIM_BDTR_MOE;

    /* enable TIM16 */
    TIM16->CR1 |= TIM_CR1_CEN;
}

uint16_t TIM16_Input_Capture( void )
{
   /* wait for TIM16 input 1 to be captured */
   while ( ( TIM16->SR & TIM_SR_CC1IF ) == 0U )
   {
      /* do nothing */
   }

   /* return the content of the counter value transferred by 
      the last input capture 1 */
   return TIM16->CCR1;
}
