#ifndef SYSTICK_H
#define SYSTICK_H

    #include <stdint.h>
    #include "stm32f0xx.h"

    void SysTick_Init( void );
    void SysTick_Exception_Init( void );
    uint32_t Get_SysTick( void );
    void Systick_Delay_Ms( uint32_t delay );

#endif
