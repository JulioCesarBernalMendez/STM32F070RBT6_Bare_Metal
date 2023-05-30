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
    SPI1->CR1 &= SPI_CR1_SSM;

    /* SPI1 Internal Slave select (this bit only has effect when SSM = 1).
       The value of this bit is forced onto the NSS pin and
       the I/O value of the NSS pin is ignored */
    /* SPI1->CR1 |= SPI_CR1_SSI; */

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
    SPI1->CR1 |= SPI_CR1_CPHA;

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

    /* enable SPI1 RX interrupt */
    /* SPI1_RX_Interrupt_Init(); */

    /* Set CS pin to IDLE state (HIGH) */
    SPI1_CS_Disable();

    /* enable SPI1 */
    SPI1->CR1 |= SPI_CR1_SPE;
}

#if 0
void SPI1_RX_Interrupt_Init( void )
{
    /* disable interrupts */
    __disable_irq();

    /* SPI1 RXNE event generated if FIFO >= 8 bits */
    SPI1->CR2 |= SPI_CR2_FRXTH;

    /* SPI1 RX buffer not empty interrupt enabled */
    SPI1->CR2 |= SPI_CR2_RXNEIE;

    /* enable interrupt 25 (SPI1 global interrupt) in NVIC.
       Another method to do this is by using:
    NVIC_EnableIRQ( SPI1_IRQn ) */
    NVIC->ISER[ 0 ] |= SPI1_ENABLE_IRQ;

    /* set interrupt SPI1 (N=25) priority to zero (highest priority).
       ISPRx. With x = N / 4 = 25 / 4, then x = 6. Therefore ISPRx = ISPR6.
       Byte offset = N MOD 4 = 25 MOD 4, therefore Byte offset = 1.
       NOTE: processor implements only bits 7 and 6 of each field.
       Another method to do this is by using:
    NVIC_SetPriority( SPI1_IRQn, 0U ); */
    NVIC->IP[ 6 ] &= ~NVIC_IPR_PRI25_7;
    NVIC->IP[ 6 ] &= ~NVIC_IPR_PRI25_6;

    /* enable interrupts */
    __enable_irq();
}
#endif

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
    uint8_t temp;

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

        /* NOTE: SPI1 is configured in 2-line unidirectional and 
                 full duplex mode, therefore for each data sent over MISO,
                 data received in the MISO pin is sampled every clock cycle,
                 meaning that we send and receive data at the same time
                 although that may not be our intention. */

        /* wait for RX buffer to receive 1 byte data (FRXTH = 1) */
        while ( ( SPI1->SR & SPI_SR_RXNE ) == 0U )
        {
            /* do nothing */
        }

        /* store data read on MISO (this data will be ignored and not used)
            in order to clear the RX buffer. Also by reading data the 
            RXNE flag (receive buffer not empty) is automatically cleared */
        temp = ( uint8_t )( SPI1->DR );
    }
      
    /* wait for SPI1 bus to be free */
    while ( ( SPI1->SR & SPI_SR_BSY ) == SPI_SR_BSY )
    {
        /* do nothing */
    }

    /* clear the overrun flag just in case the buffer is full and 
       attempt to store more receiving data on MISO is made */
    SPI1->SR &= ~SPI_SR_OVR;
}

void SPI1_Read( uint8_t *read, uint8_t size )
{   
    uint8_t item;

    /* NOTE: SPI1 is configured in 2-line unidirectional and 
             full duplex mode, therefore for each data sent over MISO,
             data received in the MISO pin is sampled every clock cycle,
             meaning that we send and receive data at the same time
             although that may not be our intention. */
    
    /* store each data byte into the array */
    for ( item = 0; item < size; item++, read++ )
    {
        /* send dummy data (this allows us to receive data on MISO) */
        *( uint8_t * )( &( SPI1->DR ) ) = 0U;

        /* wait for RX buffer to receive data */
        while ( ( SPI1->SR & SPI_SR_RXNE ) == 0U )
        {
            /* do nothing */
        }

        /* store received byte (this also clears
        the SPI1 RX buffer not empty flag) */
        *read = ( uint8_t )( SPI1->DR );
    }
}
