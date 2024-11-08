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
**                                                                           **
**  $FILENAME   : FlsLoader_Platform.c $                                     **
**                                                                           **
**  $CC VERSION : \main\dev_tc23x\23 $                                       **
**                                                                           **
**  $DATE       : 2016-09-29 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains AURIX derivative (platform specific)    **
**                functionality of FlsLoader driver.                         **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                                                                            **
    TRACEABILITY : [cover parentID=] [/cover]
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of Tasking sfr file */
#include "IfxFlash_reg.h"
#include "IfxSrc_reg.h"
#include "FlsLoader_Local.h"
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define FLSLOADER_EEPROM_OFFSETEND      (FLSLOADER_DFLASH_BANK_SIZE - 1U)
#define FLSLOADER_DF_EEPROM_SEGMENT     (0xAF0U)
#define FLSLOADER_DF_UCB_SEGMENT        (0xAF1U)
#define FLSLOADER_EEPROM_SECTOR_SIZE    (0x1FFFU)
#define FLSLOADER_UCB_SECTOR_SIZE_1     (0x3FFU)
#define FLSLOADER_MOD_8                 (0x7U)
#define FLSLOADER_NUM_8                 (0x8U)
#define FLSLOADER_UCB_DATA_SIZE         (0x80U)
#define FLSLOADER_UCB_SECTOR_SIZE       (0x400U)
#define FLSLOADER_UCBSIZE               (0x4000U)
#define FLSLOADER_NUM_OF_UCB_SECTORS    (16U)
#define FLSLOADER_UCB_OFFSETEND         (0x0003FFFU)

#define FLSLOADER_NUM_UCB_SECTORS       (1U)
#define FLSLOADER_NUM_UCB_PAGES         (16U)
#define FLSLOADER_PAGE_SIZE             (8U)
#define FLSLOADER_SHIFT_BY_12           (12U)
#define FLSLOADER_ADDRESS_MASK          ((FlsLoader_AddressType)(0x000FFFFFU))
#define FLSLOADER_DFLASH_SECTOR_SIZE    (0x2000U)

#define FLSLOADER_MAX_SECTORS           ((uint32) 27U)

#define FLASH_HW_MODULE ((volatile Ifx_FLASH *)(void *) &(MODULE_FLASH0))
#define FLASH0_PROCONP ((volatile Ifx_FLASH_PROCONP*)(void *)(&FLASH0_PROCONP0))
#define FLASH0_PROCONOTP \
         ((volatile Ifx_FLASH_PROCONOTP*)(void *)(&FLASH0_PROCONOTP0))
#define FLASH0_PROCONWOP \
         ((volatile Ifx_FLASH_PROCONWOP*)(void *)(&FLASH0_PROCONWOP0))

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/
#define FLSLOADER_START_SEC_CONST_32BIT
#include "MemMap.h"

/* Array of PFlash sector start address offsets. 
   Size of this array has to be the number of sectors in PFx which has 
   maximum number of sectors */
/*IFX_MISRA_RULE_08_07_STATUS=FlsLoader_PFlashSectorOffset is a constant 
  array hence not declared inside the function*/
static const uint32 FlsLoader_PFlashSectorOffset[FLSLOADER_MAX_SECTORS]=
{
  FLSLOADER_PF_S0,
  FLSLOADER_PF_S1,
  FLSLOADER_PF_S2,
  FLSLOADER_PF_S3,
  FLSLOADER_PF_S4,
  FLSLOADER_PF_S5,
  FLSLOADER_PF_S6,
  FLSLOADER_PF_S7,
  FLSLOADER_PF_S8,
  FLSLOADER_PF_S9,
  FLSLOADER_PF_S10,
  FLSLOADER_PF_S11,
  FLSLOADER_PF_S12,
  FLSLOADER_PF_S13,
  FLSLOADER_PF_S14,
  FLSLOADER_PF_S15,
  FLSLOADER_PF_S16,
  FLSLOADER_PF_S17,
  FLSLOADER_PF_S18,
  FLSLOADER_PF_S19,
  FLSLOADER_PF_S20,
  FLSLOADER_PF_S21,
  FLSLOADER_PF_S22,
  FLSLOADER_PF_S23,
  FLSLOADER_PF_S24,
  FLSLOADER_PF_S25,
  FLSLOADER_PF_S26
};

/* UCB_PFLASH Content */
#if ((FLSLOADER_PF0_PROT == WRITE_PROTECTION)||\
     (FLSLOADER_PF0_PROT == READ_PROTECTION))
/* UCB_PFLASH
  page 0 -bytes 0-3: FLASH0_PROCONP0
          bytes 4-7: FLASH0_PROCONP1
  page 1 -bytes 8-11 : all zeroes
          bytes 12-15 : all zeroes
  page 2 -bytes 16-19: copy of FLASH0_PROCONP0
          bytes 20-23: copy of FLASH0_PROCONP1
  page 3 -bytes 24-27 : all zeroes
          bytes 28-31 : all zeroes
  page 4 -bytes 32-39: PW0
  page 5 -bytes 40-47: PW1
  page 6 -bytes 48-55: PW2
  page 7 -bytes 56-63: PW3
  page 8 -bytes 64-71: PW0
  page 9  -bytes 72-79: PW1
  page 10 -bytes 80-87: PW2
  page 11 -bytes 88-95: PW3
  page 12 -bytes 96-103: all zeroes
  page 13 -bytes 104-111: all zeroes
  page 14 -bytes 112-115: confirmation code
           bytes 116-119: zeroes
  page 15 -bytes 120-123: confirmation code
           bytes 124-127: zeroes
*/

  /*IFX_MISRA_RULE_08_07_STATUS=FlsLoader_UcbPFlashPage0 is a constant array 
  hence not declare in the calling function*/
static const uint8 FlsLoader_UcbPFlashPage0[FLSLOADER_UCB_SIZE] =
{
FLSLOADER_PROCONP0_BYTE0,FLSLOADER_PROCONP0_BYTE1,
FLSLOADER_PROCONP0_BYTE2,FLSLOADER_PROCONP0_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
FLSLOADER_PROCONP0_BYTE0,FLSLOADER_PROCONP0_BYTE1,
FLSLOADER_PROCONP0_BYTE2,FLSLOADER_PROCONP0_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
FLSLOADER_P0_PW00_B0,FLSLOADER_P0_PW00_B1,
FLSLOADER_P0_PW00_B2,FLSLOADER_P0_PW00_B3,
FLSLOADER_P0_PW01_B0,FLSLOADER_P0_PW01_B1,
FLSLOADER_P0_PW01_B2,FLSLOADER_P0_PW01_B3,
FLSLOADER_P0_PW10_B0,FLSLOADER_P0_PW10_B1,
FLSLOADER_P0_PW10_B2,FLSLOADER_P0_PW10_B3,
FLSLOADER_P0_PW11_B0,FLSLOADER_P0_PW11_B1,
FLSLOADER_P0_PW11_B2,FLSLOADER_P0_PW11_B3,
FLSLOADER_P0_PW20_B0,FLSLOADER_P0_PW20_B1,
FLSLOADER_P0_PW20_B2,FLSLOADER_P0_PW20_B3,
FLSLOADER_P0_PW21_B0,FLSLOADER_P0_PW21_B1,
FLSLOADER_P0_PW21_B2,FLSLOADER_P0_PW21_B3,
FLSLOADER_P0_PW30_B0,FLSLOADER_P0_PW30_B1,
FLSLOADER_P0_PW30_B2,FLSLOADER_P0_PW30_B3,
FLSLOADER_P0_PW31_B0,FLSLOADER_P0_PW31_B1,
FLSLOADER_P0_PW31_B2,FLSLOADER_P0_PW31_B3,
FLSLOADER_P0_PW00_B0,FLSLOADER_P0_PW00_B1,
FLSLOADER_P0_PW00_B2,FLSLOADER_P0_PW00_B3,
FLSLOADER_P0_PW01_B0,FLSLOADER_P0_PW01_B1,
FLSLOADER_P0_PW01_B2,FLSLOADER_P0_PW01_B3,
FLSLOADER_P0_PW10_B0,FLSLOADER_P0_PW10_B1,
FLSLOADER_P0_PW10_B2,FLSLOADER_P0_PW10_B3,
FLSLOADER_P0_PW11_B0,FLSLOADER_P0_PW11_B1,
FLSLOADER_P0_PW11_B2,FLSLOADER_P0_PW11_B3,
FLSLOADER_P0_PW20_B0,FLSLOADER_P0_PW20_B1,
FLSLOADER_P0_PW20_B2,FLSLOADER_P0_PW20_B3,
FLSLOADER_P0_PW21_B0,FLSLOADER_P0_PW21_B1,
FLSLOADER_P0_PW21_B2,FLSLOADER_P0_PW21_B3,
FLSLOADER_P0_PW30_B0,FLSLOADER_P0_PW30_B1,
FLSLOADER_P0_PW30_B2,FLSLOADER_P0_PW30_B3,
FLSLOADER_P0_PW31_B0,FLSLOADER_P0_PW31_B1,
FLSLOADER_P0_PW31_B2,FLSLOADER_P0_PW31_B3,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
FLSLOADER_CONFIRMATION_CODE_B0,FLSLOADER_CONFIRMATION_CODE_B1,
FLSLOADER_CONFIRMATION_CODE_B2,FLSLOADER_CONFIRMATION_CODE_B3,
0x00U,0x00U,0x00U,0x00U,
FLSLOADER_CONFIRMATION_CODE_B0,FLSLOADER_CONFIRMATION_CODE_B1,
FLSLOADER_CONFIRMATION_CODE_B2,FLSLOADER_CONFIRMATION_CODE_B3,
0x00U,0x00U,0x00U,0x00U
};
#endif
  /*#if ((FLSLOADER_PF0_PROT == WRITE_PROTECTION)||...*/

  /* UCB_DFLASH Content */
