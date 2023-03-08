#ifndef INTERRUPTS_H
#define INTERRUPTS_H

    #include "stm32f0xx.h"

    extern void toggle_led( void );
    void DMA_USART2_Tx_Complete_Callback( void );

#endif
