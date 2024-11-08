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
**  $FILENAME   : Fls_17_Pmu.c $                                             **
**                                                                           **
**  $CC VERSION : \main\111 $                                                **
**                                                                           **
**  $DATE       : 2017-04-20 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION  : This file contains                                        **
**                 - functionality of Flash driver                           **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/******************************************************************************
   Traceability:
                 [cover parentID=SAS_AS_FLS107_FLS159,DS_AS_FLS208,
                  DS_AS_FLS267,DS_AS_FLS006_FLS260_FLS261,DS_NAS_FLS_PR912_1,
                  DS_NAS_FLS_PR912_2,DS_NAS_FLS_PR912_3,DS_NAS_FLS_PR228,
                  DS_NAS_FLS_PR624_PR759_PR760_PR761,DS_NAS_FLS_PR919,
                  DS_NAS_FLS_PR915,DS_AS_FLS002,DS_AS_FLS003,SAS_NAS_ALL_PR749,
                  SAS_NAS_ALL_PR630_PR631,SAS_NAS_ALL_PR470,SAS_NAS_ALL_PR70,
                  SAS_NAS_ALL_PR1652]
                 [/cover]
******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "IfxFlash_reg.h"

#include "IfxSrc_reg.h"

/* Inclusion of Mcal Specific Global Header File */
#include "Mcal.h"

/* Inclusion of Flash header file */
#include "Fls_17_Pmu.h"

/* Inclusion of Flash command cycles file */
#include "Fls_17_Pmu_ac.h"

/* Inclusion of Fls_17_Pmu module's local header file */
#include "Fls_17_Pmu_Local.h"

/*
 the macro FLS_RAM_EMULATION_TEST should be defined
 in Fls_17_Pmu_Cfg.h
 */
#ifdef FLS_RAM_EMULATION_TEST
#if (FLS_RAM_EMULATION_TEST == 1U)
#include "FlsEmulation.h"
#endif
#endif

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/*
For internal debugging, this macro maps the FSR bitfields to RAM for error
simulation
*/
#ifdef IFX_FLS_DEBUG
  extern volatile uint32 TestFls_DebugMask;
#endif
#ifdef IFX_FLS_DEBUG03
  extern volatile uint32 TestFls_DebugMask03;
#endif
#ifdef IFX_FLS_DEBUG04
  extern volatile uint32 TestFls_DebugMask04;
#endif
#ifdef IFX_FLS_DEBUG07
  extern volatile uint32 TestFls_DebugMask07;
#endif
#ifdef IFX_FLS_DEBUG08
  extern volatile uint32 TestFls_DebugMask08;
#endif

#define ZERO_VALUE               (0U)
#define TWO_VALUE                (2U)
#define THREE_VALUE              (3U)
#define FOUR_VALUE               (4U)

#define FLS_OPER_BIT_POSITION    (11U)

/* Linear Adress for data flash bank 0 */
#define DFLASH_BANK0_LINEAR      (FLS_BASE_ADDRESS + FLS_SECTOR_STARTADDRESS)

/* Linear Adress for data flash bank 1 */
#define DFLASH_BANK1_LINEAR      (DFLASH_BANK0_LINEAR + FLS_SECTOR_SIZE)

/* Page size mask used for checking if address is page aligned*/
/* ASSUMPTIONS : Page size is of the format 2^y ; y is a whole number */
#define PAGE_SIZE_MASK        (FLS_PAGE_SIZE-1U)  /* MOD_128 */

/* Protection and sequence errors */
#define FLS_PROER_SQER_ERROR   (0x00003000U)
/* Protection and sequence errors */
#define FLS_PVER_ERROR         (0x02000000U)
/* Operational error */
#define FLS_OPER_ERROR         (0x00000800U)
/* Uncorrectable bit error */
#define FLS_BIT_ERROR          (0x00200000U)
/* Dflash busy status */
#define FLS_D0BUSY             (0x00000002U)
/* FCON register setting */
#define FLS_FCON_SETTING       (0xFFFF803FU)
/* FCON register reset setting */
#define FLS_FCON_RESET_SETTING (0x78FF803FU)
/* Disable traps on Uncorrectable errors */
#define FLS_MARD_TRAP          (0x00008000U)

/* SPND bit in MARD register */
#define FLS_MARD_SPND          (0x00000008U)

/* Word size - 4 bytes */
#define FLS_WORD_SIZE          (4U)
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

#define FLS_17_PMU_START_SEC_CODE
#include "MemMap.h"

static void Fls_lMainErase(uint8 BankType);

static void Fls_lMainWrite(uint8 BankType);

static void Fls_lMainRead(uint8 BankType);

static void Fls_lMainCompare(uint8 BankType);

static void Fls_lErrorHandler(uint8 JobType);

#if(FLS_AC_LOAD_ON_JOB_START == STD_ON)
static void Fls_lLoadAccessCode(uint8 JobRequest);

static void Fls_lUnLoadAccessCode(uint8 JobRequest);
#endif

IFX_LOCAL_INLINE boolean Fls_lSpndTimeout(uint32 TimeOutCount);

IFX_LOCAL_INLINE boolean Fls_lResmErTimeout(uint32 TimeOutCount);

static void Fls_lMainWriteJobStart(uint32 PhysicalAddress);

static void Fls_lMainEraseJobStart(uint32 PhysicalAddress);

IFX_LOCAL_INLINE boolean Fls_lPverChk(void);

#if (FLS_DEV_ERROR_DETECT == STD_ON)
IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Read(const Fls_17_Pmu_StateType* StatePtr);

IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Write(const Fls_17_Pmu_StateType* StatePtr);

IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Erase(const Fls_17_Pmu_StateType* StatePtr);

IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Compare(const Fls_17_Pmu_StateType* 
                                                                     StatePtr);

static uint8 Fls_lUninitDET(const uint8 ServiceID);

static uint8 Fls_lTimeOutDET(const uint8 ServiceID);

IFX_LOCAL_INLINE void Fls_lCalculateWriteTimeoutCount(void);

IFX_LOCAL_INLINE Std_ReturnType Fls_lPageEraseCheck(
                                                   const uint32 * AddrCheckPtr,
                                                   uint32 Length,
                                                   const uint8 ServiceID);

IFX_LOCAL_INLINE Std_ReturnType Fls_lCheckEraseDet(
                                               Fls_AddressType TargetAddress,
                                               Fls_LengthType Length);

IFX_LOCAL_INLINE Std_ReturnType Fls_lCheckWriteDet(
                                                 Fls_AddressType TargetAddress,
                                                 const uint8 *SourceAddressPtr,
                                                 Fls_LengthType Length);

#if (FLS_IFX_FEE_USED == STD_OFF)
IFX_LOCAL_INLINE boolean Fls_lCheckWrittenPage(uint32 PageLength);
#endif

#endif /* End of FLS_DEV_ERROR_DETECT */

/*Check for command sequence error and protection error*/
IFX_LOCAL_INLINE uint32 Fls_lChkSeqProtErrors(void);

/*Check for operational error */
IFX_LOCAL_INLINE uint32 Fls_lChkOperError(void);

/*Check for operational error */
IFX_LOCAL_INLINE uint32 Fls_lChkOperError_initmode(void);

/*Check for double bit errors (DBER) */
IFX_LOCAL_INLINE uint32 Fls_lChkBitErrors(void);

/* Return the Busy status of the Data Flash Bank */
/* FLASH0_FSR.U & (1U<< (1U + (Bank) ) */
IFX_LOCAL_INLINE uint32 Fls_lHWBusyCheck(void);

/* get the write mode , page mode or burst mode */
IFX_LOCAL_INLINE uint8 Fls_lGetWriteMode(void);

/* Get max read length according to the FLS mode */
IFX_LOCAL_INLINE Fls_LengthType Fls_lGetReadModeLength(void);

#define FLS_17_PMU_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#if (FLS_PB_FIXEDADDR == STD_OFF)


#define FLS_17_PMU_START_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
const Fls_17_Pmu_ConfigType *Fls_ConfigPtr;
#define FLS_17_PMU_STOP_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#else

#define FLS_17_PMU_START_SEC_CONST_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
const Fls_17_Pmu_ConfigType * const Fls_ConfigPtr = &Fls_17_Pmu_ConfigRoot[0];
#define FLS_17_PMU_STOP_SEC_CONST_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


#endif

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define FLS_17_PMU_START_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS249,DS_AS_FLS351,DS_AS_FLS014,
   DS_AS_FLS191,DS_AS_FLS086,DS_AS_FLS323_FLS324,DS_AS_FLS268,
   DS_NAS_FLS_PR913,DS_NAS_FLS_PR734_1,DS_AS403_FLS310_1_FLS312_1,
   DS_NAS_HE2_FLS_PR3018,DS_NAS_EP_FLS_PR3018][/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_Init(const Fls_17_Pmu_ConfigType* ConfigPtr)      **
**                                                                            **
** Service ID:  0                                                             **
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
** Description : Driver Module Initialization function                        **
** Service for FLASH initialization. The Initialization function shall        **
** initialize all Flash relevant registers with the values of the structure   **
** referenced by the parameter ConfigPtr.                                     **
**                                                                            **
*******************************************************************************/
void Fls_17_Pmu_Init(const Fls_17_Pmu_ConfigType* ConfigPtr)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint32 FconReg;
  uint32 DummyRetVal;

  #if( FLS_DEV_ERROR_DETECT == STD_ON )
  /*
  FLS015: ConfigPtr shall not be NULL pointer
  */
  #if (FLS_PB_FIXEDADDR == STD_OFF)
  if (ConfigPtr == NULL_PTR)
  #else
  if (ConfigPtr != &Fls_17_Pmu_ConfigRoot[0])
  #endif
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID,FLS_SID_INIT,
                      FLS_E_PARAM_CONFIG);/* Report to DET */
  }   
  /* Check if the driver is BUSY */
  else if( ( ConfigPtr->FlsStateVarPtr->FlsJobType[0] != FLS_NONE) ||
         ( ConfigPtr->FlsStateVarPtr->FlsJobType[1] != FLS_NONE ) )
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID,FLS_SID_INIT,
                      FLS_E_BUSY);/* Report to DET */
  }
  else
  /* FLS_DEV_ERROR_DETECT */
  #endif

  {
    /*  ------------------------------------------------------------------ **
    **                 Flash Module Initialization                         **
    **  ------------------------------------------------------------------ */
    /*
    FLS014: Initialization of flash driver and its relevant registers
    */
    /* Store the configuration pointer */
    #if (FLS_PB_FIXEDADDR == STD_OFF)
    Fls_ConfigPtr = ConfigPtr;
    #else
    Fls_ConfigPtr->FlsStateVarPtr->DriverInitialised = (boolean)TRUE;
    #endif

    /* Clears error flags of FSR register */
    Fls_ResetReadCmdCycle();
    
    /* Enter Critical Section */
    Fls_lSchMEnter_Init();
 
    /* Reset EndInit protection to access EndInit protected registers */
    FLS_SFR_INIT_RESETENDINIT();                                         
 
    /* Clear the wait state setting for DFLash and Set Dflash Waitstates
       according to configuration  */
    FconReg = (uint32)FLS_SFR_INIT_USER_MODE_READ32(FLASH0_FCON.U);

    #if (FLS_RESET_SFR_INIT == STD_ON)
    /* Clear EOBM, PROERM, SQERM, VOPERM and DFlash wait state setting */
    FconReg &= FLS_FCON_RESET_SETTING;
    /* Clear SELD0 */
    FLS_SFR_INIT_USER_MODE_MODIFY32(FLASH0_MARD.U,FLS_MARD_SELDO_CLEARMASK, \
                                    FLS_WRITE_ZERO_SETMASK)
    /* Disable interrupt. Reserve bit access is ensured for SRC_PMU00 register*/
    FLS_SFR_INIT_MODIFY32(SRC_PMU00.U,FLS_SRC_SRE_CLEARMASK, \
                          FLS_WRITE_ZERO_SETMASK)
    #else
    /* Clear DFlash Wait states */
    FconReg &= FLS_FCON_SETTING;
    #endif

    /* Set DFlash Wait states according to configuration. Reserve bit access 
    is ensured for FLASH0_FCON register */
    FconReg |= ((unsigned_int)(ConfigPtr->FlsWaitStates));
    FLS_SFR_INIT_WRITE32(FLASH0_FCON.U,(unsigned_int)FconReg) ;
 
    #if( FLS_USE_INTERRUPTS == STD_ON)
    /* Enable End of Busy Interrupt Mask. Reserve bit access is ensured for 
    FLASH0_FCON register*/
    FLS_SFR_INIT_MODIFY32(FLASH0_FCON.U,FLS_FCON_EOBM_CLEARMASK, \
                                          FLS_FCON_EOBM_SETMASK)
    #endif
 
    /* Disable traps on Uncorrectable errors */
    /* If SPNDERR is set, it will get cleared by writing 1 to itself */
    FLS_SFR_INIT_MODIFY32(FLASH0_MARD.U,FLS_WRITE_ONE_CLEARMASK, FLS_MARD_TRAP)
 
     /* Set End Init Protection */
    FLS_SFR_INIT_SETENDINIT();
 
    /* Exit Critical Section */
    Fls_lSchMExit_Init(); 
 
    #if( FLS_USE_INTERRUPTS == STD_ON)
    /*Enable interrupt. Reserve bit access is ensured for SRC_PMU00 register*/
    FLS_SFR_INIT_MODIFY32(SRC_PMU00.U,FLS_SRC_SRE_CLEARMASK, \
                                      FLS_SRC_SRE_SETMASK)
    #endif
    
    /*  ------------------------------------------------------------------ **
    **                 Global Variable Initialization                      **
    **  ------------------------------------------------------------------ */
    /*
     After having finished the module initialization,
     the Flash Driver state shall be set to FLS_IDLE and and the jobs result
     shall be set to FLS_JOB_OK.
    */
    StatePtr = ConfigPtr->FlsStateVarPtr;
 
    /*Set JobResult to ok  */
    StatePtr->FlsJobResult = MEMIF_JOB_OK;
 
    /* Setting the Jobtype of bank0,1 to none */
    StatePtr->FlsJobType[BANK0] = FLS_NONE;
    StatePtr->FlsJobType[BANK1] = FLS_NONE;

    /* Initialize Job Status */
    StatePtr->JobStarted.Write = 0U;
    StatePtr->JobStarted.Erase = 0U;
    
    /* Assign default mode */
    StatePtr->FlsMode = Fls_lSetDefaultMode();

    /* Initialize OPER status */
    StatePtr->FlsOper = 0U;

    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    /* Enable timeout handling */
    StatePtr->FlsTimeoutControl = 1U;
    #endif

    /* Check for OPER */
    DummyRetVal = Fls_lChkOperError_initmode();

    /* FlsOper will be updated by Fls_lChkOperError() */
    UNUSED_PARAMETER(DummyRetVal)
  }
}/* End of Function: Fls_Init */

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS250_FLS001_1,DS_AS_FLS218_221,
   DS_AS_FLS327,DS_AS_FLS328_FLS329,DS_AS_FLS330,DS_AS_FLS145,DS_AS_FLS209_1,
   DS_AS_FLS020_FLS021_FLS065_FLS023,DS_NAS_FLS_PR734_2,DS_NAS_FLS_PR1058_1,
   DS_AS403_FLS310_2_FLS311_1_FLS312_2]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_Erase( Fls_AddressType TargetAddress,   **
**                                    Fls_LengthType Length                   **
**                                  )                                         **
**                                                                            **
** Service ID:  1                                                             **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  TargetAddress and Length                                **
**                                                                            **
** Parameters (out): E_OK: command has been accepted                          **
**                   E_NOT_OK: command has not been accepted                  **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This API Erases Flash. It copies the given parameters to     **
** driver internal variables, initiates the erase job, sets the driver status **
** to MEMIF_BUSY and job result to MEMIF_JOB_PENDING.                         **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_Erase( Fls_AddressType TargetAddress,
                          Fls_LengthType Length
                        )
{
  Fls_17_Pmu_StateType*   StatePtr;
  volatile uint32  PhysicalAddress;
  uint8            JobType[2U];
  Std_ReturnType   ReturnValue;

  /* check for development error */
  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
  if(Fls_lCheckEraseDet(TargetAddress, Length) == E_NOT_OK)
  {
    ReturnValue = E_NOT_OK;
  }
  else
  /* FLS_DEV_ERROR_DETECT */
  #endif

  {
    /* Check for OPER error */
    if(Fls_lChkOperError() == 0U)
    {
      ReturnValue = E_OK;
    
      /* Initialise Local variables */
      StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

      /* By Default set the physical address to bank0 */
      PhysicalAddress = DFLASH_BANK0_PHYSICAL;
    
      JobType[0] = FLS_NONE;
      JobType[1] = FLS_NONE;
    
      /* Check for the bank type */
      if((TargetAddress + Length) <= FLS_SECTOR_SIZE)
      {
        /* Set the job type of Bank0 to Flash erase */
        JobType[0] = FLS_ERASE;
      }
      else if(TargetAddress == FLS_SECTOR_SIZE)
      {
        /* Set the job type of Bank1 to Flash erase */
        JobType[1] = FLS_ERASE;
        /*Set the physical address to bank1 */
        PhysicalAddress = DFLASH_BANK1_PHYSICAL;
      }
      else
      {
        /* Set the job type of both the Bank's to Flash erase */
        JobType[0] = FLS_ERASE;
        JobType[1] = FLS_ERASE;
      }
     
      #if(FLS_AC_LOAD_ON_JOB_START == STD_ON )
      Fls_lLoadAccessCode(FLS_ERASE);
      #endif
       
      /* Set the flash job result to job pending */
      StatePtr->FlsJobResult = MEMIF_JOB_PENDING;
       
      /* Update the new job on affected banks */
      /* The Bit OR operation is used as either s->FlsJobType[x] or */
      /* NewJobType[x] must be FLS_NONE else BUSY DET would have occured*/
      StatePtr->FlsJobType[0] |= JobType[0];
      StatePtr->FlsJobType[1] |= JobType[1];
    
      #if (FLS_DEV_ERROR_DETECT == STD_ON)
      if (StatePtr->FlsTimeoutControl != 0U)
      {
        /* Update Timeout count for erase */
        StatePtr->FlsEraseTimeoutCycleCount = 
          (uint32)(FLS_17_PMU_ERASE_TIME / Fls_ConfigPtr->FlsCallCycle);
        if((FLS_17_PMU_ERASE_TIME % Fls_ConfigPtr->FlsCallCycle) != 0U)
        {
          StatePtr->FlsEraseTimeoutCycleCount++;
        }
        /* for erasing 2 sectors the count has to be doubled */
        if((JobType[0] == FLS_ERASE) && (JobType[1] == FLS_ERASE))
        {
          StatePtr->FlsEraseTimeoutCycleCount *= 2U;
        }
      }
      #endif /* FLS_DEV_ERROR_DETECT == STD_ON */

      /* Check for hardware status */
      if(Fls_lHWBusyCheck() == 0U)
      {
        /* Indicate that the ERASE Job has been started */
        StatePtr->JobStarted.Erase = 1U;
    
        /* FLS_RAM_EMULATION_TEST */
        #ifdef FLS_RAM_EMULATION_TEST
        #if (FLS_RAM_EMULATION_TEST == 1U)
        if(FlsEmulation_EraseSyncError() != 0)
        {
          (void) FlsEmulation_Erase(PhysicalAddress);
        }
        else
        #endif
        #endif
        {
          /* Enter Critical Section */
          Fls_lSchMEnter_Erase();
          
          /* Execute the ERASE COMMAND CYCLES */
          Fls_CallEraseCommand(PhysicalAddress);
          
          /* Exit Crtical Section */
          Fls_lSchMExit_Erase();
        }
    
        /* Check for Sequence error,Protection error  */
        if(Fls_lChkSeqProtErrors() != 0U)
        {
          ReturnValue = E_NOT_OK;
  
          /* Report Production errors to DEM */
          Fls_lDemReportError_Erase();
   
          Fls_lErrorHandler(FLS_ERASE);    
        }
        /* Check for command sequence Timeout */
        else if (StatePtr->FlsTimeoutErr == FLS_ERASE)
        {
          ReturnValue = E_NOT_OK;
          /* As per AI00157728 - timeout error need not be reported to DEM */
          Fls_lErrorHandler(FLS_ERASE);
        }
        else
        {
          /* dummy else */
        }
      }
    }
    else
    {
      /* Job cannot be started because of OPER error */
      ReturnValue = E_NOT_OK;
    }
  }

  return(ReturnValue);
}


