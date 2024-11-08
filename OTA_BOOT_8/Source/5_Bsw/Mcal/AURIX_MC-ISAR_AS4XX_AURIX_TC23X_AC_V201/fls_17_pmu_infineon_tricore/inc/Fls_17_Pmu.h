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
**  $FILENAME   : Fls_17_Pmu.h $                                             **
**                                                                           **
**  $CC VERSION : \main\62 $                                                 **
**                                                                           **
**  $DATE       : 2017-04-11 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION  : This file exports funtionality of Flash driver            **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/******************************************************************************
   Traceability:
                 [cover parentID=DS_AS_FLS353,DS_AS_FLS308_FLS309,
                  DS_AS_FLS352_FLS354_FLS355,DS_AS_FLS_PR2112,
                  DS_AS_FLS109_1,DS_NAS_FLS_PR916,DS_AS_FLS_PR2113,
                  DS_AS_FLS109_2,DS_AS_FLS161,DS_AS403_FLS004,DS_AS_FLS160,
                  DS_AS403_FLS320,DS_AS403_FLS321,DS_AS_FLS216_FLS217,
                  DS_AS_FLS262,DS_AS_FLS263,DS_AS_FLS_PR2114,
                  DS_AS_FLS213_1,DS_AS_FLS213_2,SAS_NAS_ALL_PR746,
                  DS_AS403_FLS302,DS_AS403_FLS303,DS_AS403_FLS304,
                  SAS_NAS_ALL_PR748,SAS_NAS_ALL_PR630_PR631,
                  DS_NAS_EP_FLS_PR3054,DS_NAS_HE2_FLS_PR3054]
                 [/cover]
******************************************************************************/

#ifndef  FLS_17_PMU_H
#define  FLS_17_PMU_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Inclusion of Platform_Types.h and Compiler.h  */
#include "Std_Types.h"

/* Global functions like Set/Reset END INIT protection bit, 
  Enable/Disable interrupts, Atomic write function */
#include "Mcal.h"

/* Fls Module Configuration header file */
#include "Fls_17_Pmu_Cfg.h"

/* Inclusion from Diagnostic Error Manager File */
#if ((FLS_E_COMPARE_FAILED_DEM_REPORT == ENABLE_DEM_REPORT) ||  \
     (FLS_E_ERASE_FAILED_DEM_REPORT == ENABLE_DEM_REPORT) ||    \
     (FLS_E_WRITE_FAILED_DEM_REPORT == ENABLE_DEM_REPORT) ||    \
     (FLS_E_READ_FAILED_DEM_REPORT == ENABLE_DEM_REPORT))
#include "Dem.h"

#endif

/* Conditional Inclusion of Developement Error Tracer File */
#if  ( FLS_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif

#if ((FLS_USER_MODE_INIT_API_ENABLE == STD_ON) || \
     (FLS_USER_MODE_RUNTIME_API_ENABLE == STD_ON) || \
     (FLS_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON))
           #include "Fls_Protect.h"
#endif

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/*
    Published paramters
*/ 

/* FLS Vendor ID */
#define FLS_17_PMU_VENDOR_ID                  ((uint16)17)  
/* FLS Module ID */
#define FLS_17_PMU_MODULE_ID                  ((uint16)92)

#if( FLS_DEV_ERROR_DETECT == STD_ON)
/* Development error values - DET's */
/* API service called with wrong parameter */
#define FLS_E_PARAM_CONFIG           ((uint8)1)
/* API service called with wrong/inconsistent Address(s)*/
#define FLS_E_PARAM_ADDRESS          ((uint8)2)
/* API service called with wrong/inconsistent length(s)*/
#define FLS_E_PARAM_LENGTH           ((uint8)3)
/* API service called with wrong/inconsistent Data(s)*/
#define FLS_E_PARAM_DATA             ((uint8)4)
/* API service called without module initalization */
#define FLS_E_UNINIT                 ((uint8)5)
/* API service called while    Driver is still busy  */
#define FLS_E_BUSY                   ((uint8)6)
/* Erase Verification (blank check) failed */
#define FLS_E_VERIFY_ERASE_FAILED    ((uint8)7)
/* Write Verification (compare) failed */
#define FLS_E_VERIFY_WRITE_FAILED    ((uint8)8)
/* Timeout for Erase/write jobs */
#define FLS_E_TIMEOUT                ((uint8)9)
/* API service called with NULL parameter */
#define FLS_E_PARAM_POINTER          ((uint8)10)


/* API Service ID's    */
#define FLS_SID_INIT           ((uint8)0)
#define FLS_SID_ERASE          ((uint8)1)
#define FLS_SID_WRITE          ((uint8)2)
#define FLS_SID_CANCEL         ((uint8)3)
#define FLS_SID_GETSTATUS      ((uint8)4)
#define FLS_SID_GETJOBRESULT   ((uint8)5)
#define FLS_SID_MAIN           ((uint8)6)
#define FLS_SID_READ           ((uint8)7)
#define FLS_SID_COMPARE        ((uint8)8)
#define FLS_SID_SETMODE        ((uint8)9)
#define FLS_SID_GETVERSIONINFO ((uint8)16)

