#include <stdint.h>

#define PERIPH_BASE            ( 0x40000000UL )
 
#define AHB1_PERIPH_OFFSET     ( 0x00020000UL )
#define AHB1_PERIPH_BASE       ( PERIPH_BASE + AHB1_PERIPH_OFFSET )

#define RCC_PERIPH_OFFSET      ( 0x00001000UL )
#define RCC_PERIPH_BASE        ( AHB1_PERIPH_BASE + RCC_PERIPH_OFFSET )

#define RCC_AHBEN_R_OFFSET     ( 0x00000014UL )
#define RCC_AHBEN_R            ( *( volatile uint32_t* )( RCC_PERIPH_BASE + RCC_AHBEN_R_OFFSET ) )

#define GPIOA_PERIPH_OFFSET    ( 0x08000000UL )
#define GPIOA_PERIPH_BASE      ( PERIPH_BASE + GPIOA_PERIPH_OFFSET )

#define GPIOx_MODE_R_OFFSET    ( 0x00000000UL )
#define GPIOA_MODE_R           ( *( volatile uint32_t* )( GPIOA_PERIPH_BASE + GPIOx_MODE_R_OFFSET ) )

#define GPIOx_OD_R_OFFSET      ( 0x00000014UL )
#define GPIOA_ODR_R            ( *( volatile uint32_t* )( GPIOA_PERIPH_BASE + GPIOx_OD_R_OFFSET ) )

#define GPIOA_EN_BIT           ( 0x01U << 17 )
#define GPIOA_CLK_ENBL()       ( RCC_AHBEN_R |= GPIOA_EN_BIT )

#define GPIOx_PIN_5            ( 0x01U << 5 )
#define LED_PIN                GPIOx_PIN_5

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
    GPIOA_MODE_R &= ~( 0x01U << 11 );
    GPIOA_MODE_R |=  ( 0x01U << 10 );

    /* GPIOA5 as push-pull output upon reset */

    /* GPIOA5 as low speed output upon reset */
}

void toggle_led( void )
{
    GPIOA_ODR_R ^= ( 1 << 5 );
}