#if (FLSLOADER_DF0_PROT != NO_PROTECTION)
  /* UCB_DFLASH
  page 0 -bytes 0-3: PROCOND
          bytes 4-7: all zeroes
  page 1 -bytes 8-11 : all zeroes
          bytes 12-15 : all zeroes
  page 2 -bytes 16-19: copy of PROCOND
          bytes 20-23: all zeroes
  page 3 -bytes 24-27 : all zeroes
          bytes 28-31 : all zeroes
  page 4 -bytes 32-39: PW0
  page 5 -bytes 40-47: PW1
  page 6 -bytes 48-55: PW2
  page 7 -bytes 56-63: PW3
  page 8 -bytes 64-71: PW0
  page 9  -bytes 72-79: PW1
  page 10 -bytes 80-87: PW2
  page 11 -bytes 88-95: PW3
  page 12 -bytes 96-103: all zeroes
  page 13 -bytes 104-111: all zeroes
  page 14 -bytes 112-115: confirmation code
           bytes 116-119: zeroes
  page 15 -bytes 120-123: confirmation code
           bytes 124-127: zeroes*/

  /*IFX_MISRA_RULE_08_07_STATUS=lsLoader_UcbDFlashPage0 is a constant array 
  hence not declare in the calling function*/
static const uint8 FlsLoader_UcbDFlashPage0[FLSLOADER_UCB_SIZE] = {
FLSLOADER_PROCOND_BYTE0,FLSLOADER_PROCOND_BYTE1,
FLSLOADER_PROCOND_BYTE2,FLSLOADER_PROCOND_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
FLSLOADER_PROCOND_BYTE0,FLSLOADER_PROCOND_BYTE1,
FLSLOADER_PROCOND_BYTE2,FLSLOADER_PROCOND_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
FLSLOADER_DF0_PW00_B0,FLSLOADER_DF0_PW00_B1,
FLSLOADER_DF0_PW00_B2,FLSLOADER_DF0_PW00_B3,
FLSLOADER_DF0_PW01_B0,FLSLOADER_DF0_PW01_B1,
FLSLOADER_DF0_PW01_B2,FLSLOADER_DF0_PW01_B3,
FLSLOADER_DF0_PW10_B0,FLSLOADER_DF0_PW10_B1,
FLSLOADER_DF0_PW10_B2,FLSLOADER_DF0_PW10_B3,
FLSLOADER_DF0_PW11_B0,FLSLOADER_DF0_PW11_B1,
FLSLOADER_DF0_PW11_B2,FLSLOADER_DF0_PW11_B3,
FLSLOADER_DF0_PW20_B0,FLSLOADER_DF0_PW20_B1,
FLSLOADER_DF0_PW20_B2,FLSLOADER_DF0_PW20_B3,
FLSLOADER_DF0_PW21_B0,FLSLOADER_DF0_PW21_B1,
FLSLOADER_DF0_PW21_B2,FLSLOADER_DF0_PW21_B3,
FLSLOADER_DF0_PW30_B0,FLSLOADER_DF0_PW30_B1,
FLSLOADER_DF0_PW30_B2,FLSLOADER_DF0_PW30_B3,
FLSLOADER_DF0_PW31_B0,FLSLOADER_DF0_PW31_B1,
FLSLOADER_DF0_PW31_B2,FLSLOADER_DF0_PW31_B3,
FLSLOADER_DF0_PW00_B0,FLSLOADER_DF0_PW00_B1,
FLSLOADER_DF0_PW00_B2,FLSLOADER_DF0_PW00_B3,
FLSLOADER_DF0_PW01_B0,FLSLOADER_DF0_PW01_B1,
FLSLOADER_DF0_PW01_B2,FLSLOADER_DF0_PW01_B3,
FLSLOADER_DF0_PW10_B0,FLSLOADER_DF0_PW10_B1,
FLSLOADER_DF0_PW10_B2,FLSLOADER_DF0_PW10_B3,
FLSLOADER_DF0_PW11_B0,FLSLOADER_DF0_PW11_B1,
FLSLOADER_DF0_PW11_B2,FLSLOADER_DF0_PW11_B3,
FLSLOADER_DF0_PW20_B0,FLSLOADER_DF0_PW20_B1,
FLSLOADER_DF0_PW20_B2,FLSLOADER_DF0_PW20_B3,
FLSLOADER_DF0_PW21_B0,FLSLOADER_DF0_PW21_B1,
FLSLOADER_DF0_PW21_B2,FLSLOADER_DF0_PW21_B3,
FLSLOADER_DF0_PW30_B0,FLSLOADER_DF0_PW30_B1,
FLSLOADER_DF0_PW30_B2,FLSLOADER_DF0_PW30_B3,
FLSLOADER_DF0_PW31_B0,FLSLOADER_DF0_PW31_B1,
FLSLOADER_DF0_PW31_B2,FLSLOADER_DF0_PW31_B3,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
FLSLOADER_CONFIRMATION_CODE_B0,FLSLOADER_CONFIRMATION_CODE_B1,
FLSLOADER_CONFIRMATION_CODE_B2,FLSLOADER_CONFIRMATION_CODE_B3,
0x00U,0x00U,0x00U,0x00U,
FLSLOADER_CONFIRMATION_CODE_B0,FLSLOADER_CONFIRMATION_CODE_B1,
FLSLOADER_CONFIRMATION_CODE_B2,FLSLOADER_CONFIRMATION_CODE_B3,
0x00U,0x00U,0x00U,0x00U
};
#endif
  /*#if (FLSLOADER_PF0_PROT != NO_PROTECTION)*/

  /* UCB OTP FLASH content */
#if ((FLSLOADER_PF0_PROT == OTP_PROTECTION)||\
     (FLSLOADER_PF0_PROT == WOP_PROTECTION))
/* UCB_OTP
  page 0 -bytes 0-3: PROCONOTP0
          bytes 4-7: PROCONOTP1
  page 1 -bytes 8-15 : all zeroes
  page 2 -bytes 16-19: copy of PROCONOTP0
          bytes 20-23: copy of PROCONOTP1
  page 3 -bytes 24-31 : all zeroes
  page 4 -bytes 32-35: PROCONWOP0
          bytes 36-39: PROCONWOP1
  page 5  -bytes 40-47: all zeroes
  page 6  -bytes 48-51: copy of PROCONWOP0
  page    -bytes 52-55: copy of ROCONWOP1
  page 7   -bytes 56-63: all zeroes
  page 8   -bytes 64-71: all zeroes
  page 9  -bytes 72-79: all zeroes
  page 10  -bytes 80-87: all zeroes
  page 11  -bytes 88-95: all zeroes
  page 12  -bytes 96-103: all zeroes
  page 13  -bytes 104-111 : all zeroes
  page 14   bytes 112-115: confirmation code
           -bytes 116-119: all zeroes
  page 15  -bytes 120-123: copy of confirmation code
            bytes 124-127: all zeroes
*/

  /*IFX_MISRA_RULE_08_07_STATUS=lsLoader_UcbOTPPFlashPage0 is a constant array 
  hence not declare in the calling function*/
