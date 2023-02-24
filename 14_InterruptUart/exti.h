#ifndef EXTI_H
#define EXTI_H

    #include "stm32f0xx.h"
    
    void USART2_Interrupt_Init( void );

    #define NVIC_ISER_SETENA_28    (1U << 28)

    #define NVIC_IPR_PRI28_7       (1U << 7)
    #define NVIC_IPR_PRI28_6       (1U << 6)
    #define NVIC_IPR_PRI28_5       (1U << 5)
    #define NVIC_IPR_PRI28_4       (1U << 4)
    #define NVIC_IPR_PRI28_3       (1U << 3)
    #define NVIC_IPR_PRI28_2       (1U << 2)
    #define NVIC_IPR_PRI28_1       (1U << 1)
    #define NVIC_IPR_PRI28_0       (1U << 0)

#endif
