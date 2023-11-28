/********************************************************************************************************
*  [FILE NAME]   :      <TIMER1.1>                                                                      *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 19, 2023>                                                                 *
*  [Description} :      <Header file for the AVR Timer1 driver>                                        *
********************************************************************************************************/


#ifndef TIMER1_H_
#define TIMER1_H_

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "DIO.h"
#include "Global_Interrupt.h"

#define TCCR1A_REG      SFR_IO8(0x2F)
#define TCCR1B_REG      SFR_IO8(0x2E)
#define TCNT1H_REG      SFR_IO8(0x2D)
#define TCNT1L_REG      SFR_IO8(0x2C)
#define OCR1AH_REG      SFR_IO8(0x2B)
#define OCR1AL_REG      SFR_IO8(0x2A)
#define OCR1A_REG       SFR_IO16(0x2A)
#define OCR1BH_REG      SFR_IO8(0x29)
#define OCR1BL_REG      SFR_IO8(0x28)
#define OCR1B_REG       SFR_IO16(0x28)
#define ICR1H_REG       SFR_IO8(0x27)
#define ICR1L_REG       SFR_IO8(0x26)
#define ICR1_REG        SFR_IO16(0x26)
#define TIMSK1_REG      SFR_IO8(0x39)
#define TIFR1_REG       SFR_IO8(0x38)

/* TCCR1A */
#define WGM10_Bit   0
#define WGM11_Bit   1
#define FOC1B_Bit   2
#define FOC1A_Bit   3
#define COM1B0_Bit  4
#define COM1B1_Bit  5
#define COM1A0_Bit  6
#define COM1A1_Bit  7

/* TCCR1B */
#define CS10_Bit    0
#define CS11_Bit    1
#define CS12_Bit    2
#define WGM12_Bit   3
#define WGM13_Bit   4
#define ICES1_Bit   6
#define ICNC1_Bit   7

/* TIMSK */
#define TOIE1_Bit   2
#define OCIE1B_Bit  3
#define OCIE1A_Bit  4
#define TICIE1_Bit  5

/* TIFR */
#define TOV1_Bit    2
#define OCF1B_Bit   3
#define OCF1A_Bit   4
#define ICF1_Bit    5

/*******************************************************************************
*                         Data Types Declaration                               *
*******************************************************************************/

typedef void(*Timer1Callback)(void);

typedef enum
{
	TMR1_Normal,
	TMR1_PhaseCorrectPWM8Bit,
	TMR1_PhaseCorrectPWM9Bit,
	TMR1_PhaseCorrectPWM10Bit,
	TMR1_CTC_OCR1A,
	TMR1_FastPWM8Bit,
	TMR1_FastPWM9Bit,
	TMR1_FastPWM10Bit,
	TMR1_PhaseFrequencyCorrectPWM_ICR1,
	TMR1_PhaseFrequencyCorrectPWM_OCR1A,
	TMR1_PhaseCorrectPWM_ICR1,
	TMR1_PhaseCorrectPWM_OCR1A,
	TMR1_CTC_ICR1,
	TMR1_Reserved0,
	TMR1_FastPWM_ICR1,
	TMR1_FastPWM_OCR1A	
}Timer1_Modes_t;

typedef enum
{
	TMR1_OC1xDisabled,
	TMR1_OC1xToggleOnCompareMatch,
	TMR1_OC1xClearOnCompareMatch,
	TMR1_OC1xSetOnCompareMatch
}Timer1_OC1xModes_t;

typedef enum
{
	TMR1_COMA,
	TMR1_COMB
}CompareOutputModeUnit_t;

typedef enum
{
	TOIE1_DISABLE,
	TOIE1_ENABLE
}TOIE1_t;

typedef enum
{
	OCIE1A_DISABLE,
	OCIE1A_ENABLE
}OCIE1A_t;

typedef enum
{
	OCIE1B_DISABLE,
	OCIE1B_ENABLE
}OCIE1B_t;

typedef enum
{
	TICIE1_DISABLE,
	TICIE1_ENABLE
}TICIE1_t;

typedef enum
{
	ICES1_FallingEdge,
	ICES1_RisingEdge
}ICES1EDGE_t;
typedef enum
{
	TMR1NO_CLK_SRC,
	TMR1NO_PRESCALING,
	TMR1PRESCALING_CLK8,
	TMR1PRESCALING_CLK64,
	TMR1PRESCALING_CLK256,
	TMR1PRESCALING_CLK1024,
	TMR1EXT_CLK_FALLING,
	TMR1EXT_CLK_RISING
}Timer1_Clock_t;

typedef struct
{
	Timer1_Modes_t Mode;
	Timer1_Clock_t CLK;
	Timer1_OC1xModes_t   COM1xMode;
	CompareOutputModeUnit_t COM1xChannel;
    ICES1EDGE_t    ICU_EDGE;
}Timer1_Config_t;

Std_ReturnType TIMER1_Init(const Timer1_Config_t* TMR1_Configurations);
Std_ReturnType TIMER1_Stop();
Std_ReturnType TIMER1_ICU_Init(const Timer1_Config_t* TMR1_Configurations);
Std_ReturnType TIMER1_ICU_MeasureFreq(const Timer1_Config_t* TMR1_Configurations, uint64* Frequency);
Std_ReturnType TIMER1_EnableInterrupt(const Timer1_Config_t* TMR1_Configurations, Timer1Callback callback);
Std_ReturnType TIMER1_DisableInterrupt(const Timer1_Config_t* TMR1_Configurations, Timer1Callback callback);

Std_ReturnType TIMER1_PWM_SetDuty(const Timer1_Config_t* TMR1_Configurations, uint8 DutyCycle);

#endif /* TIMER1_H_ */