static const uint8 FlsLoader_UcbOTPFlashPage0[FLSLOADER_UCB_SIZE] = {
FLSLOADER_PROCONOTP0_BYTE0,FLSLOADER_PROCONOTP0_BYTE1,
FLSLOADER_PROCONOTP0_BYTE2,FLSLOADER_PROCONOTP0_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
FLSLOADER_PROCONOTP0_BYTE0,FLSLOADER_PROCONOTP0_BYTE1,
FLSLOADER_PROCONOTP0_BYTE2,FLSLOADER_PROCONOTP0_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
FLSLOADER_PROCONWOP0_BYTE0,FLSLOADER_PROCONWOP0_BYTE1,
FLSLOADER_PROCONWOP0_BYTE2,FLSLOADER_PROCONWOP0_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
FLSLOADER_PROCONWOP0_BYTE0,FLSLOADER_PROCONWOP0_BYTE1,
FLSLOADER_PROCONWOP0_BYTE2,FLSLOADER_PROCONWOP0_BYTE3,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,
FLSLOADER_CONFIRMATION_CODE_B0,FLSLOADER_CONFIRMATION_CODE_B1,
FLSLOADER_CONFIRMATION_CODE_B2,FLSLOADER_CONFIRMATION_CODE_B3,
0x00U,0x00U,0x00U,0x00U,
FLSLOADER_CONFIRMATION_CODE_B0,FLSLOADER_CONFIRMATION_CODE_B1,
FLSLOADER_CONFIRMATION_CODE_B2,FLSLOADER_CONFIRMATION_CODE_B3,
0x00U,0x00U,0x00U,0x00U};

#endif
/*  #if ((FLSLOADER_PF0_PROT == OTP_PROTECTION)...*/

#define FLSLOADER_STOP_SEC_CONST_32BIT
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                    Prototypes Of Local Functions                           **
*******************************************************************************/
#define FLSLOADER_START_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
#if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
  /*
  Function FlsLoader_lAddressPageCheck
  Input Parameters: TargetAddress
  */
IFX_LOCAL_INLINE uint8 FlsLoader_lAddressPageCheck(
                                         FlsLoader_AddressType TargetAddress);
  /*
  Function FlsLoader_lDFlashCheck
  Input Parameters: TargetAddress
  */
IFX_LOCAL_INLINE boolean FlsLoader_lDFlashCheck(
                                     FlsLoader_AddressType TargetAddress);
  /*
  Function FlsLoader_lDFlashAddressCheck
  Input Parameters: TargetAddress
  */
IFX_LOCAL_INLINE uint8 FlsLoader_lDFlashAddressCheck(
                                           FlsLoader_AddressType TargetAddress);
  /*
  Function FlsLoader_lDFlashLengthCheck
  Input Parameters: TargetAddress,Length
  */
IFX_LOCAL_INLINE uint8 FlsLoader_lDFlashLengthCheck(
               FlsLoader_LengthType Length,FlsLoader_AddressType TargetAddress);
    /*
  Function FlsLoader_lPFlashCheck
  Input Parameters: TargetAddress
  */
IFX_LOCAL_INLINE boolean FlsLoader_lPFlashCheck(
                                     FlsLoader_AddressType TargetAddress);
  /*
  Function FlsLoader_lPFlashAddressCheck
  Input Parameters: TargetAddress
  */
IFX_LOCAL_INLINE uint8 FlsLoader_lPFlashAddressCheck(
                                           FlsLoader_AddressType TargetAddress);
  /*
  Function FlsLoader_lDFlashPageCheck
  Input Parameters: TargetAddress,Length
  */
IFX_LOCAL_INLINE boolean FlsLoader_lDFlashPageCheck(
             FlsLoader_AddressType TargetAddress,FlsLoader_LengthType Length);
#endif

#if (FLSLOADER_ENABLE_LOCKCHECK == STD_ON)
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lPFlashSectorCheck (
                                    FlsLoader_AddressType TargetAddress,
                                    FlsLoader_LengthType Length);

IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lCheckSectorProt(
                                    uint32 TargetAddress,
                                    FlsLoader_LengthType Length);

IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lSectorProtCheck(
                                    uint32 SectorNumber,
                                    FlsLoader_LengthType Length );

#endif
#define FLSLOADER_STOP_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#define FLSLOADER_START_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#if (FLSLOADER_LOCK_UNLOCK_API == STD_ON)
IFX_LOCAL_INLINE FlsLoader_ReturnType  FlsLoader_lLockCmdCycles(
                        FlsLoader_AddressType StartAddress,
                        FlsLoader_AddressType UcbSectorAddress,
                        const FlsLoader_AddressType *PdataPtr);

IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lLockCmdCyclesWrite(
                        FlsLoader_AddressType StartAddress,
                        FlsLoader_AddressType UcbSectorAddress,
                        const FlsLoader_AddressType *PdataPtr);
#endif

IFX_LOCAL_INLINE uint32 FlsLoader_lSectorNumber(uint32 Offset);
#define FLSLOADER_STOP_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
/******************************************************************************
**                      Private Variable Definitions                         **
******************************************************************************/
#ifdef IFX_FLSLOADER_DEBUG
  #define FLSLOADER_START_SEC_VAR_32BIT
  #include "MemMap.h"
    extern volatile uint32 TestFlsloader_DebugMask00;
    extern volatile uint32 TestFlsloader_DebugMask01;
    extern volatile uint32 TestFlsloader_DebugMask03;
    extern volatile uint32 TestFlsloader_DebugMask09;
    extern volatile uint32 TestFlsloader_DebugMask06;
    extern volatile uint32 TestFlsloader_DebugMask08;
    extern volatile uint32 TestFlsloader_DebugMask10;
    extern volatile uint32 TestFlsloader_DebugMask07;
    extern volatile uint32 TestFlsloader_DebugMask11;
    extern volatile uint32 TestFlsloader_DebugMask12;
    extern volatile uint32 TestFlsloader_DebugMask13;
    extern volatile uint32 TestFlsloader_DebugMask14;
    extern volatile uint32 TestFlsloader_DebugMask15;
  #define FLSLOADER_STOP_SEC_VAR_32BIT
  #include "MemMap.h"
#endif

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

#define FLSLOADER_START_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#if (FLSLOADER_LOCK_UNLOCK_API == STD_ON)
/*******************************************************************************
** Syntax           : FlsLoader_ReturnType FlsLoader_lLock(void)              **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters (in)  : none                                                    **
**                                                                            **
** Parameters (out) :                                                         **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function Locks the required profile and called     **
**                    from Lock API.                                          **
**                                                                            **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_lLock(void)
{
  FlsLoader_ReturnType RetError;
  #if (FLSLOADER_DF0_PROT != NO_PROTECTION)
  FlsLoader_ReturnType RetErrorDF;
  #endif
  
  #if ((FLSLOADER_DF0_PROT != NO_PROTECTION) ||\
       (FLSLOADER_PF0_PROT != NO_PROTECTION))  
  FlsLoader_AddressType StartAddress;
  FlsLoader_AddressType UcbSectorAddress;
  const FlsLoader_AddressType *PdataPtr ;
  StartAddress = FLSLOADER_DFLASH0_START_ADDRESS;
  #endif  
  
  #if ((FLSLOADER_DF0_PROT == NO_PROTECTION) &&\
       (FLSLOADER_PF0_PROT == NO_PROTECTION))
  RetError = FLSLOADER_E_OK;
  #else
  #if ((FLSLOADER_PF0_PROT == WRITE_PROTECTION)||\
       (FLSLOADER_PF0_PROT == READ_PROTECTION))
  UcbSectorAddress = FLSLOADER_UCB_PFLASH;
  /*IFX_MISRA_RULE_11_05_STATUS="FlsLoader_UcbPFlashPage0" attempt to cast away
  const in terms of pointer access.*/
  PdataPtr = (FlsLoader_AddressType *)(void*)(&FlsLoader_UcbPFlashPage0[0]);
  RetError = FlsLoader_lLockCmdCycles(StartAddress,UcbSectorAddress,PdataPtr);
  #elif ((FLSLOADER_PF0_PROT == OTP_PROTECTION)||\
         (FLSLOADER_PF0_PROT == WOP_PROTECTION))
  UcbSectorAddress = FLSLOADER_UCB_OTP;
  /*IFX_MISRA_RULE_11_05_STATUS="FlsLoader_UcbOTPFlashPage0" attempt to 
  cast away const in terms of pointer access.*/
  PdataPtr = (FlsLoader_AddressType *)(void*)(&FlsLoader_UcbOTPFlashPage0[0]);
  RetError = FlsLoader_lLockCmdCycles(StartAddress,UcbSectorAddress,PdataPtr);
  #endif
  #if (FLSLOADER_DF0_PROT != NO_PROTECTION)
  UcbSectorAddress = FLSLOADER_UCB_DFLASH;
  /*IFX_MISRA_RULE_11_05_STATUS="FlsLoader_UcbDFlashPage0" attempt to cast away
  const in terms of pointer access.*/
  PdataPtr = (FlsLoader_AddressType *)((void*)FlsLoader_UcbDFlashPage0);
  RetErrorDF = FlsLoader_lLockCmdCycles(StartAddress,UcbSectorAddress,PdataPtr);
  #if (FLSLOADER_PF0_PROT != NO_PROTECTION)
  if(RetError == FLSLOADER_E_OK)
  #endif
  {
    RetError = RetErrorDF;
  }
  #endif
  #endif
  return RetError;
}

