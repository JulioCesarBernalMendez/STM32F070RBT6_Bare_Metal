#ifndef DMA_H
#define DMA_H

    #include <stdint.h>
    #include "stm32f0xx.h"

    void DMA_USART2_Tx_Init( uint32_t src, uint32_t dest, uint16_t length );
    void DMA_USART2_Tx_Interrupt_Init( void );

    #define NVIC_ISER_SETENA_11    (1U << 11)

    #define NVIC_IPR_PRI11_7       (1U << 31)
    #define NVIC_IPR_PRI11_6       (1U << 30)
    #define NVIC_IPR_PRI11_5       (1U << 29)
    #define NVIC_IPR_PRI11_4       (1U << 28)
    #define NVIC_IPR_PRI11_3       (1U << 27)
    #define NVIC_IPR_PRI11_2       (1U << 26)
    #define NVIC_IPR_PRI11_1       (1U << 25)
    #define NVIC_IPR_PRI11_0       (1U << 24)

#endif
