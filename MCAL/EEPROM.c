/********************************************************************************************************
 *  [FILE NAME]   :      <EEPROM.c>                                                                     *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 16, 2023>                                                                *
 *  [Description] :      <Source file for the AVR EEPROM driver>                                        *
 ********************************************************************************************************/


#include "EEPROM.h"

/**
* @brief Configures the EECR_REG register with 0 to initialize EEPROM.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*/
Std_ReturnType EEPROM_Init()
{
	// Clear EEPROM control bits
	CLEAR_BIT(EECR_REG, EEMWE_BIT);
	CLEAR_BIT(EECR_REG, EEWE_BIT);

	return E_OK;
}

/**
* @brief Configures the EEAR, EEDR and EECR registers with the specified BITS settings to write a byte.
* @param Addr A 16-bit variable to an EEPROM data address.
* @param Data A 8-bit variable to an EEPROM data.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*     - E_NOT_OK: The function encountered an error.
*/
Std_ReturnType EEPROM_WriteByte(uint16 Addr, uint8 Data)
{
	// Check if address is out of range
	if ( Addr > EEPROM_MAX_ADDR )
	{
		return E_NOT_OK;
	}

	// Wait for previous write operation to complete
	while (READ_BIT(EECR_REG, EEWE_BIT));

	// Set EEPROM address
	EEAR_REG = Addr + EEPROM_START_ADDR;

	// Set data to be written
	EEDR_REG = Data;

	// Set programming mode
	SET_BIT(EECR_REG, EEMWE_BIT);

	// Start EEPROM write operation
	SET_BIT(EECR_REG, EEWE_BIT);

	return E_OK;
}

/**
* @brief Configures the EEAR, EEDR and EECR registers with the specified BITS settings to read a byte.
* @param Addr A 16-bit variable to an EEPROM data address.
* @param DataPtr A 8-bit pointer to an EEPROM data.
* @return The status of the function.
*     - E_OK: The function executed successfully.
*     - E_NOT_OK: The function encountered an error.
*/
Std_ReturnType EEPROM_ReadByte(uint16 Addr, uint8* DataPtr)
{
	// Check if address is out of range or data pointer is null
	if (Addr > EEPROM_MAX_ADDR || DataPtr == NULL) {
		return E_NOT_OK;
	}

	// Wait for previous write operation to complete
	while (READ_BIT(EECR_REG, EEWE_BIT));

	// Set EEPROM address
	EEAR_REG = Addr + EEPROM_START_ADDR;

	// Start EEPROM read operation
	SET_BIT(EECR_REG, EERE_BIT);

	// Read data from EEPROM
	*DataPtr = EEDR_REG;

	return E_OK;
}