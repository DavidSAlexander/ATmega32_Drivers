
/********************************************************************************************************
 *  [FILE NAME]   :      <PB.c>                                                                         *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Source file for the AVR PushButton driver>                                    *
 ********************************************************************************************************/

#include "PB.h"

/**
 *
 * @param PB
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType PB_Init(const PButton *PBtn)
{
    Std_ReturnType ret = E_OK;
    if ( PBtn == NULL ) ret = E_NOT_OK;
    else
    {
        DIO_PinConfig PBcfg =
        {
            .PORTx = PBtn->PortName,
            .PINx = PBtn->PinName,
            .DIRx = PIN_INPUT,
            .STATEx = ((PBtn->ResType == InternalPullup)?HIGH:LOW),
        };
        ret = DIO_PinDirSetup(&PBcfg);
    }
    return ret;
}

/**
 *
 * @param PB
 * @param PBS
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType PB_Read(const PButton *PBtn, Button_Status *PBS)
{
    Std_ReturnType ret = E_OK;
    if ( PBtn == NULL ) ret = E_NOT_OK;
    else
    {
        DIO_PinLogic Logic;
		DIO_PinConfig BTN = 
		{
			.PORTx = PBtn->PortName,
			.PINx  = PBtn->PinName
		};
        ret = DIO_PinRead(&BTN, &Logic);
        if (PBtn->ButtonType == NormallyClosed)
        {
            if (PBtn->ResType == Pulldown)
            {
                if (Logic == LOW)
                    *PBS = ButtonPressed;
                else if (Logic == HIGH)
                    *PBS = ButtonReleased;
            }
            else if (PBtn->ResType == Pullup || PBtn->ResType == InternalPullup)
            {
                if (Logic == LOW)
                    *PBS = ButtonReleased;
                else if (Logic == HIGH)
                    *PBS = ButtonPressed;
            }
            else ret = E_NOT_OK;
        }
        else if (PBtn->ButtonType == NormallyOpened)
        {
            if (PBtn->ResType == Pulldown)
            {
                if (Logic == LOW)
                    *PBS = ButtonReleased;
                else if (Logic == HIGH)
                    *PBS = ButtonPressed;
            }
            else if (PBtn->ResType == Pullup || PBtn->ResType == InternalPullup)
            {
                if (Logic == LOW)
                    *PBS = ButtonPressed;
                else if (Logic == HIGH)
                    *PBS = ButtonReleased;
            }
            else ret = E_NOT_OK;
        }
        else ret = E_NOT_OK;
    }
    return ret;
}
