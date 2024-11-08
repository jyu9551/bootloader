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
**  $FILENAME   : Fls_Protect.h $                                            **
**                                                                            **
**   $CC VERSION : \main\3 $                                                  **
**                                                                            **
**   $DATE       : 2016-04-23 $                                               **
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

#ifndef  FLS_PROTECT_CFG_H
#define  FLS_PROTECT_CFG_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of McalOsConfig.h */
#include "McalOsConfig.h"
#include MCAL_OS_HEADER
#include "Fls_17_Pmu_Cfg.h"

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define OS_AREA_DRV_FLS                  (101U)
#if (FLS_USER_MODE_INIT_API_ENABLE == STD_ON) 
#define FLS_INIT_RESETENDINIT()          TRUSTED_ApplResetEndInit()
#define FLS_INIT_SETENDINIT()            TRUSTED_ApplSetEndInit()
#define FLS_INIT_WRITE32(reg,value)      \
                            OS_WritePeripheral32((OS_AREA_DRV_FLS),\
                            (volatile uint32*)(volatile void*)(&(reg)),(value));
#define FLS_INIT_MODIFY32(reg,clearmask,setmask)  \
                                      OS_ModifyPeripheral32((OS_AREA_DRV_FLS),\
              (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#if (FLS_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  #define FLS_INIT_USER_MODE_OS_API_READ32(reg)   \
       OS_ReadPeripheral32((OS_AREA_DRV_FLS),\
                                  (volatile uint32*)(volatile void*)(&(reg)))
  #define FLS_INIT_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask)  \
              OS_ModifyPeripheral32((OS_AREA_DRV_FLS),\
             (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#endif                                                                       
#endif /* FLS_USER_MODE_INIT_API_ENABLE == STD_ON  */                        
 
#if (FLS_USER_MODE_RUNTIME_API_ENABLE == STD_ON) 
#if (FLS_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  #define FLS_RUNTIME_USER_MODE_OS_API_READ32(reg)   \
                   OS_ReadPeripheral32((OS_AREA_DRV_FLS),\
                                    (volatile uint32*)(volatile void*)(&(reg)))
  #define FLS_RUNTIME_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask)    \
                 OS_ModifyPeripheral32((OS_AREA_DRV_FLS),\
            (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#endif                                                                       
#endif /* FLS_USER_MODE_RUNTIME_API_ENABLE == STD_ON  */                        
#endif 