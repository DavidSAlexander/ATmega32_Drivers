
/********************************************************************************************************
*  [FILE NAME]   :      <PWM1.h>                                                                        *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 22, 2023>                                                                 *
*  [Description} :      <Header file for the AVR Timer1 PWMs driver>                                    *
********************************************************************************************************/


#ifndef PWM1_H_
#define PWM1_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "TIMER1.h"

typedef enum
{
	PWM1_PhaseCorrectPWM8Bit             = 1,
	PWM1_PhaseCorrectPWM9Bit,
	PWM1_PhaseCorrectPWM10Bit,
	PWM1_FastPWM8Bit                     = 5,
	PWM1_FastPWM9Bit,
	PWM1_FastPWM10Bit,
	PWM1_PhaseFrequencyCorrectPWM_ICR1,
	PWM1_PhaseFrequencyCorrectPWM_OCR1A,
	PWM1_PhaseCorrectPWM_ICR1,
	PWM1_PhaseCorrectPWM_OCR1A,
	PWM1_FastPWM_ICR1                    = 14,
	PWM1_FastPWM_OCR1A
}PWM1_Modes_t;

typedef enum
{
	PWM1_OC1xDisconnected,
	PWM1_OC1xToggleOnCompareMatch,
	PWM1_OC1xClearOnCompareMatch,
	PWM1_OC1xSetOnCompareMatch
}PWM1_CompareOutputModes_t;

typedef enum
{
	PWM1_OC1A,
	PWM1_OC1B
}PWM1_Channel_t;

typedef enum
{
	PWM1NO_CLK_SRC,
	PWM1NO_PRESCALING,
	PWM1PRESCALING_CLK8,
	PWM1PRESCALING_CLK64,
	PWM1PRESCALING_CLK256,
	PWM1PRESCALING_CLK1024,
	PWM1EXT_CLK_FALLING,
	PWM1EXT_CLK_RISING
}PWM1_Clock_t;

typedef struct 
{
	PWM1_Modes_t               PWM1_Mode;
	PWM1_CompareOutputModes_t  PWM1_OC1xMode;
	PWM1_Channel_t             PWM1_Channel;
	PWM1_Clock_t               PWM1_CLK;
	uint16                     PWM1_TopValue;
	
}PWM1_Config_t;

Std_ReturnType PWM1_Init(PWM1_Config_t* PWMcfg);
Std_ReturnType PWM1_SetFrequency(PWM1_Config_t* PWMcfg, uint32 Freq);
Std_ReturnType PWM1_SetDuty(PWM1_Config_t* PWMcfg, uint8 Duty);
Std_ReturnType PWM1_GetOCR1x();
Std_ReturnType PWM1_SetOCR1x();
Std_ReturnType PWM1_GetICR1();
Std_ReturnType PWM1_SetICR1();



#endif /* PWM1_H_ */