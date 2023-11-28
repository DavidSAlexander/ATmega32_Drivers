
/********************************************************************************************************
 *  [FILE NAME]   :      <CHR_LCD.c>                                                                    *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Source file for the Characters LCD driver>                                    *
 ********************************************************************************************************/

#include "CHR_LCD.h"

static Std_ReturnType LCD_Write4Bits(const LCD_t *LCD, uint8 _data_command);
static Std_ReturnType LCD_EnableControl(const LCD_t *LCD);

/*
 *
 * @param LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_Init(LCD_t *LCD)
{
    Std_ReturnType ret = E_OK;
    uint8 DataPinsCounter = ZERO_INIT;
    if(NULL == LCD || LCD->LCD_Bits < LCD4Bit || LCD->LCD_Bits > LCD8Bit)
    {
        ret = E_NOT_OK;
    }
    else
    {
		LCD->LCD_EN.DIRx = PIN_OUTPUT;
		LCD->LCD_RS.DIRx = PIN_OUTPUT;
        ret = DIO_PinDirSetup(&(LCD->LCD_RS));
        ret = DIO_PinDirSetup(&(LCD->LCD_EN));
        for(DataPinsCounter=ZERO_INIT; DataPinsCounter<LCD->LCD_Bits; DataPinsCounter++)
        {
			LCD->LCD_Data[DataPinsCounter].DIRx = PIN_OUTPUT;
            ret = DIO_PinDirSetup(&(LCD->LCD_Data[DataPinsCounter]));
        }
        _delay_ms(20);
        if ( LCD->LCD_Bits == LCD4Bit )
        {
            ret = LCD_WriteCommand(LCD, _LCD_4BIT_MODE_2_LINE_INIT1);
            _delay_ms(5);
            ret = LCD_WriteCommand(LCD, _LCD_8BIT_MODE_2_LINE_INIT1);
            _delay_ms(150);
            ret = LCD_WriteCommand(LCD, _LCD_4BIT_MODE_2_LINE);
        }
        else
        {
            ret = LCD_WriteCommand(LCD, _LCD_8BIT_MODE_2_LINE);
            _delay_ms(5);
            ret = LCD_WriteCommand(LCD, _LCD_8BIT_MODE_2_LINE);
            _delay_ms(150);
            ret = LCD_WriteCommand(LCD, _LCD_8BIT_MODE_2_LINE);
        }
        ret = LCD_WriteCommand(LCD, _LCD_CLEAR);
        ret = LCD_WriteCommand(LCD, _LCD_RETURN_HOME);
        ret = LCD_WriteCommand(LCD, _LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = LCD_WriteCommand(LCD, _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        if ( LCD->LCD_Bits == LCD4Bit ) ret = LCD_WriteCommand(LCD, _LCD_4BIT_MODE_2_LINE);
        else ret = LCD_WriteCommand(LCD, _LCD_8BIT_MODE_2_LINE);
        ret = LCD_WriteCommand(LCD, _LCD_DDRAM_START);
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param command
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteCommand(const LCD_t *LCD, uint8 Command)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD || LCD->LCD_Bits < LCD4Bit || LCD->LCD_Bits > LCD8Bit)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* R/W Pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write Logic (0) to the "Register Select" Pin to select the "Instruction Register"  */
        ret = DIO_PinWrite(&(LCD->LCD_RS), LOW);

        if ( LCD->LCD_Bits == LCD4Bit )
        {
            /* Send the Command through the "4-Pins" Data lines */
            ret = LCD_Write4Bits(LCD, Command >> 4);
            /* Send the Enable Signal on the "E" Pin */
            ret = LCD_EnableControl(LCD);
            /* Send the Command through the "4-Pins" Data lines */
            ret = LCD_Write4Bits(LCD, Command);
            _delay_us(500);
            ret = DIO_PinWrite(&(LCD->LCD_EN), LOW);
            _delay_us(500);
        }
        else
        {
            uint8 PinCounter = 0;
            /* Send the Command through the (8-Pins" Data lines */
            for(PinCounter = 0; PinCounter < 8; ++PinCounter)
            {
                ret = DIO_PinWrite(&(LCD->LCD_Data[PinCounter]), (Command >> PinCounter) & (uint8)0x01);
            }
        }
        /* Send the Enable Signal on the "E" Pin */
        ret = LCD_EnableControl(LCD);
    }
    return ret;
}

