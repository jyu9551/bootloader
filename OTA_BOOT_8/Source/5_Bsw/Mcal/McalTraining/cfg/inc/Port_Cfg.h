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
**  FILENAME  : Port_Cfg.h                                                    **
**                                                                            **
**  $CC VERSION : \main\dev_tc23x_as4.0.3\8 $                                **
**                                                                            **
**  DATE, TIME: 2024-08-13, 09:38:45                                      **
**                                                                            **
**  GENERATOR : Build b141014-0350                                          **
**                                                                            **
**  AUTHOR    : DL-AUTOSAR-Engineering                                        **
**                                                                            **
**  VENDOR    : Infineon Technologies                                         **
**                                                                            **
**  DESCRIPTION  : Port configuration generated from 
                   ECU configuration file (Port.bmd )                         **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H


#define PORT_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_AR_RELEASE_REVISION_VERSION  (3U)

#define PORT_SW_MAJOR_VERSION  (3U)
#define PORT_SW_MINOR_VERSION  (1U)
#define PORT_SW_PATCH_VERSION  (0U)

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/


/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/

/*
                     Container : PortGeneralConfiguration
*/


/*
Configuration: PORT_DEV_ERROR_DETECT
- if Selected, DET is Enabled 
- if Deselected, DET is Disabled 
*/
#define PORT_DEV_ERROR_DETECT       (STD_OFF)

/*
Configuration: PORT_VERSION_INFO_API 
- if Selected,  Function Port_GetVersionInfo is available  
- if Deselected, Function Port_GetVersionInfo is not available 
*/
#define PORT_VERSION_INFO_API       (STD_OFF)

/*
Configuration: PORT_SET_PIN_MODE_API 
- if Selected,  Function Port_SetPinMode is available  
- if Deselected, Function Port_SetPinMode is not available 
*/
#define PORT_SET_PIN_MODE_API       (STD_OFF)

/*
Configuration: PORT_SET_PIN_DIRECTION_API
- if Selected,  Function Port_SetPinDirection is available  
- if Deselected, Function Port_SetPinDirection is not available 
*/
#define PORT_SET_PIN_DIRECTION_API  (STD_OFF)

/* Port Fixed Address usage */
#define PORT_PB_FIXEDADDR           (STD_OFF)

/*
Configuration: PORT_RUNNING_IN_USER_0_MODE_ENABLE 
- if STD_ON, enable User0 mode 
- if STD_OFF, enable User1 mode 
*/
#define PORT_RUNNING_IN_USER_0_MODE_ENABLE   (STD_OFF)

/*
Configuration: PORT_USER_MODE_INIT_API_ENABLE
- if STD_ON, Enable Protected Mode(user mode) in Init API 
- if STD_OFF, Enable Supervisor mode in Init API  
*/
#define PORT_USER_MODE_INIT_API_ENABLE         (STD_OFF)

/*
Configuration: PORT_USER_MODE_RUNTIME_API_ENABLE
- if STD_ON, Enable Protected Mode(user mode) in APIs other than Init/Deinit APIs
- if STD_OFF, Enable Supervisor mode in APIs other than Init/Deinit APIs
*/
#define PORT_USER_MODE_RUNTIME_API_ENABLE           (STD_OFF)



/*
Configuration: PORT_SAFETY_ENABLE
- if Selected,  Safety feature is enabled
- if Deselected, Safety features is disabled 
*/
#define PORT_SAFETY_ENABLE  (STD_OFF)


/*
Configuration: PORT_INIT_CHECK_API
- if Selected,  Safety feature is enabled
- if Deselected, Safety features is disabled 
*/
#define PORT_INIT_CHECK_API  (STD_OFF)

                                              
/* Definition to specify the ports available on the microcontroller
   Bit value = 0 implies the port is not available
   Bit value = 1 implies the port is available
   Bit 0 is for Port 0, Bit 1 is for Port 1, ... , Bit 31 is for Port 31 
*/
#define PORTS_AVAILABLE_00_31       (0x00f0ec05U)

/* Definition to specify the ports available on the microcontroller
   Bit value = 0 implies the port is not available
   Bit value = 1 implies the port is available
   Bit 0 is for Port 32, Bit 1 is for Port 33, ... , Bit 9 is for Port 41 
*/
#define PORTS_AVAILABLE_32_63       (0x00000306U)

