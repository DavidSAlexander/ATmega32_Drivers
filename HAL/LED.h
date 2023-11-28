
/********************************************************************************************************
 *  [FILE NAME]   :      <LED.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Header file for LED configurations driver>                                    *
 ********************************************************************************************************/

#ifndef LED_H
#define	LED_H

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/

#include "../MCAL/DIO.h"

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
	LED_OFF,
	LED_ON
} LED_Status;

typedef enum
{
	LED_SOURCE,
	LED_SINK
} LED_Circuit;

typedef struct
{
    uint8 PortName    : 3;
    uint8 PinName     : 3;
    uint8 LedState    : 1;
	uint8 LedCircuit  : 1;
} SingleLED;

typedef struct
{
    uint8 PortName    : 3;
    uint8 LedsState   : 8;
	uint8 LedCircuit  : 1;
} PortLEDs;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

 Std_ReturnType LED_Init(const SingleLED *LED);
 Std_ReturnType LEDs_Init(const PortLEDs *LEDs);
 Std_ReturnType LED_Control(const SingleLED *LED, LED_Status Status);
 Std_ReturnType LEDs_Control(const PortLEDs *LEDs, uint8 Status);
 Std_ReturnType LED_Toggle(const SingleLED *LED);
 Std_ReturnType LEDs_Toggle(const PortLEDs *LEDs);


#endif	/* LED_H */
