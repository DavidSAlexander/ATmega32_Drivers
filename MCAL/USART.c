
/********************************************************************************************************
*  [FILE NAME]   :      <USART.c>                                                                       *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 22, 2023>                                                                 *
*  [Description} :      <Source file for the AVR USART driver>                                          *
********************************************************************************************************/

#include "USART.h"

static uint8 RX_Buffer[USART_TX_BUFFER_SIZE];
static volatile uint8 RX_BufferHead = 0;
static volatile uint8 RX_BufferTail = 0;

static uint8 TX_Buffer[USART_TX_BUFFER_SIZE];
static volatile uint8 TX_BufferHead = 0;
static volatile uint8 TX_BufferTail = 0;

static volatile uint8 TX_IndexBuffer = 0;
static volatile uint8 RX_IndexBuffer = 0;
static volatile boolean RX_DataIsReady = FALSE;
static volatile boolean RX_DataIsAvailable = FALSE;

Std_ReturnType USART_Init(const USART_Config_t* USARTcfg)
{
	Std_ReturnType ret = E_OK;
	
	uint8  UCSRA_Init = 0;
	uint8  UCSRB_Init = 0;
	uint8  UCSRC_Init = 0;
	uint16 UBRR_Init  = 0;
	
	uint8 Divisor = 16;
	if ( USARTcfg->USART_DoubleSpeedStatus == USART_DoubleSpeedEnabled && USARTcfg->USART_OperationMode == USART_AsynchronousMode  )
	Divisor = 8;
	
	/* Set the USART Data Size */
	UCSRC_Init |= (USARTcfg->USART_DataSize & 0x03) << UCSZ0_BIT;

	/* Set the USART Operation Mode */
	if (USARTcfg->USART_OperationMode == USART_SynchronousMode)
	{
		UCSRC_Init |= (1 << UMSEL_BIT);
	}

	/* Set the USART Clock Polarity */
	if (USARTcfg->USART_ClockPolarity == USART_RisingRxFallingTxEdge)
	{
		UCSRC_Init |= (1 << UCPOL_BIT);
	}

	/* Set the USART Stop Bits */
	if (USARTcfg->USART_StopBits == USART_2BitsStop)
	{
		UCSRC_Init |= (1 << USBS_BIT);
	}

	/* Set the USART Parity Check */
	UCSRC_Init |= (USARTcfg->USART_ParityCheck << 4);

	/* Set the USART Double Transmission Speed */
	if (USARTcfg->USART_DoubleSpeedStatus == USART_DoubleSpeedEnabled)
	{
		UCSRA_Init |= (1 << U2X_BIT);
	}

	/* Set the USART Interrupt Status */
	if (USARTcfg->USART_InterruptStatus == USART_InterruptEnabled)
	{
		ENABLE_GIE();
		UCSRB_Init |= (1 << RXCIE_BIT) | (1 << TXCIE_BIT) | (1 << UDRIE_BIT);
	}

	UBRR_Init = F_CPU / Divisor / USARTcfg->USART_BaudRate - 1;
	UBRRH_REG = (uint8) (UBRR_Init >> 8);
	UBRRL_REG = (uint8) (UBRR_Init & 0xFF);

	UCSRA_REG = UCSRA_Init;
	UCSRB_REG = UCSRB_Init | (1 << RXEN_BIT) | (1 << TXEN_BIT);
	UCSRC_REG = UCSRC_Init | (1 << URSEL_BIT);
	return ret;
}

Std_ReturnType USART_Transmit_Byte(const USART_Config_t* USARTcfg, uint8 data)
{
	Std_ReturnType ret = E_OK;
	if (USARTcfg->USART_InterruptStatus == USART_InterruptEnabled)
	{
		if ( (TX_BufferHead + 1) % USART_TX_BUFFER_SIZE  != TX_BufferTail)
		{
			TX_Buffer[TX_BufferHead] = data;
			TX_BufferHead = (TX_BufferHead + 1) % USART_TX_BUFFER_SIZE;
			SET_BIT(UCSRB_REG, UDRIE_BIT);
		}
		else return ret;
	}
	else if (USARTcfg->USART_InterruptStatus == USART_InterruptDisabled)
	{
		if ((TX_BufferHead + 1) % USART_TX_BUFFER_SIZE  != TX_BufferTail)
		{
			TX_Buffer[TX_BufferHead] = data;
			TX_BufferHead = (TX_BufferHead + 1) % USART_TX_BUFFER_SIZE;
		}
		
		while (TX_BufferHead != TX_BufferTail)
		{
			UDR_REG = TX_Buffer[TX_BufferTail];
			TX_BufferTail = (TX_BufferTail + 1) % USART_TX_BUFFER_SIZE;
			while (BIT_IS_CLEAR(UCSRA_REG,TXC_BIT));
			SET_BIT(UCSRA_REG,TXC_BIT);
		}
	}
	return ret;
}

