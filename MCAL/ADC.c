/********************************************************************************************************
*  [FILE NAME]   :      <ADC.h>                                                                         *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 12, 2023>                                                                 *
*  [Description] :      <Source file for the AVR ADC driver>                                            *
********************************************************************************************************/

#include "ADC.h"

/**
* @brief Configures the ADMUX register with the specified ADC settings.
* @param ADC_Config A pointer to an ADC_Channel struct containing the desired ADC settings.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*     - E_NOT_OK: The function encountered an error.
*/
static Std_ReturnType ADMUX_Config(ADC_Channel *ADC_Config)
{
	Std_ReturnType ret = E_OK;

	/* Check if ADC_Config pointer is NULL or if any of the settings are out of range */
	if (NULL == ADC_Config ||
	ADC_Config->ResultAdjust < Right_Adjusted || ADC_Config->ResultAdjust > Left_Adjusted ||
	ADC_Config->VoltageReference < External_AREF || ADC_Config->VoltageReference > Internal_VREF)
	{
		ret = E_NOT_OK;
	}
	else
	{
		/* Set the MUX bits to select the desired ADC channel */
		ADC_Config->ADMUX_Bits.MUX_bits = ADC_CHANNEL0;

		/* Set the ADLAR bit to control the result adjustment */
		ADC_Config->ADMUX_Bits.ADLAR_bit = ADC_Config->ResultAdjust;

		/* Set the REFS bits to select the desired voltage reference */
		ADC_Config->ADMUX_Bits.REFS_bits = ADC_Config->VoltageReference;

		/* Write the ADMUX register with the configured settings */
		ADMUX_REG = *(uint8*)&ADC_Config->ADMUX_Bits;
	}
	return ret;
}

/**
* @brief Configures the ADCSRA register with the specified ADC settings.
* @param ADC_Config A pointer to an ADC_Channel struct containing the desired ADC settings.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*     - E_NOT_OK: The function encountered an error.
*/
static Std_ReturnType ADCSRA_Config(ADC_Channel *ADC_Config)
{
	Std_ReturnType ret = E_OK;

	/* Check if ADC_Config pointer is NULL or if any of the settings are out of range */
	if (NULL == ADC_Config ||
	ADC_Config->Prescaller < CLK_2_ || ADC_Config->Prescaller > CLK_128 ||
	ADC_Config->InturruptMode < ADCInterruptDisabled || ADC_Config->InturruptMode > ADCInterruptEnabled)
	{
		ret = E_NOT_OK;
	}
	else
	{
		/* Set the ADPS bits to select the desired ADC prescaler */
		ADC_Config->ADCSRA_Bits.ADPS_bits = ADC_Config->Prescaller;

		/* Check if the specified ADC frequency is within the allowable range */
		if ( F_CPU /
		(ADC_Config->Prescaller <= CLK_2 ? 2 : (ADC_Config->Prescaller == CLK_4 ? 4 :
		(ADC_Config->Prescaller == CLK_8 ? 8 : (ADC_Config->Prescaller == CLK_16 ? 16 :
		(ADC_Config->Prescaller == CLK_32 ? 32 : (ADC_Config->Prescaller == CLK_64 ? 64 : 128)))))) < MIN_ADC_FREQ || ( F_CPU /
		(ADC_Config->Prescaller == CLK_2 ? 2 : (ADC_Config->Prescaller == CLK_4 ? 4 :
		(ADC_Config->Prescaller == CLK_8 ? 8 : (ADC_Config->Prescaller == CLK_16 ? 16 :
		(ADC_Config->Prescaller == CLK_32 ? 32 : (ADC_Config->Prescaller == CLK_64 ? 64 : 128)))))) > MAX_ADC_FREQ ) )
		{
			return E_NOT_OK;
		}

		/* Set the ADIE bit to control the ADC interrupt enable */
		ADC_Config->ADCSRA_Bits.ADIE_bit = ADC_Config->InturruptMode;

		/* Clear the ADIF bit */
		ADC_Config->ADCSRA_Bits.ADIF_bit  = 0;

		/* Disable auto-trigger mode */
		ADC_Config->ADCSRA_Bits.ADATE_bit = 0;

		/* Disable ADC conversion */
		ADC_Config->ADCSRA_Bits.ADSC_bit  = 0;

		/* Enable ADC */
		ADC_Config->ADCSRA_Bits.ADEN_bit  = 1;

		/* Write the ADCSRA register with the configured settings */
		ADCSRA_REG = *(uint8*)&ADC_Config->ADCSRA_Bits;
		if ( BIT_IS_SET(ADCSRA_REG, ADIE_Bit) )
		{
			ENABLE_GIE();
		}
	}
	return ret;
}

/**
* @brief Initializes the ADC with the specified settings.
* @param ADC_Config A pointer to an ADC_Channel struct containing the desired ADC settings.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*     - E_NOT_OK: The function encountered an error.
*/
Std_ReturnType ADC_Init(ADC_Channel *ADC_Config)
{
	Std_ReturnType ret = E_OK;

	/* Configure ADMUX register */
	ret = ADMUX_Config(ADC_Config);

	/* If ADMUX configuration was successful, configure ADCSRA register */
	if (ret == E_OK)
	{
		ret = ADCSRA_Config(ADC_Config);
	}
	return ret;
}

/**
* @brief Reads the ADC value from the specified ADC channel.
* @param ADC_Config A pointer to an ADC_Channel struct containing the desired ADC settings.
* @param ADC_Result A pointer to a variable to store the ADC result.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*     - E_NOT_OK: The function encountered an error.
*/
Std_ReturnType ADC_Read(ADC_Index ADC_Ch, uint16 *ADC_Result)
{
	/* Check if ADC_Config pointer is NULL */
	if (ADC_Ch < ADC_CHANNEL0 || ADC_Ch > ADC_CHANNEL7)
	{
		return E_NOT_OK;
	}

	/* Check if ADC_Result pointer is NULL */
	if (NULL == ADC_Result)
	{
		return E_NOT_OK;
	}
	
	ADMUX_REG |= (uint8)ADC_Ch;
	
	/* Start ADC conversion */
	SET_BIT(ADCSRA_REG, ADSC_Bit);                       // Set ADSC bit

	if ( BIT_IS_CLEAR(ADCSRA_REG, ADIE_Bit) )            // ADC Interrupt Disabled
	{
		/* Wait for conversion to complete */
		while ( BIT_IS_CLEAR(ADCSRA_REG, ADIF_Bit) );    // check ADIF bit
		
		/* clear ADIF bit */
		SET_BIT(ADCSRA_REG, ADIF_Bit);
	}
	
	/* Read ADC result */
	*ADC_Result = ADCL_REG | (ADCH_REG << 8);

	return E_OK;
}
ISR(ADC_vect)
{
	SET_BIT(ADCSRA_REG, ADIF_Bit);
}