/*******************************************************************************
** Syntax   :  IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lLockCmdCycles(**
**                        FlsLoader_AddressType TargetAddress ,               **
**                        FlsLoader_AddressType UcbSectorAddress,             **
**                        FlashLoader_DataType* PdataPtr                      **
**                    )                                                       **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters (in)  : none                                                    **
**                                                                            **
** Parameters (out) :                                                         **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function Locks the required profile and called     **
**                    from Lock  API.                                         **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lLockCmdCycles(
                         FlsLoader_AddressType StartAddress,
                         FlsLoader_AddressType UcbSectorAddress,
                         const FlsLoader_AddressType *PdataPtr)
{
  FlsLoader_ReturnType  RetError;
  volatile uint32       BusyCheck;
  uint32                InnerCount;
  uint32                Icr;
  volatile uint32       FlashFsr;
  
  #if ((FLSLOADER_PF0_PROT == OTP_PROTECTION)||\
       (FLSLOADER_PF0_PROT == WOP_PROTECTION))
  uint32 LoopCount;
  #endif

  /* Enter Critical Section */
  Icr = FlsLoader_lDisableInts();

  #ifndef IFX_FLSLOADER_DEBUG
  /* Erase the corresponding User Configuraiton Block */
  /* cycle 1 */
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA50(StartAddress))  = 
                                                             UcbSectorAddress;
  /* cycle 2 */
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA58(StartAddress))  = 
                                                    FLSLOADER_NUM_UCB_SECTORS;
  /* cycle 3 */
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(StartAddress))  = 
                                                           FLSLOADER_VALUE_80;
  /* cycle 4 */
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(StartAddress))  = 
                                                           FLSLOADER_VALUE_50;
  #endif
  InnerCount = 0U;

  #ifdef  IFX_FLSLOADER_DEBUG
  FlashFsr =  TestFlsloader_DebugMask12;
  #else
  FlashFsr = FLASH0_FSR.B.ERASE;
  #endif

  while((FlashFsr != FLSLOADER_BIT_SET) &&
       (InnerCount < FLSLOADER_CMDCYCLE_TIMEOUT))
  {
    InnerCount++;
    #ifdef  IFX_FLSLOADER_DEBUG
    FlashFsr =  TestFlsloader_DebugMask12;
    #else
    FlashFsr = FLASH0_FSR.B.ERASE;
    #endif   
  }

  if(InnerCount < FLSLOADER_CMDCYCLE_TIMEOUT)
  {
    /* Check for FLASH BUSY flag */
    /* PnBUSY and D0BUSY */
    InnerCount = 0U;
    #ifdef  IFX_FLSLOADER_DEBUG
    BusyCheck =  TestFlsloader_DebugMask03;
    #else
    BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_NUM_2);
    #endif
    while(( BusyCheck != 0U) && 
          (InnerCount < FLSLOADER_BUSY_TIMEOUT))
    {
      InnerCount++;
      #ifdef  IFX_FLSLOADER_DEBUG
      BusyCheck = TestFlsloader_DebugMask03;
      #else
      BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_NUM_2);
      #endif
    }
    if (InnerCount < FLSLOADER_BUSY_TIMEOUT )
    {
      /* This check makes sure that  erase operation is requested,
      No protection error and no sequence error has occurred. */
      
      /* This check makes sure No protection error and no sequence error
      and No operation error has occurred.*/
      #ifdef IFX_FLSLOADER_DEBUG
      RetError = TestFlsloader_DebugMask13;
      #else
      RetError = FlsLoader_lProtSeqOperErrorCheck();
      #endif
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
  /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
  /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 for address
  comparison/calculation purpose.*/
   *((FlsLoader_AddressType*)FlsLoader_lClearStatus(StartAddress)) = 
                                                           FLSLOADER_VALUE_FA;

  /* Enter Critical Section */
  FlsLoader_lEnableInts(Icr);

  /* Erase is completed now program 16 pages of UCB in Page mode. */

  /* Enter Critical Section */
  Icr = FlsLoader_lDisableInts();

  #if ((FLSLOADER_PF0_PROT == OTP_PROTECTION)||\
      (FLSLOADER_PF0_PROT == WOP_PROTECTION))
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  if(UcbSectorAddress == FLSLOADER_UCB_OTP)
  {
    for(LoopCount = 0U;
        (LoopCount < FLSLOADER_NUM_8);
        LoopCount++)
    {
      if (RetError == FLSLOADER_E_OK)
      {
        RetError = FlsLoader_lLockCmdCyclesWrite(StartAddress,
                         UcbSectorAddress,PdataPtr);
        UcbSectorAddress+=FLSLOADER_UCB_DATA_SIZE;
      }
    }
  }
  else 
  #endif
  {
    if (RetError == FLSLOADER_E_OK)
    {
      RetError = FlsLoader_lLockCmdCyclesWrite(StartAddress,
                       UcbSectorAddress,PdataPtr);
    }
  }

  /* Exit Crtical Section */
  FlsLoader_lEnableInts(Icr);

  return(RetError);
}
/*******************************************************************************
** Syntax   :  IFX_LOCAL_INLINE FlsLoader_ReturnType                          **
**                                FlsLoader_lLockCmdCyclesWrite(              **
**                                FlsLoader_AddressType TargetAddress ,       **
**                                FlsLoader_AddressType UcbSectorAddress,     **
**                                const FlashLoader_DataType* PdataPtr        **
**                                )                                           **
**                                                                            **
** Service ID       :   NA                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters (in)  : none                                                    **
**                                                                            **
** Parameters (out) :                                                         **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function Locks the required profile and called     **
**                    from Lock  API.                                         **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lLockCmdCyclesWrite(
                         FlsLoader_AddressType StartAddress,
                         FlsLoader_AddressType UcbSectorAddress,
                         const FlsLoader_AddressType *PdataPtr)
{
  FlsLoader_ReturnType      RetError;
  uint32                    Count;
  uint32                    InCount;
  volatile uint32           FlashBusyStat;
  volatile uint32         BusyCheck;
  FlsLoader_AddressType    *LoadPageAddress;
  RetError = FLSLOADER_E_OK;
  
  for (Count = 0U; 
      (Count < FLSLOADER_NUM_UCB_PAGES) && (RetError == FLSLOADER_E_OK);
       Count++)
  {
    /* Enter the Page Mode */
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 for 
    address comparison/calculation purpose.*/
    /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 for 
    address comparison/calculation purpose.*/
    *((FlsLoader_AddressType*)FlsLoader_lEnterPageMode(StartAddress)) =
                                             FLSLOADER_DFLASH_VAL;
    InCount = 0U;
    #ifdef  IFX_FLSLOADER_DEBUG
    FlashBusyStat =  TestFlsloader_DebugMask08;
    #else
    FlashBusyStat = FLASH0_FSR.B.DFPAGE;
    #endif
    /* Check if Program Flash is in page mode */
    while((FlashBusyStat != FLSLOADER_BIT_SET) && 
         (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
    {
      InCount++;
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat =  TestFlsloader_DebugMask08;
      #else
      FlashBusyStat = FLASH0_FSR.B.DFPAGE;
      #endif
    }
    /* Check PFPAGE/DFPAGE bit was SET */
    if(InCount < FLSLOADER_CMDCYCLE_TIMEOUT)
    {
      /* Load the data */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
      type volatile uint32 to avoid compiler optimization in command 
      cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
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
      /* cycle 1 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
      type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA50(StartAddress))
                                    = UcbSectorAddress;
      /* cycle 2 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
      type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AA58(StartAddress))
                                    =  FLSLOADER_VALUE_00;
      /* cycle 3 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
      type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
        *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(StartAddress))
                                    =  FLSLOADER_VALUE_A0;
      /*cycle 4 */
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
      type volatile uint32 to avoid compiler optimization in 
      command cycles*/
      /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to uint32 
      for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FLSLOADER_GET_CYCLE_AAA8(StartAddress))
                                    = FLSLOADER_VALUE_AA;
      
      InCount = 0U;
      #ifdef  IFX_FLSLOADER_DEBUG
      FlashBusyStat =  TestFlsloader_DebugMask14;
      #else
      FlashBusyStat = FLASH0_FSR.B.PROG;
      #endif
      while((FlashBusyStat != FLSLOADER_BIT_SET) &&
            (InCount < FLSLOADER_CMDCYCLE_TIMEOUT))
      {
        InCount++;
        #ifdef  IFX_FLSLOADER_DEBUG
        FlashBusyStat =  TestFlsloader_DebugMask14;
        #else
        FlashBusyStat = FLASH0_FSR.B.PROG;
        #endif
      }
      if (InCount < FLSLOADER_CMDCYCLE_TIMEOUT)
      {
        /* Check for FLASH BUSY flag */
        InCount = 0U;
        #ifdef  IFX_FLSLOADER_DEBUG
        BusyCheck = TestFlsloader_DebugMask15;
        #else
        BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_NUM_2);
        #endif
        while((BusyCheck != 0U)&& 
             (InCount < FLSLOADER_BUSY_TIMEOUT))
        {
          InCount++;
          #ifdef IFX_FLSLOADER_DEBUG
          BusyCheck = TestFlsloader_DebugMask15;
          #else
          BusyCheck = (FLASH0_FSR.U & (uint32)FLSLOADER_NUM_2);
          #endif
        }
        if (InCount < FLSLOADER_BUSY_TIMEOUT )
        {
        /* This check makes sure No protection error and no sequence error
        and No operation error has occurred.*/
        #ifdef IFX_FLSLOADER_DEBUG
        RetError = TestFlsloader_DebugMask14;
        #else
        RetError = FlsLoader_lProtSeqOperErrorCheck();
        #endif
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
      /*IFX_MISRA_RULE_11_01_STATUS=The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      /*IFX_MISRA_RULE_11_03_STATUS=The Pointers are type-cast to 
      uint32 for address comparison/calculation purpose.*/
      *((FlsLoader_AddressType*)FlsLoader_lClearStatus(StartAddress))
                                    =  FLSLOADER_VALUE_FA;

      /* Iterate the TargetAddress to the next page Address */
      UcbSectorAddress = UcbSectorAddress + FLSLOADER_PAGE_SIZE;
    }  /* End of InCount < FLSLOADER_CMDCYCLE_TIMEOUT*/
    else
    {
      RetError = FLSLOADER_E_NOT_OK;
    }  
  }  /* End of For Loop */
  return(RetError);
}
#endif
  /* #if (FLSLOADER_LOCK_UNLOCK_API == STD_ON)*/

