/********************************************************************************************************
*  [FILE NAME]   :      <I2C.c>                                                                        *
*  [AUTHOR]      :      <David S. Alexander>                                                           *
*  [DATE CREATED]:      <June 25, 2023>                                                                *
*  [Description} :      <Source file for the AVR I2C driver>                                           *
********************************************************************************************************/

#include "I2C.h"

Std_ReturnType I2C_Init(const I2C_Config_t* I2Ccfg)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (I2Ccfg->I2C_InterruptStatus == I2C_InterruptDisabled || I2Ccfg->I2C_InterruptStatus == I2C_InterruptEnabled)
		{
			TWCR_REG |= I2Ccfg->I2C_InterruptStatus;
		}else return E_NOT_OK;
		
		if (I2Ccfg->I2C_Prescaler >= I2C_Prescaler_1 && I2Ccfg->I2C_Prescaler <= I2C_Prescaler_64 )
		{
			TWSR_REG |= I2Ccfg->I2C_Prescaler;
		}else return E_NOT_OK;
		
		if (I2Ccfg->I2C_Address >= 0x01 && I2Ccfg->I2C_Address <= 0x7F )
		{
			TWAR_REG |= (I2Ccfg->I2C_Address & 0xFE);
		}else return E_NOT_OK;
		uint8 Prescaler = I2Ccfg->I2C_Prescaler == I2C_Prescaler_1?1:
                   		  I2Ccfg->I2C_Prescaler == I2C_Prescaler_4?4:
		                  I2Ccfg->I2C_Prescaler == I2C_Prescaler_16?16:64;
		//Prescaler = pow(4, I2Ccfg->I2C_Prescaler);
		//TWBR_REG  = ((F_CPU / (2 * Prescaler * I2Ccfg->I2C_Frequency)) - 8) / Prescaler;
		TWBR_REG = 12;
		//TWBR_REG = (F_CPU / ( 2 * I2Ccfg->I2C_Frequency * Prescaler ) ) - 8;
		//TWBR_REG = (F_CPU - (16UL * I2Ccfg->I2C_Frequency)) / ( 2UL * I2Ccfg->I2C_Frequency * 1);
	}
	TWCR_REG |= (1<<TWEN_BIT);
	return ret;
}
Std_ReturnType I2C_WriteByte(const I2C_Config_t* I2Ccfg, uint8 data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (I2Ccfg->I2C_InterruptStatus == I2C_InterruptDisabled)
		{
			TWDR_REG = data;
			SET_BIT(TWCR_REG, TWINT_BIT);
			while(BIT_IS_CLEAR(TWCR_REG,TWINT_BIT));
		}
	}
	return ret;
}
Std_ReturnType I2C_ReadByteWithAck(const I2C_Config_t* I2Ccfg, uint8* data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg || NULL == data)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TWCR_REG |= (1 << TWINT_BIT) | (1 << TWEN_BIT) | (1 << TWEA_BIT);
		while(BIT_IS_CLEAR(TWCR_REG,TWINT_BIT));
		*data = TWDR_REG;
	}
	return ret;
}
Std_ReturnType I2C_ReadByteWithNack(const I2C_Config_t* I2Ccfg, uint8* data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg || NULL == data)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TWCR_REG = (1 << TWINT_BIT) | (1 << TWEN_BIT);
		while(BIT_IS_CLEAR(TWCR_REG,TWINT_BIT));
		*data = TWDR_REG;
	}
	return ret;
}
Std_ReturnType I2C_GeneralCallRecognition(I2C_GCE_t I2Cgce)
{
	Std_ReturnType ret = E_OK;
	if (I2Cgce != I2C_DisableGCE && I2Cgce != I2C_EnableGCE)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TWAR_REG |= I2Cgce;
	}
	return ret;
}
uint8 I2C_GetStatus(void)
{
	uint8 Status;
	Status = TWSR_REG & I2C_NoRelevantStatusInformation;
	
	return Status;
}
void I2C_Start()
{
	TWCR_REG |= (1 << TWINT_BIT) | (1 << TWSTA_BIT);

	while(BIT_IS_CLEAR(TWCR_REG,TWINT_BIT));
}
void I2C_Stop()
{
	TWCR_REG |= (1 << TWINT_BIT) | (1 << TWSTO_BIT);
}