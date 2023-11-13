#ifndef USART_H
#define USART_H
    
    #include "stm32f0xx.h"
    #include "exti.h"

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

    void USART2_Init( void );

    void USART2_Transmit_Character( uint8_t data );
    void USART2_Transmit_Array( const uint8_t *str );
    uint8_t USART2_Receive_Character( void );

#endif