/*******************************************************************************
** Syntax :           FlsLoader_ReturnType FlsLoader_lCheckOTPWOP(void)       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : FLSLOADER_E_OK                                          **
**                    FLSLOADER_E_ROMVERSION                                  **
**                                                                            **
** Description      : This function checks whether all the sectors have been  **
**                    protected under OTP and WOP                             **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_lCheckOTPWOP(void)
{
  FlsLoader_ReturnType  RetError;
  uint32                Fls0Check;
  uint32                Fls0WOPCheck;
  RetError = FLSLOADER_E_OK;

  #ifdef IFX_FLSLOADER_DEBUG
  Fls0Check = TestFlsloader_DebugMask00;
  Fls0WOPCheck = TestFlsloader_DebugMask00;
  #else
  Fls0Check = (uint32)(FLASH0_PROCONOTP0.U & FLSLOADER_PROCON_CHECK);
  Fls0WOPCheck = (uint32)(FLASH0_PROCONWOP0.U & FLSLOADER_PROCON_CHECK);
  #endif

  if ((Fls0Check == FLSLOADER_PROCON_CHECK) ||
      (Fls0WOPCheck == FLSLOADER_PROCON_CHECK))
  {
    RetError = FLSLOADER_E_ROMVERSION;
  }
  return (RetError);
}

/*******************************************************************************
** Syntax :           IFX_LOCAL_INLINE uint32 FlsLoader_lSectorNumber(        **
**                      uint32 Offset                                         **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : offset: PFLASH0 Offset address                          **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : SectorNumber                                            **
**                                                                            **
** Description      : This function will return the sector number             **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 FlsLoader_lSectorNumber(uint32 Offset)
{
  uint32 SectorNumber;
  for(SectorNumber = 0U;
      SectorNumber < FLSLOADER_NUM_OF_PF0_SECTORS;
      SectorNumber++)
  {
    if(FlsLoader_PFlashSectorOffset[SectorNumber] == Offset)
    {
      break;
    }
  }
 if(SectorNumber == FLSLOADER_NUM_OF_PF0_SECTORS)
  {
    SectorNumber = FLSLOADER_INV;
  }
  return (SectorNumber);
}

#define FLSLOADER_STOP_SEC_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#define FLSLOADER_START_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#if (FLSLOADER_DEV_ERROR_DETECT == STD_ON)
/*******************************************************************************
** Syntax             FlsLoader_ReturnType FlsLoader_lWriteDetCheck(          **
**                      FlsLoader_LengthType Length,                          **
**                      FlsLoader_AddressType TargetAddress                   **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : Length :Length of the data                              **
**                    TargetAddress :Destination Address in Flash             **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : RetError                                                **
**                                                                            **
** Description      : This function is DET Check function for                 **
**                    FlsLoader_lWrite                                        **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_lWriteDetCheck(FlsLoader_LengthType Length,
                                           FlsLoader_AddressType TargetAddress)
{
  uint8                     ErrorHook;
  FlsLoader_ReturnType      RetError;
  FlsLoader_LengthType      TargetEnd;
  /* Init RetError to OK */
  RetError = FLSLOADER_E_OK;

  /* check for Init DET */
  if (FlsLoader_InitStatus != FLSLOADER_INITIALIZED)
  {
    /* CD_FlsLoader034 :Report missing initialization DET */
    Det_ReportError(FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_WRITE,
                    FLSLOADER_E_UNINIT);
    RetError = FLSLOADER_E_NOT_OK;
  }
  /* Check of TargetAddress
  TargetAddress can be multiple of 256 bytes in caseof PFLASH or 32 bytes.
  incase of DFLASH. */
  /* Report Invalid TargetAddress to DET */
  if ( RetError != FLSLOADER_E_NOT_OK)
  {
    ErrorHook = FlsLoader_lAddressPageCheck(TargetAddress);
    if  (0U == ErrorHook)
    {
      Det_ReportError(FLSLOADER_MODULE_ID,
                      FLSLOADER_INSTANCE_ID,
                      FLSLOADER_SID_WRITE,
                      FLSLOADER_E_PARAM_ADDRESS);
      RetError = FLSLOADER_E_NOT_OK;
    }
  }

  if ( RetError != FLSLOADER_E_NOT_OK)
  {
    /* Returns true if, position is DFLASH address */
    if (FlsLoader_lCheckFlashType(TargetAddress))
    {
      /* Given TargetAddress and Length should not exceed the flash boundary */
      /* Its Data Flash assign,Report invalid Length to DET */
      if (FlsLoader_lDFlashPageCheck(TargetAddress,Length))
      {
        Det_ReportError(FLSLOADER_MODULE_ID,
                        FLSLOADER_INSTANCE_ID,
                        FLSLOADER_SID_WRITE,
                        FLSLOADER_E_PARAM_LENGTH);
        RetError = FLSLOADER_E_NOT_OK;
      }
    }
    else /*PFlash*/
    {
      TargetEnd = (Length + TargetAddress)-1U;
      /*PFLASH0*/
      if ((Length == (FlsLoader_LengthType)0U) ||
         ((Length & ((FlsLoader_LengthType)FLSLOADER_MOD_32)) != 0U))
      {
        Det_ReportError(FLSLOADER_MODULE_ID,
                        FLSLOADER_INSTANCE_ID,
                        FLSLOADER_SID_WRITE,
                        FLSLOADER_E_PARAM_LENGTH);
        RetError = FLSLOADER_E_NOT_OK;
      }
      else if(TargetEnd > (FlsLoader_LengthType)FLSLOADER_PFLASH0_END)
      {
        Det_ReportError(
                        FLSLOADER_MODULE_ID,
                        FLSLOADER_INSTANCE_ID,
                        FLSLOADER_SID_WRITE,
                        FLSLOADER_E_PARAM_LENGTH);
        RetError = FLSLOADER_E_NOT_OK;
      }
      else
      {
        /* dummy else */
      }
    }
  }
  return (RetError);
}
/*******************************************************************************
** Syntax           :  FlsLoader_ReturnType FlsLoader_lEraseDetCheck(         **
**                      FlsLoader_LengthType Length,                          **
**                      FlsLoader_AddressType TargetAddress                   **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : Length :Length of the data                              **
**                    TargetAddress :Destination Address in Flash             **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : RetError                                                **
**                                                                            **
** Description      : This function is DET Check  for FlsLoader_Erase         **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_lEraseDetCheck(
                                      FlsLoader_LengthType Length,
                                      FlsLoader_AddressType TargetAddress)
{
  FlsLoader_ReturnType  RetError;
  uint32                Offset;
  uint32                SectorNumber;
  uint8                 ErrorHook;

  RetError = FLSLOADER_E_OK;
  
  if (FlsLoader_InitStatus != FLSLOADER_INITIALIZED)
  {
    /* CD_FlsLoader039: Report missing initilization to DET */
    Det_ReportError(FLSLOADER_MODULE_ID,
                    FLSLOADER_INSTANCE_ID,
                    FLSLOADER_SID_ERASE,
                    FLSLOADER_E_UNINIT);
    RetError = FLSLOADER_E_NOT_OK;
  }
  else
  {
    /* CD_FlsLoader041: Report invalid TargetAddress to DET */
    if (FlsLoader_lCheckFlashType(TargetAddress))
    { /* Check for DFLASH Address*/
      if (FlsLoader_lDFlashCheck(TargetAddress))
      {
        ErrorHook = FlsLoader_lDFlashAddressCheck(TargetAddress);
        if  (0U == ErrorHook)
        {
          Det_ReportError(FLSLOADER_MODULE_ID,
                          FLSLOADER_INSTANCE_ID,
                          FLSLOADER_SID_ERASE,
                          FLSLOADER_E_PARAM_ADDRESS);
          RetError = FLSLOADER_E_NOT_OK;
        }
      }
      if (RetError != FLSLOADER_E_NOT_OK)
      {
        ErrorHook = FlsLoader_lDFlashLengthCheck(Length,TargetAddress);
        if(0U == ErrorHook)
        {
          Det_ReportError(FLSLOADER_MODULE_ID,
                          FLSLOADER_INSTANCE_ID,
                          FLSLOADER_SID_ERASE,
                          FLSLOADER_E_PARAM_LENGTH);
          RetError = FLSLOADER_E_NOT_OK;
        }
      }
    }
    else
    {
      /* Check for PFLASH Address.*/
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
      if(TargetAddress < FLSLOADER_PFLASH0_START_ADDRESS)
      {
        Det_ReportError(FLSLOADER_MODULE_ID,
                        FLSLOADER_INSTANCE_ID,
                        FLSLOADER_SID_ERASE,
                        FLSLOADER_E_PARAM_ADDRESS);
        RetError = FLSLOADER_E_NOT_OK;
      }
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
      else if(TargetAddress > FLSLOADER_PFLASH0_END)
      {
        Det_ReportError(FLSLOADER_MODULE_ID,
                      FLSLOADER_INSTANCE_ID,
                      FLSLOADER_SID_ERASE,
                      FLSLOADER_E_PARAM_ADDRESS);
        RetError = FLSLOADER_E_NOT_OK;
      }
      else
      {
        ErrorHook = FlsLoader_lPFlashAddressCheck(TargetAddress);
        if  (0U == ErrorHook)
        {
          Det_ReportError(FLSLOADER_MODULE_ID,
                          FLSLOADER_INSTANCE_ID,
                          FLSLOADER_SID_ERASE,
                          FLSLOADER_E_PARAM_ADDRESS);
          RetError = FLSLOADER_E_NOT_OK;
        }
      }
      if (RetError != FLSLOADER_E_NOT_OK)
      {
        /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
        volatile uint32 to avoid compiler optimization in command cycles*/
        Offset = TargetAddress - FLSLOADER_PFLASH0_START_ADDRESS;
        SectorNumber = FlsLoader_lSectorNumber(Offset);
        if ((Length == 0U)||
           ((SectorNumber + Length) > FLSLOADER_NUM_OF_PF0_SECTORS))
        {
          Det_ReportError(FLSLOADER_MODULE_ID,
                          FLSLOADER_INSTANCE_ID,
                          FLSLOADER_SID_ERASE,
                          FLSLOADER_E_PARAM_LENGTH);
          RetError = FLSLOADER_E_NOT_OK;
        }
      }
    }
  } /* end of if check FlashType */
  return (RetError);
}
/*******************************************************************************
** Syntax   :   IFX_LOCAL_INLINE uint8 FlsLoader_lAddressPageCheck(           **
**                      FlsLoader_AddressType TargetAddress                   **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress : Address passed                          **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : RetVal                                                  **
**                                                                            **
** Description      : This function returns the give TargetAddress last two   **
**                    nibbles are zero value                                  **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 FlsLoader_lAddressPageCheck(
                                           FlsLoader_AddressType TargetAddress)
{
  uint8     RetVal;
  /* Init ErrorHook to false */
  RetVal = 0U;
  /* Check of TargetAddress
  TargetAddress can be multiple of 256 bytes or 32 bytes. */
  /* CD_FlsLoader036 :Report Invalid TargetAddress to DET */
  if (FlsLoader_lPFlashCheck(TargetAddress))
  {
    /* First byte is already checked, last two nibbles must be zero.*/
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    if ((TargetAddress & (FlsLoader_AddressType)FLSLOADER_MOD_32) == 
                                                          FLSLOADER_POSITION_0 )
    {
      RetVal = 1U; /* valid sector Address */
    }
  }
  else
  {
    if (FlsLoader_lDFlashCheck(TargetAddress))
    {
      /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
      volatile uint32 to avoid compiler optimization in command cycles*/
      /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of type 
      volatile uint32 to avoid compiler optimization in command cycles*/
      if ( (TargetAddress & (FlsLoader_AddressType)FLSLOADER_MOD_8) == 
                                                          FLSLOADER_POSITION_0 )
      {
        RetVal = 1U;
      }
    }
  } /* end of if TargetAddress check */
  return (RetVal);
}
/*******************************************************************************
** Syntax           :  IFX_LOCAL_INLINE boolean FlsLoader_lDFlashCheck(       **
**                                       FlsLoader_AddressType TargetAddress) **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress : Address passed                          **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : RetVal                                                  **
**                                                                            **
** Description      : Checks that TargetAddress is within DFLASH Address Range**
*******************************************************************************/
IFX_LOCAL_INLINE boolean FlsLoader_lDFlashCheck(
                         FlsLoader_AddressType TargetAddress)
{
  boolean RetVal;
  RetVal  = FALSE;

  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  if(TargetAddress >= FLSLOADER_DFLASH0_START_ADDRESS)
  {
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    if(TargetAddress <= FLSLOADER_DFLASH0_END)
    {
      RetVal = TRUE;
    }
  }
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  if(TargetAddress >= FLSLOADER_UCB_START_ADDRESS)
  {
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    if(TargetAddress <= FLSLOADER_UCB_END_ADDRESS)
    {
      RetVal = TRUE;
    }
  }
  return (RetVal);
}

