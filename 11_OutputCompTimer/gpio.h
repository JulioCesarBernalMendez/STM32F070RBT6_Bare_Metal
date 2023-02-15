#ifndef GPIO_H
#define GPIO_H

    #define GPIOB_CLK_ENBL()       (RCC->AHBENR |= RCC_AHBENR_GPIOBEN)
    
    #define GPIO_AFRH_AFSEL14_0    (0x01U << 24)
    #define GPIO_AFRH_AFSEL14_1    (0x01U << 25)
    #define GPIO_AFRH_AFSEL14_2    (0x01U << 26)
    #define GPIO_AFRH_AFSEL14_3    (0x01U << 27)

#endif
