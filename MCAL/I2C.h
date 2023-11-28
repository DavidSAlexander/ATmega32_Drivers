/********************************************************************************************************
 *  [FILE NAME]   :      <I2C.c>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 26, 2023>                                                                *
 *  [Description} :      <Header file for the AVR I2C driver>                                           *
 ********************************************************************************************************/

#ifndef I2C_H_
#define I2C_H_

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

#define TWBR_REG      SFR_IO8(0x00)
#define TWSR_REG      SFR_IO8(0x01)
#define TWAR_REG      SFR_IO8(0x02)
#define TWDR_REG      SFR_IO8(0x03)
#ifndef TWCR_REG
#define TWCR_REG      SFR_IO8(0x36)
#endif
 
/* TWCR */
#define TWINT_BIT   7
#define TWEA_BIT    6
#define TWSTA_BIT   5
#define TWSTO_BIT   4
#define TWWC_BIT    3
#define TWEN_BIT    2
#define TWIE_BIT    0

/* TWAR */
#define TWA6_BIT    7
#define TWA5_BIT    6
#define TWA4_BIT    5
#define TWA3_BIT    4
#define TWA2_BIT    3
#define TWA1_BIT    2
#define TWA0_BIT    1
#define TWGCE_BIT   0

/* TWSR */
#define TWS7_BIT    7
#define TWS6_BIT    6
#define TWS5_BIT    5
#define TWS4_BIT    4
#define TWS3_BIT    3
#define TWPS1_BIT   1
#define TWPS0_BIT   0

#define I2C_StartConditionTransmitted                      0x08
#define I2C_RepeatedStartConditionTransmitted              0x10
#define I2C_SLA_WTransmitted_ACKReceived                   0x18
#define I2C_SLA_WTransmitted_NACKReceived                  0x20
#define I2C_DataByteTransmitted_ACKReceived                0x28
#define I2C_DataByteTransmitted_NACKReceived               0x30
#define I2C_ArbitrationLost_SLA_W                          0x38
#define I2C_SLA_RTransmitted_ACKReceived                   0x40
#define I2C_SLA_RTransmitted_NACKReceived                  0x48
#define I2C_DataByteReceived_ACKReturned                   0x50
#define I2C_DataByteReceived_NACKReturned                  0x58
#define I2C_OwnSLA_WReceived_ACKReturned                   0x60
#define I2C_ArbitrationLost_OwnSLA_W_ACKReturned           0x68
#define I2C_GeneralCallAddressReceived_ACKReturned         0x70
#define I2C_ArbitrationLost_GeneralCall_ACKReturned        0x78
#define I2C_DataByteReceivedUsingGeneralCall_ACKReturned   0x80
#define I2C_DataByteReceivedUsingGeneralCall_NACKReturned  0x88
#define I2C_OwnSLA_RReceived_ACKReturned                   0x90
#define I2C_ArbitrationLost_OwnSLA_R_ACKReturned           0x98
#define I2C_STOPorRepeatedSTARTReceived                    0xA0
#define I2C_SLA_RTransmitted_ACKReturned                   0xA8
#define I2C_ArbitrationLost_SLA_R_ACKReturned              0xB0

#define I2C_LastDataByteTransmitted_ACKReceived            0xC8
#define I2C_NoRelevantStatusInformation                    0xF8

typedef enum
{
	I2C_InterruptDisabled,
	I2C_InterruptEnabled
} I2C_InterruptStatus_t;

typedef enum
{
	I2C_Read,
	I2C_Write
} I2C_ReadWrite_t;

typedef enum
{
	I2C_Master,
	I2C_Slave
} I2C_Mode_t;

typedef enum
{
	I2C_DisableGCE,
	I2C_EnableGCE
} I2C_GCE_t;

typedef enum
{
	I2C_Prescaler_1,
	I2C_Prescaler_4,
	I2C_Prescaler_16,
	I2C_Prescaler_64
} I2C_Prescaler_t;

typedef struct
{
	I2C_InterruptStatus_t  I2C_InterruptStatus;
	I2C_Prescaler_t        I2C_Prescaler;
	uint32                 I2C_Frequency;
	I2C_Mode_t             I2C_Mode;
	uint8                  I2C_Address;
} I2C_Config_t;

Std_ReturnType I2C_Init(const I2C_Config_t* I2Ccfg);
Std_ReturnType I2C_WriteByte(const I2C_Config_t* I2Ccfg, uint8 data);
Std_ReturnType I2C_ReadByteWithAck(const I2C_Config_t* I2Ccfg, uint8* data);
Std_ReturnType I2C_ReadByteWithNack(const I2C_Config_t* I2Ccfg, uint8* data);
Std_ReturnType I2C_GeneralCallRecognition(I2C_GCE_t I2Cgce);
uint8          I2C_GetStatus(void);
void I2C_Start();
void I2C_Stop();


#endif /* I2C_H_ */