Std_ReturnType USART_Receive_Byte(const USART_Config_t* USARTcfg, uint8* data)
{
	Std_ReturnType ret = E_OK;
	uint8 BufferIndex = RX_BufferHead;
	if (USARTcfg->USART_InterruptStatus == USART_InterruptEnabled )
	{
		if (BufferIndex != RX_BufferTail)
		{
			*data = RX_Buffer[RX_BufferTail];
			RX_BufferTail = (RX_BufferTail + 1) % USART_RX_BUFFER_SIZE;
			RX_DataIsReady = TRUE;
		}
	}

	else if (USARTcfg->USART_InterruptStatus == USART_InterruptDisabled )
	{
		if (BufferIndex != RX_BufferTail)
		{
			*data = RX_Buffer[RX_BufferTail];
			RX_BufferTail = (RX_BufferTail + 1) % USART_RX_BUFFER_SIZE;
			RX_DataIsReady = TRUE;
		}
		else if ( BufferIndex == RX_BufferTail )
		{
			// Wait for data to be received
			while(BIT_IS_CLEAR(UCSRA_REG, RXC_BIT));
			SET_BIT(UCSRA_REG,TXC_BIT);
			
			// Check for errors
			if (BIT_IS_SET(UCSRA_REG, FE_BIT) || BIT_IS_SET(UCSRA_REG, DOR_BIT) || BIT_IS_SET(UCSRA_REG, PE_BIT))
			{
				if (UDR_REG);      // Clear error flags
				ret = E_NOT_OK;
			}
			else
			{
				if (((RX_BufferHead + 1) % USART_RX_BUFFER_SIZE) != RX_BufferTail)
				{
					RX_Buffer[RX_BufferHead] = UDR_REG;
					RX_BufferHead = (RX_BufferHead + 1) % USART_RX_BUFFER_SIZE;
				}
			}
		}
	}
	return ret;
}

Std_ReturnType USART_Transmit_String(const USART_Config_t* USARTcfg, uint8* data)
{
	Std_ReturnType ret = E_OK;
	uint8 Counter = ZERO_INIT;
	while (data[Counter] != '\0')
	{
		USART_Transmit_Byte(USARTcfg, data[Counter++]);
		_delay_ms(1);
	}
	return ret;
}

Std_ReturnType USART_Receive_String(const USART_Config_t* USARTcfg, uint8* str)
{
	Std_ReturnType ret = E_OK;
	uint8 ReceivedCharacter = ZERO_INIT;
	uint8 strBuffer[USART_RX_BUFFER_SIZE];
	USART_Receive_Byte(USARTcfg, &ReceivedCharacter);
	if (  RX_DataIsReady == TRUE )
	{
		strBuffer[RX_IndexBuffer] = ReceivedCharacter;
		RX_DataIsReady = FALSE;
		if ( strBuffer[RX_IndexBuffer] == '\r' )
		{
			strBuffer[RX_IndexBuffer] = '\0';
			memcpy(str, strBuffer, RX_IndexBuffer + 1);
			RX_IndexBuffer = ZERO_INIT;
			RX_DataIsAvailable = TRUE;
		}
		
		else if ( strBuffer[RX_IndexBuffer] != '\r' && strBuffer[RX_IndexBuffer] != '\0' )
		{
			//str[RX_IndexBuffer] = ReceivedCharacter;
			RX_IndexBuffer = ( RX_IndexBuffer + 1 ) % USART_RX_BUFFER_SIZE;
		}
		
	}
	return ret;
}
Std_ReturnType USART_StringReady(USART_StringStatus_t* status)
{
	Std_ReturnType ret = E_OK;
	if (NULL == status || *status < USART_StringUnavailable || *status > USART_StringAvailable) 
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (RX_DataIsAvailable == TRUE)
		{
			*status = USART_StringAvailable;
			RX_DataIsAvailable = FALSE;
		}
		else
		{
			*status = USART_StringUnavailable;
		}
	}
	return ret;
}
Std_ReturnType USART_Flush(void)
{
	while ( UCSRA & (1<<RXC) ) if (UDR_REG);
	return E_OK;
}

ISR(USART_RXC_vect)
{
	if (((RX_BufferHead + 1) % USART_RX_BUFFER_SIZE) != RX_BufferTail)
	{
		RX_Buffer[RX_BufferHead] = UDR_REG;
		RX_BufferHead = (RX_BufferHead + 1) % USART_RX_BUFFER_SIZE;
		if (BIT_IS_SET(UCSRA_REG, FE_BIT) || BIT_IS_SET(UCSRA_REG, DOR_BIT) || BIT_IS_SET(UCSRA_REG, PE_BIT))
		{
			if (UDR_REG);      // Clear error flags
		}
	}
}
ISR(USART_TXC_vect)
{
	//TX_BufferHead = (TX_BufferHead + 1) % RX_BUFFER_SIZE;
}
ISR(USART_UDRE_vect)
{
	if (TX_BufferHead != TX_BufferTail)
	{
		UDR_REG = TX_Buffer[TX_BufferTail];
		TX_BufferTail = (TX_BufferTail + 1) % USART_TX_BUFFER_SIZE;
	}
	CLEAR_BIT(UCSRB_REG, UDRIE_BIT);
}