/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS223,DS_AS_FLS226,
DS_AS_FLS251_FLS001_2, DS_AS_FLS331_FLS332_FLS333,DS_AS_FLS334,DS_AS_FLS146,
DS_AS_FLS026_FLS027_FLS030_FLS066_FLS157,DS_AS_FLS209_2,
DS_NAS_FLS_PR734_3,DS_NAS_FLS_PR1058_2,DS_NAS_FLS_PR1068_1,DS_NAS_FLS_PR1068_2,
DS_AS403_FLS310_3_FLS311_2_FLS312_3]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_Write( Fls_AddressType TargetAddress,   **
**                                    const uint8 *SourceAddressPtr,          **
**                                    Fls_LengthType Length                   **
**                                  )                                         **
**                                                                            **
** Service ID:  2                                                             **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :SourceAddressPtr,TargetAddress and Length                 **
**                                                                            **
** Parameters (out): E_OK: command has been accepted                          **
**                   E_NOT_OK: command has not been accepted                  **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : Writing to given flash address.                              **
** Service for writing one or more flash pages. This service copies the given **
** parameters to driver internal variables, initiate a Write job, set driver  **
** status to MEMIF_BUSY and job result to MEMIF_JOB_PENDING.                  **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_Write( Fls_AddressType TargetAddress,
                          const uint8 *SourceAddressPtr,
                          Fls_LengthType Length
                        )
{
  Fls_17_Pmu_StateType   *StatePtr;
  volatile uint32 PhysicalAddress;
  uint32          *PageStartAddressPtr;
  Std_ReturnType  RetVal;
  uint8           WriteMode;
  uint8           JobType[2];

  RetVal = E_OK;

  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
  if(Fls_lCheckWriteDet(TargetAddress,SourceAddressPtr,Length) == E_NOT_OK)
  {
    RetVal = E_NOT_OK;
  }
  else
  #endif
  {
    /* Initialise Local variables */
    StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
    JobType[0] = FLS_NONE;
    JobType[1] = FLS_NONE;

    /* PVER check for IFX FEE, clear FlsPver */
    #if(FLS_VER_ERROR_CHECK == STD_ON)
    #if(FLS_IFX_FEE_USED == STD_ON)
    StatePtr->FlsPver = 0U;
    #endif
    #endif

    /* Check for whether write operation is only for Data Bank0 */
    if((TargetAddress + Length) <= FLS_SECTOR_SIZE)
    {
      /*Assign jobtype of Data Bank0 to write */
      JobType[0] = FLS_WRITE;
  
      /* Determine the page start address */
      /*IFX_MISRA_RULE_11_01_STATUS=TargetAddress is converted to pointer*/
      /*IFX_MISRA_RULE_11_03_STATUS=TargetAddress is converted to pointer*/
      PageStartAddressPtr = (uint32*)(FLS_BASE_ADDRESS + TargetAddress);
  
      /* Setting the PhysicalAddress to DataBank0 */
      PhysicalAddress = DFLASH_BANK0_PHYSICAL;
    }
    /* Check for whether write operation is only for Data Bank1 */
    else if((TargetAddress) >= FLS_SECTOR_SIZE)
    {
      /*Assign jobtype of Data Bank1 to write */
      JobType[BANK1] = FLS_WRITE;
      /* Setting the PageStartAddress to DataBank1 PageAddress */
      /*IFX_MISRA_RULE_11_01_STATUS=TargetAddress is converted to pointer*/
      /*IFX_MISRA_RULE_11_03_STATUS=TargetAddress is converted to pointer*/
      PageStartAddressPtr = (uint32*)( FLS_BASE_ADDRESS +
                                   (TargetAddress - FLS_SECTOR_SIZE) +
                                   FLS_OFFSET );
  
      /* Setting the PhysicalAddress to DataBank1 */
      PhysicalAddress = DFLASH_BANK1_PHYSICAL;
    }
    else
    {
      /* Assign jobtype of both the banks to write */
      JobType[BANK0] = FLS_WRITE;
      JobType[BANK1] = FLS_WRITE;
  
      /* Determine the page start address */
      /*IFX_MISRA_RULE_11_01_STATUS=TargetAddress is converted to pointer*/
      /*IFX_MISRA_RULE_11_03_STATUS=TargetAddress is converted to pointer*/
      PageStartAddressPtr = (uint32*)(FLS_BASE_ADDRESS + TargetAddress);
  
      /* Setting the PhysicalAddress to DataBank0 */
      PhysicalAddress = DFLASH_BANK0_PHYSICAL;
    }
  
  /************************* DET **********************************/
    /* Check for the BUSY DET */
    #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
    /*If DET occurred then return without executing the routine.*/
    if(Fls_lBusyDET_Write(StatePtr) == 1U)
    {
      RetVal = E_NOT_OK;
    } 
    else
    #endif /* FLS_DEV_ERROR_DETECT */
  /***************************************************************************/
    {
      /* Check OPER error status */
      if(Fls_lChkOperError() == 0U)
      {
        #if( FLS_AC_LOAD_ON_JOB_START == STD_ON)
        Fls_lLoadAccessCode(FLS_WRITE);
        #endif
      
        /* 1. Set the flash job result to job pending */
        StatePtr->FlsJobResult = MEMIF_JOB_PENDING;
      
        /* By Default Setting the PageStartAddress to DataBank0 PageAddress */
        /*IFX_MISRA_RULE_11_01_STATUS=Pointer to Flash location is stored in 
          global uint32 variable*/
        /*IFX_MISRA_RULE_11_03_STATUS=Pointer to Flash location is stored in 
          global uint32 variable*/
        StatePtr->FlsWriteAddress = (uint32)PageStartAddressPtr;
      
        /* Assigning Length to global variable FlsWriteLength*/
        StatePtr->FlsWriteLength = Length;
      
        /* Assigning SourceAddressPtr to Global variable FlsWriteBufferPtr */
        StatePtr->FlsWriteBufferPtr = SourceAddressPtr;
      
        /* 2. Update the new job on affected banks */
        /* The Bit OR operation is used as either s->FlsJobType[x] */
        /*  or NewJobType[x] must be FLS_NONE (0) else BUSY DET would have 
            occured */
        StatePtr->FlsJobType[0] |= JobType[0];
        StatePtr->FlsJobType[1] |= JobType[1];
    
        #if (FLS_DEV_ERROR_DETECT == STD_ON)
        if (StatePtr->FlsTimeoutControl != 0U)
        {
          Fls_lCalculateWriteTimeoutCount();
        }
        #endif /* FLS_DEV_ERROR_DETECT == STD_ON */

        if(Fls_lHWBusyCheck() == 0U)
        {
          #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
          #if (FLS_IFX_FEE_USED == STD_OFF)
          if(Fls_lPageEraseCheck(PageStartAddressPtr,Length,FLS_SID_WRITE) == 
                                                                 E_NOT_OK)
          {
            Fls_lErrorHandler(FLS_WRITE);
            RetVal = E_NOT_OK;
          }
          else  
          #endif /*FLS_IFX_FEE_USED*/
          #endif /* FLS_DEV_ERROR_DETECT */
          {
            /* Indicate that the WRITE Job has started */
            StatePtr->JobStarted.Write = 1U;
            
            /* Enter Critical Section */
            Fls_lSchMEnter_Write(); 
            
            /* Execute the Write command sequence */
            WriteMode = Fls_lGetWriteMode();
            Fls_CallWriteCommand(PhysicalAddress,StatePtr,WriteMode);
            
            /* Exit Crtical Section */
            Fls_lSchMExit_Write(); 
            
            #ifdef FLS_RAM_EMULATION_TEST
            #if  (FLS_RAM_EMULATION_TEST == 1U)
            (void) FlsEmulation_Write(StatePtr->FlsWriteAddress,
                               StatePtr->FlsWriteBufferPtr);
            #endif
            #endif /* FLS_RAM_EMULATION_TEST */
            
            /* Check for Sequence error,Protection error */
            if(Fls_lChkSeqProtErrors() != 0U)
            {
             /* Report Production errors to DEM */
              Fls_lDemReportError_Write();
              /* Call to Fls_lErrorHandler local function */
              Fls_lErrorHandler(FLS_WRITE);
              RetVal = E_NOT_OK;
            }
            /* Check for command cycle Timeout */
            else if (StatePtr->FlsTimeoutErr == FLS_WRITE)
            {
            /* As per AI00157728 - timeout error need not be reported to DEM */
              /* Call to Fls_lErrorHandler local function */
              Fls_lErrorHandler(FLS_WRITE);
              RetVal = E_NOT_OK;
            } 
            else
            {
              /* dummy else */
            }
          }
        }
      }
      else
      {
        RetVal = E_NOT_OK;
      }
    }
  }
  return(RetVal);
}


#if (FLS_COMPARE_API == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS257,DS_AS_FLS241,DS_AS_FLS244,
DS_AS_FLS150_FLS151_FLS152_FLS153_FLS273,DS_NAS_FLS_PR734_4,DS_AS_FLS209_3,
DS_AS403_FLS341_FLS342_FLS343,DS_AS403_FLS344,
DS_AS403_FLS310_4_FLS311_3_FLS312_4]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_Compare( Fls_AddressType SourceAddress, **
**                                      const uint8 *TargetAddressPtr,        **
**                                      Fls_LengthType Length                 **
**                                    );                                      **
**                                                                            **
** Service ID:  8                                                             **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :SourceAddressPtr,TargetAddress and Length                 **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    :  E_OK: command has been accepted                         **
**                    E_NOT_OK: command has not been accepted                 **
**                                                                            **
** Description : Service for comparing the contents of an area of flash memory**
** with that of an application data buffer. This service shall copy the given **
** parameters to driver internal variables, initiate a compare job, set the   **
** driver status to MEMIF_BUSY, set the job result to MEMIF_JOB_PENDING       **
** and return with E_OK.                                                      **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_Compare( Fls_AddressType SourceAddress,
                            const uint8 *TargetAddressPtr,
                            Fls_LengthType Length
                           )
{
  Fls_17_Pmu_StateType   *StatePtr;
  volatile uint32 PhysicalAddress;
  uint8           JobType[2];
  Std_ReturnType  ReturnValue;
  /* Initialise Local variables */
/****************************** DET *******************************************/
  #if( FLS_DEV_ERROR_DETECT == STD_ON )
  /*
  FLS015: API  called before initialization
  */
  if(Fls_lUninitDET(FLS_SID_COMPARE) == 1U)
  {
    ReturnValue = E_NOT_OK;
  }

  /*
  FLS026: Checking whether given SourceAddress is valid or not
  */
  else if( SourceAddress >= FLS_TOTAL_SIZE )
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, FLS_17_PMU_INSTANCE_ID,
                     FLS_SID_COMPARE,
                     FLS_E_PARAM_ADDRESS
                   );/* Report to DET */

    ReturnValue = E_NOT_OK;
  }

  /*
  FLS004: Check for Pointer to an application buffer
  */
  else if(TargetAddressPtr == NULL_PTR)
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, FLS_17_PMU_INSTANCE_ID,
                   FLS_SID_COMPARE,
                   FLS_E_PARAM_DATA
                 );/* Report to DET */

    ReturnValue = E_NOT_OK;
  }
  /*
  FLS027: Checking whether given Length is valid or not
  */
  else if((Length == 0U) || ((SourceAddress + Length) > FLS_TOTAL_SIZE))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, FLS_17_PMU_INSTANCE_ID,
                     FLS_SID_COMPARE,
                     FLS_E_PARAM_LENGTH
                   );/* Report to DET */

    ReturnValue = E_NOT_OK;
  }
  /* Checking whether FLS is Busy */
  else
 #endif /* FLS_DEV_ERROR_DETECT == STD_ON */
/******************************************************************************/
  {
    /* Initialise Local variables */
    StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
    JobType[0] = FLS_NONE;
    JobType[1] = FLS_NONE;
    ReturnValue = E_OK;
  
    /* Check for whether read operation is only for Data Bank0 */
    if((SourceAddress + Length) <= FLS_SECTOR_SIZE)
    {
      /*Assign jobtype of Data Bank0 to Compare */
      JobType[0] = FLS_COMPARE;
      PhysicalAddress = (uint32)(FLS_BASE_ADDRESS + SourceAddress);
    }
    /* Check for whether read operation is only for Data Bank1 */
    else if(SourceAddress >= FLS_SECTOR_SIZE)
    {
      /*Assign jobtype of Data Bank1 to read */
      JobType[1] = FLS_COMPARE;
      PhysicalAddress = (uint32)( FLS_BASE_ADDRESS +
                                  (SourceAddress - FLS_SECTOR_SIZE) +
                                  FLS_OFFSET
                                );
    }
    else
    {
      /*Assign jobtype of both the banks to read */
      JobType[0] = FLS_COMPARE;
      JobType[1] = FLS_COMPARE;
      PhysicalAddress = (uint32)(FLS_BASE_ADDRESS + SourceAddress);
  
    }
  
  /************************* DET *******************************/
  
    /* Check for the BUSY DET */
    #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
    /*If DET occurred then return without executing the routine.*/
    if(Fls_lBusyDET_Compare(StatePtr) == 1U)
    {
      ReturnValue = E_NOT_OK;
    }
    else
    #endif
  /****************************************************************************/
    {
      /* 1. Set the flash job result to job pending */
      StatePtr->FlsJobResult = MEMIF_JOB_PENDING;
    
      /*IFX_MISRA_RULE_11_05_STATUS=The Read Buffer is also used for Compare 
        as parallel READ and COMPARE operations are NOT supported. 
        Hence TargetAddressPtr is typecasted to uint8* and assigned to 
        FlsReadBufferPtr. It is ensured that the contents pointed by 
        TargetAddressPtr are not modified*/
      StatePtr->FlsReadBufferPtr = (uint8 *)(void*)TargetAddressPtr;
    
      /* Assigning Length to Global variable FlsReadLength*/
      StatePtr->FlsReadLength = Length;
    
      StatePtr->FlsReadAddress = PhysicalAddress;
    
      /*3. Update the new job on affected banks */
      /* The Bit OR operation is used as either s->FlsJobType[x] */
      /*  or NewJobType[x] must be FLS_NONE (0) else 
         BUSY DET would have occured */
      StatePtr->FlsJobType[0] |= JobType[0];
      StatePtr->FlsJobType[1] |= JobType[1];
    }
  }
  return(ReturnValue);
}
/* FLS_COMPARE_API == STD_ON */
#endif