#endif /* FLS_DEV_ERROR_DETECT */

/* Options of FlsJobType */
#define FLS_NONE      ((uint8)0)
#define FLS_WRITE     ((uint8)1)
#define FLS_ERASE     ((uint8)2)
#define FLS_READ      ((uint8)3)
#define FLS_COMPARE   ((uint8)4)
#define FLS_READSYNC  ((uint8)5)
#define FLS_CANCEL    ((uint8)6)
#define FLS_COMPARESYNC    ((uint8)7)
#define FLS_VERIFYERASE    ((uint8)8)

/* Bank Type */
#define BANK0 (0U)
#define BANK1 (1U)

#define PAGE_WRITE   (0U)
#define BURST_WRITE  (1U)

#define FLS_BURST_PAGE_SIZE (32U)
#define FLS_WORDLINE_SIZE   (512U)

#define FLS_SUBSECTOR_SIZE (FLS_SECTOR_SIZE/FLS_ERASE_SECTORS)


/* Options for DFLS Read Access Wait States.Note SysClk(Mhz)/ WaitStates <=63*/
#define FLS_WAIT_STATE_READACCESS1  (1U)
#define FLS_WAIT_STATE_READACCESS2  (2U)
#define FLS_WAIT_STATE_READACCESS3  (3U)
#define FLS_WAIT_STATE_READACCESS4  (4U)
#define FLS_WAIT_STATE_READACCESS5  (5U)
#define FLS_WAIT_STATE_READACCESS6  (6U)  /* Default Wait States */
#define FLS_WAIT_STATE_READACCESS7  (7U)
#define FLS_WAIT_STATE_READACCESS8  (8U)
#define FLS_WAIT_STATE_READACCESS9  (9U)
#define FLS_WAIT_STATE_READACCESS10 (10U)
#define FLS_WAIT_STATE_READACCESS11 (11U)  
#define FLS_WAIT_STATE_READACCESS12 (12U)
#define FLS_WAIT_STATE_READACCESS13 (13U)
#define FLS_WAIT_STATE_READACCESS14 (14U)  
#define FLS_WAIT_STATE_READACCESS15 (15U)
#define FLS_WAIT_STATE_READACCESS16 (16U)
#define FLS_WAIT_STATE_READACCESS17 (17U)
#define FLS_WAIT_STATE_READACCESS18 (18U)
#define FLS_WAIT_STATE_READACCESS19 (19U)
#define FLS_WAIT_STATE_READACCESS20 (20U)
#define FLS_WAIT_STATE_READACCESS21 (21U)
#define FLS_WAIT_STATE_READACCESS22 (22U)
#define FLS_WAIT_STATE_READACCESS23 (23U)
#define FLS_WAIT_STATE_READACCESS24 (24U)
#define FLS_WAIT_STATE_READACCESS25 (25U)
#define FLS_WAIT_STATE_READACCESS26 (26U)
#define FLS_WAIT_STATE_READACCESS27 (27U)
#define FLS_WAIT_STATE_READACCESS28 (28U)
#define FLS_WAIT_STATE_READACCESS29 (29U)
#define FLS_WAIT_STATE_READACCESS30 (30U)
#define FLS_WAIT_STATE_READACCESS31 (31U)
#define FLS_WAIT_STATE_READACCESS32 (32U)
#define FLS_WAIT_STATE_READACCESS33 (33U)
#define FLS_WAIT_STATE_READACCESS34 (34U)
#define FLS_WAIT_STATE_READACCESS35 (35U)
#define FLS_WAIT_STATE_READACCESS36 (36U)
#define FLS_WAIT_STATE_READACCESS37 (37U)
#define FLS_WAIT_STATE_READACCESS38 (38U)
#define FLS_WAIT_STATE_READACCESS39 (39U)
#define FLS_WAIT_STATE_READACCESS40 (40U)
#define FLS_WAIT_STATE_READACCESS41 (41U)
#define FLS_WAIT_STATE_READACCESS42 (42U)
#define FLS_WAIT_STATE_READACCESS43 (43U)
#define FLS_WAIT_STATE_READACCESS44 (44U)
#define FLS_WAIT_STATE_READACCESS45 (45U)
#define FLS_WAIT_STATE_READACCESS46 (46U)
#define FLS_WAIT_STATE_READACCESS47 (47U)
#define FLS_WAIT_STATE_READACCESS48 (48U)
#define FLS_WAIT_STATE_READACCESS49 (49U)
#define FLS_WAIT_STATE_READACCESS50 (50U)
#define FLS_WAIT_STATE_READACCESS51 (51U)
#define FLS_WAIT_STATE_READACCESS52 (52U)
#define FLS_WAIT_STATE_READACCESS53 (53U)
#define FLS_WAIT_STATE_READACCESS54 (54U)
#define FLS_WAIT_STATE_READACCESS55 (55U)
#define FLS_WAIT_STATE_READACCESS56 (56U)
#define FLS_WAIT_STATE_READACCESS57 (57U)
#define FLS_WAIT_STATE_READACCESS58 (58U)
#define FLS_WAIT_STATE_READACCESS59 (59U)
#define FLS_WAIT_STATE_READACCESS60 (60U)
#define FLS_WAIT_STATE_READACCESS61 (61U)
#define FLS_WAIT_STATE_READACCESS62 (62U)
#define FLS_WAIT_STATE_READACCESS63 (63U)

