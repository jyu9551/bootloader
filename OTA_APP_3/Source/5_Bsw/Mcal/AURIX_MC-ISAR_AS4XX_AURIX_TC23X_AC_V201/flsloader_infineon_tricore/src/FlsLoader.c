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
**  $FILENAME   : FlsLoader.c $                                              **
**                                                                           **
**  $CC VERSION : \main\95 $                                                 **
**                                                                           **
**  $DATE       : 2015-08-04 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains                                         **
**                functionality of Flsloader driver.                         **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                                                                            **
TRACEABILITY : [cover parentID=DS_NAS_FLSLOADER_PR647,
DS_NAS_FLSLOADER_PR127,DS_NAS_FLSLOADER_PR647,
DS_NAS_FLSLOADER_PR123,DS_NAS_FLSLOADER_PR759_PR760_PR761,
DS_NAS_FLSLOADER_PR1597_PR1598_PR1599_PR1600_PR1686_PR1687_PR1761_PR1762,
DS_NAS_FLSLOADER_PR1604_PR1605_PR1606_PR1607_PR1688_PR1689_PR1763_PR1764,
DS_NAS_FLSLOADER_PR637,DS_NAS_FLSLOADER_PR638,DS_NAS_FLSLOADER_PR639,
SAS_NAS_ALL_PR630_PR631,DS_NAS_FLSLOADER_PR115,DS_NAS_FLSLOADER_PR2986]
[/cover]
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

  /* Inclusion of Tasking sfr file */
#include "IfxFlash_reg.h"

#include "IfxSrc_reg.h"

  /* Inclusion of Mcal Specific Global Header File */
#include "Mcal.h"

  /* Inclusion of Flash header file */
#include "FlsLoader.h"

  /* Inclusion of Flash header file */
#include "FlsLoader_Local.h"

#include "IfxCpu_reg.h"

#include "IfxScu_reg.h"

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#ifndef FLSLOADER_SW_MAJOR_VERSION
  #error "FLSLOADER_SW_MAJOR_VERSION is not defined. "
#endif

#ifndef FLSLOADER_SW_MINOR_VERSION
  #error "FLSLOADER_SW_MINOR_VERSION is not defined. "
#endif

#ifndef FLSLOADER_SW_PATCH_VERSION
  #error "FLSLOADER_SW_PATCH_VERSION is not defined. "
#endif

#if ( FLSLOADER_SW_MAJOR_VERSION != 2U )
  #error "FLSLOADER_SW_MAJOR_VERSION does not match. "
#endif

#if ( FLSLOADER_SW_MINOR_VERSION != 2U )
  #error "FLSLOADER_SW_MINOR_VERSION does not match. "
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
  /* To differentiate between PFLASH and DFLASH the following value is written
  in the command cycle. */
#define FLSLOADER_PFLASH_VAL            ((FlsLoader_AddressType)(0x00000050U))

  /* The flash bank are assigned with values with respect to location in
  FLASH_FLASH0_FSR register. */

#define FLSLOADER_PROG_MASK             ((uint32)(0x00000080U))
#define FLSLOADER_ERASE_MASK            ((uint32)(0x00000100U))
  /*The following to macros are used to determine the status of Flash. */
#define FLSLOADER_NORMAL                (0U)
#define FLSLOADER_SUSPEND               (1U)
#define FLSLOADER_UNINTIALIZED          (0U)

  /* Flash NULL */
#define FLSLOADER_NULL                  ((void *) 0)
#define FLSLOADER_ZERO_DATA             (0x00000000U)
#define FLSLOADER_PFLASH_ADDRESS_MASK   ((FlsLoader_AddressType)(0x00FFFFFFU))
#define FLSLOADER_ADDRESS_HI_5_MASK     ((FlsLoader_AddressType)(0x000FFFFFU))
#define FLSLOADER_SHIFT_BY_16           (16U)
#define FLSLOADER_SHIFT_BY_8            (8U)
#define FLSLOADER_SHIFT_BY_15           (15U)
#define FLSLOADER_NUM_5                 (5U)
#define FLSLOADER_NUM_32                (32U)
#define FLSLOADER_NUM_256               (256U)
#define FLSLOADER_NUM_8                 (8U)
#define FLSLOADER_UCB0                  (0U)
#define FLSLOADER_UCB1                  (1U)
#define FLSLOADER_PWD                   (8U)
#define FLSLOADER_NUM_3                 (3U)
  /* (17*8)bytes UCB <P/D>FLASH content */
#define FLSLOADER_FLASH_TRAP_DIS        (0x00008000U)
#define FLSLOADER_FLASH_TRAP_EN         (0xFFFF7FFFU)

  /* Program Verify Error */
#define FLSLOADER_PVER_ERROR                  (0x02000000U)
  /* Erase Verify Error */
#define FLSLOADER_EVER_ERROR                  (0x04000000U)

  /* Flash command should point this Address */
#define FLSLOADER_CMD_BASE  ((FlsLoader_AddressType)(0xAF000000U))

#define FLASH_HW_MODULE ((volatile Ifx_FLASH *)(void *) &(MODULE_FLASH0))
#define FLASH0_PROCONP ((volatile Ifx_FLASH_PROCONP*)(void *)(&FLASH0_PROCONP0))
#define FLASH0_PROCONOTP \
         ((volatile Ifx_FLASH_PROCONOTP*)(void *)(&FLASH0_PROCONOTP0))
#define FLASH0_PROCONWOP \
         ((volatile Ifx_FLASH_PROCONWOP*)(void *)(&FLASH0_PROCONWOP0))



#define FLSLOADER_WDT_PASSWORD_LOWER_MASK  ( 0x000000FCu )

  /* mask for password Bits 8-15 */
#define FLSLOADER_WDT_PASSWORD_UPPER_MASK  ( 0x0000FF00u )

  /* mask for password bits */
#define FLSLOADER_WDT_PASSWORD_MASK   \
                                    (FLSLOADER_WDT_PASSWORD_LOWER_MASK | \
                                     FLSLOADER_WDT_PASSWORD_UPPER_MASK)

#define FLSLOADER_WDT_RELOAD_MASK          ( 0xFFFF0000u )


  /* mask for LCK and ENDINIT status bits */
#define FLSLOADER_WDT_STATUS_MASK          ( 0x00000003u )


/*******************************************************************************
**                      Private function like Macros                          **
*******************************************************************************/

  /*
  Function Like Macro:FlsLoader_lGetMode
  macro to get the kind of flash
  Input Parameters: FlashType
  */
  /*IFX_MISRA_RULE_19_07_STATUS=Function like macro used for code simplication 
  and readability*/
#define FlsLoader_lGetMode(FlashType) (((FlashType)!=(FLSLOADER_DFLASH_BANK0)) \
                             ? (FLSLOADER_PFLASH_VAL) : (FLSLOADER_DFLASH_VAL))

/******************************************************************************
**                      Global Type Definitions                              **
******************************************************************************/
/******************************************************************************
**                      Private Type Definitions                             **
******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/******************************************************************************
**                      Private Variable Definitions                         **
******************************************************************************/

#ifdef IFX_FLSLOADER_DEBUG
#define FLSLOADER_START_SEC_VAR_32BIT
#include "MemMap.h"
    extern volatile uint32 TestFlsloader_DebugMask01;
    extern volatile uint32 TestFlsloader_DebugMask02;
    extern volatile uint32 TestFlsloader_DebugMask04;
    extern volatile uint32 TestFlsloader_DebugMask05;
    extern volatile uint32 TestFlsloader_DebugMask06;
    extern volatile uint32 TestFlsloader_DebugMask07;
    extern volatile uint32 TestFlsloader_DebugMask09;
    
#define FLSLOADER_STOP_SEC_VAR_32BIT
#include "MemMap.h"
#endif

#define FLSLOADER_START_SEC_VAR_8BIT
#include "MemMap.h"
  static uint8 FlsLoader_Status;