#if  (FLS_CANCEL_API  ==  STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS252,DS_AS_FLS229,DS_AS_FLS230,
DS_AS_FLS335,DS_AS_FLS336,DS_AS_FLS033,DS_AS_FLS147,DS_AS_FLS348,
DS_NAS_FLS_PR734_6,DS_AS403_FLS356]
                 [/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_Cancel(Void)                                      **
**                                                                            **
** Service ID:  3                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description :                                                              **
** Service for Canceling an ongoing flash job.                                **
** Read and Compare jobs will be cancelled immediately. Write and Erase jobs  **
** will be cancelled in driver immediately but hardware might still be busy   **
** until the ongoing operation is completed for the already issued commands   **
*******************************************************************************/
void Fls_17_Pmu_Cancel(void)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* ArrPtr;
  boolean JobCanceled;

  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
  /*FLS015: API called before initialization */
  if (Fls_lUninitDET(FLS_SID_CANCEL) == 0U)
  #endif
  {
    StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
    ArrPtr = StatePtr->FlsJobType;
    JobCanceled = (boolean)FALSE;
  
    /* Cancel WRITE Job */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    if((FLS_WRITE == ArrPtr[0U]) || (FLS_WRITE == ArrPtr[1U]))
    {
      /* Set the JobType to NONE; 
         This shall be done before calling Fls_lUnLoadAccessCode() */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      ArrPtr[0U] = FLS_NONE;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      ArrPtr[1U] = FLS_NONE;

      #if (FLS_AC_LOAD_ON_JOB_START == STD_ON)
      Fls_lUnLoadAccessCode(FLS_WRITE);
      #endif

      StatePtr->FlsJobResult = MEMIF_JOB_CANCELED;

      JobCanceled = (boolean)TRUE;
    }
    /* Cancel ERASE Job */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    else if((FLS_ERASE == ArrPtr[0U]) || (FLS_ERASE == ArrPtr[1U]))
    {
      /* Set the JobType to NONE;
         This shall be done before calling Fls_lUnLoadAccessCode() */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      ArrPtr[0U] = FLS_NONE;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      ArrPtr[1U] = FLS_NONE;

      #if (FLS_AC_LOAD_ON_JOB_START == STD_ON)
      Fls_lUnLoadAccessCode(FLS_ERASE);
      #endif

      StatePtr->FlsJobResult = MEMIF_JOB_CANCELED;

      JobCanceled = (boolean)TRUE;
    }
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    else if((FLS_READ == ArrPtr[0U])    || (FLS_READ == ArrPtr[1U])  ||
            (FLS_COMPARE == ArrPtr[0U]) || (FLS_COMPARE == ArrPtr[1U]))
    {
      /* Cancel READ / COMPARE Job */
      /* Set the JobType to NONE */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
      ArrPtr[0U] = FLS_NONE;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      ArrPtr[1U] = FLS_NONE;

      StatePtr->FlsJobResult = MEMIF_JOB_CANCELED;

      JobCanceled = (boolean)TRUE;
    }
    else
    {
      /* Set the JobType to NONE */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      ArrPtr[0U] = FLS_NONE;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      ArrPtr[1U] = FLS_NONE;
    }
  
    /* Reset JobStartStatus */
    StatePtr->JobStarted.Write = 0U;
    StatePtr->JobStarted.Erase = 0U;
  
    StatePtr->NotifCaller = FLS_CANCEL_JOB;
  
    if((JobCanceled == (boolean)TRUE) &&
       (Fls_ConfigPtr->FlsJobErrorNotificationPtr != NULL_PTR)
      )
    {
      /* FlsJobErrorNotificationPtr should be called */
      (Fls_ConfigPtr->FlsJobErrorNotificationPtr)();
    }
  
    StatePtr->NotifCaller = FLS_NO_JOB;
  }
}

#endif /* #if (FLS_CANCEL_API == STD_ON) */

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR1084_PR123_7]
                 [/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_CancelNonEraseJobs(void)                          **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description :                                                              **
** Service for Canceling an ongoing flash jobs except the erase job.          **
** Read and Compare jobs will be cancelled immediately. Write job             **
** will be cancelled in driver immediately but hardware might still be busy   **
** until the ongoing operation is completed for the already issued commands   **
*******************************************************************************/
void Fls_17_Pmu_CancelNonEraseJobs(void)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* ArrPtr;
  boolean JobCanceled;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  ArrPtr = StatePtr->FlsJobType;
  JobCanceled = (boolean)FALSE;

  /* Cancel non-erase job on Bank 0 & 1 */

  /* Cancel WRITE Job */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if((FLS_WRITE == ArrPtr[0U]) || (FLS_WRITE == ArrPtr[1U]))
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    ArrPtr[0U] = FLS_NONE;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    ArrPtr[1U] = FLS_NONE;
    #if (FLS_AC_LOAD_ON_JOB_START == STD_ON)
    Fls_lUnLoadAccessCode(FLS_WRITE);
    #endif

    StatePtr->FlsJobResult = MEMIF_JOB_CANCELED;

    JobCanceled = (boolean)TRUE;

    /* Reset JobStart Status */
    StatePtr->JobStarted.Write = 0U;

  }
  /* Cancel READ / COMPARE Job */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  else if((FLS_READ == ArrPtr[0U])    || (FLS_READ == ArrPtr[1U]) ||
          (FLS_COMPARE == ArrPtr[0U]) || (FLS_COMPARE == ArrPtr[1U]))
  {
      
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/  
    ArrPtr[0U] = FLS_NONE;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    ArrPtr[1U] = FLS_NONE;

    StatePtr->FlsJobResult = MEMIF_JOB_CANCELED;

    JobCanceled = (boolean)TRUE;
  }
  else
  {
    /* Reset JobType of Non-Erase Job */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    ArrPtr[0] = ((ArrPtr[0] == FLS_ERASE) ? FLS_ERASE : FLS_NONE);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    ArrPtr[1] = ((ArrPtr[1] == FLS_ERASE) ? FLS_ERASE : FLS_NONE);
  }

  /* Set the NotifCaller to Cancel Job */
  Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_CANCEL_JOB;

  if((JobCanceled == (boolean)TRUE) &&
     (Fls_ConfigPtr->FlsJobErrorNotificationPtr != NULL_PTR)
    )
  {
    /* FlsJobErrorNotificationPtr should be called */
    (Fls_ConfigPtr->FlsJobErrorNotificationPtr)();
  }
  Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_NO_JOB;

}

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS258,DS_AS_FLS156,DS_AS_FLS155,
DS_NAS_FLS_PR734_7]
                 [/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_SetMode(MemIf_ModeType Mode)                      **
**                                                                            **
** Service ID:  9                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : Mode MEMIF_MODE_SLOW:                                    **
**                        Slow read access                                    **
**                                                                            **
**                        MEMIF_MODE_FAST:                                    **
**                        Fast read access                                    **
**                                                                            **
** Parameters (out): : None                                                   **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This API sets the Flash driver's operation mode.             **
**               FAST or SLOW mode impacts only Read/Compare operations.      **
**               Based on the currently active mode, the number of bytes to be**
**               read from Flash in each call of Fls_17_Pmu_MainFunction is   **
**               taken from the configuration                                 **
*******************************************************************************/
#if  (FLS_SET_MODE_API  ==  STD_ON)
void Fls_17_Pmu_SetMode(MemIf_ModeType Mode)
{
  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
  uint8 *ArrPtr;
  
  ArrPtr = Fls_ConfigPtr->FlsStateVarPtr->FlsJobType;

  /* Check if the driver is BUSY */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if( (ArrPtr[0] != FLS_NONE) ||
      (ArrPtr[1] != FLS_NONE)
    )
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                             FLS_17_PMU_INSTANCE_ID,
                             FLS_SID_SETMODE,
                             FLS_E_BUSY);/* Report to DET */
  }
  else
  #endif 
  {  
    Fls_ConfigPtr->FlsStateVarPtr->FlsMode = Mode;
  }
}
#endif

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS269,DS_AS_FLS255,DS_AS_FLS038,
DS_AS_FLS037_FLS212,DS_AS_FLS040,DS_AS_FLS345,DS_AS_FLS346,DS_AS_FLS039,
DS_AS_FLS117,DS_AS_FLS272_FLS359_FLS360,DS_AS403_FLS361,
DS_AS_FLS196,DS_AS_FLS220_FLS225_FLS238_FLS243,DS_NAS_FLS_PR734_8]
                 [/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_MainFunction(void)                                **
**                                                                            **
** Service ID:  6                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out): : None                                                   **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description :                                                              **
** This service shall perform the processing of the flash read,write          **
**         and erase jobs                                                     **
*******************************************************************************/
void Fls_17_Pmu_MainFunction(void)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8 *ArrPtr;
#if(FLS_USE_INTERRUPTS == STD_OFF)
  uint32 FlashFsrStatus;
#endif
  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
  uint8 ErrorUninitFlag;
  uint8 ErrorTimeoutFlag;
  
  /*FLS015: API  called before initialization */
  ErrorUninitFlag = Fls_lUninitDET(FLS_SID_MAIN);
  if (ErrorUninitFlag == 0U)
  {
    ErrorTimeoutFlag = Fls_lTimeOutDET(FLS_SID_MAIN);
    if (ErrorTimeoutFlag == 0U)
  #endif
    {
      StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
      ArrPtr = StatePtr->FlsJobType;

      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
        contents*/
      if( (ArrPtr[BANK0] != FLS_NONE) ||
          (ArrPtr[BANK1] != FLS_NONE)
        )
      {
      #if(FLS_USE_INTERRUPTS == STD_OFF)
 /******************************** ERASE *************************************/
             /* Read the FSR.SPND bit of status register */
         FlashFsrStatus = (uint32)(((unsigned_int)FLASH0_FSR.U & \
                                      FLS_FSR_SPND_MASK) >> \
                                                FLS_FSR_SPND_BITPOS);
        /* Check for erase operation is pending in either of the bank  and 
             Flash is not in suspended state */
        if((Fls_lHWBusyCheck() == 0U) && (FlashFsrStatus == 0U))
        {
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          if(ArrPtr[BANK0] == FLS_ERASE)
          {
            /* Fls_Main_Erase operation should be called  */
            Fls_lMainErase((uint8)BANK0);
          }
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          else if(ArrPtr[BANK1] == FLS_ERASE)
          {
            /* Fls_Main_Erase operation should be called  */
            Fls_lMainErase((uint8)BANK1);
          }
          else{/* To avoid MISRA warning */}
        }
 
 /******************************** WRITE ************************************/
        /* Check for write operation is pending in either of the bank */
        if( Fls_lHWBusyCheck() == 0U)
        {
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          if(ArrPtr[BANK0] == FLS_WRITE)
          {
            /* Fls_Main_Write operation should be called  */
            Fls_lMainWrite((uint8)BANK0);
          }
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          else if(ArrPtr[BANK1] == FLS_WRITE)
          {
            /* Fls_Main_Write operation should be called  */
            Fls_lMainWrite((uint8)BANK1);
          }
          else{/* To avoid MISRA warning */}
        }
 
      /* FLS_USE_INTERRUPTS == STD_OFF */
      #endif
 /******************************** READ **************************************/
        /* Check for read operation is pending in either of the bank */
        if( Fls_lHWBusyCheck() == 0U)
        {
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          if(ArrPtr[BANK0] == FLS_READ)
          {
            /* Fls_Main_Read operation should be called  */
            Fls_lMainRead((uint8)BANK0);
          }
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          else if(ArrPtr[BANK1] == FLS_READ)
          {
            /* Fls_Main_Read operation should be called  */
            Fls_lMainRead((uint8)BANK1);
          }
          else{/* To avoid MISRA warning */}            
        }
 
 /****************************** COMPARE ************************************/
        /* Check for Compare operation is pending in bank 0 */
        if( Fls_lHWBusyCheck() == 0U)
        {
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          if(ArrPtr[BANK0] == FLS_COMPARE)
          {
            /* Fls_Main_Compare operation should be called  */
            Fls_lMainCompare((uint8)BANK0);
          }
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          else if(ArrPtr[BANK1] == FLS_COMPARE)
          {
            /* Fls_Main_Compare operation should be called  */
            Fls_lMainCompare((uint8)BANK1);
          }
          else{/* To avoid MISRA warning */}
        }
      }
    }
  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )  
  }
  #endif
}

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS236,DS_AS_FLS239,DS_AS_FLS209_4,
DS_AS_FLS097_FLS098_FLS099_FLS100_FLS158,DS_NAS_FLS_PR734_5,
DS_AS_FLS256_FLS001_3,DS_AS403_FLS337_FLS338_FLS339,DS_AS403_FLS340,
DS_AS403_FLS310_5_FLS311_4_FLS312_5]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_Read( Fls_AddressType SourceAddress,    **
**                                   uint8 *TargetAddressPtr,                 **
**                                   Fls_LengthType Length                    **
**                                 );                                         **
**                                                                            **
** Service ID:  7                                                             **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : SourceAddress,TargetAddressPtr and Length                **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    :  E_OK: command has been accepted                         **
**                    E_NOT_OK: command has not been accepted                 **
**                                                                            **
** Description : Service for reading from flash memory. This service copies   **
** the given parameters to driver internal variables, set driver status to    **
** MEMIF_BUSY and job result to MEMIF_JOB_PENDING.                            **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_Read(Fls_AddressType SourceAddress,
                               uint8 *TargetAddressPtr,
                               Fls_LengthType Length
                              )
{
  Fls_17_Pmu_StateType  *StatePtr;
  volatile uint32 PhysicalAddress;
  Std_ReturnType ReturnValue;
  uint8 JobType[2];

/**************************** DET *********************************************/
  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
  /* FLS015: API  called before initialization */
  if(Fls_lUninitDET(FLS_SID_READ) == 1U)
  {
    ReturnValue = E_NOT_OK;
  }
 /* FLS097: Checking whether given SourceAddress is valid or not */
  else if(SourceAddress >= (FLS_TOTAL_SIZE))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID,FLS_SID_READ,
                     FLS_E_PARAM_ADDRESS);/* Report to DET */

    ReturnValue = E_NOT_OK;
  }
  /*Check for Pointer to an application buffer*/
  else if(TargetAddressPtr == NULL_PTR)
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                    FLS_17_PMU_INSTANCE_ID,FLS_SID_READ,
                     FLS_E_PARAM_DATA);/* Report to DET */

    ReturnValue = E_NOT_OK;
  }
  /* FLS098: Checking whether given Length is valid or not */
  else if((Length == 0U) || ((SourceAddress+Length) > (FLS_TOTAL_SIZE)))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID, FLS_SID_READ,
                     FLS_E_PARAM_LENGTH);/* Report to DET */

    ReturnValue = E_NOT_OK;
  }
  else
  #endif
/*****************************************************************************/
  {
    /* Initialise Local variables */
    StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
    ReturnValue = (Std_ReturnType)E_OK;
  
    JobType[0] = FLS_NONE;
    JobType[1] = FLS_NONE;
  
    /* Check for whether read operation is only for Data Bank0 */
    if((SourceAddress + Length) <= FLS_SECTOR_SIZE)
    {
      /*Assign jobtype of Data Bank0 to read */
      JobType[BANK0] = FLS_READ;
      PhysicalAddress = (uint32)(FLS_BASE_ADDRESS + SourceAddress);
  
    }
    /* Check for whether read operation is only for Data Bank1 */
    else if(SourceAddress >= FLS_SECTOR_SIZE)
    {
       /*Assign jobtype of Data Bank1 to read */
      JobType[BANK1] = FLS_READ;
      PhysicalAddress = (uint32)( FLS_BASE_ADDRESS +
                                  (SourceAddress - FLS_SECTOR_SIZE) +
                                  FLS_OFFSET
                                );
    }
    else
    {
      /*Assign jobtype of both the banks to read */
      JobType[BANK0] = FLS_READ;
      JobType[BANK1] = FLS_READ;
      PhysicalAddress = (uint32)(FLS_BASE_ADDRESS + SourceAddress);
    }
  
  /*************************** DET ********************************************/
  
    /* Check for the BUSY DET */
    #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
    /*If DET occurred then return without executing the routine.*/
    if(Fls_lBusyDET_Read(StatePtr) == 1U)
    {
      ReturnValue = E_NOT_OK;
    }
    else
    #endif
  /****************************************************************************/
    {
      /* 2. Set the flash job result to job pending */
      StatePtr->FlsJobResult = MEMIF_JOB_PENDING;
    
      /* Assigning TargetAddressPtr to Global variable FlsReadBufferPtr */
      StatePtr->FlsReadBufferPtr = TargetAddressPtr;
    
      /* Assigning Length to Global variable FlsReadLength*/
      StatePtr->FlsReadLength = Length;
    
      StatePtr->FlsReadAddress = PhysicalAddress;
    
      /*3. Update the new job on affected banks */
      /* The Bit OR operation is used as either s->FlsJobType[x] */
      /*  or NewJobType[x] must be FLS_NONE (0) 
          else BUSY DET would have occured */
      StatePtr->FlsJobType[0] |= JobType[0];
      StatePtr->FlsJobType[1] |= JobType[1];    
    }
  }
  return(ReturnValue);
}

/*********************** AUTOSAR SPECIFIC IMPLEMENTATION **********************/
#if (FLS_GET_STATUS_API  ==  STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS253,DS_AS_FLS034,DS_NAS_FLS_PR734_11,
                                  DS_AS403_FLS357]
                 [/cover]
********************************************************************************
** Syntax : MemIf_StatusType Fls_17_Pmu_GetStatus(void)                       **
**                                                                            **
** Service ID:  4                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out): : None                                                   **
**                                                                            **
** Return value    :  MemIf_StatusType                                        **
**                                                                            **
** Description :                                                              **
** This service shall return the driver state synchronously                   **
*******************************************************************************/
MemIf_StatusType Fls_17_Pmu_GetStatus(void)
{
  MemIf_StatusType ReturnValue;
  uint8* JobTypePtr;

  #if (FLS_PB_FIXEDADDR == STD_OFF)
  if(Fls_ConfigPtr == NULL_PTR)
  #else
  if(Fls_ConfigPtr->FlsStateVarPtr->DriverInitialised == (boolean)FALSE)
  #endif
  {
    ReturnValue = MEMIF_UNINIT;
  }
  else
  {
    JobTypePtr = Fls_ConfigPtr->FlsStateVarPtr->FlsJobType;

    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    if((JobTypePtr[0] != FLS_NONE) ||
       (JobTypePtr[1] != FLS_NONE)
      )
    {
      ReturnValue = MEMIF_BUSY;
    }
    else
    {
      ReturnValue = MEMIF_IDLE;
    }
  }
  /* FLS034: Service shall return driver state synchronously. */
  return(ReturnValue);
} /* End of Function: Fls_GetStatus */
#endif

#if (FLS_GET_STATUS_API  ==  STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR1085_PR123_6]
                 [/cover]
