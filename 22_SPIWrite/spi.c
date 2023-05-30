#include "spi.h"

void SPI1_GPIO_Init( void )
{
    /* enable GPIOA clock access */
    GPIOA_CLK_ENBL();

    /* PA4 (SPI_CE) as output */
    GPIOA->MODER &= ~GPIO_MODER_MODER4_1;
    GPIOA->MODER |=  GPIO_MODER_MODER4_0;

    /* PA5 (SPI1_SCK), PA6 (SPI1_MISO) and PA7 (SPI1_MOSI)
       in alternate function mode */
    GPIOA->MODER |=  GPIO_MODER_MODER5_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER5_0;
    GPIOA->MODER |=  GPIO_MODER_MODER6_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER6_0;
    GPIOA->MODER |=  GPIO_MODER_MODER7_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER7_0;

    /* PA4 (SPI1_CE), PA5 (SPI1_SCK), PA6 (SPI1_MISO) and PA7 (SPI1_MOSI)
       in output push-pull mode */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;

    /* PA5 in AF0 (SPI1_SCK) */
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL5_3;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL5_2;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL5_1;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL5_0;

    /* PA6 in AF0 (SPI1_MISO) */
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL6_3;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL6_2;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL6_1;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL6_0;

    /* PA7 in AF0 (SPI1_MOSI) */
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL7_3;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL7_2;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL7_1;
    GPIOA->AFR[ 0 ] &= ~GPIO_AFRL_AFSEL7_0;
}

void SPI1_Init( void )
{   
    /* enable GPIO resources for SPI1 */
    SPI1_GPIO_Init();
   
    /* enable SPI1 clock access */
    SPI1_CLK_ENBL();

    /* SPI1 2-line unidirectional data mode */
    SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
    
    /* SPI1 CRC disabled */
    SPI1->CR1 &= ~SPI_CR1_CRCEN;

    /* SPI1 in full-duplex mode */
    SPI1->CR1 &= ~SPI_CR1_RXONLY;

    /* SPI1 software slave management disabled */
    SPI1->CR1 &= ~SPI_CR1_SSM;

    /* SPI1 MSB transmitted/received first */
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    /* SPI1 baud rate fPCLK/32.
       With fPCLK = 8 MHz (default),
       therefore SPI1 baud = 250 KHz */
    SPI1->CR1 |=  SPI_CR1_BR_2;
    SPI1->CR1 &= ~SPI_CR1_BR_1;
    SPI1->CR1 &= ~SPI_CR1_BR_0;

    /* SPI1 as master */
    SPI1->CR1 |= SPI_CR1_MSTR;

    /* SPI1 clock is HIGH when idle */
    SPI1->CR1 |= SPI_CR1_CPOL;

    /* SPI1 clock phase, 1st clock transition is the
       1st data capture edge */
    SPI1->CR1 &= ~SPI_CR1_CPHA;

    /* SPI1 RXNE event generated if FIFO >= 8 bits */
    SPI1->CR2 |= SPI_CR2_FRXTH;

    /* SPI1 8 bits Data size */
    SPI1->CR2 &= ~SPI_CR2_DS_3;
    SPI1->CR2 |=  SPI_CR2_DS_2;
    SPI1->CR2 |=  SPI_CR2_DS_1;
    SPI1->CR2 |=  SPI_CR2_DS_0;

    /* SPI1 Motorola mode frame format */
    SPI1->CR2 &= ~SPI_CR2_FRF;

    /* SPI1 no NSS pulse */
    SPI1->CR2 &= ~SPI_CR2_NSSP;

    /* SPI1 SS output enabled */
    SPI1->CR2 |= SPI_CR2_SSOE;
    
    /* Set CS pin to IDLE state (HIGH) */
    SPI1_CS_Disable();

    /* enable SPI1 */
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_CS_Disable( void )
{
    /* Set SPI1 CS pin to IDLE state (HIGH) */
    GPIOA->ODR |= GPIO_ODR_4;
}

void SPI1_CS_Enable( void )
{
    /* Set SPI1 CS pin to LOW state (slave selected) */
    GPIOA->ODR &= ~GPIO_ODR_4;
}

void SPI1_Write( uint8_t *data, uint8_t size )
{
    uint8_t item;

    /* wait for SPI1 bus to be free */
    while ( ( SPI1->SR & SPI_SR_BSY ) == SPI_SR_BSY )
    {
        /* do nothing */
    }
    
    /* loop through the data array */
    for ( item = 0U; item < size; item++ )
    {   
        /* send current byte from the data array */
        *( uint8_t * )( &( SPI1->DR ) ) = data[ item ];

        /* wait for SPI1 transmit buffer to be empty */
        while ( ( SPI1->SR & SPI_SR_TXE ) == 0U )
        {
            /* do nothing */
        }
    }
      
    /* wait for SPI1 bus to be free */
    while ( ( SPI1->SR & SPI_SR_BSY ) == SPI_SR_BSY )
    {
        /* do nothing */
    }
}