#if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  uint8  FlsLoader_InitStatus;
#endif
#define FLSLOADER_STOP_SEC_VAR_8BIT
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor 
  directives is allowed only for MemMap.h*/
#include "MemMap.h"

#define FLSLOADER_START_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor 
  directives is allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
**                    Prototypes Of Local Functions                           **
*******************************************************************************/
  /* funtion to check program verification error */
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lPverCheck(void);

  /* funtion to check Erase verification error */
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lEverCheck(void);

IFX_LOCAL_INLINE void FlsLoader_lResetSafetyENDINIT(void);

IFX_LOCAL_INLINE void FlsLoader_lSetSafetyENDINIT(void);


IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lBurstWrite (
                            uint32 BurstLoadPageCount,
                            FlsLoader_AddressType BurstOffSet,
                            uint32 FlashType,
                            const uint8 *SourceAddressPtr,
                            FlsLoader_AddressType TargetAddress,
                            uint32 NumberOfBursts);

IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lPageWrite (
                           uint32 LoadPageCount,
                           FlsLoader_AddressType OffSet,
                           uint32 FlashType,
                           const uint8 *SourceAddressPtr,
                           FlsLoader_AddressType TargetAddress,
                           uint32 NoOfPages);

#define FLSLOADER_STOP_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor 
  directives is allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
  /* Mapping the code */
#define FLSLOADER_START_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor 
  directives is allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1623]                    **
**            [/cover]                                                        **
** Syntax           :  FlsLoader_ReturnType FlsLoader_Init(                   **
**                      const FlsLoader_ConfigType* Address)                  **
**                                                                            **
** Service ID       : 0x00                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : Address :NULL Expected                                  **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType:(If DET is not enabled)            **
**                    1.FLSLOADER_E_OK                                        **
**                    2.FLSLOADER_E_ROMVERSION                                **
**                    1.FLSLOADER_E_NOT_OK                                    **
**                                                                            **
** Description      : This function initilizes the flash module and checks    **
**                    if all the flash sectors is configured as ROM           **
**                                                                            **
*******************************************************************************/
/*
CD_FlsLoader028: This is the initialization function for FlsLoader.
*/
FlsLoader_ReturnType FlsLoader_Init(const FlsLoader_ConfigType* Address)
{
  FlsLoader_ReturnType  RetError;
  UNUSED_PARAMETER(Address)

  /* Init RetError to OK */
  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  RetError = FLSLOADER_E_OK; 
  #endif
  /*
  Check if Passed Address is NULL
  */

  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  if ( Address != FLSLOADER_NULL )
  {
    /* Report FLSLOADER_E_PARAM_IGNORED DET */
    Det_ReportError(
                    FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_INIT,
                    FLSLOADER_E_PARAM_IGNORED);
  
    RetError = FLSLOADER_E_NOT_OK;
  }
  if (RetError != FLSLOADER_E_NOT_OK)
  #endif
  {
    RetError = FlsLoader_lCheckOTPWOP();
    /*FLASH_FCON register will be initialized in the start up. */
    FlsLoader_Status = FLSLOADER_NORMAL;
    #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
    FlsLoader_InitStatus = FLSLOADER_INITIALIZED;
    #endif
    Mcal_ResetENDINIT();
    /* Disable traps on Uncorrectable errors */
    FLASH0_MARP.U |= FLSLOADER_FLASH_TRAP_DIS;
    /* Set End Init Protection */
    Mcal_SetENDINIT();
    /* Disable traps on Uncorrectable errors */
    FLASH0_MARD.U |= FLSLOADER_FLASH_TRAP_DIS;
  }
  return (RetError);
}  /*End of Fls_Loader_Init */

  /*Preprocessor switch to enable/disable the API to deinitialze
  FlsLoader. */
#if (FLSLOADER_DEINIT_API == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1624]                    **

** Syntax           :  FlsLoader_ReturnType FlsLoader_DeInit(void)            **
**            [/cover]                                                        **
** Service ID       : 0x01                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : void                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType:(If DET is not enabled)            **
**                    1. FLSLOADER_E_OK                                       **
**                    2. FLSLOADER_E_BUSY                                     **
**                    1.FLSLOADER_E_BUSY                                      **
**                                                                            **
** Description      : This function deinitilizes the flash module.            **
**                    This Function sets the registers to their default       **
**                    state and executes the reset to read command.           **
**                                                                            **
*******************************************************************************/
  /*
  CD_FlsLoader030: Service for FlsLaderDeInitialization
  */
FlsLoader_ReturnType FlsLoader_DeInit(void)
{
  FlsLoader_ReturnType  RetError;
  uint32 Fls0FsrCheck;
  /* Init RetError to OK */
  RetError = FLSLOADER_E_OK;

  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  if (FlsLoader_InitStatus != FLSLOADER_INITIALIZED)
  {
    /* CD_FlsLoader032: FlsLoader not initialized,Report to DET*/
    Det_ReportError (
                      FLSLOADER_MODULE_ID,
                      FLSLOADER_INSTANCE_ID,
                      FLSLOADER_SID_DEINIT,
                      FLSLOADER_E_UNINIT);
    RetError = FLSLOADER_E_NOT_OK;
  }
  if (RetError != FLSLOADER_E_NOT_OK )
  #endif
  {
    /* Check for Flash Busy*/
    #ifdef IFX_FLSLOADER_DEBUG
    Fls0FsrCheck = ((FLASH0_FSR.U & ((uint32)FLSLOADER_FLASH_BUSY_MASK)) |
                     TestFlsloader_DebugMask01);
    #else  /*#ifdef IFX_FLSLOADER_DEBUG*/
    Fls0FsrCheck = (FLASH0_FSR.U & (uint32)(FLSLOADER_FLASH_BUSY_MASK));
    #endif  /*#ifdef IFX_FLSLOADER_DEBUG*/
    if (Fls0FsrCheck)
    {
      /* Report FLSLOADER_E_BUSY DET */
      RetError = FLSLOADER_E_BUSY;
      #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
      Det_ReportError (
                      FLSLOADER_MODULE_ID,
                      FLSLOADER_INSTANCE_ID,
                      FLSLOADER_SID_DEINIT,
                      (uint8)FLSLOADER_E_BUSY);
      #endif
    }
    else
    {
      FlsLoader_Status = FLSLOADER_NORMAL;
      #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
      FlsLoader_InitStatus = FLSLOADER_UNINTIALIZED;
      #endif
      /* Reset to Read PFLASH, DFLASH (both banks) */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
      volatile uint32 to avoid compiler optimization in command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
      address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
      address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FlsLoader_lResetRead
      (FLSLOADER_DFLASH0_START_ADDRESS)) = FLSLOADER_VALUE_F0;
    }
    /* Reset EndInit protection */
    Mcal_ResetENDINIT();
    /* Enable traps on Uncorrectable errors */
    FLASH0_MARP.U &= FLSLOADER_FLASH_TRAP_EN;
    /* Set End Init Protection */
    Mcal_SetENDINIT();
    /* Enable traps on Uncorrectable errors */
    FLASH0_MARD.U &= FLSLOADER_FLASH_TRAP_EN; 
  }
  return (RetError);
}  /*End of function FlsLoader_DeInit */
#endif  /*(FLSLOADER_DEINIT_API == STD_ON) */
#define FLSLOADER_STOP_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor 
  directives is allowed only for MemMap.h*/
#include "MemMap.h"

#define FLSLOADER_START_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor 
  directives is allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1625]                    **