/* Options for DFLS Error Correction Wait States */
#define FLS_WAIT_STATE_ERRORCORRECTION0 (0U)
#define FLS_WAIT_STATE_ERRORCORRECTION1 (1U)
#define FLS_WAIT_STATE_ERRORCORRECTION2 (2U)
#define FLS_WAIT_STATE_ERRORCORRECTION3 (3U)
#define FLS_WAIT_STATE_ERRORCORRECTION4 (4U)
#define FLS_WAIT_STATE_ERRORCORRECTION5 (5U)
#define FLS_WAIT_STATE_ERRORCORRECTION6 (6U)
#define FLS_WAIT_STATE_ERRORCORRECTION7 (7U)

/* Wordline fail count to be suported */
#define FLS_WL_FAIL_COUNT (1U)

/* Erase/Program verification using VER bit */
/* Refer AI00195118 - Check for PVER should be enabled */
/* If PVER check has to be disabled, then strict margin check for '0'
   should be implemented in Fls_17_Pmu_CompareWordsSync API; this is needed
   to detect early interrupted programming operation. */
#define FLS_VER_ERROR_CHECK (STD_ON)

#define FLS_MARD_SELDO_CLEARMASK                (0xFFFFFFFDU)
#define FLS_SRC_SRE_CLEARMASK                   (0x7F3F18FFU)
#define FLS_SRC_SRE_SET_CLEARMASK               (0x7FFFFFFFU)
#define FLS_SRC_SRE_SETMASK                     (0x00000400U)
#define FLS_FCON_EOBM_CLEARMASK                 (0x7F0FFFFFU)
#define FLS_FCON_RES_MASK                       (0xFF0FFFFFU)
#define FLS_FCON_EOBM_SETMASK                   (0x80000000U)
#define FLS_FSR_SPND_MASK                       (0x08000000U)
#define FLS_MARD_SPNDERR_CLEARMASK              (0xFFFFFFEFU)
#define FLS_MARD_SPNDERR_SETMASK                (0x00000010U)
#define FLS_MARD_SPNDERR_MASK                   (0x00000010U)
#define FLS_FSR_DFPAGE_MASK                     (0x00000400U)
#define FLS_FSR_PROG_MASK                       (0x00000080U)
#define FLS_FSR_ERASE_MASK                      (0x00000100U)
#define FLS_WRITE_ONE_CLEARMASK                 (0xFFFFFFFFU)
#define FLS_WRITE_ZERO_SETMASK                  (0x00000000U)
#define FLS_FSR_SPND_BITPOS                     (27U)
#define FLS_MARD_SPNDERR_BITPOS                 (4U)
#define FLS_FSR_DFPAGE_BITPOS                   (10U)
#define FLS_FSR_PROG_BITPOS                     (7U)
#define FLS_FSR_ERASE_BITPOS                    (8U)



#if (FLS_USER_MODE_INIT_API_ENABLE == STD_ON)
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/    
#define FLS_SFR_INIT_RESETENDINIT()           (FLS_INIT_RESETENDINIT())

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_INIT_SETENDINIT()             (FLS_INIT_SETENDINIT())

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
 initialiser*/
#define FLS_SFR_INIT_WRITE32(reg,value)       FLS_INIT_WRITE32((reg), (value))

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/
#define FLS_SFR_INIT_MODIFY32(reg,clearmask,setmask)     \
        FLS_INIT_MODIFY32((reg),(clearmask),(setmask))


#if (FLS_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_INIT_USER_MODE_READ32(reg)                 \
        FLS_INIT_USER_MODE_OS_API_READ32(reg)
    
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/    
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/  
#define FLS_SFR_INIT_USER_MODE_MODIFY32(reg,clearmask,setmask) \
        FLS_INIT_USER_MODE_OS_API_MODIFY32((reg),(clearmask),(setmask))

#else

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_INIT_USER_MODE_READ32(reg)                   (reg)
  
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/
#define FLS_SFR_INIT_USER_MODE_MODIFY32(reg,clearmask,setmask) \
        { uint32 (val) ; (val) = (reg) ; \
          (val) &= (clearmask); (val) |= (setmask);\
          (reg) = (val);\
        }