********************************************************************************
** Syntax : MemIf_StatusType Fls_17_Pmu_GetBankStatus(uint32 BankNum)         **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) :  BankNum                                                 **
**                                                                            **
** Parameters (out): : None                                                   **
**                                                                            **
** Return value    :  MemIf_StatusType                                        **
**                                                                            **
** Description :                                                              **
**         This service shall return the driver state synchronously           **
*******************************************************************************/
MemIf_StatusType Fls_17_Pmu_GetBankStatus(uint32 BankNum)
{
  MemIf_StatusType ReturnValue;
  uint8* JobTypePtr;

  #if (FLS_PB_FIXEDADDR == OFF)
  if(Fls_ConfigPtr == NULL_PTR)
  #else
  if(Fls_ConfigPtr->FlsStateVarPtr->DriverInitialised == (boolean)FALSE)
  #endif
  {
    ReturnValue = MEMIF_UNINIT;
  }
  else
  {
    JobTypePtr = Fls_ConfigPtr->FlsStateVarPtr->FlsJobType;

    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    if(JobTypePtr[BankNum] != FLS_NONE)
    {
      ReturnValue = MEMIF_BUSY;
    }
    else
    {
      ReturnValue = MEMIF_IDLE;
    }
  }
  /* FLS034: Service shall return driver state synchronously. */
  return(ReturnValue);
}

/* FLS_GET_STATUS_API  ==  STD_ON */
#endif

#if  (FLS_GET_JOB_RESULT_API  ==  STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS254,DS_AS_FLS035,DS_NAS_FLS_PR734_12,
DS_AS_FLS036,DS_AS403_FLS358]
                 [/cover]
********************************************************************************
** Syntax : MemIf_JobResultType Fls_17_Pmu_GetJobResult(void)                 **
**                                                                            **
** Service ID:  5                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out): : None                                                   **
**                                                                            **
** Return value    :  MemIf_JobResultType                                     **
**                                                                            **
** Description :                                                              **
**         This service shall return the result of the last job synchronously **
*******************************************************************************/
MemIf_JobResultType Fls_17_Pmu_GetJobResult(void)
{
  Fls_17_Pmu_StateType  *StatePtr;
  MemIf_JobResultType RetVal;

  #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
  /* FLS015: API  called before initialization */
  if (Fls_lUninitDET(FLS_SID_GETJOBRESULT) != 0U)
  {
     RetVal = MEMIF_JOB_FAILED;
  }
  else
  #endif
  {
    StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
    RetVal = StatePtr->FlsJobResult;
  }

  return(RetVal);
}
#endif
/*FLS_GET_JOB_RESULT_API == STD_ON */


#if (FLS_IFX_FEE_USED == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR734_10,DS_NAS_FLS_PR1147_PR123_1]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_ReadWordsSync( 
                                             Fls_AddressType SourceAddress,**
**                                           const uint32 *TargetAddressPtr,  **
**                                           uint32 Length                    **
**                                         );                                 **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : SourceAddress, TargetAddressPtr and Length               **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : E_OK: Read successful                                    **
**                   E_NOT_OK: HW busy or read failed with ECC error          **
**                                                                            **
** Description : Service for reading word data from flash bank synchronously  **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_ReadWordsSync(Fls_AddressType SourceAddress,
                                        const uint32 *TargetAddressPtr,
                                        uint32 Length)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* JobTypePtr;
  volatile uint32 PhysicalAddress;
  Std_ReturnType RetVal;
  uint32 *SrcPtr;
  uint32 *DestPtr;
  uint32 BankNum;

  #ifdef FLS_RAM_EMULATION_TEST
  #if (FLS_RAM_EMULATION_TEST == 1U)
  uint32 EmulationLength;
  
  /* Convert to byte read for emulation */
  EmulationLength = Length << 2U;
  
  #endif
  #endif

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  JobTypePtr = StatePtr->FlsJobType;
  RetVal = E_OK;


  if(SourceAddress >= FLS_SECTOR_SIZE)
  {
    PhysicalAddress = (uint32)( FLS_BASE_ADDRESS +
                                (SourceAddress - FLS_SECTOR_SIZE) +
                                FLS_OFFSET
                              );
    BankNum = 1U;
  }
  else
  {
    PhysicalAddress = (uint32)( FLS_BASE_ADDRESS + SourceAddress );
    BankNum = 0U;
  }

  SrcPtr = (uint32*)PhysicalAddress;
  DestPtr = (uint32*)(void *)TargetAddressPtr;
  /* Check if no operation ongoing */
  if(JobTypePtr[BankNum] == FLS_NONE)
  {
    /* Modify the GlobalVar JobType so that no new requests can be accepted on *
     * the Bank till ReadSync is finished */
    JobTypePtr[BankNum] = FLS_READSYNC;
   
   do
    {
      *DestPtr = *SrcPtr;
      /* Point to the next word */
      DestPtr++;
       /* Increment by one word */
      SrcPtr++;
      Length--;
    }while(Length > 0U);  
       
    #ifdef FLS_RAM_EMULATION_TEST
    #if (FLS_RAM_EMULATION_TEST == 1U)
    {
      Std_ReturnType EmulationRetVal;

      FlsEmulation_ReadRequest.FlashAddress = PhysicalAddress;
      FlsEmulation_ReadRequest.Buffer = (uint8*)(void*)TargetAddressPtr;
      FlsEmulation_ReadRequest.Length = (uint32)EmulationLength;

      EmulationRetVal = FlsEmulation_Read(FlsEmulation_ReadRequest.FlashAddress,
                                    FlsEmulation_ReadRequest.Buffer,
                                    FlsEmulation_ReadRequest.Length);

      if (EmulationRetVal == E_NOT_OK)
      {
        /* Restore the JobType with previous tasks */
        JobTypePtr[BankNum] = FLS_NONE;   
        /* Report Dem */
        Fls_lDemReportError_Read();

        RetVal = EmulationRetVal;
      }
    }
    #endif
    #endif

    /* Read the data */

    /* Restore the JobType with previous tasks */
    JobTypePtr[BankNum] = FLS_NONE;

    /* Check for uncorrectable bit error */
    if(Fls_lChkBitErrors())
    {
      /* FLS106: Read Job fails due to hardware error */
      /* Report Production errors to DEM */
      Fls_lDemReportError_Read();
     
      /* Return E_NOT_OK */
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    RetVal = E_NOT_OK;
  }

   /* Execute Clear Status command */
  if( (StatePtr->FlsJobType[0] == FLS_NONE) &&
       (StatePtr->FlsJobType[1] == FLS_NONE)
    )
  {
    Fls_ClearStatusCmdCycle();
  }

  return(RetVal);
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR1146_PR123_2]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_CompareWordsSync(                       **
                                             Fls_AddressType SourceAddress,   **
**                                           const uint32 *TargetAddressPtr,  **
**                                           uint32 Length);                  **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : SourceAddress, TargetAddressPtr and Length               **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : E_OK: Compare successful                                 **
**                   E_NOT_OK: HW busy or compare failed or ECC error         **
**                                                                            **
** Description : Service for comparing data from flash bank synchronously     **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_CompareWordsSync(Fls_AddressType SourceAddress,
                                           const uint32 *TargetAddressPtr,
                                           uint32 Length)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* JobTypePtr;
  volatile uint32 PhysicalAddress;
  Std_ReturnType RetVal;
  uint32 *SrcPtr;
  uint32 BankNum;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  JobTypePtr = StatePtr->FlsJobType;
  RetVal = E_OK;

  if(SourceAddress >= FLS_SECTOR_SIZE)
  {
    PhysicalAddress = (uint32)( FLS_BASE_ADDRESS +
                                (SourceAddress - FLS_SECTOR_SIZE) +
                                 FLS_OFFSET
                              );
    BankNum = 1U;
  }
  else
  {
    PhysicalAddress = (uint32)( FLS_BASE_ADDRESS + SourceAddress );
    BankNum = 0U;
  }

  SrcPtr = (uint32*)PhysicalAddress;
  /* Check if no operation ongoing */
  if( (JobTypePtr[BankNum] == FLS_NONE)
        #if(FLS_VER_ERROR_CHECK == STD_ON)
        #if(FLS_IFX_FEE_USED == STD_ON)
        && (StatePtr->FlsPver == 0U)
        #endif
        #endif        
     )
  {
    /* Modify the GlobalVar JobType so that no new requests can be accepted on *
     * the Bank till CompareSync is finished */
    JobTypePtr[BankNum] = FLS_COMPARESYNC;

    #ifdef FLS_RAM_EMULATION_TEST
    #if (FLS_RAM_EMULATION_TEST == 1U)
    {
      Std_ReturnType EmulationRetVal;

      FlsEmulation_ReadRequest.FlashAddress = PhysicalAddress;
      FlsEmulation_ReadRequest.Buffer = (uint8*)(void*)TargetAddressPtr;
      FlsEmulation_ReadRequest.Length = (uint32)Length;

      EmulationRetVal = FlsEmulation_CompareSync(
                                    FlsEmulation_ReadRequest.FlashAddress,
                                    FlsEmulation_ReadRequest.Buffer,
                                    FlsEmulation_ReadRequest.Length);

      if (EmulationRetVal == E_NOT_OK)
      {
        /* Restore the JobType with previous tasks */
        JobTypePtr[BankNum] = FLS_NONE;
        RetVal = EmulationRetVal;
      }
    }
    #endif
    #endif

    do
    {
      /* COMPARE THE FLASH DATA WITH THE BUFFER DATA (word Compare)*/
      if( *TargetAddressPtr != *SrcPtr )
      {
        RetVal = E_NOT_OK;
        Length = 0U;
      }
      else
      {
        TargetAddressPtr++;
        SrcPtr++;
        Length--;
      }
    }while(Length > 0U);

    /* Restore the JobType with previous tasks */
    JobTypePtr[BankNum] = FLS_NONE;
  
    /* Check for uncorrectable bit error */
    if(Fls_lChkBitErrors())
    {
      /* Return E_NOT_OK */
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  /* Execute Clear Status command */
  if( (StatePtr->FlsJobType[0U] == FLS_NONE) &&
     (StatePtr->FlsJobType[1U] == FLS_NONE))
  {
    Fls_ClearStatusCmdCycle();
  }

  return(RetVal);
}


/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR1145_PR123_3]
                  [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_VerifyErase( uint32 BankNum,            **
                                 uint32 * const UnerasedWordlineAddressPtr    **
                                 uint8 * const UnerasedWordlineCountPtr       **
**                               );                                           **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : BankNum                                                  **
**                                                                            **
** Parameters (out): UnerasedWordlineAddressPtr,UnerasedWordlineCountPtr      **
**                                                                            **
** Return value    :  E_OK: Erase verification successful                     **
**                    E_NOT_OK: HW busy or erase verification failed          **
**                                                                            **
** Description : Service for verifying erase of a sector                      **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_VerifyErase( uint32 BankNum,
                                   uint32 * UnerasedWordlineAddressPtr,
                                   uint8 * const UnerasedWordlineCountPtr )
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* JobTypePtr;
  volatile uint32 PhysicalAddress;
  Std_ReturnType RetVal;
  volatile uint32 *SrcPtr;
  uint32 Length;
  uint32 UnerasableWLAddr;
  uint8 FailCount;
  uint8 CompareFail;
  uint32 TempVal;
  
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  JobTypePtr = StatePtr->FlsJobType;
  RetVal = E_OK;
  FailCount = 0U;
  
  if(BankNum == BANK0)
  {
    PhysicalAddress = DFLASH_BANK0_PHYSICAL;
  }
  else
  {
    PhysicalAddress = DFLASH_BANK1_PHYSICAL;
  }

  SrcPtr = (volatile uint32*)PhysicalAddress;
  /* Check if no operation ongoing */
  if(JobTypePtr[BankNum] == FLS_NONE)
  {
    /* For verify erase testing */
    #ifdef FLS_17_PMU_VERIFYERASE_TEST
    #if (FLS_17_PMU_VERIFYERASE_TEST == 1U)
    RetVal = FlsEmulation_VerifyErase(PhysicalAddress,
                                      UnerasedWordlineAddressPtr,
                                      UnerasedWordlineCountPtr);
    #endif
    #endif

    /* Modify the GlobalVar JobType so that no new requests can be accepted on *
     * the Bank till VerifyErase is finished */
    JobTypePtr[BankNum] = FLS_VERIFYERASE;
    Length = FLS_SECTOR_SIZE;
    
    do
    {
      CompareFail = 0U;
      
      /* COMPARE THE FLASH DATA WITH zero (first 16 Bytes of the Wordline)*/
      TempVal = ((*SrcPtr) | *(SrcPtr + 1U) | *(SrcPtr + 2U)| *(SrcPtr + 3U));
      
      if(TempVal !=0U)
      {
        CompareFail = 1U;
      }
      if(Fls_lChkBitErrors())
      {
        CompareFail = 1U;
        Fls_ClearStatusCmdCycle();
      }
      
      if(CompareFail == 1U)
      {
        FailCount++;
        if(FailCount > FLS_WL_FAIL_COUNT)
        {
           RetVal = E_NOT_OK;
        }
        else
        {
          /* Update the Unerasable WL address */
          UnerasableWLAddr = (uint32)SrcPtr - PhysicalAddress;
                    
          if(PhysicalAddress == DFLASH_BANK1_PHYSICAL)
          {
            UnerasableWLAddr += FLS_SECTOR_SIZE;
          }
          
          *UnerasedWordlineAddressPtr = UnerasableWLAddr;
          
          /* increment SrcPtr to next wordline, divide by 4 for SrcPtr is
           uint32 ptr*/
          SrcPtr += (FLS_WORDLINE_SIZE/FLS_WORD_SIZE);
          Length -= FLS_WORDLINE_SIZE;
          UnerasedWordlineAddressPtr++;
        }
      }
      else
      {
        /* increment SrcPtr to next wordline, divide by 4 for SrcPtr is
           uint32 ptr*/
        SrcPtr += (FLS_WORDLINE_SIZE/FLS_WORD_SIZE);
        Length -= FLS_WORDLINE_SIZE;
      }
    
    }while((Length != 0U) && (FailCount <= FLS_WL_FAIL_COUNT) );
    
    *UnerasedWordlineCountPtr = FailCount;

    /* Restore the JobType with previous tasks */
    JobTypePtr[BankNum] = FLS_NONE;
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  return(RetVal);
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_HE2_FLS_PR3046]
..................[cover parentID=DS_NAS_EP_FLS_PR3046]
..................[cover parentID=SAS_NAS_HE2_FLS_PR3046]
..................[cover parentID=SAS_NAS_EP_FLS_PR3046]
                  [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_VerifySectorErase( uint32 BankNum,      **
**                                 uint32 * const UnerasedWordlineAddressPtr  **
**                                 uint8 * const UnerasedWordlineCountPtr     **
**                               uint8 Sector);                               **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : BankNum,Sector                                           **
**                                                                            **
** Parameters (out): UnerasedWordlineAddressPtr,UnerasedWordlineCountPtr      **
**                                                                            **
** Return value    :  E_OK: Erase verification successful                     **
**                    E_NOT_OK: HW busy or erase verification failed          **
**                                                                            **
** Description : Service for verifying erase of a sector                      **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_VerifySectorErase( uint32 BankNum,
                                   uint32 * UnerasedWordlineAddressPtr,
                                   uint8 * const UnerasedWordlineCountPtr,
                                   uint8 Sector)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* JobTypePtr;
  volatile uint32 PhysicalAddress;
  Std_ReturnType RetVal;
  volatile uint32 *SrcPtr;
  uint32 Length;
  uint32 UnerasableWLAddr;
  uint8 FailCount;
  uint8 CompareFail;
  uint32 TempVal;
  
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  JobTypePtr = StatePtr->FlsJobType;
  RetVal = E_OK;
  FailCount = *UnerasedWordlineCountPtr;

  
  if(BankNum == BANK0)
  {
    PhysicalAddress = DFLASH_BANK0_PHYSICAL;
  }
  else
  {
    PhysicalAddress = DFLASH_BANK1_PHYSICAL;
  }

  SrcPtr = (volatile uint32*)(PhysicalAddress + (FLS_SUBSECTOR_SIZE * Sector));
  /* Check if no operation ongoing */
  if(JobTypePtr[BankNum] == FLS_NONE)
  {
    /* For verify erase testing */
    #ifdef FLS_17_PMU_VERIFYERASE_TEST
    #if (FLS_17_PMU_VERIFYERASE_TEST == 1U)
    RetVal = FlsEmulation_VerifyErase(PhysicalAddress,
                                      UnerasedWordlineAddressPtr,
                                      UnerasedWordlineCountPtr);
    #endif
    #endif

    /* Modify the GlobalVar JobType so that no new requests can be accepted on *
     * the Bank till VerifyErase is finished */
    JobTypePtr[BankNum] = FLS_VERIFYERASE;
    Length = FLS_SUBSECTOR_SIZE;/*FLS_SECTOR_SIZE;*/
    
    do
    {
      CompareFail = 0U;
      
      /* COMPARE THE FLASH DATA WITH zero (first 16 Bytes of the Wordline)*/
      TempVal = ((*SrcPtr) | *(SrcPtr + 1U) | *(SrcPtr + 2U)| *(SrcPtr + 3U));
      
      if(TempVal !=0U)
      {
        CompareFail = 1U;
      }
      if(Fls_lChkBitErrors())
      {
        CompareFail = 1U;
        Fls_ClearStatusCmdCycle();
      }
      
      if(CompareFail == 1U)
      {
        FailCount++;
        if(FailCount > FLS_WL_FAIL_COUNT)
        {
           RetVal = E_NOT_OK;
        }
        else
        {
          /* Update the Unerasable WL address */
          UnerasableWLAddr = (uint32)SrcPtr - PhysicalAddress;
                    
          if(PhysicalAddress == DFLASH_BANK1_PHYSICAL)
          {
            UnerasableWLAddr += FLS_SECTOR_SIZE;
          }
          
          *UnerasedWordlineAddressPtr = UnerasableWLAddr;
          
          /* increment SrcPtr to next wordline, divide by 4 for SrcPtr is
           uint32 ptr*/
          SrcPtr += (FLS_WORDLINE_SIZE/FLS_WORD_SIZE);
          Length -= FLS_WORDLINE_SIZE;
          UnerasedWordlineAddressPtr++;
        }
      }
      else
      {
        /* increment SrcPtr to next wordline, divide by 4 for SrcPtr is
           uint32 ptr*/
        SrcPtr += (FLS_WORDLINE_SIZE/FLS_WORD_SIZE);
        Length -= FLS_WORDLINE_SIZE;
      }
    
    }while((Length != 0U) && (FailCount <= FLS_WL_FAIL_COUNT) );
    
    *UnerasedWordlineCountPtr = FailCount;
    
    /* Restore the JobType with previous tasks */
    JobTypePtr[BankNum] = FLS_NONE;
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  return(RetVal);
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR1148_PR123_8]
                 [/cover]
********************************************************************************
** Syntax : Fls_17_Pmu_Job_Type Fls_17_Pmu_GetNotifCaller(void)               **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  FLS Job that raised the notification                    **
**                                                                            **
** Description :                                                              **
** Ths function returns the FLS Job that raised the notification              **
*******************************************************************************/
Fls_17_Pmu_Job_Type Fls_17_Pmu_GetNotifCaller(void)
{
  return(Fls_ConfigPtr->FlsStateVarPtr->NotifCaller);
}

#endif /* FLS_IFX_FEE_USED == STD_ON */

#if (FLS_USE_ERASESUSPEND == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR1149_PR123_4]
                  [cover parentID=DS_NAS_FLS_PR3158_1]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_SuspendErase(uint32 BankNum)            **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BankNum - Sector on which erase is to be suspended      **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  E_OK - Erase suspend successful                         **
**                    E_NOT_OK - Erase suspend failed                         **
**                                                                            **
** Description :                                                              **
** Ths function suspends an ongoing erase of a sector.                        **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_SuspendErase(uint32 BankNum)
{
  Std_ReturnType RetVal;
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* JobTypePtr;
  uint32 TimeOutCount;
  uint32 CheckSpndBit;
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  JobTypePtr = StatePtr->FlsJobType;
  
  CheckSpndBit = FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U);
  /*Check if SPND of FSR is equal to 1U */
  if(((CheckSpndBit & FLS_FSR_SPND_MASK) >> FLS_FSR_SPND_BITPOS)==1U)
  {  
    RetVal = E_OK;
    /* Set job status for the bank to None */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    JobTypePtr[BankNum] = FLS_NONE;
    /* Reset JobStartStatus */
    StatePtr->JobStarted.Erase = 0U;
  }
  else
  {
    #ifdef IFX_FLS_DEBUG03
    if (TestFls_DebugMask03 != 1)
    #endif
    {
      /* Update SPND bit of MARD */
    FLS_SFR_RUNTIME_USER_MODE_MODIFY32(FLASH0_MARD.U,FLS_WRITE_ONE_CLEARMASK, \
                                       FLS_MARD_SPND)
    }
    TimeOutCount = FLS_ERASESUSPEND_TIMEOUT;
    do 
    {
      TimeOutCount-- ;
    }while(((((FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
               FLS_FSR_SPND_MASK) >> FLS_FSR_SPND_BITPOS)) == 0U) && \
                                                          (TimeOutCount > 0U));
             
  
    if ((boolean)TRUE == Fls_lSpndTimeout(TimeOutCount))
    {
      /* Clear the SPNDERR bit by writing 1 */
      FLS_SFR_RUNTIME_USER_MODE_MODIFY32(FLASH0_MARD.U, \
            FLS_WRITE_ONE_CLEARMASK,FLS_MARD_SPNDERR_SETMASK)
      RetVal = E_NOT_OK;
 
     /* If suspend operation not completed within a configured time duration\
        report DEM */ 
     if(ZERO_VALUE == TimeOutCount)
     { 
        Fls_lDemReportError_SpndResme();
     }
    }
    else if(FLS_D0BUSY == Fls_lHWBusyCheck())
    {
      /* If hardware is still busy, Clear the SPNDERR bit by writing 1 */
      FLS_SFR_RUNTIME_USER_MODE_MODIFY32(FLASH0_MARD.U, \
                   FLS_WRITE_ONE_CLEARMASK,FLS_MARD_SPNDERR_SETMASK)
      RetVal = E_NOT_OK;
    }
    else
    {
      RetVal = E_OK;
      /* Set job status for the bank to None */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
      JobTypePtr[BankNum] = FLS_NONE;
      /* Reset JobStartStatus */
      StatePtr->JobStarted.Erase = 0U;
      /* Set the flash job result to job ok */
      StatePtr->FlsJobResult = MEMIF_JOB_OK;
    }
  }
  
  return RetVal;
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_FLS_PR1150_PR123_5]
                  [cover parentID=DS_NAS_FLS_PR3158_2]
                 [/cover]
********************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_ResumeErase (uint32 BankNum )           **
**                                                                            **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BankNum - Sector on which erase is to be resumed        **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  E_OK - Erase suspend successful                         **
**                    E_NOT_OK - Erase suspend failed                         **
**                                                                            **
** Description :                                                              **
** This function suspends an ongoing erase of a sector.                       **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_ResumeErase (uint32 BankNum)
{
  Std_ReturnType RetVal;
  Fls_17_Pmu_StateType  *StatePtr;
  uint8* JobTypePtr;
  volatile uint32 PhysicalAddress;
  uint32 TimeOutCount;
  uint32 CheckSpndBit;
  
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  JobTypePtr = StatePtr->FlsJobType;
  RetVal = E_OK;
  
  /* Check if SPND bit of MARD is equal to 1U */
  CheckSpndBit = FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U);
  CheckSpndBit = CheckSpndBit & FLS_FSR_SPND_MASK;
  if((CheckSpndBit >> FLS_FSR_SPND_BITPOS)==1U)
  {
    if(Fls_lChkOperError() == 0U)
    {
      if(BankNum == BANK0)
      {
        PhysicalAddress = DFLASH_BANK0_PHYSICAL;
      }
      else
      {
        PhysicalAddress = DFLASH_BANK1_PHYSICAL;
      }
      
      #if( FLS_AC_LOAD_ON_JOB_START == STD_ON)
      Fls_lLoadAccessCode(FLS_ERASE);
      #endif
      
      #ifdef IFX_FLS_DEBUG04
      if (TestFls_DebugMask04 != 0)
      #endif
      {
        /* Enter Critical Section */
        Fls_lSchMEnter_ResumeErase();
            
        /* Execute the Resume ERASE COMMAND CYCLES */
        Fls_ResumeEraseCmdCycle(PhysicalAddress);
            
        /* Exit Crtical Section */
        Fls_lSchMExit_ResumeErase(); 
      }
      
      if(Fls_lChkSeqProtErrors() != 0U)
      {
        /* Enter Critical Section */
        Fls_lSchMEnter_ResumeErase();
        
        Fls_ClearStatusCmdCycle();
        
        /* Exit Crtical Section */
        Fls_lSchMExit_ResumeErase(); 
        
        RetVal = E_NOT_OK;
      }
      else
      {
        TimeOutCount = FLS_ERASESUSPEND_TIMEOUT;
        do 
        {
          TimeOutCount-- ;
        }while((((FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
                  FLS_FSR_SPND_MASK) >> FLS_FSR_SPND_BITPOS)==1U) && \
                                                        (TimeOutCount > 0U));
          
        if(0U != Fls_lChkOperError())
        {
          /* OPER error occured */
          RetVal = E_NOT_OK;
        }
        else if((boolean)TRUE == Fls_lResmErTimeout(TimeOutCount))
        {
          /* If timeout happens and Suspend bit is not cleared return E_NOT_OK*/
          RetVal = E_NOT_OK;
          /* If resume operation not completed within a configured time 
          duration report DEM */ 
            Fls_lDemReportError_SpndResme();          
        }
        else if(0U == Fls_lHWBusyCheck())
        {
          /* If hardware is still not busy i.e., if erase did not resume, 
             then return E_NOT_OK */
          RetVal = E_NOT_OK;
        }
        else
        {
          /* Restore the JobType with previous erase */
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
            FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
            contents*/
          JobTypePtr[BankNum] = FLS_ERASE;
          /* 1. Set the flash job result to job pending */
          StatePtr->FlsJobResult = MEMIF_JOB_PENDING;
          /*Indicate that the ERASE Job has been started */
          StatePtr->JobStarted.Erase = 1U;
        }
      }
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }

  return RetVal;
}

#endif /* FLS_USE_ERASESUSPEND == STD_ON */

/*******************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_GetOperStatus(void)                     **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  E_OK - No OPER error                                    **
**                    E_NOT_OK - OPER error has occurred                      **
**                                                                            **
** Description : This function provides the OPER error status                 **
*******************************************************************************/
Std_ReturnType Fls_17_Pmu_GetOperStatus(void)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint32 OPER_Status;
  Std_ReturnType RetVal;

  RetVal = E_OK;
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  #ifndef FLS_RAM_EMULATION_TEST
  #ifdef IFX_FLS_DEBUG
  OPER_Status = (((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) | \
                                           TestFls_DebugMask) & FLS_OPER_ERROR);
  #else
  OPER_Status = ((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
                                                            FLS_OPER_ERROR);
  #endif 
  #else
  OPER_Status = (FlsEmulation_ReadStatusRegister() & FLS_OPER_ERROR);
  #endif
  
  if((0U != OPER_Status) || (0U != StatePtr->FlsOper))
  {
    RetVal = E_NOT_OK;
  }
  
  return (RetVal);
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_HE2_FLS_PR2939,DS_NAS_EP_FLS_PR2939]
                  [/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_ControlTimeoutDet(uint8 Param)                    **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  Param                                                   **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This is the function to disable and re-enable detection /    **
**               reporting of time-out DET                                    **
*******************************************************************************/
void Fls_17_Pmu_ControlTimeoutDet(uint8 Param)
{

#if (FLS_DEV_ERROR_DETECT == STD_ON)

  Fls_17_Pmu_StateType   *StatePtr;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  StatePtr->FlsTimeoutControl = Param;

#else

  UNUSED_PARAMETER(Param)

#endif

}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS104,DS_AS_FLS022,
DS_AS403_FLS313,DS_AS403_FLS315,
DS_AS_FLS167_1,DS_AS_FLS347_1,DS_AS_FLS140_FLS_143_1,
DS_NAS_FLS_PR1058_3]
                 [/cover]
********************************************************************************
** Syntax : static void Fls_lMainErase(uint8 BankType)                        **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non-reentrant                                                 **
**                                                                            **
** Parameters (in) : BankType    DFlash Bank                                  **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Performs the erase operation during Interrupt/Cyclic         **
**               Job handling                                                 **
*******************************************************************************/
static void Fls_lMainErase(uint8 BankType)
{
  Fls_17_Pmu_StateType       *StatePtr;
  MemIf_JobResultType LastJobResult;
  volatile uint32     PhysicalAddress;
  Std_ReturnType      RetVal;
  
  #if ( FLS_IFX_FEE_USED ==  STD_OFF )
  #if ( FLS_DEV_ERROR_DETECT == STD_ON )
  uint32              *ErasePtr;
  uint32 Count;
  #endif
  #endif

  /******************* Initialise the global variables ***************/
  RetVal = E_OK;
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  LastJobResult = StatePtr->FlsJobResult;

  /* Obtain the physical address for the write operations */
  if(BankType == BANK0)
  {
    PhysicalAddress = DFLASH_BANK0_PHYSICAL;
  }
  else
  {
    PhysicalAddress = DFLASH_BANK1_PHYSICAL;
  }

  if(StatePtr->JobStarted.Erase == 0U)
  {
    /* Erase job has to be started */
    Fls_lMainEraseJobStart(PhysicalAddress);
  }
  else
  {
    /* Erase job has already been started */
    if(Fls_lChkOperError() != 0U) /* Check for operational error */
    {
      /* FLS104: Erase Job fails due to hardware error */
      
          /* Report Production errors to DEM */
          Fls_lDemReportError_Erase();
   
      /* Call Fls_lErrorHandler local function */
      Fls_lErrorHandler(FLS_ERASE);
      RetVal = E_NOT_OK;
    }
    
    /******************************** DET *************************************/
    #if ( FLS_IFX_FEE_USED ==  STD_OFF )
    #if ( FLS_DEV_ERROR_DETECT == STD_ON )
    else
    {
      /********* Check for DFlash Sector content equal to zero *****/
      /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to pointer*/
      /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to pointer*/
      ErasePtr = (uint32 *)PhysicalAddress;
      Count = 0U;
      do
      {
        if((*ErasePtr)== 0U)
        {
          Count++;
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic on a pointer to 
            Flash location*/
          ErasePtr++;
        }
        else
        {
          /* FLS022: If block is not completely erased error should be */
          /* reported                                                  */
          /* Call Fls_lErrorHandler local function */
          Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                           FLS_17_PMU_INSTANCE_ID,FLS_SID_MAIN,
                           FLS_E_VERIFY_ERASE_FAILED);/* Report to DET */
          Fls_lErrorHandler(FLS_ERASE);
          RetVal = E_NOT_OK; /*return from function*/
          break;
        }
      } while(Count < (FLS_SECTOR_SIZE / FLS_WORD_SIZE));
      /*(FLS_SECTOR_SIZE / 4U) is used since it is a word comparison */
      /* FLS_DEV_ERROR_DETECT */
    }
    #endif
    #endif
    
    if(RetVal == E_OK)
    {
      StatePtr->FlsJobType[BankType] = FLS_NONE;
    
      /* Update the JobResult */
      StatePtr->FlsJobResult = LastJobResult;
    
      /* Set the Bank type to the next bank */
      BankType = BANK1;
      PhysicalAddress = DFLASH_BANK1_PHYSICAL;

      if(StatePtr->FlsJobType[BankType] == FLS_ERASE)
      {
        /* Start Erase operation */
        Fls_lMainEraseJobStart(PhysicalAddress);
      }
      else /* Job completed successfully */
      {
        /* Set to JOB_OK if no warnings  */
        LastJobResult = MEMIF_JOB_OK;
         
        #if( FLS_AC_LOAD_ON_JOB_START == STD_ON)
        Fls_lUnLoadAccessCode(FLS_ERASE);
        #endif
    
        /* Execute Clear Status command */
        Fls_ClearStatusCmdCycle();
    
        Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_ERASE_JOB;
    
        if(Fls_ConfigPtr->FlsJobEndNotificationPtr != NULL_PTR)
        {
          /* FlsJobEndNotificationPtr should be called */
          (Fls_ConfigPtr->FlsJobEndNotificationPtr)();
        }
    
        Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_NO_JOB;
    
        /* Update the global status variables */
        StatePtr->FlsJobResult = LastJobResult;
    
        /* Clear JobStarted bit for operation JobType */
        StatePtr->JobStarted.Erase = 0U;
    
      }/* End of Job evaluation */
    }
  }
}

