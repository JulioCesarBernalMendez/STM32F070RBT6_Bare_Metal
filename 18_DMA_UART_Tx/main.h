#ifndef MAIN_H
#define MAIN_H

    #include <stdint.h>
    #include <stdio.h>
    #include "stm32f0xx.h"
    #include "gpio.h"
    #include "usart.h"
    #include "dma.h"

    #define LED_PIN    GPIO_ODR_5

    void toggle_led( void );

#endif
