#include "gpio.h"

void GPIOA_Init( void )
{   
    /* enable GPIOA clock access */
    GPIOA_CLK_ENBL();

    /* GPIOA5 (BOARD LED) as output */
    GPIOA->MODER &= ~( 0x01U << 11 );
    GPIOA->MODER |=  ( 0x01U << 10 );
}

void GPIOC_Init( void )
{
    /* enable GPIOC clock access */
    GPIOC_CLK_ENBL();

    /* GPIOC13 (BOARD BUTTON) as input */
    GPIOC->MODER &= ~GPIO_MODER_MODER13_1;
    GPIOC->MODER &= ~GPIO_MODER_MODER13_0;

    /* BOARD BUTTON uses a PULL-UP resistor by default */

    /* configure GPIOC13 (BOARD button) as an interrupt on both
       falling and rising edge  */
    GPIOC_EXTI13_Init();
}