**            [/cover]                                                        **
** Syntax           :  FlsLoader_ReturnType FlsLoader_Write(                  **
**                      FlsLoader_AddressType TargetAddress,                  **
**                      FlsLoader_LengthType Length,                          **
**                      const uint8 *SourceAddressPtr                         **
**                    )                                                       **
**                                                                            **
** Service ID       : 0x02                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : Address:Source data Address                             **
**                    Length :Length of the data in bytes                     **
**                    - multiple of 256 bytes(8 pages)for PFLASH              **
**                    - multiple of 32 bytes (4 pages)for DFLASH              **
**                    TargetAddress :Destination Address in Flash             **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType                                    **
**                    1.FLSLOADER_E_OK                                        **
**                    2.FLSLOADER_E_BUSY                                      **
**                    3.FLSLOADER_E_NOT_OK                                    **
**                    4.FLSLOADER_E_LOCKED                                    **
**                                                                            **
**                    1.FLSLOADER_E_NOT_OK for all errors                     **
**                                                                            **
** Description      : This function is used to program a page of internal     **
**                    flash using Burst mode of programming                   **
**                    This function does not check whether the TargetAddress  **
**                    crosses the boundry from one sector to another          **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_Write(FlsLoader_AddressType TargetAddress,
                                     FlsLoader_LengthType Length,
                                     const uint8 *SourceAddressPtr)
{
  FlsLoader_ReturnType  RetError;
  FlsLoader_AddressType OffSet;
  FlsLoader_AddressType BurstOffSet;
  uint32                FlashType;
  uint32                NumberOfBursts;
  uint32                FlsFsrCheck;
  uint32                Icr;
  uint32                BytesToWrite;
  uint32                NoOfPages;
  uint32                LoadPageCount;
  uint32                BurstLoadPageCount;


  /*Block 1: DET Check */
  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  RetError = FlsLoader_lWriteDetCheck(Length,TargetAddress);

  if (RetError != FLSLOADER_E_NOT_OK)
  #endif
  {
    #ifdef IFX_FLSLOADER_DEBUG
    FlsFsrCheck = ((FLASH0_FSR.U & ((uint32)FLSLOADER_FLASH_BUSY_MASK)) |
                    TestFlsloader_DebugMask01);
    #else  /*#ifdef IFX_FLSLOADER_DEBUG*/
    /*Check for Busy state of FLASH
    Registers differ between Checking of Busy state of PFlash and DFlash */
    FlsFsrCheck = (FLASH0_FSR.U & ((uint32)FLSLOADER_FLASH_BUSY_MASK));
    #endif  /*#ifdef IFX_FLSLOADER_DEBUG*/
    if ( FlsFsrCheck )
    {
      /* Report FLSLOADER_E_BUSY DET */
      #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
      Det_ReportError(
                    FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_WRITE,
                    (uint8)FLSLOADER_E_BUSY);
      #endif
      RetError = FLSLOADER_E_BUSY;
    }
    else if ( SourceAddressPtr == FLSLOADER_NULL)
    {
      RetError = FLSLOADER_E_NOT_OK;
    }
    else
    { 
      /*PFLASH0*/
      /* Block 2: check the Flash Type DFLASH/PFLASH .
      If the Flash type is PFLASH check PMU0. */
      FlashType = FlsLoader_lGetFlashType (TargetAddress);
      /* Protection Installation check  */
      /* Preprocessor switch to enable/disable the lockcheck functionality */
      /*#if (FLSLOADER_ENABLE_LOCKCHECK == STD_ON)*/
      #if (FLSLOADER_ENABLE_LOCKCHECK == STD_ON)
      RetError = FlsLoader_lFlashLockCheck(TargetAddress,FlashType,Length);
      if (FLSLOADER_E_OK == RetError)
      #endif
      {
        /* Count the number of loops */
        if (FlashType != FLSLOADER_DFLASH_BANK0)
        {
          NumberOfBursts  = Length >> FLSLOADER_SHIFT_BY_8; /* Divide by 256 */
          LoadPageCount  = FLSLOADER_NUM_4;
          BurstLoadPageCount = FLSLOADER_NUM_32;
          OffSet       = (FlsLoader_AddressType)FLSLOADER_NUM_32;
          BurstOffSet  = (FlsLoader_AddressType)FLSLOADER_NUM_256;
          BytesToWrite = (Length - (NumberOfBursts * FLSLOADER_NUM_256));
          NoOfPages    = (BytesToWrite / FLSLOADER_NUM_32);
        }
        else
        {
          NumberOfBursts  = Length >> FLSLOADER_NUM_5; /* Divide by 32 */
          LoadPageCount      = 1U;
          BurstLoadPageCount = FLSLOADER_NUM_4;
          OffSet       = (FlsLoader_AddressType)FLSLOADER_NUM_8;
          BurstOffSet  = (FlsLoader_AddressType)FLSLOADER_NUM_32;
          BytesToWrite = (Length - (NumberOfBursts * FLSLOADER_NUM_32));
          NoOfPages    = (BytesToWrite / FLSLOADER_NUM_8);
        }
      
        /* Block 5: Execute the command sequence . */
        /*Initiating the state to normal*/
        FlsLoader_Status = FLSLOADER_NORMAL; 
        /* Disable and restore interrupts for critical
        command cycles. As there are 2 critical command cycles, 
        the entire write operation is protected. */
        /* copy the Address to local variable*/
        Icr = FlsLoader_lDisableInts();
        
        /*Write in Burst Mode*/
        RetError = FlsLoader_lBurstWrite (BurstLoadPageCount,BurstOffSet,
                                          FlashType,SourceAddressPtr,
                                          TargetAddress,NumberOfBursts);
        
        if ((RetError == FLSLOADER_E_OK) && (NoOfPages != 0U))
        {
          /* Write Remaining Pages */
          TargetAddress += NumberOfBursts * 
                          (BurstLoadPageCount << FLSLOADER_NUM_3);
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
          user buffer efficiently*/
          SourceAddressPtr += NumberOfBursts * 
                          (BurstLoadPageCount << FLSLOADER_NUM_3);
          RetError = FlsLoader_lPageWrite (LoadPageCount,
                                           OffSet,FlashType,SourceAddressPtr,
                                           TargetAddress,NoOfPages);
        }
        FlsLoader_lEnableInts(Icr);
      }
    }
  }
  return (RetError);
}  /*End of function OCFLS_Write */
#define FLSLOADER_STOP_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives 
  is allowed only for MemMap.h*/
#include "MemMap.h"

#define FLSLOADER_START_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives 
  is allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1626]                    **
