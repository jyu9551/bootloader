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
**   FILENAME   : SchM.c                                                      **
**                                                                            **
**   CC VERSION : \main\39                                                    **
**                                                                            **
**   DATE       : 2016-04-05                                                  **
**                                                                            **
**   AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                            **
**   VENDOR      : Infineon Technologies                                      **
**                                                                            **
**   DESCRIPTION : This file contains                                         **
**                 - stub for OS functionality.                               **
**                                                                            **
**                                                                            **
**   MAY BE CHANGED BY USER [yes/no]: yes                                     **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of Platform_Types.h and Compiler.h */
#include "Std_Types.h"
#include "McalOsConfig.h"
#include "SchM_Adc.h"
#include "SchM_Icu_17_GtmCcu6.h"
#include "SchM_Can_17_MCanP.h"
#include "SchM_Spi.h"
#include "SchM_Fls_17_Pmu.h"


#include MCAL_OS_HEADER

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/


/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/


/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/


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
**                      Private Function Definitions                          **
*******************************************************************************/


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/********************************ADC*******************************************/

void SchM_Enter_Adc_StartGroup(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_StopGroup(void)
{
  (void)SuspendAllInterrupts();
}
/*Commented for MISRA C compilation */

void SchM_Enter_Adc_ReadGroup(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_EnableHwTrig(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_DisableHwTrig(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_GetGrpStatus(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_GetStreamLastPtr(void)
{
  (void)SuspendAllInterrupts();
}

/* Commented for MISRA C compilation*/

void SchM_Enter_Adc_ScheduleStart(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_ScheduleStop(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_ScheduleNext(void)
{
  (void)SuspendAllInterrupts();
}


void SchM_Enter_Adc_PushQueue(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_Adc_PopQueue(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Adc_StartGroup(void)
{
  (void) ResumeAllInterrupts();
}

void SchM_Exit_Adc_StopGroup(void)
{
  (void) ResumeAllInterrupts();
}
/*Commented for MISCA C compilation */

void SchM_Exit_Adc_ReadGroup(void)
{
  (void) ResumeAllInterrupts();
}

void SchM_Exit_Adc_EnableHwTrig(void)
{
  (void) ResumeAllInterrupts();
}

void SchM_Exit_Adc_DisableHwTrig(void)
{
  (void) ResumeAllInterrupts();
}

void SchM_Exit_Adc_GetGrpStatus(void)
{
  (void) ResumeAllInterrupts();
}

void SchM_Exit_Adc_GetStreamLastPtr(void)
{
  (void) ResumeAllInterrupts();
}
/*Commented for MISCA C compilation */

void SchM_Exit_Adc_ScheduleStart(void)
{
  (void) ResumeAllInterrupts();
}

void SchM_Exit_Adc_ScheduleStop(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Exit_Adc_ScheduleNext(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Exit_Adc_PushQueue(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Exit_Adc_PopQueue(void)
{
  (void)ResumeAllInterrupts();
}

/********************************ICU*******************************************/
/*Commented for MISCA C compilation */
#if 0 
void SchM_Enter_Icu17_GtmCcu6_EnableWakeup(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Icu_17_GtmCcu6_EnableWakeup(void)
{
  (void)ResumeAllInterrupts();
}
#endif 
void SchM_Enter_Icu_17_GtmCcu6_EnableNotification(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Icu_17_GtmCcu6_EnableNotification(void)
{
  (void)ResumeAllInterrupts();
}

/*Commented for MISCA C compilation */
#if 0
void SchM_Enter_Icu_17_GtmCcu6_ResetEdgeCount(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Icu_17_GtmCcu6_ResetEdgeCount(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Icu_17_GtmCcu6_Ccu6IenUpdate(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Icu_17_GtmCcu6_Ccu6IenUpdate(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Icu_17_GtmCcu6_CcuInterruptHandle(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Icu_17_GtmCcu6_CcuInterruptHandle(void)
{
  (void)ResumeAllInterrupts();
}

#endif
/*Commented for MISCA C compilation */
#if 0 
void SchM_Enter_Icu_17_GtmCcu6_CcuVariableupdate(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Icu_17_GtmCcu6_CcuVariableupdate(void)
{
  (void)ResumeAllInterrupts();
}
#endif 
/********************************CAN*******************************************/

void SchM_Enter_Can_17_MCanP_CanDisInt(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Can_17_MCanP_CanDisInt(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Can_17_MCanP_CanEnInt(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Can_17_MCanP_CanEnInt(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Can_17_MCanP_CanWrMO(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Can_17_MCanP_CanWrMO(void)
{
  (void)ResumeAllInterrupts();
}

/********************************FlexRay***************************************/
/*Commented for MISRA c compilation */
/*void SchM_Enter_Fr_17_Eray_ControllerInit(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Fr_17_Eray_ControllerInit(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Fr_17_Eray_SetWakeupChannel(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Fr_17_Eray_SetWakeupChannel(void)
{
  (void)ResumeAllInterrupts();
}*/

/********************************SPI*******************************************/
/*Commented for MISCA C compilation */
#if 0 
void SchM_Enter_Spi_WriteIB(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Exit_Spi_WriteIB(void)
{
  (void)ResumeAllInterrupts();
}
 
void SchM_Enter_Spi_AsyncTransmit(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Exit_Spi_AsyncTransmit(void)
{
  (void)ResumeAllInterrupts();
}
#endif 
void SchM_Enter_Spi_GetSequenceResult(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Exit_Spi_GetSequenceResult(void)
{
  (void)ResumeAllInterrupts();
}
/*Commented for MISCA C compilation */
#if 0 
void SchM_Enter_Spi_Cancel(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Exit_Spi_Cancel(void)
{
  (void)ResumeAllInterrupts();
}
#endif 
void SchM_Enter_Spi_Init(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Exit_Spi_Init(void)
{
  (void)ResumeAllInterrupts();
}
void SchM_Enter_Spi_DeInit(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Exit_Spi_DeInit(void)
{
  (void)ResumeAllInterrupts();
}
void SchM_Enter_Spi_SyncTransmit(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Exit_Spi_SyncTransmit(void)
{
  (void)ResumeAllInterrupts();
}


/********************************WatchDog**************************************/
/*Commented for MISRA c Compilation */

void SchM_Enter_Wdg_17_Scu_TimerHandling(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Enter_Wdg_17_Scu_Trigger(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Enter_Wdg_17_Scu_ChangeMode(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Enter_Wdg_17_Scu_SafetyInit(void)
{
  (void)SuspendAllInterrupts();
} 
void SchM_Enter_Wdg_17_Scu_SafetyTrigger(void)
{
  (void)SuspendAllInterrupts();
}
void SchM_Enter_Wdg_17_Scu_SafetyOffMode(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Wdg_17_Scu_TimerHandling(void)
{
  (void)ResumeAllInterrupts();
}
void SchM_Exit_Wdg_17_Scu_Trigger(void)
{
  (void)ResumeAllInterrupts();
}
void SchM_Exit_Wdg_17_Scu_ChangeMode(void)
{
  (void)ResumeAllInterrupts();
}
void SchM_Exit_Wdg_17_Scu_SafetyInit(void)
{
  (void)ResumeAllInterrupts();
} 
void SchM_Exit_Wdg_17_Scu_SafetyTrigger(void)
{
  (void)ResumeAllInterrupts();
}
void SchM_Exit_Wdg_17_Scu_SafetyOffMode(void)
{
   (void)ResumeAllInterrupts();
}

/********************************FLS**************************************/


void SchM_Enter_Fls_17_Pmu_Init(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Fls_17_Pmu_Init(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Fls_17_Pmu_Erase(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Fls_17_Pmu_Erase(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Fls_17_Pmu_Write(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Fls_17_Pmu_Write(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Fls_17_Pmu_Main(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Fls_17_Pmu_Main(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Fls_17_Pmu_ResumeErase(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Fls_17_Pmu_ResumeErase(void)
{
  (void)ResumeAllInterrupts();
}

/*******************************FLSLOADER**************************************/

/*Commented for MISCA C compilation */
#if 0 
void SchM_Enter_FlsLoader_Erase(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_FlsLoader_Write(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Enter_FlsLoader_lLock(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_FlsLoader_lLock(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Exit_FlsLoader_Write(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Exit_FlsLoader_Erase(void)
{
  (void)ResumeAllInterrupts();
}
#endif 

/********************************ETH*******************************************/

/*Commented for MISCA c Compilation */
/*
void SchM_Enter_Eth_17_EthMac_Transmit(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Eth_17_EthMac_Transmit(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Eth_17_EthMac_ProvideTxBuffer(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Eth_17_EthMac_ProvideTxBuffer(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Eth_17_EthMac_SetControllerMode(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Eth_17_EthMac_SetControllerMode(void)
{
  (void)ResumeAllInterrupts();
}

void SchM_Enter_Eth_17_EthMac_TxRxIrqHandler(void)
{
  (void)SuspendAllInterrupts();
}

void SchM_Exit_Eth_17_EthMac_TxRxIrqHandler(void)
{
  (void)ResumeAllInterrupts();
}
*/
