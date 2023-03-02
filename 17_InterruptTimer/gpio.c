#include "gpio.h"

void GPIOA_Init( void )
{   
    /* enable GPIOA clock access */
    GPIOA_CLK_ENBL();

    /* GPIOA5 (BOARD LED) as output */
    GPIOA->MODER &= ~( 0x01U << 11 );
    GPIOA->MODER |=  ( 0x01U << 10 );
}
