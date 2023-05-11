#include <stdint.h>
#include "stm32f0xx.h"
#include "main.h"
#include "usart.h"
#include "systick.h"

int main( void )
{
    const uint8_t message[] = "Hello World!!!\n\r";

    USART2_Init();
    SysTick_Init();

    while( 1 )
    {
        USART2_Transmit_Array( message );
        Systick_Delay_Ms( 1000 );
    }

    return 0;
}
