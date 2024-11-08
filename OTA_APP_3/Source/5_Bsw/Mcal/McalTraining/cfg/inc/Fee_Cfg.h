/******************************************************************************
**                                                                           **
** Copyright (C) Infineon Technologies (2013)                                **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to Infineon      **
** Technologies. Passing on and copying of this document, and communication  **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : Fee_Cfg.h $                                                **
**                                                                           **
**  $CC VERSION : \main\26 $                                                 **
**                                                                           **
**  DATE, TIME: 2021-11-10, 10:43:08                                         **
**                                                                           **
**  GENERATOR : Build b141014-0350                                           **
**                                                                           **
**  AUTHOR    : DL-AUTOSAR-Engineering                                       **
**                                                                           **
**  VENDOR    : Infineon Technologies                                        **
**                                                                           **
**  DESCRIPTION  : FEE configuration generated out of ECU configuration      **
**                   file (Fee.bmd)                                          **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/

#ifndef  FEE_CFG_H
#define  FEE_CFG_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/* Typedefs Imported from Memory Abstract Interface */ 
#include "MemIf_Types.h"

/* Callback functions imported from NvM Module */
#include "NvM_Cbk.h"

/* Functions imported from Fls Module */
#include "Fls_17_Pmu.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/* FEE AS version information */
#define FEE_AS_VERSION (403)
#define FEE_AR_RELEASE_MAJOR_VERSION  (4U)
#define FEE_AR_RELEASE_MINOR_VERSION  (0U)
#define FEE_AR_RELEASE_REVISION_VERSION  (3U)

/* Vendor specific implementation version information */
#define FEE_SW_MAJOR_VERSION  (2U)
#define FEE_SW_MINOR_VERSION  (4U)
#define FEE_SW_PATCH_VERSION  (0U)

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
                        
/*******************************************************************************
**                    Static configuration parameters                         **
*******************************************************************************/

/* Development error detection enabled/disabled */
#define FEE_DEV_ERROR_DETECT       (STD_OFF)

/* Fee_GetVersionInfo API enabled/disabled */
#define FEE_VERSION_INFO_API       (STD_OFF)

/* Fee_GetCycleCount API enabled/disabled */
#define FEE_GET_CYCLE_COUNT_API    (STD_ON)

/* Fee_SetMode API enabled/disabled */
#define FEE_SET_MODE_SUPPORTED     (STD_OFF)

/* Fee_17_GetPrevData API enabled/disabled */
#define FEE_GET_PREV_DATA_API      (STD_ON)

#define FEE_MAX_BYTES_PER_CYCLE      (64U)
/* FEE programs state pages upon detection of virgin flash */
#define FEE_VIRGIN_FLASH_ILLEGAL_STATE      (STD_OFF)

/* Enable/Disable Debug support  */
#define FEE_DEBUG_SUPPORT     (STD_OFF)

/* Erase suspend/resume feature supported in FLS */
#define FEE_FLS_SUPPORTS_ERASE_SUSPEND  (STD_ON)

/* DFlash WordLine size */
#define FEE_DFLASH_WORDLINE_SIZE     (512U)

#define FEE_CONTINUE          (0U)
#define FEE_STOP_AT_GC        (1U)

#define FEE_UNCFG_BLK_OVERFLOW_HANDLE    (FEE_CONTINUE)

/* Virtual page size, i.e., DF_EEPROM page size */
#define FEE_VIRTUAL_PAGE_SIZE      (8U)

/* Logical block's overhead in bytes */
#define FEE_BLOCK_OVERHEAD         (17U)

/* Logical block's data page overhead in bytes */
#define FEE_PAGE_OVERHEAD          (1U)

/* Maximum blocking (delay) time in ms */
#define FEE_MAXIMUM_BLOCKING_TIME  (10U)

/* Maximum number of configured blocks to be handled */
#define FEE_MAX_BLOCK_COUNT        (34U)

/* Symbolic names of logical blocks */
#ifdef FeeConf_FeeBlockConfiguration_IgnCycle 
/* to prevent double declaration */
#error FeeConf_FeeBlockConfiguration_IgnCycle already defined
#else 
#define FeeConf_FeeBlockConfiguration_IgnCycle ((uint16)1)
#endif /* #ifdef FeeConf_FeeBlockConfiguration_IgnCycle */

#ifdef FeeConf_FeeBlockConfiguration_VariantCode 
/* to prevent double declaration */
#error FeeConf_FeeBlockConfiguration_VariantCode already defined
#else 
#define FeeConf_FeeBlockConfiguration_VariantCode ((uint16)2)
#endif /* #ifdef FeeConf_FeeBlockConfiguration_VariantCode */


#define FEE_DISABLE_DEM_REPORT   (0U)
#define FEE_ENABLE_DEM_REPORT    (1U)

/* DEM Configurations */
#define FEE_GC_INIT_DEM_REPORT       (FEE_DISABLE_DEM_REPORT)
#define FEE_WRITE_DEM_REPORT         (FEE_DISABLE_DEM_REPORT)
#define FEE_READ_DEM_REPORT          (FEE_DISABLE_DEM_REPORT)
#define FEE_GC_WRITE_DEM_REPORT      (FEE_DISABLE_DEM_REPORT)
#define FEE_GC_READ_DEM_REPORT       (FEE_DISABLE_DEM_REPORT)
#define FEE_GC_ERASE_DEM_REPORT      (FEE_DISABLE_DEM_REPORT)
#define FEE_INVALIDATE_DEM_REPORT    (FEE_DISABLE_DEM_REPORT)
#define FEE_WRITE_CYCLES_DEM_REPORT  (FEE_DISABLE_DEM_REPORT)
#define FEE_GC_TRIG_DEM_REPORT       (FEE_DISABLE_DEM_REPORT)
#define FEE_UNCFG_BLK_DEM_REPORT     (FEE_DISABLE_DEM_REPORT)
#define FEE_DEM_ENABLED              (STD_OFF)

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define FEE_CONFIG_PTR      (Fee_CfgPtr)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

#endif /* #ifndef FEE_CFG_H */
