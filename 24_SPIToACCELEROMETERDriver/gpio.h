#ifndef GPIO_H
#define GPIO_H

    #include "stm32f0xx.h"

    /* Definitions required by the SPI1 peripheral (ADXL345 accelerometer) */
    #define GPIOA_CLK_ENBL()          (RCC->AHBENR |= RCC_AHBENR_GPIOAEN)

    #define GPIO_AFRL_AFSEL5_3        (0x01U << 23)
    #define GPIO_AFRL_AFSEL5_2        (0x01U << 22)
    #define GPIO_AFRL_AFSEL5_1        (0x01U << 21)
    #define GPIO_AFRL_AFSEL5_0        (0x01U << 20)

    #define GPIO_AFRL_AFSEL6_3        (0x01U << 27)
    #define GPIO_AFRL_AFSEL6_2        (0x01U << 26)
    #define GPIO_AFRL_AFSEL6_1        (0x01U << 25)
    #define GPIO_AFRL_AFSEL6_0        (0x01U << 24)

    #define GPIO_AFRL_AFSEL7_3        (0x01U << 31)
    #define GPIO_AFRL_AFSEL7_2        (0x01U << 30)
    #define GPIO_AFRL_AFSEL7_1        (0x01U << 29)
    #define GPIO_AFRL_AFSEL7_0        (0x01U << 28)

    /* Definitions required by the I2C1 peripheral (LCD) */
    #define GPIOB_CLK_ENBL()          (RCC->AHBENR |= RCC_AHBENR_GPIOBEN)

    #define GPIO_AFRL_AFSEL8_3        (0x01U << 3)
    #define GPIO_AFRL_AFSEL8_2        (0x01U << 2)
    #define GPIO_AFRL_AFSEL8_1        (0x01U << 1)
    #define GPIO_AFRL_AFSEL8_0        (0x01U << 0)

    #define GPIO_AFRL_AFSEL9_3        (0x01U << 7)
    #define GPIO_AFRL_AFSEL9_2        (0x01U << 6)
    #define GPIO_AFRL_AFSEL9_1        (0x01U << 5)
    #define GPIO_AFRL_AFSEL9_0        (0x01U << 4)

    /* Definitions required by EXTI lines for INT1 and INT2 interrupts of the ADXL345 */
    #define SYSCFG_CLK_ENBL()         (RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN)

    #define NVIC_ISER_SETENA_5        (0x01U << 5)
    #define NVIC_ICER_CLRENA_5        (0x01U << 5)

    #define NVIC_IPR1_PRI5_7          (0x01U << 15)
    #define NVIC_IPR1_PRI5_6          (0x01U << 14)
    #define NVIC_IPR1_PRI5_5          (0x01U << 13)
    #define NVIC_IPR1_PRI5_4          (0x01U << 12)
    #define NVIC_IPR1_PRI5_3          (0x01U << 11)
    #define NVIC_IPR1_PRI5_2          (0x01U << 10)
    #define NVIC_IPR1_PRI5_1          (0x01U << 9)
    #define NVIC_IPR1_PRI5_0          (0x01U << 8)

    /* Definitions required by EXTI lines for the board button interrupt */
    #define GPIOC_CLK_ENBL()          (RCC->AHBENR |= RCC_AHBENR_GPIOCEN)

    #define NVIC_ISER_SETENA_7        (0x01 << 7)

    #define NVIC_IPR1_PRI7_7          (0x01U << 31)
    #define NVIC_IPR1_PRI7_6          (0x01U << 30)
    #define NVIC_IPR1_PRI7_5          (0x01U << 29)
    #define NVIC_IPR1_PRI7_4          (0x01U << 28)
    #define NVIC_IPR1_PRI7_3          (0x01U << 27)
    #define NVIC_IPR1_PRI7_2          (0x01U << 26)
    #define NVIC_IPR1_PRI7_1          (0x01U << 25)
    #define NVIC_IPR1_PRI7_0          (0x01U << 24)

    void GPIO_Interrupt_Init( void );
    void GPIOC_EXTI0_1_Init( void );
    void GPIOC_Init( void );
    void GPIOC_EXTI13_Init( void );

#endif