/*******************************************************************************
** Syntax : static void Fls_lMainEraseJobStart(uint32 PhysicalAddress)        **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non-reentrant                                                 **
**                                                                            **
** Parameters (in) : Physical Address - Physical address for Erase            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Performs the erase operation during Interrupt/Cyclic         **
**               Job handling                                                 **
*******************************************************************************/
static void Fls_lMainEraseJobStart(uint32 PhysicalAddress)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint32                  SeqProtErr;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  /* Indicate that the ERASE Job has been started */
  StatePtr->JobStarted.Erase = 1U;
  
  /* Enter Critical Section */
  Fls_lSchMEnter_Main();
  
  /* Execute ACCESS CODE for erase operation */
  Fls_CallEraseCommand(PhysicalAddress);
  
  /* Exit Crtical Section */
  Fls_lSchMExit_Main();
  
  #ifdef FLS_RAM_EMULATION_TEST
  #if (FLS_RAM_EMULATION_TEST == 1U)
  (void) FlsEmulation_Erase(PhysicalAddress);
  #endif
  #endif
  
  SeqProtErr = Fls_lChkSeqProtErrors();
  /* Check for Sequence error,Protection error bit */
  if(SeqProtErr != 0U)
  {    
          /* Report Production errors to DEM */
          Fls_lDemReportError_Erase();
     
    /* Call to Fls_lErrorHandler local function */
    Fls_lErrorHandler(FLS_ERASE);
  }
  /* Check for command cycle Timeout */
  else if (StatePtr->FlsTimeoutErr == FLS_ERASE)
  {
    /* As per AI00157728 - timeout error need not be reported to DEM */
    /* Call to Fls_lErrorHandler local function */
    Fls_lErrorHandler(FLS_ERASE); 
  }
  else
  {
    /* dummy else */
  }
}

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS106,DS_AS_FLS167_4,
   DS_AS_FLS347_4,DS_NAS_FLS_PR1057,DS_AS403_FLS317]
                 [/cover]
