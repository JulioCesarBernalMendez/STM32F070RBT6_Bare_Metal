#ifndef MAIN_H_
#define MAIN_H_
    
    #include <stdint.h>
    #include "stm32f0xx.h"
    #include "gpio.h"
    #include "systick.h"

    #define LED_PIN    GPIO_ODR_5

    void Toggle_Led( void );

#endif
