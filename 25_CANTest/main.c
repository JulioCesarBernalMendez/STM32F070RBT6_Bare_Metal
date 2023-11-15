/* This is a draft project in which communication has been established through SPI1 and SPI2 in order to
   write and receive data over a CAN bus by using a CAN module that contains both a CAN controller (MCP2515) and a CAN transceiver (TJA1050).
   A proper CAN driver will be developed soon using this project as a reference.
   
   There are 2 nodes (2 CAN Modules) in the project tied together through the CAN bus and they're
   commanded via SPI1 and SPI2 on the nucleo board.

   Both nodes are initialized at a 125KHz baud rate (bit timing details can be found in the code comments).

   Node 1 sends a standard CAN frame containing 2 data bytes (DLC = 2, byte 0 = 0x0A and byte 1 = 0xA0)
   to the ID = 0x555 via CAN to the node 2 (right one) which uses the mask and filter registers to receive any data destined to the ID = 0x555.

   Finally the data length code and the RXB0D0 and RXB0D1 (receive buffer bytes zero and one) registers of the receiving node are read
   in order to verify data was correctly retrieved from the sending node. */

#include "main.h"

/* SPI1 Pins:
   PA4 - CS
   PA5 - SCK
   PA6 - MISO
   PA7 - MOSI
   
   SPI2 Pins:
   PB12 - CS
   PB13 - SCK
   PB14 - MISO
   PB15 - MOSI */

/* System clock frequency global variable (system_stm32f0xx.c) */
extern uint32_t SystemCoreClock;

/* hold values to be written/read to/from MCP2515 (CAN controller) */
uint8_t spi_write[ 5 ];
uint8_t spi_read[ 3 ];

