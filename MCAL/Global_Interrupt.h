
/********************************************************************************************************
*  [FILE NAME]   :      <External_Interrupt.h>                                                          *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 17, 2023>                                                                 *
*  [Description} :      <Header file for the AVR Global Interrupt driver>                               *
********************************************************************************************************/


#ifndef GLOBAL_INTERRUPT_H_
#define GLOBAL_INTERRUPT_H_

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"

#define SREG_         SFR_IO8(0x3F)

/* SREG */
#define GIE_Bit       7  

void ENABLE_GIE(void);
void DISABLE_GIE(void);


#endif /* GLOBAL_INTERRUPT_H_ */
