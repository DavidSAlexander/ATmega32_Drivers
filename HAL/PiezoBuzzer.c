
/********************************************************************************************************
 *  [FILE NAME]   :      <PiezoBuzzer.c>                                                                *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Aug 3, 2023>                                                                  *
 *  [Description} :      <Source file for the AVR PiezoBuzzer driver>                                   *
 ********************************************************************************************************/ 



/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "PiezoBuzzer.h"

Std_ReturnType PBuzzer_Init(const PBuzzer_t *Buzz)
{
	 Std_ReturnType ret = E_OK;
	 if ( Buzz == NULL ) ret = E_NOT_OK;
	 else
	 {
		 DIO_PinConfig PBuzzercfg =
		 {
			 .PORTx  = Buzz->PortName,
			 .PINx   = Buzz->PinName,
			 .DIRx   = PIN_OUTPUT,
			 .STATEx = ((Buzz->PBuzzerType == PBuzzer_PNP)?HIGH:LOW)
		 };
		 ret = DIO_PinDirSetup(&PBuzzercfg);
	 }
	 return ret;
}
Std_ReturnType PBuzzer_Control(const PBuzzer_t *Buzz, PBuzzerStatus_t BuzzS)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == Buzz || BuzzS < PBuzzerOFF || BuzzS > PBuzzerON ) ret = E_NOT_OK;
	else
	{
		DIO_PinConfig PBuzzerPin =
		{
			.PORTx  = Buzz->PortName,
			.PINx   = Buzz->PinName,
			.STATEx = Buzz->PBuzzerType,
		};
		ret = DIO_PinWrite(&PBuzzerPin, Buzz->PBuzzerType == PBuzzer_NPN?BuzzS:!BuzzS);
	}
	return ret;
}
Std_ReturnType PBuzzer_Toggle(const PBuzzer_t *Buzz)
{
	Std_ReturnType ret = E_OK;
	if ( NULL == Buzz ) ret = E_NOT_OK;
	else
	{
		DIO_PinConfig PBuzzerPin =
		{
			.PORTx = Buzz->PortName,
			.PINx = Buzz->PinName
		};
		ret = DIO_PinToggle(&PBuzzerPin);
	}
	return ret;
}