********************************************************************************
** Syntax : static void Fls_lMainRead(uint8  BankType)                        **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non-reentrant                                                 **
**                                                                            **
** Parameters (in) : uint8   DFlash Bank                                      **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Performs the Read operation during Interrupt/Cyclic          **
**               Job handling                                                 **
*******************************************************************************/
static void Fls_lMainRead(uint8  BankType)
{
  Fls_17_Pmu_StateType       *StatePtr;
  Fls_LengthType      ReadCount;
  MemIf_JobResultType LastJobResult;
  uint8               *SourcePtr;
  Fls_LengthType      MaxRead;

  /************* Initialise Local Variables *****************************/
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  /* Get max read length according to the FLS mode */
   MaxRead = Fls_lGetReadModeLength();
    
  /* Check whether the given length is greater than MAX_READ */
  if(StatePtr->FlsReadLength > MaxRead)
  {
    /* Load Max Byte to read in one cycle */
    ReadCount = MaxRead;
    /* Decrement the count from the total count to read */
    StatePtr->FlsReadLength = StatePtr->FlsReadLength - ReadCount;
  }
  else
  {
    ReadCount = StatePtr->FlsReadLength;
    StatePtr->FlsReadLength = 0U;
  }

  /* Obtain the Read Address */
  /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
  /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
  SourcePtr = (uint8 *)(StatePtr->FlsReadAddress);

  /**********************************************************************/
  #ifdef FLS_RAM_EMULATION_TEST
  #if (FLS_RAM_EMULATION_TEST == 1U)
  FlsEmulation_ReadRequest.FlashAddress = StatePtr->FlsReadAddress;
  FlsEmulation_ReadRequest.Buffer = StatePtr->FlsReadBufferPtr;
  FlsEmulation_ReadRequest.Length = ReadCount;
  #endif
  #endif

  do
  {  /* Determine the physical address for reading */
     /* Check whether sourceptr reached end of Bank0 */
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
    /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
    if( (SourcePtr == (uint8 *)(DFLASH_BANK0_PHYSICAL + FLS_SECTOR_SIZE)) &&
        ((BankType != BANK1) &&
        (StatePtr->FlsJobType[BANK1] == FLS_READ))
      )
    {
      StatePtr->FlsReadAddress = DFLASH_BANK1_PHYSICAL;
      /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
      /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
      SourcePtr = (uint8 *)(StatePtr->FlsReadAddress);
      BankType = BANK1;
      StatePtr->FlsJobType[BANK0] = FLS_NONE;

    }

    /* READ DATA AND STORE THE DATA AT DESTINATION */
    /*IFX_MISRA_RULE_11_01_STATUS=Buffer address is converted to a pointer*/
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
    /*IFX_MISRA_RULE_11_03_STATUS=Buffer address is converted to a pointer*/
    /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
    if( (ReadCount >= FLS_WORD_SIZE) &&
        ( (((uint32)SourcePtr & THREE_VALUE) == 0U) &&
         (((uint32)(StatePtr->FlsReadBufferPtr) & THREE_VALUE) == 0U))
      )
    {
      /* Perform Word Read to save runtime */
      *(uint32*)(void*)(StatePtr->FlsReadBufferPtr) = 
                                           *(uint32*)(void*)SourcePtr;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
        user buffer efficiently*/
      (StatePtr->FlsReadBufferPtr)+= FLS_WORD_SIZE;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used on Flash
        pointer to access Flash locations efficiently*/
      SourcePtr+= FLS_WORD_SIZE;
      ReadCount -= FLS_WORD_SIZE;
    }
    else
    {
      *(StatePtr->FlsReadBufferPtr) = *SourcePtr;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
        user buffer efficiently*/
      (StatePtr->FlsReadBufferPtr)++;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used on Flash
        pointer to access Flash locations efficiently*/
      SourcePtr++;
      ReadCount--;
    }

  }while(((ReadCount)) > 0U);

  #ifdef FLS_RAM_EMULATION_TEST
  #if (FLS_RAM_EMULATION_TEST == 1U)
  {
    uint32 x;
    x = FlsEmulation_Read(FlsEmulation_ReadRequest.FlashAddress,
                    FlsEmulation_ReadRequest.Buffer,
                    FlsEmulation_ReadRequest.Length);
    if (x == E_NOT_OK)
    {    
      /* Report Dem */
      Fls_lDemReportError_Read();    
      /* Call to Fls_lErrorHandler local function */
      Fls_lErrorHandler(FLS_READ);
    }
  }
  #endif
  #endif

  /* Update the Read Address */
  /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
  /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
  StatePtr->FlsReadAddress = (uint32)SourcePtr;

  /* Check for single bit or double bit error */

  if(Fls_lChkBitErrors())
  {
    /* FLS106: Read Job fails due to hardware error */
    /* Report Production errors to DEM */
    Fls_lDemReportError_Read();  
    /* Call to Fls_lErrorHandler local function */
    Fls_lErrorHandler(FLS_READ);
  }
  else
  {

    if(StatePtr->FlsReadLength == 0U)
    {
      /* Set Job Result to JOB OK if no warnings */
      LastJobResult = MEMIF_JOB_OK;
      /* 1. Clear the Job request */
      StatePtr->FlsJobType[BankType] = FLS_NONE;

      /* 2. Update the global status variables */
      StatePtr->FlsJobResult = LastJobResult;

      Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_READ_JOB;

      if(Fls_ConfigPtr->FlsJobEndNotificationPtr != NULL_PTR)
      {
         /* FlsJobEndNotificationPtr should be called */
         (Fls_ConfigPtr->FlsJobEndNotificationPtr)();
      }

      Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_NO_JOB;

    }
  }
}


/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS154,DS_AS_FLS200,
   DS_AS_FLS167_3,DS_AS_FLS347_3_FLS349,DS_NAS_FLS_PR1057,DS_AS403_FLS318]
                 [/cover]
********************************************************************************
** Syntax : static void Fls_lMainCompare(uint8  BankType)                     **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non-reentrant                                                 **
**                                                                            **
** Parameters (in) : uint8   DFlash Bank                                      **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Performs the Compare operation during Interrupt/Cyclic       **
**               Job handling                                                 **
*******************************************************************************/
static void Fls_lMainCompare(uint8  BankType)
{
  Fls_17_Pmu_StateType       *StatePtr;
  Fls_LengthType      ReadCount;
  MemIf_JobResultType LastJobResult;
  uint8               *SourcePtr;
  Fls_LengthType      MaxRead;
  
  /************ Initialise Local variables ****************************/
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  LastJobResult = StatePtr->FlsJobResult;

  /* Get max read length according to the FLS mode */
  MaxRead = Fls_lGetReadModeLength();
  
  /* Check whether the given length is greater than MAX_READ */
  if(StatePtr->FlsReadLength > MaxRead)
  {
    /* Load Max Byte to read in one cycle */
    ReadCount = MaxRead;
    /* Decrement the count from the totalcount to read */
    StatePtr->FlsReadLength = StatePtr->FlsReadLength - ReadCount;
  }
  else
  {
    ReadCount = StatePtr->FlsReadLength;
    StatePtr->FlsReadLength = 0U;
  }

  /* Obtain the address for the compare operation */
  /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
  /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
  SourcePtr = (uint8 *)(StatePtr->FlsReadAddress);
  /**********************************************************************/
  do
  {
    /* Get the actual physical address when BankType changes */
    /* Check whether source ptr reached to end of Bank0 */
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
    /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
    if((SourcePtr == (uint8 *)(DFLASH_BANK0_PHYSICAL + FLS_SECTOR_SIZE)) &&
       ( (BankType != BANK1) &&
         (StatePtr->FlsJobType[BANK1] == FLS_COMPARE))
      )
    {
      /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to a pointer*/
      /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to a pointer*/
      SourcePtr = (uint8 *)(DFLASH_BANK1_PHYSICAL);
      BankType = BANK1;
      StatePtr->FlsJobType[BANK0] = FLS_NONE;
    }
    
    /* Check if Address is word aligned */
    /*IFX_MISRA_RULE_11_01_STATUS=Pointer is typecasted to uint32 for 
      performing bit-wise operation*/
    /*IFX_MISRA_RULE_11_01_STATUS=Pointer is typecasted to uint32 for 
      performing bit-wise operation*/
    /*IFX_MISRA_RULE_11_03_STATUS=Pointer is typecasted to uint32 for 
      performing bit-wise operation*/
    /*IFX_MISRA_RULE_11_03_STATUS=Pointer is typecasted to uint32 for 
      performing bit-wise operation*/
    if( (((uint32)SourcePtr & (FLS_WORD_SIZE - 1U)) == 0U) && 
       ((((uint32)(StatePtr->FlsReadBufferPtr) & (FLS_WORD_SIZE - 1U)) == 0U) &&
           (ReadCount >= FLS_WORD_SIZE)  )
      )
    {
      /* COMPARE THE FLASH DATA WITH THE BUFFER DATA (Word Compare) */
      if( *(uint32*)(void*)(StatePtr->FlsReadBufferPtr) != 
          *(uint32*)(void*)SourcePtr )
      {
        LastJobResult = MEMIF_BLOCK_INCONSISTENT;
        /* No need to compare any more data */
        ReadCount = 0U;
      }
      else
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
          user buffer efficiently*/
        StatePtr->FlsReadBufferPtr += FLS_WORD_SIZE;
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
          Flash location efficiently*/
        SourcePtr += FLS_WORD_SIZE;
        ReadCount -= FLS_WORD_SIZE;
      }
    }
    else
    {
      /* COMPARE THE FLASH DATA WITH THE BUFFER DATA (Byte Compare)*/
      if( *(StatePtr->FlsReadBufferPtr) != *SourcePtr )
      {
        LastJobResult = MEMIF_BLOCK_INCONSISTENT;
        /* No need to compare any more data */
        ReadCount = 0U;

      }
      else
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
          user buffer efficiently*/
        StatePtr->FlsReadBufferPtr++;
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
          Flash location efficiently*/
        SourcePtr++;
        ReadCount--;
      }
    }
  }while(ReadCount > 0U);

  /* Update the compare address */
  /*IFX_MISRA_RULE_11_01_STATUS=Pointer is stored in address variable*/
  /*IFX_MISRA_RULE_11_03_STATUS=Pointer is stored in address variable*/
  StatePtr->FlsReadAddress = (uint32)SourcePtr;

  /* Check if Non-correctable ECC error was present thus leading to failure */
  if(Fls_lChkBitErrors())
  {
    /* FLS154: Compare Job fails due to hardware error */
    /* Report Production errors to DEM */
    Fls_lDemReportError_Compare();  
    /* Call to Fls_lErrorHandler local function */
    Fls_lErrorHandler(FLS_COMPARE);
  }
  /* Check if block was inconsistent */
  else if(LastJobResult == MEMIF_BLOCK_INCONSISTENT)
  {
    /* Update the global status variables */
    StatePtr->FlsJobResult = LastJobResult;

    /* Clear the failed Job from Bank0 */
    if(StatePtr->FlsJobType[BANK0] == FLS_COMPARE)
    {
      StatePtr->FlsJobType[BANK0] = FLS_NONE;
    }
    /* Clear the failed Job from Bank1 */
    if(StatePtr->FlsJobType[BANK1] == FLS_COMPARE)
    {
      StatePtr->FlsJobType[BANK1] = FLS_NONE;
    }

    /* Execute Clear Status command */
    Fls_ClearStatusCmdCycle();

    Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_COMPARE_JOB;

    if(Fls_ConfigPtr->FlsJobErrorNotificationPtr != NULL_PTR)
    {
      /* FlsJobErrorNotificationPtr should be called */
      (Fls_ConfigPtr->FlsJobErrorNotificationPtr)();
    }
    Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_NO_JOB;
  }
  /* No failures detected */
  else
  {
    /* Job completed */
    if ( StatePtr->FlsReadLength == 0U)
    {
      /* Update the result to JOB_OK if no warnings */
      LastJobResult = MEMIF_JOB_OK;

      /* 1. Update the Global Status variables */
      StatePtr->FlsJobResult = LastJobResult;

      /* 2. Clear the Job request */
      StatePtr->FlsJobType[BankType] = FLS_NONE;

      Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_COMPARE_JOB;

      if(Fls_ConfigPtr->FlsJobEndNotificationPtr != NULL_PTR)
      {
         /* FlsJobEndNotificationPtr should be called */
         (Fls_ConfigPtr->FlsJobEndNotificationPtr)();
      }

      Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_NO_JOB;

    }
    /* Job not yet completed */
    else
    {
      /* Avoid MISRA error */
    }
  }
}

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS105,DS_AS_FLS055,
DS_AS_FLS056,DS_AS_FLS167_2,DS_AS_FLS347_2,
DS_AS_FLS141_FLS_143_2,DS_NAS_FLS_PR1058_4,DS_NAS_FLS_PR1059,
DS_AS403_FLS316,DS_AS403_FLS314]
                 [/cover]
********************************************************************************
** Syntax : static void Fls_lMainWrite(uint8 BankType)                        **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non-reentrant                                                 **
**                                                                            **
** Parameters (in) : BankType   DFlash Bank                                   **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Performs the Write operation during Interrupt/Cyclic         **
**               Job handling                                                 **
*******************************************************************************/
static void Fls_lMainWrite(uint8 BankType)
{
  Fls_17_Pmu_StateType       *StatePtr;
  MemIf_JobResultType LastJobResult;
  volatile uint32     PhysicalAddress;
  boolean             Error;
  uint8               PageLength;

  /******************* Initialise local variables ********************/
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  /* Obtain the physical address for the write operations */
  if(BankType == BANK0)
  {
    PhysicalAddress = DFLASH_BANK0_PHYSICAL;
  }
  else
  {
    PhysicalAddress = DFLASH_BANK1_PHYSICAL;
  }

  if(StatePtr->JobStarted.Write == 0U)
  {
    #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
    #if (FLS_IFX_FEE_USED == STD_OFF)
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is typecased to a pointer*/
    /*IFX_MISRA_RULE_11_03_STATUS=Flash address is typecased to a pointer*/
    if(Fls_lPageEraseCheck((uint32 *)(void*)(StatePtr->FlsWriteAddress),
                           StatePtr->FlsWriteLength,
                           FLS_SID_WRITE) == E_NOT_OK)
    {
      Fls_lErrorHandler(FLS_WRITE);
    }
    else  
    #endif /*FLS_IFX_FEE_USED*/
    #endif /* FLS_DEV_ERROR_DETECT */
    {
      /* Start Write operation */
      Fls_lMainWriteJobStart(PhysicalAddress);
    }
  }
  else
  {
    /* Check for operational error */
    if(Fls_lChkOperError() != 0U)
    {
      /* FLS105: Write Job fails due to hardware error */
      /* Report Production errors to DEM */
      Fls_lDemReportError_Write();
      
      Error = (boolean)TRUE;
    }
    /* Check for Program verification error */
    #if(FLS_IFX_FEE_USED == STD_OFF)
    #if(FLS_VER_ERROR_CHECK == STD_ON)
    else if((boolean)TRUE == Fls_lPverChk())
    {
      Error = (boolean)TRUE;
      /* Report Production errors to DEM */
      Fls_lDemReportError_Write();
    }     
    #endif  /* FLS_VER_ERROR_CHECK == STD_ON */
    #endif  /* FLS_IFX_FEE_USED == STD_OFF */
    else
    {
      if( Fls_lGetWriteMode() == PAGE_WRITE )
      {
        PageLength = FLS_PAGE_SIZE;
      }
      else
      {
        PageLength = FLS_BURST_PAGE_SIZE;
      }
  
      #if (FLS_IFX_FEE_USED == STD_OFF)
      #if (FLS_DEV_ERROR_DETECT == STD_ON)
      /* Check for DFLASHx Page content equal to Source value */
      Error = Fls_lCheckWrittenPage((uint32)PageLength);
      #else
      Error = (boolean)FALSE;
      #endif /* FLS_DEV_ERROR_DETECT == STD_ON */
      #else
      Error = (boolean)FALSE;
      #endif /* FLS_IFX_FEE_USED == STD_OFF */
  
      /* PageStartAddress is incremented by PageLength bytes */
      StatePtr->FlsWriteAddress += PageLength;
      /* length is decremented by PageLength */
      StatePtr->FlsWriteLength -= PageLength;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to access
        user buffer efficiently*/
      StatePtr->FlsWriteBufferPtr = (StatePtr->FlsWriteBufferPtr +
                                   PageLength);
  
    }
  
    if(Error == (boolean)FALSE)
    {
      /* Check for Program verification error */
      #if(FLS_IFX_FEE_USED == STD_ON)
      #if(FLS_VER_ERROR_CHECK == STD_ON)
      if ((boolean)TRUE == Fls_lPverChk())
      {
          /* update FlsPver to be used in CompareWordsSync */
        StatePtr->FlsPver = 1U;
      }
      #endif  /* FLS_VER_ERROR_CHECK == STD_ON */
      #endif  /* FLS_IFX_FEE_USED == STD_ON */
        
      /* Check for Flash write Job is finished completely  */
      if(StatePtr->FlsWriteLength == 0U)
      {
        #if( FLS_AC_LOAD_ON_JOB_START == STD_ON)
        Fls_lUnLoadAccessCode(FLS_WRITE);
        #endif
  
        /* Indicate Job successfully completed if no warnings seen till now */
        LastJobResult = MEMIF_JOB_OK;
        /* 1. Update the global status variables */
        StatePtr->FlsJobResult = LastJobResult;
        /* 2. Clear the Write Job */
        StatePtr->FlsJobType[BankType] = FLS_NONE;
        /* 3. Clear the Write Job Started Status bit */
        StatePtr->JobStarted.Write = 0U;
  
        /* Execute Clear Status command */
        Fls_ClearStatusCmdCycle();
  
        Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_WRITE_JOB;
  
        if(Fls_ConfigPtr->FlsJobEndNotificationPtr != NULL_PTR)
        {
          /* FlsJobEndNotificationPtr should be called */
          (Fls_ConfigPtr->FlsJobEndNotificationPtr)();
        }
        Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_NO_JOB;
  
      }
      else
      {
        /* Update the physical address if end of Bank0 is reached */
        /* Check whether PageStartAddress is reached to end of Bank0 */
        if((StatePtr->FlsWriteAddress ==
                              (DFLASH_BANK0_PHYSICAL + FLS_SECTOR_SIZE))
            &&
           (BankType != BANK1)
          )
        {
          StatePtr->FlsWriteAddress = DFLASH_BANK1_PHYSICAL;
          PhysicalAddress = DFLASH_BANK1_PHYSICAL;
          StatePtr->FlsJobType[BANK0] = FLS_NONE;
        }
  
        #if (FLS_IFX_FEE_USED == STD_OFF)
        #if  ( FLS_DEV_ERROR_DETECT == STD_ON )
        /* CHECK FOR DFLASHx PAGE IS ERASED */
        /*IFX_MISRA_RULE_11_01_STATUS=Flash address is typecased to a pointer*/
        /*IFX_MISRA_RULE_11_03_STATUS=Flash address is typecased to a pointer*/
        if(Fls_lPageEraseCheck((uint32 *)(void*)(StatePtr->FlsWriteAddress),
                             StatePtr->FlsWriteLength,FLS_SID_MAIN) == E_NOT_OK)
        {
          Error = (boolean)TRUE;
          Fls_lErrorHandler(FLS_WRITE);
        }
        /* Check needed only when DET is STD_ON */
        if(Error == (boolean)FALSE)
        #endif /* FLS_DEV_ERROR_DETECT == STD_ON */
        #endif /* FLS_IFX_FEE_USED == STD_OFF */
  
        {
          /* Start Write operation */
          Fls_lMainWriteJobStart(PhysicalAddress);
        }
      }
    }
    else
    {
      /* Call to Fls_lErrorHandler local function */
      Fls_lErrorHandler(FLS_WRITE);
    }
  }
}/* end of Fls_lMainWrite */