/* Definition to specify the ports that are read only ports on the
   microcontroller
   Bit value = 0 implies the port readable/writable
   Bit value = 1 implies the port is read only port
   Bit 0 is for Port 0, Bit 1 is for Port 1, ... , Bit 31 is for Port 31 
*/
#define PORTS_READONLY_00_31        (0x00000000U)

/* Definition to specify the ports that are read only ports on the
   microcontroller
   Bit value = 0 implies the port readable/writable
   Bit value = 1 implies the port is read only port
   Bit 0 is for Port 32, Bit 1 is for Port 33, ... , Bit 9 is for Port 41 
*/
#define PORTS_READONLY_32_63        (0x00000300U)

/* Maximum Port Number(Hex) */
#define PORT_MAX_NUMBER             (41U)

/* Maximum PortPinID */
#define PORT_MAX_PIN_ID             (0x29fU)

/* Max Port Number available */
#define PORT_MAX_NUM                (15U)


#define PORTS_PCSR_00_31        (0x00000000U)

/* Definition to specify the ports that PCSR on the
   microcontroller
   Bit value = 0 implies the port supports PCSR
   Bit value = 1 implies the port does not support PCSR
   Bit 0 is for Port 32, Bit 1 is for Port 33, ... , Bit 9 is for Port 41 
*/
#define PORTS_PCSR_32_63        (0x00000300U)

/* Definitions to specify the pins available in the port 
   Bit value = 0 implies the pin is not available
   Bit value = 1 implies the pin is available
   Bit 0 is for Pin 0, Bit 1 is for Pin 1, ... , Bit 15 is for Pin 15
*/
#define PORT_AVAILABLE_PINS_PORT0  (0x13ffU)
#define PORT_AVAILABLE_PINS_PORT1  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT2  (0x01ffU)
#define PORT_AVAILABLE_PINS_PORT3  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT4  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT5  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT6  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT7  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT8  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT9  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT10  (0x006eU)
#define PORT_AVAILABLE_PINS_PORT11  (0x1f4cU)
#define PORT_AVAILABLE_PINS_PORT12  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT13  (0x000fU)
#define PORT_AVAILABLE_PINS_PORT14  (0x01ffU)
#define PORT_AVAILABLE_PINS_PORT15  (0x01ffU)
#define PORT_AVAILABLE_PINS_PORT16  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT17  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT18  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT19  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT20  (0x7fcdU)
#define PORT_AVAILABLE_PINS_PORT21  (0x00fcU)
#define PORT_AVAILABLE_PINS_PORT22  (0x001fU)
#define PORT_AVAILABLE_PINS_PORT23  (0x0002U)
#define PORT_AVAILABLE_PINS_PORT24  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT25  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT26  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT27  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT28  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT29  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT30  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT31  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT32  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT33  (0x1fffU)
#define PORT_AVAILABLE_PINS_PORT34  (0x000fU)
#define PORT_AVAILABLE_PINS_PORT35  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT36  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT37  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT38  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT39  (0x0000U)
#define PORT_AVAILABLE_PINS_PORT40  (0x0fffU)
#define PORT_AVAILABLE_PINS_PORT41  (0x0fffU)
/* Total nubber of port config sets */
#define PORT_CONFIG_COUNT   (1U)
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* 
  User Defined Symbolic names of all port pins Port_ConfigSet0
*/

/*
                                   Port0                                      
*/

/* Symbolic Name: Port0 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_0
#define PORT_0_PIN_0                          ((Port_PinType)(0x000))
#endif

/* Symbolic Name: Port0 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_1
#define PORT_0_PIN_1                          ((Port_PinType)(0x001))
#endif

/* Symbolic Name: Port0 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_2
#define PORT_0_PIN_2                          ((Port_PinType)(0x002))
#endif

/* Symbolic Name: Port0 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_3
#define PORT_0_PIN_3                          ((Port_PinType)(0x003))
#endif

/* Symbolic Name: Port0 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_4
#define PORT_0_PIN_4                          ((Port_PinType)(0x004))
#endif

/* Symbolic Name: Port0 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_5
#define PORT_0_PIN_5                          ((Port_PinType)(0x005))
#endif

/* Symbolic Name: Port0 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_6
#define PORT_0_PIN_6                          ((Port_PinType)(0x006))
#endif

/* Symbolic Name: Port0 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_7
#define PORT_0_PIN_7                          ((Port_PinType)(0x007))
#endif

/* Symbolic Name: Port0 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_8
#define PORT_0_PIN_8                          ((Port_PinType)(0x008))
#endif

/* Symbolic Name: Port0 Pin9 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_9
#define PORT_0_PIN_9                          ((Port_PinType)(0x009))
#endif

/* Symbolic Name: Port0 Pin12 */
/* To prevent double declaration */
#ifndef  PORT_0_PIN_12
#define PORT_0_PIN_12                          ((Port_PinType)(0x00c))
#endif

