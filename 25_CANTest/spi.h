#ifndef SPI_H
#define SPI_H

    #include <stdint.h>
    #include "stm32f0xx.h"
    #include "gpio.h"
    
    /* --------------------------------- SPI1 --------------------------------- */
    #define SPI1_CLK_ENBL()    (RCC->APB2ENR |= RCC_APB2ENR_SPI1EN)

    void SPI1_GPIO_Init( void );
    void SPI1_Init( void );
    void SPI1_Write( uint8_t *data, uint8_t size );
    void SPI1_Read( uint8_t *read, uint8_t size );

    void SPI1_CS_Disable( void );
    void SPI1_CS_Enable( void );

    /* --------------------------------- SPI2 --------------------------------- */
    #define SPI2_CLK_ENBL()    (RCC->APB1ENR |= RCC_APB1ENR_SPI2EN)

    void SPI2_GPIO_Init( void );
    void SPI2_Init( void );
    void SPI2_Write( uint8_t *data, uint8_t size );
    void SPI2_Read( uint8_t *read, uint8_t size );

    void SPI2_CS_Disable( void );
    void SPI2_CS_Enable( void );

#endif