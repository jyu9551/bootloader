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
**  $FILENAME   : Fls_17_Pmu_ac.h $                                          **
**                                                                           **
**  $CC VERSION : \main\20 $                                                 **
**                                                                           **
**  $DATE       : 2016-03-23 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION  : This file exports command sequences of Flash driver       **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/

#ifndef  FLS_17_PMU_AC_H
#define  FLS_17_PMU_AC_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Fls_17_Pmu_Cfg.h"

#ifdef FLS_RAM_EMULATION_TEST
#if (FLS_RAM_EMULATION_TEST == 1U)
#include "FlsEmulation.h"
#endif
#endif

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/

/*  The following macros are used in the command cycles.*/
#define CYCLE_5554  ((uint32)(0x00005554U))
#define CYCLE_AAA8  ((uint32)(0x0000AAA8U))
#define CYCLE_55F0  ((uint32)(0x000055F0U))
#define CYCLE_AA50  ((uint32)(0x0000AA50U))
#define CYCLE_AA58  ((uint32)(0x0000AA58U))

#define MASK_HIGHER4_NIBBLES     ((uint32)0xFFFF0000U)
#define POSITION_0               ((uint32)0)

/* The following values are used in the command cycles */
#define VALUE_80    ((uint32)(0x00000080U))
#define VALUE_F0    ((uint32)(0x000000F0U))
#define VALUE_A0    ((uint32)(0x000000A0U))
#define VALUE_5D    ((uint32)(0x0000005DU))
#define VALUE_7A    ((uint32)(0x0000007AU))
#define VALUE_00    ((uint32)(0x00000000U))
#define VALUE_FA    ((uint32)(0x000000FAU))
#define VALUE_50    ((uint32)(0x00000050U))
#define VALUE_70    ((uint32)(0x00000070U))
#define VALUE_CC    ((uint32)(0x000000CCU))
#define VALUE_AA    ((uint32)(0x000000AAU))

#define USER_LEVEL0           (0U)

/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

#define FLS_17_PMU_START_SEC_SPL_CODE_WRITECMD_BEGIN
#include "MemMap.h"
void Fls_WriteCmdCycles(volatile uint32 StartAddress,
                        uint32 PageAddress, 
                        const Fls_AddressType* ProgramDataPtr,
                        uint8 WriteMode);

#define FLS_17_PMU_STOP_SEC_SPL_CODE_WRITECMD_BEGIN
#include "MemMap.h"
                   
                   
#define FLS_17_PMU_START_SEC_SPL_CODE_ERASECMD_BEGIN
#include "MemMap.h"                               
void Fls_EraseCmdCycles(volatile uint32 StartAddress);
#define FLS_17_PMU_STOP_SEC_SPL_CODE_ERASECMD_BEGIN
#include "MemMap.h"

#define FLS_17_PMU_START_SEC_CODE
#include "MemMap.h"

void Fls_ResetReadCmdCycle(void);

void Fls_ClearStatusCmdCycle(void);

void Fls_CallEraseCommand(volatile uint32 PhysicalAddress);
void Fls_CallWriteCommand(volatile uint32 PhysicalAddress, 
                          const Fls_17_Pmu_StateType *s_ptr,
                          uint8 WriteMode);
void Fls_ResumeEraseCmdCycle(volatile uint32 StartAddress); 

/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
/*******************************************************************************
** Syntax:                                                                    **
** _IFXEXTERN_ IFX_INLINE void Cycle5554(volatile uint32 Address, uint32 Data)**
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : Address, Data                                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value : None                                                        **
**                                                                            **
** Description : cmd cycle formation                                          **
*******************************************************************************/
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
_IFXEXTERN_ IFX_INLINE void Cycle5554(volatile uint32 Address, uint32 Data)
{
  /*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
  uint32 StartAddress;
  StartAddress = ((Address) & MASK_HIGHER4_NIBBLES) + CYCLE_5554;
  /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
     address comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
     for address comparison/calculation purpose.*/
  *((uint32*)StartAddress) = Data;
  return;
}

