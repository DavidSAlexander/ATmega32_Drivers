/********************************************************************************************************
 *  [FILE NAME]   :      <LDR.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 16, 2023>                                                                *
 *  [Description] :      <Header file for LDR driver>                                                   *
 ********************************************************************************************************/ 


#ifndef LDR_H_
#define LDR_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../MCAL/ADC.h"

/*******************************************************************************
 *                          DataType Declarations                              *
 *******************************************************************************/

typedef enum
{
	LDR_Pullup,
	LDR_Pulldown
} LDR_Circuit;

typedef struct
{
	ADC_Index    LDR_ADC_PinIndex;
	ADC_Channel  LDR_ADC_Cfg;
	uint16       LDR_ADC_Value;
	float32      LDR_VoltageValue;
}LDR_t;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType LDR_Init(LDR_t *ldr);
Std_ReturnType LDR_Read(LDR_t *ldr);


#endif /* LDR_H_ */