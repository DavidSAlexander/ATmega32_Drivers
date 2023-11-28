
/********************************************************************************************************
*  [FILE NAME]   :      <USART.h>                                                                       *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 22, 2023>                                                                 *
*  [Description} :      <Header file for the AVR USART driver>                                          *
********************************************************************************************************/


#ifndef USART_H_
#define USART_H_

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

#define USART_TX_BUFFER_SIZE 256
#define USART_RX_BUFFER_SIZE 256

#define UDR_REG        SFR_IO8(0x0C)
#define UCSRA_REG      SFR_IO8(0x0B)
#define UCSRB_REG      SFR_IO8(0x0A)
#define UCSRC_REG      SFR_IO8(0x20)
#define UBRRH_REG      SFR_IO8(0x20)
#define UBRRL_REG      SFR_IO8(0x09)
#define UBRR_REG       SFR_IO16(0x09)

/* UCSRA */
#define RXC_BIT      7
#define TXC_BIT      6
#define UDRE_BIT     5
#define FE_BIT       4
#define DOR_BIT      3
#define PE_BIT       2
#define U2X_BIT      1
#define MPCM_BIT     0

/* UCSRB */
#define RXCIE_BIT    7
#define TXCIE_BIT    6
#define UDRIE_BIT    5
#define RXEN_BIT     4
#define TXEN_BIT     3
#define UCSZ2_BIT    2
#define RXB8_BIT     1
#define TXB8_BIT     0

/* UCSRC */
#define URSEL_BIT    7
#define UMSEL_BIT    6
#define UPM1_BIT     5
#define UPM0_BIT     4
#define USBS_BIT     3
#define UCSZ1_BIT    2
#define UCSZ0_BIT    1
#define UCPOL_BIT    0

/* UBRRH */
#define URSEL_BIT    7

typedef enum
{
	USART_2400bps   = 2400,
	USART_4800bps   = 4800,
	USART_9600bps   = 9600,
	USART_14406bps  = 14406,
	USART_19200bps  = 19200,
	USART_38400bps  = 38400,
	USART_57600bps  = 57600,
	USART_115200bps = 115200,
	USART_128000bps = 128000,
	USART_256000bps = 256000
}USART_BuadRates_t;

typedef enum
{
	USART_ParityCheckDisabled,
	USART_ParityCheckEnabledEven = 2,
	USART_ParityCheckEnabledOdd
}USART_ParityCheck_t;

typedef enum
{
	USART_1BitStop,
	USART_2BitsStop
}USART_StopBits_t;

typedef enum
{
	USART_InterruptDisabled,
	USART_InterruptEnabled
}USART_Interrupt_t;

typedef enum
{
	USART_RisingTxFallingRxEdge,
	USART_RisingRxFallingTxEdge
}USART_ClockPolaritySelect_t;

typedef enum
{
	USART_AsynchronousMode,
	USART_SynchronousMode
}USART_OperationMode_t;

typedef enum
{
	USART_5BitsDataSize,
	USART_6BitsDataSize,
	USART_7BitsDataSize,
	USART_8BitsDataSize,
	USART_9BitsDataSize = 7
}USART_DataSize_t;

typedef enum
{
	USART_DoubleSpeedDisabled = 16,
	USART_DoubleSpeedEnabled  = 8
}USART_DoubleTransmissionSpeed_t;

typedef enum
{
	USART_StringUnavailable,
	USART_StringAvailable
}USART_StringStatus_t;

typedef struct
{
  USART_BuadRates_t				     USART_BaudRate;
  USART_DataSize_t					 USART_DataSize;
  USART_OperationMode_t				 USART_OperationMode;
  USART_ClockPolaritySelect_t		 USART_ClockPolarity;
  USART_Interrupt_t					 USART_InterruptStatus;
  USART_StopBits_t					 USART_StopBits;
  USART_ParityCheck_t				 USART_ParityCheck;
  USART_DoubleTransmissionSpeed_t    USART_DoubleSpeedStatus;
  	
}USART_Config_t;

Std_ReturnType USART_Init(const USART_Config_t* USARTcfg);
Std_ReturnType USART_Transmit_Byte(const USART_Config_t* USARTcfg, uint8 data);
Std_ReturnType USART_Receive_Byte(const USART_Config_t* USARTcfg, uint8* data);
Std_ReturnType USART_Transmit_String(const USART_Config_t* USARTcfg, uint8* data);
Std_ReturnType USART_Receive_String(const USART_Config_t* USARTcfg, uint8* data);
Std_ReturnType USART_Flush(void);
Std_ReturnType USART_StringReady(USART_StringStatus_t* status);


#endif /* USART_H_ */