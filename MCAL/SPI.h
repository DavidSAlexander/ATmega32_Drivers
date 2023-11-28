/********************************************************************************************************
 *  [FILE NAME]   :      <SPI.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 25, 2023>                                                                *
 *  [Description} :      <Header file for the AVR SPI driver>                                           *
 ********************************************************************************************************/


#ifndef SPI_H_
#define SPI_H_

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

#define SPI_TX_BUFFER_SIZE 256
#define SPI_RX_BUFFER_SIZE 256

#define SPCR_REG      SFR_IO8(0x0D)
#define SPSR_REG      SFR_IO8(0x0E)
#define SPDR_REG      SFR_IO8(0x0F)

#define SPI_DDR       SFR_IO8(0x17)
#define SPI_PORT      SFR_IO8(0x18)

/* SPCR_REG */
#define SPIE_BIT      7
#define SPE_BIT       6
#define DORD_BIT      5
#define MSTR_BIT      4
#define CPOL_BIT      3
#define CPHA_BIT      2
#define SPR1_BIT      1
#define SPR0_BIT      0

/* SPSR_REG */
#define SPIF_BIT      7
#define WCOL_BIT      6
#define SPI2X_BIT     0

/* SPI_PINS */
#define SCK_PIN       7
#define MISO_PIN      6
#define MOSI_PIN      5
#define SS_PIN        4

#define SPI_MasterReceiveOperation   1
#define SPI_SlaveReceiveOperation    2
#define SPI_MasterSendOperation      3
#define SPI_MasterTransferOperation  5

typedef enum
{
	SPI_Mode0,
	SPI_Mode1,
	SPI_Mode2,
	SPI_Mode3
}SPI_Modes_t;

typedef enum
{
	SPI_Freq_Prescaller_4,
	SPI_Freq_Prescaller_16,
	SPI_Freq_Prescaller_64,
	SPI_Freq_Prescaller_128,
	SPI_Freq_Prescaller_2_2X,
	SPI_Freq_Prescaller_8_2X,
	SPI_Freq_Prescaller_32_2X,
	SPI_Freq_Prescaller_64_2X
}SPI_Frequencies_t;

typedef enum
{
	SPI_Master,
	SPI_Slave
}SPI_Direction_t;

typedef enum
{
	SPI_DoupleSpeedDisabled,
	SPI_SPI_DoupleSpeedEnabled
}SPI_DoupleSpeed_t;

typedef enum
{
	SPI_MSB,
	SPI_LSB
}SPI_DataOrder_t;  

typedef enum
{
	SPI_Idle,
	SPI_Busy
}SPI_LineStatus_t;

typedef enum
{
	SPI_InterruptDisabled,
	SPI_InterruptEnabled
}SPI_Interrupt_t;

typedef enum
{
	SPI_EnableSS,
	SPI_DisableSS
}SPI_SetSS_t;

typedef struct  
{
	SPI_Modes_t         SPI_Mode;
	SPI_Frequencies_t   SPI_Freq;
	SPI_Interrupt_t     SPI_Interrupt;
	SPI_DataOrder_t     SPI_DataOrder;
	SPI_DoupleSpeed_t   SPI_DoupleSpeedMode;
	SPI_Direction_t     SPI_Direction;
}SPI_Config_t;

Std_ReturnType SPI_Init(const SPI_Config_t* SPIcfg);
Std_ReturnType SPI_Transfer(const SPI_Config_t* SPIcfg, uint8* data);
Std_ReturnType SPI_Send(const SPI_Config_t* SPIcfg, uint8 data);
Std_ReturnType SPI_Receive(const SPI_Config_t* SPIcfg, uint8 *Data);
Std_ReturnType SPI_TransferDMA(const SPI_Config_t* SPIcfg, uint8 *TX_Data, uint8 *RX_Data, uint16 Length);
Std_ReturnType SPI_IsBusy(const SPI_Config_t* SPIcfg, SPI_LineStatus_t *State);
Std_ReturnType SPI_Set_SS(const SPI_Config_t* SPIcfg, SPI_SetSS_t State);
Std_ReturnType SPI_DaisyChain(const SPI_Config_t* SPIcfg, uint8 *TX_Data, uint8 *RX_Data, uint16 Length, uint8 NumOfDevices);
Std_ReturnType SPI_ClearReadBuffer(void);
Std_ReturnType SPI_ClearWriteBuffer(void);


#endif /* SPI_H_ */