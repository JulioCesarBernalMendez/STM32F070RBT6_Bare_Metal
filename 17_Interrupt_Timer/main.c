#include "main.h"

int main( void )
{   
    /* initialize GPIOA5 (BOARD LED) as output */
    GPIOA_Init();

    /* enable:
       - TIM3 as general purpose timer
       - and TIM3 overflow interrupt */
    TIM3_Init();

    while( 1 )
    {   
        /* do nothing,
           if a TIM3 timeout occurs (1 sec),
           then the LED pin will toggle due to
           the TIM3 interrupt */
    }

    return 0;
}