**            [/cover]                                                        **
** Syntax           :  FlsLoader_ReturnType FlsLoader_Erase(                  **
**                      FlsLoader_AddressType TargetAddress,                  **
**                      FlsLoader_LengthType Length                           **
**                    )                                                       **
**                                                                            **
** Service ID       : 0x03                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : Length - Number of sectors                              **
**                    TargetAddress - Starting Address of the sector          **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     :  FlsLoader_ReturnType(DET is not enabled)               **
**                     1.FLSLOADER_E_OK                                       **
**                     2.FLSLOADER_E_BUSY                                     **
**                     4.FLSLOADER_E_LOCKED                                   **
**                                                                            **
**                     1.FLSLOADER_E_NOT_OK for all errors but for busy.      **
**                       For busy state FLSLOADER_E_BUSY is returned          **
**                                                                            **
** Description      : This function erases the sector of the internal flash.  **
*******************************************************************************/
/* Service to Erase a sector of Flash */
FlsLoader_ReturnType FlsLoader_Erase(FlsLoader_AddressType TargetAddress,
                                     FlsLoader_LengthType Length )
{
  FlsLoader_ReturnType  RetError;
  FlsLoader_AddressType StartAddress;
  uint32                FlsFsrCheck;
  uint32                FlsFSRErase;
  volatile uint32       BusyCheck;
  uint32                InnerCount;
  uint32                Icr;
  uint32                FlashType;

  /*Init RetError to OK */
  /*RetError = (FlsLoader_ReturnType)FLSLOADER_E_OK;*/

  /*Block 1: check DET */
  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  RetError = FlsLoader_lEraseDetCheck(Length,TargetAddress);

  if (RetError == FLSLOADER_E_OK)
  #endif
  {
    #ifdef IFX_FLSLOADER_DEBUG
    FlsFsrCheck = ((FLASH0_FSR.U & ((uint32)FLSLOADER_FLASH_BUSY_MASK)) |
                      TestFlsloader_DebugMask01);
    #else  /*#ifdef IFX_FLSLOADER_DEBUG*/
    /* Check for Busy state of FLASH
    Registers differ between Checking of Busy state of PFlash and DFlash */
    FlsFsrCheck = (FLASH0_FSR.U & ((uint32)FLSLOADER_FLASH_BUSY_MASK));
    #endif  /*#ifdef IFX_FLSLOADER_DEBUG*/
    if ( FlsFsrCheck )
    {
      /* Report FLSLOADER_E_BUSY DET */
      #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
      Det_ReportError(
                     FLSLOADER_MODULE_ID,
                     FLSLOADER_INSTANCE_ID,
                     FLSLOADER_SID_ERASE,
              (uint8)FLSLOADER_E_BUSY);
      #endif
      RetError = FLSLOADER_E_BUSY;
    }
    else
    {
      /* Extract the flash type */
      FlashType = FlsLoader_lGetFlashType (TargetAddress);

      /* Given Address belongs to Data FLASH */
      /*Check if the Target Address passed is UCB protected */
      /* Check if global write protection is installed in DFLASH*/
      /*Check if the Target Address passed is UCB protected */
      #if (FLSLOADER_ENABLE_LOCKCHECK == STD_ON)
      /* Protection Check Algorithm */
      /* Preprocessor switch to enable/disable the lockcheck functionality */
      /* Check if global write protection is installed */
      RetError = FlsLoader_lEraseLockCheck (TargetAddress,FlashType,Length);

      if (RetError == FLSLOADER_E_OK)
      #endif
      {
        StartAddress = FLSLOADER_CMD_BASE;
        Icr = FlsLoader_lDisableInts();
        if (FlashType != FLSLOADER_DFLASH_BANK0)
        {
          FlsLoader_lResetSafetyENDINIT();
        }
        DSYNC();
        FlsLoader_Status = FLSLOADER_NORMAL; /* Initiating the state to normal*/
        #ifndef IFX_FLSLOADER_DEBUG
        /* Safety Reset Endinit is required for PFlash */
        /* Enter Critical Section */
        /* cycle 1 */
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
        /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA50(StartAddress))  = 
                                                                  TargetAddress;
        /* cycle 2 */
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
        /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA58(StartAddress))  = 
                                                                   Length;
        /* cycle 3 */
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
        /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(StartAddress))  = 
                                                             FLSLOADER_VALUE_80;
        /* cycle 4 */
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
        /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(StartAddress))  = 
                                                             FLSLOADER_VALUE_50;
        #endif
        /* Exit Critical Section */
        if (FlashType != FLSLOADER_DFLASH_BANK0)
        {
          FlsLoader_lSetSafetyENDINIT();
        }
        DSYNC();

        InnerCount = 0U;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlsFSRErase =  TestFlsloader_DebugMask06;
        #else
        FlsFSRErase = FLASH0_FSR.B.ERASE;
        #endif

        while((FlsFSRErase != FLSLOADER_BIT_SET) &&
              (InnerCount < FLSLOADER_CMDCYCLE_TIMEOUT))
        {
          InnerCount++;
          #ifdef  IFX_FLSLOADER_DEBUG
          FlsFSRErase =  TestFlsloader_DebugMask06;
          #else
          FlsFSRErase = FLASH0_FSR.B.ERASE;
          #endif   
        }
        
        if(InnerCount < FLSLOADER_CMDCYCLE_TIMEOUT)
        {
          /* Check for FLASH BUSY flag */
          /* PnBUSY and D0BUSY */
          InnerCount = 0U;
          #ifdef  IFX_FLSLOADER_DEBUG
          BusyCheck = TestFlsloader_DebugMask02;
          #else
          BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_FLASH_BUSY_MASK);
          #endif
          while (( BusyCheck != 0U) &&
                 ( InnerCount < FLSLOADER_BUSY_TIMEOUT))
          {
            InnerCount++;
            #ifdef  IFX_FLSLOADER_DEBUG
            BusyCheck = TestFlsloader_DebugMask02;
            #else
            BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_FLASH_BUSY_MASK);
            #endif
          }
          if (InnerCount < FLSLOADER_BUSY_TIMEOUT )
          {
            /* This check makes sure No protection error and no sequence error
            and No operation error has occurred.*/
            RetError = FlsLoader_lProtSeqOperErrorCheck();
            if(RetError == FLSLOADER_E_OK)
            {
              /* This check makes sure No erase verification error */
              RetError = FlsLoader_lEverCheck();
            }
          }
          else
          {
            RetError = FLSLOADER_E_NOT_OK;
          }
        }
        else
        {
          RetError = FLSLOADER_E_NOT_OK;
        }
        /* Clear the status if the operation is successful.*/
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
        /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
        address comparison/calculation purpose.*/
        *((FlsLoader_AddressType*)FlsLoader_lClearStatus(StartAddress))
                          = FLSLOADER_VALUE_FA;
      
        FlsLoader_lEnableInts(Icr);
      }
    }
  }
  return(RetError);
}  /* End of Function */
#define FLSLOADER_STOP_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#define FLSLOADER_START_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

  /* Preprocessor switch to enable/disable the FlsLoader_Lock API */
