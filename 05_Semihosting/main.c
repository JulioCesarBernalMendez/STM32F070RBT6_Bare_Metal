#include <stdint.h>
#include <stdio.h>
#include "stm32f0xx.h"
#include "main.h"
#include "systick.h"

extern void initialise_monitor_handles();

int main( void )
{   
    initialise_monitor_handles();

    char message[] = "This is semihosting!\n\r";

    SysTick_Init();

    while( 1 )
    {
        printf( message );
        Systick_Delay_Ms( 1000 );
    }

    return 0;
}
