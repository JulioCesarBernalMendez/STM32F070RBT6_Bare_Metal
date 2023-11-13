#include "lcd.h"

static uint8_t byte;
static uint8_t DDRAMADDR = 0U;

void LCD_Init( LCD_HandleTypeDef *hlcd )
{
    uint8_t i2cByte;

    /* Initialize TIM3 */
    TIM3_Init();

    /* Inititalize I2C1 */
    I2C1_Init();

    /* Wait 40 ms */
    TIM3_Delay_ms( 40U );

    /* Function set (this is sent in 8 bit mode)
       DB7 = 0, DB6 = 0, DB5 = 1, DB4 = 1 = ST7066U in 8 bit mode
       backlight = backlight ON = 1, E = 0, RW = 0, RS = 0
       (not used) DB3 = N = x, DB2 = F = x, DB1 = x, DB0 = x */
    i2cByte = 0x38U;
    I2C1_Write( hlcd->lcdaddress, &i2cByte, 1U );
    LCD_Start_Data_Write( hlcd, i2cByte );

    /* Wait 37us */
    TIM3_Delay_us( 37U ),

    /* 1st try, set ST7066U to (8/4) bit datalength, (1/2) display line
       and (5x8/5x11) dots font mode */
    LCD_Send_Instruction( hlcd, FUNCTION_SET_REG );

    /* 2nd try, set ST7066U to (8/4) bit datalength, (1/2) display line
       and (5x8/5x11) dots font mode */
    LCD_Send_Instruction( hlcd, FUNCTION_SET_REG );
        
    /* Turn (on/off) display, (enable/disable) cursor, 
       and (enable/disable) blink */
    LCD_Send_Instruction( hlcd, DISPLAY_ON_OFF_REG );

    /* Clear display */
    LCD_Send_Instruction( hlcd, CLEAR_DISPLAY_REG );

    /* Set (left/right) cursor direction and (shift/no shift) display */
    LCD_Send_Instruction( hlcd, ENTRY_MODE_SET_REG );
}

void LCD_Start_Data_Write( LCD_HandleTypeDef *hlcd, uint8_t prevData )
{   
    /* Enable E signal without touching previous data.
       This tells the ST7066U driver to accept current data on its pins */
    prevData |= E_ENABLED;
    I2C1_Write( hlcd->lcdaddress, &prevData, 1U );

    /* Disable E signal.
       Prepare driver for future commands or data */
    prevData &= ~E_ENABLED;
    I2C1_Write( hlcd->lcdaddress, &prevData, 1U );
}

void LCD_Send_Instruction( LCD_HandleTypeDef *hlcd, uint8_t instruction )
{
    /* Instruction's MOST SIGNIFICANT nibble + light + write instruction */
    byte = ( instruction & 0xF0U ) | hlcd->backlight | RW_WRITE | RS_INSTRUCTION;

    switch ( instruction )
    {	
        /* Add extra parameters (if any) into the instruction's MOST SIGNIFICANT nibble */

        case FUNCTION_SET_REG:
            byte |= hlcd->datalenght;
            break;

        case SET_DDRAM_ADDRESS_REG:
            byte |= ( DDRAMADDR & 0x70U );
            break;

        default:
            /* Do nothing */
            break;
    }

    /* Send MOST SIGNIFICANT nibble + extra parameters over I2C1 */
    I2C1_Write( hlcd->lcdaddress, &byte, 1U );
    LCD_Start_Data_Write( hlcd, byte );

    /* Instruction's LEAST SIGNIFICANT nibble + light + write instruction */
    byte = ( instruction << 4 ) | hlcd->backlight | RW_WRITE | RS_INSTRUCTION;
        
    switch ( instruction )
    {	
        /* Add extra parameters (if any) into the instruction's LEAST SIGNIFICANT nibble */

        case ENTRY_MODE_SET_REG:
            byte |= ( hlcd->incrementddram | hlcd->shiftdisplay ) << 4;
            break;

        case DISPLAY_ON_OFF_REG:
            byte |= ( hlcd->display | hlcd->cursor | hlcd->blink ) << 4;
            break;

        case CURSOR_DISPLAY_SHIFT_REG:
            byte |= hlcd->shiftcursor << 4;
            break;

        case FUNCTION_SET_REG:
            byte |= ( hlcd->displayline | hlcd->font ) << 4;
            break;

        case SET_DDRAM_ADDRESS_REG:
            byte |= ( DDRAMADDR & 0x0FU ) << 4;
            break;

        default:
            /* Do nothing */
            break;
    }

    /* Send LEAST SIGNIFICANT nibble + extra parameters over I2C1 */
    I2C1_Write( hlcd->lcdaddress, &byte, 1U );
    LCD_Start_Data_Write( hlcd, byte );

    if ( ( instruction == CLEAR_DISPLAY_REG ) || ( instruction == RETURN_HOME_REG ) )
    {   
        /* Wait 1.52 ms */
        TIM3_Delay_us( 1520U );
    }
    else
    {
        /* Wait 37 us */
        TIM3_Delay_us( 37U );
    }
}

void LCD_Write_To_DDRAM( LCD_HandleTypeDef *hlcd, uint8_t data )
{
    /* Data's MSB nible + extra parameters */
    byte = ( data & 0xF0U ) | hlcd->backlight | RW_WRITE | RS_DATA;
    I2C1_Write( hlcd->lcdaddress, &byte, 1 );
    LCD_Start_Data_Write( hlcd, byte );

    /* Data's LSB nible + extra parameters */
    byte = ( data << 4 ) | hlcd->backlight | RW_WRITE | RS_DATA;
    I2C1_Write( hlcd->lcdaddress, &byte, 1U );
    LCD_Start_Data_Write( hlcd, byte );

    /* Wait 37 us */
    TIM3_Delay_us( 37U );
}

void LCD_API_Move_Cursor( LCD_HandleTypeDef *hlcd, uint8_t row, uint8_t col )
{
    if ( row <= 1U && col <= 15U )
    {   
        if ( row == 1U )
        {
            row = 0x40U;
        }

        DDRAMADDR = row + col;
        LCD_Send_Instruction( hlcd, SET_DDRAM_ADDRESS_REG );
    }
}

void LCD_API_Print_Character( LCD_HandleTypeDef *hlcd, uint8_t character )
{
    if ( character >= ' ' && character <= '}' )
    {
        LCD_Write_To_DDRAM( hlcd, character );
    }
}

void LCD_API_Print_String( LCD_HandleTypeDef *hlcd, uint8_t *string, uint8_t size )
{
    uint8_t i;

    for ( i = 0U; i < size; i++ )
    {
        LCD_API_Print_Character( hlcd, string[ i ] );
    }
}

void LCD_API_Shift_Screen_Left( LCD_HandleTypeDef *hlcd, uint8_t positions, uint32_t delay_ms )
{
    uint8_t i;

    hlcd->shiftcursor = SC_RL_SHIFT_DISPLAY_LEFT;

    for ( i = 0U; i < positions; i++ )
    {
        LCD_Send_Instruction( hlcd, CURSOR_DISPLAY_SHIFT_REG );
        TIM3_Delay_ms( delay_ms );
    }
}

void LCD_API_Shift_Screen_Right( LCD_HandleTypeDef *hlcd, uint8_t positions, uint32_t delay_ms )
{
    uint8_t i;

    hlcd->shiftcursor = SC_RL_SHIFT_DISPLAY_RIGHT;

    for ( i = 0U; i < positions; i++ )
    {
        LCD_Send_Instruction( hlcd, CURSOR_DISPLAY_SHIFT_REG );
        TIM3_Delay_ms( delay_ms );
    }
}
