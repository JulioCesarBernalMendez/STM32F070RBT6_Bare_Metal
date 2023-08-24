#ifndef PRINT_LCD_H
#define PRINT_LCD_H

    #include <stdint.h>
    #include "stm32f0xx.h"
    #include "lcd.h"
    #include "accelerometer.h"

    /* Functions used for displaying data on the LCD */
    uint8_t *uint16_t_to_string( uint16_t value, uint8_t *dest_str );
    uint8_t *reverse_string( uint8_t *src_string );
    void lcd_print_one_axis_info( LCD_HandleTypeDef *hlcd, float axis );
    void lcd_print_all_axes_info( LCD_HandleTypeDef *hlcd, float x, float y, float z );
    void lcd_print_single_tap( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t tap_source, float thresh_tap );
    void lcd_print_double_tap( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t tap_source, float thresh_tap );
    void lcd_print_activity( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t act_source, float thresh_act );
    void lcd_print_inactivity( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t inact_axes, float thresh_inact );
    void lcd_print_freefall( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, float thresh_ff );
    void lcd_print_watermark( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel, uint8_t watermark_samples, uint8_t fifo_entries );
    void lcd_print_overrun( LCD_HandleTypeDef *hlcd, ACCEL_HandleTypeDef *haccel );

#endif
