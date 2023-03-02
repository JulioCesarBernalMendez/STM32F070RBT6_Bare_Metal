#ifndef EXTI_H
#define EXTI_H

    #include "stm32f0xx.h"

    #define NVIC_ISER_SETENA_16    (1U << 16)

    #define NVIC_IPR_PRI16_7       (1U << 7)
    #define NVIC_IPR_PRI16_6       (1U << 6)
    #define NVIC_IPR_PRI16_5       (1U << 5)
    #define NVIC_IPR_PRI16_4       (1U << 4)
    #define NVIC_IPR_PRI16_3       (1U << 3)
    #define NVIC_IPR_PRI16_2       (1U << 2)
    #define NVIC_IPR_PRI16_1       (1U << 1)
    #define NVIC_IPR_PRI16_0       (1U << 0)

    void TIM3_Interrupt_Init( void );

#endif
