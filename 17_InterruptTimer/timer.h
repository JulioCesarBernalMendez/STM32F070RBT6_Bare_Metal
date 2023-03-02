#ifndef TIMER_H
#define TIMER_H

    #include <stdint.h>
    #include "stm32f0xx.h"
    #include "exti.h"

    #define TIM3_CLK_ENBL()    (RCC->APB1ENR |= RCC_APB1ENR_TIM3EN)

    void TIM3_Init( void );

#endif
