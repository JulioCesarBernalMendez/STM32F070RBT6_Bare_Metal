#include "timer.h"

void TIM3_Init( void )
{
    /* Enable TIM3 clock */
    TIM3_CLK_ENBL();

    /* TIM3 as upcounter */
    TIM3->CR1 &= ~TIM_CR1_DIR;

    /* Counter over/under flow, setting UG bit or update generation through
       slave mode controller are selected as UEV (update event) sources */
    TIM3->CR1 &= ~TIM_CR1_URS;

    /* TIM3 UEV (update event) generation enabled */
    TIM3->CR1 &= ~TIM_CR1_UDIS;
        
    /* TIM3 prescaler.
       CK_CNT = (PSC + 1) / fck_psc
       = (0 + 1) / 20MHz
       = 50ns */
    TIM3->PSC = 0x0000U;
}

void TIM3_Delay_us( uint32_t useconds )
{   
    uint32_t i;

    /* TIM3 auto-reload register.
       In order to generate a timebase = 1us:
       AUTORELOAD = (1us / TIM3_period) - 1
       with TIM3_period = 50ns */
    TIM3->ARR = 0x0013U;

    /* Clear TIM3 counter */
    TIM3->CNT = 0x0000U;

    /* Clear update flag */
    TIM3->SR &= ~TIM_SR_UIF;

    /* Enable TIM3 */
    TIM3->CR1 |= TIM_CR1_CEN;
        
    /* Wait entered microseconds */
    for ( i = 0; i < useconds; i++ )
    {
        /* Wait for update flag (1us) */
        while ( ( TIM3->SR & TIM_SR_UIF ) == 0U )
        {
            /* Do nothing */
        }

        /* Clear update flag */
        TIM3->SR &= ~TIM_SR_UIF;
    }

    /* Disable TIM3 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void TIM3_Delay_ms( uint32_t mseconds )
{
    uint32_t i;

    /* TIM3 auto-reload register.
       In order to generate a timebase = 1ms:
       AUTORELOAD = (1ms / TIM3_period) - 1
       with TIM3_period = 50ns */
    TIM3->ARR = 0x4E1F;

    /* Clear TIM3 counter */
    TIM3->CNT = 0x0000U;

    /* Clear update flag */
    TIM3->SR &= ~TIM_SR_UIF;

    /* Enable TIM3 */
    TIM3->CR1 |= TIM_CR1_CEN;

    /* Wait entered miliseconds */
    for ( i = 0; i < mseconds; i++ )
    {
        /* Wait for update flag (1ms) */
        while ( ( TIM3->SR & TIM_SR_UIF ) == 0U )
        {
            /* Do nothing */
        }

        /* Clear update flag */
        TIM3->SR &= ~TIM_SR_UIF;
    }

    /* Disable TIM3 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