#if (FLSLOADER_LOCK_UNLOCK_API == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1627]                    **
**            [/cover]                                                        **
** Syntax           :  FlsLoader_ReturnType FlsLoader_Lock(                   **
**                              void                                          **
**                                                                            **
**                           )                                                **
**                                                                            **
** Service ID       : 0x04                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : FlsLoader_AddressType TargetAddress,                    **
**                    FlsLoader_LengthType Length                             **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     :  FlsLoader_ReturnType:(DET is not enabled)              **
**                     1.FLSLOADER_E_OK                                       **
**                     2.FLSLOADER_E_BUSY                                     **
**                     3.FLSLOADER_E_LOCKED                                   **
**                     4.FLSLOADER_E_NOT_OK for all errors but for busy.      **
**                       For busy state FLSLOADER_E_BUSY is returned          **
**                                                                            **
** Description      : This function is used to  Apply the sector protection to**
**                    the internal  Flash of the microcontroller.             **
**                    If any sector is previously locked FLSLOADER_E_LOCKED   **
**                    is returned.                                            **
**                    No valid parameters are expected here.                  **
**                    This API cannot be suspended                            **
**                    Service to Lock the internal flash of the microcontroller.
**                    Locking is dependent on the parameters given and not the**
**                    passsed parameters.                                     **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_Lock(void)
{
  uint32                FlashBusy;
  FlsLoader_ReturnType  RetError ;
  uint32                FlsFpro;
  uint32                FlsFproinp;
  uint32                FlsFproind;
  uint32                FlsFproinotp;

  RetError = FLSLOADER_E_OK ;

  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  if (FlsLoader_InitStatus != FLSLOADER_INITIALIZED)
  {
    /* Report missing initialization to DET */
    Det_ReportError(FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_LOCK,
                    FLSLOADER_E_UNINIT);
    RetError = FLSLOADER_E_NOT_OK;
  }
  #endif
  /* (FLSLOADER_DEV_ERROR_DETECT == STD_ON) */

  /* Check if PFLASH0/1 or DFLASH is busy */
  #ifdef IFX_FLSLOADER_DEBUG
  FlashBusy = TestFlsloader_DebugMask01;
  #else
  FlashBusy = (uint32)(FLASH0_FSR.U & FLSLOADER_FLASH_BUSY_MASK);
  #endif  /*#ifdef IFX_FLSLOADER_DEBUG*/
  if (FlashBusy != FLSLOADER_ZERO_DATA)
  {
    /* Report Flash Busy to DET */
    RetError = FLSLOADER_E_BUSY;
    #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(
                  FLSLOADER_MODULE_ID,
                  FLSLOADER_INSTANCE_ID,
                  FLSLOADER_SID_LOCK,
           (uint8)FLSLOADER_E_BUSY);
    #endif
    /*(FLSLOADER_DEV_ERROR_DETECT == STD_ON)*/
  }
    /* Protection should not be installed for the corresponding user level.*/
  if (RetError == FLSLOADER_E_OK)
  {
    /* Check if read/write/OTP/WOP is installled in PFLASH or DFLASH */
    FlsFpro = FLASH0_FPRO.B.PRODISP;
    #ifdef  IFX_FLSLOADER_DEBUG
    FlsFproinp    = TestFlsloader_DebugMask07;
    FlsFproind    = TestFlsloader_DebugMask07;
    FlsFproinotp  = TestFlsloader_DebugMask07;
    #else
    FlsFproinp = FLASH0_FPRO.B.PROINP;
    FlsFproind = FLASH0_FPRO.B.PROIND;
    FlsFproinotp = FLASH0_FPRO.B.PROINOTP;
    #endif
    if(FlsFpro != 1U)
    {
      if(FlsFproinp == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
      if(FlsFproind == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
    }
    if(FlsFproinotp == 1U)
    {
      RetError = FLSLOADER_E_LOCKED;
    }
  }

  if (RetError == FLSLOADER_E_OK)
    {
      RetError = FlsLoader_lLock();
    }
  return (RetError);
}  /* End of function FlsLoader_Lock */
#endif  /*(FLSLOADER_LOCK_UNLOCK_API == STD_ON) */


#if (FLSLOADER_LOCK_UNLOCK_API == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1629]                    **
**            [/cover]                                                        **
** Syntax           :  FlsLoader_ReturnType FlsLoader_UnLock(                 **
**                       FlsLoader_AddressType TargetAddress,                 **
**                       FlsLoader_ValueType* Password                        **
**                                                                            **
**                    )                                                       **
**                                                                            **
** Service ID       : 0x05                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   :                                                         **
**                    Password: pointer to the 4 double word(256 bit)         **
**                               Password for UCB protection                  **
**                    TargetAddress: Target address of user config block.     **
**                    Possible values are 0xAF100000, 0xAF100400.             **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType:(DET is not enabled)               **
**                    1.FLSLOADER_E_OK                                        **
**                    2.FLSLOADER_E_BUSY                                      **
**                    3.FLSLOADER_E_NOT_OK                                    **
**                                                                            **
** Description      : This function is used to  unlock(protect) the protection
                      in                                PFLASH and DFLASH     **
                      protection.                                             **
**                    A wrong password will result in Protection Error        **
**                    This API needs to be called twice if the user needs to  **
**                    unlock both PFLASH and DFLASH protection                **
**                    This API cannot be suspended                            **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_UnLock(
                                    FlsLoader_AddressType TargetAddress,
                                    FlsLoader_ValueType* Password
                                    )
{
  FlsLoader_ReturnType   RetError ;
  FlsLoader_AddressType  StartAddress;
  uint32                 FlsErrCheck;
  volatile uint32        FlsBusyStat;
  volatile uint32        FlsFpro;
  volatile uint32        FlsFprodisd;
  volatile uint8         UserConfig;
  volatile uint8         Index;
  volatile FlsLoader_ValueType* Passwordptr;

  /* Init RetError to OK */
  RetError = FLSLOADER_E_OK ;
  Passwordptr = Password;

  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  if (FlsLoader_InitStatus != FLSLOADER_INITIALIZED)
  {
    /*  Report missing initialization to DET */
    Det_ReportError(FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_UNLOCK,
                    FLSLOADER_E_UNINIT);
    RetError = FLSLOADER_E_NOT_OK;
  }
  else
  {
    /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    if (TargetAddress != FLSLOADER_UCB_PFLASH)
    {
    /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
      if (TargetAddress != FLSLOADER_UCB_DFLASH)
      {
        Det_ReportError(
                   FLSLOADER_MODULE_ID,
                   FLSLOADER_INSTANCE_ID,
                   FLSLOADER_SID_UNLOCK,
                   FLSLOADER_E_PARAM_ADDRESS);
        RetError = FLSLOADER_E_NOT_OK;
      }
    }
  }
  #endif
  /* #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON) */

  /* PROTECTION in PFLASH  */
  /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  if ( TargetAddress == FLSLOADER_UCB_PFLASH )
  {
    /* PROTECTION in PFLASH  */
    UserConfig = FLSLOADER_UCB0;
  }
  else
  {
    /* PROTECTION in DFLASH  */
    UserConfig = FLSLOADER_UCB1;
  }
  /* Start address for the command cycle */
  StartAddress = FLSLOADER_DFLASH0_START_ADDRESS;
  #ifdef IFX_FLSLOADER_DEBUG
  FlsBusyStat = TestFlsloader_DebugMask01;
  #else
  FlsBusyStat = (uint32)(FLASH0_FSR.U & (uint32)FLSLOADER_FLASH_BUSY_MASK);
  #endif  /*#ifdef IFX_FLSLOADER_DEBUG*/

  if (FlsBusyStat != 0U)
  {
    /* Report Pflash busy to DET */
    #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(
                    FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_UNLOCK,
             (uint8)FLSLOADER_E_BUSY);
    #endif
    RetError = FLSLOADER_E_BUSY;
  }
  if (RetError == FLSLOADER_E_OK)
  {
    /* cycle 1 */
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
    address comparison/calculation purpose.*/
    /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
    address comparison/calculation purpose.*/
    *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_553C(StartAddress))  = 
                                                              UserConfig;
    /* Cycle 2 to Cycle 9 : Passwords for disabling protection */
    for(Index =(uint8)0U; Index < FLSLOADER_PWD; Index++)
    {
      /* provide 8 passwords */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
      volatile uint32 to avoid compiler optimization in command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
      address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
      address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
      access the Address and is within allowed range*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_553C(StartAddress))  = 
                                                         Passwordptr[Index];
    }
    if (UserConfig == FLSLOADER_UCB0)
    {
      /*Check if there are no protection and sequence errors*/
      FlsErrCheck = FlsLoader_lProtSeqOperErrorCheck();
      /* Check if PFlash Protection is Disabled*/
      FlsFpro     = FLASH0_FPRO.B.PRODISP;
      /* return if protection is not disabled or if there are errors */
      if ((FlsFpro == 0U) || (FlsErrCheck == FLSLOADER_E_NOT_OK))
      {
      /* return if protection is not disabled or if there are errors */
        RetError = FLSLOADER_E_NOT_OK;
      }
    }
    else
    {
      /*Check if there are no protection and sequence errors*/
      FlsErrCheck = FlsLoader_lProtSeqOperErrorCheck();
      /* Check if DFlash Protection is Disabled*/
      FlsFprodisd = FLASH0_FPRO.B.PRODISD;
      /* return if protection is not disabled or if there are errors */
      if ((FlsFprodisd == 0U) || (FlsErrCheck == FLSLOADER_E_NOT_OK))
      {
        /* return if protection is not disabled or if there are errors */
        RetError = FLSLOADER_E_NOT_OK;
      }
    }

    if (RetError == (FlsLoader_ReturnType)FLSLOADER_E_OK)
    {
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
      volatile uint32 to avoid compiler optimization in command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32 for 
      address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 for 
      address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FlsLoader_lClearStatus(StartAddress)) = 
                                                      FLSLOADER_VALUE_FA;
    }
  }
  return (RetError) ;
}
#endif  /* if (FLSLOADER_LOCK_UNLOCK_API == STD_ON)    */

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1628]                    **
**            [/cover]                                                        **
** Syntax           :  FlsLoader_ReturnType FlsLoader_Suspend(                **
**                      void                                                  **
**                    )                                                       **
**                                                                            **
** Service ID       : 0x06                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : void                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : FLSLOADER_E_OK - API excecuted succesffully             **
**                    FLSLOADER_E_NOT_OK - DET errors occured                 **
**                                                                            **
**                                                                            **
** Description      : This function is used to  suspend flash write or        **
                      erase operation.                                        **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_Suspend (void)
{
  FlsLoader_ReturnType RetError ;
  RetError = FLSLOADER_E_OK;

  #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  if (FlsLoader_InitStatus != FLSLOADER_INITIALIZED)
  {
    /* Report missing initialization to DET */
    Det_ReportError(
                    FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_SUSPEND,
                    FLSLOADER_E_UNINIT);
    
    RetError = FLSLOADER_E_NOT_OK;
  }
  #endif

  FlsLoader_Status = FLSLOADER_SUSPEND;
  return(RetError);
}  /*End of suspend function */

