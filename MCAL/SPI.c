/*******************************************************************************************************
*  [FILE NAME]   :      <SPI.h>                                                                        *
*  [AUTHOR]      :      <David S. Alexander>                                                           *
*  [DATE CREATED]:      <June 25, 2023>                                                                *
*  [Description} :      <Header file for the AVR SPI driver>                                           *
********************************************************************************************************/

#include "SPI.h"

static uint8 SPI_RX_Buffer[SPI_TX_BUFFER_SIZE];
static volatile uint8 SPI_RX_BufferHead = 0;
static volatile uint8 SPI_RX_BufferTail = 0;

static uint8 SPI_TX_Buffer[SPI_TX_BUFFER_SIZE];
static volatile uint8 SPI_TX_BufferHead = 0;
static volatile uint8 SPI_TX_BufferTail = 0;

static volatile boolean RX_DataIsReady = FALSE;
static volatile boolean RX_DataIsAvailable = FALSE;
static volatile boolean TX_DataIsReady = FALSE;
static volatile boolean TX_DataIsAvailable = FALSE;

static volatile uint8 SPI_OperationMode = 0;

static Std_ReturnType SPI_Shift(uint8 *Data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == Data)
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint8 TimeElapsed = 0;
		SPDR_REG = 0xFF;
		while (!(SPSR_REG & (1 << SPIF_BIT)))
		{
			_delay_ms(1);
			TimeElapsed++;
			if (TimeElapsed >= 50)
			return E_NOT_OK;
		}
		*Data = SPDR_REG;
	}
	return ret;
}
Std_ReturnType SPI_Init(const SPI_Config_t* SPIcfg)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint8 SPCR_Init = 0x00;
		if (SPIcfg->SPI_Direction == SPI_Master || SPIcfg->SPI_Direction == SPI_Slave)
		{
			SPCR_Init |= (1<<SPE_BIT);
			if (SPIcfg->SPI_Direction == SPI_Master)
			{
				SPI_DDR |=  ( (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN) );
				SPI_DDR &= ~(1 << MISO_PIN);
				SPCR_Init |= (1 << MSTR_BIT);
			}
			else if (SPIcfg->SPI_Direction == SPI_Slave)
			{
				SPI_DDR |= (1<<MISO_PIN);
				SPI_DDR &= ~((1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN));
			}
		}
		else return E_NOT_OK;
		
		if (SPIcfg->SPI_Mode >= SPI_Mode0 && SPIcfg->SPI_Mode <= SPI_Mode3)
		{
			SPCR_Init |= (SPIcfg->SPI_Mode << 2);
		}
		else return E_NOT_OK;
		
		if (SPIcfg->SPI_Freq >= SPI_Freq_Prescaller_4 && SPIcfg->SPI_Freq <= SPI_Freq_Prescaller_64_2X &&
		SPIcfg->SPI_DoupleSpeedMode >= SPI_DoupleSpeedDisabled && SPIcfg->SPI_DoupleSpeedMode <= SPI_SPI_DoupleSpeedEnabled)
		{
			SPCR_Init |= (SPIcfg->SPI_Freq & 0x03);
			SPCR_Init |= (SPIcfg->SPI_Freq & 0x04);
		}
		
		if (SPIcfg->SPI_DataOrder <= SPI_LSB && SPIcfg->SPI_DataOrder >= SPI_MSB)
		{
			SPCR_Init |= (SPIcfg->SPI_DataOrder << DORD_BIT);
		}
		else return E_NOT_OK;
		
		if (SPIcfg->SPI_Interrupt >= SPI_InterruptDisabled && SPIcfg->SPI_Interrupt <= SPI_InterruptEnabled )
		{
			SPCR_Init |= (SPIcfg->SPI_Interrupt << SPIE_BIT);
			ENABLE_GIE();
		}
		else return E_NOT_OK;
		SPCR_REG = SPCR_Init;
	}
	return ret;
}
Std_ReturnType SPI_Transfer(const SPI_Config_t* SPIcfg, uint8 *data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (SPIcfg->SPI_Interrupt == SPI_InterruptDisabled)
		{
			
			uint8 TimeElapsed = 0;
			SPDR_REG = *data;
			while (!(SPSR_REG & (1 << SPIF_BIT)))
			{
				_delay_ms(1);
				TimeElapsed++;
				if (TimeElapsed >= 50)
				return E_NOT_OK;
			}
			*data = SPDR_REG;
		}
		else if (SPIcfg->SPI_Interrupt == SPI_InterruptEnabled)
		{
			if (SPI_TX_BufferHead != SPI_TX_BufferTail)
			{
				SPDR_REG = SPI_TX_Buffer[SPI_TX_BufferTail];
				*data = SPDR_REG;
			}
			if ( (SPI_TX_BufferHead + 1) % SPI_TX_BUFFER_SIZE  != SPI_TX_BufferTail)
			{
				SPI_TX_Buffer[SPI_TX_BufferHead] = *data;
				SPI_TX_BufferHead = (SPI_TX_BufferHead + 1) % SPI_TX_BUFFER_SIZE;
			}
		}
		else ret = E_NOT_OK;
	}
	return ret;
}
Std_ReturnType SPI_Send(const SPI_Config_t* SPIcfg, uint8 data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (SPIcfg->SPI_Interrupt == SPI_InterruptDisabled)
		{
			if (!(SPSR_REG & (1 << SPIF_BIT)))
			SPDR_REG = data;
			//while (!(SPSR_REG & (1 << SPIF_BIT)));
		}
		else if (SPIcfg->SPI_Interrupt == SPI_InterruptEnabled)
		{
			SPI_OperationMode = SPI_MasterSendOperation;
			if (SPI_TX_BufferHead != SPI_TX_BufferTail && TX_DataIsAvailable == FALSE)
			{
				SPDR_REG = SPI_TX_Buffer[SPI_TX_BufferTail];
				TX_DataIsAvailable = TRUE;
			}
			if ( (SPI_TX_BufferHead + 1) % SPI_TX_BUFFER_SIZE  != SPI_TX_BufferTail)
			{
				SPI_TX_Buffer[SPI_TX_BufferHead] = data;
				SPI_TX_BufferHead = (SPI_TX_BufferHead + 1) % SPI_TX_BUFFER_SIZE;
			}
		}
	}
	return ret;
}
Std_ReturnType SPI_Receive(const SPI_Config_t* SPIcfg, uint8 *Data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (SPIcfg->SPI_Interrupt == SPI_InterruptDisabled)
		{
			if ( SPIcfg->SPI_Direction == SPI_Master )
			{
				ret = SPI_Shift(Data);
			}
			else
			{
				if ((SPSR_REG & (1 << SPIF_BIT)))
				*Data = SPDR_REG;
			}
		}
		else if (SPIcfg->SPI_Interrupt == SPI_InterruptEnabled)
		{
			if ( SPIcfg->SPI_Direction == SPI_Master )
			{
				SPI_OperationMode = SPI_MasterReceiveOperation;
				if (RX_DataIsAvailable == FALSE)
				{
					SPDR_REG = 0xFF;
					RX_DataIsAvailable = TRUE;
				}
				if (RX_DataIsReady == TRUE)
				{
					*Data = SPDR_REG;
					RX_DataIsReady = FALSE;
				}
			}
			else
			{
				SPI_OperationMode = SPI_SlaveReceiveOperation;
				if (SPI_RX_BufferHead != SPI_RX_BufferTail)
				{
					*Data = SPI_RX_Buffer[SPI_RX_BufferTail];
					SPI_RX_BufferTail = (SPI_RX_BufferTail + 1) % SPI_RX_BUFFER_SIZE;
				}
			}
		}
	}
	return ret;
}
Std_ReturnType SPI_TransferDMA(const SPI_Config_t* SPIcfg, uint8 *TX_Data, uint8 *RX_Data, uint16 Length)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		
	}
	return ret;
}
Std_ReturnType SPI_IsBusy(const SPI_Config_t* SPIcfg, SPI_LineStatus_t *State)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		
	}
	return ret;
}
Std_ReturnType SPI_Set_SS(const SPI_Config_t* SPIcfg, SPI_SetSS_t State)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg || SPIcfg->SPI_Direction == SPI_Slave)
	{
		ret = E_NOT_OK;
	}
	else
	{
		SPI_PORT |= (State<<SS_PIN);
	}
	return ret;
}
Std_ReturnType SPI_DaisyChain(const SPI_Config_t* SPIcfg, uint8 *TX_Data, uint8 *RX_Data, uint16 Length, uint8 NumOfDevices)
{
	Std_ReturnType ret = E_OK;
	if (NULL == SPIcfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		
	}
	return ret;
}