/*******************************************************************************
** Syntax : static void Fls_lMainWriteJobStart(uint32 PhysicalAddress)        **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non-reentrant                                                 **
**                                                                            **
** Parameters (in) : Physical Address - Physical address for Write            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Performs the Write operation during Interrupt/Cyclic         **
**               Job handling                                                 **
*******************************************************************************/
static void Fls_lMainWriteJobStart(uint32 PhysicalAddress)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint32                  SeqProtErr;
  uint8                   WriteMode;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  /* Mark that the Job has been started */
  StatePtr->JobStarted.Write = 1U;

  /*Enter Critical Section*/
  Fls_lSchMEnter_Main();

  /************* Call Write command sequence *************/
  WriteMode = Fls_lGetWriteMode();
  Fls_CallWriteCommand(PhysicalAddress,StatePtr,WriteMode);
  
  /* Exit Crtical Section */
  Fls_lSchMExit_Main();

  #ifdef FLS_RAM_EMULATION_TEST
  #if (FLS_RAM_EMULATION_TEST == 1U)
  {
    (void) FlsEmulation_Write(StatePtr->FlsWriteAddress,
                              StatePtr->FlsWriteBufferPtr);
  }
  #endif
  #endif

  SeqProtErr = Fls_lChkSeqProtErrors();

  /* Check for Sequence error,Protection error bit */
  if(SeqProtErr != 0U)
  {
    /* Report Production errors to DEM */
    Fls_lDemReportError_Write();
    
    /* Call to Fls_lErrorHandler local function */
    Fls_lErrorHandler(FLS_WRITE);
  }
    /* Check for Command Cycle Timeout Error */
  else if (StatePtr->FlsTimeoutErr == FLS_WRITE)
  {
    /* As per AI00157728 - timeout error need not be reported to DEM */
    /* Call to Fls_lErrorHandler local function */
    Fls_lErrorHandler(FLS_WRITE); 
  } 
  else
  {
    /* dummy else */
  }
}

/*******************************************************************************
** Traceability : [cover parentID=DS_AS_FLS234,DS_AS_FLS233,DS_AS_FLS235]
                 [/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_Isr(void)                                         **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : void                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : This routine is used instead of Main function if configured  **
** for Erase and Write Jobs.                                                  **
*******************************************************************************/
#if( FLS_USE_INTERRUPTS == STD_ON)
void Fls_17_Pmu_Isr(void)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8 *ArrPtr;
  uint32 FlashFsrStatus;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  ArrPtr = StatePtr->FlsJobType;

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if( (ArrPtr[BANK0] != FLS_NONE) ||
      (ArrPtr[BANK1] != FLS_NONE)
    )
  {
/************************** ERASE *********************************/
    /* Check for erase operation is pending in either of the bank */
    FlashFsrStatus = (uint32)(((unsigned_int)FLASH0_FSR.U & \
                                                    FLS_FSR_SPND_MASK) >> \
                                                       FLS_FSR_SPND_BITPOS);

    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    if(ArrPtr[BANK0] == FLS_ERASE)
    {
      if((Fls_lHWBusyCheck() == 0U) && (FlashFsrStatus == 0U))
      {
        /* Fls_Main_Erase operation should be called  */
        Fls_lMainErase((uint8)BANK0);
      }
    }
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    else if(ArrPtr[BANK1] == FLS_ERASE)
    {
      if((Fls_lHWBusyCheck() == 0U) && (FlashFsrStatus == 0U))
      {
        /* Fls_Main_Erase operation should be called  */
        Fls_lMainErase((uint8)BANK1);
      }
    }
    else
    {
      /* Avoid MISRA error */
    }

/************************** WRITE *********************************/
    /* Check for write operation is pending in either of the bank */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    if(ArrPtr[BANK0] == FLS_WRITE)
    {
      /* Fls_Main_Write operation should be called  */
      Fls_lMainWrite((uint8)BANK0);
    }
    /* Check for write operation is pending in either of the bank */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    else if(ArrPtr[BANK1] == FLS_WRITE)
    {
      /* Fls_Main_Write operation should be called  */
      Fls_lMainWrite((uint8)BANK1);
    }
    else
    {
      /* Avoid MISRA error */
    }
  }
}
/* FLS_USE_INTERRUPTS == STD_ON */
#endif

/*******************************************************************************
** Syntax : static void Fls_lErrorHandler(uint8 JobType)                      **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : JobType                                                  **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : This routine is called when error occurred during            **
**               flash operations                                             **
*******************************************************************************/
static void Fls_lErrorHandler(uint8 JobType)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8 *ArrPtr;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  ArrPtr = StatePtr->FlsJobType;

  if(JobType == FLS_ERASE)
  {
    #if( FLS_AC_LOAD_ON_JOB_START == STD_ON)
    Fls_lUnLoadAccessCode(FLS_ERASE);
    #endif
    StatePtr->JobStarted.Erase = 0U;
  }
  else if(JobType == FLS_WRITE)
  {
    #if( FLS_AC_LOAD_ON_JOB_START == STD_ON)
    Fls_lUnLoadAccessCode(FLS_WRITE);
    #endif
    StatePtr->JobStarted.Write = 0U;
  }
  else
  {
    /* Avoid MISRA 60 */
  }

  /* Clear the failed Job from Bank0 */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if(ArrPtr[BANK0] == JobType)
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    ArrPtr[BANK0] = FLS_NONE;
  }
  /* Clear the failed Job from Bank1 */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if(ArrPtr[BANK1] == JobType)
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
      contents*/
    ArrPtr[BANK1] = FLS_NONE;
  }

  /* Update the global status variables */
  StatePtr->FlsJobResult = MEMIF_JOB_FAILED;

  /* Execute Clear Status command */
  #ifndef IFX_FLS_DEBUG 
  Fls_ClearStatusCmdCycle();
  #endif

  Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = JobType;

  if(Fls_ConfigPtr->FlsJobErrorNotificationPtr != NULL_PTR)
  {
    /* FlsJobErrorNotificationPtr should be called */
    (Fls_ConfigPtr->FlsJobErrorNotificationPtr)();
  }

  Fls_ConfigPtr->FlsStateVarPtr->NotifCaller = FLS_NO_JOB;
}

#if(FLS_AC_LOAD_ON_JOB_START == STD_ON)
/*******************************************************************************
** Syntax : static void Fls_lLoadAccessCode(uint8 JobRequest)                 **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : JobRequest                                               **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : This routine is called to load the flash access code         **
**               in to RAM.                                                   **
*******************************************************************************/
static void Fls_lLoadAccessCode(uint8 JobRequest)
{
  uint32* SrcStartPtr;
  uint32* DestPtr;
  uint32* SrcEndPtr;
  volatile uint32 Size;

  /* Loading Write Access Code */
  if(JobRequest == FLS_WRITE)
  {
    SrcStartPtr = (uint32*)(void*)(&FLS_AC_WRITE_SOURCE_START);
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to pointer*/
    DestPtr = (uint32*)(void*)(Fls_ConfigPtr->FlsAccessCodeWritePtr);
    SrcEndPtr = (uint32*)(void*)(&FLS_AC_WRITE_SOURCE_END);
  }
  /* Loading Erase Access Code */
  else
  {
    SrcStartPtr = (uint32*)(void*)(&FLS_AC_ERASE_SOURCE_START);
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to pointer*/
    DestPtr = (uint32*)(void*)(Fls_ConfigPtr->FlsAccessCodeErasePtr);
    SrcEndPtr = (uint32*)(void*)(&FLS_AC_ERASE_SOURCE_END);
  }

  /*IFX_MISRA_RULE_11_01_STATUS=Pointer is typecasted to uint32 for performing
    arithmetic operation*/
  /*IFX_MISRA_RULE_11_01_STATUS=Pointer is typecasted to uint32 for performing
    arithmetic operation*/
  /*IFX_MISRA_RULE_11_03_STATUS=Pointer is typecasted to uint32 for performing
    arithmetic operation*/
  /*IFX_MISRA_RULE_11_03_STATUS=Pointer is typecasted to uint32 for performing
    arithmetic operation*/
  Size = (uint32)SrcEndPtr - (uint32)SrcStartPtr;

  /* Copy one more word for cases where actual size is not divisible by 4*/
  if((Size & THREE_VALUE) != 0U)
  {
    Size = Size + FOUR_VALUE;
  }
  
  /* Divide the Size by 4 to get Size in words */
  Size = Size >> (uint32)TWO_VALUE; 

  /* Copy access code to RAM */
  do
  {
    *DestPtr = *SrcStartPtr;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used for efficient
      access of memory*/
    DestPtr++;
    /*IFX_MISRA_RULE_17_01_STATUS=Pointer arithmetic is used for efficient
      access of memory*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used for efficient
      access of memory*/
    SrcStartPtr++;
    Size--;
  }while(Size != 0U);
}

/*******************************************************************************
** Syntax : static void Fls_lUnLoadAccessCode(uint8 JobRequest)               **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : JobRequest                                               **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : This routine is called to unload the flash access code.      **
**               Basically sets the RAM code area to 0                        **
*******************************************************************************/
static void Fls_lUnLoadAccessCode(uint8 JobRequest)
{
  uint32* SrcStartPtr;
  uint32* DestPtr;
  uint32* SrcEndPtr;
  volatile uint32 Size;

  /* UnLoading Write Access Code */
  if(JobRequest == FLS_WRITE)
  {
    SrcStartPtr = (uint32*)(void*)(&FLS_AC_WRITE_SOURCE_START);
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to pointer*/
    DestPtr = (uint32*)(void*)(Fls_ConfigPtr->FlsAccessCodeWritePtr);
    SrcEndPtr = (uint32*)(void*)(&FLS_AC_WRITE_SOURCE_END);
  }
  /* UnLoading Erase Access Code */
  else
  {
    SrcStartPtr = (uint32*)(void*)(&FLS_AC_ERASE_SOURCE_START);
    /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to pinter*/
    DestPtr = (uint32*)(void*)(Fls_ConfigPtr->FlsAccessCodeErasePtr);
    SrcEndPtr = (uint32*)(void*)(&FLS_AC_ERASE_SOURCE_END);
  }

  /*IFX_MISRA_RULE_11_01_STATUS=Pointer is converted to uint32 for performing
    arithmetic operation*/
  /*IFX_MISRA_RULE_11_01_STATUS=Pointer is converted to uint32 for performing
    arithmetic operation*/
  /*IFX_MISRA_RULE_11_03_STATUS=Pointer is converted to uint32 for performing
    arithmetic operation*/
  /*IFX_MISRA_RULE_11_03_STATUS=Pointer is converted to uint32 for performing
    arithmetic operation*/
  Size = (uint32)SrcEndPtr - (uint32)SrcStartPtr;

  /* Copy one more word for cases where actual size is not divisible by 4*/
  if((Size & THREE_VALUE) != 0U)
  {
    Size = Size + FOUR_VALUE;
  }
  
  /* Divide the Size by 4 to get Size in words */
  Size = Size >> (uint32)TWO_VALUE;

  do
  {
    *DestPtr = ZERO_VALUE;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used for efficient
      access of memory*/
    DestPtr++;
    Size--;
  }while(Size != 0U);
}
#endif
/* End of FLS_AC_LOAD_ON_JOB_START == STD_ON */

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE boolean Fls_lSpndTimeout(uint32 TimeOutCount)     **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : TimeOutCount - Timeout count                             **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks if erase suspend error or timeout error  **
**               occured                                                      **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fls_lSpndTimeout(uint32 TimeOutCount)
{
  boolean RetVal;
  uint32 Checkspnderrbit;
  Checkspnderrbit =  FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_MARD.U);
  Checkspnderrbit =  Checkspnderrbit & FLS_MARD_SPNDERR_MASK ;
  Checkspnderrbit = Checkspnderrbit >> FLS_MARD_SPNDERR_BITPOS ; 
  #ifdef IFX_FLS_DEBUG03
  if (((( Checkspnderrbit) | \
           TestFls_DebugMask03) == 1U) || (TimeOutCount == 0U))
                                   
  #else
  if ((Checkspnderrbit == 1U) || (TimeOutCount == 0U))
  #endif
  {
    RetVal = (boolean)TRUE; 
  }
  else
  {
    RetVal = (boolean)FALSE; 
  }
  return (RetVal);
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE boolean Fls_lResmErTimeout(uint32 TimeOutCount)   **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : TimeOutCount - Timeout count                             **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks if erase resume timeout error occured    **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fls_lResmErTimeout(uint32 TimeOutCount)
{
  boolean RetVal;
  
  #ifdef IFX_FLS_DEBUG04
  TimeOutCount = TimeOutCount & TestFls_DebugMask04;
  #endif

  if (TimeOutCount == 0U)
  {
    RetVal = (boolean)TRUE; 
  }
  else
  {
    RetVal = (boolean)FALSE; 
  }
  return (RetVal);
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE boolean Fls_lPverChk(void)                        **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks if PVER error occured                    **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fls_lPverChk(void)
{
  uint32 TempFSR;
  boolean RetVal;
  
  #ifdef IFX_FLS_DEBUG
  TempFSR = ((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) | \
                                                     TestFls_DebugMask);
  #else
  TempFSR = (uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U);
  #endif

  if ((TempFSR & FLS_PVER_ERROR) == FLS_PVER_ERROR)
  {
    RetVal = (boolean)TRUE; 
  }
  else
  {
    RetVal = (boolean)FALSE; 
  }
  return (RetVal);
}

#if(FLS_DEV_ERROR_DETECT == STD_ON)
/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Read                           **
**                                ( const Fls_17_Pmu_StateType* StatePtr)     **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : StatePtr : FLS Global variable structure                 **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : ErrorFlag - DET has occured (1) or not (0)               **
**                                                                            **
** Description : This routine detects if the new Read job can be accepted     **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Read( const Fls_17_Pmu_StateType* StatePtr )
{
  uint8 ErrorFlag;

  const uint8 *ArrPtr;
  ArrPtr = StatePtr->FlsJobType;
  ErrorFlag = 0U;

  /* No other job has been accepted */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if((ArrPtr[0] != FLS_NONE) || (ArrPtr[1] != FLS_NONE))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID,
                     FLS_17_PMU_INSTANCE_ID,
                     FLS_SID_READ,
                     FLS_E_BUSY);/* Report to DET */
    ErrorFlag = 1U;
  }

  return(ErrorFlag);
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Compare                        **
**                                   ( const Fls_17_Pmu_StateType* StatePtr)  **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : StatePtr : FLS Global variable structure                 **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : ErrorFlag - DET has occured (1) or not (0)               **
**                                                                            **
** Description : This routine detects if the new Compare job can be accepted  **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Compare( 
                                         const Fls_17_Pmu_StateType* StatePtr )
{
  uint8 ErrorFlag;
  const uint8 *ArrPtr;
  ArrPtr = StatePtr->FlsJobType;
  ErrorFlag = 0U;

  /* No other job has been accepted */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if((ArrPtr[0] != FLS_NONE) || (ArrPtr[1] != FLS_NONE))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID,
                     FLS_17_PMU_INSTANCE_ID,
                     FLS_SID_COMPARE,
                     FLS_E_BUSY);/* Report to DET */
    ErrorFlag = 1U;
  }

  return(ErrorFlag);
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Write                          **
**                                 ( const Fls_17_Pmu_StateType* StatePtr )   **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : StatePtr : FLS Global variable structure                 **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : ErrorFlag - DET has occured (1) or not (0)               **
**                                                                            **
** Description : This routine detects if the new Write job can be accepted    **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Write(const Fls_17_Pmu_StateType* StatePtr)
{
  uint8 ErrorFlag;
  const uint8 *ArrPtr;
  ArrPtr = StatePtr->FlsJobType;
  ErrorFlag = 0U;

  /* No other job has been accepted */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if((ArrPtr[0] != FLS_NONE) || (ArrPtr[1] != FLS_NONE))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID,
                     FLS_17_PMU_INSTANCE_ID,
                     FLS_SID_WRITE,
                     FLS_E_BUSY);/* Report to DET */
    ErrorFlag = 1U;
  }

  return(ErrorFlag);
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Erase                          **
**                                 ( const Fls_17_Pmu_StateType* StatePtr)    **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : StatePtr : FLS Global variable structure                 **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : ErrorFlag - DET has occured (1) or not (0)               **
**                                                                            **
** Description : This routine detects if the new Erase job can be accepted    **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fls_lBusyDET_Erase(const Fls_17_Pmu_StateType* StatePtr)
{
  uint8 ErrorFlag;

  const uint8 *ArrPtr;
  ArrPtr = StatePtr->FlsJobType;
  ErrorFlag = 0U;

  /* No other job has been accepted */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    FlsJobType[FLS_NUMBER_OF_SECTORS] to efficiently access its 
    contents*/
  if((ArrPtr[0] != FLS_NONE) || (ArrPtr[1] != FLS_NONE))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID,
                     FLS_17_PMU_INSTANCE_ID,
                     FLS_SID_ERASE,
                     FLS_E_BUSY);/* Report to DET */
    ErrorFlag = 1U;
  }

  return(ErrorFlag);
}

/*******************************************************************************
** Syntax: static uint8 Fls_lUninitDET( const uint8 ServiceID )               **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : JobResult                                                **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : ErrorFlag - DET has occured (1) or not (0)               **
**                                                                            **
** Description : This routine performs the UNINIT DET check                   **
*******************************************************************************/
static uint8 Fls_lUninitDET( const uint8 ServiceID )
{
  uint8 ErrorFlag;

  ErrorFlag = 0U;
  #if (FLS_PB_FIXEDADDR == STD_OFF)
  if(Fls_ConfigPtr == NULL_PTR)
  #else
  if(Fls_ConfigPtr->FlsStateVarPtr->DriverInitialised == (boolean)FALSE)
  #endif
  {
     /* Report to DET */
     Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                       FLS_17_PMU_INSTANCE_ID,ServiceID,
                      FLS_E_UNINIT);/* Report to DET */

     ErrorFlag = 1U;
  }

  return (ErrorFlag);
}

