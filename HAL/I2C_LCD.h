
/********************************************************************************************************
 *  [FILE NAME]   :      <I2C_LCD.h>                                                                    *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Header file for the Characters LCD driver using I2C>                          *
 ********************************************************************************************************/ 


#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "I2C.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define LCD_DATA_BITS_MODE 4

#if((LCD_DATA_BITS_MODE != 4))

#error "Number of Data bits should be equal to 4 "

#endif

#if (LCD_DATA_BITS_MODE == 4)

#define LCD_LAST_PORT_PINS

#ifdef LCD_LAST_PORT_PINS
#define LCD_FIRST_DATA_PIN_ID         PIN4_ID
#else
#define LCD_FIRST_DATA_PIN_ID         PIN0_ID
#endif

#endif

/* LCD Commands */
#define LCD_CLEAR_COMMAND              0x01
#define LCD_GO_TO_HOME                 0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE  0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE   0x28
#define LCD_CURSOR_OFF                 0x0C
#define LCD_CURSOR_ON                  0x0F
#define LCD_SET_CURSOR_LOCATION        0x80

#define ERROR 0
#define SUCCESS 1

#define LCD_RS_PIN_ID                  0
#define LCD_RW_PIN_ID                  1
#define LCD_E_PIN_ID                   2


#define slave_address_fixed_bits	0x20 << 1		//0x20 as A0=A1=A2=0
#define	WRITE			0
#define READ			1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

static uint8 PCF8574_write(uint8 data);
void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *Str);
void LCD_moveCursor(uint8 row,uint8 col);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_intgerToString(int data);
void LCD_clearScreen(void);

#endif /* I2C_LCD_H_ */