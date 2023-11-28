/********************************************************************************************************
*  [FILE NAME]   :      <External_Interrupt.c>                                                          *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 17, 2023>                                                                 *
*  [Description} :      <Source file for the AVR External Interrupt driver>                             *
********************************************************************************************************/

#include "External_Interrupt.h"


static ExternalInterruptCallback int0Callback = NULL;
static ExternalInterruptCallback int1Callback = NULL;
static ExternalInterruptCallback int2Callback = NULL;

Std_ReturnType ExternalInterrupt_Init(ExternalInterruptType Interrupt, ExternalInterruptMode ExtIntMode, ExternalInterruptCallback ExtIntCallback)
{
	Std_ReturnType ret = E_OK;
	// Set interrupt mode
	switch (Interrupt)
	{
		case EXTERNAL_INTERRUPT_0:
		MCUCR_REG = ( (MCUCR_REG & 0xF0) | (0x03 & ExtIntMode) );  
		int0Callback = ExtIntCallback;
		break;
		case EXTERNAL_INTERRUPT_1:
		MCUCR_REG = ( (MCUCR_REG & 0xF0) | ( (0x03 & ExtIntMode) << 2 ));
		int1Callback = ExtIntCallback;
		break;
		case EXTERNAL_INTERRUPT_2:
		SET_BIT(MCUCR_REG, ISC2_Bit);
		int2Callback = ExtIntCallback;
		break;
		default:
		ret = E_NOT_OK;
		break;
	}
	return ret;
}

Std_ReturnType ExternalInterrupt_Enable(ExternalInterruptType Interrupt)
{
	Std_ReturnType ret = E_OK;
	// Enable interrupt
	ENABLE_GIE();
	switch (Interrupt)
	{
		case EXTERNAL_INTERRUPT_0:
		SET_BIT(GICR_REG, INT0_Bit);
		break;
		case EXTERNAL_INTERRUPT_1:
		SET_BIT(GICR_REG, INT1_Bit);
		break;
		case EXTERNAL_INTERRUPT_2:
		SET_BIT(GICR_REG, INT2_Bit);
		break;
		default:
		ret = E_NOT_OK;
		break;
	}
	return ret;
}

Std_ReturnType ExternalInterrupt_Disable(ExternalInterruptType Interrupt)
{
	Std_ReturnType ret = E_OK;
	// Disable interrupt
	DISABLE_GIE();
	switch (Interrupt)
	{
		case EXTERNAL_INTERRUPT_0:
		CLEAR_BIT(GICR_REG, INT0_Bit);
		break;
		case EXTERNAL_INTERRUPT_1:
		CLEAR_BIT(GICR_REG, INT1_Bit);
		break;
		case EXTERNAL_INTERRUPT_2:
		CLEAR_BIT(GICR_REG, INT2_Bit);
		break;
		default:
		ret = E_NOT_OK;
		break;
	}
	return ret;
}

// Interrupt service routine for INT0
ISR (INT0_vect)
{
	if (int0Callback != NULL)
	{
		int0Callback();
	}
}
// Interrupt service routine for INT1
ISR (INT1_vect)
{
	if (int1Callback != NULL)
	{
		int1Callback();
	}
}

// Interrupt service routine for INT2
ISR (INT2_vect)
{
	if (int2Callback != NULL)
	{
		int2Callback();
	}
}