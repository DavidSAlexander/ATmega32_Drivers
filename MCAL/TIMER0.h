/********************************************************************************************************
*  [FILE NAME]   :      <TIMER0.h>                                                                      *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 18, 2023>                                                                 *
*  [Description} :      <Header file for the AVR Timer0 driver>                                         *
********************************************************************************************************/


#ifndef TIMER0_H_
#define TIMER0_H_

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

#define TCCR0_REG      SFR_IO8(0x33)
#define TCNT0_REG      SFR_IO8(0x32)
#define OCR0_REG       SFR_IO8(0x3C)
#define TIMSK0_REG     SFR_IO8(0x39)
#define TIFR0_REG      SFR_IO8(0x38)

/* TCCR0 */
#define CS00_Bit    0
#define CS01_Bit    1
#define CS02_Bit    2
#define WGM01_Bit   3
#define COM00_Bit   4
#define COM01_Bit   5
#define WGM00_Bit   6
#define FOC0_Bit    7

/* TIMSK */
#define TOIE0_Bit   0 
#define OCIE0_Bit   1

/* TIFR */
#define TOV0_Bit    0
#define OCF0_Bit    1


/*******************************************************************************
*                         Data Types Declaration                               *
*******************************************************************************/

typedef void(*Timer0Callback)(void);

typedef enum
{
	TMR0_Normal,
	TMR0_PhaseCorrectPWM,
	TMR0_CTC,
	TMR0_FastPWM
}Timer0_Modes_t;

typedef enum
{
	TMR0_OC0Disabled,
	TMR0_OC0ToggleOnCompareMatch,
	TMR0_OC0ClearOnCompareMatch,
	TMR0_OC0SetOnCompareMatch
}PWM0_Modes_t;

typedef enum
{
	TOIE0_DISABLE = 0x00,
	TOIE0_ENABLE  = 0x01
}TOIE0_t;

typedef enum
{
	OCIE0_DISABLE = 0x00,
	OCIE0_ENABLE  = 0x02
}OCIE0_t;

typedef enum
{
	TMR0NO_CLK_SRC,
	TMR0NO_PRESCALING,
	TMR0PRESCALING_CLK8,
	TMR0PRESCALING_CLK64,
	TMR0PRESCALING_CLK256,
	TMR0PRESCALING_CLK1024,
	TMR0EXT_CLK_FALLING,
	TMR0EXT_CLK_RISING
}Timer0_Clock_t;

typedef struct
{
	Timer0_Modes_t Mode;
	Timer0_Clock_t CLK;
	PWM0_Modes_t   OC0Mode;
}Timer0_Config_t;


/*******************************************************************************
*                            Functions Declaration                             *
*******************************************************************************/

Std_ReturnType TIMER0_Init(const Timer0_Config_t* TMR0_Configurations);
Std_ReturnType TIMER0_Stop();

Std_ReturnType TIMER0_GetOCR0(const Timer0_Config_t* TMR0_Configurations, uint8* TicksNumber);
Std_ReturnType TIMER0_SetOCR0(const Timer0_Config_t* TMR0_Configurations, uint8 TicksNumber);

Std_ReturnType TIMER0_GetTCNT0(const Timer0_Config_t* TMR0_Configurations, uint8* TicksNumber);
Std_ReturnType TIMER0_SetTCNT0(const Timer0_Config_t* TMR0_Configurations, uint8 TicksNumber);

Std_ReturnType TIMER0_EnableInterrupt(const Timer0_Config_t* TMR0_Configurations, Timer0Callback callback);
Std_ReturnType TIMER0_DisableInterrupt(const Timer0_Config_t* TMR0_Configurations, Timer0Callback callback);

Std_ReturnType TIMER0_PWM_SetDuty(const Timer0_Config_t* TMR0_Configurations, uint8 DutyCycle);
#endif /* TIMER0_H_ */