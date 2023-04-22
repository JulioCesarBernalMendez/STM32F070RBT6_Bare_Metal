#include "lcd.h"

/* lcd parameters (to be replaced by a future structure) */
static uint8_t backlight = BACKLIGHT_ENABLED;

static uint8_t S  = SHIFT_DISPLAY_OFF;
static uint8_t ID = ID_DDRAM_ADDRESS_RIGHT;

static uint8_t B = CURSOR_BLINK_ON;
static uint8_t C = CURSOR_ON;
static uint8_t D = DISPLAY_ON;

static uint8_t SCRL = SC_RL_SHIFT_DISPLAY_LEFT;

static uint8_t F  = DISPLAY_FONT_TYPE_5x8;
static uint8_t N  = DISPLAY_2_LINES;
static uint8_t DL = INTERFACE_DATA_4_BIT;

static uint8_t DDRAMADDR = 0;

static uint8_t byte;

void LCD_Init( uint8_t lcdAddress )
{
    uint8_t i2cByte;

    /* initialize TIM3 */
    TIM3_Init();

    /* inititalize I2C1 */
    I2C1_Init();

    /* wait 41 ms */
    TIM3_Delay_ms( 41U );

    /* function set (this is sent in 8 bit mode)
       DB7 = 0, DB6 = 0, DB5 = 1, DB4 = 1 = ST7066U in 8 bit mode
       backlight = 1 = backlight ON, E = 0, RW = 0, RS = 0
       (not used) DB3 = x = N, DB2 = x = F, DB1 = x, DB0 = x */
    i2cByte = 0x38;
    I2C1_Write( lcdAddress, &i2cByte, 1 );
    LCD_Start_Data_Write( lcdAddress, i2cByte );

    /* 1st try, set ST7066U to 4 bit, 2 lines and 5x8 dots mode */
    DL = INTERFACE_DATA_4_BIT;
    N  = DISPLAY_2_LINES;
    F  = DISPLAY_FONT_TYPE_5x8;
    LCD_Send_Instruction( lcdAddress, FUNCTION_SET_REG );

    /* 2nd try, set ST7066U to 4 bit, 2 lines and 5x8 dots mode */
    DL = INTERFACE_DATA_4_BIT;
    N  = DISPLAY_2_LINES;
    F  = DISPLAY_FONT_TYPE_5x8;
    LCD_Send_Instruction( lcdAddress, FUNCTION_SET_REG );
        
    /* turn on display, enable cursor, disable blink */
    D = DISPLAY_ON;
    C = CURSOR_OFF;
    B = CURSOR_BLINK_OFF;
    LCD_Send_Instruction( lcdAddress, DISPLAY_ON_OFF_REG );

    /* clear display */
    LCD_Send_Instruction( lcdAddress, CLEAR_DISPLAY_REG );

    /* set cursor direction to the right, no shift display */
    ID = ID_DDRAM_ADDRESS_RIGHT;
    S  = SHIFT_DISPLAY_OFF;
    LCD_Send_Instruction( lcdAddress, ENTRY_MODE_SET_REG );
}

void LCD_Start_Data_Write( uint8_t slaveAddress, uint8_t prevData )
{   
    /* enable E signal without touching previous data.
       This tells the ST7066U driver to accept current data on its pins */
    prevData |= E_ENABLED;
    I2C1_Write( slaveAddress, &prevData, 1 );

    /* disable E signal.
       Prepare driver for future commands or data */
    prevData &= ~E_ENABLED;
    I2C1_Write( slaveAddress, &prevData, 1 );
}

