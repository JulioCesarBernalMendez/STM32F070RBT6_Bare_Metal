#ifndef SYSTICK_H
#define SYSTICK_H

    #include <stdint.h>
    #include "stm32f0xx.h"

    void SysTick_Init( void );
    void Systick_Delay_Ms( uint32_t delay );

#endif
