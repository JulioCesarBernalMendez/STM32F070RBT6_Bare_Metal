#ifndef INTERRUPTS_H
#define INTERRUPTS_H

    #include "stm32f0xx.h"
    #include <stdint.h>

    extern uint32_t tick;

    void SysTick_Callback( void );

#endif