/*******************************************************************************
** Syntax:                                                                    **
**_IFXEXTERN_ IFX_INLINE void CycleAAA8(volatile uint32 Address, uint32 Data) **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : Address, Data                                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value : None                                                        **
**                                                                            **
** Description : cmd cycle formation                                          **
*******************************************************************************/
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
_IFXEXTERN_ IFX_INLINE void CycleAAA8(volatile uint32 Address, uint32 Data)
{
  /*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
  uint32 StartAddress;
  StartAddress = ((Address) & MASK_HIGHER4_NIBBLES) + CYCLE_AAA8;
  /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
     address comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
     for address comparison/calculation purpose.*/
  *((uint32*)StartAddress) = Data;
  return;
}

/*******************************************************************************
** Syntax:                                                                    **
** _IFXEXTERN_ IFX_INLINE void CycleAA50(volatile uint32 Address, uint32 Data)**
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : Address, Data                                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value : None                                                        **
**                                                                            **
** Description : cmd cycle formation                                          **
*******************************************************************************/
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
_IFXEXTERN_ IFX_INLINE void CycleAA50(volatile uint32 Address, uint32 Data)
{
  /*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
  uint32 StartAddress;
  StartAddress = ((Address) & MASK_HIGHER4_NIBBLES) + CYCLE_AA50;
  /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
     address comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
     for address comparison/calculation purpose.*/
  *((uint32*)StartAddress) = Data;
  return;
}

/*******************************************************************************
** Syntax:                                                                    **
** _IFXEXTERN_ IFX_INLINE void CycleAA58(volatile uint32 Address, uint32 Data)**
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : Address, Data                                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value : None                                                        **
**                                                                            **
** Description : cmd cycle formation                                          **
*******************************************************************************/
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
_IFXEXTERN_ IFX_INLINE void CycleAA58(volatile uint32 Address, uint32 Data)
{
  /*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
  uint32 StartAddress;
  StartAddress = ((Address) & MASK_HIGHER4_NIBBLES) + CYCLE_AA58;
  /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
     address comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
     for address comparison/calculation purpose.*/
  *((uint32*)StartAddress) = Data;
  return;
}


/*******************************************************************************
** Syntax:                                                                    **
 _IFXEXTERN_ IFX_INLINE void EnterPageMode(volatile uint32 address,           **
**                                                  uint32 Data)              **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : address , Data                                           **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value : None                                                        **
**                                                                            **
** Description : cmd cycle formation                                          **
*******************************************************************************/
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
_IFXEXTERN_ IFX_INLINE void EnterPageMode(volatile uint32 Address, uint32 Data)
{
  /*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
  uint32 StartAddress;
  
  StartAddress = ((Address) & MASK_HIGHER4_NIBBLES) + CYCLE_5554;
  /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
     address comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
     for address comparison/calculation purpose.*/
  *((uint32*)StartAddress) = Data;
 
  return;
}

/*******************************************************************************
** Syntax:                                                                    **
 _IFXEXTERN_ IFX_INLINE void LoadPage(volatile uint32 Address,                **
**                                    const Fls_AddressType* DataPtr)         **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : address                                                  **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value : None                                                        **
**                                                                            **
** Description : cmd cycle formation                                          **
*******************************************************************************/
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
  files*/
_IFXEXTERN_ IFX_INLINE void LoadPage(volatile uint32 Address, 
                                     const Fls_AddressType* DataPtr)
{
  /*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions defined in header 
     files*/
  Fls_AddressType* StartAddress;
  /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
     address comparison/calculation purpose.*/  
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
     for address comparison/calculation purpose.*/
  StartAddress = (Fls_AddressType *)(((Address) & MASK_HIGHER4_NIBBLES)
                 + CYCLE_55F0);

  *StartAddress = *DataPtr;
  /* Page assembly buffer pointer gets incremented automatically */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
  *(StartAddress + 1) = *(DataPtr + 1U);
 
  return;
}

/*******************************************************************************
**                      Local Constant Definitions                            **
*******************************************************************************/


/*******************************************************************************
**                      Local Variable Definitions                            **
*******************************************************************************/
#define FLS_17_PMU_STOP_SEC_CODE
#include "MemMap.h"

#endif