#endif /* FLS_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON  */

#else
    
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_INIT_RESETENDINIT()               (Mcal_ResetENDINIT())

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/
#define  FLS_SFR_INIT_WRITE32(reg, value)         ((reg) = (value))

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/
#define FLS_SFR_INIT_MODIFY32(reg, clearmask,setmask) \
        {  uint32 (val) ; (val) = (reg) ; \
           (val) &= (clearmask); (val) |= (setmask);\
           (reg) = (val);\
        }

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_INIT_SETENDINIT()                    (Mcal_SetENDINIT())
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_INIT_USER_MODE_READ32(reg)             (reg)

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/
#define FLS_SFR_INIT_USER_MODE_MODIFY32(reg,clearmask,setmask)  \
        {  uint32 (val) ; (val) = (reg) ; \
          (val) &= (clearmask); (val) |= (setmask);\
          (reg) = (val);\
        }
#endif /* FLS_USER_MODE_INIT_API_ENABLE == STD_ON  */

#if (FLS_USER_MODE_RUNTIME_API_ENABLE == STD_ON)
  /*FLSUserMode is enabled*/

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#if (FLS_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_RUNTIME_USER_MODE_READ32(reg)        \
        FLS_RUNTIME_USER_MODE_OS_API_READ32(reg)

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/    
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/
#define FLS_SFR_RUNTIME_USER_MODE_MODIFY32(reg,clearmask,setmask)       \
        FLS_RUNTIME_USER_MODE_OS_API_MODIFY32((reg),(clearmask),(setmask))
#else
    
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define FLS_SFR_RUNTIME_USER_MODE_READ32(reg)                 (reg)
  
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/    
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/
#define FLS_SFR_RUNTIME_USER_MODE_MODIFY32(reg,clearmask,setmask) \
        { uint32 (val) ; (val) = (reg) ; \
          (val) &= (clearmask); (val) |= (setmask);\
          (reg) = (val);\
        }
#endif /* FLS_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON  */
#else

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/    
#define FLS_SFR_RUNTIME_USER_MODE_READ32(reg)             (reg)

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
  modification of OS protected calls, so it cannot be expand to a braced 
  initialiser*/  
#define FLS_SFR_RUNTIME_USER_MODE_MODIFY32(reg,clearmask,setmask)  \
        { uint32 (val) ; (val) = (reg) ;\
          (val) &= (clearmask); (val) |= (setmask);\
          (reg) = (val);\
        }
#endif /* FLS_USER_MODE_RUNTIME_API_ENABLE = STD_ON */

/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
typedef uint32 Fls_AddressType;

typedef uint32 Fls_LengthType;

typedef struct Fls_JobStartType
{
  unsigned_int Reserved1 :1;
  unsigned_int Write     :1;
  unsigned_int Erase     :1;
  unsigned_int Read      :1;
  unsigned_int Compare   :1;
  unsigned_int Reserved2 :3;
} Fls_JobStartType;

/* Return values of the function Fls_17_Pmu_GetNotifCaller */

typedef uint8 Fls_17_Pmu_Job_Type;

#define FLS_NO_JOB        ((Fls_17_Pmu_Job_Type)FLS_NONE)
#define FLS_WRITE_JOB     ((Fls_17_Pmu_Job_Type)FLS_WRITE)
#define FLS_ERASE_JOB     ((Fls_17_Pmu_Job_Type)FLS_ERASE) 
#define FLS_READ_JOB      ((Fls_17_Pmu_Job_Type)FLS_READ)
#define FLS_COMPARE_JOB   ((Fls_17_Pmu_Job_Type)FLS_COMPARE)
#define FLS_READSYNC_JOB  ((Fls_17_Pmu_Job_Type)FLS_READSYNC)
#define FLS_CANCEL_JOB    ((Fls_17_Pmu_Job_Type)FLS_CANCEL)


typedef struct Fls_17_Pmu_StateType
{
  /* Source Address for read job and
     destination Page Address for write job */
  uint32 FlsReadAddress;
  uint32 FlsWriteAddress;

  /* Erase and Write Timeout Cycle Count */
  #if (FLS_DEV_ERROR_DETECT == STD_ON)
  uint32 FlsEraseTimeoutCycleCount;
  uint32 FlsWriteTimeoutCycleCount;
  #endif

  /* Number of bytes to read or write */
  Fls_LengthType FlsReadLength;
  Fls_LengthType FlsWriteLength;

  /* Destination pointer for read job and
     source pointer for write job */
  uint8* FlsReadBufferPtr;
  const uint8* FlsWriteBufferPtr;

  /* Variable used to store Job Result of the Flash */
  MemIf_JobResultType FlsJobResult;
  
  /* FLS Mode - Fast or Slow */
  MemIf_ModeType FlsMode;
  
  /* Job type for which notification was raised */
  Fls_17_Pmu_Job_Type NotifCaller;

  /* Status to indicate if the job has been started */
  Fls_JobStartType JobStarted;

  /* Two instances of current jobtype are maintained for two data banks */
  uint8 FlsJobType[FLS_NUMBER_OF_SECTORS];

  /* FLS Driver initialized status */
  #if (FLS_PB_FIXEDADDR == STD_ON)
  uint8 DriverInitialised;
  #endif

  /* PVER error status */
  #if(FLS_VER_ERROR_CHECK == STD_ON)
  #if(FLS_IFX_FEE_USED == STD_ON)
  uint8 FlsPver;
  #endif
  #endif
  
  /* OPER error status */
  uint8 FlsOper;

  /* Command Sequence Timeout Error Status */
  uint8 FlsTimeoutErr;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
  /* Timeout handling status */
  uint8 FlsTimeoutControl;
#endif

} Fls_17_Pmu_StateType;


