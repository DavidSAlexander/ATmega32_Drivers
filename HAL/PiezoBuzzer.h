
/********************************************************************************************************
 *  [FILE NAME]   :      <PiezoBuzzer.h>                                                                *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Aug 3, 2023>                                                                  *
 *  [Description} :      <Header file for the AVR PiezoBuzzer driver>                                   *
 ********************************************************************************************************/ 


#ifndef PIEZOBUZZER_H_
#define PIEZOBUZZER_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../MCAL/DIO.h"

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
	PBuzzer_NPN,
	PBuzzer_PNP
} PBuzzerDriver_t;

typedef enum
{
	PBuzzerOFF,
	PBuzzerON
} PBuzzerStatus_t;

typedef struct
{
	uint8           PortName    : 2;
	uint8           PinName     : 3;
	PBuzzerDriver_t PBuzzerType : 1;
} PBuzzer_t;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType PBuzzer_Init(const PBuzzer_t *Buzz);
Std_ReturnType PBuzzer_Control(const PBuzzer_t *Buzz, PBuzzerStatus_t BuzzS);
Std_ReturnType PBuzzer_Toggle(const PBuzzer_t *Buzz);



#endif /* PIEZOBUZZER_H_ */