int main( void )
{  

   /* Update SystemCoreClock global variable (it has to be read as 48000000 after this function's execution) */
   SystemCoreClockUpdate();

   /* Initialize TIM3 peripheral (0.5us time base) */
   TIM3_Init();

   /* Initialize MCP2515 #1 (uses SPI1, see pins at the top of this file) */
   MCP2515_1_Init();

   /* Initialize MCP2515 #2 (uses SPI2, see pins at the top of this file) */
   MCP2515_2_Init();
   

   /******************************** WRITE DATA FROM MCP2515_1 to MCP2515_2 via CAN BUS ********************************/
   /* Write to TXB0SIDH and TXB0SIDL registers in that order (NOTE: EXIDE in TXBnSIDL specifies if frame to be sent is standard/extended).
      NOTE: this assigns a standard ID value of 0x555 = 10101010101 */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x31U;      /* TXB0SIDH register address (first address to write to) */
   spi_write[ 2 ] = 0xAAU;      /* TXB0SIDH = 0xAA: SID[10:3] = 10101010 (made up value) */
   spi_write[ 3 ] = 0xA0U;      /* TXB0SIDL = 0xA0: SID[2:0] = 101 (made up value), EXIDE = transmit standard ID, ED[17:16] = 00 */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 4U ); /* Request WRITE to the address */
   SPI1_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );

   /* Write to TXB0DLC and TXB0D0 (data to be sent) registers in that order */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x35U;      /* TXB0DLC register address (first address to write to) */
   spi_write[ 2 ] = 0x02U;      /* TXB0DLC = 0x02: RTR = transmit message is data frame, DLC[3:0] = 2 data byte */
   spi_write[ 3 ] = 0x0AU;      /* TXB0D0 = 0x0A (data frame, byte 1) */
   spi_write[ 4 ] = 0xA0U;      /* TXB0D1 = 0xA0 (data frame, byte 2) */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 5U ); /* Request WRITE to the address */
   SPI1_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );

   /* Write to TXB0CTRL register */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x30U;      /* TXB0CTRL register address */
   spi_write[ 2 ] = 0x0BU;      /* TXB0CTRL = 0x0B: TXREQ = request transmission, TXP[1:0] = highest priority
                                                    NOTE: when TXREQ is set, ABTF, MLOA and TXERR are cleared automatically */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 3U ); /* Request WRITE to the address */
   SPI1_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );

   /* wait for frame to be transmitted (when 2 data bytes are sent, total CAN frame length is <= 72 bits, then 72 * (1 / 125KHz) = 576us) */
   TIM3_Delay_us( 576U ); /* CAN frame length = 8*n + 44 + floor( (34 + 8*n - 1) / 4 ), with n = number of data bytes */


   /******************************** READ CAN_NODE_1 REGISTERS ********************************/
   /* Read (in burst mode) from CNF3, CNF2 and CNF1 registers to confirm data has been written in them */
   spi_write[ 0 ] = 0x03U;      /* READ instruction */
   spi_write[ 1 ] = 0x28U;      /* CNF3 register address (first address to read from) */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 2U ); /* Request READ from address */
   SPI1_Read( spi_read, 3U );   /* Read 3 data bytes (one for CNF3, one for CNF2 and one for CNF1 in that order)  */
   SPI1_CS_Disable();

   #if 0
   /* Read from CANCTRL register to verify writing process is working */
   spi_write[ 0 ] = 0x03U;          /* READ instruction */
   spi_write[ 1 ] = 0x0FU;          /* CANCTRL register address (XFh, X = doesn't care) */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 2U );     /* Request READ from address */
   SPI1_Read( &spi_read[ 0 ], 1U ); /* Read 1 data byte  */
   SPI1_CS_Disable();

   /* Read from CANSTAT */
   spi_write[ 0 ] = 0x03U;          /* READ instruction */
   spi_write[ 1 ] = 0x0EU;          /* CANSTAT register address (XEh, X = doesn't care) */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 2U );     /* Request READ from address */
   SPI1_Read( &spi_read[ 0 ], 1U ); /* Read 1 data byte  */
   SPI1_CS_Disable();

   /* Read (in burst mode) from TXB0SIDH and TXB0SIDL registers to confirm data has been written in them */
   spi_write[ 0 ] = 0x03U;      /* READ instruction */
   spi_write[ 1 ] = 0x31U;      /* TXB0SIDH register address (first address to read from) */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 2U ); /* Request READ from address */
   SPI1_Read( spi_read, 2U );   /* Read 2 data bytes (one for TXB0SIDH and one for TXB0SIDL in that order)  */
   SPI1_CS_Disable();
   #endif

   /* Read (in burst mode) from TXB0DLC, TXB0D0 and TXB0D1 registers to confirm data has been written in them */
   spi_write[ 0 ] = 0x03U;      /* READ instruction */
   spi_write[ 1 ] = 0x35U;      /* TXB0DLC register address (first address to read from) */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 2U ); /* Request READ from address */
   SPI1_Read( spi_read, 3U );   /* Read 3 data bytes (one for TXB0DLC, one for TXB0D0 and one for TXB0D1 in that order)  */
   SPI1_CS_Disable();

   /* Read from TXB0CTRL */
   spi_write[ 0 ] = 0x03U;          /* READ instruction */
   spi_write[ 1 ] = 0x30U;          /* TXB0CTRL register address */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 2U );     /* Request READ from address */
   SPI1_Read( &spi_read[ 0 ], 1U ); /* Read 1 data byte  */
   SPI1_CS_Disable();

   
   /******************************** READ CAN_NODE_2 REGISTERS ********************************/
   /* Read (in burst mode) from CNF3, CNF2 and CNF1 registers to confirm data has been written in them */
   spi_write[ 0 ] = 0x03U;      /* READ instruction */
   spi_write[ 1 ] = 0x28U;      /* CNF3 register address (first address to read from) */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 2U ); /* Request READ from address */
   SPI2_Read( spi_read, 3U );   /* Read 3 data bytes (one for CNF3, one for CNF2 and one for CNF1 in that order)  */
   SPI2_CS_Disable();

   #if 0
   /* Read from CANCTRL register to verify writing process is working */
   spi_write[ 0 ] = 0x03U;          /* READ instruction */
   spi_write[ 1 ] = 0x0FU;          /* CANCTRL register address (XFh, X = doesn't care) */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 2U );     /* Request READ from address */
   SPI1_Read( &spi_read[ 0 ], 1U ); /* Read 1 data byte  */
   SPI1_CS_Disable();
   #endif

   /* Read from CANSTAT */
   spi_write[ 0 ] = 0x03U;          /* READ instruction */
   spi_write[ 1 ] = 0x0EU;          /* CANSTAT register address (XEh, X = doesn't care) */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 2U );     /* Request READ from address */
   SPI2_Read( &spi_read[ 0 ], 1U ); /* Read 1 data byte  */
   SPI2_CS_Disable();

   /* Read (in burst mode) from RXB0DLC, RXB0D0 and RXB0D1 registers to determine the number of data bytes and data written into RXB0 and RXB1 buffers */
   spi_write[ 0 ] = 0x03U;      /* READ instruction */
   spi_write[ 1 ] = 0x65U;      /* RXB0DLC register address */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 2U ); /* Request READ from address */
   SPI2_Read( spi_read, 3U );   /* Read 3 data bytes (one for RXB0DLC, one for RXB0D0 and one for RXB0D1 in that order)  */
   SPI2_CS_Disable();

   /* Read from RXB0CTRL */
   spi_write[ 0 ] = 0x03U;          /* READ instruction */
   spi_write[ 1 ] = 0x60U;          /* RXB0CTRL register address (XEh, X = doesn't care) */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 2U );     /* Request READ from address */
   SPI2_Read( &spi_read[ 0 ], 1U ); /* Read 1 data byte  */
   SPI2_CS_Disable();

   while( 1 )
   {  
      /* do nothing */
   }

   return 0;
}

