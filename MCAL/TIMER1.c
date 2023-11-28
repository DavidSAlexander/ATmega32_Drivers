/********************************************************************************************************
*  [FILE NAME]   :      <TIMER1.c>                                                                      *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 18, 2023>                                                                 *
*  [Description} :      <Source file for the AVR Timer1 driver>                                         *
********************************************************************************************************/

#include "TIMER1.h"

volatile uint64 ICUfreq = ZERO_INIT;
volatile uint16 PrescalerVal = ZERO_INIT;

Std_ReturnType TIMER1_Init(const Timer1_Config_t* TMR1_Configurations)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR1_Configurations)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if ( TMR1_Configurations->Mode >= TMR1_Normal && TMR1_Configurations->Mode <= TMR1_FastPWM_OCR1A &&
		TMR1_Configurations->CLK >= TMR1NO_CLK_SRC && TMR1_Configurations->CLK <= TMR1EXT_CLK_RISING )
		{
			TCCR1A_REG |= (TMR1_Configurations->Mode & 0x03);
			TCCR1B_REG  = ((TCCR1B_REG & 0xE7) | ( (TMR1_Configurations->Mode & 0x0C) << 1 )) | TMR1_Configurations->CLK;
			if (TMR1_Configurations->COM1xChannel == TMR1_COMA)
			{
				TCCR1A_REG |= ( TMR1_Configurations->COM1xMode << 6 );
			}
			else if (TMR1_Configurations->COM1xChannel == TMR1_COMB)
			{
				TCCR1B_REG |= ( TMR1_Configurations->COM1xMode << 4 );
			}
			//OCR1A_REG = 0;
			// ICR1_REG = (F_CPU / (2 * 500000));   // Phase correct mode set frequency
			//ICR1_REG = OCR1A_REG;
			//OCR1A_REG = (F_CPU / (2 * 1000)) - 1;   // Phase correct mode set frequency
		}
		else ret = E_NOT_OK;

	}
	return ret;
}
Std_ReturnType TIMER1_ICU_Init(const Timer1_Config_t* TMR1_Configurations)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR1_Configurations)
	{
		ret = E_NOT_OK;
	}
	else
	{
		PrescalerVal =
		TMR1_Configurations->CLK==TMR1NO_PRESCALING?1:
		TMR1_Configurations->CLK==TMR1PRESCALING_CLK8?8:
		TMR1_Configurations->CLK==TMR1PRESCALING_CLK64?64:
		TMR1_Configurations->CLK==TMR1PRESCALING_CLK256?256:
		TMR1_Configurations->CLK==TMR1PRESCALING_CLK1024?1024:1;
		
		if ( TMR1_Configurations->ICU_EDGE <= ICES1_RisingEdge && TMR1_Configurations->ICU_EDGE >= ICES1_FallingEdge )
		{
			TCCR1B_REG |= ( TMR1_Configurations->ICU_EDGE << 6 );
		}
		else return E_NOT_OK;
		DIO_PinConfig D6Pin =
		{
			.PORTx  = PORTD_INDEX,
			.PINx   = PIN6_INDEX,
			.DIRx   = PIN_INPUT,
			.STATEx = DIO_InternalPullup
		};
		DIO_PinDirSetup(&D6Pin);
		ENABLE_GIE();
		SET_BIT(TIMSK1_REG, TICIE1_Bit);
		//GlobalTMR1Config = (Timer1_Config_t*) TMR1_Configurations;
		TCCR1B_REG |= (1 << ICNC1_Bit);
	}
	return ret;
}

Std_ReturnType TIMER1_ICU_MeasureFreq(const Timer1_Config_t* TMR1_Configurations, uint64* Frequency)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR1_Configurations || NULL == Frequency)
	{
		ret = E_NOT_OK;
	}
	else
	{
		*Frequency = ICUfreq;
	}
	return ret;
}
Std_ReturnType TIMER1_Stop()
{
	Std_ReturnType ret = E_OK;
	
	return ret;
}
Std_ReturnType TIMER1_EnableInterrupt(const Timer1_Config_t* TMR1_Configurations, Timer1Callback callback)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR1_Configurations)
	{
		ret = E_NOT_OK;
	}
	else
	{
		
	}
	return ret;
}
Std_ReturnType TIMER1_DisableInterrupt(const Timer1_Config_t* TMR1_Configurations, Timer1Callback callback)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR1_Configurations)
	{
		ret = E_NOT_OK;
	}
	else
	{
		
	}
	return ret;
}
Std_ReturnType TIMER1_PWM_Phase_Init(const Timer1_Config_t* TMR1_Configurations, uint8 DutyCycle)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR1_Configurations || DutyCycle < 0 || DutyCycle > 100 )
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (TMR1_Configurations->COM1xChannel == TMR1_COMA)
		{
			//OCR1A_REG = (DutyCycle * 65535/100);
			OCR1A_REG = 10;
		}
	}
	return ret;
}
Std_ReturnType TIMER1_PWM_SetDuty(const Timer1_Config_t* TMR1_Configurations, uint8 DutyCycle)
{
	Std_ReturnType ret = E_OK;
	if (NULL == TMR1_Configurations || DutyCycle < 0 || DutyCycle > 100 )
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (TMR1_Configurations->COM1xChannel == TMR1_COMA)
		{
			//OCR1A_REG = (DutyCycle * 65535/100);
			OCR1A_REG = 10;
		}
	}
	return ret;
}
ISR(TIMER1_CAPT_vect)
{
	static uint32 last_capture = 0;
    uint16 current_capture = ICR1_REG;
    uint32 time_diff = 0;
	
	if (current_capture >= last_capture)
	{
		time_diff = current_capture - last_capture;
	}
	else
	{
		time_diff = (0xFFFF - last_capture) + current_capture + 1;
	}
	ICUfreq = F_CPU  / (time_diff * PrescalerVal);
	last_capture = current_capture;
}