#ifndef I2C_H
#define I2C_H

    #include "stm32f0xx.h"
    #include "gpio.h"

    #define I2C_CR1_DNF_3           (0x01U << 11)
    #define I2C_CR1_DNF_2           (0x01U << 10)
    #define I2C_CR1_DNF_1           (0x01U << 9)
    #define I2C_CR1_DNF_0           (0x01U << 8)

    #define I2C_CR2_NBYTES_7        (0x01U << 23)
    #define I2C_CR2_NBYTES_6        (0x01U << 22)
    #define I2C_CR2_NBYTES_5        (0x01U << 21)
    #define I2C_CR2_NBYTES_4        (0x01U << 20)
    #define I2C_CR2_NBYTES_3        (0x01U << 19)
    #define I2C_CR2_NBYTES_2        (0x01U << 18)
    #define I2C_CR2_NBYTES_1        (0x01U << 17)
    #define I2C_CR2_NBYTES_0        (0x01U << 16)

    #define SLAVE_ADDR_7b_Msk       (0x7FU)
    #define I2C_CR2_SADD_7_1_Pos    (0x01U)

    #define I2C1_CLK_ENBL()         (RCC->APB1ENR |= RCC_APB1ENR_I2C1EN)

    void I2C1_Init( void );
    void I2C1_Write( uint8_t slaveAddr, uint8_t *data, uint8_t size );

#endif