/*******************************************************************************
** Syntax: static uint8 Fls_lTimeOutDET( const uint8 ServiceID )              **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : ServiceID                                                **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : ErrorFlag - DET has occured (1) or not (0)               **
**                                                                            **
** Description : This routine performs the TIMEOUT DET check                  **
*******************************************************************************/
static uint8 Fls_lTimeOutDET(const uint8 ServiceID)
{
  Fls_17_Pmu_StateType  *StatePtr;
  uint8 ErrorFlag;
  
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  ErrorFlag = 0U;
  
  if (StatePtr->FlsTimeoutControl != 0U)
  {
    if(StatePtr->JobStarted.Erase != 0U)
    {
      if(StatePtr->FlsEraseTimeoutCycleCount == 0U)
      {
        ErrorFlag = 1U;
      }
      else
      {
        StatePtr->FlsEraseTimeoutCycleCount--;
      }
    }
    
    if(StatePtr->JobStarted.Write != 0U)
    {
      if(StatePtr->FlsWriteTimeoutCycleCount == 0U)
      {
        ErrorFlag = 1U;
      }
      else
      {
        StatePtr->FlsWriteTimeoutCycleCount--;
      }
    }
    
    if(ErrorFlag == 1U)
    {
      /* Report timeout DEM */
      Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID,ServiceID,
                     FLS_E_TIMEOUT);/* Report to DET */
    }
  }
  
  return ErrorFlag;
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE boolean Fls_lCheckWrittenPage(uint32 PageLength)  **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : PageLength                                               **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : TRUE - programing error , FALSE - No programming error   **
**                                                                            **
** Description : Checks if the page was written without errors                **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fls_lCheckWrittenPage(uint32 PageLength)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint32          *PageStartAddressPtr;
  const uint32    *SourcePtr;
  uint32          Count;
  boolean         Error;
  
  Count = 0U;
  Error = (boolean)FALSE;
  
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  SourcePtr = (const uint32 *)(const void *)(StatePtr->FlsWriteBufferPtr);
  /*IFX_MISRA_RULE_11_01_STATUS=Flash address is converted to pointer*/
  /*IFX_MISRA_RULE_11_03_STATUS=Flash address is converted to pointer*/
  PageStartAddressPtr = (uint32 *)(void*)(StatePtr->FlsWriteAddress);
  do
  {
    if(*(PageStartAddressPtr)== *(SourcePtr))
    {
      Count++;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
        access the memory*/
      PageStartAddressPtr++;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
        access the memory*/
      SourcePtr++;
    }
    else
    {
      /* FLS056: If the block has been not completely reprogrammed  */
      /*         after Writing a Flash block.                       */
      #if (FLS_DEV_ERROR_DETECT == STD_ON)
      Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                       FLS_17_PMU_INSTANCE_ID,FLS_SID_MAIN,
                       FLS_E_VERIFY_WRITE_FAILED);/* Report to DET */
      #endif
      /* Call to Fls_lErrorHandler local function */
      Error = (boolean)TRUE;
      /*Nothing more to compare */
      Count = (PageLength/FLS_WORD_SIZE);
    }
  }while(Count < (PageLength/FLS_WORD_SIZE));
  
  if(Error == (boolean)FALSE)
  {
    /* Check for Non-correctable ECC errors during the above check */
    if(Fls_lChkBitErrors())
    {
       /* Report to DET */
       #if (FLS_DEV_ERROR_DETECT == STD_ON)           
       Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID,FLS_17_PMU_INSTANCE_ID,
                        FLS_SID_MAIN,FLS_E_VERIFY_WRITE_FAILED);
       #endif
       Error = (boolean)TRUE;
    }
  }
  return Error;
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE void Fls_lCalculateWriteTimeoutCount(void)        **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : None                                                     **
**                                                                            **
** Description : Calculates the write timeout count                           **
*******************************************************************************/
IFX_LOCAL_INLINE void Fls_lCalculateWriteTimeoutCount(void)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint32          Length;
  uint32          WriteTimeOutCount;
  uint32          PageStartAddress;

  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  
  PageStartAddress = StatePtr->FlsWriteAddress;
  Length = StatePtr->FlsWriteLength;
  /* Update Timeout count for write */
  WriteTimeOutCount =
        (uint32)(FLS_17_PMU_WRITE_TIME / Fls_ConfigPtr->FlsCallCycle);
  if(WriteTimeOutCount == 0U)
  {
    WriteTimeOutCount = 1U;
  }
  else 
  { 
    if((FLS_17_PMU_WRITE_TIME % Fls_ConfigPtr->FlsCallCycle) != 0U)
    {
      WriteTimeOutCount++;
    }
  }

  StatePtr->FlsWriteTimeoutCycleCount = WriteTimeOutCount;

  while(Length > FLS_PAGE_SIZE)
  {
    if( (Length <= (FLS_BURST_PAGE_SIZE - FLS_PAGE_SIZE)) ||
        ((PageStartAddress & (FLS_BURST_PAGE_SIZE - 1U)) != 0U) )
    {
      Length -= FLS_PAGE_SIZE;
      PageStartAddress += FLS_PAGE_SIZE;
    }
    else
    {
      Length -= FLS_BURST_PAGE_SIZE;
      PageStartAddress += FLS_BURST_PAGE_SIZE;
    }
    StatePtr->FlsWriteTimeoutCycleCount += WriteTimeOutCount;
  }
  
  /* Add one extra count since MainFunction needs to be called once after
     all the data is written for checking errors and to set status */
  StatePtr->FlsWriteTimeoutCycleCount += 1U;
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE Std_ReturnType Fls_lPageEraseCheck(               **
**                                            const uint32* AddrCheckPtr,     **
**                                                  uint32  Length,           **
**                                            const uint8   ServiceID)        **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : AddrCheckPtr - address of the page, Length - page length **
**                   ServiceID - Service ID of the API for the DET            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : E_OK - page was erased correctly                         **
**                   E_NOT_OK - Page was not erased correctly                 **
**                                                                            **
** Description : Checks if the page was erased correctly                      **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fls_lPageEraseCheck(const uint32 * AddrCheckPtr,
                                                    uint32 Length,
                                                    const uint8 ServiceID)
{
  Std_ReturnType  RetVal;
  uint8           Loop;
  uint8           PageLength;
  
  RetVal = E_OK;
  
  Loop = 0U;
  
  /*IFX_MISRA_RULE_11_01_STATUS=Pointer is typecasted to uint32 for performing
    bit-wise operation*/
  /*IFX_MISRA_RULE_11_03_STATUS=Pointer is typecasted to uint32 for performing
    bit-wise operation*/
  if( (Length <= (FLS_BURST_PAGE_SIZE - FLS_PAGE_SIZE)) ||
      ((((uint32)AddrCheckPtr) & (FLS_BURST_PAGE_SIZE - 1U)) != 0U) )
  {
    PageLength = FLS_PAGE_SIZE;
  }
  else
  {
    PageLength = FLS_BURST_PAGE_SIZE;
  }
  /* convert page length in bytes to Word Length */
  PageLength = PageLength/FLS_WORD_SIZE;
  do
  {
    if((*(AddrCheckPtr)) == 0U)
    {
       Loop++;
       /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
         access the memory*/
       AddrCheckPtr++;
    }
    else
    {
       /* FLS055: Before Writing if block is not completely erased.*/
       /* Call to Fls_lErrorHandler local function */
       #if (FLS_DEV_ERROR_DETECT == STD_ON)
       Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                        FLS_17_PMU_INSTANCE_ID,ServiceID,
                        FLS_E_VERIFY_ERASE_FAILED);/* Report to DET */
       #endif

       RetVal = E_NOT_OK; /* Return from FSunction */
    }
  } while((RetVal == E_OK) && (Loop < PageLength));
  
  return RetVal;
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE Std_ReturnType Fls_lCheckEraseDet(               **
**                                         Fls_AddressType TargetAddress,     **
**                                         Fls_LengthType Length)             **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  TargetAddress and Length                                **
**                                                                            **
** Parameters (out): E_OK: No development Error detected                      **
**                   E_NOT_OK: development Error is detected                  **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : This routine checks for development error for                **
**               Fls_17_Pmu_Erase API                                         **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fls_lCheckEraseDet(
                                                Fls_AddressType TargetAddress,
                                                Fls_LengthType Length)
{
  Fls_17_Pmu_StateType*   StatePtr;
  Std_ReturnType   ReturnValue;

  /* API called before initialization? */
  if(Fls_lUninitDET(FLS_SID_ERASE) == 1U)
  {
    ReturnValue = E_NOT_OK;
  }

  /* Checking whether given TargetAddress is valid or not */
  else if(!(((FLS_BASE_ADDRESS+(uint32)TargetAddress) == DFLASH_BANK0_LINEAR) ||
        ((FLS_BASE_ADDRESS+(uint32)TargetAddress) == DFLASH_BANK1_LINEAR)))
  {
    /* Report to DET */
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID,
                     FLS_SID_ERASE,
                     FLS_E_PARAM_ADDRESS);

    ReturnValue = E_NOT_OK;
  }

  /* Checking whether given Length is valid or not */
  else if( (Length == (Fls_LengthType)0) ||
      (!(((TargetAddress + Length) == FLS_SECTOR_SIZE) ||
       ((TargetAddress + Length) == FLS_TOTAL_SIZE))))
  {
    /* Report to DET */
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                      FLS_17_PMU_INSTANCE_ID,FLS_SID_ERASE,
                      FLS_E_PARAM_LENGTH);

    ReturnValue = E_NOT_OK;
  }
  else
  {
    StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

    if(Fls_lBusyDET_Erase(StatePtr) == 1U)
    {
      ReturnValue = E_NOT_OK;
    }
    else
    {
      ReturnValue = E_OK;
    }
  }

  return ReturnValue;
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE Std_ReturnType Fls_lCheckWriteDet(               **
**                                         Fls_AddressType TargetAddress,     **
**                                         const uint8 *SourceAddressPtr,     **
**                                         Fls_LengthType Length)             **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  TargetAddress, SourceAddressPtr and Length              **
**                                                                            **
** Parameters (out): E_OK: No development Error detected                      **
**                   E_NOT_OK: development Error is detected                  **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : This routine checks for development error for                **
**               Fls_17_Pmu_Write API                                         **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fls_lCheckWriteDet(
                                                 Fls_AddressType TargetAddress,
                                                 const uint8 *SourceAddressPtr,
                                                 Fls_LengthType Length)
{
  Std_ReturnType  RetVal;

  RetVal = E_OK;

  /* API called before initialization */
  if(Fls_lUninitDET(FLS_SID_WRITE) == 1U)
  {
    RetVal = E_NOT_OK;
  }
  /* Checking whether given TargetAddress is valid or not */
  else if( (TargetAddress > (FLS_TOTAL_SIZE - FLS_PAGE_SIZE)) ||
         ((TargetAddress & PAGE_SIZE_MASK) != 0U) )
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID,FLS_SID_WRITE,
                     FLS_E_PARAM_ADDRESS);/* Report to DET */

    RetVal = E_NOT_OK;
  }
  /* Checking whether given Length is valid or not */
  else if((Length == 0U) || (((Length & PAGE_SIZE_MASK) != 0U)
          || ((TargetAddress + Length) > FLS_TOTAL_SIZE)))
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                     FLS_17_PMU_INSTANCE_ID,FLS_SID_WRITE,
                     FLS_E_PARAM_LENGTH);/* Report to DET */

    RetVal = E_NOT_OK;
  }
  /* Check for Pointer to an application buffer */
  else if(SourceAddressPtr == NULL_PTR)
  {
    Det_ReportError( (uint16)FLS_17_PMU_MODULE_ID, 
                      FLS_17_PMU_INSTANCE_ID,FLS_SID_WRITE,
                     FLS_E_PARAM_DATA);/* Report to DET */

    RetVal = E_NOT_OK;
  }
  else
  {
    /* dummy else */
  }

  return RetVal;
}

#endif
/* FLS_DEV_ERROR_DETECT == STD_ON */

/*******************************************************************************
** Syntax:IFX_LOCAL_INLINE uint8 Fls_lGetWriteMode(void);                     **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : PAGE_WRITE - Page write to be done                       **
**                   BURST_WRITE - Burst write to be done                     **
**                                                                            **
** Description : This function checks whether Burst Write can be performed or **
**               Page Write has to be performed                               **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fls_lGetWriteMode(void)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint8           RetVal;
 
  RetVal = BURST_WRITE;
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  
  if( (StatePtr->FlsWriteLength <= (FLS_BURST_PAGE_SIZE - FLS_PAGE_SIZE)) ||
       ((StatePtr->FlsWriteAddress & (FLS_BURST_PAGE_SIZE - 1U)) != 0U) )
  {
    RetVal = PAGE_WRITE;
  }
  
  return RetVal;
}

/*******************************************************************************
** Syntax:IFX_LOCAL_INLINE Fls_LengthType Fls_lGetReadModeLength(void);       **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : max read length as per read mode                         **
**                                                                            **
** Description : Get max read length according to the FLS mode                **
*******************************************************************************/
IFX_LOCAL_INLINE Fls_LengthType Fls_lGetReadModeLength(void)
{
  Fls_17_Pmu_StateType   *StatePtr;
  Fls_LengthType  MaxRead;
 
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;
  MaxRead = Fls_ConfigPtr->FlsFastRead;
  
  if(StatePtr->FlsMode == MEMIF_MODE_SLOW)
  {
    MaxRead = Fls_ConfigPtr->FlsSlowRead;
  }
  
  return MaxRead;
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint32 Fls_lChkSeqProtErrors(void)                **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : Seq and protection bits of FSR                           **
**                                                                            **
** Description : Checks for seq and protection errors                         **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fls_lChkSeqProtErrors(void)
{
  uint32 RetVal;
  #ifndef FLS_RAM_EMULATION_TEST
  #ifdef IFX_FLS_DEBUG
  RetVal =(((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) | \
                              TestFls_DebugMask) & FLS_PROER_SQER_ERROR);
  #else
  RetVal = ((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
                                                   FLS_PROER_SQER_ERROR);
  #endif
  #else
  RetVal = (FlsEmulation_ReadStatusRegister() & FLS_PROER_SQER_ERROR);
  #endif
 
 return RetVal;
}


/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint32 Fls_lChkOperError(void)                    **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : operational error bit of FSR                             **
**                                                                            **
** Description : Checks for operational error                                 **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fls_lChkOperError(void)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint32 RetVal;
  
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  #ifndef FLS_RAM_EMULATION_TEST
  #ifdef IFX_FLS_DEBUG
  RetVal = (( (uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) | \
                                     TestFls_DebugMask) & FLS_OPER_ERROR);
  #else
  RetVal = ((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
                                                        FLS_OPER_ERROR);
  #endif
  #else 
  RetVal = (FlsEmulation_ReadStatusRegister() & FLS_OPER_ERROR);
  #endif
  
  if(RetVal != 0U)
  {
    StatePtr->FlsOper = 1U;
  }
  
  if(StatePtr->FlsOper != 0U)
  {
    RetVal = FLS_OPER_ERROR;
    if(Fls_ConfigPtr->FlsIllegalStateNotificationPtr != NULL_PTR)
    {
       /* FlsIllegalStateNotificationPtr should be called */
       (Fls_ConfigPtr->FlsIllegalStateNotificationPtr)();
    }
  }
  
  return (RetVal);
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint32 Fls_lChkOperError_initmode(void)           **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : operational error bit of FSR                             **
**                                                                            **
** Description : Checks for operational error in init stage                   **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fls_lChkOperError_initmode(void)
{
  Fls_17_Pmu_StateType   *StatePtr;
  uint32 RetVal;
  
  StatePtr = Fls_ConfigPtr->FlsStateVarPtr;

  #ifndef FLS_RAM_EMULATION_TEST
  #ifdef IFX_FLS_DEBUG
  RetVal = (( (uint32)FLS_SFR_INIT_USER_MODE_READ32(FLASH0_FSR.U) | \
                                     TestFls_DebugMask) & FLS_OPER_ERROR);
  #else
  RetVal = ((uint32)FLS_SFR_INIT_USER_MODE_READ32(FLASH0_FSR.U) & \
                                                        FLS_OPER_ERROR);
  #endif
  #else 
  RetVal = (FlsEmulation_ReadStatusRegister() & FLS_OPER_ERROR);
  #endif
  
  if(RetVal != 0U)
  {
    StatePtr->FlsOper = 1U;
  }
  
  if(StatePtr->FlsOper != 0U)
  {
    RetVal = FLS_OPER_ERROR;
    if(Fls_ConfigPtr->FlsIllegalStateNotificationPtr != NULL_PTR)
    {
       /* FlsIllegalStateNotificationPtr should be called */
       (Fls_ConfigPtr->FlsIllegalStateNotificationPtr)();
    }
  }
  
  return (RetVal);
}
/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint32 Fls_lChkBitErrors(void)                    **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : Non correctable bit of FSR                               **
**                                                                            **
** Description : Checks for double ECC errors                                 **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fls_lChkBitErrors(void)
{
  uint32 RetVal;
  #ifndef FLS_RAM_EMULATION_TEST
  #ifdef IFX_FLS_DEBUG
  RetVal = (( (uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) | \
                                     TestFls_DebugMask ) & FLS_BIT_ERROR);
  #else
  RetVal = ((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
                                                           FLS_BIT_ERROR);
  #endif
  #else
  RetVal = (FlsEmulation_ReadStatusRegister() & FLS_BIT_ERROR);
  #endif
 
 return RetVal;
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint32 Fls_lHWBusyCheck(void)                     **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : Busy bit of FSR                                          **
**                                                                            **
** Description : Checks for Busy status of the Data Flash Bank                **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fls_lHWBusyCheck(void)
{
  uint32 RetVal;

  #ifdef IFX_FLS_DEBUG07
  RetVal = (( (uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) | \
                                        TestFls_DebugMask07 ) & FLS_D0BUSY);
  #else
  #ifdef IFX_FLS_DEBUG08
  RetVal = (( (uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
                                     TestFls_DebugMask08 ) & FLS_D0BUSY);
  #else
  RetVal = ((uint32)FLS_SFR_RUNTIME_USER_MODE_READ32(FLASH0_FSR.U) & \
                                                            FLS_D0BUSY);
  #endif
  #endif
 
  return RetVal;
}


#define FLS_17_PMU_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
