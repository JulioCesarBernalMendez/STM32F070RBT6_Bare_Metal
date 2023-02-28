#ifndef INTERRUPTS_H
#define INTERRUPTS_H

    #include "stm32f0xx.h"
    #include <stdint.h>
    #include <stdio.h>

    void ADC1_EOC_Callback( void );
    extern void initialise_monitor_handles();
    
#endif
