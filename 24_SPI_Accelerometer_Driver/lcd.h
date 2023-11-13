#ifndef LCD_H
#define LCD_H

    #include <stdint.h>
    #include "timer.h"
    #include "i2c.h"

    /* ST7066U registers */
    #define CLEAR_DISPLAY_REG           (0x01U)
    #define RETURN_HOME_REG             (0x02U)
    #define ENTRY_MODE_SET_REG          (0x04U)
    #define DISPLAY_ON_OFF_REG          (0x08U)
    #define CURSOR_DISPLAY_SHIFT_REG    (0x10U)
    #define FUNCTION_SET_REG            (0x20U)
    #define SET_DDRAM_ADDRESS_REG       (0x80U)

    /* ST7066U pin values */
    #define RS_INSTRUCTION              (0x00U)
    #define RS_DATA                     (0x01U)
    #define RW_WRITE                    (0x00U)
    #define RW_READ                     (0x02U)
    #define E_DISABLED                  (0x00U)
    #define E_ENABLED                   (0x04U)
    #define BACKLIGHT_DISABLED          (0x00U)
    #define BACKLIGHT_ENABLED           (0x08U)

    /* Entry mode register parameter values */
    #define SHIFT_DISPLAY_OFF           (0x00U)
    #define SHIFT_DISPLAY_ON            (0x01U)
    #define ID_DDRAM_ADDRESS_LEFT       (0x00U)
    #define ID_DDRAM_ADDRESS_RIGHT      (0x02U)

    /* Display on/off register parameter values */
    #define CURSOR_BLINK_OFF            (0x00U)
    #define CURSOR_BLINK_ON             (0x01U)
    #define CURSOR_OFF                  (0x00U)
    #define CURSOR_ON                   (0x02U)
    #define DISPLAY_OFF                 (0x00U)
    #define DISPLAY_ON                  (0x04U)

    /* Cursor display shift register parameter values */
    #define SC_RL_SHIFT_CURSOR_LEFT     (0x00U)
    #define SC_RL_SHIFT_CURSOR_RIGHT    (0x04U)
    #define SC_RL_SHIFT_DISPLAY_LEFT    (0x08U)
    #define SC_RL_SHIFT_DISPLAY_RIGHT   (0x0CU)

    /* Function set register parameter values */
    #define DISPLAY_FONT_TYPE_5x8       (0x00U)
    #define DISPLAY_FONT_TYPE_5x11      (0x04U)
    #define DISPLAY_1_LINE              (0x00U)
    #define DISPLAY_2_LINES             (0x08U)
    #define INTERFACE_DATA_4_BIT        (0x00U)
    #define INTERFACE_DATA_8_BIT        (0x10U)

    typedef struct
    {   
        uint8_t lcdaddress;
        uint8_t backlight;
        uint8_t shiftdisplay;
        uint8_t incrementddram;
        uint8_t blink;
        uint8_t cursor;
        uint8_t display;
        uint8_t shiftcursor;
        uint8_t font;
        uint8_t displayline;
        uint8_t datalenght;
    } LCD_HandleTypeDef;

    /* High-level functions */
    void LCD_Init( LCD_HandleTypeDef *hlcd );
    void LCD_API_Move_Cursor( LCD_HandleTypeDef *hlcd, uint8_t row, uint8_t col );
    void LCD_API_Print_Character( LCD_HandleTypeDef *hlcd, uint8_t character );
    void LCD_API_Print_String( LCD_HandleTypeDef *hlcd, uint8_t *string, uint8_t size );
    void LCD_API_Shift_Screen_Left( LCD_HandleTypeDef *hlcd, uint8_t positions, uint32_t delay_ms );
    void LCD_API_Shift_Screen_Right( LCD_HandleTypeDef *hlcd, uint8_t positions, uint32_t delay_ms );

    /* Low-level functions */
    void LCD_Start_Data_Write( LCD_HandleTypeDef *hlcd, uint8_t prevData );
    void LCD_Send_Instruction( LCD_HandleTypeDef *hlcd, uint8_t instruction );
    void LCD_Write_To_DDRAM( LCD_HandleTypeDef *hlcd, uint8_t data );
    
#endif
