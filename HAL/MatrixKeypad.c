
/********************************************************************************************************
 *  [FILE NAME]   :      <MatrixKeypad.c>                                                               *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 11, 2023>                                                                *
 *  [Description} :      <Source file for the Matrix Keypad driver>                                     *
 ********************************************************************************************************/

#include "MatrixKeypad.h"

static const uint8 KeypadValues[KEYPAD_ROWS][KEYPAD_COLUMNS] =
{
	{'1', '2', '3', '/'},
	{'4', '5', '6', '*'},
	{'7', '8', '9', '-'},
	{'#', '0', '=', '+'}
};

static Std_ReturnType ResetRowsPins(const MatrixKeypad *Keypad, uint8 ResistorsState)
{
	Std_ReturnType ret = E_OK;
	uint8 RowsCounter = ZERO_INIT;
	for ( ; RowsCounter < KEYPAD_ROWS; RowsCounter++ )
	{
		ret = DIO_PinWrite(&(Keypad->KeypadRowsPins[RowsCounter]), !ResistorsState);
		if ( ret == E_NOT_OK ) return ret;
	}
	return ret;
}

/**
 *
 * @param Keypad
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType MatrixKeypad_Init(MatrixKeypad *Keypad)
{
	Std_ReturnType ret = E_OK;
	if(NULL == Keypad)
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint8 ResCircuit = Keypad->ResistorsCircuit==InternalPullupRes?HIGH:LOW;
		uint8 RowsCounter = ZERO_INIT, ColumnsCounter = ZERO_INIT;
		for ( ; RowsCounter < KEYPAD_ROWS; RowsCounter++ )
		{
			Keypad->KeypadRowsPins[RowsCounter].DIRx = PIN_OUTPUT;
			ret = DIO_PinDirSetup(&(Keypad->KeypadRowsPins[RowsCounter]));
			if ( ret == E_NOT_OK ) return ret;
		}
		
		for ( ; ColumnsCounter < KEYPAD_COLUMNS; ColumnsCounter++ )
		{
			Keypad->KeypadColumnsPins[ColumnsCounter].DIRx = PIN_INPUT;
			Keypad->KeypadColumnsPins[ColumnsCounter].STATEx = ResCircuit;
			ret = DIO_PinDirSetup(&(Keypad->KeypadColumnsPins[ColumnsCounter]));
			if ( ret == E_NOT_OK ) return ret;
		}
	}
	return ret;
}
/**
 * @brief function scans and read the pressed keypad button
 * @param Keypad
 * @param Value
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType MatrixKeypad_Read(MatrixKeypad *Keypad, uint8 *Value)
{
	Std_ReturnType ret = E_OK;
	if(NULL == Keypad)
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint8 RowsCounter = ZERO_INIT, ColumnsCounter = ZERO_INIT, BtnValue = '\0';
		uint8 PressedLogic = ( Keypad->ResistorsCircuit == PulldownRes?HIGH:LOW );
		uint16 timeout = 0, thresholdTime = 200;
		for ( ; RowsCounter < KEYPAD_ROWS; RowsCounter++ )
		{
			ret = ResetRowsPins(Keypad, PressedLogic);
			ret = DIO_PinWrite(&(Keypad->KeypadRowsPins[RowsCounter]), PressedLogic);
			ColumnsCounter = ZERO_INIT;
			for ( ; ColumnsCounter < KEYPAD_COLUMNS; ColumnsCounter++ )
			{
				ret = DIO_PinRead(&(Keypad->KeypadColumnsPins[ColumnsCounter]), &BtnValue);
				if (BtnValue == PressedLogic) 
				{
				while(BtnValue == PressedLogic)
				 {
					  ret = DIO_PinRead(&(Keypad->KeypadColumnsPins[ColumnsCounter]), &BtnValue);
					  if (++timeout > thresholdTime)  // Button is stuck 
					    {  
							*Value = '!';
							return E_NOT_OK;
				        }
						_delay_ms(1); 
			     }
				 *Value = KeypadValues[RowsCounter][ColumnsCounter];
				 return E_OK;
				}
				 timeout = 0;
			}
		}
	}
	return ret;
}