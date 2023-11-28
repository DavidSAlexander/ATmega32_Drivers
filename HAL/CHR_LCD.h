
/********************************************************************************************************
 *  [FILE NAME]   :      <CHR_LCD.h>                                                                    *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Header file for the Characters LCD driver>                                    *
 ********************************************************************************************************/

#ifndef CHR_LCD_H
#define	CHR_LCD_H

/* ---------------------- Includes ---------------------- */

#include "../MCAL/DIO.h"

/* ----------------- Macro Declarations ----------------- */

#define _LCD_CLEAR                                  0X01
#define _LCD_RETURN_HOME                            0x02
#define _LCD_ENTRY_MODE_DEC_SHIFT_OFF               0x04
#define _LCD_ENTRY_MODE_DEC_SHIFT_ON                0x05
#define _LCD_ENTRY_MODE_INC_SHIFT_OFF               0x06
#define _LCD_ENTRY_MODE_INC_SHIFT_ON                0x07
#define _LCD_CURSOR_MOVE_SHIFT_LEFT                 0x10
#define _LCD_CURSOR_MOVE_SHIFT_RIGHT                0x14
#define _LCD_DISPLAY_SHIFT_LEFT                     0x18
#define _LCD_DISPLAY_SHIFT_RIGHT                    0x1C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF    0x0C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON     0x0D
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF     0x0E
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON      0x0F
#define _LCD_DISPLAY_OFF_CURSOR_OFF                 0x08
#define _LCD_8BIT_MODE_2_LINE                       0x38
#define _LCD_4BIT_MODE_2_LINE                       0x28
#define _LCD_4BIT_MODE_2_LINE_INIT1                 0x33
#define _LCD_8BIT_MODE_2_LINE_INIT1                 0x32

#define _LCD_CGRAM_START                            0x40
#define _LCD_DDRAM_START                            0x80

#define ROW1                                        0x01
#define ROW2                                        0x02
#define ROW3                                        0x03
#define ROW4                                        0x04


/* --------------------- DataType Declarations -------------------- */

typedef enum
{
    LCD4Bit = 4,  
    LCD8Bit = 8

}LCD_Connections;
typedef struct
{
    DIO_PinConfig   LCD_RS;
    DIO_PinConfig   LCD_EN;
    DIO_PinConfig   LCD_Data[8];
    LCD_Connections LCD_Bits;

}LCD_t;

/* ----------------- Software Interfaces Declarations ----------------- */

Std_ReturnType LCD_Init(LCD_t *LCD);
Std_ReturnType LCD_WriteCommand(const LCD_t *LCD, uint8 Command);
Std_ReturnType LCD_WriteChar(const LCD_t *LCD, uint8 Data);
Std_ReturnType LCD_WriteCharInPos(const LCD_t *LCD,uint8 Row, uint8 Column, uint8 Data);
Std_ReturnType LCD_WriteString(const LCD_t *LCD, uint8 *str);
Std_ReturnType LCD_WriteStringInPos(const LCD_t *LCD, uint8 Row, uint8 Column, uint8 *Str);
Std_ReturnType LCD_WriteCustomChar(const LCD_t *LCD, uint8 Row, uint8 Column, const uint8 ArrChar[], uint8 MemPos);
Std_ReturnType LCD_WriteInteger(const LCD_t *LCD, sint32 Num);
Std_ReturnType LCD_WriteIntegerInPos(const LCD_t *LCD, uint8 Row, uint8 Column, sint32 Num);
Std_ReturnType LCD_SetCursor(const LCD_t *LCD, uint8 Row, uint8 Column);
Std_ReturnType LCD_Clear(const LCD_t *LCD);

#endif	/* CHR_LCD_H */