/*******************************************************************************
** Syntax   :   IFX_LOCAL_INLINE uint8 FlsLoader_lDFlashAddressCheck(         **
**                      FlsLoader_AddressType TargetAddress                   **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress : Address passed                          **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : RetVal                                                  **
**                                                                            **
** Description      : This function returns if the given TargetAddress        **
**                    matches the start of DF_EEPROM or DF_UCB sector address **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 FlsLoader_lDFlashAddressCheck(
                                           FlsLoader_AddressType TargetAddress)
{
  uint16               DfSegment;
  uint8                RetVal;
  RetVal = 0U;
  DfSegment = (uint16)(TargetAddress >> FLSLOADER_SHIFT_BY_20);
  if (DfSegment == FLSLOADER_DF_EEPROM_SEGMENT)
  {
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
    type volatile uint32 to avoid compiler optimization in 
    command cycles*/
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
    type volatile uint32 to avoid compiler optimization in 
    command cycles*/
    if ((TargetAddress & (FlsLoader_AddressType)FLSLOADER_EEPROM_SECTOR_SIZE) 
        == FLSLOADER_POSITION_0)
    {
      RetVal = 1U; /*Valid DF_EEPROM Start Sector Address */
    }
  }
  if (DfSegment == FLSLOADER_DF_UCB_SEGMENT)
  {
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
    type volatile uint32 to avoid compiler optimization in 
    command cycles*/
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
    type volatile uint32 to avoid compiler optimization in 
    command cycles*/
    if ((TargetAddress & (FlsLoader_AddressType)FLSLOADER_UCB_SECTOR_SIZE_1) 
        == FLSLOADER_POSITION_0)
    {
      RetVal = 1U; /*Valid DF_UCB Start Sector Address */
    }
  }
  return (RetVal);
}
/*******************************************************************************
** Syntax   :   IFX_LOCAL_INLINE uint8 FlsLoader_lDFlashLengthCheck(          **
**                      FlsLoader_LengthType Length,                          **
**                      FlsLoader_AddressType TargetAddress                   **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress : Address passed                          **
**                    Length        : Length of the data                      **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : RetVal                                                  **
**                                                                            **
** Description      : This function returns if the given Length               **
**                    is within DF_EEPROM or DF_UCB range                     **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 FlsLoader_lDFlashLengthCheck(
               FlsLoader_LengthType Length,FlsLoader_AddressType TargetAddress)
{
  uint32               DFLengthCheck;
  uint16               DfSegment;
  uint8                RetVal;
  
  RetVal = 1U;
  DfSegment = (uint16)(TargetAddress >> FLSLOADER_SHIFT_BY_20);
  if (Length == 0U)
  {
    RetVal = 0U;
  }
  if (DfSegment == FLSLOADER_DF_EEPROM_SEGMENT)
  {
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
    type volatile uint32 to avoid compiler optimization in 
    command cycles*/
    DFLengthCheck = ((uint32)(TargetAddress + (FlsLoader_AddressType)
                            (Length * FLSLOADER_DFLASH_SECTOR_SIZE)))-1U;
    if (( DFLengthCheck > FLSLOADER_DFLASH0_END ) || 
        ( Length > FLSLOADER_NUM_OF_DF_SECTORS ))
    {
      RetVal = 0U; /*InValid DF_EEPROM Sector Length */
    }
  }
  if (DfSegment == FLSLOADER_DF_UCB_SEGMENT)
  {
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
    type volatile uint32 to avoid compiler optimization in 
    command cycles*/
    DFLengthCheck = ((uint32)(TargetAddress + (FlsLoader_AddressType)
                            (Length * FLSLOADER_UCB_SECTOR_SIZE)))-1U;
    if (( DFLengthCheck > FLSLOADER_UCB_END_ADDRESS ) ||
        ( Length > FLSLOADER_NUM_OF_UCB_SECTORS ))
    {
      RetVal = 0U; /*InValid DF_EEPROM Sector Length */
    }    
  }
  return (RetVal);
}