/*
Type:Fls_NotifFunctionPtrType
Funtion pointer for JobEnd and JobError Notification 
*/
typedef void (*Fls_NotifFunctionPtrType)(void);

typedef void (*Fls_WriteCmdPtrType)( uint32 StartAddress,
                                     uint32 PageAddress,
                                     const Fls_AddressType* ProgramDataPtr,
                                     uint8 WriteMode);

typedef void (*Fls_EraseCmdPtrType)(uint32 StartAddress);
/*
  This is the type of Flash external data structure containing 
  the overall initialization data for the Flash Driver 
*/
typedef struct Fls_17_Pmu_ConfigType 
{
  Fls_17_Pmu_StateType   *FlsStateVarPtr;
  /* FLS_CYCLE_TIME, Cycle time of calls of the flash drivers main function: 
     This parameter is not used in implementation as there is no deadline 
     monitoring */

  /*FLS_MAX_ERASE, Maximum number of bytes to erase one cycle of flash driver 
    job processing : This parameter is fixed to FLS_SECTOR_SIZE, as erase is 
    always sector wise*/

  /* FLS_MAX_WRITE, Maximum number of bytes Write in one cycle of flash driver
    job processing : This parameter is fixed to FLS_PAGE_SIZE as write is 
    always page wise.*/  

  /* FLS_MAX_READ, Maximum number of bytes to Read in one cycle of FLS driver
    job processing */   
  Fls_LengthType FlsFastRead;
  Fls_LengthType FlsSlowRead;
  /* Funtion pointer for JobEndNotification.*/
  Fls_NotifFunctionPtrType FlsJobEndNotificationPtr;

  /* Funtion pointer for JobErrorNotification. */
  Fls_NotifFunctionPtrType FlsJobErrorNotificationPtr;

  /* Funtion pointer for Illegal State Notification */
  Fls_NotifFunctionPtrType FlsIllegalStateNotificationPtr;

  /* Wait state configuration for Read access and error correction */
  uint32 FlsWaitStates;

  /* Funtion pointer for Write access code */
  Fls_WriteCmdPtrType FlsAccessCodeWritePtr;
 
  /* Funtion pointer for Erase access code */ 
  Fls_EraseCmdPtrType FlsAccessCodeErasePtr;

  /* FLS call cycle */
  #if (FLS_DEV_ERROR_DETECT == STD_ON)
  uint32 FlsCallCycle;
  #endif
  
  #if(FLS_E_COMPARE_FAILED_DEM_REPORT == ENABLE_DEM_REPORT)
  Dem_EventIdType FlsCompareFailedId;
  #endif
  
  #if(FLS_E_ERASE_FAILED_DEM_REPORT == ENABLE_DEM_REPORT)
  Dem_EventIdType FlsEraseFailedId;
  #endif
  
  #if(FLS_E_READ_FAILED_DEM_REPORT == ENABLE_DEM_REPORT)
  Dem_EventIdType FlsReadFailedId;
  #endif
  
  #if(FLS_E_WRITE_FAILED_DEM_REPORT == ENABLE_DEM_REPORT)
  Dem_EventIdType FlsWriteFailedId;
  #endif
  
  #if(FLS_E_SUSPND_RESUME_TIMEOUT_DEM_REPORT == ENABLE_DEM_REPORT)
  Dem_EventIdType FlsSuspendResumeTimeoutId;
  #endif

  /* Default mode of FLS Driver */
  MemIf_ModeType FlsDefaultMode;

} Fls_17_Pmu_ConfigType;

/*   Typedefs for FLS INIT API to be used by MEMIF module */
typedef void (*Fls_Init_Type)(const Fls_17_Pmu_ConfigType* ConfigPtr);


/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
#if (FLS_PB_FIXEDADDR == STD_OFF)
#define FLS_17_PMU_START_SEC_VAR_32BIT
#include "MemMap.h"
/*IFX_MISRA_RULE_08_08_STATUS=Fls_ConfigPtr is declared extern only here 
  and is defined in Fls_17_Pmu.c. This violation is reported incorrectly by 
  PC-lint tool */
