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
**  $FILENAME   : Fls_Irq.c $                                                **
**                                                                           **
**  $CC VERSION : \main\16 $                                                 **
**                                                                           **
**  $DATE       : 2016-06-27 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains the implementation of interrupt frame   **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of Platform_Types.h and Compiler.h */
#include "Std_Types.h"

/* Include Mcal.h to import the library functions */ 
#include "Mcal.h"

/* Own header file, this includes own configuration file also */
#include "Fls_17_Pmu.h"

/* Include Irq_Cfg.h  */
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

/*******************************************************************************
** Syntax:           void PMU0SR0_ISR(void)                                   **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       Non-reentrant                                            **
**                                                                            **
** Parameters (in):  none                                                     **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value:     none                                                     **
**                                                                            **
** Description :     This is the Interrput Service Routine for PMU0 SR0       **
*******************************************************************************/
#if((IRQ_PMU0_SR0_PRIO > 0) || (IRQ_PMU0_SR0_CAT == IRQ_CAT23))
#if((IRQ_PMU0_SR0_PRIO > 0) && (IRQ_PMU0_SR0_CAT == IRQ_CAT1))
IFX_INTERRUPT(PMU0SR0_ISR, 0, IRQ_PMU0_SR0_PRIO)
#elif IRQ_PMU0_SR0_CAT == IRQ_CAT23
ISR(PMU0SR0_ISR)
#endif
{
  /* Enable Global Interrupts */
#if (IRQ_PMU0_SR0_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  #if( FLS_USE_INTERRUPTS == STD_ON)
  /* Call to Flash Interrupt funtion */
  Fls_17_Pmu_Isr();
  #endif
}
#endif

/*******************************************************************************
** Syntax:           void PMU0SR1_ISR(void)                                   **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       Non-reentrant                                            **
**                                                                            **
** Parameters (in):  none                                                     **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value:     none                                                     **
**                                                                            **
** Description :     This is the Interrput Service Routine for PMU0 SR1       **
*******************************************************************************/
#if((IRQ_PMU0_SR1_PRIO > 0) || (IRQ_PMU0_SR1_CAT == IRQ_CAT23))
#if((IRQ_PMU0_SR1_PRIO > 0) && (IRQ_PMU0_SR1_CAT == IRQ_CAT1))
IFX_INTERRUPT(PMU0SR1_ISR, 0, IRQ_PMU0_SR1_PRIO)
#elif IRQ_PMU0_SR1_CAT == IRQ_CAT23
ISR(PMU0SR1_ISR)
#endif
{
  /* Enable Global Interrupts */
#if (IRQ_PMU0_SR1_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif
}
#endif

#define IRQ_STOP_SEC_CODE
#include "MemMap.h"

