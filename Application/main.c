#include <avr/io.h>
#include "../Service/FreeRTOS/FreeRTOS.h"
#include "../Service/FreeRTOS/FreeRTOSConfig.h"
#include "../Service/FreeRTOS/task.h"
#include "../Service/FreeRTOS/semphr.h"
#include "AppConfig.h"
#include <libhttp.h>

uint8 flag = 0;

SingleLED LED1 =
{
	.PortName   = PORTA_INDEX,
	.PinName    = PIN5_INDEX,
	.LedState   = LED_ON,
	.LedCircuit = LED_SOURCE
};
SingleLED LED2 =
{
	.PortName   = PORTA_INDEX,
	.PinName    = PIN6_INDEX,
	.LedState   = LED_OFF,
	.LedCircuit = LED_SOURCE
};
PButton Button1 =
{
	.PortName   = PORTD_INDEX,
	.PinName    = PIN3_INDEX,
	.ResType    = InternalPullup,
	.ButtonType = NormallyOpened
};

/* Define Tasks Priorities */
#define  LedOn_PRIORITY    (2)
#define  ButtonOn_PRIORITY (3)

void Motor_Code(void*pvParamter)
{
	while(1)
	{
		if (flag == 1)
		{
			vTaskDelay(3000);
			LED_Toggle(&LED1);
			LED_Toggle(&LED2);
			flag = 0;
		}
	}
}

void ButtonIntro_code (void * pvParamter)
{
	Button_Status btn1 = ButtonReleased;
	while(1)
	{
		vTaskDelay(100);
		PB_Read(&Button1, &btn1);
		if ( btn1 == ButtonPressed )
		{
			flag = 1 ;
		}
	}
}

/*Create Task Handles Create */
xTaskHandle Task1Hand = NULL;
xTaskHandle Task2Hand = NULL;

int main(void)
{
	LED_Init(&LED1);
	LED_Init(&LED2);
	PB_Init(&Button1);
	
	xTaskCreate(Motor_Code,       (const char *)"Task1", 85, NULL,LedOn_PRIORITY   , NULL);  
	xTaskCreate(ButtonIntro_code, (const char *)"Task2", 85, NULL,ButtonOn_PRIORITY, NULL);
	
	vTaskStartScheduler();

	return 0;
}
















