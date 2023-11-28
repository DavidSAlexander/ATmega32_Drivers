
/********************************************************************************************************
 *  [FILE NAME]   :      I2C_LCD.h>                                                                     *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 29, 2023>                                                                *
 *  [Description} :      <Source file for the Characters LCD driver using I2C>                          *
 ********************************************************************************************************/

#include "I2C_LCD.h"

uint8 global_LCD=0;

static uint8 PCF8574_write(uint8 data)
{
	I2C_Start();
	if (I2C_GetStatus() != I2C_Start)
		return ERROR;

	I2C_writeByte((slave_address_fixed_bits) | WRITE);
	if (I2C_GetStatus() != I2C_MT_SLA_W_ACK)
		return ERROR;

	I2C_writeByte(data);
	if (I2C_GetStatus() != I2C_MT_DATA_ACK)
			return ERROR;

	I2C_Stop();

	return SUCCESS;
}


void LCD_init(void)
{
	I2C_init();

	LCD_sendCommand(LCD_GO_TO_HOME);				//--- Return to Home
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);				//--- 4-Bit Mode 2 - Row Select
	LCD_sendCommand(LCD_CURSOR_ON);	    //--- Cursor on, Blinking on
	LCD_sendCommand(LCD_CLEAR_COMMAND);				//--- Clear LCD

}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command)
{

	CLEAR_BIT(global_LCD,LCD_RS_PIN_ID);
	PCF8574_write(global_LCD); /* Instruction Mode RS=0 */
	CLEAR_BIT(global_LCD,LCD_RW_PIN_ID);
	PCF8574_write(global_LCD); /* write data to LCD so RW=0 */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	SET_BIT(global_LCD,LCD_E_PIN_ID);
	PCF8574_write(global_LCD);  /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	#ifdef LCD_LAST_PORT_PINS
		global_LCD = (global_LCD & 0x0F) | (command & 0xF0);
	#else
		global_LCD = (global_LCD & 0xF0) | ((command & 0xF0) >> 4);
	#endif
	PCF8574_write(global_LCD);					//--- Send Data From PCF8574 to LCD PORT

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(global_LCD,LCD_E_PIN_ID);
	PCF8574_write(global_LCD);   /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	SET_BIT(global_LCD,LCD_E_PIN_ID);
	PCF8574_write(global_LCD);  /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	//lcd_port_value = 0;
	#ifdef LCD_LAST_PORT_PINS
		global_LCD = (global_LCD & 0x0F) | ((command & 0x0F) << 4);
	#else
		global_LCD = (global_LCD & 0xF0) | (command & 0x0F);
	#endif
		PCF8574_write(global_LCD);

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(global_LCD,LCD_E_PIN_ID);
	PCF8574_write(global_LCD);   /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
}

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data)
{
	SET_BIT(global_LCD,LCD_RS_PIN_ID);
	PCF8574_write(global_LCD ); /* Data Mode RS=1 */

	CLEAR_BIT(global_LCD,LCD_RW_PIN_ID);
	PCF8574_write(global_LCD); /* write data to LCD so RW=0 */
	_delay_ms(1); /* delay for processing Tas = 50ns */
    SET_BIT(global_LCD,LCD_E_PIN_ID);
	PCF8574_write(global_LCD);  /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	#ifdef LCD_LAST_PORT_PINS
		global_LCD = (global_LCD & 0x0F) | (data & 0xF0);
	#else
		global_LCD = (global_LCD & 0xF0) | ((data & 0xF0) >> 4);
	#endif
	PCF8574_write(global_LCD);					//--- Send Data From PCF8574 to LCD PORT

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(global_LCD,LCD_E_PIN_ID);
	PCF8574_write(global_LCD);   /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	SET_BIT(global_LCD,LCD_E_PIN_ID);
	PCF8574_write(global_LCD);  /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	//lcd_port_value = 0;
	#ifdef LCD_LAST_PORT_PINS
		global_LCD = (global_LCD & 0x0F) | ((data & 0x0F) << 4);
	#else
		global_LCD = (global_LCD & 0xF0) | (data & 0x0F);
	#endif
		PCF8574_write(global_LCD);

		_delay_ms(1); /* delay for processing Tdsw = 100ns */
		CLEAR_BIT(global_LCD,LCD_E_PIN_ID);
		PCF8574_write(global_LCD);   /* Disable LCD E=0 */
		_delay_ms(1); /* delay for processing Th = 13ns */
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}

}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 lcd_memory_address;
	
	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
		case 0:
			lcd_memory_address=col;
				break;
		case 1:
			lcd_memory_address=col+0x40;
				break;
		case 2:
			lcd_memory_address=col+0x10;
				break;
		case 3:
			lcd_memory_address=col+0x50;
				break;
	}					
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_moveCursor(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
   LCD_displayString(buff); /* Display the string */
}

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* Send clear display command */
}
*/