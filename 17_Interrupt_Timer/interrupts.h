#ifndef INTERRUPTS_H
#define INTERRUPTS_H

    #include "stm32f0xx.h"
    #include <stdint.h>

    #define LED_PIN    GPIO_ODR_5

    void TIM3_Callback( void );    
    void toggle_led( void );

#endif
