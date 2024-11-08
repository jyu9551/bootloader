/******************************************************************************
**                                                                           **
** Copyright (C) Infineon Technologies (2015)                                **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to Infineon      **
** Technologies. Passing on and copying of this document, and communication  **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : Spi_Local.h $                                              **
**                                                                           **
**  $CC VERSION : \main\29 $                                                 **
**                                                                           **
**  $DATE       : 2016-07-18 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains local functionality of Spi driver.      **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/

#ifndef SPI_LOCAL_H
#define SPI_LOCAL_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include "Spi.h"

#if (SPI_SLAVE_ENABLE == STD_ON)
#include "SpiSlave.h"
#endif
/*******************************************************************************
**                      Global Macro Definitions                             **
*******************************************************************************/
/* This macro to find the index for Job in the array Spi_JobResult. */
#define SPI_JOB_INDEX   ((uint8)4)
/*
  This macro represents the number of bits required to hold the staus of jobs
*/
#define SPI_JOB_STATUS_BITS  ((uint8)2U)
/*
  This macro extracts the status of the job from two bits
*/
#define SPI_JOB_STATUS_EXTRACT  (3U)

/*
  This macro represents the number of bits required to hold the staus of seq
*/
#define SPI_SEQUENCE_STATUS_BITS  (2U)
/*
This macro is used to find the position of the Job in the array Spi_JobResult
*/
#define SPI_JOB_BIT_POSITION  ((uint8)15)

#if ((SPI_SLAVE_ENABLE == STD_ON) || (SPI_LEVEL_DELIVERED == 1U)          \
                                  || (SPI_LEVEL_DELIVERED == 2U))
/* Dma CHCFGR  reset value */
#define SPI_DMA_CHCFGR_RESETVAL (0x00000000U)

/* Mask to clear SCH in CHCSR */
#define SPI_DMA_CHCSR_SCH_CLR_MASK (~0x80000000U)

/* Macro to Clear WRP Buffer, Dma Channel , TRL Interrupt flags */
#define SPI_DMA_CHCSR_CLR_INTFLAGS (0x06000000U)
#define SPI_DMA_TSR_CLR_INTFLAGS   (0x00040000U)

/* Dma address control reset value */
#define SPI_ADDR_CTRL_RESETVAL (~0x000FFFFFU)

/* Dma ADICR register interrut control,
   enable transaction request lost clear mask */
#define SPI_ADICR_INTCT_ETRL_CLR_MASK (~0x0C800000U)

#endif
/* #if ( (SPI_SLAVE_ENABLE == STD_ON) || (SPI_LEVEL_DELIVERED == 1U)
                                      || (SPI_LEVEL_DELIVERED == 2U))     */

/* AI00250482,AI00249141 fixed */
#define SPI_START_SEC_VAR_8BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
/* Variable Stores the Number of IB Channels in the Configuration */
/*IFX_MISRA_RULE_08_08_STATUS=For violations reported incorrectly only:
Spi_NoOfIBChannels is declared extern only here and is defined in Spi_Local.h.
This violation is reported incorrectly by PC-lint tool  */
extern uint8 Spi_NoOfIBChannels;
#define SPI_STOP_SEC_VAR_8BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#define SPI_START_SEC_ASIL0_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
#if (SPI_SAFETY_ENABLE == STD_ON)
  /* Variable to Hold Job Result. */
  /*IFX_MISRA_RULE_08_08_STATUS=For violations reported incorrectly only:
Spi_AsilJobResult is declared extern only here and is defined in Spi_Local.h.
This violation is reported incorrectly by PC-lint tool  */
  extern uint32 Spi_AsilJobResult[SPI_JOB_ASIL_ARRAY_INDEX];
