#ifndef GPIO_H
#define GPIO_H

    #include "stm32f0xx.h"
    #include "exti.h"

    #define GPIOA_CLK_ENBL()    (RCC->AHBENR |= RCC_AHBENR_GPIOAEN)
    #define GPIOC_CLK_ENBL()    (RCC->AHBENR |= RCC_AHBENR_GPIOCEN)

    void GPIOA_Init( void );
    void GPIOC_Init( void );

#endif
