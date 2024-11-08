/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2013)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. Passing on and copying of this document, and communication   **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME  : Port_PBCfg.c                                                  **
**                                                                            **
**  $CC VERSION : \main\dev_tc23x\7 $                                        **
**                                                                            **
**  DATE, TIME: 2024-08-13, 09:38:45                                      **
**                                                                            **
**  GENERATOR : Build b141014-0350                                          **
**                                                                            **
**  AUTHOR    : DL-AUTOSAR-Engineering                                        **
**                                                                            **
**  VENDOR    : Infineon Technologies                                         **
**                                                                            **
**  DESCRIPTION  : Port configuration generated from ECU 
                   configuration file ( Port.bmd)                             **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: No                                       **
**                                                                            **
*******************************************************************************/

 
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Inclusion of Port Header file */
#include "Port.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
** Name             : Portx_lPdrConfig                                        **
**                                                                            **
** Description      : Macro to configure Portx Pdr register parameters        **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Function like macros used for simplification  of 
        configuration generation and increase the readability of config file*/
/* Macro definition for PORT pad drive control register Pn_PDR0 */
#define  Portx_lPdrConfig1(Pd0,Pd1,Pd2,Pd3,Pd4,Pd5,Pd6,Pd7)                    \
         ( ((uint32)(Pd7) << (uint32)28) | ((uint32)(Pd6) << (uint32)24) |     \
           ((uint32)(Pd5) << (uint32)20) | ((uint32)(Pd4) << (uint32)16) |     \
           ((uint32)(Pd3) << (uint32)12)|  ((uint32)(Pd2) << (uint32)8)  |     \
           ((uint32)(Pd1) << (uint32)4) | (uint32)(Pd0)                        \
         )
/*IFX_MISRA_RULE_19_07_STATUS=Function like macros used for simplification  of 
        configuration generation and increase the readability of config file*/
/* Macro definition for PORT pad drive control register Pn_PDR1 */
#define  Portx_lPdrConfig2(Pd8,Pd9,Pd10,Pd11,Pd12,Pd13,Pd14,Pd15)              \
         ( ((uint32)(Pd15) << (uint32)28) | ((uint32)(Pd14) << (uint32)24) |   \
           ((uint32)(Pd13) << (uint32)20) | ((uint32)(Pd12) << (uint32)16) |   \
           ((uint32)(Pd11) << (uint32)12)|  ((uint32)(Pd10) << (uint32)8)  |   \
           ((uint32)(Pd9) << (uint32)4) | (uint32)(Pd8)                        \
         )                  
 /*IFX_MISRA_RULE_19_07_STATUS=Function like macros used for simplification  of 
        configuration generation and increase the readability of config file*/        
/* Macro definition for PORT Pad Disable Control Register */
#define Port_lDiscSet(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15)   \
  ((uint16)(                                                                    \
            (uint32)(b0) | ((uint32)(b1) << (uint32)1) |                       \
            ((uint32)(b2) << (uint32)2) | ((uint32)(b3) << (uint32)3) |        \
            ((uint32)(b4) << (uint32)4) | ((uint32)(b5) << (uint32)5) |        \
            ((uint32)(b6) << (uint32)6) | ((uint32)(b7) << (uint32)7) |        \
            ((uint32)(b8) << (uint32)8) | ((uint32)(b9) << (uint32)9) |        \
            ((uint32)(b10) << (uint32)10) | ((uint32)(b11) << (uint32)11) |    \
            ((uint32)(b12) << (uint32)12) | ((uint32)(b13)<< (uint32)13) |     \
            ((uint32)(b14) << (uint32)14) | ((uint32)(b15) << (uint32)15)      \
           ))

/*IFX_MISRA_RULE_19_07_STATUS=Function like macros used for simplification  of 
        configuration generation and increase the readability of config file*/
