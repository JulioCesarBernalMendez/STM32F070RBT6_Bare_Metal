#ifndef EXTI_H
#define EXTI_H

    #include "stm32f0xx.h"
    
    #define NVIC_ISER_SETENA_12    (1U << 12)

    #define NVIC_IPR_PRI12_7       (1U << 7)
    #define NVIC_IPR_PRI12_6       (1U << 6)
    #define NVIC_IPR_PRI12_5       (1U << 5)
    #define NVIC_IPR_PRI12_4       (1U << 4)
    #define NVIC_IPR_PRI12_3       (1U << 3)
    #define NVIC_IPR_PRI12_2       (1U << 2)
    #define NVIC_IPR_PRI12_1       (1U << 1)
    #define NVIC_IPR_PRI12_0       (1U << 0)

    void ADC1_Interrupt_Init( void );

#endif
