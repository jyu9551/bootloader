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
**  $FILENAME   : Eru_Irq.c $                                                **
**                                                                           **
**  $CC VERSION : \main\12 $                                                 **
**                                                                           **
**  $DATE       : 2016-06-27 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains the interrupt frames for the ERU.       **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* MCAL header file containing global macros, type definitions and functions 
  needed by all MCAL drivers. */
#include "Mcal.h"

/* Include ICU header file - Icu.h */
#include "Icu_17_GtmCcu6.h"

/* MCAL driver utility functions for setting interrupt priority initialization 
  and interrupt category */
#include "Irq.h"

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
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

/*******************************************************************************
** Syntax:           void ERU_ISR_SRN00(void)                                 **
**                                                                            **
** Service ID:       NA                                                       **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       Reentrant                                                **
**                                                                            **
** Parameters (in):  none                                                     **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value:     none                                                     **
**                                                                            **
** Description :     This is Interrput Service request nodes 1 for the  ERU.  **
*******************************************************************************/
#if((IRQ_SCU_ERU_SR0_PRIO > 0) || (IRQ_SCU_ERU_SR0_CAT == IRQ_CAT23))
#if((IRQ_SCU_ERU_SR0_PRIO > 0) && (IRQ_SCU_ERU_SR0_CAT == IRQ_CAT1))
IFX_INTERRUPT(SCUERUSR0_ISR, 0, IRQ_SCU_ERU_SR0_PRIO)
#elif IRQ_SCU_ERU_SR0_CAT == IRQ_CAT23
ISR(SCUERUSR0_ISR)
#endif
{
  /* Enable Global Interrupts */
#if (IRQ_SCU_ERU_SR0_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif
  /* Call Icu Interrupt funtion */
  Icu_17_GtmCcu6_Eru_Isr();  
}
#endif



#define IRQ_STOP_SEC_CODE
#include "MemMap.h"