/**
 *
 * @param LCD Pointer to the LCD configurations
 * @param data The data that we need to send to the LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteChar(const LCD_t *LCD, uint8 Data)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD || LCD->LCD_Bits < LCD4Bit || LCD->LCD_Bits > LCD8Bit)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* R/W Pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write Logic (1) to the "Register Select" Pin to select the "Data Register"  */
        ret = DIO_PinWrite(&(LCD->LCD_RS), HIGH);
        if ( LCD->LCD_Bits == LCD4Bit )
        {
            /* Send the Data through the (4-Pins" Data lines */
            ret = LCD_Write4Bits(LCD, Data >> 4);
            /* Send the Enable Signal on the "E" Pin */
            ret = LCD_EnableControl(LCD); 
            /* Send the Data through the (4-Pins" Data lines */
            ret = LCD_Write4Bits(LCD, Data);
        }
        else
        {
            uint8 PinsCounter = ZERO_INIT;
            /* Send the Command through the (8-Pins" Data lines */
            for(PinsCounter = 0; PinsCounter < 8; ++PinsCounter)
            {
                ret = DIO_PinWrite(&(LCD->LCD_Data[PinsCounter]), (Data >> PinsCounter) & (uint8)0x01);
            }
        }
        /* Send the Enable Signal on the "E" Pin */
        ret = LCD_EnableControl(LCD);
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param Data
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteCharInPos(const LCD_t *LCD,uint8 Row, uint8 Column, uint8 Data)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = LCD_SetCursor(LCD, Row, Column);
        ret = LCD_WriteChar(LCD, Data);
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteString(const LCD_t *LCD, uint8 *Str)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        while(*Str)
        {
            ret = LCD_WriteChar(LCD, *Str++);  
        }
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param Str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteStringInPos(const LCD_t *LCD, uint8 Row, uint8 Column, uint8 *Str)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = LCD_SetCursor(LCD, Row, Column);
        while(*Str)
        {
            ret = LCD_WriteChar(LCD, *Str++);
        }
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param ArrChar
 * @param MemPos
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteCustomChar(const LCD_t *LCD, uint8 Row, uint8 Column, const uint8 ArrChar[], uint8 MemPos)
{
    Std_ReturnType ret = E_OK;
    uint8 LCD_counter = ZERO_INIT;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = LCD_WriteCommand(LCD, (_LCD_CGRAM_START+(MemPos*8)));
        for(LCD_counter=0; LCD_counter<=7; ++LCD_counter)
        {
            ret = LCD_WriteChar(LCD, ArrChar[LCD_counter]);
        }
        ret = LCD_WriteCharInPos(LCD, Row, Column, MemPos);
    }
    return ret;
}
/**
 *
 * @param LCD
 * @param DataCommand
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType LCD_Write4Bits(const LCD_t *LCD, uint8 DataCommand)
{
    Std_ReturnType ret = E_OK;
    ret = DIO_PinWrite(&(LCD->LCD_Data[0]), (DataCommand >> 0) & (uint8)0x01);
    ret = DIO_PinWrite(&(LCD->LCD_Data[1]), (DataCommand >> 1) & (uint8)0x01);
    ret = DIO_PinWrite(&(LCD->LCD_Data[2]), (DataCommand >> 2) & (uint8)0x01);
    ret = DIO_PinWrite(&(LCD->LCD_Data[3]), (DataCommand >> 3) & (uint8)0x01);
    return ret;
}

/**
 *
 * @param LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType LCD_EnableControl(const LCD_t *LCD)
{
    Std_ReturnType ret = E_OK;
    ret = DIO_PinWrite(&(LCD->LCD_EN), HIGH);
    _delay_us(750);
    ret = DIO_PinWrite(&(LCD->LCD_EN), LOW);
    _delay_us(750);
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Coulmn
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_SetCursor(const LCD_t *LCD, uint8 Row, uint8 Coulmn)
{
    Std_ReturnType ret = E_OK;
	if (Coulmn > 0)
	{
		 Coulmn--;
	}
    switch(Row)
    {
    case ROW1 :
        ret = LCD_WriteCommand(LCD, (0x80 + Coulmn));
        break;
    case ROW2 :
        ret = LCD_WriteCommand(LCD, (0xC0 + Coulmn));
        break;
    case ROW3 :
        ret = LCD_WriteCommand(LCD, (0x94 + Coulmn));
        break;
    case ROW4 :
        ret = LCD_WriteCommand(LCD, (0xD4 + Coulmn));
        break;
    default :
        ;
    }
    return ret;
}
/**
 *
 * @param LCD
 * @param Num
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteInteger(const LCD_t *LCD, sint32 Num)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        sint8 str[16];
        boolean isNegative = FALSE;
        if ( Num < 0 )
        {
            Num = -Num;
            isNegative = TRUE;
        }
        int i = 0;
        do
        {
            str[i++] = '0' + Num % 10;
            Num /= 10;
        }
        while (Num > 0);
        if (isNegative) str[i++] = '-';
        str[i] = '\0';

        // reverse the string
        for (int j = 0; j < i / 2; j++)
        {
            char temp = str[j];
            str[j] = str[i - j - 1];
            str[i - j - 1] = temp;
        }
        LCD_WriteString(LCD,(uint8*)str); /* Display the string */
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param Num
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_WriteIntegerInPos(const LCD_t *LCD, uint8 Row, uint8 Column, sint32 Num)
{
	Std_ReturnType ret = E_OK;
	if(NULL == LCD)
	{
		ret = E_NOT_OK;
	}
	else
	{
		sint8 str[16];
		boolean isNegative = FALSE;
		if ( Num < 0 )
		{
			Num = -Num;
			isNegative = TRUE;
		}
		int i = 0;
		do
		{
			str[i++] = '0' + Num % 10;
			Num /= 10;
		}
		while (Num > 0);
		if (isNegative) str[i++] = '-';
		str[i] = '\0';

		// reverse the string
		for (int j = 0; j < i / 2; j++)
		{
			char temp = str[j];
			str[j] = str[i - j - 1];
			str[i - j - 1] = temp;
		}
		LCD_WriteStringInPos(LCD, Row, Column, (uint8*)str); /* Display the string */
		LCD_WriteChar(LCD,' ');
	}
	return ret;
}
/**
 *
 * @param LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_Clear(const LCD_t *LCD)
{
	Std_ReturnType ret = E_OK;
	if(NULL == LCD)
	{
		ret = E_NOT_OK;
	}
	else
	{
		LCD_WriteCommand(LCD, _LCD_CLEAR );
	}
	return ret;
}