#endif
/*(SPI_SAFETY_ENABLE == STD_ON) */
#define SPI_STOP_SEC_ASIL0_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#define SPI_START_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
/* Global QM Variables  */
/* Variable to Hold Job Result. */
#if (SPI_QM_MASTER_MODULES_USED != 0U)
/*IFX_MISRA_RULE_08_08_STATUS=For violations reported incorrectly only:
Spi_QmJobResult is declared extern only here and is defined in Spi_Local.h.
This violation is reported incorrectly by PC-lint tool  */
extern uint32 Spi_QmJobResult[SPI_JOB_QM_ARRAY_INDEX];
#endif
#define SPI_STOP_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                              Variable Declarations                         **
*******************************************************************************/

/*
 * To be used for all global or static variables (unspecified size )
 * that are never  initialized.
 */
#define SPI_START_SEC_ASIL1_VAR_UNSPECIFIED
/*
  Allows to map variables, constants and code of modules to individual
  memory sections.
*/
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
/* ((SPI_LEVEL_DELIVERED == 1U) || (SPI_LEVEL_DELIVERED == 2U) \
                                 || (SPI_SLAVE_ENABLE == STD_ON))  */

#if (SPI_SLAVE_ENABLE == STD_ON)

/*IFX_MISRA_RULE_08_08_STATUS=For violations reported incorrectly only:
Spi_SlaveChnlAccess is declared extern only here and is defined in SpiSlave.c.
This violation is reported incorrectly by PC-lint tool  */
extern Spi_SlaveChnlAccessType Spi_SlaveChnlAccess[SPI_NUM_ASIL_SLAVE_MODULES];


#endif

#define SPI_STOP_SEC_ASIL1_VAR_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                             Function Declarations                          **
*******************************************************************************/
#define SPI_START_SEC_CODE
#include "MemMap.h"

extern Spi_JobResultType Spi_lGetJobStatus(Spi_JobType Job);

/* Function to Know if Module/job/sequence is of ASIL/QM kind */
#if (SPI_SAFETY_ENABLE == STD_ON)
/* AI00237167: Windriver compiler bug workaround. IFX_INLINE removed */
extern uint8 Spi_lGetModuleKind(uint8 Module);
extern Spi_SafetyJobType Spi_lGetJobKind (Spi_JobType JobId );
extern Spi_SafetySeqType Spi_lGetSequenceKind (Spi_SequenceType SequenceId );
#endif
/*(SPI_SAFETY_ENABLE == STD_ON)*/

#if (SPI_SLAVE_ENABLE == STD_ON)

/* The Slave Bus Handler should be called from Isr of
 DMA RX when received data count is equal to DmaReceiveCount */
extern void Spi_lSlaveBusHandler(uint32 Module,uint8 ApiAccessId);

/* The Slave Error Handler should be called from Isr of
 DMA RX when received data count is equal to
 DmaReceiveCount and error occured*/
extern void Spi_lSlaveErrorHandler(uint32 Module, uint32 DemError,\
                                                             uint8 ApiAccessId);

/* Get the Status for Channel Interrupt Flag */
extern uint32 Spi_lSlaveDmaGetTxIntStatus(void);

/* CLear the Dma Chanel Request flag */
extern void Spi_lSlaveDmaClrIntrFlag(void);

#if ( SPI_INITCHECK_API == STD_ON )
extern Std_ReturnType Spi_lSlaveInitCheck(const Spi_ConfigType* ConfigPtr);
#endif
/*******************************************************************************
** Syntax           :  Spi_JobResultType Spi_lSlaveGetJobStatus               **
** (                                                                          **
**   Spi_JobType Job                                                          **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Job - Job Id                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Job result.                                             **
**                                                                            **
** Description      : Get the Job Result                                      **
**                                                                            **
** Traceability : [cover parentID=DS_AS403_SPI038]                 [/cover]   **
*******************************************************************************/
extern Spi_JobResultType Spi_lSlaveGetJobStatus(Spi_JobType Job);

/*******************************************************************************
** Traceability :                                                             **
** Syntax : Std_ReturnType Spi_lSlaveInit(const Spi_ConfigType* ConfigPtr)    **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  ConfigPtr - Pointer to configuration set                **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description :Initialization Service for SPI SLAVE .                        **
** The Initialization service initializes all SPI SLAVE relevant              **
** registers and global variables with the values of the structure            **
** referenced by the parameter ConfigPtr.                                     **
*******************************************************************************/
extern Std_ReturnType Spi_lSlaveInit
(
  const Spi_ConfigType* ConfigPtr
);

