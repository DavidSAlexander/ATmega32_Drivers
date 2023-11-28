
/********************************************************************************************************
 *  [FILE NAME]   :      <LM35Sensor.h>                                                                 *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Aug 3, 2023>                                                                  *
 *  [Description} :      <Header file for the AVR LM35Sensor driver>                                    *
 ********************************************************************************************************/ 


#ifndef LM35SENSOR_H_
#define LM35SENSOR_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../MCAL/ADC.h"

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
   LM35_Fahrenheit,
   LM35_Celsius
}LM35Degree_t;

typedef struct
{
	ADC_Index    LM35PinIndex;
	ADC_Channel  LM35Cfg;
	
}LM35_t;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType LM35_Init(LM35_t *LM35Pin);
Std_ReturnType LM35_Read(const LM35_t *LM35Pin, const LM35Degree_t LM35Deg, float32 *LM35Reading);

#endif /* LM35SENSOR_H_ */