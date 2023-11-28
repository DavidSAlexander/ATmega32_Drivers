

#ifndef _TSM_HW_REG_H_
#define _TSM_HW_REG_H_

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
	TSM_NotInitialized,
	TSM_Initialized
}TSM_Status_t;

typedef enum
{
	TSM_TaskSuspended,
	TSM_TaskReady,
	TSM_TaskRunning,
	TSM_TaskDormant
}TSM_TaskState_t;

typedef struct
{
	uint8            Priority;
	uint16           FirstDelayTime;
	uint32           Periodicity;
	void             (*taskPtr)(void);
	TSM_TaskState_t  TSM_TaskStatus;   
	boolean          FirstDelayEnabled;      
}TSM_TaskHandler_t;

#endif /* _TSM_HW_REG_H_ */
