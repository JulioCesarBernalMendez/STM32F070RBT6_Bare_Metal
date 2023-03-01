#ifndef SYSTICK_H
#define SYSTICK_H

    #include <stdint.h>
    #include "stm32f0xx.h"
    #include "exti.h"

    void SysTick_Init( void );
    uint32_t Get_SysTick( void );

#endif
