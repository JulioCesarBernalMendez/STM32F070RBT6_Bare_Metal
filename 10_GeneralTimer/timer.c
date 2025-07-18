#include <stdint.h>
#include "stm32f0xx.h"
#include "timer.h"

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