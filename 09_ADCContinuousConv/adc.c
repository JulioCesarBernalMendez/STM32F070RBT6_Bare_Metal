#include "stm32f0xx.h"
#include "adc.h"

void ADC_Init( void )
{
    /* enable GPIOA clock */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    /* GPIOA0 (ADC_IN0) as analog input */
    GPIOA->MODER |= GPIO_MODER_MODER0_1;
    GPIOA->MODER |= GPIO_MODER_MODER0_0;

    /* enable ADC clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    /* ADC in continuous conversion mode */
    ADC1->CFGR1 |= ADC_CFGR1_CONT;

    /* ADC right alignment data */
    ADC1->CFGR1 &= ~ADC_CFGR1_ALIGN;

    /* ADC 12 bits resolution */
    ADC1->CFGR1 &= ~ADC_CFGR1_RES_1;
    ADC1->CFGR1 &= ~ADC_CFGR1_RES_0;

    /* ADC channel 0 selected for conversion */
    ADC1->CHSELR |= ADC_CHSELR_CHSEL0;

    /* enable ADC */
    ADC1->CR |= ADC_CR_ADEN;
}

void ADC_Ch0_Start_Conv( void )
{
    /* wait for ADC to be ready */
    while ( ( ADC1->ISR & ADC_ISR_ADRDY ) == 0U )
    {
        /* do nothing */
    }

    /* start ADC conversion */
    ADC1->CR |= ADC_CR_ADSTART;
}

uint16_t ADC_Ch0_Read_Conv( void )
{
    /* wait for conversion to be complete */
    while ( ( ADC1->ISR & ADC_ISR_EOC ) == 0U )
    {
        /* do nothing */
    }

    /* return converted 16 bit value */
    return ( ADC1->DR & 0xFFFU );
}