/*
                                   Port2                                      
*/

/* Symbolic Name: Port2 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_0
#define PORT_2_PIN_0                          ((Port_PinType)(0x020))
#endif

/* Symbolic Name: Port2 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_1
#define PORT_2_PIN_1                          ((Port_PinType)(0x021))
#endif

/* Symbolic Name: Port2 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_2
#define PORT_2_PIN_2                          ((Port_PinType)(0x022))
#endif

/* Symbolic Name: Port2 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_3
#define PORT_2_PIN_3                          ((Port_PinType)(0x023))
#endif

/* Symbolic Name: Port2 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_4
#define PORT_2_PIN_4                          ((Port_PinType)(0x024))
#endif

/* Symbolic Name: Port2 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_5
#define PORT_2_PIN_5                          ((Port_PinType)(0x025))
#endif

/* Symbolic Name: Port2 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_6
#define PORT_2_PIN_6                          ((Port_PinType)(0x026))
#endif

/* Symbolic Name: Port2 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_7
#define PORT_2_PIN_7                          ((Port_PinType)(0x027))
#endif

/* Symbolic Name: Port2 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_2_PIN_8
#define PORT_2_PIN_8                          ((Port_PinType)(0x028))
#endif

/*
                                   Port10                                      
*/

/* Symbolic Name: Port10 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_10_PIN_1
#define PORT_10_PIN_1                          ((Port_PinType)(0x0a1))
#endif

/* Symbolic Name: Port10 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_10_PIN_2
#define PORT_10_PIN_2                          ((Port_PinType)(0x0a2))
#endif

/* Symbolic Name: Port10 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_10_PIN_3
#define PORT_10_PIN_3                          ((Port_PinType)(0x0a3))
#endif

/* Symbolic Name: Port10 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_10_PIN_5
#define PORT_10_PIN_5                          ((Port_PinType)(0x0a5))
#endif

/* Symbolic Name: Port10 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_10_PIN_6
#define PORT_10_PIN_6                          ((Port_PinType)(0x0a6))
#endif

/*
                                   Port11                                      
*/

/* Symbolic Name: Port11 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_2
#define PORT_11_PIN_2                          ((Port_PinType)(0x0b2))
#endif

/* Symbolic Name: Port11 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_3
#define PORT_11_PIN_3                          ((Port_PinType)(0x0b3))
#endif

/* Symbolic Name: Port11 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_6
#define PORT_11_PIN_6                          ((Port_PinType)(0x0b6))
#endif

/* Symbolic Name: Port11 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_8
#define PORT_11_PIN_8                          ((Port_PinType)(0x0b8))
#endif

/* Symbolic Name: Port11 Pin9 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_9
#define PORT_11_PIN_9                          ((Port_PinType)(0x0b9))
#endif

/* Symbolic Name: Port11 Pin10 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_10
#define PORT_11_PIN_10                          ((Port_PinType)(0x0ba))
#endif

/* Symbolic Name: Port11 Pin11 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_11
#define PORT_11_PIN_11                          ((Port_PinType)(0x0bb))
#endif

/* Symbolic Name: Port11 Pin12 */
/* To prevent double declaration */
#ifndef  PORT_11_PIN_12
#define PORT_11_PIN_12                          ((Port_PinType)(0x0bc))
#endif

/*
                                   Port13                                      
*/