extern const Fls_17_Pmu_ConfigType *Fls_ConfigPtr;
#define FLS_17_PMU_STOP_SEC_VAR_32BIT
#include "MemMap.h"
#else
#define FLS_17_PMU_START_SEC_CONST_32BIT
#include "MemMap.h"
extern const Fls_17_Pmu_ConfigType * const Fls_ConfigPtr;
#define FLS_17_PMU_STOP_SEC_CONST_32BIT
#include "MemMap.h"
#endif

#define FLS_17_PMU_START_SEC_POSTBUILDCFG
#include "MemMap.h"

/*Declaration of FLS postbuild configuration */
extern const Fls_17_Pmu_ConfigType 
                        Fls_17_Pmu_ConfigRoot[FLS_17_PMU_CONFIG_COUNT];

#define FLS_17_PMU_STOP_SEC_POSTBUILDCFG
#include "MemMap.h"

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

#define FLS_17_PMU_START_SEC_CODE
#include "MemMap.h"

/*******************************************************************************
  Traceability : [cover parentID=DS_AS_FLS259,DS_AS_FLS165,DS_AS403_FLS363
                                 DS_AS_FLS206,DS_AS_FLS247,DS_NAS_FLS_PR734_9,
                                 DS_NAS_FLS_PR128]
                 [/cover]
********************************************************************************
** Syntax : void Fls_17_Pmu_GetVersionInfo( 
                                        Std_VersionInfoType *VersioninfoPtr ) **
**                                                                            **
** Service ID:      16                                                        **
**                                                                            **
** Sync/Async:      Synchronous                                               **
**                                                                            **
** Reentrancy:      reentrant                                                 **
**                                                                            **
** Parameters (in): VersioninfoPtr : Pointer to where to store the version    **
**                                   information of this module.              **
**                                                                            **
** Parameters (out):none                                                      **
**                                                                            **
** Return value:    none                                                      **
**                                                                            **
** Description :    This service returns the version information of           **
**                  this module.                                              **
**                  The version information includes:                         **
**                  Module Id,Vendor Id , Vendor specific version numbers     **
**                                                                            **
*******************************************************************************/
#if  ((FLS_VERSION_INFO_API == STD_ON) && (FLS_DEV_ERROR_DETECT == STD_ON))
/*IFX_MISRA_RULE_19_04_STATUS=GetVersionInfo cannot be expanded to a braced 
  initialiser*/
/*IFX_MISRA_RULE_19_07_STATUS=GetVersionInfo is declared as a function like 
  macro as per AUTOSAR*/
#define  Fls_17_Pmu_GetVersionInfo(VersionInfoPtr)                           \
{                                                                            \
  if ((VersionInfoPtr) != NULL_PTR)                                          \
  {                                                                          \
  /* Note that versioninfo pointer is not checked for NULL as the user       \
     is supposed to send the memory allocated pointer */                     \
  /* FLS Module ID */                                                        \
  ((Std_VersionInfoType*)(VersionInfoPtr))->moduleID = FLS_17_PMU_MODULE_ID; \
  /* FLS vendor ID */                                                        \
  ((Std_VersionInfoType*)(VersionInfoPtr))->vendorID = FLS_17_PMU_VENDOR_ID; \
  /* major version of FLS */                                                 \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_major_version =               \
                                     (uint8)FLS_17_PMU_SW_MAJOR_VERSION;     \
  /* minor version of FLS */                                                 \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_minor_version =               \
                                     (uint8)FLS_17_PMU_SW_MINOR_VERSION;     \
  /* patch version of FLS */                                                 \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_patch_version =               \
                                     (uint8)FLS_17_PMU_SW_PATCH_VERSION;     \
  }                                                                          \
  else                                                                       \
  {                                                                          \
    Det_ReportError(                                                         \
                     (uint16)FLS_17_PMU_MODULE_ID,                           \
                     FLS_17_PMU_INSTANCE_ID,                                 \
                     FLS_SID_GETVERSIONINFO,                                 \
                     FLS_E_PARAM_POINTER                                     \
                   );/* Report to  DET */                                    \
  }                                                                          \
                                                                             \
}
#elif (FLS_VERSION_INFO_API == STD_ON) && (FLS_DEV_ERROR_DETECT == STD_OFF)
/*IFX_MISRA_RULE_19_04_STATUS=GetVersionInfo cannot be expanded to a braced 
  initialiser*/
/*IFX_MISRA_RULE_19_07_STATUS=GetVersionInfo is declared as a function like 
  macro as per AUTOSAR*/
