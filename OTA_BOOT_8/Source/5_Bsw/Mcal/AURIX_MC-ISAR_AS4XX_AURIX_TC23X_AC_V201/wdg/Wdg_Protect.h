/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2016)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. Passing on and copying of this document, and communication   **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  $FILENAME   : Wdg_Protect.h $                                            **
**                                                                            **
**   $CC VERSION : \main\4 $                                                  **
**                                                                            **
**   $DATE       : 2016-05-02 $                                               **
**                                                                            **
**   AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                            **
**   VENDOR      : Infineon Technologies                                      **
**                                                                            **
**  DESCRIPTION  :This is a demo file and the contents of this
                         file has to be modified by the customer              **
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

#ifndef  WDG_PROTECT_CFG_H
#define  WDG_PROTECT_CFG_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of McalOsConfig.h */
#include "McalOsConfig.h"
#include MCAL_OS_HEADER
#include "Wdg_17_Scu_Cfg.h"

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define OS_AREA_DRV_WDG                  (101U)
#if (WDG_USER_MODE_INIT_API_ENABLE == STD_ON) 
#define WDG_INIT_GETCOREID()          TRUSTED_Mcal_GetCoreId()
#define WDG_INIT_RESETENDINIT()     TRUSTED_ApplResetEndInit()
#define WDG_INIT_SETENDINIT()       TRUSTED_ApplSetEndInit()
#define WDG_INIT_WRITE32(reg,value)  OS_WritePeripheral32((OS_AREA_DRV_WDG),\
                           (volatile uint32*)(volatile void*)(&(reg)),(value))
#define WDG_INIT_MODIFY32(reg,clearmask,setmask)  \
                     OS_ModifyPeripheral32((OS_AREA_DRV_WDG),\
            (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#if (WDG_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)                            
  #define WDG_INIT_USER_MODE_OS_API_WRITE32(reg,value)  \
                    OS_WritePeripheral32((OS_AREA_DRV_WDG),\
                          (volatile uint32*)(volatile void*)(&(reg)),(value))
  #define WDG_INIT_USER_MODE_OS_API_READ32(reg)   \
                 OS_ReadPeripheral32((OS_AREA_DRV_WDG),\
                                   (volatile uint32*)(volatile void*)(&(reg)))
#endif                                                                       
#endif /* WDG_USER_MODE_INIT_API_ENABLE == STD_ON  */                        
                                                                            
#if (WDG_USER_MODE_RUNTIME_API_ENABLE == STD_ON) 

#define WDG_RUNTIME_GETCOREID()        TRUSTED_Mcal_GetCoreId()
#define WDG_RUNTIME_RESETENDINIT()   TRUSTED_ApplResetEndInit()
#define WDG_RUNTIME_SETENDINIT()     TRUSTED_ApplSetEndInit()
#define WDG_RUNTIME_WRITE32(reg, value) OS_WritePeripheral32((OS_AREA_DRV_WDG),\
                          (volatile uint32*)(volatile void*)(&(reg)),(value))
#define WDG_RUNTIME_MODIFY32(reg,clearmask,setmask)  \
                     OS_ModifyPeripheral32((OS_AREA_DRV_WDG),\
             (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#if (WDG_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)                            
  #define WDG_RUNTIME_USER_MODE_OS_API_WRITE32(reg, value)  \
                    OS_WritePeripheral32((OS_AREA_DRV_WDG),\
                          (volatile uint32*)(volatile void*)(&(reg)),(value))
  #define WDG_RUNTIME_USER_MODE_OS_API_READ32(reg)    \
                            OS_ReadPeripheral32((OS_AREA_DRV_WDG),\
                                   (volatile uint32*)(volatile void*)(&(reg)))
#endif                                                                       
#endif /* WDG_USER_MODE_RUNTIME_API_ENABLE == STD_ON  */                        
 #endif 