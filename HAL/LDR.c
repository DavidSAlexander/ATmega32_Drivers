/********************************************************************************************************
*  [FILE NAME]   :      <LDR.c>                                                                         *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 16, 2023>                                                                 *
*  [Description] :      <Source file for LDR driver>                                                    *
********************************************************************************************************/


#include "LDR.h"

static Std_ReturnType LDR_CalculateVoltage(LDR_t *ldr)
{
	ldr->LDR_VoltageValue = ( ldr->LDR_ADC_Value * 5.0 ) / 1023.0;
	return E_OK;
}
Std_ReturnType LDR_Init(LDR_t *ldr)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == ldr ) ret = E_NOT_OK;
	else
	{
		ret = ADC_Init(&(ldr->LDR_ADC_Cfg));
	}
	return ret;
}
Std_ReturnType LDR_Read(LDR_t *ldr)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == ldr ) ret = E_NOT_OK;
	else
	{
		ret = ADC_Read(ldr->LDR_ADC_PinIndex,&(ldr->LDR_ADC_Value));
		if ( ret == E_OK ) ret = LDR_CalculateVoltage(ldr);
	}
	return ret;
}
