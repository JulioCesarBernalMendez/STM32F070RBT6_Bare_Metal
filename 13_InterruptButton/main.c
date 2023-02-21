#include "main.h"

int main( void )
{   
    /* initialize BOARD BUTTON*/
    GPIOA_Init();

    /* initialize BOARD LED as output
       and interrupt line for both
       rising and falling edge */
    GPIOC_Init();

    while( 1 )
    {   
        /* do nothing */
    }

    return 0;
}