/* Macro definition for PORT PCSR register */
#define Port_lPcsr(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15)      \
  ((uint32)(                                                                    \
            ((uint32)(b0)) | ((uint32)(b1) << (uint32)1) |                     \
            ((uint32)(b2) << (uint32)2) | ((uint32)(b3) << (uint32)3) |        \
            ((uint32)(b4) << (uint32)4) | ((uint32)(b5) << (uint32)5) |        \
            ((uint32)(b6) << (uint32)6) | ((uint32)(b7) << (uint32)7) |        \
            ((uint32)(b8) << (uint32)8) | ((uint32)(b9) << (uint32)9) |        \
            ((uint32)(b10) << (uint32)10) | ((uint32)(b11) << (uint32)11) |    \
            ((uint32)(b12) << (uint32)12) | ((uint32)(b13) << (uint32)13) |    \
            ((uint32)(b14) << (uint32)14) | ((uint32)(b15) << (uint32)15)      \
           ))

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/
/* MISRA RULE 87 VIOLATION: Inclusion of MemMap.h in between the code can't 
   be avoided as it is required for mapping global variables, constants 
   and code
*/ 
/*lint -esym( 960, 87 ) Note 960: Violates MISRA Required Rule 87, 
                       only preprocessor statements and comments before 
                       '#include MemMap.h'
*/
/*lint -e537 Warning 537: Repeated include file MemMap.h */


#define PORT_START_SEC_POSTBUILDCFG
#include "MemMap.h"

/*******************************************************************************
**                      Configuration Options                                 **
*******************************************************************************/
/*
                     Container : PortPinConfiguration
*/

/* 
Configuration Options: Physical pin level
-LOW  (Low Volatage Level)
-HIGH (High Voltage Level) 
*/
#define  PORT_PIN_LOW                       (0x00U)
#define  PORT_PIN_HIGH                      (0x01U)

/* Macros to define pin Default Input Output control value */
#define  PORT_PIN_DEFAULT                   (0x10U)

/* 
Configuration Options: Pin input characteristics 
-NO PULL
-PULL DOWN
-PULL UP
*/
#define  PORT_PIN_IN_NO_PULL                (0x00U)
#define  PORT_PIN_IN_PULL_DOWN              (0x08U)
#define  PORT_PIN_IN_PULL_UP                (0x10U)

/* 
Configuration Options: Pin output characteristics 
-PUSHPULL
-OPENDRAIN
*/
#define  PORT_PIN_OUT_PUSHPULL              (0x00U)
#define  PORT_PIN_OUT_OPENDRAIN             (0x40U)

/*
Configuration Options: Pin Pad Level  
*/
#define  PORT_PDR_CMOS_AUTOMOTIVE_LEVEL     (0x0U)

/*
Configuration Options: Pin driver strength
*/
#define  PORT_CMOS_SPEED_GRADE1             (0x0U)
#define  PORT_CMOS_SPEED_GRADE2             (0x1U)
#define  PORT_CMOS_SPEED_GRADE3             (0x2U)
#define  PORT_CMOS_SPEED_GRADE4             (0x3U)

/* Pin driver strength value for the non available pins*/
#define  PORT_PIN_PAD_LEVEL_DEFAULT         (0x0U)

/* Configuration Options: Pin drive strength, for class Adc type pad    */
#define  PORT_PDR_ADC                       (0x1U)

/*
  Some of the Pins doesn't have driver strength, To access the same
  pin this null value macro is introduced.
*/
#define  PORT_PDR_ZERO                      (0x0U)

/* For the Px_DISC register value set */
#define  PORT_PIN_ANALOG_INPUT_ENABLE       (0x1U)
#define  PORT_PIN_ANALOG_INPUT_DISABLE      (0x0U)

/* For PCSR register */
#define PORT_PCSR_DEFAULT                   (0x0U)
#define PORT_PCSR_ENABLE                    (0x1U)
#define PORT_PCSR_DISABLE                   (0x0U)

/******************************************************************************/