#define FLSLOADER_STOP_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
  /* Inline Functions */

#define FLSLOADER_START_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLSLOADER_PR1629]                    **
**            [/cover]                                                        **
** Syntax       :IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lPverCheck  (**
**                                                                            **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   :                                                         **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : value: RetError                                         **
**                                                                            **
** Description      : This function checks for Program Verify Error           **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lPverCheck(void)
{
  volatile uint32 FlsPeverErr ;
  FlsLoader_ReturnType  RetError;

  RetError = FLSLOADER_E_OK;

  #ifdef  IFX_FLSLOADER_DEBUG
  FlsPeverErr = (FLASH0_FSR.U & FLSLOADER_PVER_ERROR)||
                 TestFlsloader_DebugMask06;
  #else
  FlsPeverErr = (uint32)(FLASH0_FSR.U & FLSLOADER_PVER_ERROR);
  #endif
  
  if (FlsPeverErr)
  {
    RetError = FLSLOADER_E_NOT_OK;
  }
  return (RetError);
}
/*******************************************************************************
** Syntax          :IFX_LOCAL_INLINE FlsLoader_ReturnType                     **
**                                  FlsLoader_lEverCheck(void)                **
**                                                                            **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   :                                                         **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : value: RetError                                         **
**                                                                            **
** Description      : This function checks for Erase Verify Error             **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lEverCheck(void)
{
  volatile uint32 FlsEverError;
  FlsLoader_ReturnType  RetError;

  RetError = FLSLOADER_E_OK;
  #ifdef  IFX_FLSLOADER_DEBUG
  FlsEverError = TestFlsloader_DebugMask09;
  #else
  FlsEverError = (uint32)(FLASH0_FSR.U & FLSLOADER_EVER_ERROR);
  #endif
  
  if (FlsEverError)
  {
    RetError = FLSLOADER_E_NOT_OK;
  }
  return (RetError);
}
/*******************************************************************************
** Syntax           : uint32 FlsLoader_lDisableInts                           **
**                    (                                                       **
**                      void                                                  **
**                    )                                                       **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : reentrant                                               **
**                                                                            **
** Parameters (in)  : none                                                    **
**                                                                            **
** Parameters (out) :                                                         **
** Return value     : uint32                                                  **
**                                                                            **
** Description      : This function disables the global interrupt flag.       **
**                                                                            **
*******************************************************************************/
uint32 FlsLoader_lDisableInts(void)
{
  uint32 Icr;
  /* read interrupt control register */
  ISYNC(); /* execute isync */
  Icr = (uint32)MFCR(CPU_ICR);
  DISABLE();
  return (Icr);

}
/*******************************************************************************
** Syntax           : void FlsLoader_lEnableInts                              **
**                    (                                                       **
**                      uint32 Icr                                            **
**                    )                                                       **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : reentrant                                               **
**                                                                            **
** Parameters (in)  : none                                                    **
**                                                                            **
** Parameters (out) :                                                         **
** Return value     : uint32                                                  **
**                                                                            **
** Description      : This function restores the global interrupt flag.       **
**                                                                            **
*******************************************************************************/
void FlsLoader_lEnableInts(uint32 Icr)
{
  #ifdef IFX_FLSLOADER_DEBUG
  Icr = TestFlsloader_DebugMask05;
  #endif/*#ifdef IFX_FLSLOADER_DEBUG*/
  if ((uint32)Icr & ((uint32)1U << (uint32)FLSLOADER_SHIFT_BY_15))
  {
  /* interrupts were enabled, enable again */
    ENABLE();
  }

}
/*******************************************************************************
** Syntax           : void FlsLoader_lResetSafetyENDINIT                      **
**                    (                                                       **
**                      void                                                  **
**                    )                                                       **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : reentrant                                               **
**                                                                            **
** Parameters (in)  : none                                                    **
**                                                                            **
** Parameters (out) :                                                         **
** Return value     : void                                                    **
**                                                                            **
** Description      : This function resets the safety end init flag.          **
** When FLsLoader driver is executing write or erase, it is assumed that      **
** there are no other tasks running in parallel or from another core that     **
** requires the service of safety WDG endinit function.                       **
** Password sequencing will not be enabled and not checked within this        **
** function, also Time based check is not enabled                             **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void FlsLoader_lResetSafetyENDINIT(void)
{
  uint32 NewValCon0;
  uint32 NewPw;                   /* next password to be used */
  uint32 NewRel;                  /* next reload value to be used */
 
  uint32 Result;
  
  /* Safety Wdg register is acquired */
  volatile Ifx_SCU_WDTS_CON0* ScuWdtcon0Ptr 
                              = &SCU_WDTS_CON0;
  /* save old value */
  NewValCon0  = SCU_WDTS_CON0.U;
  /* get valid next password */

  Result = NewValCon0 & FLSLOADER_WDT_PASSWORD_MASK;
  Result = ( Result & FLSLOADER_WDT_PASSWORD_UPPER_MASK) |
             ((~(Result & FLSLOADER_WDT_PASSWORD_LOWER_MASK)) 
             & FLSLOADER_WDT_PASSWORD_LOWER_MASK );

  /* password sequencing will not be enabled at this time,
  no need for the run time calculation of password */
  NewPw = Result;
  /* get valid next timer reload */
  Result = NewValCon0 & FLSLOADER_WDT_RELOAD_MASK;
  /* Time control enable shall not be enabled by the system */
  NewRel= Result;
  /* build required password WDTCON0 value */
  NewValCon0 =  ( (NewRel) |              /* new reload value  */
                  (NewPw)  |              /* new Password */
                  (1u) );                 /* Clear LCK, keep ENDINIT set */

  ScuWdtcon0Ptr->U  = NewValCon0;        /* password access to unlock */

  NewValCon0 &= ~ FLSLOADER_WDT_STATUS_MASK;/*clear ENDINIT and LCK Bit */
  NewValCon0 |= FLSLOADER_NUM_2;                    /* set LCK Bit */

  ScuWdtcon0Ptr->U = NewValCon0;  /* write access and lock for Timeout mode */
}

/*******************************************************************************
** Syntax           : void FlsLoader_lSetSafetyENDINIT                        **
**                    (                                                       **
**                      void                                                  **
**                    )                                                       **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : reentrant                                               **
**                                                                            **
** Parameters (in)  : none                                                    **
**                                                                            **
** Parameters (out) :                                                         **
** Return value     : void                                                    **
**                                                                            **
** Description      : This function sets  the safety end init flag.           **
** When FLsLoader driver is executing write or erase, it is assumed that      **
** there are no other tasks running in parallel or from another core that     **
** requires the service of safety WDG endinit function.                       **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void FlsLoader_lSetSafetyENDINIT(void)
{
  uint32 NewValCon0;
  uint32 NewPw;                   /* next password to be used */
  uint32 NewRel;                  /* next timer reload to be used */
  uint32 Result;

  volatile Ifx_SCU_WDTS_CON0* ScuWdtcon0Ptr 
                                  = &SCU_WDTS_CON0;
  /* save old value */
  NewValCon0  =SCU_WDTS_CON0.U;

  Result = NewValCon0 & FLSLOADER_WDT_PASSWORD_MASK;
  Result = ( Result & FLSLOADER_WDT_PASSWORD_UPPER_MASK) |
             ((~(Result & FLSLOADER_WDT_PASSWORD_LOWER_MASK)) 
             & FLSLOADER_WDT_PASSWORD_LOWER_MASK );
  NewPw = Result;

  Result = NewValCon0 & FLSLOADER_WDT_RELOAD_MASK;
  NewRel= Result;
  /* build required password WDTCON0 value */
  NewValCon0  =  ( (NewRel) |             /* new reload value  */
                  (NewPw)   |             /* new Password */
                  (1u) );                 /* Clear LCK, keep ENDINIT set */
  SCU_WDTS_CON0.U   = NewValCon0;         /*  unlock with password access */
  /* clear timer reload and restore saved and corrected reload value */
  NewValCon0 &= ~FLSLOADER_WDT_STATUS_MASK;/* clear ENDINIT and LCK Bit*/
  NewValCon0 |= FLSLOADER_NUM_3;         /* set LCK and ENDINIT Bit */
  ScuWdtcon0Ptr->U = NewValCon0;         /* write modify access and lock */

}


