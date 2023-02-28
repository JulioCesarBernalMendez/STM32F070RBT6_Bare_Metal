#include "adc.h"

void ADC1_Init( void )
{
    /* enable GPIOA clock */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    /* GPIOA0 (ADC_IN0) as analog */
    GPIOA->MODER |= GPIO_MODER_MODER0_1;
    GPIOA->MODER |= GPIO_MODER_MODER0_0;

    /* enable ADC1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    /* ADC1 in single conversion mode */
    ADC1->CFGR1 |= ADC_CFGR1_CONT;

    /* ADC1 right alignment data */
    ADC1->CFGR1 &= ~ADC_CFGR1_ALIGN;

    /* ADC1 12 bits resolution */
    ADC1->CFGR1 &= ~ADC_CFGR1_RES_1;
    ADC1->CFGR1 &= ~ADC_CFGR1_RES_0;

    /* ADC1 channel 0 selected for conversion */
    ADC1->CHSELR |= ADC_CHSELR_CHSEL0;

    /* enable ADC1 */
    ADC1->CR |= ADC_CR_ADEN;

    /* enable ADC1 end of conversion interrupt */
    ADC1_Interrupt_Init();
}

void ADC1_Ch0_Start_Conv( void )
{
    /* wait for ADC1 to be ready */
    while ( ( ADC1->ISR & ADC_ISR_ADRDY ) == 0U )
    {
        /* do nothing */
    }

    /* start ADC1 conversion */
    ADC1->CR |= ADC_CR_ADSTART;
}
