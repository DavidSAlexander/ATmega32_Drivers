
/********************************************************************************************************
 *  [FILE NAME]   :      <External_Interrupt.h>                                                         *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 17, 2023>                                                                *
 *  [Description} :      <Header file for the AVR External Interrupt driver>                            *
 ********************************************************************************************************/
#ifndef EXTERNAL_INTERRUPT_H_
#define EXTERNAL_INTERRUPT_H_ 

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"


#define GIFR_REG      SFR_IO8(0x3A)
#define GICR_REG      SFR_IO8(0x3B)
#define MCUCSR_REG    SFR_IO8(0x34)
#define MCUCR_REG     SFR_IO8(0x35)


/* MCUCR */
#define ISC11_Bit   3
#define ISC10_Bit   2
#define ISC01_Bit   1
#define ISC00_Bit   0

/* MCUCSR */
#define ISC2_Bit    6

/* GICR */
#define INT1_Bit    7
#define INT0_Bit    6
#define INT2_Bit    5

/* GIFR */
#define INTF1_Bit   7
#define INTF0_Bit   6
#define INTF2_Bit   5

typedef enum {
	EXTERNAL_INTERRUPT_0,
	EXTERNAL_INTERRUPT_1,
	EXTERNAL_INTERRUPT_2
} ExternalInterruptType;

typedef enum {
	EXTERNAL_INTERRUPT_LOW_LEVEL,
	EXTERNAL_INTERRUPT_LOGICAL_CHANGE,
	EXTERNAL_INTERRUPT_FALLING_EDGE,
	EXTERNAL_INTERRUPT_RISING_EDGE
} ExternalInterruptMode;

typedef void (*ExternalInterruptCallback)(void);

Std_ReturnType ExternalInterrupt_Init(ExternalInterruptType Interrupt, ExternalInterruptMode ExtIntMode, ExternalInterruptCallback ExtIntCallback);
Std_ReturnType ExternalInterrupt_Enable(ExternalInterruptType Interrupt);
Std_ReturnType ExternalInterrupt_Disable(ExternalInterruptType Interrupt);

#endif /* EXTERNAL_INTERRUPT_H_ */