static const Port_n_ConfigType Port_kConfiguration0[] = 
{
/*                              Port0                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 8*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         PORT_PIN_LOW,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         PORT_PIN_LOW,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 8 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port2                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_HIGH,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_HIGH,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port10                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         (PORT_PIN_DEFAULT),/*Pin 0*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 3*/
         (PORT_PIN_DEFAULT),/*Pin 4*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 6*/
         (PORT_PIN_DEFAULT),/*Pin 7*/
         (PORT_PIN_DEFAULT),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         0U,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_HIGH,/* Pin 3 */ 
         0U,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         0U,/* Pin 7 */ 
         0U,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port11                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         (PORT_PIN_DEFAULT),/*Pin 0*/
         (PORT_PIN_DEFAULT),/*Pin 1*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 3*/
         (PORT_PIN_DEFAULT),/*Pin 4*/
         (PORT_PIN_DEFAULT),/*Pin 5*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT3),/*Pin 6*/
         (PORT_PIN_DEFAULT),/*Pin 7*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT3),/*Pin 8*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT3),/*Pin 9*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 10*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 11*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         0U,/* Pin 0 */ 
         0U,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         0U,/* Pin 4 */ 
         0U,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         0U,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         PORT_PIN_LOW,/* Pin 9 */ 
         PORT_PIN_LOW,/* Pin 10 */ 
         PORT_PIN_LOW,/* Pin 11 */ 
         PORT_PIN_LOW,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 0 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 4 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 6 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 8 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 9 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 10 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 11 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port13                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 3*/
         (PORT_PIN_DEFAULT),/*Pin 4*/
         (PORT_PIN_DEFAULT),/*Pin 5*/
         (PORT_PIN_DEFAULT),/*Pin 6*/
         (PORT_PIN_DEFAULT),/*Pin 7*/
         (PORT_PIN_DEFAULT),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         0U,/* Pin 4 */ 
         0U,/* Pin 5 */ 
         0U,/* Pin 6 */ 
         0U,/* Pin 7 */ 
         0U,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 4 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 5 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 6 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port14                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port15                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT1),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_HIGH,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port20                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 0*/
         (PORT_PIN_DEFAULT),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 3*/
         (PORT_PIN_DEFAULT),/*Pin 4*/
         (PORT_PIN_DEFAULT),/*Pin 5*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_NO_PULL | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT5),/*Pin 8*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 9*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 10*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT3),/*Pin 11*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 12*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT3),/*Pin 13*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT3),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         0U,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         0U,/* Pin 4 */ 
         0U,/* Pin 5 */ 
         PORT_PIN_HIGH,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         PORT_PIN_HIGH,/* Pin 8 */ 
         PORT_PIN_LOW,/* Pin 9 */ 
         PORT_PIN_LOW,/* Pin 10 */ 
         PORT_PIN_LOW,/* Pin 11 */ 
         PORT_PIN_LOW,/* Pin 12 */ 
         PORT_PIN_LOW,/* Pin 13 */ 
         PORT_PIN_LOW,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 4 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 8 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 9 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 10 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 11 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 12 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 13 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port21                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         (PORT_PIN_DEFAULT),/*Pin 0*/
         (PORT_PIN_DEFAULT),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 7*/
         (PORT_PIN_DEFAULT),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         0U,/* Pin 0 */ 
         0U,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         0U,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 0 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port22                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 4*/
         (PORT_PIN_DEFAULT),/*Pin 5*/
         (PORT_PIN_DEFAULT),/*Pin 6*/
         (PORT_PIN_DEFAULT),/*Pin 7*/
         (PORT_PIN_DEFAULT),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         0U,/* Pin 5 */ 
         0U,/* Pin 6 */ 
         0U,/* Pin 7 */ 
         0U,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 5 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 6 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port23                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         (PORT_PIN_DEFAULT),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 1*/
         (PORT_PIN_DEFAULT),/*Pin 2*/
         (PORT_PIN_DEFAULT),/*Pin 3*/
         (PORT_PIN_DEFAULT),/*Pin 4*/
         (PORT_PIN_DEFAULT),/*Pin 5*/
         (PORT_PIN_DEFAULT),/*Pin 6*/
         (PORT_PIN_DEFAULT),/*Pin 7*/
         (PORT_PIN_DEFAULT),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         0U,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         0U,/* Pin 2 */ 
         0U,/* Pin 3 */ 
         0U,/* Pin 4 */ 
         0U,/* Pin 5 */ 
         0U,/* Pin 6 */ 
         0U,/* Pin 7 */ 
         0U,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 2 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 3 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 4 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 5 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 6 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port33                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT1),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT1),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT1),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 8*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 9*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 10*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 11*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         PORT_PIN_LOW,/* Pin 9 */ 
         PORT_PIN_LOW,/* Pin 10 */ 
         PORT_PIN_LOW,/* Pin 11 */ 
         PORT_PIN_LOW,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 8 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 9 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 10 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 11 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port34                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_ALT1),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_OUT | PORT_PIN_OUT_PUSHPULL | PORT_PIN_MODE_GPIO),/*Pin 3*/
         (PORT_PIN_DEFAULT),/*Pin 4*/
         (PORT_PIN_DEFAULT),/*Pin 5*/
         (PORT_PIN_DEFAULT),/*Pin 6*/
         (PORT_PIN_DEFAULT),/*Pin 7*/
         (PORT_PIN_DEFAULT),/*Pin 8*/
         (PORT_PIN_DEFAULT),/*Pin 9*/
         (PORT_PIN_DEFAULT),/*Pin 10*/
         (PORT_PIN_DEFAULT),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         0U,/* Pin 4 */ 
         0U,/* Pin 5 */ 
         0U,/* Pin 6 */ 
         0U,/* Pin 7 */ 
         0U,/* Pin 8 */ 
         0U,/* Pin 9 */ 
         0U,/* Pin 10 */ 
         0U,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE3),/* Pin 3 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 4 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 5 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 6 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 8 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 9 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 10 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port40                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 8*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 9*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 10*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         PORT_PIN_LOW,/* Pin 9 */ 
         PORT_PIN_LOW,/* Pin 10 */ 
         PORT_PIN_LOW,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 8 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 9 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 10 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   },
