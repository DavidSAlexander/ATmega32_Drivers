
/*******************************************************************************************************
 *  [FILE NAME]   :      <BIT_MACROS.h>                                                                *
 *  [AUTHOR]      :      <David S. Alexander>                                                          *
 *  [DATE CREATED]:      <June 10, 2023>                                                               *
 *  [Description} :      <Header file for common macros used>                                          *
 ******************************************************************************************************/
#ifndef STD_MACROS_H
#define STD_MACROS_H

/*******************************************************************************
 *                                 Definitions                                 *
 *******************************************************************************/

#define BIT_MASK         (uint8)1
#define PORT_MASK        (uint8)0xFF
#define MAX_PORT_NUM     (uint8)4
#define MAX_PIN_NUM      (uint8)8
#define MAX_LOGIC_NUM    (uint8)1
#define MAX_STATE_NUM    (uint8)1

/*******************************************************************************
 *                          Macro Functions Declarations                       *
 *******************************************************************************/

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT)        (REG|=(BIT_MASK<<BIT))

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT)      (REG&=(~(BIT_MASK<<BIT)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT)     (REG^=(BIT_MASK<<BIT))

/* Read a certain bit in any register */
#define READ_BIT(REG,BIT)       ((REG >> BIT) & BIT_MASK)

/* Rotate right the register value with specific number of rotates */
#define ROR(REG,num)            (REG= (REG>>num) | (REG<<(MAX_PIN_NUM - num)))

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,num)            (REG= (REG<<num) | (REG>>(MAX_PIN_NUM - num)))

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT)     (REG & (BIT_MASK<<BIT))

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT)   (!(REG & (BIT_MASK<<BIT)))

/* Access these special function registers of the AVR microcontroller */
#define SFR_IO8(io_addr) MMIO_BYTE((io_addr) + SFR_OFFSET)

/* Access these special function registers of the AVR microcontroller */
#define SFR_IO16(io_addr) MMIO_WORD((io_addr) + SFR_OFFSET)

/* Read or write a single byte of data from/to a memory-mapped I/O address */
#define MMIO_BYTE(mem_addr) (*(volatile uint8 *)(mem_addr))

/* Read or write a single byte of data from/to a memory-mapped I/O address */
#define MMIO_WORD(mem_addr) (*(volatile uint16 *)(mem_addr))

#ifndef SFR_OFFSET
#define SFR_OFFSET 0x20
#endif

#endif
