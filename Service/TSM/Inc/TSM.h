/********************************************************************************************************
*  [FILE NAME]   :      <TSM.h>                                                                         *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <July 23, 2023>                                                                 *
*  [Description} :      <Header file for the AVR Time Schedular Manager driver>                         *
********************************************************************************************************/

#ifndef _SCM_H_
#define _SCM_H_


/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType TSM_Init(void);
Std_ReturnType TSM_DeInit(void);
Std_ReturnType TSM_Start(void);
Std_ReturnType TSM_Stop(void);
Std_ReturnType TSM_CreatTask(uint8 TaskPriority, uint32 TaskPeriod, uint8 FirstDelay, void (*taskCodePtr)(void));
Std_ReturnType TSM_RemoveTask(uint8 TaskPriority);
Std_ReturnType TSM_ResumeTask(uint8 TaskPriority);
Std_ReturnType TSM_SuspendTask(uint8 TaskPriority);
Std_ReturnType TSM_ReportTask(uint8 TaskPriority, uint8* TaskState);
Std_ReturnType TSM_Handller(void);
void           TSM_ISRHandler(void);

#endif /* _SCM_H_ */