Std_ReturnType SPI_ClearReadBuffer(void)
{
	SPI_RX_BufferHead = 0;
	SPI_RX_BufferTail = 0;
	return E_OK;
}

Std_ReturnType SPI_ClearWriteBuffer(void)
{
	SPI_TX_BufferHead = 0;
	SPI_TX_BufferTail = 0;
	return E_OK;
}

ISR(SPI_STC_vect)
{
	if (SPCR_REG & (1 << MSTR_BIT))    // Master
	{
		if ( SPI_OperationMode == SPI_MasterReceiveOperation )
		{
			RX_DataIsReady = TRUE;
		}
		else if ( SPI_OperationMode == SPI_MasterSendOperation )
		{
			TX_DataIsAvailable = FALSE;
			SPI_TX_BufferTail = (SPI_TX_BufferTail + 1) % SPI_TX_BUFFER_SIZE;
		}
	}
	else                                // Slave
	{
		if ( SPI_OperationMode == SPI_SlaveReceiveOperation )
		{
			if (((SPI_RX_BufferHead + 1) % SPI_RX_BUFFER_SIZE) != SPI_RX_BufferTail)
			{
				SPI_RX_Buffer[SPI_RX_BufferHead] = SPDR_REG;
				SPI_RX_BufferHead = (SPI_RX_BufferHead + 1) % SPI_RX_BUFFER_SIZE;
			}
		}
		else if ( SPI_OperationMode == SPI_MasterSendOperation )
		{
			TX_DataIsAvailable = FALSE;
			SPI_TX_BufferTail = (SPI_TX_BufferTail + 1) % SPI_TX_BUFFER_SIZE;
		}
	}
}