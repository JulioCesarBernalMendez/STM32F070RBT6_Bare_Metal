#include <stdint.h>
#include <stdio.h>
#include "stm32f0xx.h"
#include "main.h"
#include "usart.h"

extern void initialise_monitor_handles();

int main( void )
{
    uint8_t data;

    USART2_Init();
    initialise_monitor_handles();

    while( 1 )
    {
        data = USART2_Receive_Character();
        printf( "%c\n", data );
        data = data; /* dummy line to set a breakpoint */
    }

    return 0;
}
