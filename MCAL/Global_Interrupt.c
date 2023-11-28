
/********************************************************************************************************
*  [FILE NAME]   :      <External_Interrupt.h>                                                          *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 17, 2023>                                                                 *
*  [Description} :      <Source file for the AVR Global Interrupt driver>                               *
********************************************************************************************************/

#include "Global_Interrupt.h"

void ENABLE_GIE(void)
{
	SET_BIT(SREG_, GIE_Bit);
}
void DISABLE_GIE(void)
{
	CLEAR_BIT(SREG_, GIE_Bit);
}