/*                              Port41                       */
  {
    {
      /* Port pins direction, characteristics and mode configuration */
      {
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 0*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 1*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 2*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 3*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 4*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 5*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 6*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 7*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 8*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 9*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_DOWN | PORT_PIN_MODE_GPIO),/*Pin 10*/
         ((uint8)PORT_PIN_IN | PORT_PIN_IN_PULL_UP | PORT_PIN_MODE_GPIO),/*Pin 11*/
         (PORT_PIN_DEFAULT),/*Pin 12*/
         (PORT_PIN_DEFAULT),/*Pin 13*/
         (PORT_PIN_DEFAULT),/*Pin 14*/
         (PORT_PIN_DEFAULT) /*Pin 15*/ 
      }
    },
    {
      /* Port pins initial level configuration */
      {
         PORT_PIN_LOW,/* Pin 0 */ 
         PORT_PIN_LOW,/* Pin 1 */ 
         PORT_PIN_LOW,/* Pin 2 */ 
         PORT_PIN_LOW,/* Pin 3 */ 
         PORT_PIN_LOW,/* Pin 4 */ 
         PORT_PIN_LOW,/* Pin 5 */ 
         PORT_PIN_LOW,/* Pin 6 */ 
         PORT_PIN_LOW,/* Pin 7 */ 
         PORT_PIN_LOW,/* Pin 8 */ 
         PORT_PIN_LOW,/* Pin 9 */ 
         PORT_PIN_LOW,/* Pin 10 */ 
         PORT_PIN_LOW,/* Pin 11 */ 
         0U,/* Pin 12 */ 
         0U,/* Pin 13 */ 
         0U,/* Pin 14 */ 
         0U /* Pin 15 */ 
      }
    },      
    /* Port pins drive strength1 configuration */
    Portx_lPdrConfig1(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 0 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 1 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 2 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 3 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 4 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 5 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 6 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1) /* Pin 7 */ 
                     ),
    /* Port pins drive strength2 configuration */
    Portx_lPdrConfig2(
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 8 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 9 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 10 */ 
         (PORT_PDR_CMOS_AUTOMOTIVE_LEVEL | PORT_CMOS_SPEED_GRADE1),/* Pin 11 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 12 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 13 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT),/* Pin 14 */ 
         (PORT_PIN_PAD_LEVEL_DEFAULT) /* Pin 15 */ 
                     )
   }
};

