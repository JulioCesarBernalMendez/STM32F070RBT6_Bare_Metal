#ifndef INTERRUPTS_H
#define INTERRUPTS_H
    
    #include <stdint.h>
    #include "stm32f0xx.h"
    #include "accelerometer.h"
    #include "main.h" /* Contains ADXL345_OVERRUN_INTERRUPT symbolic constant that prevents reading the DATA registers when OVERRUN is enabled*/

    void SysTick_Callback( void );
    void EXTI0_Callback( void );
    void EXTI1_Callback( void );
    void EXTI13_Callback( void );

#endif
