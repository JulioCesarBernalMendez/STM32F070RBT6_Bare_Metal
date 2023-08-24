#include "i2c.h"

void I2C1_Init( void )
{
    /* Enable GPIOB clock access */
    GPIOB_CLK_ENBL();

    /* GPIOB8 (I2C1_SCL) in alternate function mode */
    GPIOB->MODER |=  GPIO_MODER_MODER8_1;
    GPIOB->MODER &= ~GPIO_MODER_MODER8_0;

    /* GPIOB8 (I2C1_SCL) as open-drain output */
    GPIOB->OTYPER |= GPIO_OTYPER_OT_8;

    /* GPIOB8 (I2C1_SCL) with pull-up resistor */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR8_1;
    GPIOB->PUPDR |=  GPIO_PUPDR_PUPDR8_0;

    /* GPIOB8 in AF1 (I2C1_SCL) */
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRL_AFSEL8_3;
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRL_AFSEL8_2;
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRL_AFSEL8_1;
    GPIOB->AFR[ 1 ] |=  GPIO_AFRL_AFSEL8_0;

    /* GPIOB9 (I2C1_SDA) in alternate function mode */
    GPIOB->MODER |=  GPIO_MODER_MODER9_1;
    GPIOB->MODER &= ~GPIO_MODER_MODER9_0;

    /* GPIOB9 (I2C1_SDA) as open-drain output */
    GPIOB->OTYPER |= GPIO_OTYPER_OT_9;

    /* GPIOB9 (I2C1_SDA) with pull-up resistor */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR9_1;
    GPIOB->PUPDR |=  GPIO_PUPDR_PUPDR9_0;

    /* GPIOB9 in AF1 (I2C1_SDA) */
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRL_AFSEL9_3;
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRL_AFSEL9_2;
    GPIOB->AFR[ 1 ] &= ~GPIO_AFRL_AFSEL9_1;
    GPIOB->AFR[ 1 ] |=  GPIO_AFRL_AFSEL9_0;

    /* Enable I2C1 clock access */
    I2C1_CLK_ENBL();

    /* Analog filter enabled */
    I2C1->CR1 &= ~I2C_CR1_ANFOFF;

    /* Select HSI (= 8 MHz) as I2C1 clock source*/
    RCC->CFGR3 &= ~RCC_CFGR3_I2C1SW;

    /* Digital filter disabled */
    I2C1->CR1 &= ~I2C_CR1_DNF_3;
    I2C1->CR1 &= ~I2C_CR1_DNF_2;
    I2C1->CR1 &= ~I2C_CR1_DNF_1;
    I2C1->CR1 &= ~I2C_CR1_DNF_0;

    /* Transfer will be complete after NBYTES data transfer */
    I2C1->CR2 &= ~I2C_CR2_RELOAD;

    /* I2C1 in automatic end mode.
       Master automatically sends a STOP condition once the number
       of bytes in NBYTES has been transferred */
    I2C1->CR2 |= I2C_CR2_AUTOEND;

    /* I2C1 in 7 bit addressing mode */
    I2C1->CR2 &= ~I2C_CR2_ADD10;

    /* Set timing register (I2C Timing Configuration Tool
       for STM32F0xx V1.0.1 was used to get this value)
       PRESC  = 0  (0x00)
       SCLDEL = 2  (0x02)
       SDADEL = 0  (0x00)
       SCLH   = 29 (0x1D)
       SCLL   = 43 (0x2B)
        
       NOTE: I2C1 in standard mode (100KHz) */
    I2C1->TIMINGR = 0x00201D2BU;

    /* Enable I2C1 */
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Write( uint8_t slaveAddr, uint8_t *data, uint8_t size )
{   
    uint8_t byteCtr;
        
    /* Set slave address */
    I2C1->CR2 |= ( slaveAddr & SLAVE_ADDR_7b_Msk ) << I2C_CR2_SADD_7_1_Pos;

    /* Set number of bytes to be sent */
    I2C1->CR2 |= size << I2C_CR2_NBYTES_Pos;

    /* Set read/write bit (0 = write operation) */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
        
    /* Set start bit */
    I2C1->CR2 |= I2C_CR2_START;

    /* After this, if the I2C1 bus is free ...
       master automatically sends start bit + slave address + write bit */

    /* If slave address is correct, then the slave will send an ACK bit */
        
    /* Wait for transmit data register (TXD) to be empty */
    while ( ( ( I2C1->ISR & I2C_ISR_TXE ) != I2C_ISR_TXE ) )
    {
        /* Do nothing */
    }

    /* Send each byte from the data array over I2C1 */
    for ( byteCtr = 0; byteCtr < size; byteCtr++, data++ )
    {   
        /* Transmit current data byte */
        I2C1->TXDR = ( uint32_t ) ( *data );

        /* Wait for transmit data register (TXD) to be sent */
        while ( ( ( I2C1->ISR & I2C_ISR_TXE ) != I2C_ISR_TXE ) )
        {
            /* Do nothing */
        }
    }

    /* Once transmission is complete ...
       master automatically sends a stop bit (due to autoend mode enabled) */
        
    /* Wait for I2C1 bus to be free (stop bit sent) */
    while ( ( I2C1->ISR & I2C_ISR_BUSY ) == I2C_ISR_BUSY )
    {
        /* Do nothing */
    }
}