static const uint16 Port_DiscSet0[] = 
{     
/*                              Port40                       */
   Port_lDiscSet(
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 0 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 1 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 2 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 3 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 4 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 5 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 6 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 7 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 8 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 9 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 10 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 11 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE,/* Pin 12 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE,/* Pin 13 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE,/* Pin 14 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE /* Pin 15 */ 
                ),
/*                              Port41                       */
   Port_lDiscSet(
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 0 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 1 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 2 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 3 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 4 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 5 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 6 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 7 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 8 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 9 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 10 */ 
                 PORT_PIN_ANALOG_INPUT_ENABLE,/* Pin 11 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE,/* Pin 12 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE,/* Pin 13 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE,/* Pin 14 */ 
                 PORT_PIN_ANALOG_INPUT_DISABLE /* Pin 15 */ 
                )
};

/* PCSR configuration */
static const Port_n_PCSRConfigType Port_kPCSRConfig0[] = 
{

               /*           Port40            */
   Port_lPcsr(
               PORT_PCSR_DEFAULT,  /*Pin0*/
               PORT_PCSR_DISABLE,  /*Pin1*/
               PORT_PCSR_DISABLE,  /*Pin2*/
               PORT_PCSR_DEFAULT,  /*Pin3*/
               PORT_PCSR_DEFAULT,  /*Pin4*/
               PORT_PCSR_DEFAULT,  /*Pin5*/
               PORT_PCSR_DEFAULT,  /*Pin6*/
               PORT_PCSR_DEFAULT,  /*Pin7*/
               PORT_PCSR_DEFAULT,  /*Pin8*/
               PORT_PCSR_DISABLE,  /*Pin9*/
               PORT_PCSR_DISABLE,  /*Pin10*/
               PORT_PCSR_DEFAULT,  /*Pin11*/
               PORT_PCSR_DEFAULT,  /*Pin12*/
               PORT_PCSR_DEFAULT,  /*Pin13*/
               PORT_PCSR_DEFAULT,  /*Pin14*/
               PORT_PCSR_DEFAULT   /*Pin15*/
              ),

               /*           Port41            */
   Port_lPcsr(
               PORT_PCSR_DEFAULT,  /*Pin0*/
               PORT_PCSR_DISABLE,  /*Pin1*/
               PORT_PCSR_DISABLE,  /*Pin2*/
               PORT_PCSR_DEFAULT,  /*Pin3*/
               PORT_PCSR_DEFAULT,  /*Pin4*/
               PORT_PCSR_DEFAULT,  /*Pin5*/
               PORT_PCSR_DEFAULT,  /*Pin6*/
               PORT_PCSR_DEFAULT,  /*Pin7*/
               PORT_PCSR_DEFAULT,  /*Pin8*/
               PORT_PCSR_DISABLE,  /*Pin9*/
               PORT_PCSR_DISABLE,  /*Pin10*/
               PORT_PCSR_DEFAULT,  /*Pin11*/
               PORT_PCSR_DEFAULT,  /*Pin12*/
               PORT_PCSR_DEFAULT,  /*Pin13*/
               PORT_PCSR_DEFAULT,  /*Pin14*/
               PORT_PCSR_DEFAULT   /*Pin15*/
              )
};




const Port_ConfigType Port_ConfigRoot[1] =
{

  {
        /* Port Configuration set 0 */
    &Port_kConfiguration0[0],
    /* Port 40-th and 41-th Disc configuration set 0 */
    &Port_DiscSet0[0],

    /* PCSR Configuration */
    &Port_kPCSRConfig0[0]
  }
};

#define PORT_STOP_SEC_POSTBUILDCFG
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"


/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/


