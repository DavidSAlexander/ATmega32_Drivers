/********************************************************************************************************
*  [FILE NAME]   :      <TSM.c>                                                                         *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <July 23, 2023>                                                                 *
*  [Description} :      <Source file for the AVR TSM Time Schedular driver>                             *
********************************************************************************************************/

#include "../../../LIB/BIT_MACROS.h"
#include "../../../LIB/DEVICE_CONFIG.h"
#include "../../../LIB/STD_LIBRARIES.h"
#include "../../../LIB/STD_TYPES.h"
#include "../Inc/TSM_Config.h"
#include "../Inc/TSM_Private.h"
#include "../Inc/TSM.h"
#include "../../../MCAL/TIMER0.h"
#include "../../../MCAL/Global_Interrupt.h"

uint32 SchedTicks = 0;
TSM_TaskHandler_t TSM_Tasks[MAX_NO_TASK];
uint8 TCNT0_Val = 0;
Timer0_Config_t TMR0TSMcfg;

volatile TSM_Status_t TSM_Status = TSM_NotInitialized;

Std_ReturnType TSM_Init(void)
{
	Std_ReturnType ret = E_OK;
	if ( TimeMS <= ZERO_INIT ) ret = E_NOT_OK;
	else
	{
		TCNT0_Val = 255 - (((uint32)TimeMS * F_CPU) / ((uint32)1024 * 1000)); // 240
		TMR0TSMcfg.CLK  = TMR0PRESCALING_CLK1024;
		TMR0TSMcfg.Mode = TMR0_Normal;
		TIMER0_Init(&TMR0TSMcfg);
		TIMER0_SetTCNT0(&TMR0TSMcfg, TCNT0_Val);
		TSM_Status = TSM_Initialized;
	}
	return ret;
}
Std_ReturnType TSM_DeInit(void)
{
	Std_ReturnType ret = E_OK;
	TSM_Status = TSM_NotInitialized;
	TCNT0_Val = ZERO_INIT;
	ret = TIMER0_DisableInterrupt(&TMR0TSMcfg, TSM_ISRHandler);
	ret = TIMER0_SetTCNT0(&TMR0TSMcfg, TCNT0_Val);
	return ret;
}
Std_ReturnType TSM_Start(void)
{
	Std_ReturnType ret = E_OK;
	if ( TSM_Status != TSM_Initialized ) ret = E_NOT_OK;
	else
	{
		ret = TIMER0_EnableInterrupt(&TMR0TSMcfg, TSM_ISRHandler);
		ENABLE_GIE();
	}
	return ret;
}
Std_ReturnType TSM_Stop(void)
{
	Std_ReturnType ret = E_OK;
	if ( TSM_Status != TSM_Initialized ) ret = E_NOT_OK;
	else
	{
		ret = TIMER0_DisableInterrupt(&TMR0TSMcfg, TSM_ISRHandler);
	}
	return ret;
}
Std_ReturnType TSM_Handller(void)
{
	Std_ReturnType ret = E_OK;

	if (TSM_Status != TSM_Initialized)
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint8 SchedCounter = 0;
		for (SchedCounter = 0; SchedCounter < MAX_NO_TASK; SchedCounter++)
		{
			if (TSM_Tasks[SchedCounter].FirstDelayEnabled == TRUE)
			{
				TSM_Tasks[SchedCounter].FirstDelayTime-= TimeMS;
				if (TSM_Tasks[SchedCounter].FirstDelayTime == 0 && TSM_Tasks[SchedCounter].TSM_TaskStatus == TSM_TaskReady)
				{
					TSM_Tasks[SchedCounter].TSM_TaskStatus = TSM_TaskRunning;
					TSM_Tasks[SchedCounter].taskPtr();
					if (TSM_Tasks[SchedCounter].TSM_TaskStatus != TSM_TaskSuspended && TSM_Tasks[SchedCounter].TSM_TaskStatus != TSM_TaskDormant)
					{
						TSM_Tasks[SchedCounter].TSM_TaskStatus = TSM_TaskReady;
					}
					TSM_Tasks[SchedCounter].FirstDelayTime = TSM_Tasks[SchedCounter].Periodicity;
				}
			}
			else if (SchedTicks % TSM_Tasks[SchedCounter].Periodicity == 0 && TSM_Tasks[SchedCounter].TSM_TaskStatus == TSM_TaskReady)
			{
				TSM_Tasks[SchedCounter].TSM_TaskStatus = TSM_TaskRunning;
				TSM_Tasks[SchedCounter].taskPtr();
				if (TSM_Tasks[SchedCounter].TSM_TaskStatus != TSM_TaskSuspended && TSM_Tasks[SchedCounter].TSM_TaskStatus != TSM_TaskDormant)
				{
					TSM_Tasks[SchedCounter].TSM_TaskStatus = TSM_TaskReady;
				}
			}
			else {}
		}
	}
	return ret;
}
Std_ReturnType TSM_CreatTask(uint8 TaskPriority, uint32 TaskPeriod, uint8 FirstDelay, void (*taskCodePtr)(void))
{
	Std_ReturnType ret = E_OK;
	if (TaskPriority >= MAX_NO_TASK || NULL == taskCodePtr || TSM_Status != TSM_Initialized ) ret = E_NOT_OK;
	else
	{
		TSM_Tasks[TaskPriority].Priority          = TaskPriority;
		TSM_Tasks[TaskPriority].TSM_TaskStatus    = TSM_TaskDormant;
		TSM_Tasks[TaskPriority].Periodicity       = TaskPeriod;
		TSM_Tasks[TaskPriority].taskPtr           = taskCodePtr;
		TSM_Tasks[TaskPriority].TSM_TaskStatus    = TSM_TaskReady;
		TSM_Tasks[TaskPriority].FirstDelayTime    = FirstDelay;
		TSM_Tasks[TaskPriority].FirstDelayEnabled = FirstDelay > 0 ? TRUE : FALSE;
	}
	return ret;
}
Std_ReturnType TSM_RemoveTask(uint8 TaskPriority)
{
	Std_ReturnType ret = E_OK;
	if ( TaskPriority >= MAX_NO_TASK || TSM_Status != TSM_Initialized ) ret = E_NOT_OK;
	else
	{
		TSM_Tasks[TaskPriority].TSM_TaskStatus = TSM_TaskDormant;
	}
	return ret;
}
Std_ReturnType TSM_ResumeTask(uint8 TaskPriority)
{
	Std_ReturnType ret = E_OK;
	if ( TaskPriority >= MAX_NO_TASK || TSM_Status != TSM_Initialized ) ret = E_NOT_OK;
	else
	{
		TSM_Tasks[TaskPriority].TSM_TaskStatus = TSM_TaskReady;
	}
	return ret;
}
Std_ReturnType TSM_SuspendTask(uint8 TaskPriority)
{
	Std_ReturnType ret = E_OK;
	if ( TaskPriority >= MAX_NO_TASK || TSM_Status != TSM_Initialized ) ret = E_NOT_OK;
	else
	{
		TSM_Tasks[TaskPriority].TSM_TaskStatus = TSM_TaskSuspended;
	}
	return ret;
}
Std_ReturnType TSM_ReportTask(uint8 TaskPriority, uint8* TaskState)
{
	Std_ReturnType ret = E_OK;
	if ( TaskPriority >= MAX_NO_TASK || TSM_Status != TSM_Initialized ) ret = E_NOT_OK;
	else
	{
		*TaskState = TSM_Tasks[TaskPriority].TSM_TaskStatus;
	}
	return ret;
}
void TSM_ISRHandler()
{
	SchedTicks+= TimeMS;
	TIMER0_SetTCNT0(&TMR0TSMcfg, TCNT0_Val);
	TSM_Handller();
}