#define  Fls_17_Pmu_GetVersionInfo(VersionInfoPtr)                           \
{                                                                            \
  if ((VersionInfoPtr) != NULL_PTR)                                          \
  {                                                                          \
  /* Note that versioninfo pointer is not checked for NULL as the user       \
     is supposed to send the memory allocated pointer */                     \
  /* FLS Module ID */                                                        \
  ((Std_VersionInfoType*)(VersionInfoPtr))->moduleID = FLS_17_PMU_MODULE_ID; \
  /* FLS vendor ID */                                                        \
  ((Std_VersionInfoType*)(VersionInfoPtr))->vendorID = FLS_17_PMU_VENDOR_ID; \
  /* major version of FLS */                                                 \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_major_version =               \
                                     (uint8)FLS_17_PMU_SW_MAJOR_VERSION;     \
  /* minor version of FLS */                                                 \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_minor_version =               \
                                     (uint8)FLS_17_PMU_SW_MINOR_VERSION;     \
  /* patch version of FLS */                                                 \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_patch_version =               \
                                     (uint8)FLS_17_PMU_SW_PATCH_VERSION;     \
  }                                                                          \
}
#else

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_GetVersionInfo(VersioninfoPtr) \
(ERROR_FlsVersionInfoApi_NOT_SELECTED)

#endif


/*******************************************************************************
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
extern void Fls_17_Pmu_Init(const Fls_17_Pmu_ConfigType* ConfigPtr);


/*******************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_Erase( Fls_AddressType TargetAddress,   **
**                                    Fls_LengthType Length                   **
**                                  )                                         **
**                                                                            **
** Service ID:  1                                                             **
**                                                                            **
** Sync/Async:  ASynchronous                                                  **
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
extern Std_ReturnType Fls_17_Pmu_Erase( Fls_AddressType TargetAddress,
                                 Fls_LengthType Length
                               );


/*******************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_Write( Fls_AddressType TargetAddress,   **
**                                    const uint8 *SourceAddressPtr,          **
**                                    Fls_LengthType Length                   **
**                                  );                                        **
**                                                                            **
** Service ID:  2                                                             **
**                                                                            **
** Sync/Async:  ASynchronous                                                  **
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
extern Std_ReturnType Fls_17_Pmu_Write( Fls_AddressType TargetAddress,
                                 const uint8 *SourceAddressPtr,
                                 Fls_LengthType Length
                               );


#if (FLS_COMPARE_API == STD_ON)
/*******************************************************************************
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
** Parameters (in) :SourceAddress, TargetAddressPtr and Length                **
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
extern Std_ReturnType Fls_17_Pmu_Compare( Fls_AddressType SourceAddress,
                            const uint8 *TargetAddressPtr,
                            Fls_LengthType Length
                          );
#else

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_Compare( SourceAddress, \
                     TargetAddressPtr, \
                     Length          \
                   )                 \
(ERROR_FlsCompareApi_NOT_SELECTED)

#endif

#if  (FLS_CANCEL_API  ==  STD_ON)
/*******************************************************************************
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
extern void Fls_17_Pmu_Cancel(void);

#else

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_Cancel(void) \
(ERROR_FlsCancelApi_NOT_SELECTED)

#endif

/*******************************************************************************
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
extern void Fls_17_Pmu_CancelNonEraseJobs(void);

#if  (FLS_GET_STATUS_API  ==  STD_ON)
/*******************************************************************************
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
**         This service shall return the driver state synchronously           **
*******************************************************************************/
extern MemIf_StatusType Fls_17_Pmu_GetStatus(void);

/*******************************************************************************
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
extern MemIf_StatusType Fls_17_Pmu_GetBankStatus(uint32 BankNum);

#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_GetStatus() \
(ERROR_FlsGetStatusApi_NOT_SELECTED)

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_GetBankStatus() \
(ERROR_FlsGetStatusApi_NOT_SELECTED)

#endif

#if (FLS_GET_JOB_RESULT_API  ==  STD_ON)
/*******************************************************************************
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
extern MemIf_JobResultType Fls_17_Pmu_GetJobResult(void);

#else

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_GetJobResult() \
(ERROR_FlsGetJobResultApi_NOT_SELECTED)
#endif

/*******************************************************************************
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
**         This service shall perform the processing of the flash read,write  **
**         and erase jobs                                                     **
*******************************************************************************/
extern void Fls_17_Pmu_MainFunction(void);

/*******************************************************************************
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
extern void Fls_17_Pmu_SetMode(MemIf_ModeType Mode);
#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_SetMode(Mode) \
(ERROR_FlsSetModeApi_NOT_SELECTED)
#endif

/*******************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_Read( Fls_AddressType SourceAddress,    **
**                                   uint8 *TargetAddressPtr,                 **
**                                   Fls_LengthType Length                    **
**                                 );                                         **
** Service ID:  7                                                             **
**                                                                            **
** Sync/Async:  ASynchronous                                                  **
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
extern Std_ReturnType Fls_17_Pmu_Read( Fls_AddressType SourceAddress,
                                uint8 *TargetAddressPtr, 
                                Fls_LengthType Length
                               );


#if (FLS_IFX_FEE_USED == STD_ON)
/*******************************************************************************
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
extern Std_ReturnType Fls_17_Pmu_ReadWordsSync( Fls_AddressType SourceAddress,
                                 const uint32 *TargetAddressPtr,
                                 uint32 Length
                               );

/*******************************************************************************
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
** Return value    :  None                                                    **
**                                                                            **
** Description :                                                              **
** Ths function returns the FLS Job that raised the notification              **
*******************************************************************************/
extern Fls_17_Pmu_Job_Type Fls_17_Pmu_GetNotifCaller(void);

