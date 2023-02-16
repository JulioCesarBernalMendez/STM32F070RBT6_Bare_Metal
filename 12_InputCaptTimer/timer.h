#ifndef TIMER_H
#define TIMER_H

    #define TIM3_CLK_ENBL()     (RCC->APB1ENR |= RCC_APB1ENR_TIM3EN)
    #define TIM15_CLK_ENBL()    (RCC->APB2ENR |= RCC_APB2ENR_TIM15EN)
    #define TIM16_CLK_ENBL()    (RCC->APB2ENR |= RCC_APB2ENR_TIM16EN)

    void TIM3_Init( void );
    void TIM3_Delay_1s( uint32_t seconds );
    void TIM15_Init( void );
    void TIM16_Init( void );
    uint16_t TIM16_Input_Capture( void );

#endif