/* Symbolic Name: Port13 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_13_PIN_0
#define PORT_13_PIN_0                          ((Port_PinType)(0x0d0))
#endif

/* Symbolic Name: Port13 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_13_PIN_1
#define PORT_13_PIN_1                          ((Port_PinType)(0x0d1))
#endif

/* Symbolic Name: Port13 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_13_PIN_2
#define PORT_13_PIN_2                          ((Port_PinType)(0x0d2))
#endif

/* Symbolic Name: Port13 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_13_PIN_3
#define PORT_13_PIN_3                          ((Port_PinType)(0x0d3))
#endif

/*
                                   Port14                                      
*/

/* Symbolic Name: Port14 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_0
#define PORT_14_PIN_0                          ((Port_PinType)(0x0e0))
#endif

/* Symbolic Name: Port14 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_1
#define PORT_14_PIN_1                          ((Port_PinType)(0x0e1))
#endif

/* Symbolic Name: Port14 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_2
#define PORT_14_PIN_2                          ((Port_PinType)(0x0e2))
#endif

/* Symbolic Name: Port14 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_3
#define PORT_14_PIN_3                          ((Port_PinType)(0x0e3))
#endif

/* Symbolic Name: Port14 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_4
#define PORT_14_PIN_4                          ((Port_PinType)(0x0e4))
#endif

/* Symbolic Name: Port14 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_5
#define PORT_14_PIN_5                          ((Port_PinType)(0x0e5))
#endif

/* Symbolic Name: Port14 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_6
#define PORT_14_PIN_6                          ((Port_PinType)(0x0e6))
#endif

/* Symbolic Name: Port14 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_7
#define PORT_14_PIN_7                          ((Port_PinType)(0x0e7))
#endif

/* Symbolic Name: Port14 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_14_PIN_8
#define PORT_14_PIN_8                          ((Port_PinType)(0x0e8))
#endif

/*
                                   Port15                                      
*/

/* Symbolic Name: Port15 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_0
#define PORT_15_PIN_0                          ((Port_PinType)(0x0f0))
#endif

/* Symbolic Name: Port15 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_1
#define PORT_15_PIN_1                          ((Port_PinType)(0x0f1))
#endif

/* Symbolic Name: Port15 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_2
#define PORT_15_PIN_2                          ((Port_PinType)(0x0f2))
#endif

/* Symbolic Name: Port15 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_3
#define PORT_15_PIN_3                          ((Port_PinType)(0x0f3))
#endif

/* Symbolic Name: Port15 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_4
#define PORT_15_PIN_4                          ((Port_PinType)(0x0f4))
#endif

/* Symbolic Name: Port15 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_5
#define PORT_15_PIN_5                          ((Port_PinType)(0x0f5))
#endif

/* Symbolic Name: Port15 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_6
#define PORT_15_PIN_6                          ((Port_PinType)(0x0f6))
#endif

/* Symbolic Name: Port15 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_7
#define PORT_15_PIN_7                          ((Port_PinType)(0x0f7))
#endif

/* Symbolic Name: Port15 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_15_PIN_8
#define PORT_15_PIN_8                          ((Port_PinType)(0x0f8))
#endif

/*
                                   Port20                                      
*/

/* Symbolic Name: Port20 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_0
#define PORT_20_PIN_0                          ((Port_PinType)(0x140))
#endif

/* Symbolic Name: Port20 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_2
#define PORT_20_PIN_2                          ((Port_PinType)(0x142))
#endif

/* Symbolic Name: Port20 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_3
#define PORT_20_PIN_3                          ((Port_PinType)(0x143))
#endif

/* Symbolic Name: Port20 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_6
#define PORT_20_PIN_6                          ((Port_PinType)(0x146))
#endif

/* Symbolic Name: Port20 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_7
#define PORT_20_PIN_7                          ((Port_PinType)(0x147))
#endif

/* Symbolic Name: Port20 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_8
#define PORT_20_PIN_8                          ((Port_PinType)(0x148))
#endif

/* Symbolic Name: Port20 Pin9 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_9
#define PORT_20_PIN_9                          ((Port_PinType)(0x149))
#endif

/* Symbolic Name: Port20 Pin10 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_10
#define PORT_20_PIN_10                          ((Port_PinType)(0x14a))
#endif

/* Symbolic Name: Port20 Pin11 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_11
#define PORT_20_PIN_11                          ((Port_PinType)(0x14b))
#endif

/* Symbolic Name: Port20 Pin12 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_12
#define PORT_20_PIN_12                          ((Port_PinType)(0x14c))
#endif

/* Symbolic Name: Port20 Pin13 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_13
#define PORT_20_PIN_13                          ((Port_PinType)(0x14d))
#endif

/* Symbolic Name: Port20 Pin14 */
/* To prevent double declaration */
#ifndef  PORT_20_PIN_14
#define PORT_20_PIN_14                          ((Port_PinType)(0x14e))
#endif