void MCP2515_1_Init()
{
   /* Initialize SPI1 peripheral (6MHz) */
   SPI1_Init();

   /* Send MCP2515 RESET instruction */
   spi_write[ 0 ] = 0xC0U; /* RESET instruction */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 1U );
   SPI1_CS_Disable();

   /* MCP2515 must wait for an OST period for the oscillator to stabilize, with OST = 128 OSC1 cycles
      and OSC1 = 8MHz for the CAN Module, therefore OST = (128)(1/8MHz) = 16us */
   TIM3_Delay_us( 16U );

   /* Write data to CNF3, CNF2 and CNF1 registers in that order.
      Next values produce a 125KHz CAN Baud Rate. NOTE: Fosc = 8MHz in the CAN module
      SyncSeg = 1TQ   (fixed by CAN protocol)
      PropSeg = 3TQ   (programmed in CNF2)
      PS1     = 6TQ   (programmed in CNF2)
      PS2     = 6TQ   (programmed in CNF3)
      SJW     = 1TQ   (programmed in CNF1)
      TQ      = 500ns (programmed in CNF1) */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x28U;      /* CNF3 register address (first address to write to) */
   spi_write[ 2 ] = 0x05U;      /* CNF3 = 0x05: SOF = don't care (since CLKEN = 0 in CANCTRL), WAKFIL = disabled,
                                           PHSEG2 = 5 -> PS2 = (PHSEG2 + 1)TQ = 6TQ (since BTLMODE = 1 in CNF2) */
   spi_write[ 3 ] = 0xAAU;      /* CNF2 = 0xAA: BTLMODE = PS2's length determined by PHSEG2 in CNF3, SAM = bus sampled once at sample point,
                                           PHSEG1 = 5 -> PS1 = (PHSEG1 + 1)TQ = 6TQ, PRSEG = 2 -> PropSeg = (PRSEG + 1)TQ = 3TQ */
   spi_write[ 4 ] = 0x01U;      /* CNF1 = 0x01: SJW = 1TQ, BRP = 1 -> TQ = 2(BRP + 1)/Fosc = 2(2)/8MHz = 500ns */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 5U ); /* Request WRITE to the address */
   SPI1_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );
   
   /* Write to CANCTRL register */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x0FU;      /* CANCTRL register address (XFh, X = doesn't care) */
   spi_write[ 2 ] = 0x00U;      /* CANCTRL = 0x00: REQOP = set normal operation mode, ABAT = terminate request to abort all transmissions,
                                        OSM = one-shot mode disabled, CLKEN = CLKOUT pin disabled, CLKPRE = System Clock / 1 */
   SPI1_CS_Enable();
   SPI1_Write( spi_write, 3U ); /* Request WRITE to the address */
   SPI1_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );
}

