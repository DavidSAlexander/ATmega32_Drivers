
/********************************************************************************************************
*  [FILE NAME]   :      <PWM1.c>                                                                        *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 22, 2023>                                                                 *
*  [Description} :      <Source file for the AVR Timer1 PWMs driver>                                    *
********************************************************************************************************/

#include "PWM1.h"

volatile uint8 PWM1PrescalerVal = ZERO_INIT;
Std_ReturnType PWM1_Init(PWM1_Config_t* PWMcfg)
{
	Std_ReturnType ret = E_OK;
	if (NULL == PWMcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		PWM1PrescalerVal =
		PWMcfg->PWM1_CLK==PWM1NO_PRESCALING?1:
		PWMcfg->PWM1_CLK==PWM1PRESCALING_CLK8?8:
		PWMcfg->PWM1_CLK==PWM1PRESCALING_CLK64?64:
		PWMcfg->PWM1_CLK==PWM1PRESCALING_CLK256?256:
		PWMcfg->PWM1_CLK==PWM1PRESCALING_CLK1024?1024:1;
		
		if ( PWMcfg->PWM1_Mode >= PWM1_PhaseCorrectPWM8Bit && PWMcfg->PWM1_Mode <= PWM1_FastPWM_OCR1A &&
		PWMcfg->PWM1_CLK >= PWM1NO_CLK_SRC && PWMcfg->PWM1_CLK <= PWM1EXT_CLK_RISING )
		{
			TCCR1A_REG |= (PWMcfg->PWM1_Mode & 0x03);
			TCCR1B_REG  = ((TCCR1B_REG & 0xE7) | ( (PWMcfg->PWM1_Mode & 0x0C) << 1 )) | PWMcfg->PWM1_CLK;
			if (PWMcfg->PWM1_Channel == PWM1_OC1A)
			{
				TCCR1A_REG |= ( PWMcfg->PWM1_OC1xMode << 6 );
			}
			else if (PWMcfg->PWM1_Channel == PWM1_OC1B)
			{
				TCCR1A_REG |= ( PWMcfg->PWM1_OC1xMode << 4 );
			}
			switch (PWMcfg->PWM1_Mode)
			{
				case PWM1_FastPWM8Bit:
				PWMcfg->PWM1_TopValue = 0x00FF;
				break;
				case PWM1_FastPWM9Bit:
				PWMcfg->PWM1_TopValue = 0x01FF;
				break;
				case PWM1_FastPWM10Bit:
				PWMcfg->PWM1_TopValue = 0x03FF;
				break;
				case PWM1_PhaseCorrectPWM8Bit:
				PWMcfg->PWM1_TopValue = 0x00FF;
				break;
				case PWM1_PhaseCorrectPWM9Bit:
				PWMcfg->PWM1_TopValue = 0x01FF;
				break;
				case PWM1_PhaseCorrectPWM10Bit:
				PWMcfg->PWM1_TopValue = 0x03FF;
				break; 
				case PWM1_PhaseFrequencyCorrectPWM_ICR1:
				ICR1_REG = (( F_CPU / PWM1PrescalerVal ) /  (2 * 10000));   // Phase correct mode default frequency
				PWMcfg->PWM1_TopValue = ICR1_REG;
				break; 
				case PWM1_PhaseCorrectPWM_ICR1:
				ICR1_REG = (( F_CPU / PWM1PrescalerVal ) /  (2 * 10000)); 
				PWMcfg->PWM1_TopValue = ICR1_REG;
				break; 
				case PWM1_FastPWM_ICR1:
				ICR1_REG = ( F_CPU  /  (PWM1PrescalerVal * 2 * 10000)) - 1;
				PWMcfg->PWM1_TopValue = ICR1_REG;
				break; 
				case PWM1_FastPWM_OCR1A:
				OCR1A_REG = 0xFFFF;
				PWMcfg->PWM1_TopValue = OCR1A_REG;
				break; 
				default:
				break;
			}
		}
		else ret = E_NOT_OK;
	}
	return ret;
}
Std_ReturnType PWM1_SetFrequency(PWM1_Config_t* PWMcfg, uint32 Freq)
{
	Std_ReturnType ret = E_OK;
	if (NULL == PWMcfg || Freq < 1)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (PWMcfg->PWM1_Mode == PWM1_FastPWM_ICR1)
		ICR1_REG = (F_CPU / (PWM1PrescalerVal * Freq)) - 1;
		else 
		ICR1_REG = (F_CPU / (PWM1PrescalerVal * Freq));
		PWMcfg->PWM1_TopValue = ICR1_REG;
		
	}
	return ret;
}
Std_ReturnType PWM1_SetDuty(PWM1_Config_t* PWMcfg, uint8 Duty)
{
	Std_ReturnType ret = E_OK;
	if (NULL == PWMcfg || Duty < 0 || Duty > 100)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (PWMcfg->PWM1_Channel == PWM1_OC1A)
		{
			OCR1A_REG = (uint16)(((uint32)Duty * PWMcfg->PWM1_TopValue) / 100);
		}
		else if (PWMcfg->PWM1_Channel == PWM1_OC1B)
		{
			OCR1B_REG = (uint16)(((uint32)Duty * PWMcfg->PWM1_TopValue) / 100);
		}
	}
	return ret;
}
/*Std_ReturnType PWM1_GetOCR1x();
Std_ReturnType PWM1_SetOCR1x();
Std_ReturnType PWM1_GetICR1();
Std_ReturnType PWM1_SetICR1();*/