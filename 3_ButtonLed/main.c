#include <stdint.h>
#include "stm32f0xx.h"
#include "main.h"

void GPIOA_Init( void );
void GPIOC_Init( void );

int main( void )
{
    GPIOA_Init();
    GPIOC_Init();

    while( 1 )
    {   
        /* if board button is not pressed 
           (button uses inverted logic) */
        if ( ( GPIOC->IDR & BUTTON_PIN ) == BUTTON_PIN )
        {
            /* turn off led */
            GPIOA->ODR &= ~( LED_PIN );
        }
        else /* if button is pressed */
        {
            /* turn on led */
            GPIOA->ODR |= LED_PIN;
        }

    }

    return 0;
}

void GPIOA_Init( void )
{
    GPIOA_CLK_ENBL();

    /* GPIOA5 (BOARD LED) as output */
    GPIOA->MODER &= ~( 0x01U << 11 );
    GPIOA->MODER |=  ( 0x01U << 10 );

    /* GPIOA5 as push-pull output upon reset */

    /* GPIOA5 as low speed output upon reset */
}

void GPIOC_Init( void )
{
    GPIOC_CLK_ENBL();

    /* GPIOC13 (BOARD BUTTON) as input */
    GPIOC->MODER &=  ~( 0x01U << 27 );
    GPIOC->MODER &=  ~( 0x01U << 26 );

    /* BOARD BUTTON uses a PULL-UP resistor */
}
