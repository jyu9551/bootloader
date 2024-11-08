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
**  $FILENAME   : Hsm_Irq.c $                                                **
**                                                                           **
**  $CC VERSION : \main\4 $                                                  **
**                                                                           **
**  $DATE       : 2016-06-27 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains CAN Module interrupt frames             **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"
#include "Mcal.h"
#include "Irq.h"

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
#define IRQ_START_SEC_CODE
#include "MemMap.h"
/******************************************************************************
** Syntax : void HSMSR0_ISR(void)                                          **
**                                                                           **
** Service ID: NA                                                            **
**                                                                           **
** Sync/Async: Synchronous                                                   **
**                                                                           **
** Reentrancy: Reentrant                                                     **
**                                                                           **
** Parameters (in): None                                                     **
**                                                                           **
** Parameters (out): None                                                    **
**                                                                           **
** Return value: None                                                        **
**                                                                           **
** Description : Service for HSM SR0                                         **
**                                                                           **
******************************************************************************/
#if((IRQ_HSM_SR0_PRIO > 0) || (IRQ_HSM_SR0_CAT == IRQ_CAT23))
#if((IRQ_HSM_SR0_PRIO > 0) && (IRQ_HSM_SR0_CAT == IRQ_CAT1))
IFX_INTERRUPT(HSMSR0_ISR, 0, IRQ_HSM_SR0_PRIO)
#elif IRQ_HSM_SR0_CAT == IRQ_CAT23
ISR(HSMSR0_ISR)
#endif
{
#if (IRQ_HSM_SR0_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif
  /* User Code here */
}
#endif


/******************************************************************************
** Syntax : void HSMSR1_ISR(void)                                          **
**                                                                           **
** Service ID: NA                                                            **
**                                                                           **
** Sync/Async: Synchronous                                                   **
**                                                                           **
** Reentrancy: Reentrant                                                     **
**                                                                           **
** Parameters (in): None                                                     **
**                                                                           **
** Parameters (out): None                                                    **
**                                                                           **
** Return value: None                                                        **
**                                                                           **
** Description : Service for HSM SR1                                         **
**                                                                           **
******************************************************************************/
#if((IRQ_HSM_SR1_PRIO > 0) || (IRQ_HSM_SR1_CAT == IRQ_CAT23))
#if((IRQ_HSM_SR1_PRIO > 0) && (IRQ_HSM_SR1_CAT == IRQ_CAT1))
IFX_INTERRUPT(HSMSR1_ISR, 0, IRQ_HSM_SR1_PRIO)
#elif IRQ_HSM_SR1_CAT == IRQ_CAT23
ISR(HSMSR1_ISR)
#endif
{
#if (IRQ_HSM_SR1_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif
  /* User Code here */
}
#endif

#define IRQ_STOP_SEC_CODE
#include "MemMap.h"