/*******************************************************************************
** Syntax:Std_ReturnType Fls_17_Pmu_CompareWordsSync(                         **
                                             Fls_AddressType SourceAddress,   **
**                                           uint32 *TargetAddressPtr,        **
**                                           uint32 Length);                  **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : SourceAddress, TargetAddressPtr, Length                  **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : E_OK: Compare successful                                 **
**                   E_NOT_OK: HW busy or compare failed or ECC error         **
**                                                                            **
** Description : Service for comparing data from flash bank synchronously     **
*******************************************************************************/
extern Std_ReturnType Fls_17_Pmu_CompareWordsSync( 
                                 Fls_AddressType SourceAddress,
                                 const uint32 *TargetAddressPtr,
                                 uint32 Length);

/*******************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_VerifyErase( uint32 BankNum,            **
                                   uint32 * const UnerasedWordlineAddressPtr, **
                                   uint8 * const UnerasedWordlineCountPtr );  **
**                                 );                                         **
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
extern Std_ReturnType Fls_17_Pmu_VerifyErase( uint32 BankNum, 
                                   uint32 * UnerasedWordlineAddressPtr,
                                   uint8 * const UnerasedWordlineCountPtr );
                                   
/*******************************************************************************
** Syntax : Std_ReturnType Fls_17_Pmu_VerifySectorErase( uint32 BankNum,      **
                                   uint32 * const UnerasedWordlineAddressPtr, **
                                   uint8 * const UnerasedWordlineCountPtr,    **
                                   uint8 Sector);                             **
**                                 );                                         **
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
** Description : Service for verifying erase of a logical sector              **
*******************************************************************************/
extern Std_ReturnType Fls_17_Pmu_VerifySectorErase( uint32 BankNum, 
                                   uint32 * UnerasedWordlineAddressPtr,
                                   uint8 * const UnerasedWordlineCountPtr,
                                   uint8 Sector);

#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_ReadWordsSync(SourceAddress,TargetAddressPtr,Length) \
(ERROR_IFX_FEE_NOT_CONFIGURED)

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_GetNotifCaller() \
(ERROR_IFX_FEE_NOT_CONFIGURED)

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_CompareWordsSync(SourceAddress,TargetAddressPtr,\
                                                        Length) \
(ERROR_IFX_FEE_NOT_CONFIGURED)

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_VerifyErase(BankNum,UnerasedWLAddrPtr,UnerasedWLCountPtr) \
(ERROR_IFX_FEE_NOT_CONFIGURED)

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_VerifySectorErase(BankNum,UnerasedWLAddrPtr, \
                                         UnerasedWLCountPtr,Sector) \
(ERROR_IFX_FEE_NOT_CONFIGURED)

#endif /* #if (FLS_IFX_FEE_USED == STD_ON) */

#if (FLS_USE_ERASESUSPEND == STD_ON)
/*******************************************************************************
** Syntax : Std_ReturnType  Fls_17_Pmu_SuspendErase (uint32 BankNum)          **
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
extern Std_ReturnType Fls_17_Pmu_SuspendErase(uint32 BankNum);

/*******************************************************************************
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
** Ths function suspends an ongoing erase of a sector.                        **
*******************************************************************************/
extern Std_ReturnType Fls_17_Pmu_ResumeErase (uint32 BankNum);

#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_SuspendErase(BankNum) \
(ERROR_FlsUseEraseSuspend_NOT_SELECTED)

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_ResumeErase(BankNum) \
(ERROR_FlsUseEraseSuspend_NOT_SELECTED)

#endif /* FLS_USE_ERASESUSPEND == STD_ON */

/*******************************************************************************
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

extern void Fls_17_Pmu_Isr(void);

#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Fls_17_Pmu_Isr() \
(ERROR_FlsUseInterrupts_NOT_SELECTED)

#endif

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
**                    E_NOT_OK - OPER error has occured                       **
**                                                                            **
** Description : This function provides the OPER error status                 **
*******************************************************************************/
extern Std_ReturnType Fls_17_Pmu_GetOperStatus(void);

/*******************************************************************************
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
**               reporting of timeout DET                                     **
*******************************************************************************/
extern void Fls_17_Pmu_ControlTimeoutDet(uint8 Param);

/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/

#define FLS_17_PMU_STOP_SEC_CODE
#include "MemMap.h"

#endif

