#ifndef EXTI_H
#define EXTI_H

    #include "stm32f0xx.h"
    
    void GPIOC_EXTI13_Init( void );

    #define SYSCFG_EXTICR4_EXTI13_3    (1U << 7);
    #define SYSCFG_EXTICR4_EXTI13_2    (1U << 6);
    #define SYSCFG_EXTICR4_EXTI13_1    (1U << 5);
    #define SYSCFG_EXTICR4_EXTI13_0    (1U << 4);

    #define NVIC_ISER_SETENA_7         (1U << 7)

    #define NVIC_IPR_PRI7_7            (1U << 31)
    #define NVIC_IPR_PRI7_6            (1U << 30)
    #define NVIC_IPR_PRI7_5            (1U << 29)
    #define NVIC_IPR_PRI7_4            (1U << 28)
    #define NVIC_IPR_PRI7_3            (1U << 27)
    #define NVIC_IPR_PRI7_2            (1U << 26)
    #define NVIC_IPR_PRI7_1            (1U << 25)
    #define NVIC_IPR_PRI7_0            (1U << 24)

#endif
