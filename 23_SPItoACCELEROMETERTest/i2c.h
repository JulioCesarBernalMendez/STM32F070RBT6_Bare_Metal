#ifndef I2C_H
#define I2C_H

    #include "stm32f0xx.h"
    #include "gpio.h"

    #define I2C_CR1_DNF_3           (0x01U << 11)
    #define I2C_CR1_DNF_2           (0x01U << 10)
    #define I2C_CR1_DNF_1           (0x01U << 9)
    #define I2C_CR1_DNF_0           (0x01U << 8)

    #define SLAVE_ADDR_7b_Msk       (0x7FU)
    #define I2C_CR2_SADD_7_1_Pos    (0x01U)

    #define I2C1_CLK_ENBL()         (RCC->APB1ENR |= RCC_APB1ENR_I2C1EN)

    void I2C1_Init( void );
    void I2C1_Write( uint8_t slaveAddr, uint8_t *data, uint8_t size );

#endif
