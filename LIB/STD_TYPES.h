
  /*******************************************************************************************************
 *  [FILE NAME]   :      <STD_TYPES.h>                                                                   *
 *  [AUTHOR]      :      <David S. Alexander>                                                            *
 *  [DATE CREATED]:      <June 10, 2023>                                                                  *
 *  [Description} :      <Common - Platform Types Abstraction>                                           *
 *********************************************************************************************************/

#ifndef STD_TYPES_H_
#define STD_TYPES_H_


/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/

#ifndef FALSE
#define FALSE                 (0u)
#endif
#ifndef TRUE
#define TRUE                  (1u)
#endif 

#define LOGIC_HIGH            (1u)
#define LOGIC_LOW             (0u)

#ifndef NULL
#define NULL                  ((void*)0)
#endif

#define LOGIC_HIGH            (1u)
#define LOGIC_LOW             (0u)

#define E_OK                  (Std_ReturnType)0x01
#define E_NOT_OK              0x00

#define Port_INPUT            0x00
#define Port_OUTPUT           0xFF
#define Pin_INPUT             0x00
#define Pin_OUTPUT            0x01
#define Port_HIGH             0xFF
#define Port_LOW              0x00

#define ZERO_INIT             0x00

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/ 

typedef unsigned char         uint8;         
typedef signed char           sint8;        
typedef unsigned short        uint16;        
typedef signed short          sint16;        
typedef unsigned long         uint32;     
typedef signed long           sint32;       
typedef unsigned long long    uint64;         
typedef signed long long      sint64;         
typedef float                 float32;
typedef double                float64;
typedef uint8                 Std_ReturnType;
typedef unsigned char         boolean;

#endif /* STD_TYPE_H_ */
