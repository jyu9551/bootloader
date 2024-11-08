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
**  FILENAME  : Lin_17_AscLin_Cfg.h                                           **
**                                                                            **
**  $CC VERSION : \main\43 $                                                 **
**                                                                            **
**  DATE, TIME: 2019-09-18, 11:14:08                                          **
**                                                                            **
**  GENERATOR : Build b141014-0350                                            **
**                                                                            **
**  MODULE DECRIPTION : Lin_17_AscLin.bmd/.xdm                                **
**                                                                            **
**  VARIANT   : VariantPB                                                     **
**                                                                            **
**  PLATFORM  : Infineon Aurix                                                **
**                                                                            **
**  AUTHOR    : DL-AUTOSAR-Engineering                                        **
**                                                                            **
**  VENDOR    : Infineon Technologies                                         **
**                                                                            **
**  DESCRIPTION  : Lin configuration generated out of ECU configuration       **
**                 file                                                       **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR_SWS_LIN_Driver, Release 4.0.3                  **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: No                                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
  TRACEABILITY : [cover parentID= DS_AS_LIN224,DS_AS4XX_LIN_PR2849,
DS_NAS_LIN_PR127,SAS_NAS_LIN_PR914,SAS_NAS_LIN_PR915,SAS_NAS_LIN_PR916,
SAS_AS4XX_LIN_PR678,DS_NAS_HE2_LIN_PR3054_1,DS_NAS_HE2_LIN_PR3054_2,
DS_NAS_HE2_LIN_PR3054_4,DS_NAS_EP_LIN_PR3054_1,DS_NAS_EP_LIN_PR3054_2,
DS_NAS_EP_LIN_PR3054_4] [/cover] 
*******************************************************************************/
#ifndef LIN_17_ASCLIN_CFG_H 
#define LIN_17_ASCLIN_CFG_H 
  

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/


/*Section to Generate Autosar Specific Version Information*/


#define LIN_17_ASCLIN_AR_RELEASE_MAJOR_VERSION  (4U)
#define LIN_17_ASCLIN_AR_RELEASE_MINOR_VERSION  (0U)
#define LIN_17_ASCLIN_AR_RELEASE_REVISION_VERSION  (3U)

/*Section to Generate Vendor Specific Version Information*/
/* Vendor specific implementation version information */
#define LIN_17_ASCLIN_SW_MAJOR_VERSION   (2U)
#define LIN_17_ASCLIN_SW_MINOR_VERSION   (3U)
#define LIN_17_ASCLIN_SW_PATCH_VERSION   (1U)


/*
         Container : LinGeneralConfiguration
*/

/*
Configuration: LIN_DEV_ERROR_DETECT 
- if STD_ON, DET is Enabled 
- if STD_OFF,DET is Disabled
The detection of all development errors is configurable (On / Off) 
at precompile time. The switch LIN_DEV_ERROR_DETECT (see chapter 10) shall
activate or deactivate the detection of all development errors.

If the switch LIN_DEV_ERROR_DETECT is enabled API parameter checking
is also enabled. 
*/
#define LIN_DEV_ERROR_DETECT  (STD_OFF)


/*
Configuration: LIN_VERSION_INFO_API 
- if STD_ON, Function Lin_GetVersionInfo is available  
- if STD_OFF,Function Lin_GetVersionInfo is not available 
*/
#define LIN_VERSION_INFO_API  (STD_OFF)

/*
Configuration: LIN_PB_FIXEDADDR 
- if STD_ON, Fixed Address feature is Enabled 
- if STD_OFF,Fixed Address feature is Disabled
*/
#define LIN_PB_FIXEDADDR  (STD_OFF)

/*
Configuration: LIN_RESET_SFR_INIT 
- if STD_ON :SFRs will be reset to default values before intialization 
- if STD_OFF :SFRs will not be reset to default values before intialization
*/
#define LIN_RESET_SFR_INIT  (STD_ON)


/*
Configuration: LIN_USER_MODE_RUNTIME_API_ENABLE
- if STD_ON, all APIs except Init and DeInit APIs are running in user mode and 
             protected register access is enabled for those APIs.
- if STD_OFF, all APIs except Init and DeInit APIs are running in supervisor mode and 
             protected register access is disabled for those APIs.
*/
#define LIN_USER_MODE_RUNTIME_API_ENABLE  (STD_OFF)

/*
Configuration: LIN_USER_MODE_INIT_API_ENABLE
- if STD_ON, Init API is running in user mode and 
             protected register access is enabled for Init API.
- if STD_OFF, Init API is running in running in supervisor mode and 
             protected register access is disabled for Init API.
*/
#define LIN_USER_MODE_INIT_API_ENABLE  (STD_OFF)

/*
Configuration: LIN_RUNNING_IN_USER_0_MODE_ENABLE
- if STD_ON, user0 mode is enabled.
- if STD_OFF, user1 mode is enbled.
*/
#define LIN_RUNNING_IN_USER_0_MODE_ENABLE  (STD_OFF)

/*
Configuration: LIN_INDEX 
*/
#define LIN_INDEX  (2U)

/* Number of ASCLIN busses */
#define LIN_MAX_HW_UNIT (2U)

#define LIN_ASCLIN0 (0U)
#define LIN_ASCLIN1 (1U)




/* 
Configuration : LIN_MAXIMUM_CHANNEL 
It is the maximum lin channels configured.
*/
#define LIN_MAXIMUM_CHANNEL (1U)


/*
Configuration: LIN_ASCLINx_USED 
- if STD_ON, ASCLINx is used   
- if STD_OFF,ASCLINx is not used  
*/

#define LIN_ASCLIN0_USED  (STD_OFF)

#define LIN_ASCLIN1_USED  (STD_ON)




/* Total no. of config sets */
#define LIN_17_ASCLIN_CONFIG_COUNT  (1U)



/* Module Debug Support */
#define LIN_DEBUG_SUPPORT   (STD_ON)

/* LinChannelWakeupSupport is enabled for atleast 1 
                                              channel on all the config sets */
#define LIN_GLOBAL_CHANNEL_WAKEUP_SUPPORT   (STD_ON)

#define LIN_ASCLIN_CLC (0x08U)


/* 
   Lin Channel ID Enumerations for Channel Configuration Instance 
   LinGlobalConfig_0
*/
#ifndef LinConf_LinChannel_LinChannel_0
#define LinConf_LinChannel_LinChannel_0  ((uint8)0U)
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
**                      Global Inline Function Definitions                    **
*******************************************************************************/
#endif  /* LIN_17_ASCLIN_CFG_H */

