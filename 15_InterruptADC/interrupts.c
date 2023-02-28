#include "interrupts.h"

void ADC1_IRQHandler( void )
{
    /* if ADC1 end of conversion is complete */
    if ( ( ADC1->ISR & ADC_ISR_EOC ) == ADC_ISR_EOC )
    {
        /* clear end of conversion flag */
        ADC1->ISR |= ADC_ISR_EOC;

        /* execute ADC1 end of conversion callback */
        ADC1_EOC_Callback();
    }
}

void ADC1_EOC_Callback( void )
{
    uint16_t adcdata;
    
    initialise_monitor_handles();

    /* update converted 16 bit value */
    adcdata = ( ADC1->DR & 0xFFFU );

    /* print updated (due to EOC interrupt) ADC1 value */
    printf( "ADC = %u\n", adcdata );
}