/*******************************************************************************
** Syntax : Std_ReturnType Spi_lSlaveDeInit (void)                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  1                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : E_OK: de-initialisation command has been accepted.       **
**                   E_NOT_OK: de-initialisation command has not been accepted**
**                                                                            **
** Description : Service For SPI SLAVE Deinitialization                       **
**                                                                            **
*******************************************************************************/
extern void Spi_lSlaveDeInit(void);
#endif



#if ((SPI_LEVEL_DELIVERED == 1U)||(SPI_LEVEL_DELIVERED == 2U))
#if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
/*******************************************************************************
** Syntax : static void Spi_lIntSeqAsyncTransmit(Spi_SequenceType Sequence)   **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : Sequence : Sequence number                               **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : This function will populate the Queue depending upon the     **
**               priority of jobs. updating the jobs also depends on the      **
**               previously loaded sequences are interruptible or not.        **
**                                                                            **
*******************************************************************************/
extern void Spi_lIntSeqAsyncTransmit
(
Spi_SequenceType Sequence,
Spi_JobAndSeqAccessType   *JobSeqDataPtr
);
#endif
#endif

#if ((SPI_LEVEL_DELIVERED == 1U) || (SPI_LEVEL_DELIVERED == 2U) \
                                 || (SPI_SLAVE_ENABLE == STD_ON))
/*******************************************************************************
** Syntax : static Std_ReturnType Spi_lIsQueueAvailable                       **
** (                                                                          **
**    uint16 Jobs,                                                            **
**    Spi_JobAndSeqAccessType *JobSeqDataPtr                                  **
** )                                                                          **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant (for different channel Ids)                        **
**                                                                            **
** Parameters (in) :   uint16 Jobs                                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    :                                                          **
**                                                                            **
**                                                                            **
** Description :  This will increment the queue index and wrap around when    **
**                Queue length reached(if the jobs already in the Q finished  **
*******************************************************************************/
extern Std_ReturnType Spi_lIsQueueAvailable
(
  uint16 Jobs,
  const Spi_JobAndSeqAccessType *JobSeqDataPtr
);

/*******************************************************************************
** Syntax : _IFXEXTERN_ IFX_INLINE uint16 Spi_lIncrementAndRoundIndex         **
** (                                                                          **
**   uint16 StartIndex                                                        **
** )                                                                          **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant (for different channel Ids)                        **
**                                                                            **
** Parameters (in) :   uint16 StartIndex                                      **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    :                                                          **
**                                                                            **
**                                                                            **
** Description :  This will increment the queue index and wrap around when    **
**                Queue length reached(if the jobs already in the Queue       **
                  finished).                                                  **
*******************************************************************************/
_IFXEXTERN_ IFX_INLINE uint16 Spi_lIncrementAndRoundIndex(uint16 StartIndex)
/*IFX_MISRA_RULE_08_05_STATUS=Exemption: Allowed only for inline function in
                              header files*/
{
  StartIndex++;
  if (StartIndex == (SPI_MAX_JOB_TRIG_Q_LENGTH))
  {
    StartIndex = 0U;
  }
  return StartIndex;
}
/*******************************************************************************
** Syntax : extern  void Spi_lHwDisableQspiErrIntr                            **
** (                                                                          **
**   uint32 Module                                                            **
** )                                                                          **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : Module - Hw module no.                                   **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    :                                                          **
**                                                                            **
** Description :  Function to Disable Spi Hw error    Interrupt               **
*******************************************************************************/
extern void  Spi_lHwDisableQspiErrIntr(uint32 Module);

#endif

/*Memory Map of the SPI Code*/
#define SPI_STOP_SEC_CODE
#include "MemMap.h"

#endif /* End of SPI_LOCAL_H */
