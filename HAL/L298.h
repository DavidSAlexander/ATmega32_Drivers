
/********************************************************************************************************
 *  [FILE NAME]   :      <L298.h>                                                                       *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Aug 3, 2023>                                                                  *
 *  [Description} :      <Header file for the AVR L298 Motor driver>                                    *
 ********************************************************************************************************/ 


#ifndef L298_H_
#define L298_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../MCAL/DIO.h"
#include "../MCAL/PWM1.h"

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/
typedef enum
{
	L298_DisablePWM,
	L298_EnablePWM
}L298_PWM_State;

typedef enum
{
	L298_Backward,
	L298_Forward
}L298_Direction;

typedef struct
{
    uint8           ENA_PortName : 2;
    uint8           ENA_PinName  : 3;	
	L298_PWM_State  ENA_PWMState : 1;
}L298ENA_t;

typedef struct
{
	 uint8           ENB_PortName : 2;
	 uint8           ENB_PinName  : 3;
	 L298_PWM_State  ENB_PWMState : 1;
}L298ENB_t;

typedef struct
{
	 uint8           IN1_PortName : 2;
	 uint8           IN1_PinName  : 3;
	 L298_PWM_State  IN1_PWMState : 1;
}L298IN1_t;

typedef struct
{
	uint8           IN2_PortName : 2;
	uint8           IN2_PinName  : 3;
	L298_PWM_State  IN2_PWMState : 1;
}L298IN2_t;

typedef struct
{
	uint8           IN3_PortName : 2;
	uint8           IN3_PinName  : 3;
	L298_PWM_State  IN3_PWMState : 1;
}L298IN3_t;

typedef struct
{
	uint8           IN4_PortName : 2;
	uint8           IN4_PinName  : 3;
	L298_PWM_State  IN4_PWMState : 1;
}L298IN4_t;

typedef struct
{
	L298ENA_t L298ENA_Pin;
	L298IN1_t L298IN1_Pin;
	L298IN2_t L298IN2_Pin;
}L298_MotorA_t;

typedef struct
{
	L298ENB_t L298ENB_Pin;
	L298IN3_t L298IN3_Pin;
	L298IN4_t L298IN4_Pin;
}L298_MotorB_t;

typedef struct
{
	L298_MotorA_t MotorA;
	L298_MotorB_t MotorB;
}L298_t;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType L298_Init(const L298_t *Motors);

Std_ReturnType L298_InitMotorA(const L298_MotorA_t *MotorA);
Std_ReturnType L298_InitMotorB(const L298_MotorB_t *MotorB);


Std_ReturnType L298_SetSpeedMotors(const L298_t *L298M, sint8 MA, sint8 MB);
Std_ReturnType L298_SetDirectionMotors(const L298_t *L298M, L298_Direction MA, L298_Direction MB);

Std_ReturnType L298_SetSpeedMotorA(const L298_MotorA_t *MotorA, sint8 MA);
Std_ReturnType L298_SetSpeedMotorB(const L298_MotorB_t *MotorB, sint8 MB);

Std_ReturnType L298_SetDirectionMotorA(const L298_MotorA_t *MotorA, L298_Direction MA);
Std_ReturnType L298_SetDirectionMotorB(const L298_MotorB_t *MotorB, L298_Direction MB);

#endif /* L298_H_ */