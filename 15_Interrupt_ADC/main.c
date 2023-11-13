#include "main.h"

int main( void )
{
    /* enable:
       - ADC1 in continuous conversion mode
       - and end of conversion interrupt */
    ADC1_Init();

    /* start ADC1 conversion. This is only called once since ADC1 is
       configured in continuous conversion mode */
    ADC1_Ch0_Start_Conv();

    while( 1 )
    {   
        /* do nothing,
           if an ADC1 end of conversion interrupt occurs,
           then the read value will be printed */
    }

    return 0;
}
