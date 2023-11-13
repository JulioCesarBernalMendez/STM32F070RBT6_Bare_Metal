/* File used for better understanding on how the PCF8574 I2C to ST7066U (LCD Driver) module is 
   implemented to initialize the LCD and printing of 'A' character on 2nd row, last column.
   Based on this procedure a specialized software driver will be developed for an easier
   handling of the LCD */

#include "main.h"

int main( void )
{   
    /* initialize SysTick */
    SysTick_Init();

    /* inititalize I2C1 */
    I2C1_Init();

    /*********************************************************
     *                    LCD INITIALIZATION                 *
     *********************************************************/
    uint8_t i2cByte;

    /* wait 41 ms */
    Systick_Delay_Ms( 41 );

    /*********************** FUNCTION SET ********************/
    /* send 0x38 = 0011 1000
       function set
       DB7       = 0
       DB6       = 0
       DB5       = 1
       DB4       = 1 = L = ST066U in 8 bit mode
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x = N -- NOT USED
       DB2       = x = F -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x30; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x34; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x30; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal

    /* wait 1 ms */
    Systick_Delay_Ms( 1 );

    /*********************** FUNCTION SET ********************/
    /* send 0x20 = 0010 0000
       function set MSB nibble
       DB7       = 0
       DB6       = 0
       DB5       = 1
       DB4       = 0 = L = ST066U in 4 bit mode
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x20; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x24; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x20; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

    /* send 0x80 = 1000 0000
       function set LSB nibble
       DB7       = 1 = N = 2 line display
       DB6       = 0 = F = 5x8 dots format
       DB5       = x
       DB4       = x 
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x80; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x84; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x80; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal

    /* wait 1 ms */
    Systick_Delay_Ms( 1 );

    /*********************** FUNCTION SET ********************/
    /* send 0x20 = 0010 0000
       function set MSB nibble
       DB7       = 0
       DB6       = 0
       DB5       = 1
       DB4       = 0 = L = ST066U in 4 bit mode
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x20; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x24; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x20; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

    /* send 0x80 = 1000 0000
       function set LSB nibble
       DB7       = 1 = N = 2 line display
       DB6       = 0 = F = 5x8 dots format
       DB5       = X
       DB4       = X 
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x80; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x84; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x80; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal

    /* wait 1 ms */
    Systick_Delay_Ms( 1 );

    /*********************** DISPLAY ON/OFF CONTROL ********************/
    /* send 0x00 = 0000 0000
       display on/off control MSB nibble
       DB7       = 0
       DB6       = 0
       DB5       = 0
       DB4       = 0
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x00; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x04; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x00; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

    /* send 0xC0 = 1100 0000
       display on/off control LSB nibble
       DB7       = 1
       DB6       = 1 = D = display ON
       DB5       = 0 = C = cursor OFF
       DB4       = 0 = B = blink OFF
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0xF0; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0xF4; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0xF0; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal

    /* wait 1 ms */
    Systick_Delay_Ms( 1 );

    /*********************** CLEAR DISPLAY ********************/
    /* send 0x00 = 0000 0000
       clear display MSB nibble
       DB7       = 0
       DB6       = 0
       DB5       = 0
       DB4       = 0
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x00; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x04; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x00; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

    /* send 0x10 = 0001 0000
       clear display LSB nibble
       DB7       = 0
       DB6       = 0
       DB5       = 0
       DB4       = 1
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x10; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x14; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x10; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal

    /* wait 2 ms */
    Systick_Delay_Ms( 2 );

    /*********************** ENTRY MODE ********************/
    /* send 0x00 = 0000 0000
       entry mode MSB nibble
       DB7       = 0
       DB6       = 0
       DB5       = 0
       DB4       = 0
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x00; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x04; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x00; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

    /* send 0x60 = 0110 0000
       entry mode LSB nibble
       DB7       = 0
       DB6       = 1
       DB5       = 1 = I/D = cursor/blink moves to the right and DDRAM address is increased by 1
       DB4       = 0 = S = shift of display not performed
       backlight = 1 = backlight ON
       E         = 0 
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
    i2cByte = 0x60; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x64; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
    i2cByte = 0x68; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
    Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */



    Systick_Delay_Ms( 20 ); /* for me to know (in Logic Analyzer) where the printing to LCD starts */
    /*********************************************************
     *                PRINT CHARACTER ON LCD                 *
     *********************************************************/
    /* Set DDRAM to 0x4FH (MSB Nibble) <- This DDRAM address corresponds to second row - last column on the LCD
       DB7       = 1
       DB6       = 1 = AC6
       DB5       = 0 = AC5
       DB4       = 0 = AC4

       backlight = 1 = backlight ON
       E         = 0
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
   i2cByte = 0xC8; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0xCC; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0xC8; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

   /* Set DDRAM to 0x4FH (MSB Nibble)
       DB7       = 1 = AC3
       DB6       = 1 = AC2
       DB5       = 1 = AC1
       DB4       = 1 = AC0

       backlight = 1 = backlight ON
       E         = 0
       RW        = 0
       RS        = 0

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
   i2cByte = 0xF8; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0xFC; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0xF8; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

   /* Write to DDRAM the value 0x41H (MSB Nibble) <- This value corresponds to character A
       DB7       = 0 = D7
       DB6       = 1 = D6
       DB5       = 0 = D5
       DB4       = 0 = D4

       backlight = 1 = backlight ON
       E         = 0
       RW        = 0 \
       RS        = 1 / RW = 0 & RS = 1 means write data to ST066U's RAM

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
   i2cByte = 0x49; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0x4D; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0x49; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

   /* Write to DDRAM the value 0x41H (LSB Nibble)
       DB7       = 0 = D7
       DB6       = 0 = D6
       DB5       = 0 = D5
       DB4       = 1 = D4

       backlight = 1 = backlight ON
       E         = 0
       RW        = 0 \
       RS        = 1 / RW = 0 & RS = 1 means write data to ST066U's RAM

       DB3       = x     -- NOT USED
       DB2       = x     -- NOT USED
       DB1       = x     -- NOT USED
       DB0       = x     -- NOT USED */
   i2cByte = 0x19; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 );
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0x1D; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 1, for ST066U to accept the data
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */
   i2cByte = 0x19; I2C1_Write( SLAVE_ADDRESS, &i2cByte, 1 ); // E = 0, reset E signal
   Systick_Delay_Ms( 1 ); /* wait 1 ms, for stop bit to be sent */

    while( 1 )
    {   
        /* do nothing */
    }

    return 0;
}
