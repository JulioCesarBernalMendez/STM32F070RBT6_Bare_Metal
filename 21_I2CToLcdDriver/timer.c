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
       CK_CNT = ( PSC + 1 ) / fck_psc
       = ( 0 + 1 ) / 48MHz
       = 20.834ns */
    TIM3->PSC = 0x0000U;
}

void TIM3_Delay_us( uint32_t useconds )
{   
    uint32_t i;

    /* TIM3 auto-reload register.
       In order to generate a timebase = 1us:
       AUTORELOAD = (1us / TIM3_period) - 1
       with TIM3_period = 20.834ns */
    TIM3->ARR = 0x002FU;

    /* clear TIM3 counter */
    TIM3->CNT = 0x0000U;

    /* enable TIM3 */
    TIM3->CR1 |= TIM_CR1_CEN;
        
    /* wait entered microseconds */
    for ( i = 0; i < useconds; i++ )
    {
        /* wait for update flag (1us) */
        while ( ( TIM3->SR & TIM_SR_UIF ) == 0U )
        {
            /* do nothing */
        }

        /* clear update flag */
        TIM3->SR &= ~TIM_SR_UIF;
    }

    /* disable TIM3 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void TIM3_Delay_ms( uint32_t mseconds )
{
    uint32_t i;

    /* TIM3 auto-reload register.
       In order to generate a timebase = 1ms:
       AUTORELOAD = (1ms / TIM3_period) - 1
       with TIM3_period = 20.834ns */
    TIM3->ARR = 0x0BB7FU;

    /* clear TIM3 counter */
    TIM3->CNT = 0x0000U;

    /* enable TIM3 */
    TIM3->CR1 |= TIM_CR1_CEN;
        
    /* wait entered microseconds */
    for ( i = 0; i < mseconds; i++ )
    {
        /* wait for update flag (1ms) */
        while ( ( TIM3->SR & TIM_SR_UIF ) == 0U )
        {
            /* do nothing */
        }

        /* clear update flag */
        TIM3->SR &= ~TIM_SR_UIF;
    }

    /* disable TIM3 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
