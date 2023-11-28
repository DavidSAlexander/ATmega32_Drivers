
/********************************************************************************************************
 *  [FILE NAME]   :      <LM35Sensor.c>                                                                 *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Aug 3, 2023>                                                                  *
 *  [Description} :      <Source file for the AVR LM35Sensor driver>                                    *
 ********************************************************************************************************/ 



/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "LM35Sensor.h"


Std_ReturnType LM35_Init(LM35_t *LM35Pin)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == LM35Pin ) ret = E_NOT_OK;
	else
	{
		LM35Pin->LM35Cfg.VoltageReference = External_AVCC;
		LM35Pin->LM35Cfg.InturruptMode = ADCInterruptEnabled;
		LM35Pin->LM35Cfg.ResultAdjust = Right_Adjusted;
		ret = ADC_Init(&(LM35Pin->LM35Cfg));
	}
	return ret;
}
Std_ReturnType LM35_Read(const LM35_t *LM35Pin, const LM35Degree_t LM35Deg, float32 *LM35Reading)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == LM35Pin || LM35Deg > LM35_Celsius || LM35Deg < LM35_Fahrenheit ) ret = E_NOT_OK;
	else
	{
		uint16 tReading = ZERO_INIT;
		ret = ADC_Read(LM35Pin->LM35PinIndex, &tReading);
		if (ret == E_OK)
		{
			if (LM35Deg == LM35_Celsius)
			{
				*LM35Reading = (float32)((tReading * 5000.0) / (1024 * 10.0));
			}
			else
			{
				*LM35Reading = (float32)((tReading * 5000.0) / (1024 * 10.0));
				*LM35Reading = (*LM35Reading * 9.0 / 5.0) + 32.0;
			}
		}
	}
	return ret;
}