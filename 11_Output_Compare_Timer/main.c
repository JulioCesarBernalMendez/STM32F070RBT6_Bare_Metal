#include <stdint.h>
#include <stdio.h>
#include "stm32f0xx.h"
#include "main.h"
#include "timer.h"

int main( void )
{   
    /* initialize TIM15 in output compare mode */
    TIM15_Init();

    while( 1 )
    {   
        /* do nothing */
    }

    return 0;
}
