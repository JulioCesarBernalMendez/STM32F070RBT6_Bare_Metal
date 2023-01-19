#include <stdint.h>

#define RCC_BASE_ADDR        0x40021000
#define AHBENR               0x00000014
#define IOPA_EN_BIT          ( 1 << 17 )
#define GPIOA_CLK_ENBL()     ( *( unsigned int* )( RCC_BASE_ADDR + AHBENR ) |= IOPA_EN_BIT )

#define GPIOA_BASE_ADDR      0x48000000
#define GPIOx_MODER          0x00000000
#define GPIOx_OTYPER         0x00000004
#define GPIOx_OSPEEDR        0x00000008
#define GPIOx_ODR            0x00000014

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
    *( uint32_t* )( GPIOA_BASE_ADDR + GPIOx_MODER ) &= ~( 1 << 11 );
    *( uint32_t* )( GPIOA_BASE_ADDR + GPIOx_MODER ) |=  ( 1 << 10 );

    /* GPIOA5 as push-pull output */
    *( uint32_t* )( GPIOA_BASE_ADDR + GPIOx_OTYPER ) &= ~( 1 << 5 );

    /* GPIOA5 as low speed output */
    *( uint32_t* )( GPIOA_BASE_ADDR + GPIOx_OSPEEDR ) &= ~( 1 << 10 );
}

void toggle_led( void )
{
    *( uint32_t* )( GPIOA_BASE_ADDR + GPIOx_ODR ) ^= ( 1 << 5 );
}