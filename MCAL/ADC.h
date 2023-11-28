/********************************************************************************************************
 *  [FILE NAME]   :      <ADC.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 12, 2023>                                                                *
 *  [Description} :      <Header file for the AVR ADC driver>                                           *
 ********************************************************************************************************/

#ifndef ADC_H_
#define ADC_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/

/* ADC Registers */
#define ADC_REG      SFR_IO16(0x04)  /* ADC Data Register */
#define ADCL_REG     SFR_IO8(0x04)   /* ADC Data Register Low */
#define ADCH_REG     SFR_IO8(0x05)   /* ADC Data Register High */
#define ADCSRA_REG   SFR_IO8(0x06)   /* ADC Control and Status Register A */
#define ADMUX_REG    SFR_IO8(0x07)   /* ADC Multiplexer Selection Register */

/* ADCSRA */
#define ADPS0_Bit   0
#define ADPS1_Bit   1
#define ADPS2_Bit   2
#define ADIE_Bit    3
#define ADIF_Bit    4
#define ADATE_Bit   5
#define ADSC_Bit    6
#define ADEN_Bit    7

/* ADMUX */
#define MUX0_Bit    0
#define MUX1_Bit    1
#define MUX2_Bit    2
#define MUX3_Bit    3
#define MUX4_Bit    4
#define ADLAR_Bit   5
#define REFS0_Bit   6
#define REFS1_Bit   7


#define MIN_ADC_FREQ 50000UL
#define MAX_ADC_FREQ 200000UL


/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

/* ADC channel index */
typedef enum
{
	ADC_CHANNEL0,   /* ADC channel 0 */
	ADC_CHANNEL1,   /* ADC channel 1 */
	ADC_CHANNEL2,   /* ADC channel 2 */
	ADC_CHANNEL3,   /* ADC channel 3 */
	ADC_CHANNEL4,   /* ADC channel 4 */
	ADC_CHANNEL5,   /* ADC channel 5 */
	ADC_CHANNEL6,   /* ADC channel 6 */
	ADC_CHANNEL7    /* ADC channel 7 */
}ADC_Index;

/* ADC prescaler select */
typedef enum
{
	CLK_2_,         /* Clock / 2 */
	CLK_2,          /* Clock / 2 */
	CLK_4,          /* Clock / 4 */
	CLK_8,          /* Clock / 8 */
	CLK_16,         /* Clock / 16 */
	CLK_32,         /* Clock / 32 */
	CLK_64,         /* Clock / 64 */
	CLK_128         /* Clock / 128 */
}ADC_Prescaller;

/* ADC result adjustment select */
typedef enum
{
	Right_Adjusted, /* Right-adjusted result */
	Left_Adjusted   /* Left-adjusted result */
}ADC_ResultAdjust;

/* ADC interrupt select */
typedef enum
{
	ADCInterruptDisabled,  /* Interrupt disabled */
	ADCInterruptEnabled    /* Interrupt enabled */
}ADC_Interrupt;

/* ADC voltage reference options */
typedef enum
{
	External_AREF,      /* External AREF voltage reference */
	External_AVCC,      /* External AVCC voltage reference */
	Internal_VREF = 3   /* Internal VREF voltage reference */
}ADC_VoltageReference;

/* ADC Multiplixer options */
typedef struct
{
	uint8 MUX_bits  :5;
	uint8 ADLAR_bit :1;
	uint8 REFS_bits :2;
}ADMUX_t;

/* ADC ADCSRA options */
typedef struct
{
	uint8 ADPS_bits :3;
	uint8 ADIE_bit  :1;
	uint8 ADIF_bit  :1;
	uint8 ADATE_bit :1;
	uint8 ADSC_bit  :1;
	uint8 ADEN_bit  :1;
}ADCSRA_t;

/* ADC channel configuration */
typedef struct
{
	ADC_ResultAdjust      ResultAdjust;        /* ADC result adjustment */
	ADC_Prescaller        Prescaller;          /* ADC prescaler */
	ADC_VoltageReference  VoltageReference;    /* ADC voltage reference */
	ADC_Interrupt         InturruptMode;       /* ADC interrupt mode */
	ADCSRA_t              ADCSRA_Bits;         /* ADC ADCSRA Bits */
	ADMUX_t               ADMUX_Bits;          /* ADC multiplixer Bits */
}ADC_Channel;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

/* Initialize ADC module */
Std_ReturnType ADC_Init(ADC_Channel *ADC_Num);

/* Read from ADC module */
Std_ReturnType ADC_Read(ADC_Index ADC_Ch, uint16 *ADC_Result);

#endif /* ADC_H_ */