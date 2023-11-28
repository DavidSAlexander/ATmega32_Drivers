
/********************************************************************************************************
 *  [FILE NAME]   :      <DIO.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 10, 2023>                                                                *
 *  [Description} :      <Source file for the AVR DIO driver>                                           *
 ********************************************************************************************************/

#include "DIO.h"

volatile uint8 *DDR_REG[]  = {&DIO_DDRA, &DIO_DDRB, &DIO_DDRC, &DIO_DDRD};
volatile uint8 *PIN_REG[]  = {&DIO_PINA, &DIO_PINB, &DIO_PINC, &DIO_PIND};
volatile uint8 *PORT_REG[] = {&DIO_PORTA, &DIO_PORTB, &DIO_PORTC, &DIO_PORTD};
	
/*
 * Description :
 * Setup the direction of the required pin input/output.
 * If the input port number or pin number are not correct, The function will not handle the request.
 */
Std_ReturnType DIO_PinDirSetup(const DIO_PinConfig *Pin_Config)
{
    /*
     * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
     * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
     * In this case the input is not valid port/pin number
     */
    Std_ReturnType ret = E_OK;
    if ( NULL == Pin_Config || Pin_Config->PINx >= MAX_PIN_NUM || Pin_Config->PINx < 0 || Pin_Config->PORTx >= MAX_PORT_NUM || Pin_Config->PORTx < 0 ) ret = E_NOT_OK;
    else
    {
        switch (Pin_Config->DIRx)
        {
        case PIN_OUTPUT:
            SET_BIT(*DDR_REG[Pin_Config->PORTx], Pin_Config->PINx);
            DIO_PinWrite(Pin_Config, (Pin_Config->STATEx == HIGH?HIGH:LOW));
            break;
        case PIN_INPUT:
            CLEAR_BIT(*DDR_REG[Pin_Config->PORTx], Pin_Config->PINx);
            DIO_PinWrite(Pin_Config, (Pin_Config->STATEx == HIGH?HIGH:LOW));
            break;
        default:
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType DIO_PinDirStatus(const DIO_PinConfig *Pin_Config, DIO_PinDirection *Dir_Status)
{
    /*
     * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
     * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
     * In this case the input is not valid port/pin number
     */
    Std_ReturnType ret = E_OK;
    if ( NULL == Pin_Config || Pin_Config->PINx >= MAX_PIN_NUM || Pin_Config->PINx < 0 || Pin_Config->PORTx >= MAX_PORT_NUM || Pin_Config->PORTx < 0 ) ret = E_NOT_OK;
    else
    {
        *Dir_Status = READ_BIT(*DDR_REG[Pin_Config->PORTx],Pin_Config->PINx);
    }
    return ret;
}
Std_ReturnType DIO_PinWrite(const DIO_PinConfig *Pin_Config, DIO_PinLogic Logic)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == Pin_Config || Pin_Config->PINx >= MAX_PIN_NUM || Pin_Config->PINx < 0 || Pin_Config->PORTx >= MAX_PORT_NUM || Pin_Config->PORTx < 0 ) ret = E_NOT_OK;
    else
    {
        switch (Logic)
        {
        case HIGH:
            SET_BIT(*(PORT_REG[Pin_Config->PORTx]), Pin_Config->PINx);
            break;
        case LOW:
            CLEAR_BIT(*PORT_REG[Pin_Config->PORTx], Pin_Config->PINx);
            break;
        default:
            ret = E_NOT_OK;
        }
    }
    return ret;
}
Std_ReturnType DIO_PinRead(const DIO_PinConfig *Pin_Config, DIO_PinLogic *Logic)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == Pin_Config || Pin_Config->PINx >= MAX_PIN_NUM || Pin_Config->PINx < 0 || Pin_Config->PORTx >= MAX_PORT_NUM || Pin_Config->PORTx < 0 ) ret = E_NOT_OK;
    else
    {
        *Logic = READ_BIT(*PIN_REG[Pin_Config->PORTx],Pin_Config->PINx);
    }
    return ret;
}
Std_ReturnType DIO_PinToggle(const DIO_PinConfig *Pin_Config)
{
    Std_ReturnType ret = E_OK;
    if ( NULL == Pin_Config || Pin_Config->PINx >= MAX_PIN_NUM || Pin_Config->PINx < 0 || Pin_Config->PORTx >= MAX_PORT_NUM || Pin_Config->PORTx < 0 ) ret = E_NOT_OK;
    else
    {
        TOGGLE_BIT(*PORT_REG[Pin_Config->PORTx],Pin_Config->PINx);
    }
    return ret;
}

Std_ReturnType DIO_PortDirSetup(DIO_PortIndex Port_Config, uint8 deriction)
{
    Std_ReturnType ret = E_OK;
    if ( Port_Config >= MAX_PORT_NUM || Port_Config < 0 || deriction < 0 || deriction > 0xFF ) ret = E_NOT_OK;
    else
    {
        *DDR_REG[Port_Config] = deriction;
    }
    return ret;
}
Std_ReturnType DIO_PortDirStatus(DIO_PortIndex Port_Status, uint8 *Status)
{
    Std_ReturnType ret = E_OK;
    if ( Port_Status >= MAX_PORT_NUM || Port_Status < 0  ) ret = E_NOT_OK;
    else
    {
        *Status = *PORT_REG[Port_Status];
    }
    return ret;
}
Std_ReturnType DIO_PortWrite(const DIO_PortIndex Port_Index, uint8 Logic)
{
    Std_ReturnType ret = E_OK;
    if ( Port_Index >= MAX_PORT_NUM || Port_Index < 0 || Logic < 0 || Logic > 0xFF ) ret = E_NOT_OK;
    else
    {
        *PORT_REG[Port_Index] = Logic;
    }
    return ret;
}
Std_ReturnType DIO_PortRead(const DIO_PortIndex Port_Index, uint8 *Logic)
{
    Std_ReturnType ret = E_OK;
    if ( Port_Index >= MAX_PORT_NUM || Port_Index < 0 ) ret = E_NOT_OK;
    else
    {
        *Logic = *PIN_REG[Port_Index];
    }
    return ret;
}
Std_ReturnType DIO_PortToggle(const DIO_PortIndex Port_Index)
{
    Std_ReturnType ret = E_OK;
    if ( Port_Index >= MAX_PORT_NUM || Port_Index < 0 ) ret = E_NOT_OK;
    else
    {
        *PORT_REG[Port_Index] ^= 0xFF;
    }
    return ret;
}
Std_ReturnType DIO_PortROL(DIO_PortIndex Port, uint8 NUM)
{
    Std_ReturnType ret = E_OK;
    if ( Port >= MAX_PORT_NUM || Port < 0 || NUM >= MAX_PIN_NUM || Port < 0 ) ret = E_NOT_OK;
    else
    {
        ROL(*PORT_REG[Port],NUM);
    }
    return ret;
}

Std_ReturnType DIO_PortROR(DIO_PortIndex Port, uint8 NUM)
{
    Std_ReturnType ret = E_OK;
    if ( Port >= MAX_PORT_NUM || Port < 0 || NUM >= MAX_PIN_NUM || Port < 0 ) ret = E_NOT_OK;
    else
    {
        ROR(*PORT_REG[Port],NUM);
    }
    return ret;
}

