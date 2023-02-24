#ifndef INTERRUPTS_H
#define INTERRUPTS_H

    #include "stm32f0xx.h"
    #include <stdio.h>
    #include <string.h>
    #include <stdint.h>
    #include "usart.h"

    void USART2_Rx_Ready_Callback( void );
    void USART2_Tx_Complete_Callback( void );

#endif
