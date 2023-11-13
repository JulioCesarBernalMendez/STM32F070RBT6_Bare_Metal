#include <stdint.h>
#include <stdio.h>
#include "stm32f0xx.h"
#include "main.h"
#include "timer.h"

extern void initialise_monitor_handles();

int main( void )
{   
    uint16_t input_capture;

    /* initialize TIM15 as output compare mode */
    TIM15_Init();

    /* inititalie TIM16 as input compare mode */
    TIM16_Init();

    /* NOTE: GPIOB14 (TIM15_CH1 output compare) is used as input
        for GPIOA6 (TIM16_CH1 input capture) */

    initialise_monitor_handles();

    while( 1 )
    {      
        /* read input capture on TIM16 */
        input_capture = TIM16_Input_Capture();
        printf( "Input Capture = %u\n", input_capture );
    }

    return 0;
}
