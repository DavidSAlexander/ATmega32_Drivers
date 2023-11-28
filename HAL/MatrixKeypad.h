
/********************************************************************************************************
 *  [FILE NAME]   :      <MatrixKeypad.h>                                                               *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 11, 2023>                                                                *
 *  [Description} :      <Header file for the Matrix Keypad driver>                                     *
 ********************************************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/* ---------------------- Includes ---------------------- */

#include "../MCAL/DIO.h"

/* ----------------- Macro Declarations ----------------- */

#define KEYPAD_ROWS       4
#define KEYPAD_COLUMNS    4


/* --------------------- DataType Declarations -------------------- */

typedef enum
{
	PulldownRes,
	InternalPullupRes,
	PullupRes
} Keypad_InputResistor;

typedef struct 
{
	DIO_PinConfig KeypadRowsPins[KEYPAD_ROWS];
	DIO_PinConfig KeypadColumnsPins[KEYPAD_COLUMNS];
	uint8 ResistorsCircuit;
}MatrixKeypad;

/* ----------------- Software Interfaces Declarations ----------------- */

Std_ReturnType MatrixKeypad_Init(MatrixKeypad *Keypad);
Std_ReturnType MatrixKeypad_Read(MatrixKeypad *Keypad, uint8 *Value);


#endif /* KEYPAD_H_ */