/*******************************************************************************
** Syntax           :  void FlsLoader_lBurstWrite (uint32 BurstLoadPageCount, **
**                          FlsLoader_AddressType BurstOffSet,                **
**                          uint32 FlashType,                                 **
**                          const uint8 *SourceAddressPtr,                    **
**                          FlsLoader_AddressType TargetAddress,              **
**                          uint32 NumberOfBursts);                           **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : reentrant                                               **
**                                                                            **
** Parameters (in)  : BurstLoadPageCount,                                     **
**                    BurstOffSet,                                            **
**                    FlashType,                                              **
**                    PdataPtr,                                               **
**                    TargetAddress                                           **
**                                                                            **
** Parameters (out) :                                                         **
**                                                                            **
** Return value     : FLSLOADER_E_OK                                          **
**                    FLSLOADER_E_NOT_OK                                      **
**                                                                            **
** Description      :                                                         **
**                                                                            **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lBurstWrite (
                            uint32 BurstLoadPageCount,
                            FlsLoader_AddressType BurstOffSet,
                            uint32 FlashType,
                            const uint8 *SourceAddressPtr,
                            FlsLoader_AddressType TargetAddress,
                            uint32 NumberOfBursts)
{
  volatile uint32                FlashBusyStat;
  volatile uint32                BusyCheck;
  FlsLoader_AddressType         *PdataPtr;
  FlsLoader_AddressType         *LoadPageAddress;
  FlsLoader_ReturnType           RetError;
  FlsLoader_AddressType          StartAddress;
  uint32                         InCount;
  uint32                         Count;
  

  StartAddress = FLSLOADER_CMD_BASE;
  RetError = FLSLOADER_E_OK;
  /*IFX_MISRA_RULE_11_05_STATUS="PdataPtr" attempt to cast away
  volatile in terms of pointer access*/
  PdataPtr = (FlsLoader_AddressType *)(void *)(SourceAddressPtr);
 
  for (Count = 0U; 
      (Count < NumberOfBursts) && (RetError == FLSLOADER_E_OK); 
       Count++)
  {
    if (FlashType != FLSLOADER_DFLASH_BANK0)
    {
      FlsLoader_lResetSafetyENDINIT();
    }
    /* Enter the Page Mode */
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is 
    defined of type volatile uint32 to avoid compiler 
    optimization in command cycles*/
    /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast 
    to uint32 for address comparison/calculation purpose.*/
    /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast 
    to uint32 for address comparison/calculation purpose.*/
    *((FlsLoader_AddressType*)FlsLoader_lEnterPageMode(
                              StartAddress)) =
                              FlsLoader_lGetMode(FlashType);
    DSYNC();
    InCount = 0U;
    /* Check if Program Flash is in page mode */
    if (FlashType != FLSLOADER_DFLASH_BANK0)
    {
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat = TestFlsloader_DebugMask04;
      #else
      FlashBusyStat = FLASH0_FSR.B.PFPAGE;
      #endif
      while((FlashBusyStat != FLSLOADER_BIT_SET) &&
           (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
      {
        InCount++;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlashBusyStat = TestFlsloader_DebugMask04;
        #else
        FlashBusyStat = FLASH0_FSR.B.PFPAGE;
        #endif
      }
    }
    else
    {
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat = TestFlsloader_DebugMask04;
      #else
      FlashBusyStat = FLASH0_FSR.B.DFPAGE;
      #endif
      while ((FlashBusyStat != FLSLOADER_BIT_SET) &&
             (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
      {
        InCount++;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlashBusyStat = TestFlsloader_DebugMask04;
        #else
        FlashBusyStat = FLASH0_FSR.B.DFPAGE;
        #endif
      }
    }

    /* Check PFPAGE/DFPAGE bit was SET */
    if(InCount < FLSLOADER_CMDCYCLE_TIMEOUT)
    {
      for (InCount=0U; InCount < BurstLoadPageCount; InCount++)
      {
        /* Load the data */
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is 
        defined of type volatile uint32 to avoid compiler optimization 
        in command cycles*/
        /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to
        uint32 for address comparison/calculation purpose.*/
        /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
        uint32 for address comparison/calculation purpose.*/
        LoadPageAddress = 
              (FlsLoader_AddressType *)FlsLoader_lLoadPage(StartAddress);
        *LoadPageAddress = *PdataPtr;
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
        *(LoadPageAddress + 1U) = *(PdataPtr + 1U);
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
        PdataPtr += FLSLOADER_NUM_2;
        /* Load Page register gets incremented automatically. */
      }
      /* cycle 1 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA50(
      StartAddress)) = TargetAddress;
      /* cycle 2 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA58(
      StartAddress))= FLSLOADER_VALUE_00;
      /* cycle 3 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(
      StartAddress)) = FLSLOADER_VALUE_A0;
      /*cycle 4 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(
      StartAddress)) = FLSLOADER_VALUE_7A;
      DSYNC();
      /* Wait till the PROG bit is set */
      InCount = 0U;
      
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat =  TestFlsloader_DebugMask06;
      #else
      FlashBusyStat = FLASH0_FSR.B.PROG;
      #endif

      while ((FlashBusyStat != FLSLOADER_BIT_SET) &&
             (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
      {
        InCount++;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlashBusyStat =  TestFlsloader_DebugMask06;
        #else
        FlashBusyStat = FLASH0_FSR.B.PROG;
        #endif
      }
      
      if (InCount < FLSLOADER_CMDCYCLE_TIMEOUT)
      {
        /* Check for FLASH BUSY flag */
        /* PnBUSY and D0BUSY */
        InCount = 0U;
        #ifdef  IFX_FLSLOADER_DEBUG
        BusyCheck = TestFlsloader_DebugMask02;
        #else
        BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_FLASH_BUSY_MASK);
        #endif
        while ((BusyCheck != 0U) &&
              (InCount < FLSLOADER_BUSY_TIMEOUT ))
        {
          InCount++;
          #ifdef  IFX_FLSLOADER_DEBUG
          BusyCheck = TestFlsloader_DebugMask02;
          #else
          BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_FLASH_BUSY_MASK);
          #endif
        }
        /* This check makes sure No protection error and no sequence 
        error and No operation error has occurred.*/
        if (InCount < FLSLOADER_BUSY_TIMEOUT )
        {
          RetError = FlsLoader_lProtSeqOperErrorCheck();
          if(RetError == FLSLOADER_E_OK)
          {
            /* This check makes sure No program verification error */
            RetError = (FlsLoader_ReturnType)FlsLoader_lPverCheck();
          }
        }
        else
        {
          RetError = FLSLOADER_E_NOT_OK;
        }
      }
      else
      {
        RetError = FLSLOADER_E_NOT_OK;
      }
      /* Clear the status if the operation is successful.*/
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FlsLoader_lClearStatus(StartAddress))
                                  = FLSLOADER_VALUE_FA;

      /* Iterate the TargetAddress to the next page Address */
      /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of type 
      volatile uint32 to avoid compiler optimization in command
      cycles*/
      TargetAddress = TargetAddress + BurstOffSet;
    }
    else
    {
      RetError = FLSLOADER_E_NOT_OK;
    }
    if ((FlashType != FLSLOADER_DFLASH_BANK0))
    {
      FlsLoader_lSetSafetyENDINIT();
    }
  }
  return RetError;
}

/*******************************************************************************
**FlsLoader_ReturnType FlsLoader_lPageWrite (uint32 LoadPageCount,            **
**                           FlsLoader_AddressType OffSet,                    **
**                           uint32 FlashType,                                **
**                           const uint8 *SourceAddressPtr,                   **
**                           FlsLoader_AddressType TargetAddress,             **
**                           uint32 NoOfPages);                               **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : reentrant                                               **
**                                                                            **
** Parameters (in)  :  LoadPageCount,                                         **
**                     OffSet,                                                **
**                     FlashType,                                             **
**                     SourceAddressPtr,                                      **
**                     TargetAddress                                          **
**                                                                            **
** Parameters (out) :                                                         **
**                                                                            **
** Return value     : FLSLOADER_E_OK                                          **
**                    FLSLOADER_E_NOT_OK                                      **
**                                                                            **
** Description      :                                                         **
**                                                                            **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lPageWrite (
                           uint32 LoadPageCount,
                           FlsLoader_AddressType OffSet,
                           uint32 FlashType,
                           const uint8 *SourceAddressPtr,
                           FlsLoader_AddressType TargetAddress,
                           uint32 NoOfPages)
{
  volatile uint32                FlashBusyStat;
  volatile uint32                BusyCheck;
  uint32                         InCount;
  uint32                         Count;
  FlsLoader_ReturnType           RetError;
  FlsLoader_AddressType          StartAddress;
  FlsLoader_AddressType         *PdataPtr;
  FlsLoader_AddressType         *LoadPageAddress;
  
  
  StartAddress = FLSLOADER_CMD_BASE;
  RetError = FLSLOADER_E_OK;
  /*IFX_MISRA_RULE_11_05_STATUS="PdataPtr" attempt to cast away
  volatile in terms of pointer access*/
  PdataPtr = (FlsLoader_AddressType *)(void *)(SourceAddressPtr);

  for (Count = 0U; (Count < NoOfPages) && (RetError == FLSLOADER_E_OK); Count++)
  {
    if (FlashType != FLSLOADER_DFLASH_BANK0)
    {
      FlsLoader_lResetSafetyENDINIT();
    }
    /* Enter the Page Mode */
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
    type volatile uint32 to avoid compiler optimization in 
    command cycles*/
    /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to uint32
    for address comparison/calculation purpose.*/
    /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to uint32 
    for address comparison/calculation purpose.*/
    *((FlsLoader_AddressType*)FlsLoader_lEnterPageMode(StartAddress)) =
                                       FlsLoader_lGetMode(FlashType);
    DSYNC();
    InCount = 0U;
    /*Check if Program Flash is in page mode */
    if (FlashType != FLSLOADER_DFLASH_BANK0)
    {
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat = TestFlsloader_DebugMask04;
      #else
      FlashBusyStat = FLASH0_FSR.B.PFPAGE;
      #endif
      while((FlashBusyStat != FLSLOADER_BIT_SET) &&
            (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
      {
        InCount++;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlashBusyStat = TestFlsloader_DebugMask04;
        #else
        FlashBusyStat = FLASH0_FSR.B.PFPAGE;
        #endif
      }
    }
    else
    {
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat = TestFlsloader_DebugMask04;
      #else
      FlashBusyStat = FLASH0_FSR.B.DFPAGE;
      #endif
      while((FlashBusyStat != FLSLOADER_BIT_SET) &&
           (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
      {
        InCount++;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlashBusyStat = TestFlsloader_DebugMask04;
        #else
        FlashBusyStat = FLASH0_FSR.B.DFPAGE;
        #endif
      }
    }

    /* Check PFPAGE/DFPAGE bit was SET */
    if(InCount < FLSLOADER_CMDCYCLE_TIMEOUT)
    {
      for (InCount=0U; InCount < LoadPageCount; InCount++)
      {
        /* Load the data */
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is 
        defined of type volatile uint32 to avoid compiler 
        optimization in command cycles*/
        /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
        uint32 for address comparison/calculation purpose.*/
        /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
        uint32 for address comparison/calculation purpose.*/
        LoadPageAddress = 
                (FlsLoader_AddressType *)FlsLoader_lLoadPage(StartAddress);

        *LoadPageAddress = *PdataPtr;
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
        *(LoadPageAddress + 1U) = *(PdataPtr + 1U);
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic used to load 
        data into address and is within allowed range*/
        PdataPtr += FLSLOADER_NUM_2;
        /* Load Page register gets incremented automatically. */
      }
      /* cycle 1 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is 
      defined of type volatile uint32 to avoid compiler 
      optimization in command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA50(
                                     StartAddress)) = TargetAddress;
      /* cycle 2 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA58(
                               StartAddress))=  FLSLOADER_VALUE_00;
      /* cycle 3 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(
                               StartAddress))=  FLSLOADER_VALUE_A0;
      /*cycle 4 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined 
      of type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(
                                StartAddress))= FLSLOADER_VALUE_AA;
      DSYNC();
      /* Wait till the PROG bit is set */
      InCount = 0U;
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat =  TestFlsloader_DebugMask06;
      #else
      FlashBusyStat = FLASH0_FSR.B.PROG;
      #endif
      
      while((FlashBusyStat != FLSLOADER_BIT_SET) &&
            (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
      {
        InCount++;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlashBusyStat =  TestFlsloader_DebugMask06;
        #else
        FlashBusyStat = FLASH0_FSR.B.PROG;
        #endif
      }
      if (InCount < FLSLOADER_CMDCYCLE_TIMEOUT)
      {
        /* Check for FLASH BUSY flag */
        /* PnBUSY and D0BUSY */
        InCount = 0U;
        #ifdef  IFX_FLSLOADER_DEBUG
        BusyCheck = TestFlsloader_DebugMask02 ;
        #else
        BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_FLASH_BUSY_MASK);
        #endif
        while((BusyCheck != 0U) && 
              (InCount < FLSLOADER_BUSY_TIMEOUT))
        {
          InCount++;
          #ifdef  IFX_FLSLOADER_DEBUG
          BusyCheck = TestFlsloader_DebugMask02;
          #else
          BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_FLASH_BUSY_MASK);
          #endif
        }
        if (InCount < FLSLOADER_BUSY_TIMEOUT )
        {
          /* This check makes sure No protection error and no 
          sequence error and No operation error has occurred.*/
          RetError = FlsLoader_lProtSeqOperErrorCheck();
          if(RetError == (FlsLoader_ReturnType)FLSLOADER_E_OK)
          {
            /* This check makes sure No program verification error */
            RetError = (FlsLoader_ReturnType)FlsLoader_lPverCheck();
          }
        }
        else
        {
          RetError = FLSLOADER_E_NOT_OK;
        }
      }
      else
      {
        RetError = FLSLOADER_E_NOT_OK;
      }
      /* Clear the status if the operation is successful.*/
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is 
      defined of type volatile uint32 to avoid compiler 
      optimization in command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS= The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FlsLoader_lClearStatus(
                                StartAddress))= FLSLOADER_VALUE_FA;

      /*Iterate the TargetAddress to the next page Address */
      /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of 
      type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      TargetAddress = TargetAddress + OffSet;
    }
    else
    {
      RetError = FLSLOADER_E_NOT_OK;
    }
    if ((FlashType != FLSLOADER_DFLASH_BANK0))
    {
      FlsLoader_lSetSafetyENDINIT();
    }
  }
  return RetError;
}
#define FLSLOADER_STOP_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives 
  is allowed only for MemMap.h*/
#include "MemMap.h"

