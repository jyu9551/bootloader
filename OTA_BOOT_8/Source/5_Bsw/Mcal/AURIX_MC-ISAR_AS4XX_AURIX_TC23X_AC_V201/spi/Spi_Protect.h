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
**  $FILENAME   : Spi_Protect.h $                                            **
**                                                                            **
**   $CC VERSION : \main\4 $                                                  **
**                                                                            **
**   $DATE       : 2016-04-23 $                                               **
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

#ifndef  SPI_PROTECT_CFG_H
#define  SPI_PROTECT_CFG_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of McalOsConfig.h */
#include "McalOsConfig.h"
#include MCAL_OS_HEADER
#include "Spi_Cfg.h"

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define OS_AREA_DRV_SPI                  (101U)
#if (SPI_USER_MODE_INIT_API_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_INIT_RESETENDINIT() TRUSTED_ApplResetEndInit()
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_INIT_SETENDINIT() TRUSTED_ApplSetEndInit()
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_INIT_WRITE32(reg,value)  \
                      OS_WritePeripheral32((OS_AREA_DRV_SPI),(volatile uint32*)\
                                             (volatile void*)(&(reg)),(value))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_INIT_MODIFY32(reg,clearmask,setmask)   \
                                       OS_ModifyPeripheral32((OS_AREA_DRV_SPI),\
              (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#if (SPI_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
  #define SPI_INIT_USER_MODE_OS_API_WRITE32(reg,value) \
                                        OS_WritePeripheral32((OS_AREA_DRV_SPI),\
                             (volatile uint32*)(volatile void*)(&(reg)),(value))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
  #define SPI_INIT_USER_MODE_OS_API_READ32(reg)  \
                                         OS_ReadPeripheral32((OS_AREA_DRV_SPI),\
                                    (volatile uint32*)(volatile void*)(&(reg)))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define SPI_INIT_USER_MODE_MCAL_GETDSPRREMAPADDRESS(address)\
                        (TRUSTED_Mcal_GetDsprReMapAddress(address))
#endif
#endif /* SPI_USER_MODE_INIT_API_ENABLE == STD_ON  */

#if (SPI_USER_MODE_DEINIT_API_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_DEINIT_RESETENDINIT() TRUSTED_ApplResetEndInit()
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_DEINIT_SETENDINIT()     TRUSTED_ApplSetEndInit()
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_DEINIT_WRITE32(reg,value) \
                                       OS_WritePeripheral32((OS_AREA_DRV_SPI),\
                            (volatile uint32*)(volatile void*)(&(reg)),(value))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_DEINIT_MODIFY32(reg,clearmask,setmask)   \
                                      OS_ModifyPeripheral32((OS_AREA_DRV_SPI),\
              (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_DEINIT_MCAL_SETATOMIC(reg,val1,val2,val3) \
                                    (TRUSTED_Mcal_SetAtomic(reg,val1,val2,val3))
#if (SPI_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
  #define SPI_DEINIT_USER_MODE_OS_API_WRITE32(reg,value) \
                                       OS_WritePeripheral32((OS_AREA_DRV_SPI),\
                             (volatile uint32*)(volatile void*)(&(reg)),(value))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
  #define SPI_DEINIT_USER_MODE_OS_API_READ32(reg) \
                                         OS_ReadPeripheral32((OS_AREA_DRV_SPI),\
                                     (volatile uint32*)(volatile void*)(&(reg)))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define SPI_DEINIT_USER_MODE_MCAL_GETDSPRREMAPADDRESS(address)\
                        (TRUSTED_Mcal_GetDsprReMapAddress(address))
#endif
#endif /* SPI_USER_MODE_DEINIT_API_ENABLE == STD_ON  */

#if (SPI_USER_MODE_RUNTIME_API_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define SPI_RUNTIME_MODIFY32(reg,clearmask,setmask)  \
                                       OS_ModifyPeripheral32((OS_AREA_DRV_SPI),\
              (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMAGETTRANSREQLOSTFLAG(val)  \
                                      (TRUSTED_Mcal_DmaGetTransReqLostFlag(val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMAGETTRANSFERINTSTATUS(val)\
                                     (TRUSTED_Mcal_DmaGetTransferIntStatus(val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMADISABLEINTR(val) (TRUSTED_Mcal_DmaDisableIntr(val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMACHCLRINTCTETRLFLAGS(val) \
                                      (TRUSTED_Mcal_DmaChClrIntctEtrlFlags(val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMACHCLRINTRFLAGS(val) \
                                           (TRUSTED_Mcal_DmaChClrIntrFlags(val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMACLRERRFLAGS(val) (TRUSTED_Mcal_DmaClrErrFlags(val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_SETATOMIC(reg,val1,val2,val3) \
                                    (TRUSTED_Mcal_SetAtomic(reg,val1,val2,val3))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMACFGINTCONTROL(channel,val) \
                                    (TRUSTED_Mcal_DmaCfgIntControl(channel,val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMAENABLECHINTRTRIGGER(channel)\
                                 (TRUSTED_Mcal_DmaEnableChIntrTrigger(channel))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMADISABLECHINTRTRIGGER(channel)\
                                (TRUSTED_Mcal_DmaDisableChIntrTrigger(channel))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMAENABLEINTR(channel) \
                                          (TRUSTED_Mcal_DmaEnableIntr(channel))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMASETDESTADDR(channel,reg)\
                                      (TRUSTED_Mcal_DmaSetDestAddr(channel,reg))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMASETADICR(channel,val)\
                                 (TRUSTED_Mcal_DmaSetAdicr(channel,val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMASETTXCOUNT(channel,val)\
                                  (TRUSTED_Mcal_DmaSetTxCount(channel,val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMASETDATAWIDTH(channel,val)\
                                 (TRUSTED_Mcal_DmaSetDataWidth(channel,val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMASETSOURCEADDR(channel,reg) \
                                  (TRUSTED_Mcal_DmaSetSourceAddr(channel,reg))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define SPI_RUNTIME_MCAL_DMAENABLEHWTRANSFER(channel)\
                                (TRUSTED_Mcal_DmaEnableHwTransfer(channel))
#if (SPI_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
  #define SPI_RUNTIME_USER_MODE_OS_API_WRITE32(reg, value) \
                                       OS_WritePeripheral32((OS_AREA_DRV_SPI),\
                            (volatile uint32*)(volatile void*)(&(reg)),(value))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
  #define SPI_RUNTIME_USER_MODE_OS_API_READ32(reg) \
                                         OS_ReadPeripheral32((OS_AREA_DRV_SPI),\
                                     (volatile uint32*)(volatile void*)(&(reg)))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
  #define SPI_RUNTIME_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask)  \
                                       OS_ModifyPeripheral32((OS_AREA_DRV_SPI),\
              (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
   #define SPI_RUNTIME_USER_MODE_MCAL_DMASETTXCOUNT(channel,val)\
                    (TRUSTED_Mcal_DmaSetTxCount(channel,val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define SPI_RUNTIME_USER_MODE_MCAL_DMASETDATAWIDTH(channel,val)\
                    (TRUSTED_Mcal_DmaSetDataWidth(channel,val))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define SPI_RUNTIME_USER_MODE_MCAL_DMAENABLEHWTRANSFER(channel)\
                        (TRUSTED_Mcal_DmaEnableHwTransfer(channel))
#endif
#endif /* SPI_USER_MODE_RUNTIME_API_ENABLE == STD_ON  */
 #endif