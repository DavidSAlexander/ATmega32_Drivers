
/********************************************************************************************************
 *  [FILE NAME]   :      <PB.h>                                                                         *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Header file for the AVR PushButton driver>                                    *
 ********************************************************************************************************/

#ifndef PB_H
#define	PB_H

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../MCAL/DIO.h"

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/
typedef enum
{
    ButtonReleased,
    ButtonPressed
} Button_Status;

typedef enum
{
    NormallyOpened,
    NormallyClosed
} Button_Type;

typedef enum
{
    Pulldown,
    InternalPullup,
    Pullup
} Button_Resistor;

typedef struct
{
    uint8           PortName   : 3;
    uint8           PinName    : 3;
    Button_Type     ButtonType;
    Button_Resistor ResType;
} PButton;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/
Std_ReturnType PB_Init(const PButton *PB);
Std_ReturnType PB_Read(const PButton *PB, Button_Status *PBS);


#endif	/* PB_H */
