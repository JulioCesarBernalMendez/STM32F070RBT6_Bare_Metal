#ifndef GPIO_H
#define GPIO_H

    #include "stm32f0xx.h"

    #define GPIOA_CLK_ENBL()    (RCC->AHBENR |= RCC_AHBENR_GPIOAEN)
    void GPIOA_Init( void );

#endif
