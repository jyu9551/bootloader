/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2016)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. This is a demo file and the contents of this file            **
** if required shall be modified by the customer.                             **
**                                                                            **
********************************************************************************
**                                                                            **
**  $FILENAME   : Gpt_Protect.h $                                            **
**                                                                            **
**   $CC VERSION : \main\5 $                                                  **
**                                                                            **
**   $DATE       : 2016-04-21 $                                               **
**                                                                            **
**   AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                            **
**   VENDOR      : Infineon Technologies                                      **
**                                                                            **
**  DESCRIPTION  : This file file define all the first level                  **
**                 indirection macros for Protected mode support              **
**                                                                            **
**                                                                            **
**  MAY BE CHANGED BY USER [Yes/No]: Yes                                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                                                                            **
** Traceabilty      :                                                         **
**                                                                            **
*******************************************************************************/

#ifndef  GPT_PROTECT_CFG_H
#define  GPT_PROTECT_CFG_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of McalOsConfig.h */
#include "McalOsConfig.h"
#include MCAL_OS_HEADER
#include "Gpt_Cfg.h"

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define OS_AREA_DRV_GPT                  (100U)
#if (GPT_USER_MODE_INIT_API_ENABLE == STD_ON)
#if (GPT_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  #define GPT_INIT_USER_MODE_OS_API_WRITE32(reg,value)                         \
                                       OS_WritePeripheral32((OS_AREA_DRV_GPT), \
                           (volatile uint32*)(volatile void*)(&(reg)),(value))
  #define GPT_INIT_USER_MODE_OS_API_READ32(reg)                                \
                                        OS_ReadPeripheral32((OS_AREA_DRV_GPT), \
                                    (volatile uint32*)(volatile void*)(&(reg)))
  #define GPT_INIT_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask)            \
                                       OS_ModifyPeripheral32((OS_AREA_DRV_GPT),\
               (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask))
#endif
#endif /* GPT_USER_MODE_INIT_API_ENABLE == STD_ON  */

#if (GPT_USER_MODE_RUNTIME_API_ENABLE == STD_ON)
#if (GPT_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  #define GPT_RUNTIME_USER_MODE_OS_API_WRITE32(reg,value)                      \
                                        OS_WritePeripheral32((OS_AREA_DRV_GPT),\
                             (volatile uint32*)(volatile void*)(&(reg)),(value))
  #define GPT_RUNTIME_USER_MODE_OS_API_READ32(reg)                             \
                                         OS_ReadPeripheral32((OS_AREA_DRV_GPT),\
                                     (volatile uint32*)(volatile void*)(&(reg)))
#endif
#endif /* GPT_USER_MODE_RUNTIME_API_ENABLE == STD_ON  */

#if (GPT_USER_MODE_DEINIT_API_ENABLE == STD_ON)
#if (GPT_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  #define GPT_DEINIT_USER_MODE_OS_API_WRITE32(reg,value)                       \
                                        OS_WritePeripheral32((OS_AREA_DRV_GPT),\
                             (volatile uint32*)(volatile void*)(&(reg)),(value))
#endif
#endif /* GPT_USER_MODE_DEINIT_API_ENABLE == STD_ON  */
#endif