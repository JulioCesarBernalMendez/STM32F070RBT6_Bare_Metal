#ifndef MAIN_H
#define MAIN_H

    #include <stdint.h>
    #include <stdio.h>
    #include "stm32f0xx.h"
    #include "systick.h"
    #include "lcd.h"
    #include "spi.h"

    #define LCD_ADDRESS    (0x27U)

    uint8_t *uint16_t_to_string( uint16_t value, uint8_t *dest_str );
    uint8_t *reverse_string( uint8_t *src_string );
    void lcd_print_one_axis_info( LCD_HandleTypeDef *hlcd, float axis );
    void lcd_print_all_axes_info( LCD_HandleTypeDef *hlcd, float x, float y, float z );

#endif
