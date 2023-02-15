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

    /* NOTE: TIM15 is a downcounter */

    /* TIM15 channel 1 configured as output compare */
    TIM15->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM15->CCMR1 &= ~TIM_CCMR1_CC1S_0;

    /* TIM15 output compare in toggle mode  */
    TIM15->CCMR1 &= ~TIM_CCMR1_OC1M_2;
    TIM15->CCMR1 |=  TIM_CCMR1_OC1M_1;
    TIM15->CCMR1 |=  TIM_CCMR1_OC1M_0;

    /* TIM15 compare channel 1 active high */
    TIM15->CCER &= ~TIM_CCER_CC1P;

    /* TIM15 auto-reload preload enabled */
    TIM15->CR1 |= TIM_CR1_ARPE;

    /* TIM15 UEV (update event) generation enabled */
    TIM15->CR1 &= ~TIM_CR1_UDIS;

    /* TIM15 compare preloaded control enabled.
       CC1E, CC1NE (not implemented) and OC1M bits are preloaded */
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
