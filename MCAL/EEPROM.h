/********************************************************************************************************
 *  [FILE NAME]   :      <EEPROM.h>                                                                     *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 16, 2023>                                                                *
 *  [Description] :      <Header file for the AVR EEPROM driver>                                        *
 ********************************************************************************************************/


#ifndef EEPROM_H_
#define EEPROM_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../Includes/STD_TYPES.h"
#include "../Includes/DEVICE_CONFIG.h"
#include "../Includes/BIT_MACROS.h"
#include "../Includes/STD_LIBRARIES.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/

#define EECR_REG   SFR_IO8(0x1C)
#define EEDR_REG   SFR_IO8(0x1D)
#define EEAR_REG  SFR_IO16(0x1E)
#define EEARl_REG  SFR_IO8(0x1E)
#define EEARH_REG  SFR_IO8(0x1F)

#define EEPROM_START_ADDR  0x0000
#define EEPROM_MAX_ADDR    0x01FF

/* EEPROM Control Register */
#define EERIE_BIT    3
#define EEMWE_BIT    2
#define EEWE_BIT     1
#define EERE_BIT     0


/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/
 
 /* Initialize EEPROM module */
Std_ReturnType EEPROM_Init(void);

/* Read from EEPROM module */
Std_ReturnType EEPROM_ReadByte(uint16 Addr, uint8* DataPtr);

/* Read to EEPROM module */
Std_ReturnType EEPROM_WriteByte(uint16 Addr, uint8 Data);



#endif /* EEPROM_H_ */