#ifndef MAIN_H
#define MAIN_H

   #include <stdint.h>
   #include "stm32f0xx.h"
   #include "systick.h"
   #include "lcd.h"
   #include "accelerometer.h"
   #include "print_lcd.h"

   /* LCD slave device address */
   #define LCD_ADDRESS             (0x27U)

   /* Uncomment for selecting the desired calibration method.
      Leave commented for automatic calibration (offset registers) */
   /* #define NO_CALIBRATION     */
   #define MANUAL_CALIBRATION

   /* The following allows enabling of the ADXL345 interrupts.
      Uncomment to enable. Comment to disable.
      NOTES: - This program forces only one interrupt (apart from DATA_READY) to be enabled at a time and is mapped to INT2 pin
             - The DATA_READY interrupt is enabled and mapped to INT1 pin by default since it is needded for data acquisition */
   /* #define ADXL345_SINGLE_TAP_INTERRUPT */
   /* #define ADXL345_DOUBLE_TAP_INTERRUPT */
   /* #define ADXL345_ACTIVITY_INTERRUPT   */
   /* #define ADXL345_INACTIVITY_INTERRUPT */
   /* #define ADXL345_FREE_FALL_INTERRUPT  */
   /* #define ADXL345_WATERMARK_INTERRUPT  */
   /* #define ADXL345_OVERRUN_INTERRUPT    */

   /* States of the main state machine */
   #define IDLE                    (0x00U)
   #define MANUAL_OFFSET           (0x01U)
   #define SCALE_AXES              (0x02U)
   #define PRINT_AXES              (0x03U)
   #define PRINT_SINGLE_TAP        (0x04U)
   #define PRINT_DOUBLE_TAP        (0x05U)
   #define PRINT_ACTIVITY          (0x06U)
   #define PRINT_INACTIVITY        (0x07U)
   #define PRINT_FREE_FALL         (0x08U)
   #define PRINT_WATERMARK         (0x09U)
   #define PRINT_OVERRUN           (0x0AU)
   #define DISABLE_INTERRUPTS      (0x0BU)
   #define ENABLE_INTERRUPTS       (0x0CU)
   #define CLEAR_INT2_FLAGS        (0x0DU)
   #define EVALUATE_INT2           (0x0EU)
   #define GET_TAP_AXIS            (0x0FU)
   #define GET_ACTIVITY_AXIS       (0x10U)
   #define READ_FIFO_ENTRIES       (0x11U)
   #define READ_INT2_PIN           (0x12U)
   #define SET_STANDBY_MODE        (0x13U)
   #define SET_MEASUREMENT_MODE    (0x14U)

   /* state machine's functions */
   void idle( uint8_t *curr_state );
   void manual_offset( uint8_t *curr_state );
   void scale_axes( uint8_t *curr_state );
   void print_axes( uint8_t *curr_state );
   void print_single_tap( uint8_t *curr_state, uint32_t *int2_clk, uint8_t *tap_axis );
   void print_double_tap( uint8_t *curr_state, uint32_t *int2_clk, uint8_t *tap_axis );
   void print_activity( uint8_t *curr_state, uint32_t *int2_clk, uint8_t *activity_axis );
   void print_inactivity( uint8_t *curr_state, uint32_t *int2_clk );
   void print_free_fall( uint8_t *curr_state, uint32_t *int2_clk );
   void print_watermark( uint8_t *curr_state, uint8_t *entries );
   void print_overrun( uint8_t *curr_state );
   void disable_interrupts( uint8_t *curr_state );
   void enable_interrupts( uint8_t *curr_state, uint8_t *interrupts );
   void clear_int2_flags( uint8_t *curr_state, uint32_t *int2_clk );
   void evaluate_int2( uint8_t *curr_state, uint8_t *interrupts, uint8_t *int_source );
   void get_tap_axis( uint8_t *curr_state, uint8_t *tap_axis, uint8_t *int_source );
   void get_activity_axis( uint8_t *curr_state, uint8_t *activity_axis );
   void read_fifo_entries( uint8_t *curr_state, uint8_t *entries );
   void read_int2_pin( uint8_t *curr_state );
   void set_standby_mode( uint8_t *curr_state );
   void set_measurement_mode( uint8_t *curr_state );

#endif
