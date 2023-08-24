#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

    #include <stdint.h>
    #include <math.h>
    #include "stm32f0xx.h"
    #include "spi.h"
    #include "timer.h"

    /* ADXL345 registers */
    #define DEVID_REG                     (0x00U)
    #define THRESH_TAP_REG                (0x1DU)
    #define OFSX_REG                      (0x1EU)
    #define OFSY_REG                      (0x1FU)
    #define OFSZ_REG                      (0x20U)
    #define DUR_REG                       (0x21U)
    #define LATENT_REG                    (0x22U)
    #define WINDOW_REG                    (0x23U)
    #define THRESH_ACT_REG                (0x24U)
    #define THRESH_INACT_REG              (0x25U)
    #define TIME_INACT_REG                (0x26U)
    #define ACT_INACT_CTL_REG             (0x27U)
    #define THRESH_FF_REG                 (0x28U)
    #define TIME_FF_REG                   (0x29U)
    #define TAP_AXES_REG                  (0x2AU)
    #define ACT_TAP_STATUS_REG            (0x2BU)
    #define BW_RATE_REG                   (0x2CU)
    #define POWER_CTL_REG                 (0x2DU)
    #define INT_ENABLE_REG                (0x2EU)
    #define INT_MAP_REG                   (0x2FU)
    #define INT_SOURCE_REG                (0x30U)
    #define DATA_FORMAT_REG               (0x31U)
    #define DATAX0_REG                    (0x32U)
    #define DATAX1_REG                    (0x33U)
    #define DATAY0_REG                    (0x34U)
    #define DATAY1_REG                    (0x35U)
    #define DATAZ0_REG                    (0x36U)
    #define DATAZ1_REG                    (0x37U)
    #define FIFO_CTL_REG                  (0x38U)
    #define FIFO_STATUS_REG               (0x39U)

    /* ADXL345 extra bit definitions */
    #define SPI_WRITE                     (0x00U)
    #define SPI_READ                      (0x80U)
    #define MULTIPLE_BYTE_DISABLED        (0x00U)
    #define MULTIPLE_BYTE_ENABLED         (0x40U)

    #define REG_ADDRESS_MASK              (0x3FU)

    /* ADXL345 bit definitions for THRESH_TAP */
    #define THRESH_TAP_BIT_7              (0x80U)
    #define THRESH_TAP_BIT_6              (0x40U)
    #define THRESH_TAP_BIT_5              (0x20U)
    #define THRESH_TAP_BIT_4              (0x10U)
    #define THRESH_TAP_BIT_3              (0x08U)
    #define THRESH_TAP_BIT_2              (0x04U)
    #define THRESH_TAP_BIT_1              (0x02U)
    #define THRESH_TAP_BIT_0              (0x01U)

    /* ADXL345 bit definitions for OFSX */
    #define OFSX_BIT_7                    (0x80U)
    #define OFSX_BIT_6                    (0x40U)
    #define OFSX_BIT_5                    (0x20U)
    #define OFSX_BIT_4                    (0x10U)
    #define OFSX_BIT_3                    (0x08U)
    #define OFSX_BIT_2                    (0x04U)
    #define OFSX_BIT_1                    (0x02U)
    #define OFSX_BIT_0                    (0x01U)

    /* ADXL345 bit definitions for OFSY */
    #define OFSY_BIT_7                    (0x80U)
    #define OFSY_BIT_6                    (0x40U)
    #define OFSY_BIT_5                    (0x20U)
    #define OFSY_BIT_4                    (0x10U)
    #define OFSY_BIT_3                    (0x08U)
    #define OFSY_BIT_2                    (0x04U)
    #define OFSY_BIT_1                    (0x02U)
    #define OFSY_BIT_0                    (0x01U)

    /* ADXL345 bit definitions for OFSZ */
    #define OFSZ_BIT_7                    (0x80U)
    #define OFSZ_BIT_6                    (0x40U)
    #define OFSZ_BIT_5                    (0x20U)
    #define OFSZ_BIT_4                    (0x10U)
    #define OFSZ_BIT_3                    (0x08U)
    #define OFSZ_BIT_2                    (0x04U)
    #define OFSZ_BIT_1                    (0x02U)
    #define OFSZ_BIT_0                    (0x01U)

    /* ADXL345 bit definitions for DUR */
    #define DUR_BIT_7                     (0x80U)
    #define DUR_BIT_6                     (0x40U)
    #define DUR_BIT_5                     (0x20U)
    #define DUR_BIT_4                     (0x10U)
    #define DUR_BIT_3                     (0x08U)
    #define DUR_BIT_2                     (0x04U)
    #define DUR_BIT_1                     (0x02U)
    #define DUR_BIT_0                     (0x01U)

    /* ADXL345 bit definitions for LATENT */
    #define LATENT_BIT_7                  (0x80U)
    #define LATENT_BIT_6                  (0x40U)
    #define LATENT_BIT_5                  (0x20U)
    #define LATENT_BIT_4                  (0x10U)
    #define LATENT_BIT_3                  (0x08U)
    #define LATENT_BIT_2                  (0x04U)
    #define LATENT_BIT_1                  (0x02U)
    #define LATENT_BIT_0                  (0x01U)

    /* ADXL345 bit definitions for WINDOW */
    #define WINDOW_BIT_7                  (0x80U)
    #define WINDOW_BIT_6                  (0x40U)
    #define WINDOW_BIT_5                  (0x20U)
    #define WINDOW_BIT_4                  (0x10U)
    #define WINDOW_BIT_3                  (0x08U)
    #define WINDOW_BIT_2                  (0x04U)
    #define WINDOW_BIT_1                  (0x02U)
    #define WINDOW_BIT_0                  (0x01U)

    /* ADXL345 bit definitions for THRESH_ACT */
    #define THRESH_ACT_BIT_7              (0x80U)
    #define THRESH_ACT_BIT_6              (0x40U)
    #define THRESH_ACT_BIT_5              (0x20U)
    #define THRESH_ACT_BIT_4              (0x10U)
    #define THRESH_ACT_BIT_3              (0x08U)
    #define THRESH_ACT_BIT_2              (0x04U)
    #define THRESH_ACT_BIT_1              (0x02U)
    #define THRESH_ACT_BIT_0              (0x01U)

    /* ADXL345 bit definitions for THRESH_INACT */
    #define THRESH_INACT_BIT_7            (0x80U)
    #define THRESH_INACT_BIT_6            (0x40U)
    #define THRESH_INACT_BIT_5            (0x20U)
    #define THRESH_INACT_BIT_4            (0x10U)
    #define THRESH_INACT_BIT_3            (0x08U)
    #define THRESH_INACT_BIT_2            (0x04U)
    #define THRESH_INACT_BIT_1            (0x02U)
    #define THRESH_INACT_BIT_0            (0x01U)

    /* ADXL345 bit definitions for TIME_INACT */
    #define TIME_INACT_BIT_7              (0x80U)
    #define TIME_INACT_BIT_6              (0x40U)
    #define TIME_INACT_BIT_5              (0x20U)
    #define TIME_INACT_BIT_4              (0x10U)
    #define TIME_INACT_BIT_3              (0x08U)
    #define TIME_INACT_BIT_2              (0x04U)
    #define TIME_INACT_BIT_1              (0x02U)
    #define TIME_INACT_BIT_0              (0x01U)

    /* ADXL345 bit definitions for ACT_INACT_CTL */
    #define ACT_DC_COUPLED                (0x00U)
    #define ACT_AC_COUPLED                (0x80U)
    #define ACT_X_DISABLED                (0x00U)
    #define ACT_X_ENABLED                 (0x40U)
    #define ACT_Y_DISABLED                (0x00U)
    #define ACT_Y_ENABLED                 (0x20U)
    #define ACT_Z_DISABLED                (0x00U)
    #define ACT_Z_ENABLED                 (0x10U)
    #define INACT_DC_COUPLED              (0x00U)
    #define INACT_AC_COUPLED              (0x08U)
    #define INACT_X_DISABLED              (0x00U)
    #define INACT_X_ENABLED               (0x04U)
    #define INACT_Y_DISABLED              (0x00U)
    #define INACT_Y_ENABLED               (0x02U)
    #define INACT_Z_DISABLED              (0x00U)
    #define INACT_Z_ENABLED               (0x01U)

    /* ADXL345 bit definitions for THRESH_FF */
    #define THRESH_FF_BIT_7               (0x80U)
    #define THRESH_FF_BIT_6               (0x40U)
    #define THRESH_FF_BIT_5               (0x20U)
    #define THRESH_FF_BIT_4               (0x10U)
    #define THRESH_FF_BIT_3               (0x08U)
    #define THRESH_FF_BIT_2               (0x04U)
    #define THRESH_FF_BIT_1               (0x02U)
    #define THRESH_FF_BIT_0               (0x01U)

    /* ADXL345 bit definitions for TIME_FF */
    #define TIME_FF_BIT_7                 (0x80U)
    #define TIME_FF_BIT_6                 (0x40U)
    #define TIME_FF_BIT_5                 (0x20U)
    #define TIME_FF_BIT_4                 (0x10U)
    #define TIME_FF_BIT_3                 (0x08U)
    #define TIME_FF_BIT_2                 (0x04U)
    #define TIME_FF_BIT_1                 (0x02U)
    #define TIME_FF_BIT_0                 (0x01U)

    /* ADXL345 bit definitions for TAP_AXES */
    #define SUPPRESS_DISABLED             (0x00U)
    #define SUPPRESS_ENABLED              (0x08U)
    #define TAP_X_DISABLED                (0x00U)
    #define TAP_X_ENABLED                 (0x04U)
    #define TAP_Y_DISABLED                (0x00U)
    #define TAP_Y_ENABLED                 (0x02U)
    #define TAP_Z_DISABLED                (0x00U)
    #define TAP_Z_ENABLED                 (0x01U)

    /* ADXL345 bit definitions for ACT_TAP_STATUS */
    #define ACT_X_SOURCE                  (0x40U)
    #define ACT_Y_SOURCE                  (0x20U)
    #define ACT_Z_SOURCE                  (0x10U)
    #define ASLEEP_NOT_ASLEEP             (0x00U)
    #define ASLEEP_ASLEEP                 (0x08U)
    #define TAP_X_SOURCE                  (0x04U)
    #define TAP_Y_SOURCE                  (0x02U)
    #define TAP_Z_SOURCE                  (0x01U)

    /* ADXL345 bit definitions for BW_RATE */
    #define LOW_POWER_DISABLED            (0x00U)
    #define LOW_POWER_ENABLED             (0x10U)
    #define RATE_BIT_3                    (0x08U)
    #define RATE_BIT_2                    (0x04U)
    #define RATE_BIT_1                    (0x02U)
    #define RATE_BIT_0                    (0x01U)
    #define RATE_3200_HZ                  (0x0FU)
    #define RATE_1600_HZ                  (0x0EU)
    #define RATE_800_HZ                   (0x0DU)
    #define RATE_400_HZ                   (0x0CU)
    #define RATE_200_HZ                   (0x0BU)
    #define RATE_100_HZ                   (0x0AU)
    #define RATE_50_HZ                    (0x09U)
    #define RATE_25_HZ                    (0x08U)
    #define RATE_12_5_HZ                  (0x07U)
    #define RATE_6_25_HZ                  (0x06U)
    #define RATE_3_13_HZ                  (0x05U)
    #define RATE_1_56_HZ                  (0x04U)
    #define RATE_0_78_HZ                  (0x03U)
    #define RATE_0_39_HZ                  (0x02U)
    #define RATE_0_20_HZ                  (0x01U)
    #define RATE_0_10_HZ                  (0x00U)
    #define RATE_BITS_MASK                (0x0FU)

    /* ADXL345 bit definitions for POWER_CTL */
    #define LINK_INACT_ACT_CONCURRENT     (0x00U)
    #define LINK_DELAY_ACT_UNTIL_INACT    (0x20U)
    #define AUTO_SLEEP_DISABLED           (0x00U)
    #define AUTO_SLEEP_ENABLED            (0x10U)
    #define MEASURE_STANDBY               (0x00U)
    #define MEASURE_ENABLED               (0x08U)
    #define SLEEP_DISABLED                (0x00U)
    #define SLEEP_ENABLED                 (0x04U)
    #define WAKEUP_BIT_1                  (0x02U)
    #define WAKEUP_BIT_0                  (0x01U)
    #define WAKEUP_8HZ                    (0x00U)
    #define WAKEUP_4HZ                    (0x01U)
    #define WAKEUP_2HZ                    (0x02U)
    #define WAKEUP_1HZ                    (0x03U)
    #define WAKEUP_BITS_MASK              (0x03U)

    /* ADXL345 bit definitions for INT_ENABLE */
    #define DATA_READY_INT_DISABLED       (0x00U)
    #define DATA_READY_INT_ENABLED        (0x80U)
    #define SINGLE_TAP_INT_DISABLED       (0x00U)
    #define SINGLE_TAP_INT_ENABLED        (0x40U)
    #define DOUBLE_TAP_INT_DISABLED       (0x00U)
    #define DOUBLE_TAP_INT_ENABLED        (0x20U)
    #define ACTIVITY_INT_DISABLED         (0x00U)
    #define ACTIVITY_INT_ENABLED          (0x10U)
    #define INACTIVITY_INT_DISABLED       (0x00U)
    #define INACTIVITY_INT_ENABLED        (0x08U)
    #define FREE_FALL_INT_DISABLED        (0x00U)
    #define FREE_FALL_INT_ENABLED         (0x04U)
    #define WATERMARK_INT_DISABLED        (0x00U)
    #define WATERMARK_INT_ENABLED         (0x02U)
    #define OVERRUN_INT_DISABLED          (0x00U)
    #define OVERRUN_INT_ENABLED           (0x01U)
    #define ALL_INTERRUPTS_ENABLED        (0xFFU)
    #define ALL_INTERRUPTS_DISABLED       (0x00U)

    /* ADXL345 bit definitions for INT_MAP */
    #define DATA_READY_INT_MAP_INT1       (0x00U)
    #define DATA_READY_INT_MAP_INT2       (0x80U)
    #define SINGLE_TAP_INT_MAP_INT1       (0x00U)
    #define SINGLE_TAP_INT_MAP_INT2       (0x40U)
    #define DOUBLE_TAP_INT_MAP_INT1       (0x00U)
    #define DOUBLE_TAP_INT_MAP_INT2       (0x20U)
    #define ACTIVITY_INT_MAP_INT1         (0x00U)
    #define ACTIVITY_INT_MAP_INT2         (0x10U)
    #define INACTIVITY_INT_MAP_INT1       (0x00U)
    #define INACTIVITY_INT_MAP_INT2       (0x08U)
    #define FREE_FALL_INT_MAP_INT1        (0x00U)
    #define FREE_FALL_INT_MAP_INT2        (0x04U)
    #define WATERMARK_INT_MAP_INT1        (0x00U)
    #define WATERMARK_INT_MAP_INT2        (0x02U)
    #define OVERRUN_INT_MAP_INT1          (0x00U)
    #define OVERRUN_INT_MAP_INT2          (0x01U)

    /* ADXL345 bit definitions for INT_SOURCE */
    #define DATA_READY_TRIGGERED          (0x80U)
    #define SINGLE_TAP_TRIGGERED          (0x40U)
    #define DOUBLE_TAP_TRIGGERED          (0x20U)
    #define ACTIVITY_TRIGGERED            (0x10U)
    #define INACTIVITY_TRIGGERED          (0x08U)
    #define FREE_FALL_TRIGGERED           (0x04U)
    #define WATERMARK_TRIGGERED           (0x02U)
    #define OVERRUN_TRIGGERED             (0x01U)

    /* ADXL345 bit definitions for DATA_FORMAT */
    #define SELF_TEST_DISABLED            (0x00U)
    #define SELF_TEST_ENABLED             (0x80U)
    #define SPI_4WIRE_MODE                (0x00U)
    #define SPI_3WIRE_MODE                (0x40U)
    #define INT_INVERT_ACTIVE_HIGH        (0x00U)
    #define INT_INVERT_ACTIVE_LOW         (0x20U)
    #define FULL_RES_10BIT_MODE           (0x00U)
    #define FULL_RES_ENABLED              (0x08U)
    #define JUSTIFY_RIGHT                 (0x00U)
    #define JUSTIFY_LEFT                  (0x04U)
    #define RANGE_BIT_1                   (0x02U)
    #define RANGE_BIT_0                   (0x01U)
    #define RANGE_2G                      (0x00U)
    #define RANGE_4G                      (0x01U)
    #define RANGE_8G                      (0x02U)
    #define RANGE_16G                     (0x03U)

    /* ADXL345 bit definitions for FIFO_CTL */
    #define FIFO_BYPASS_MODE              (0x00U)
    #define FIFO_FIFO_MODE                (0x40U)
    #define FIFO_STREAM_MODE              (0x80U)
    #define FIFO_TRIGGER_MODE             (0xC0U)
    #define FIFO_MODE_BITS_MASK           (0xC0U)
    #define TRIGGER_EVENT_INT1            (0x00U)
    #define TRIGGER_EVENT_INT2            (0x20U)
    #define SAMPLES_BIT_4                 (0x10U)
    #define SAMPLES_BIT_3                 (0x08U)
    #define SAMPLES_BIT_2                 (0x04U)
    #define SAMPLES_BIT_1                 (0x02U)
    #define SAMPLES_BIT_0                 (0x01U)
    #define SAMPLES_BITS_MASK             (0x1FU)

    /* ADXL345 bit definitions for FIFO_STATUS */
    #define FIFO_TRIG_NO_OCCURRING        (0x00U)
    #define FIFO_TRIG_OCCURRING           (0x80U)
    #define ENTRIES_BIT_5                 (0x20U)
    #define ENTRIES_BIT_4                 (0x10U)
    #define ENTRIES_BIT_3                 (0x08U)
    #define ENTRIES_BIT_2                 (0x04U)
    #define ENTRIES_BIT_1                 (0x02U)
    #define ENTRIES_BIT_0                 (0x01U)
    #define ENTRIES_BITS_MASK             (0x3FU)

    typedef struct
    {
        uint8_t devid;
        uint8_t threshtap;
        uint8_t ofsx;
        uint8_t ofsy;
        uint8_t ofsz;
        uint8_t dur;
        uint8_t latent;
        uint8_t window;
        uint8_t threshact;
        uint8_t threshinact;
        uint8_t timeinact;
        uint8_t actinactctl;
        uint8_t threshff;
        uint8_t timeff;
        uint8_t tapaxes;
        uint8_t bwrate;
        uint8_t powerctl;
        uint8_t intenable;
        uint8_t intmap;
        uint8_t dataformat;
        uint8_t fifoctl;
    } ACCEL_HandleTypeDef;

    /* low-level functions */
    void ADXL345_Write( uint8_t reg_addr, uint8_t data );
    uint8_t ADXL345_Read( uint8_t reg_addr );

    /* high-level functions */
    void ADXL345_Init( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Read_Devid( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Standby_Mode( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Measurement_Mode( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Low_Power_Mode( ACCEL_HandleTypeDef *haccel, uint8_t output_rate );
    void ADXL345_API_Normal_Power_Mode( ACCEL_HandleTypeDef *haccel, uint8_t output_rate );
    void ADXL345_API_Sleep_Mode( ACCEL_HandleTypeDef *haccel, uint8_t wakeup_rate );
    void ADXL345_API_Wake_Up( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Auto_Sleep_Mode( ACCEL_HandleTypeDef *haccel, uint8_t wakeup_rate );
    void ADXL345_API_Link_Mode( ACCEL_HandleTypeDef *haccel, uint8_t autosleep_mode, uint8_t wakeup_rate );
    void ADXL345_API_Manual_OffsetCal( ACCEL_HandleTypeDef *haccel, int16_t *x0g, int16_t *y0g, int16_t *z0g );
    void ADXL345_API_Auto_OffsetCal( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Fifo_Bypass_Mode( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Fifo_Fifo_Mode( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Fifo_Stream_Mode( ACCEL_HandleTypeDef *haccel );
    void ADXL345_API_Fifo_Trigger_Mode( ACCEL_HandleTypeDef *haccel, uint8_t interrupts, uint8_t trigger_pin, uint8_t samples );
    void ADXL345_API_Read_Axes( ACCEL_HandleTypeDef *haccel, int16_t *x_read, int16_t *y_read, int16_t *z_read );
    float *ADXL345_API_Scale_Axes( ACCEL_HandleTypeDef *haccel, int16_t x_raw, int16_t y_raw, int16_t z_raw );
    void ADXL345_API_Self_Test( ACCEL_HandleTypeDef *haccel, int16_t *x_st, int16_t *y_st, int16_t *z_st );
    void ADXL345_API_Enable_Int( ACCEL_HandleTypeDef *haccel, uint8_t int_enable, uint8_t int_map );
    void ADXL345_API_Disable_Int( ACCEL_HandleTypeDef *haccel, uint8_t interrupts );
    uint8_t ADXL345_API_Read_Int_Source( ACCEL_HandleTypeDef *haccel );
    uint8_t ADXL345_API_Read_Tap_Source( ACCEL_HandleTypeDef *haccel );
    uint8_t ADXL345_API_Read_Activity_Source( ACCEL_HandleTypeDef *haccel );
    uint8_t ADXL345_API_Read_FIFO_Entries( ACCEL_HandleTypeDef *haccel );

#endif
