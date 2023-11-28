/********************************************************************************************************
*  [FILE NAME]   :      <TIMER0.c>                                                                      *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 18, 2023>                                                                 *
*  [Description} :      <Source file for the AVR Timer0 driver>                                         *
********************************************************************************************************/

#include "TIMER0.h"
#include "TS_CONFIG.h"

/******************************************************************************
*                               GLOBAL VARIABLES                              *
*******************************************************************************/

Timer0Callback CALLBACK_OVER_FLOW;
Timer0Callback CALLBACK_COMPARE;

/*******************************************************************************
*                            Functions Declaration                             *
*******************************************************************************/

Std_ReturnType TIMER0_Init(const Timer0_Config_t* TMR0_Configurations)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR0_Configurations) ret = E_NOT_OK;
	else
	{
		if (TMR0_Configurations->Mode >= TMR0_Normal && TMR0_Configurations->Mode <= TMR0_FastPWM)
		{
			TCCR0_REG |= ( (TMR0_Configurations->Mode & 0x02) << 5) | ( (TMR0_Configurations->Mode & 0x01 ) << 3);
			
			if (TMR0_Configurations->Mode == TMR0_PhaseCorrectPWM || TMR0_Configurations->Mode == TMR0_FastPWM)
			{
				TCCR0_REG = ( (TCCR0_REG & 0xCF) | (TMR0_Configurations->OC0Mode << 4) );
			}
		}
		else ret = E_NOT_OK;
		if ( (TMR0_Configurations->CLK >= TMR0NO_CLK_SRC) && (TMR0_Configurations->CLK <= TMR0EXT_CLK_RISING) && ret != E_NOT_OK )
		{
			TCCR0_REG |= TMR0_Configurations->CLK;
		}
	}
	return ret;
}

Std_ReturnType TIMER0_Stop()
{
	Std_ReturnType ret = E_OK;
	CLEAR_BIT(TCCR0_REG,CS00_Bit);
	CLEAR_BIT(TCCR0_REG,CS01_Bit);
	CLEAR_BIT(TCCR0_REG,CS02_Bit);
	return ret;
}


/**
* @brief Get the OCR0 register value.
* @param TicksNumber a pointer to 8-bit variable to store OCR0 value.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*     - E_NOT_OK: The function encountered an error.
*/
Std_ReturnType TIMER0_GetOCR0(const Timer0_Config_t* TMR0_Configurations, uint8* TicksNumber)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TicksNumber || NULL == TMR0_Configurations) ret = E_NOT_OK;
	else *TicksNumber = OCR0_REG;
	return ret;
}

Std_ReturnType TIMER0_SetOCR0(const Timer0_Config_t* TMR0_Configurations, uint8 TicksNumber)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == TMR0_Configurations || TicksNumber > 255 || TicksNumber < 0 ) ret = E_NOT_OK;
	else OCR0_REG = TicksNumber;
	return ret;
}

Std_ReturnType TIMER0_GetTCNT0(const Timer0_Config_t* TMR0_Configurations, uint8* TicksNumber)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == TMR0_Configurations || NULL == TicksNumber ) ret = E_NOT_OK;
	else *TicksNumber = TCNT0_REG;
	return ret;
}

Std_ReturnType TIMER0_SetTCNT0(const Timer0_Config_t* TMR0_Configurations, uint8 TicksNumber)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == TMR0_Configurations || TicksNumber > 255 ) ret = E_NOT_OK;
	else TCNT0_REG = TicksNumber;
	return ret;
}

Std_ReturnType TIMER0_EnableInterrupt(const Timer0_Config_t* TMR0_Configurations, Timer0Callback callback)
{
	Std_ReturnType ret = E_OK;
	if (NULL == callback || NULL == TMR0_Configurations) ret = E_NOT_OK;
	else
	{
		if ( TMR0_Configurations->Mode == TMR0_Normal )
		{
			ENABLE_GIE();
			SET_BIT(TIMSK0_REG, TOIE0_Bit);
			CALLBACK_OVER_FLOW = callback;
		}
		else if ( TMR0_Configurations->Mode == TMR0_CTC )
		{
			ENABLE_GIE();
			SET_BIT(TIMSK0_REG, OCIE0_Bit);
			CALLBACK_COMPARE = callback;
		}
		else ret = E_NOT_OK;
	}
	return ret;
}
Std_ReturnType TIMER0_DisableInterrupt(const Timer0_Config_t* TMR0_Configurations, Timer0Callback callback)
{
	Std_ReturnType ret = E_OK;
	
	if ( TMR0_Configurations->Mode == TMR0_Normal )
	{
		CLEAR_BIT(TIMSK0_REG, TOIE0_Bit);
		CALLBACK_OVER_FLOW = callback;
	}
	else if ( TMR0_Configurations->Mode == TMR0_CTC )
	{
		CLEAR_BIT(TIMSK0_REG, OCIE0_Bit);
		CALLBACK_COMPARE = callback;
	}
	else ret = E_NOT_OK;
	
	return ret;
}

Std_ReturnType TIMER0_PWM_SetDuty(const Timer0_Config_t* TMR0_Configurations, uint8 DutyCycle)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == TMR0_Configurations || DutyCycle < 0 || DutyCycle > 100 ) ret = E_NOT_OK;
	else
	{
		ret = TIMER0_SetOCR0(TMR0_Configurations, DutyCycle * 255/100);
	}
	return ret;
}
ISR(TIMER0_COMP_vect)
{
	CALLBACK_COMPARE();
}

ISR(TIMER0_OVF_vect)
{
	CALLBACK_OVER_FLOW();
}