/*******************************************************************************
** Syntax           :  IFX_LOCAL_INLINE boolean FlsLoader_lPFlashCheck(       **
**                                       FlsLoader_AddressType TargetAddress) **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress : Address passed                          **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : uint8: RetVal                                           **
**                                                                            **
** Description      :Checks that TargetAddress is within PFLASH Address Range **
*******************************************************************************/
IFX_LOCAL_INLINE boolean FlsLoader_lPFlashCheck(
                         FlsLoader_AddressType TargetAddress)
{
  boolean RetVal;
  RetVal = FALSE;
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  if(TargetAddress >= FLSLOADER_PFLASH0_START_ADDRESS)
  {
    /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
    volatile uint32 to avoid compiler optimization in command cycles*/
    if(TargetAddress <= FLSLOADER_PFLASH0_END)
    {
      RetVal = TRUE;
    }
  }
  return (RetVal);
}

/*******************************************************************************
** Syntax   :   IFX_LOCAL_INLINE uint8 FlsLoader_lPFlashAddressCheck(         **
**                      FlsLoader_AddressType TargetAddress                   **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress : Address passed                          **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     :  RetVal                                                 **
**                                                                            **
** Description      : This function returns if the given TargetAddress        **
**                    matches the start PFlash sector start address           **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 FlsLoader_lPFlashAddressCheck(
                                           FlsLoader_AddressType TargetAddress)
{
  uint8                RetVal;
  uint32               Offset;
  uint32               SectorNumber;
  RetVal = 1U;

  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of 
  type volatile uint32 to avoid compiler optimization in 
  command cycles*/
  Offset = TargetAddress - FLSLOADER_PFLASH0_START_ADDRESS ;

  for(SectorNumber = 0U; SectorNumber < FLSLOADER_NUM_OF_PF0_SECTORS;
      SectorNumber++)
  {
    if (FlsLoader_PFlashSectorOffset[SectorNumber] == Offset)
    {
      break;
    }
  }
  if (SectorNumber == FLSLOADER_NUM_OF_PF0_SECTORS)
  {
    RetVal = 0U;
  }
  return (RetVal);
}
/*******************************************************************************
** Syntax           :  IFX_LOCAL_INLINE boolean FlsLoader_lDFlashPageCheck(   **
**                                       FlsLoader_AddressType TargetAddress, **
                                         FlsLoader_LengthType Length )        **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress, Length                                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : boolean                                                 **
**                                                                            **
** Description      : Checks the length for DFlash operation                  **
*******************************************************************************/
IFX_LOCAL_INLINE boolean FlsLoader_lDFlashPageCheck(
                FlsLoader_AddressType TargetAddress,FlsLoader_LengthType Length)
{
  boolean                   RetVal;
  uint16                    DfSegment;
  FlsLoader_LengthType      TargetEnd;
  RetVal = FALSE;
   
  if ((Length) == (FlsLoader_LengthType)0U)
  {
    RetVal = TRUE;
  }
  if (((Length) & ((FlsLoader_LengthType)FLSLOADER_MOD_8)) != 0U)
  {
    RetVal = TRUE;
  }
  DfSegment = (uint16)(TargetAddress >> FLSLOADER_SHIFT_BY_20);
  /*IFX_MISRA_RULE_01_02_STATUS=FlsLoader_AddressType is defined of type 
  volatile uint32 to avoid compiler optimization in command cycles*/
  TargetEnd = (Length + (TargetAddress & FLSLOADER_ADDRESS_MASK)) - 1U ;
  if (DfSegment == FLSLOADER_DF_EEPROM_SEGMENT)
  {
    if ((TargetEnd > (FlsLoader_LengthType)FLSLOADER_EEPROM_OFFSETEND) ||
        (Length > FLSLOADER_DFLASH_BANK_SIZE))
    {
      RetVal = TRUE;
    }
  }
  if (DfSegment == FLSLOADER_DF_UCB_SEGMENT)
  {
    if ((TargetEnd > (FlsLoader_LengthType)FLSLOADER_UCB_OFFSETEND) ||
        (Length > FLSLOADER_UCBSIZE))
    {
      RetVal = TRUE;
    }
  }
  return (RetVal);
}
#endif /* #if (FLSLOADER_DEV_ERROR_DETECT == STD_ON) */

/*******************************************************************************
** Syntax           :  uint32 FlsLoader_lGetFlashType(                        **
**                                       FlsLoader_AddressType TargetAddress) **
                                                                              **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress                                           **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : uint32 FlashType                                        **
**                                                                            **
** Description      : Checks the whether Flash is DFlash or Pflash            **
*******************************************************************************/
uint32 FlsLoader_lGetFlashType(FlsLoader_AddressType TargetAddress)
{
  /* With WINDRIVER 5.9.4.8 + Patch TCDIAB-13823 some issues were observed 
  with variable optimisation , "volatile" is used to address this issue.*/
  volatile uint32 Flash_Type; 
  
  if (FlsLoader_lCheckFlashType(TargetAddress))
  {
    /* Given Address belongs to DFLASH */
    /* Data Flash Bank 0 or Bank 1 is assigned */
    Flash_Type = FLSLOADER_DFLASH_BANK0;
  }
  else
  {
    /* Given Address belongs to PFLASH */
    /* Program Flash Bank 0 or Bank 1 is assigned */
    Flash_Type = FLSLOADER_PFLASH_BANK0;
  }
  return Flash_Type;
}

