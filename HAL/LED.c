
/********************************************************************************************************
 *  [FILE NAME]   :      <LED.c>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Source file for LED configurations driver>                                    *
 ********************************************************************************************************/

#include "LED.h"


Std_ReturnType LED_Init(const SingleLED *LED)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == LED ) ret = E_NOT_OK;
    else
    {
        DIO_PinConfig SLED =
        {
            .PORTx = LED->PortName,
            .PINx = LED->PinName,
            .DIRx = PIN_OUTPUT,
            .STATEx = (LED->LedCircuit == LED_SOURCE?LED->LedState:!LED->LedState)
        };
        ret = DIO_PinDirSetup(&SLED);
    }
    return ret;
}
Std_ReturnType LEDs_Init(const PortLEDs *LEDs)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == LEDs ) ret = E_NOT_OK;
    else
    {
        ret = DIO_PortDirSetup(LEDs->PortName,Port_OUTPUT);
        if ( ret == E_OK ) ret = DIO_PortWrite(LEDs->PortName,LEDs->LedCircuit == LED_SOURCE?LEDs->LedsState:~LEDs->LedsState);
    }
    return ret;
}
Std_ReturnType LED_Control(const SingleLED *LED, LED_Status Status)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == LED || Status < LED_OFF || Status > LED_ON ) ret = E_NOT_OK;
	else
	{
		DIO_PinConfig SLED =
		{
			.PORTx = LED->PortName,
			.PINx = LED->PinName,
			.STATEx = LED->LedState
		};
		ret = DIO_PinWrite(&SLED,LED->LedCircuit == LED_SOURCE?Status:!Status);
	}
	return ret;
}
Std_ReturnType LEDs_Control(const PortLEDs *LEDs, uint8 Status)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == LEDs || Status < LED_OFF || Status > Port_HIGH ) ret = E_NOT_OK;
    else
    {
        ret = DIO_PortWrite(LEDs->PortName,LEDs->LedCircuit == LED_SOURCE?Status:~Status);
    }
    return ret;
}
Std_ReturnType LED_Toggle(const SingleLED *LED)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == LED ) ret = E_NOT_OK;
    else
    {
        DIO_PinConfig SLED =
        {
            .PORTx = LED->PortName,
            .PINx = LED->PinName
        };
        ret = DIO_PinToggle(&SLED);
    }
    return ret;
}
Std_ReturnType LEDs_Toggle(const PortLEDs *LEDs)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == LEDs ) ret = E_NOT_OK;
    else
    {
        DIO_PortToggle(LEDs->PortName);
    }
    return ret;
}