/*
                                   Port21                                      
*/

/* Symbolic Name: Port21 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_21_PIN_2
#define PORT_21_PIN_2                          ((Port_PinType)(0x152))
#endif

/* Symbolic Name: Port21 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_21_PIN_3
#define PORT_21_PIN_3                          ((Port_PinType)(0x153))
#endif

/* Symbolic Name: Port21 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_21_PIN_4
#define PORT_21_PIN_4                          ((Port_PinType)(0x154))
#endif

/* Symbolic Name: Port21 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_21_PIN_5
#define PORT_21_PIN_5                          ((Port_PinType)(0x155))
#endif

/* Symbolic Name: Port21 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_21_PIN_6
#define PORT_21_PIN_6                          ((Port_PinType)(0x156))
#endif

/* Symbolic Name: Port21 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_21_PIN_7
#define PORT_21_PIN_7                          ((Port_PinType)(0x157))
#endif

/*
                                   Port22                                      
*/

/* Symbolic Name: Port22 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_22_PIN_0
#define PORT_22_PIN_0                          ((Port_PinType)(0x160))
#endif

/* Symbolic Name: Port22 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_22_PIN_1
#define PORT_22_PIN_1                          ((Port_PinType)(0x161))
#endif

/* Symbolic Name: Port22 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_22_PIN_2
#define PORT_22_PIN_2                          ((Port_PinType)(0x162))
#endif

/* Symbolic Name: Port22 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_22_PIN_3
#define PORT_22_PIN_3                          ((Port_PinType)(0x163))
#endif

/* Symbolic Name: Port22 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_22_PIN_4
#define PORT_22_PIN_4                          ((Port_PinType)(0x164))
#endif

/*
                                   Port23                                      
*/

/* Symbolic Name: Port23 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_23_PIN_1
#define PORT_23_PIN_1                          ((Port_PinType)(0x171))
#endif

/*
                                   Port33                                      
*/

/* Symbolic Name: Port33 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_0
#define PORT_33_PIN_0                          ((Port_PinType)(0x210))
#endif

/* Symbolic Name: Port33 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_1
#define PORT_33_PIN_1                          ((Port_PinType)(0x211))
#endif

/* Symbolic Name: Port33 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_2
#define PORT_33_PIN_2                          ((Port_PinType)(0x212))
#endif

/* Symbolic Name: Port33 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_3
#define PORT_33_PIN_3                          ((Port_PinType)(0x213))
#endif

/* Symbolic Name: Port33 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_4
#define PORT_33_PIN_4                          ((Port_PinType)(0x214))
#endif

/* Symbolic Name: Port33 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_5
#define PORT_33_PIN_5                          ((Port_PinType)(0x215))
#endif

/* Symbolic Name: Port33 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_6
#define PORT_33_PIN_6                          ((Port_PinType)(0x216))
#endif

/* Symbolic Name: Port33 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_7
#define PORT_33_PIN_7                          ((Port_PinType)(0x217))
#endif

/* Symbolic Name: Port33 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_8
#define PORT_33_PIN_8                          ((Port_PinType)(0x218))
#endif

/* Symbolic Name: Port33 Pin9 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_9
#define PORT_33_PIN_9                          ((Port_PinType)(0x219))
#endif

/* Symbolic Name: Port33 Pin10 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_10
#define PORT_33_PIN_10                          ((Port_PinType)(0x21a))
#endif

/* Symbolic Name: Port33 Pin11 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_11
#define PORT_33_PIN_11                          ((Port_PinType)(0x21b))
#endif

/* Symbolic Name: Port33 Pin12 */
/* To prevent double declaration */
#ifndef  PORT_33_PIN_12
#define PORT_33_PIN_12                          ((Port_PinType)(0x21c))
#endif