/*******************************************************************************
** Syntax           :  FlsLoader_ReturnType                                   **
**                     FlsLoader_lFlashLockCheck (                            **
**                                    FlsLoader_AddressType TargetAddress,    **
**                                    uint32 FlashType,                       **
**                                    FlsLoader_LengthType Length)            **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       :  Synchronous                                            **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters (in)  : TargetAddress, FlashType, Length                        **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function will return if the sector is protected    **
**                                                                            **
*******************************************************************************/
#if (FLSLOADER_ENABLE_LOCKCHECK == STD_ON)
FlsLoader_ReturnType FlsLoader_lFlashLockCheck (
                        FlsLoader_AddressType TargetAddress,uint32 FlashType,
                        FlsLoader_LengthType Length)
{
  volatile uint32                FlsFpro;
  volatile uint32                FlsFprod;
  volatile uint32                FlsProconp1;
  volatile uint32                FlsProcond1;
  volatile uint32                FlsProcondrpro1;
  FlsLoader_ReturnType           RetError;
  
  RetError = FLSLOADER_E_OK;
  
  if (FlashType != FLSLOADER_DFLASH_BANK0)
  {
    #ifdef  IFX_FLSLOADER_DEBUG
    FlsFpro = TestFlsloader_DebugMask06;
    #else
    FlsFpro = FLASH0_FPRO.B.PRODISP;
    #endif
    if (FlsFpro != 1U)
    {
      #ifdef  IFX_FLSLOADER_DEBUG
      FlsProconp1 = TestFlsloader_DebugMask09;
      #else
      FlsProconp1 = FLASH0_PROCONP0.B.RPRO;
      #endif
      if (FlsProconp1 == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
      else
      {
        RetError = FlsLoader_lPFlashSectorCheck(TargetAddress,Length);
      }
    }
  }
  else
  {
    /*Check if global write protection is installed in DFLASH*/
    #ifdef  IFX_FLSLOADER_DEBUG
    FlsFprod =  TestFlsloader_DebugMask09;
    FlsProcond1 =  TestFlsloader_DebugMask06;
    FlsProcondrpro1 = TestFlsloader_DebugMask06;
    #else
    FlsFprod = FLASH0_FPRO.B.PRODISD;
    FlsProcond1 = FLASH0_PROCOND.B.L;
    FlsProcondrpro1 = FLASH0_PROCOND.B.RPRO;
    #endif
    if(FlsFprod != 1U)
    {
      if(FlsProcond1 == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
      if(FlsProcondrpro1 == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
    }
  }
  return RetError;
}

/*******************************************************************************
** Syntax           :  FlsLoader_ReturnType                                   **
**                     FlsLoader_lPFlashSectorCheck (                         **
**                                       FlsLoader_AddressType TargetAddress, **
**                                       FlsLoader_LengthType Length)         **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters (in)  : TargetAddress, Length                                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function will return if the sector is protected    **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lPFlashSectorCheck (
                          FlsLoader_AddressType TargetAddress,
                          FlsLoader_LengthType Length)
{
  
  uint32                SectorNumber;
  uint32                SectorNumber1;
  uint32                NoOfSectors;
  uint32                SectorMask;
  uint32                Offset;
  uint32                Offset1;
  volatile uint32       FlsRegProconp0;
  FlsLoader_ReturnType  RetError;
  
  RetError = FLSLOADER_E_OK;
  #ifdef  IFX_FLSLOADER_DEBUG
  FlsRegProconp0 = TestFlsloader_DebugMask10;
  #else
  FlsRegProconp0 = FLASH0_PROCONP0.U;
  #endif

  /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of 
  type volatile uint32 to avoid compiler optimization in command 
  cycles*/
  Offset = TargetAddress - FLSLOADER_PFLASH0_START_ADDRESS;
  for(SectorNumber = 0U;
      SectorNumber < FLSLOADER_NUM_OF_PF0_SECTORS;
      SectorNumber++)
  {
    if(Offset < FlsLoader_PFlashSectorOffset[SectorNumber])
    {
      break;
    }
  }
  SectorNumber--;
  TargetAddress = TargetAddress + Length ;
  /*IFX_MISRA_RULE_01_02_STATUS=TargetAddress is defined of 
  type volatile uint32 to avoid compiler optimization in command 
  cycles*/
  Offset1 = TargetAddress - FLSLOADER_PFLASH0_START_ADDRESS;
  for(SectorNumber1 = 0U;
      SectorNumber1 < FLSLOADER_NUM_OF_PF0_SECTORS;
      SectorNumber1++)
  {
    if(Offset1 < FlsLoader_PFlashSectorOffset[SectorNumber1])
    {
      break;
    }
  }
  NoOfSectors = SectorNumber1 - SectorNumber;
  SectorMask = (((uint32)1U << NoOfSectors) - (uint32)1U) << SectorNumber;
  if((SectorMask & FlsRegProconp0) != 0U)
  {
    RetError = FLSLOADER_E_LOCKED;
  }
  return RetError;
}

/*******************************************************************************
** Syntax           :  FlsLoader_ReturnType                                   **
**                     FlsLoader_lEraseLockCheck (                            **
**                                    FlsLoader_AddressType TargetAddress,    **
**                                    uint32 FlashType,                       **
**                                    FlsLoader_LengthType Length)            **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters (in)  : TargetAddress, FlashType, Length                        **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function will return if the sector is protected    **
**                                                                            **
*******************************************************************************/
FlsLoader_ReturnType FlsLoader_lEraseLockCheck (
                          FlsLoader_AddressType TargetAddress,uint32 FlashType,
                          FlsLoader_LengthType Length)
{
  volatile uint32                FlsFproD;
  volatile uint32                FlsProcond2;
  volatile uint32                FlsProcondrpro2;
  volatile uint32                FlsFpro;
  volatile uint32                FlsProconp2;
  FlsLoader_ReturnType           RetError;
  
  RetError = FLSLOADER_E_OK;
  /* Given Address belongs to Data FLASH */
  /*Check if the Target Address passed is UCB protected */
  /* Check if global write protection is installed in DFLASH*/
  if (FlashType == FLSLOADER_DFLASH_BANK0)
  {
    #ifdef  IFX_FLSLOADER_DEBUG
    FlsFproD =  TestFlsloader_DebugMask09;
    FlsProcond2 =  TestFlsloader_DebugMask06;
    FlsProcondrpro2 =  TestFlsloader_DebugMask06;
    #else
    FlsFproD = FLASH0_FPRO.B.PRODISD;
    FlsProcond2 = FLASH0_PROCOND.B.L;
    FlsProcondrpro2 = FLASH0_PROCOND.B.RPRO;
    #endif
    if (FlsFproD != 1U)
    {
      if(FlsProcond2 == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
      if(FlsProcondrpro2 == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
    }
  }
  else
  {
    /*Check if the Target Address passed is UCB protected */
    /* Protection Check Algorithm */
    /* Preprocessor switch to enable/disable the lockcheck functionality */
    /* Check if global write protection is installed */
    #ifdef  IFX_FLSLOADER_DEBUG
    FlsFpro =  TestFlsloader_DebugMask09;
    #else
    FlsFpro = FLASH0_FPRO.B.PRODISP;
    #endif
    if(FlsFpro != 1U)
    {
      #ifdef  IFX_FLSLOADER_DEBUG
      FlsProconp2 =  TestFlsloader_DebugMask06;
      #else
      FlsProconp2 = FLASH0_PROCONP0.B.RPRO;
      #endif
      if (FlsProconp2 == 1U)
      {
        RetError = FLSLOADER_E_LOCKED;
      }
      else
      {
        RetError = FlsLoader_lCheckSectorProt(TargetAddress, Length);
      }
    }
  }
  return RetError;
}

/*******************************************************************************
** Syntax  :          FlsLoader_ReturnType FlsLoader_lCheckSectorProt(        **
**                      uint32 TargetAddress, FlsLoader_LengthType Length)    **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
** Parameters(in)   : TargetAddress, Length                                   **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function will return if the sector is protected    **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lCheckSectorProt(
                                                 uint32 TargetAddress,
                                                 FlsLoader_LengthType Length )
{
  FlsLoader_ReturnType RetError;
  uint32 SectorNumber;
  uint32 Offset;

  Offset = TargetAddress - FLSLOADER_PFLASH0_START_ADDRESS;
  SectorNumber = FlsLoader_lSectorNumber(Offset);
  /* Check if any of the 26 sectors of PF0 are locked for
  write/OTP/WOP protection */
  RetError = FlsLoader_lSectorProtCheck(SectorNumber, Length);

  return RetError;
}

/*******************************************************************************
** Syntax           : FlsLoader_ReturnType FlsLoader_lSectorProtCheck(        **
**                      uint32 SectorNumber,                                  **
**                      FlsLoader_LengthType Length )                         **
**                    )                                                       **
**                                                                            **
** Service ID       : None                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : non - reentrant                                         **
**                                                                            **
**                                                                            **
** Parameters(in)   : SectorNumber, Length                                    **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : FlsLoader_ReturnType                                    **
**                                                                            **
** Description      : This function returns sector locked or not.             **
*******************************************************************************/
IFX_LOCAL_INLINE FlsLoader_ReturnType FlsLoader_lSectorProtCheck(
                                uint32 SectorNumber,FlsLoader_LengthType Length)
{
  FlsLoader_ReturnType RetError;
  uint32               SectorMask;
  uint32               FlsRegProconp0;
  uint32               FlsRegProconWop0;
  uint32               FlsRegProconOtp0;

  /* Init RetError to OK */
  RetError = FLSLOADER_E_OK;

  if(SectorNumber != FLSLOADER_INV)
  {
    #ifdef IFX_FLSLOADER_DEBUG
    FlsRegProconp0 = TestFlsloader_DebugMask10;
    FlsRegProconWop0 = TestFlsloader_DebugMask10;
    FlsRegProconOtp0 = TestFlsloader_DebugMask10;
    #else
    FlsRegProconp0 = FLASH0_PROCONP0.U;
    FlsRegProconWop0 = FLASH0_PROCONWOP0.U;
    FlsRegProconOtp0 = FLASH0_PROCONOTP0.U;

    #endif
      
    SectorMask = (((uint32)1U << Length) - (uint32)1U) << SectorNumber;

    if (((FlsRegProconp0   & SectorMask) != 0U) ||
        ((FlsRegProconOtp0 & SectorMask) != 0U) ||
        ((FlsRegProconWop0 & SectorMask) != 0U))
    {
      RetError = FLSLOADER_E_LOCKED;
    }
  }
  else
  {
    RetError = FLSLOADER_E_NOT_OK;
  }
  
  return (RetError);
}

#endif

#define FLSLOADER_STOP_SEC_WRITE_CODE
  /*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives 
  is allowed only for MemMap.h*/
#include "MemMap.h"

