#include <stdint.h>
#include <stdio.h>
#include "stm32f0xx.h"
#include "main.h"
#include "systick.h"
#include "adc.h"

extern void initialise_monitor_handles();

int main( void )
{
    initialise_monitor_handles();

    uint16_t data;

    SysTick_Init();
    ADC_Init();

    while( 1 )
    {
        ADC_Ch0_Start_Conv();
        
        data = ADC_Ch0_Read_Conv();
        printf( "ADC = %u\n", data );

        Systick_Delay_Ms( 200 );
    }

    return 0;
}
