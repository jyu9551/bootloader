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
**  FILENAME   : Os.c                                                        **
**                                                                           **
**  CC VERSION : \main\14                                                    **
**                                                                           **
**  DATE       : 2016-03-24                                                  **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains                                         **
**                - stub for OS functionality.                               **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of Platform_Types.h and Compiler.h */
#include "Std_Types.h"

/* Inclusion of Tasking sfr file */
#include "IfxCpu_reg.h"

/* Include Mcal for Library */
#include "Mcal.h"

/* Include Os.h*/
#include "Os.h"
/* Include Mcal_DmaLib */
#include "Mcal_DmaLib.h"
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
/*Commented for MISRA c compilation */

static volatile uint8 Os_IntSaveDisableCounter[3];
static volatile sint32 Os_SavedIntLevelNested[3];

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/


/*******************************************************************************
** Syntax           : void OSEKMP_UserEnableAllInterrupts(void)               **
**                                                                            **
** Service ID       :  none                                                   **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This function Enables all interrupts                    **
**                                                                            **
*******************************************************************************/
/*Commented for MISRA C Compilation */
#if 0 
void OSEKMP_UserEnableAllInterrupts(void)
{
  _enable();
}


/*******************************************************************************
** Syntax           : void OSEKMP_UserDisableAllInterrupts(void)              **
**                                                                            **
** Service ID       : none                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This function Disables all interrupts                   **
**                                                                            **
*******************************************************************************/
void OSEKMP_UserDisableAllInterrupts(void)
{
  __disable();
}
#endif

/*******************************************************************************
** Syntax           : void OSEKMP_UserSuspendAllInterrupts(void)              **
**                                                                            **
** Service ID       :  none                                                   **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This function Suspends all interrupts, nested           **
**                                                                            **
*******************************************************************************/

void OSEKMP_UserSuspendAllInterrupts(void)
{
  uint8 CoreId;
  
  /* Get the current core id */
  CoreId = Mcal_GetCoreId();

  if(Os_IntSaveDisableCounter[CoreId] == 0u)
  {
    
    (void)ISYNC();
    Os_SavedIntLevelNested[CoreId] = (sint32)MFCR(CPU_ICR);/* disable interrupts */
    (void)DISABLE();
  }
  Os_IntSaveDisableCounter[CoreId]++;
}

/******************************************************************************/


/*******************************************************************************
** Syntax           : void OSEKMP_UserResumeAllInterrupts(void)               **
**                                                                            **
** Service ID       :  none                                                   **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This function Resumes all interrupts, nested            **
**                                                                **
*******************************************************************************/
void OSEKMP_UserResumeAllInterrupts(void)
{
  uint8 CoreId;
  
  /* Get the current core id */
  CoreId = Mcal_GetCoreId();
  
  if (Os_IntSaveDisableCounter[CoreId] > 0u)
  {
    Os_IntSaveDisableCounter[CoreId]--;
  }
  
  if (Os_IntSaveDisableCounter[CoreId] == 0u)
  {
    if (((Os_SavedIntLevelNested[CoreId]) & ((0x00000001U << 15)))!=0u)
    {
      /* interrupts were enabled, enable again */
      (void)__enable();
    }
  }
}
/******************************************************************************/

/*******************************************************************************
** Syntax           : void Os_GetCurrentStackArea(void  **start, void **end)  **
**                                                                            **
** Service ID       : none                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : start: Start address of Stack, end: Stack End address   **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This function provides the stack's range                **
**                                                                **
*******************************************************************************/
#if 0
void Os_GetCurrentStackArea(void  **start, void **end)
{
  /* Temporarily dummy */
  /* this line is provided to remove unused param warning in GNU */
  UNUSED_PARAMETER(start)
  /* this line is provided to remove unused param warning in GNU */
  UNUSED_PARAMETER(end)

}
/*******************************************************************************
** Syntax           : void Os_UpdateRegSV(volatileuint32 * Address, uint32    **
**               SetMask, uint32 ClearMask, uint8 InitProtectionType,         **
**                        uint16 Area)                                        **
**                                                                            **
** Service ID       : none                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters (in) :  Address - Address of the SFR to be modified             **
** SetMask - Set mask to be ored with the value                               **
** ClearMask - Clear mask to be anded with the value                          **
** InitProtectionType - Type of the protection required for SFR update.       **
**                      MCAL_NO_ENDINIT_PROTECTION (1U)                       **
**                      MCAL_CPU_ENDINIT_PROTECTION  (2U)                     **
**                      MCAL_SAFETY_ENDINIT_PROTECTION  (3U)                  **
** Area - To be used by OS                                                    **
** Parameters(out)   : Value referenced by Address will be modified           **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This function provides the OS interface                 **
**                                                                            **
*******************************************************************************/
void Os_UpdateRegSV (volatile uint32 *Address,uint32 SetMask,uint32 ClearMask,\
                            uint8 InitProtectionType,uint16 Area)
{
  MCAL_REG_MODIFY32(*Address,~ClearMask,SetMask )
  UNUSED_PARAMETER(Area)
  UNUSED_PARAMETER(InitProtectionType)
}

#endif