/*
                                   Port34                                      
*/

/* Symbolic Name: Port34 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_34_PIN_0
#define PORT_34_PIN_0                          ((Port_PinType)(0x220))
#endif

/* Symbolic Name: Port34 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_34_PIN_1
#define PORT_34_PIN_1                          ((Port_PinType)(0x221))
#endif

/* Symbolic Name: Port34 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_34_PIN_2
#define PORT_34_PIN_2                          ((Port_PinType)(0x222))
#endif

/* Symbolic Name: Port34 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_34_PIN_3
#define PORT_34_PIN_3                          ((Port_PinType)(0x223))
#endif

/*
                                   Port40                                      
*/

/* Symbolic Name: Port40 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_0
#define PORT_40_PIN_0                          ((Port_PinType)(0x280))
#endif

/* Symbolic Name: Port40 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_1
#define PORT_40_PIN_1                          ((Port_PinType)(0x281))
#endif

/* Symbolic Name: Port40 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_2
#define PORT_40_PIN_2                          ((Port_PinType)(0x282))
#endif

/* Symbolic Name: Port40 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_3
#define PORT_40_PIN_3                          ((Port_PinType)(0x283))
#endif

/* Symbolic Name: Port40 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_4
#define PORT_40_PIN_4                          ((Port_PinType)(0x284))
#endif

/* Symbolic Name: Port40 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_5
#define PORT_40_PIN_5                          ((Port_PinType)(0x285))
#endif

/* Symbolic Name: Port40 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_6
#define PORT_40_PIN_6                          ((Port_PinType)(0x286))
#endif

/* Symbolic Name: Port40 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_7
#define PORT_40_PIN_7                          ((Port_PinType)(0x287))
#endif

/* Symbolic Name: Port40 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_8
#define PORT_40_PIN_8                          ((Port_PinType)(0x288))
#endif

/* Symbolic Name: Port40 Pin9 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_9
#define PORT_40_PIN_9                          ((Port_PinType)(0x289))
#endif

/* Symbolic Name: Port40 Pin10 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_10
#define PORT_40_PIN_10                          ((Port_PinType)(0x28a))
#endif

/* Symbolic Name: Port40 Pin11 */
/* To prevent double declaration */
#ifndef  PORT_40_PIN_11
#define PORT_40_PIN_11                          ((Port_PinType)(0x28b))
#endif

/*
                                   Port41                                      
*/

/* Symbolic Name: Port41 Pin0 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_0
#define PORT_41_PIN_0                          ((Port_PinType)(0x290))
#endif

/* Symbolic Name: Port41 Pin1 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_1
#define PORT_41_PIN_1                          ((Port_PinType)(0x291))
#endif

/* Symbolic Name: Port41 Pin2 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_2
#define PORT_41_PIN_2                          ((Port_PinType)(0x292))
#endif

/* Symbolic Name: Port41 Pin3 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_3
#define PORT_41_PIN_3                          ((Port_PinType)(0x293))
#endif

/* Symbolic Name: Port41 Pin4 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_4
#define PORT_41_PIN_4                          ((Port_PinType)(0x294))
#endif

/* Symbolic Name: Port41 Pin5 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_5
#define PORT_41_PIN_5                          ((Port_PinType)(0x295))
#endif

/* Symbolic Name: Port41 Pin6 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_6
#define PORT_41_PIN_6                          ((Port_PinType)(0x296))
#endif

/* Symbolic Name: Port41 Pin7 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_7
#define PORT_41_PIN_7                          ((Port_PinType)(0x297))
#endif

/* Symbolic Name: Port41 Pin8 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_8
#define PORT_41_PIN_8                          ((Port_PinType)(0x298))
#endif

/* Symbolic Name: Port41 Pin9 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_9
#define PORT_41_PIN_9                          ((Port_PinType)(0x299))
#endif

/* Symbolic Name: Port41 Pin10 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_10
#define PORT_41_PIN_10                          ((Port_PinType)(0x29a))
#endif

/* Symbolic Name: Port41 Pin11 */
/* To prevent double declaration */
#ifndef  PORT_41_PIN_11
#define PORT_41_PIN_11                          ((Port_PinType)(0x29b))
#endif




/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/

#endif /* PORT_CFG_H */

