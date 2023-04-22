#ifndef GPIO_H
#define GPIO_H

    #include "stm32f0xx.h"
        
    #define GPIOB_CLK_ENBL()      (RCC->AHBENR |= RCC_AHBENR_GPIOBEN)

    #define GPIO_AFRL_AFSEL8_3    (0x01U << 3)
    #define GPIO_AFRL_AFSEL8_2    (0x01U << 2)
    #define GPIO_AFRL_AFSEL8_1    (0x01U << 1)
    #define GPIO_AFRL_AFSEL8_0    (0x01U << 0)

    #define GPIO_AFRL_AFSEL9_3    (0x01U << 7)
    #define GPIO_AFRL_AFSEL9_2    (0x01U << 6)
    #define GPIO_AFRL_AFSEL9_1    (0x01U << 5)
    #define GPIO_AFRL_AFSEL9_0    (0x01U << 4)

#endif
