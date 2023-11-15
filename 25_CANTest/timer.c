#include <stdint.h>
#include "stm32f0xx.h"
#include "timer.h"

void TIM3_Init( void )
{   
    /* enable TIM3 clock */
    TIM3_CLK_ENBL();

    /* TIM3 counts up (counts from 0 up to the auto-reload value) */
    TIM3->CR1 &= ~TIM_CR1_DIR;

    /* TIM3 UEV (update event) generation enabled */
    TIM3->CR1 &= ~TIM_CR1_UDIS;

    /* TIM3 auto-reload preload disabled */
    TIM3->CR1 &= ~TIM_CR1_ARPE;
    
    /* TIM3 prescaler.
       CK_CNT period = (1 / PCLK)(PSC + 1) = (1 / 48MHz)(23 + 1) = 0.5us
       with PCLK = 48MHz (refer to SystemInit() in system_stm32f0xx.c) */
    TIM3->PSC = 0x17U;

    /* TIM3 reload value, when TIM3 counts to ARR + 1 (= 1 + 1 = 2), (0.5us)(2) = 1us has passed
       NOTE: this reload value is necessary for the TIM3_Delay_us() function */
    TIM3->ARR = 0x01U;
}

void TIM3_Delay_us( uint32_t us )
{
    uint32_t wait;

    /* clear TIM3 counter */
    TIM3->CNT = 0x00U;

    /* enable TIM3 peripheral */
    TIM3->CR1 |= TIM_CR1_CEN;

    /* generate 'us' microseconds delay */
    for ( wait = 0U; wait < us; wait++ )
    {
        /* wait for TIM3 update interrupt flag (1us) */
        while ( ( TIM3->SR & TIM_SR_UIF ) != TIM_SR_UIF )
        {
            /* do nothing */
        }

        /* clear TIM3 update interrupt flag */
        TIM3->SR &= ~TIM_SR_UIF;
    }

    /* disable TIM3 peripheral */
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
