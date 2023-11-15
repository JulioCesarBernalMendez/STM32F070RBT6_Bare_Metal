#ifndef TIMER_H
#define TIMER_H


    #define TIM3_CLK_ENBL()    (RCC->APB1ENR |= RCC_APB1ENR_TIM3EN)

    void TIM3_Init( void );
    void TIM3_Delay_us( uint32_t us );

#endif
