#include <stdint.h>
#include "main.h"

void GPIOA_Init( void );
void toggle_led( void );

int main( void )
{
    volatile uint32_t count;

    GPIOA_Init();

    while( 1 )
    {   
        /* wait a few miliseconds */
        for ( count = 0; count <= 100000; count++ )
        {
            /* do nothing */
        }

        toggle_led();
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

void toggle_led( void )
{
    GPIOA->ODR ^= ( 1 << 5 );
}