void LCD_Send_Instruction( uint8_t lcdAddress, uint8_t instruction )
{
    /* instruction's MOST SIGNIFICANT nibble + light + write instruction */
    byte = ( instruction & 0xF0U ) | backlight | RW_WRITE | RS_INSTRUCTION;

    switch ( instruction )
    {	
        /* add extra parameters (if any) into the 
           instruction's MOST SIGNIFICANT nibble */

        case FUNCTION_SET_REG:
            byte |= DL;
            break;

        case SET_DDRAM_ADDRESS_REG:
            byte |= ( DDRAMADDR & 0x70U );
            break;

        default:
            /* do nothing */
            break;
    }

    /* send MOST SIGNIFICANT nibble + extra parameters over I2C1 */
    I2C1_Write( lcdAddress, &byte, 1 );
    LCD_Start_Data_Write( lcdAddress, byte );

    /* instruction's LEAST SIGNIFICANT nibble + light + write instruction */
    byte = ( instruction << 4 ) | backlight | RW_WRITE | RS_INSTRUCTION;
        
    switch ( instruction )
    {	
        /* add extra parameters (if any) into the 
           instruction's LEAST SIGNIFICANT nibble */

        case ENTRY_MODE_SET_REG:
            byte |= ( ID | S ) << 4;
            break;

        case DISPLAY_ON_OFF_REG:
            byte |= ( D | C | B ) << 4;
            break;

        case CURSOR_DISPLAY_SHIFT_REG:
            byte |= SCRL << 4;
            break;

        case FUNCTION_SET_REG:
            byte |= ( N | F ) << 4;
            break;

        case SET_DDRAM_ADDRESS_REG:
            byte |= ( DDRAMADDR & 0x0FU ) << 4;
            break;

        default:
            /* do nothing */
            break;
    }

    /* send LEAST SIGNIFICANT nibble + extra parameters over I2C1 */
    I2C1_Write( lcdAddress, &byte, 1 );
    LCD_Start_Data_Write( lcdAddress, byte );

    if ( ( instruction == CLEAR_DISPLAY_REG ) || ( instruction == RETURN_HOME_REG ) )
    {   
        /* wait 1.52 ms */
        TIM3_Delay_us( 1520U );
    }
    else
    {
        /* wait 37 us */
        TIM3_Delay_us( 37U );
    }
}

void LCD_Write_To_DDRAM( uint8_t lcdAddress, uint8_t data )
{
    /* data's MSB nible + extra parameters */
    byte = ( data & 0xF0 ) | backlight | RW_WRITE | RS_DATA;
    I2C1_Write( lcdAddress, &byte, 1 );
    LCD_Start_Data_Write( lcdAddress, byte );

    /* data's LSB nible + extra parameters */
    byte = ( data << 4 ) | backlight | RW_WRITE | RS_DATA;
    I2C1_Write( lcdAddress, &byte, 1 );
    LCD_Start_Data_Write( lcdAddress, byte );

    /* wait 37 us */
    TIM3_Delay_us( 37U );
}

void LCD_API_Move_Cursor( uint8_t lcdAddress, uint8_t row, uint8_t col )
{
    if ( row <= 1U && col <= 15 )
    {   
        if ( row == 1 )
        {
            row = 0x40U;
        }

        DDRAMADDR = row + col;
        LCD_Send_Instruction( lcdAddress, SET_DDRAM_ADDRESS_REG );
    }
}

void LCD_API_Print_Character( uint8_t lcdAddress, uint8_t character )
{
    if ( character >= ' ' && character <= '}' )
    {
        LCD_Write_To_DDRAM( lcdAddress, character );
    }
}

void LCD_API_Print_String( uint8_t lcdAddress, uint8_t *string, uint8_t size )
{
    uint8_t i;

    for ( i = 0; i < size; i++ )
    {
        LCD_API_Print_Character( lcdAddress, string[ i ] );
    }
}

void LCD_API_Shift_Screen_Left( uint8_t lcdAddress, uint8_t positions, uint32_t delay_ms )
{
    uint8_t i;

    for ( i = 0; i < positions; i++ )
    {
        SCRL = SC_RL_SHIFT_DISPLAY_LEFT;
        LCD_Send_Instruction( lcdAddress, CURSOR_DISPLAY_SHIFT_REG );
        TIM3_Delay_ms( delay_ms );
    }
}

void LCD_API_Shift_Screen_Right( uint8_t lcdAddress, uint8_t positions, uint32_t delay_ms )
{
    uint8_t i;

    for ( i = 0; i < positions; i++ )
    {
        SCRL = SC_RL_SHIFT_DISPLAY_RIGHT;
        LCD_Send_Instruction( lcdAddress, CURSOR_DISPLAY_SHIFT_REG );
        TIM3_Delay_ms( delay_ms );
    }
}
