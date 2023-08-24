#ifndef SPI_H
#define SPI_H

    #include <stdint.h>
    #include "stm32f0xx.h"
    #include "gpio.h"
    
    #define SPI1_CLK_ENBL()        (RCC->APB2ENR |= RCC_APB2ENR_SPI1EN)
    
    #define NVIC_ISER_SETENA_25    (0x01U << 25)
    #define SPI1_ENABLE_IRQ        NVIC_ISER_SETENA_25

    #define NVIC_IPR_PRI25_7       (0x01U << 15)
    #define NVIC_IPR_PRI25_6       (0x01U << 14)
    #define NVIC_IPR_PRI25_5       (0x01U << 13)
    #define NVIC_IPR_PRI25_4       (0x01U << 12)
    #define NVIC_IPR_PRI25_3       (0x01U << 11)
    #define NVIC_IPR_PRI25_2       (0x01U << 10)
    #define NVIC_IPR_PRI25_1       (0x01U << 9)
    #define NVIC_IPR_PRI25_0       (0x01U << 8)

    void SPI1_GPIO_Init( void );
    void SPI1_Init( void );
    void SPI1_RX_Interrupt_Init( void );
    void SPI1_CS_Disable( void );
    void SPI1_CS_Enable( void );
    void SPI1_Write( uint8_t *data, uint8_t size );
    void SPI1_Read( uint8_t *read, uint8_t size );

#endif
