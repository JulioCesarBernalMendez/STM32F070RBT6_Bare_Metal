#ifndef USART_H
#define USART_H
    
    #include "stm32f0xx.h"

    #define GPIOA_CLK_ENBL()      (RCC->AHBENR  |= RCC_AHBENR_GPIOAEN)
    #define USART2_CLK_ENBL()     (RCC->APB1ENR |= RCC_APB1ENR_USART2EN)

    #define GPIO_AFRL_AFSEL2_3    (0x01U << 11)
    #define GPIO_AFRL_AFSEL2_2    (0x01U << 10)
    #define GPIO_AFRL_AFSEL2_1    (0x01U << 9)
    #define GPIO_AFRL_AFSEL2_0    (0x01U << 8)

    #define GPIO_AFRL_AFSEL3_3    (0x01U << 15)
    #define GPIO_AFRL_AFSEL3_2    (0x01U << 14)
    #define GPIO_AFRL_AFSEL3_1    (0x01U << 13)
    #define GPIO_AFRL_AFSEL3_0    (0x01U << 12)

    #define BAUD                  9600
    #define GET_USARTDIV()        (SystemCoreClock / BAUD)

    #define NVIC_ISER_SETENA_28    (1U << 28)

    #define NVIC_IPR_PRI28_7       (1U << 7)
    #define NVIC_IPR_PRI28_6       (1U << 6)
    #define NVIC_IPR_PRI28_5       (1U << 5)
    #define NVIC_IPR_PRI28_4       (1U << 4)
    #define NVIC_IPR_PRI28_3       (1U << 3)
    #define NVIC_IPR_PRI28_2       (1U << 2)
    #define NVIC_IPR_PRI28_1       (1U << 1)
    #define NVIC_IPR_PRI28_0       (1U << 0)

    void USART2_Init( void );
    void USART2_Interrupt_Init( void );

#endif
