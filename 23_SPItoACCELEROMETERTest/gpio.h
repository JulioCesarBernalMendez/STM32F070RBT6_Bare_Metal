#ifndef GPIO_H
#define GPIO_H

    #include "stm32f0xx.h"

    /* definitions required by the SPI1 peripheral (ADXL345 accelerometer) */
    #define GPIOA_CLK_ENBL()    (RCC->AHBENR |= RCC_AHBENR_GPIOAEN)

    #define GPIO_AFRL_AFSEL5_3    (0x01U << 23)
    #define GPIO_AFRL_AFSEL5_2    (0x01U << 22)
    #define GPIO_AFRL_AFSEL5_1    (0x01U << 21)
    #define GPIO_AFRL_AFSEL5_0    (0x01U << 20)

    #define GPIO_AFRL_AFSEL6_3    (0x01U << 27)
    #define GPIO_AFRL_AFSEL6_2    (0x01U << 26)
    #define GPIO_AFRL_AFSEL6_1    (0x01U << 25)
    #define GPIO_AFRL_AFSEL6_0    (0x01U << 24)

    #define GPIO_AFRL_AFSEL7_3    (0x01U << 31)
    #define GPIO_AFRL_AFSEL7_2    (0x01U << 30)
    #define GPIO_AFRL_AFSEL7_1    (0x01U << 29)
    #define GPIO_AFRL_AFSEL7_0    (0x01U << 28)

    /* definitions required by the I2C1 peripheral (LCD) */
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