void MCP2515_2_Init()
{
   /* Initialize SPI2 peripheral (6MHz) */
   SPI2_Init();

   /* Send MCP2515 RESET instruction */
   spi_write[ 0 ] = 0xC0U; /* RESET instruction */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 1U );
   SPI2_CS_Disable();

   /* MCP2515 must wait for an OST period for the oscillator to stabilize, with OST = 128 OSC1 cycles
      and OSC1 = 8MHz for the CAN Module, therefore OST = (128)(1/8MHz) = 16us */
   TIM3_Delay_us( 16U );

   /* Write data to CNF3, CNF2 and CNF1 registers in that order.
      Next values produce a 125KHz CAN Baud Rate. NOTE: Fosc = 8MHz in the CAN module
      SyncSeg = 1TQ   (fixed by CAN protocol)
      PropSeg = 3TQ   (programmed in CNF2)
      PS1     = 6TQ   (programmed in CNF2)
      PS2     = 6TQ   (programmed in CNF3)
      SJW     = 1TQ   (programmed in CNF1)
      TQ      = 500ns (programmed in CNF1)
      Sample Point = SyncSeg + PropSeg + PS2 = 1TQ + 3TQ + 6TQ = 10TQ -> 10TQ / 16TQ = 62.5% */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x28U;      /* CNF3 register address (first address to write to) */
   spi_write[ 2 ] = 0x05U;      /* CNF3 = 0x05: SOF = don't care (since CLKEN = 0 in CANCTRL), WAKFIL = disabled,
                                           PHSEG2 = 5 -> PS2 = (PHSEG2 + 1)TQ = 6TQ (since BTLMODE = 1 in CNF2) */
   spi_write[ 3 ] = 0xAAU;      /* CNF2 = 0xAA: BTLMODE = PS2's length determined by PHSEG2 in CNF3, SAM = bus sampled once at sample point,
                                           PHSEG1 = 5 -> PS1 = (PHSEG1 + 1)TQ = 6TQ, PRSEG = 2 -> PropSeg = (PRSEG + 1)TQ = 3TQ */
   spi_write[ 4 ] = 0x01U;      /* CNF1 = 0x01: SJW = 1TQ, BRP = 1 -> TQ = 2(BRP + 1)/Fosc = 2(2)/8MHz = 500ns */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 5U ); /* Request WRITE to the address */
   SPI2_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );

   /* Write to RXM0SIDH and RXM0SIDL mask registers in that order.
      NOTE1: by setting all the mask bits to 1, only a CAN frame with an ID that matches the filter value will be accepted (see RXF0SID H/L below)
      NOTE2: mask registers can only be modified/read when MCP2515 is in configuration mode
      NOTE2: EXIDE in RXFnSIDL specifies if the filter is applied to standard/extended frames  */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x20U;      /* RXM0SIDH register address (first address to write to) */
   spi_write[ 2 ] = 0xFFU;      /* RXM0SIDH = 0xFF: SID[10:3] = 11111111 (use filter bits [10:3] to evaluate standard CAN ID) */
   spi_write[ 3 ] = 0xE0U;      /* RXM0SIDL = 0xE0: SID[2:0] = 111 (use filter bits [2:0] to evaluate stand. CAN ID), EID[17:16] = don't care */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 4U ); /* Request WRITE to the address */
   SPI2_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );

   /* Write to RXF0SIDH and RXF0SIDL filter registers in that order
      NOTE1: since all mask bits are set to 1, only a CAN frame with an ID that matches the filter value (0x555 = 10101010101) will be accepted
      NOTE2: filter registers can only be modified/read when MCP2515 is in configuration mode
      NOTE3: EXIDE in RXFnSIDL specifies if the filter is applied to standard/extended frames */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x00U;      /* RXF0SIDH register address (first address to write to) */
   spi_write[ 2 ] = 0xAAU;      /* RXF0SIDH = 0xAA: SID[10:3] = 10101010 */
   spi_write[ 3 ] = 0xA0U;      /* RXF0SIDL = 0xA0: SID[2:0] = 101, EXIDE = filter applied to standard frames, EID[17:16] = don't care */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 4U ); /* Request WRITE to the address */
   SPI2_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );

   #if 0
   /* Read (in burst mode) from RXM0SIDH and RXM0SIDL registers to confirm data has been written in them.
      NOTE: mask registers are read all 0s when in any mode, except configuration mode */
   spi_write[ 0 ] = 0x03U;      /* READ instruction */
   spi_write[ 1 ] = 0x20U;      /* RXM0SIDH register address (first address to read from) */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 2U ); /* Request READ from address */
   SPI2_Read( spi_read, 2U );   /* Read 2 data bytes (one for RXM0SIDH and one for RXM0SIDL in that order)  */
   SPI2_CS_Disable();

   /* Read (in burst mode) from RXF0SIDH and RXF0SIDL registers to confirm data has been written in them.
      NOTE: filter registers are read all 0s when in any mode, except configuration mode */
   spi_write[ 0 ] = 0x03U;      /* READ instruction */
   spi_write[ 1 ] = 0x00U;      /* RXF0SIDH register address (first address to read from) */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 2U ); /* Request READ from address */
   SPI2_Read( spi_read, 2U );   /* Read 2 data bytes (one for RXF0SIDH and one for RXF0SIDL in that order)  */
   SPI2_CS_Disable();
   #endif

   /* Write to RXB0CTRL register */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x60U;      /* RXB0CTRL register address */
   spi_write[ 2 ] = 0x00U;      /* RXB0CTRL = 0x00: RXM[1:0] = receive valid messages using standard IDs that meet filter criteria, BUKT = no rollover */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 3U ); /* Request WRITE to the address */
   SPI2_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );
   
   /* Write to CANCTRL register */
   spi_write[ 0 ] = 0x02U;      /* WRITE instruction */
   spi_write[ 1 ] = 0x0FU;      /* CANCTRL register address (XFh, X = doesn't care) */
   spi_write[ 2 ] = 0x00U;      /* CANCTRL = 0x00: REQOP = set normal operation mode, ABAT = terminate request to abort all transmissions,
                                        OSM = one-shot mode disabled, CLKEN = CLKOUT pin disabled, CLKPRE = System Clock / 1 */
   SPI2_CS_Enable();
   SPI2_Write( spi_write, 3U ); /* Request WRITE to the address */
   SPI2_CS_Disable();

   /* Wait 50us for the data to be processed by the MCP2515 (time is not specified in datasheet) */
   TIM3_Delay_us( 50U );
}
