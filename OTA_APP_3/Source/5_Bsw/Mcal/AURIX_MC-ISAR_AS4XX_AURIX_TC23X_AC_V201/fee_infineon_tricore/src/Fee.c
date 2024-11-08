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
**  $FILENAME   : Fee.c $                                                    **
**                                                                           **
**  $CC VERSION : \main\49 $                                                 **
**                                                                           **
**  $DATE       : 2016-05-06 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION  : This file contains                                        **
**                 - functionality of Flash EEPROM Emulation driver          **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/******************************************************************************
   Traceability:
                  [cover parentID = SAS_NAS_ALL_PR630_PR631,SAS_NAS_ALL_PR470,
                   DS_AS_FEE005_FEE076,DS_AS_FEE104,DS_AS_FEE045,
                   DS_NAS_FEE_PR1055,DS_NAS_FEE_PR915,SAS_NAS_ALL_PR749,
                   DS_AS_FEE071_FEE001,DS_NAS_FEE_PR1061,DS_NAS_FEE_PR1056,
                   DS_NAS_FEE_PR1060,DS_AS_FEE105,DS_AS_FEE010_FEE048_FEE012_1,
                   DS_AS_FEE063_FEE106_FEE012_2,DS_AS_FEE013,SAS_NAS_ALL_PR70,
                   SAS_NAS_ALL_PR1652]
                  [/cover]
******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of Flash header file */
#include "Fee.h"

/* Inclusion of FEE Callback functionality */
#include "Fee_Cbk.h"

#if (FEE_DEM_ENABLED == STD_ON)
/* Inclusion of Dem.h  */
#include "Dem.h"
#endif

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/* Garbage Collection procedure States */

/* 1. User operation allowed states */
#define FEE_GC_REQUESTED                            (0x5U)
/*Read states of GC */
#define FEE_GC_UNCFG_START_COPY                     (0x10U)
#define FEE_GC_START_COPY                           (0x1U)

/* Erase states of GC */
#define FEE_GC_ERASE_PREV_SECTOR                    (0x3U)
#define FEE_GC_INIT_ERASE_PREV_SECTOR               (0x4U)

/* 2. User operation NOT allowed states */
/*Read states of GC */
#define FEE_GC_UNCFG_COPY_READ_NXTPAGE              (0x0U)
#define FEE_GC_COPY_READ_NXTPAGE                    (0x2U)

/* Erase-Suspend Status */
#define FEE_ERASE_NOT_SUSPENDED                     ((uint8)0)
#define FEE_ERASE_SUSPENDED                         ((uint8)1)
/* FeeEraseSuspendStatus > 1  : Resume-Erase failed */
/* FEE_RESUME_ERASE_LIMIT - 1 : No. of times Resume-Erase will be attempmted */
#define FEE_RESUME_ERASE_LIMIT                      ((uint8)4)

/* Write states of GC */

/* SECTOR0 -> Current/Prev Sector; SECTOR1 -> Next Sector */
/* V -> Valid; E -> Erased */
/* SP -> State Page; M -> Marker */

#define FEE_GC_UNCFG_COPY_WRITE          (0x6U)
#define FEE_GC_COPY_WRITE                (0x7U)
#define FEE_GC_MARK_SECTOR1_V_SP         (0x8U)
#define FEE_GC_MARK_SECTOR1_V_M          (0x9U)
#define FEE_GC_MARK_SECTOR0_E_SP         (0xAU)
#define FEE_GC_MARK_SECTOR0_E_M          (0xBU)
#define FEE_GC_INIT_MARK_SECTOR0_E_SP    (0xCU)
#define FEE_GC_INIT_MARK_SECTOR0_E_M     (0xDU)

/* 3. Non operational states of GC */
#define FEE_GC_IDLE                                 (0xEU)
#define FEE_GC_FAIL                                 (0xFU)

#define FEE_ZERO      (0U)
#define FEE_ONE       (1U)
#define FEE_TWO       (2U)
#define FEE_THREE     (3U)
#define FEE_FOUR      (4U)
#define FEE_SIX       (6U)
#define FEE_SEVEN     (7U)
#define FEE_EIGHT     (8U)
#define FEE_TWELVE    (12U)
#define FEE_SIXTEEN   (16U)

#define FEE_STATE_FIRSTFOUR_COPY_OFFSET (32U)
#define FEE_STATE_ALLONES_OFFSET        (64U)
#define FEE_STATE_NONZERO_OFFSET        (96U)
#define FEE_STATE_CRC_OFFSET            (116U)
#define FEE_STATE_HALF_WORD_SIZE        (14U)
#define FEE_STATE_NEW_FORMAT            (1U)
#define FEE_STATE_OLD_FORMAT            (0U)
#define FEE_STATE_INVALID_FORMAT      (2U)
#define FEE_CRC_START_VAL               (0U)

         
#define FEE_STATE_WLCOUNT_MASK        (0xFFFFFF00U)
#define FEE_16_BIT_ALL_ONES           (0xFFFFU)
#define FEE_32_BIT_ALL_ONES           (0xFFFFFFFFU)
#define FEE_PAGE_COUNT_MASK           (0x7FFFU)
#define FEE_VALID_MASK                (0x80U)
#define FEE_DFLASH_WORDLINE_SIZE_1    (FEE_DFLASH_WORDLINE_SIZE - 1U)

#define FEE_WRITE_LEN_MAX             (FEE_MAX_BYTES_PER_CYCLE)
#define FEE_COMPARE_LEN_MAX           (FEE_MAX_BYTES_PER_CYCLE)


/* Data bytes in a word-line = ((WordLine Size / 8) * 7) */
#define FEE_DATA_BYTES_IN_WORDLINE    (448U)

/* Word-Line Affected Type */
/* 1. WL not affected */
#define FEE_LWB_NOT_AFFECTED                      ((uint8)0x00U)
/* 2. Block sharing the last written WL boundary is (also) affected */
#define FEE_LWB_ACROSS_WL_AFFECTED                ((uint8)0x01U)
/* 3. Only the block(s) present in the last written WL is affected */
#define FEE_LWB_WITHIN_WL_AFFECTED                ((uint8)0x02U)
/* 4. Continue copying block pages in last written WL */
#define FEE_LWB_WITHIN_WL_AFFECTED_CONT           ((uint8)0x03U)
/* 5. Copy marker page in last written WL */
#define FEE_LWB_MARKER_COPY_PENDING               ((uint8)0x04U)

/* Fee_Init States - Used for detecting the Garbage Collection Interruption */
#define FEE_INITGC_IDLE                      (0U)
#define FEE_INITGC_HANDLE_INVALID            (1U)
#define FEE_INITGC_HANDLE_DIRTY              (2U)
#define FEE_INITGC_NORMAL                    (3U)
#define FEE_INITGC_PREPARE_DFLASH            (4U)
#define FEE_INITGC_COMPLETE                  (5U)
#define FEE_INITGC_FAIL                      (6U)

/* Fee Prepare DFlash States */
#define FEE_PREPDF_ERASE_S0               (0U)
#define FEE_PREPDF_ERASE_S1               (1U)
#define FEE_PREPDF_MARK_S0_ERASED_SP      (2U)
#define FEE_PREPDF_MARK_S0_ERASED_M       (3U)
#define FEE_PREPDF_MARK_S0_VALID_SP       (4U)
#define FEE_PREPDF_MARK_S0_VALID_M        (5U)
#define FEE_PREPDF_MARK_S1_ERASED_SP      (6U)
#define FEE_PREPDF_MARK_S1_ERASED_M       (7U)

/* Cache Update states */
#define FEE_CACHE_IDLE                     (0U)
#define FEE_CACHE_FIND_NEXT_BLK            (1U)
#define FEE_CACHE_UPDATE_COMPLETE          (3U)


/* Sector page values, used to store current status of the DFLASH */
#define FEE_DFLASH_SECTOR_SIZE             (FLS_SECTOR_SIZE)
#define FEE_SECTOR0                        (0U)
#define FEE_SECTOR1                        (1U)
#define FEE_DFLASH_BANK0_BASE              (0x0000U)
#define FEE_DFLASH_BANK1_BASE              (FEE_DFLASH_SECTOR_SIZE)
#define FEE_DFLASH_WORD_SIZE               (4U)

/* Sector page offsets */
#define FEE_PAGE1_DATA_OFFSET               (8U)

#define FEE_STATEPAGE_DATA_SIZE             (120U)
#define FEE_STATEPAGE_MARKER_SIZE           (8U)
#define FEE_STATEPAGE_SIZE                  (128U)
#define FEE_STATEPAGE_PROGRAM_SIZE          (120U)
#define FEE_8BIT_ALL_ONE                    (0xFFU)
#define FEE_LOG_BLOCK_NOT_FOUND             ((uint16)0xFFFFU)
#define FEE_VALID_BIT                       (0x80U)

/* Pending request types */
#define FEE_PEND_PREV_NO_REQ                (1U)
#define FEE_PEND_PREV_WRITE_REQ             (2U)
#define FEE_PEND_PREV_READ_REQ              (3U)
#define FEE_PEND_PREV_READ_NOTIF            (5U)
#define FEE_REQ_READ                        (6U)
#define FEE_REQ_NORM_WRITE                  (8U)
#define FEE_REQ_IMM_WRITE                   (9U)
#define FEE_PEND_PREV_COMPARE               (10U)
#define FEE_PEND_INTR_COMPARE               (12U)
#define FEE_PEND_INTR_READ                  (13U)
#define FEE_PEND_INTR_WRITE                 (14U)
#define FEE_WRITE_FAIL                      (15U)

/* States to monitor Garbage Collection completion, if it was interrupted */
/* earlier due to reset */
#define FEE_INITGC_NORMAL_S1                (0x1DU)
#define FEE_INITGC_NORMAL_S0                (0xD1U)
#define FEE_INITGC_DATA_COPY_S1_S0          (0x5DU)
#define FEE_INITGC_DATA_COPY_S0_S1          (0xD5U)
#define FEE_INITGC_ERASE_S1                 (0xDFU)
#define FEE_INITGC_ERASE_S0                 (0xFDU)

/* State page Repair stages */
#define FEE_REPAIR_IDLE              (0U)
#define FEE_PROGRAM_STATEPAGE        (1U)
#define FEE_PROGRAM_MARKER           (2U)

/* Job states */
#define FEE_JOB_IDLE    (0U)
#define FEE_JOB_REQSTD  (1U)
#define FEE_JOB_DONE    (2U)

/* Job results */
#define FEE_JOB_PASS    (0U)
#define FEE_JOB_FAIL    (1U)

/* DFlash read with strict margin for 0 */
#define FEE_FLS_READ_STRICT_ZERO    (0U)

/* Last Written Block Dirty Status */
#define FEE_LWB_NOT_DIRTY        (0U)
#define FEE_LWB_DIRTY            (1U)
#define FEE_LWB_CHECK_FOR_DIRTY  (2U)

/* FEE JOBS */
#define FEE_WRITE_JOB  (FLS_WRITE_JOB)
#define FEE_READ_JOB   (FLS_READ_JOB)
#define FEE_ERASE_JOB  (FLS_ERASE_JOB)
#define FEE_NO_JOB     (FLS_NO_JOB)

/* Fee Main Function States */
#define FEE_MAIN_UNINIT         (0U)
#define FEE_MAIN_INITGC         (1U)
#define FEE_MAIN_CACHEUPDATE    (2U)
#define FEE_MAIN_READ           (3U)
#define FEE_MAIN_WRITE          (4U)
#define FEE_MAIN_GC             (5U)
#define FEE_MAIN_NONE           (6U)

/* State page Status */
#define FEE_SP_CLEAN            (0U)
#define FEE_SP_DIRTY            (1U)
#define FEE_SP_INVALID          (2U)

/* Page Identifier */
#define FEE_STATE_FIRST_PAGE_ID          (0x59U)
#define FEE_STATE_NEXT_PAGE_ID           (0xC6U)
#define FEE_DATA_FIRST_PAGE_ID           (0xA3U)
#define FEE_DATA_NEXT_PAGE_ID            (0x9CU)
#define FEE_DATA_MARKER_PAGE_ID          (0x65U)

/* State Page field for verification */
#define FEE_SP_ZEROFIELD1_START          (8U)
#define FEE_SP_ZEROFIELD1_END            (120U)
#define FEE_SP_ZEROFIELD2_START          (16U)
#define FEE_SP_MARKERFIELD_START         (120U)

#define FEE_SP_MARKER_WORD_1             (0xAFAFF53AU)
#define FEE_SP_MARKER_WORD_2             (0xAFAFF5F5U)
#define FEE_DP_MARKER_WORD               (0xF5F5AF65U)
#define FEE_DP_MARKER_BYTE_2             (0xAFU)
#define FEE_DP_MARKER_BYTE_3             (0xF5U)
#define FEE_DP_MARKER_BYTE_4             (0xF5U)
#define FEE_SP_PAGE2_WORD_1              ((uint32)FEE_STATE_NEXT_PAGE_ID)

#define FEE_SP_ALL_ONES_C6               (0xFFFFFFC6U)
#define FEE_SP_NON_ZERO_C6               (0xAFAFF5C6U)
#define FEE_SP_NON_ZERO                  (0xAFAFF5F5U)

/* Sector status */
#define FEE_SP_INIT             (0U)
#define FEE_SP_DONE             (1U)

/* Sector states */
#define FEE_STATELESS           (0U)
#define FEE_VALID               (0xD1U)
#define FEE_ERASED              (0x1EU)

/* DFlash status at Init */
#define FEE_S0_S1_EMPTY         (0x0U)
#define FEE_S0_VALID            (0x1U)
#define FEE_S1_VALID            (0x2U)
#define FEE_S0_S1_VALID         (0x3U)
#define FEE_S0_S1_INVALID       (0x4U)

/* Error events during User/Internal operations */
#define FEE_WRITE               ((uint8)0x1)
#define FEE_READ                ((uint8)0x2)
#define FEE_GC_WRITE            ((uint8)0x3)
#define FEE_GC_READ             ((uint8)0x4)
#define FEE_GC_ERASE            ((uint8)0x5)
#define FEE_INVALIDATE          ((uint8)0x6)
#define FEE_GC_TRIG             ((uint8)0x7)

/* Unconfig block related constants */
#define FEE_UNCONFIG_BLK_LIMIT_CROSSED    ((uint8)0xFF)
#define FEE_UNCONFIG_BLK_DEM_REPORTED     ((uint8)0x01)

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

#define FEE_START_SEC_CODE
#include "MemMap.h"

static void Fee_lInitGC(void);

static void Fee_lLocalRead(void);

static void Fee_lLocalWrite(void);

static void Fee_lGcPrologue(void);

static void Fee_lGcStartCopy(void);

static void Fee_lUpdateCache(void);

static void Fee_lLastPgHandler(void);

static void Fee_lDecideMainJob(void);

static void Fee_lHandleIntrWrite(void);

static void Fee_lHandleWriteFail(void);

static void Fee_lHandleInitGcFail(void);

static void Fee_lClearMarkerBuffer(void);

static void Fee_lGarbageCollection(void);

static void Fee_lGcCopyReadNxtPage(void);

static void Fee_lPrepareMarkerBuffer(void);

static void Fee_lGCErrorHandler(uint8 Error);

static void Fee_lClearFeeReadWriteBuffer(void);

static void Fee_lClearFeeGcReadWriteBuffer(void);

static void Fee_lHandleUsrJobFailure(uint8 Error);

static void Fee_lHandleUserWrite(uint16 UserBlockIndex);

static void Fee_lHandleWritePrepDFLASH(const uint8 *SrcPtr);

static void Fee_lGcRead(Fls_AddressType Position, uint16 Offset);

static void Fee_lPrepareStatePageBuffer(uint8 Bank, uint8 SectorState);

static void Fee_lRegisterWriteRequest(uint8 *DataBufferPtr,
                                      const Fee_BlockType *BlockPtr);

static void Fee_lUpdateBlockInfo(Fee_CacheStatusType Status, uint16 Index);
                                 
static void Fee_lUpdateNonZeroWLInfo(Fee_SectorInfoType *SectorInfoPtr,
                                     uint32 SrcAddr);
                                              
static uint8 Fee_lGcStartCopyWrite(void);

static uint8 Fee_lGcErasePrevSector(uint8 GcState);

static uint8 Fee_lGcProgramStatePage(uint8 GcState);

static uint8 Fee_lGcWrite(Fls_AddressType Position, uint16 Offset);

static uint16  Fee_lGetUsrBlockIndex(uint16 BlockNumber);

static uint32 Fee_lGetBlkCycleCount(uint16 Index);

static boolean Fee_lGcCheckBlkCopy(void);

static boolean Fee_lIsReadAllowedbyGC(void);

static boolean Fee_lIsWriteInvldAllowedbyGC(void);

static boolean Fee_lIsUnerasableAddr(uint32 Addr);

static boolean Fee_lIsLastWrittenWLAffected(uint16 StartOffset);

static Fee_PageType Fee_lGetDFLASHPages(uint16 Size);

static Fee_PageType Fee_lGetFreeDFLASHPages(uint8 BankNumber, uint32 StartAddr);

static Fls_17_Pmu_Job_Type Fee_lGetUserJob(uint8 PendReqType);

static MemIf_StatusType Fee_lGetStatus(void);

IFX_LOCAL_INLINE void Fee_lIDSM(void);

IFX_LOCAL_INLINE void Fee_lINSM(void);

IFX_LOCAL_INLINE void Fee_lGcRequested(void);

IFX_LOCAL_INLINE void Fee_lHandleGcFail(void);

IFX_LOCAL_INLINE void Fee_lGcBlkCompare(void);

IFX_LOCAL_INLINE void Fee_lIPrepDFlashSM(void);

IFX_LOCAL_INLINE void Fee_lHandleIntrRead(void);

IFX_LOCAL_INLINE void Fee_lMainGCTransHdlr(void);

IFX_LOCAL_INLINE void Fee_lMainNoneTransHdlr(void);

IFX_LOCAL_INLINE void Fee_lMainReadTransHdlr(void);

IFX_LOCAL_INLINE void Fee_lMainWriteTransHdlr(void);

IFX_LOCAL_INLINE void Fee_lInitGCTransHandler(void);

IFX_LOCAL_INLINE void Fee_lPerformPrevBlkCheck(void);

IFX_LOCAL_INLINE void Fee_lHandleErasePrepDFLASH(void);

IFX_LOCAL_INLINE void Fee_lUpdateGcCurrBlockInfo(void);

IFX_LOCAL_INLINE void Fee_lInitGCDSMTransHandler(void);

IFX_LOCAL_INLINE void Fee_lMainCacheUpdtTransHdlr(void);

IFX_LOCAL_INLINE void Fee_lGcEraseCompare(uint8 GcState);

IFX_LOCAL_INLINE void Fee_lSetInitGc(uint8 DFlashStatus);

IFX_LOCAL_INLINE void Fee_lHandleConfigGcCopyError(void);

IFX_LOCAL_INLINE void Fee_lCheckPrevPage(uint32 EndAddr);

IFX_LOCAL_INLINE void Fee_lHandleUnConfigGcCopyError(void);

IFX_LOCAL_INLINE void Fee_lInitGCPrepDFlSTransHandler(void);

IFX_LOCAL_INLINE void Fee_lGcStatePageCompare(uint8 GcState);

IFX_LOCAL_INLINE void Fee_lHandleUserWriteReq(uint16 UserBlockIndex);

IFX_LOCAL_INLINE void Fee_lComputeNextFreePageAddr(uint32 BankNumber);

IFX_LOCAL_INLINE void Fee_lHandleUserWriteCompare(uint16 UserBlockIndex);

IFX_LOCAL_INLINE void Fee_lHandleIntrWriteCompare(uint16 UserBlockIndex);

IFX_LOCAL_INLINE void Fee_lUpdateCacheTable(uint32 BankNumber, uint32 EndAddr);

IFX_LOCAL_INLINE void Fee_lPrepareInvldReqBuff(uint32 BlockCycleCount,
                                              uint8 *ReadWriteBuffPtr,
                                              uint16 BlockIndex);

IFX_LOCAL_INLINE void Fee_lHandleBlkHeaderFound(uint8  PotentialMarkerPage,
                                               uint32 DataNextPageCount);
                                      
IFX_LOCAL_INLINE void Fee_lHandleBlkMarkerFound(uint32 LWBHeaderAddress,
                                               uint32 LWBMarkerAddress,
                                               uint16 PageCount);
                                      
IFX_LOCAL_INLINE void Fee_lPrepareWriteReqBuff(uint32 BlockCycleCount,
                                              uint8 *ReadWriteBuffPtr,
                                              uint16 BlockIndex,
                                              uint16 BlockSize);

IFX_LOCAL_INLINE void Fee_lComputeNextWLAddr(Fee_SectorInfoType *SectorInfoPtr);

IFX_LOCAL_INLINE uint8 Fee_lGetSectorInfo(uint32 BankNumber, uint32 EndAddr);

IFX_LOCAL_INLINE uint8 Fee_lEvaluateStatePage(void);

IFX_LOCAL_INLINE uint32 Fee_lEvaluateStatePageFormat(void);

IFX_LOCAL_INLINE uint8 Fee_lEvaluateStatePageMarker(void);

IFX_LOCAL_INLINE uint8 Fee_lEvaluateOldStatePage(void);

IFX_LOCAL_INLINE uint8 Fee_lEvaluateNewStatePage(void);

IFX_LOCAL_INLINE uint32 Fee_lEvaluateStatePageCrc(uint32 * DataPtr,
                 uint32 Length);

IFX_LOCAL_INLINE uint8 Fee_lCheckPrevWordline(Fee_SectorInfoType *SectorInfoPtr,
                                             uint32 EndAddr);
                                             
IFX_LOCAL_INLINE uint8 Fee_lBuildDFlashStatus(void);

IFX_LOCAL_INLINE uint16 Fee_lLocalReadStart(Std_ReturnType ReturnValue,
                                           uint16 UserBlockIndex);

IFX_LOCAL_INLINE uint32 Fee_lIsWLBeginning(uint32 address);

IFX_LOCAL_INLINE uint32 Fee_lGetNextWLAddr(uint32 address);

IFX_LOCAL_INLINE uint32 Fee_lGetWLBeginning(uint32 address);

IFX_LOCAL_INLINE uint32 Fee_lGetBlockHeaderAddress(uint32 LWBMarkerAddress, 
                                                  uint16 PageCount);
                                         
IFX_LOCAL_INLINE boolean Fee_lIsBlockAffected(uint16 BlockNumber, 
                                             uint32 BlockAddr);

IFX_LOCAL_INLINE Std_ReturnType Fee_lEvaluateBlkHeader(uint16 BlockNumber, 
                                                      uint16 BlockPageCount);

IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformWriteCheck(
                                               const Fee_BlockType * BlockPtr);

IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformReadCheck(uint16 UserBlockIndex);

IFX_LOCAL_INLINE Std_ReturnType Fee_lCheckBlkCycleCnt(
                                                const Fee_BlockType *BlockPtr,
                                                uint32 BlockCycleCount);
                                            
IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformCurrBlkCheck(
                                               const Fee_BlockType * BlockPtr);

IFX_LOCAL_INLINE boolean Fee_lGcCheckUnconfigBlkCopy(void);

IFX_LOCAL_INLINE boolean Fee_lIsSectorEndCrossed(uint32 FreePages,
                                                uint32 PageCount);

IFX_LOCAL_INLINE boolean Fee_lIsSectorEmpty(uint8 state, uint8 used);

IFX_LOCAL_INLINE boolean Fee_lIsSectorThresholdCrossed(uint32 FreePages,
                                                      uint32 PageCount);

IFX_LOCAL_INLINE boolean Fee_lGcDonotTrigger
                                      (const Fee_StateDataType *StateDataPtr);

IFX_LOCAL_INLINE boolean Fee_lIsUserReadReqstd(const Fee_StateDataType *s_ptr);

IFX_LOCAL_INLINE boolean Fee_lIsUserWriteInvldReqstd
                                             (const Fee_StateDataType *s_ptr);

static void Fee_lGcDiscardUnconfigBlk(void);

static void Fee_lUpdateUnconfigBlockInfo(uint16 BlockNumber);

#define FEE_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/
/* FEE state page Non Zero Data structure */
#define FEE_START_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
static const uint32 Fee_StateHalf[]={ 0xFFFFFFC6U, 0xFFFFFFFFU, 
                                      0xFFFFFFC6U, 0xFFFFFFFFU,
                                      0xFFFFFFC6U, 0xFFFFFFFFU,
                                      0xFFFFFFC6U, 0xFFFFFFFFU,
                                      0xFAFA5FC6U, 0xFAFA5F5FU,
                                      0xFAFA5FC6U, 0xFAFA5F5FU,
                                      0xFAFA5FC6U, 0xFFFFFFFFU};

#define FEE_STOP_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define FEE_START_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE085,
                  DS_AS_FEE120_FEE168_FEE169_FEE017]
**                                                                            **
** Syntax : void Fee_Init(void)                                               **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  0x00                                                          **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : Driver Module Initialization function                        **
**               This service shall initialize the Flash EEPROM Emulation     **
**               module using the values provided in the configuration set.   **
**                                                                            **
*******************************************************************************/
void Fee_Init(void)
{
  uint32 EndAddr;
  Fee_StateDataType  *StateDataPtr;
  Fee_SectorInfoType *SectorInfoPtr;
  Fee_GcBlkInfoType  *GcBlkInfoPtr;
  Fee_LastWrittenBlkInfoType  *LastBlkInfoPtr;
  Fee_CacheType      *BlockPtr;
  uint32 Index;
  uint8  DFlashStatus;
  uint8  ReturnVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  SectorInfoPtr = StateDataPtr->FeeSectorInfo;

  /* Initialise the state variables of various state machines */

  /* Initialize the cache related variables */
  Index = FEE_ZERO;
  do
  {
    BlockPtr = &(StateDataPtr->FeeBlockInfo[Index]);
    BlockPtr->Address = FEE_ZERO;
    BlockPtr->BlockNumber = FEE_ZERO;
    BlockPtr->Status.Valid = FEE_ZERO;
    BlockPtr->Status.Consistent = FEE_ZERO;
    BlockPtr->Status.Copied = FEE_ZERO;
    BlockPtr->Status.PrevCopyValid = FEE_ZERO;
    BlockPtr->Status.PrevCopyConsistent = FEE_ZERO;
    BlockPtr->Status.PrevCopyCopied = FEE_ZERO;
    #if (FEE_GET_PREV_DATA_API == STD_ON)
    BlockPtr->PrevCopyAddress = FEE_ZERO;
    #endif
    Index++;
  }while(Index < FEE_TOTAL_BLOCK_COUNT);
  StateDataPtr->FeeCacheState = FEE_CACHE_IDLE;

  StateDataPtr->FeeGCUnconfigBlkCopyIndex = FEE_ZERO;
  StateDataPtr->FeeUncfgBlksExceeded = FEE_ZERO;
  StateDataPtr->FeeUnConfigBlockCount = FEE_ZERO;

  GcBlkInfoPtr = &(StateDataPtr->FeeGcCurrBlkInfo);
  GcBlkInfoPtr->BlockNumber = FEE_ZERO;
  GcBlkInfoPtr->PageCount = FEE_ZERO;
  GcBlkInfoPtr->Addr = FEE_ZERO;
  GcBlkInfoPtr->Consistent = FEE_ZERO;

  for(Index = FEE_ZERO; Index < FEE_MAX_NUM_OF_BLKS_IN_WL; Index++)
  {
      StateDataPtr->FeeGcLWBGcSrcAddr[Index] = FEE_ZERO;
  }
  StateDataPtr->FeeGcBlkIndexInWL = FEE_ZERO;

  StateDataPtr->FeeGcState = FEE_GC_IDLE;
  StateDataPtr->FeeGCCopyIndex = FEE_ZERO;
  StateDataPtr->FeeGcSrcAddr = FEE_ZERO;
  StateDataPtr->FeeGcDestAddr = FEE_ZERO;
  StateDataPtr->FeeDisableGCStart = (boolean)FALSE;

  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  StateDataPtr->FeeEraseSuspendStatus = FEE_ERASE_NOT_SUSPENDED;
  StateDataPtr->FeeEraseResumeDemReported = FEE_ZERO;
  #endif

  /* Initialize the InitGC related variables */
  StateDataPtr->FeeInitGCState = FEE_INITGC_IDLE;
  StateDataPtr->FeePrepDFLASHState = FEE_PREPDF_ERASE_S0;
  StateDataPtr->FeeRepairStep = FEE_REPAIR_IDLE;
  StateDataPtr->FeeGcResumeState = FEE_ZERO;

  /* Initialize the Read and Write related variables */
  StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_NO_REQ;
  StateDataPtr->FeeLastWrittenBlockDirty = FEE_LWB_NOT_DIRTY;

  LastBlkInfoPtr = &(StateDataPtr->FeeLastWrittenBlkInfo);
  LastBlkInfoPtr->Addr = FEE_ZERO;
  LastBlkInfoPtr->BlockNumber = FEE_ZERO;
  LastBlkInfoPtr->PageCount = FEE_ZERO;
  LastBlkInfoPtr->Status.Valid = FEE_ZERO;
  LastBlkInfoPtr->Status.Consistent = FEE_ZERO;
  LastBlkInfoPtr->Status.Copied = FEE_ZERO;
  LastBlkInfoPtr->Status.PrevCopyValid = FEE_ZERO;
  LastBlkInfoPtr->Status.PrevCopyConsistent = FEE_ZERO;
  LastBlkInfoPtr->Status.PrevCopyCopied = FEE_ZERO;

  StateDataPtr->FeeNextFreePageAddr = FEE_ZERO;
  StateDataPtr->FeeBlockStartAddr = FEE_ZERO;

  StateDataPtr->FeeUserReadDestPtr = (uint8*)NULL_PTR;

  /* Initialize common variables */
  Fee_lClearFeeReadWriteBuffer();
  Fee_lClearFeeGcReadWriteBuffer();

  StateDataPtr->FeeBlkPageCount = (Fee_PageType)FEE_ZERO;
  StateDataPtr->FeeCurrReqBlockNum = FEE_ZERO;
  StateDataPtr->FeeUserWriteBytesCount = FEE_ZERO;
  StateDataPtr->FeeUserJobFailCount = FEE_ZERO;

  StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus = FEE_ZERO;
  StateDataPtr->FeeStatusFlags.FeeBlkModified = FEE_ZERO;
  StateDataPtr->FeeStatusFlags.FeeWriteInvldAPICalled = FEE_ZERO;
  StateDataPtr->FeeStatusFlags.FeeStartInitGC = FEE_ZERO;
  StateDataPtr->FeeStatusFlags.FeeCurrSector = FEE_ZERO;

  StateDataPtr->FeeMainJob = FEE_MAIN_UNINIT;
  StateDataPtr->FeeJobResult = MEMIF_JOB_OK;
  StateDataPtr->FeeIntrJob = FEE_NO_JOB;
  StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
  StateDataPtr->FeeIntrJobResult = FEE_JOB_PASS;
  StateDataPtr->FeeUserJobStatus = FEE_JOB_IDLE;
  StateDataPtr->FeeUserJobResult = FEE_JOB_PASS;
  StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;

  StateDataPtr->FeeWLAffectedType = FEE_LWB_NOT_AFFECTED;
  StateDataPtr->FeeWriteAffectedAddr = FEE_ZERO;

  /* Initialize Sector State information */
  for(Index = FEE_ZERO; Index < FEE_SECTORS; Index++)
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      FeeSectorInfo[FEE_SECTORS] to access its elements*/
    SectorInfoPtr = SectorInfoPtr + Index;
    SectorInfoPtr->State = FEE_ZERO;
    SectorInfoPtr->StateCount = FEE_ZERO;
    SectorInfoPtr->UnerasableWLCount = FEE_ZERO;
    SectorInfoPtr->UnerasableWLAddr = FEE_ZERO;
    SectorInfoPtr->Status.Dirty = FEE_ZERO;
    SectorInfoPtr->Status.Used = FEE_ZERO;
    SectorInfoPtr->StatePageAddr = FEE_ZERO;
    SectorInfoPtr->NextFreeWLAddr = FEE_ZERO;
    SectorInfoPtr->NonZeroWLCount = FEE_ZERO;
    SectorInfoPtr->NonZeroWLAddr[0] = FEE_ZERO;
    SectorInfoPtr->NonZeroWLAddr[1] = FEE_ZERO;
  }
  StateDataPtr->FeeStateCount = FEE_ZERO;
  StateDataPtr->FeeSectorCount = FEE_ZERO;
  StateDataPtr->FeeComparedLen = FEE_ZERO;
  StateDataPtr->FeeReadLen = FEE_ZERO;
  StateDataPtr->FeeUnErasableWLAddrTemp = FEE_ZERO;
  StateDataPtr->FeeUnErasableWLCountTemp = FEE_ZERO;  

  /* Read sector information */
  for(Index=FEE_ZERO; Index<FEE_SECTORS; Index++)
  {
    if(Index == FEE_ZERO)
    {
      EndAddr = FEE_DFLASH_BANK0_BASE;
    }
    else
    {
      EndAddr = FEE_DFLASH_BANK1_BASE;
    }
    StateDataPtr->FeeCurrSectSrcAddr = EndAddr +
               (FEE_DFLASH_SECTOR_SIZE - FEE_DFLASH_WORDLINE_SIZE);
    do
    {
       ReturnVal = Fee_lGetSectorInfo(Index, EndAddr);
    }while(ReturnVal == FEE_SP_INIT);
  }

  DFlashStatus = Fee_lBuildDFlashStatus();

  Fee_lSetInitGc(DFlashStatus);

  StateDataPtr->FeeStatusFlags.FeeInitAPICalled = FEE_ONE;

  if( FEE_CONFIG_PTR->FeeGCConfigSetting.FeeGcRestartPoint ==
          (unsigned_int)FEE_GC_RESTART_INIT )
  {
    /* Start Init GC:
      Check the current state of the sectors and bring them to normal state*/
    StateDataPtr->FeeStatusFlags.FeeStartInitGC = FEE_ONE;
  }

} /*End of Fee_Init*/

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE086,
                  DS_AS_FEE020_FEE121_FEE170_FEE171]
**                                                                            **
** Syntax : void Fee_SetMode( MemIf_ModeType Mode )                           **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  0x01                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  MemIf_ModeType - Desired mode for the underlying flash  **
**                                      driver.                               **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This API sets the mode of the underlying flash driver        **
**                                                                            **
*******************************************************************************/
void Fee_SetMode(MemIf_ModeType Mode)
{
#if (FEE_SET_MODE_SUPPORTED == STD_ON)

  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  Fee_StateDataType  *StateDataPtr;
  MemIf_StatusType   FeeStatus;
  uint8              Error;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  Error = FEE_ZERO;

  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled == (unsigned_int)FEE_ZERO)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_SETMODE,FEE_E_UNINIT);
    Error = FEE_ONE;
  }
  else
  {
    /* Get the FEE status */
    FeeStatus = Fee_lGetStatus();
    if(FeeStatus == MEMIF_BUSY)
    {
      /* If busy, report error */
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                     FEE_SID_SETMODE,FEE_E_BUSY);
      Error = FEE_ONE;
    }
    else if(FeeStatus == MEMIF_BUSY_INTERNAL)
    {
      /* If busy internal, report error */
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                     FEE_SID_SETMODE,FEE_E_BUSY_INTERNAL);
      Error = FEE_ONE;
    }
    else
    {
      /* dummy else */
    }
  }

  if(Error == FEE_ZERO)
  #endif /* #if (FEE_DEV_ERROR_DETECT == STD_ON) */
  {
    /* set mode of FLS */
    Fls_17_Pmu_SetMode(Mode);
  }
#else
  /* To avoid MISRA warning for unused variable */
  if(Mode != MEMIF_MODE_SLOW)
  {
  }
#endif /* #if (FEE_SET_MODE_SUPPORTED == STD_ON) */
}/* End of Fee_SetMode */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE087,
    DS_AS_FEE022_FEE122_FEE133_FEE134_FEE135_FEE136_FEE137_FEE162_FEE172_FEE173]
**                                                                            **
** Syntax : Std_ReturnType Fee_Read(uint16 BlockNumber, uint16 BlockOffset,   **
**          uint8 *DataBufferPtr, uint16 Length )                             **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x02                                                           **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber - Logical Block Number                      **
**                    BlockOffset - Read address offset inside the block      **
**                    Length - Number of bytes to read                        **
**                                                                            **
** Parameters (out):  DataBufferPtr - Pointer to data buffer                  **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : This function shall take the block start address and         **
** offset and calculate the corresponding memory read address. The address    **
** offset and length parameter can take any value within the given block      **
** this allows reading of an arbitrary number of bytes from an arbitrary start**
** address inside a logical block                                             **
**                                                                            **
*******************************************************************************/
Std_ReturnType Fee_Read(uint16 BlockNumber,
                        uint16 BlockOffset,
                        uint8 *DataBufferPtr,
                        uint16 Length)
{
  Fee_StateDataType    *StateDataPtr;
  Fee_PendReqBufType   *PendReqPtr;
  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  uint16               BlkIdx;
  #endif
  Std_ReturnType       ReturnValue;

  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  uint32 BlkSize;
  #endif

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReturnValue = E_OK;
  PendReqPtr = &(StateDataPtr->FeePendReqInfo);

  #if (FEE_DEV_ERROR_DETECT == STD_ON)

  BlkIdx = Fee_lGetUsrBlockIndex(BlockNumber);

  /* Check if the Fee_Init API is called */
  if (StateDataPtr->FeeStatusFlags.FeeInitAPICalled == (unsigned_int)FEE_ZERO)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_READ,FEE_E_UNINIT);/* Report to DET */

    ReturnValue = E_NOT_OK;
  }

  /* If BlockNumber exists in the user configuration and is within range */
  else if (BlkIdx == FEE_LOG_BLOCK_NOT_FOUND)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_READ,FEE_E_INVALID_BLOCK_NO);/* Report to DET */

    ReturnValue = E_NOT_OK;
  }
  else
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      Fee_BlockConfig[Number of configured blocks] to efficiently access its 
      elements*/
    BlkSize = FEE_CONFIG_PTR->FeeBlockConfigPtr[BlkIdx].Size;

    /* Check if the offset is valid i.e. within the block size */
    if ((uint32)BlockOffset >= BlkSize)
    {
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                      FEE_SID_READ,FEE_E_INVALID_BLOCK_OFS);/* Report to DET */

      ReturnValue = E_NOT_OK;
    }

    /* Check if the buffer pointer is non-NULL */
    else if (DataBufferPtr == (uint8*) NULL_PTR)
    {
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                      FEE_SID_READ,FEE_E_INVALID_DATA_PTR);/* Report to DET */

      ReturnValue = E_NOT_OK;
    }

    /* Check if the Length (from the given offset) is valid */
    else if ( ((uint32)BlockOffset + Length) > BlkSize )
    {
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                      FEE_SID_READ,FEE_E_INVALID_BLOCK_LEN);/* Report to DET */

      ReturnValue = E_NOT_OK;
    }

    else
    {
      /* dummy else */
    }
  }

  if (ReturnValue != E_NOT_OK)
  #endif /* End of (FEE_DEV_ERROR_DETECT == STD_ON) */

  {
    /* Execute if there is no pending request */
    if ( StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_NO_REQ )
    {
      /* Request for InitGC */
      StateDataPtr->FeeStatusFlags.FeeStartInitGC = FEE_ONE;

      StateDataPtr->FeePendReqStatus = FEE_REQ_READ;

      /* Store the read request parameters in the Pending buffer structure */
      PendReqPtr->BlockNumber = BlockNumber;
      PendReqPtr->BlockOffset = BlockOffset;
      PendReqPtr->DataBufferPtr = DataBufferPtr;
      PendReqPtr->Length = Length;
      #if (FEE_GET_PREV_DATA_API == STD_ON)
      PendReqPtr->GetPrevCopy = FEE_ZERO;
      #endif

      StateDataPtr->FeeCurrReqBlockNum = BlockNumber;
      StateDataPtr->FeeJobResult = MEMIF_JOB_PENDING;
    }
    else
    {
      /* FEE is busy with another request; raise BUSY DET */
      #if (FEE_DEV_ERROR_DETECT == STD_ON)
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                      FEE_SID_READ,FEE_E_BUSY);
      #endif
      ReturnValue = E_NOT_OK;
    }
  }

  return (ReturnValue);

}/* End of Fee_Read */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE088,
          DS_AS_FEE025_FEE123_FEE138_FEE139_FEE144_FEE163_FEE174_FEE175_FEE183]
**                                                                            **
** Syntax : Std_ReturnType Fee_Write(uint16 BlockNumber, uint8 *DataBufferPtr)**
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x03                                                           **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber - Logical Block Number                      **
**                    DataBufferPtr - Pointer to data buffer                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : This function shall take the logical block number and        **
** determine the corresponding memory write address. This routine will        **
** perform the block write operation on DFLASH (DF_EEPROM).                   **
**                                                                            **
*******************************************************************************/
Std_ReturnType Fee_Write(uint16 BlockNumber,
                         uint8 *DataBufferPtr)
{
  Fee_StateDataType     *StateDataPtr;
  const Fee_BlockType*   BlockPtr;
  uint16                 UserBlockIndex;
  Std_ReturnType         ReturnValue;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  UserBlockIndex = Fee_lGetUsrBlockIndex(BlockNumber);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    Fee_BlockConfig[Number of configured blocks] to efficiently access its 
    elements*/
  BlockPtr = FEE_CONFIG_PTR->FeeBlockConfigPtr + UserBlockIndex;
  ReturnValue = E_OK;

  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled == (unsigned_int)FEE_ZERO)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_WRITE,FEE_E_UNINIT);/* Report to DET */

    ReturnValue = E_NOT_OK;
  }

  /* Check if the block number is correct */
  else if(UserBlockIndex == FEE_LOG_BLOCK_NOT_FOUND)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_WRITE,FEE_E_INVALID_BLOCK_NO);/* Report to DET */

    ReturnValue = E_NOT_OK;
  }

  /* Check if the buffer pointer is NULL */
  else if(DataBufferPtr == (uint8*)NULL_PTR)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_WRITE,FEE_E_INVALID_DATA_PTR);/* Report to DET */
    ReturnValue = E_NOT_OK;
  }

  else
  {
    /* dummy else */
  }

  if (ReturnValue != E_NOT_OK)
  #endif /* End of FEE_DEV_ERROR_DETECT == STD_ON */

  {
    /* Execute only if there is no pending request */
    if( StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_NO_REQ )
    {
      StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus = FEE_ZERO;

      Fee_lRegisterWriteRequest(DataBufferPtr,BlockPtr);
    }
    else
    {
      /* FEE is busy with other request; raise BUSY DET */
      #if (FEE_DEV_ERROR_DETECT == STD_ON)
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                      FEE_SID_WRITE,FEE_E_BUSY);
      #endif
      ReturnValue = E_NOT_OK;
    }
  }

  return (ReturnValue);

}/* End of Fee_Write */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS4XX_FEE089,DS_AS4XX_FEE184,
                  DS_AS_FEE080_FEE081_FEE124_FEE157_FEE164,DS_AS3XX_FEE089]
**                                                                            **
** Syntax : void Fee_Cancel(void)                                             **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x04                                                           **
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
** Description : This functions shall cancel a pending request.               **
** Note: An ongoing write request is not cancelled. However, a queued         **
** write request is cancelled. Any read request either queued or under        **
** execution is cancelled.                                                    **
**                                                                            **
*******************************************************************************/
void Fee_Cancel(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint32 PerformCancel;
  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  MemIf_StatusType   FeeStatus;
  uint8  Error;
  #endif

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  PerformCancel = FEE_ONE;

  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  Error = FEE_ZERO;
  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled == (unsigned_int)FEE_ZERO)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_CANCEL,FEE_E_UNINIT);/* Report to DET */
    Error = FEE_ONE;
  }
  else
  {
    FeeStatus = Fee_lGetStatus();
    if(FeeStatus != MEMIF_BUSY)
    {
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                      FEE_SID_CANCEL,FEE_E_INVALID_CANCEL);
      Error = FEE_ONE;
    }
  }
  if(Error == FEE_ZERO)
  #endif
  {
    switch(StateDataPtr->FeePendReqStatus)
    {
      /* All these cases mean that the job is requested but not started */
      case FEE_REQ_READ:
      case FEE_REQ_NORM_WRITE:
      case FEE_REQ_IMM_WRITE:
      {
        break;
      }

      /* The following cases mean that the read job is on-going */
      case FEE_PEND_PREV_READ_REQ:
      case FEE_PEND_PREV_READ_NOTIF:
      {
        /* Cancels the read job */
        Fls_17_Pmu_CancelNonEraseJobs();
        break;
      }

      /* All other pend req states
         Ex: FEE_PEND_PREV_NO_REQ:
      */
      default:
      {
        PerformCancel = FEE_ZERO;
        break;
      }
    }

    if(PerformCancel == FEE_ONE)
    {
      /* Update the current pending request type to 'No pending request' */
      StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_NO_REQ;
      /* Reset the Invalid Status flag */
      StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus = FEE_ZERO;
      /* Set the job result to 'Cancelled' */
      StateDataPtr->FeeJobResult = MEMIF_JOB_CANCELED;
      StateDataPtr->FeeUserJobStatus = FEE_JOB_IDLE;
      StateDataPtr->FeeUserJobFailCount = FEE_ZERO;
      /* Clear the DFLASH access for FEE */
    }
  }
}


/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE090,DS_AS3XX_FEE034_1,
                  DS_AS_FEE034_FEE074_FEE128_FEE129]
**                                                                            **
** Syntax : MemIf_StatusType Fee_GetStatus(void)                              **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x05                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  MemIf_StatusType                                        **
**                                                                            **
** Description : This function returns the status of the FEE driver           **
**                                                                            **
*******************************************************************************/
MemIf_StatusType Fee_GetStatus(void)
{
  Fee_StateDataType  *StateDataPtr;
  MemIf_StatusType   ReturnStatus;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled == (unsigned_int)FEE_ZERO)
  {
    ReturnStatus = MEMIF_UNINIT;
    #if (FEE_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_GETSTATUS,
                    FEE_E_UNINIT
                   );/* Report to DET */
    #endif
  }
  else
  {
    ReturnStatus = Fee_lGetStatus();
  }

  return (ReturnStatus);

}/*End of Fee_GetStatus */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE091,DS_AS_FEE158,
                  DS_AS_FEE035_FEE125_FEE155_FEE156,DS_AS3XX_FEE035_1]
**                                                                            **
** Syntax : MemIf_JobResultType Fee_GetJobResult(void)                        **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x06                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  MemIf_JobResultType                                     **
**                                                                            **
** Description : This function shall return the result of the last job that   **
** that was executed.                                                         **
**                                                                            **
*******************************************************************************/
MemIf_JobResultType Fee_GetJobResult(void)
{
  Fee_StateDataType  *StateDataPtr;
  MemIf_JobResultType ReturnValue;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReturnValue = StateDataPtr->FeeJobResult;

  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled == (unsigned_int)FEE_ZERO)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_GETJOBRESULT,
                    FEE_E_UNINIT
                   );/* Report to DET */
    ReturnValue = MEMIF_JOB_FAILED;
  }
  #endif

  return (ReturnValue);

}/*End of Fee_GetJobResult */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE092,
                  DS_AS_FEE126_FEE140_FEE145_FEE165_FEE176_FEE177,DS_AS_FEE037]
**                                                                            **
** Syntax : Std_ReturnType Fee_InvalidateBlock(uint16 BlockNumber)            **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x07                                                           **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber : Logical Block Number                      **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : This function shall invalidate the given logical             **
** block                                                                      **
**                                                                            **
*******************************************************************************/
Std_ReturnType Fee_InvalidateBlock(uint16 BlockNumber)
{
  Fee_StateDataType   *StateDataPtr;
  const Fee_BlockType *BlockPtr;
  uint16               UserBlockIndex;
  Std_ReturnType       ReturnValue;

  ReturnValue = E_OK;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  UserBlockIndex = Fee_lGetUsrBlockIndex(BlockNumber);

  #if (FEE_DEV_ERROR_DETECT == STD_ON)
  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled == (unsigned_int)FEE_ZERO)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_INVALIDATEBLOCK,
                    FEE_E_UNINIT
                   );/* Report to DET */
    ReturnValue = E_NOT_OK;
  }
  else if( UserBlockIndex == FEE_LOG_BLOCK_NOT_FOUND)
  {
    /* Block is not cofigured */
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                    FEE_SID_INVALIDATEBLOCK,
                    FEE_E_INVALID_BLOCK_NO);/* Report to DET */
    ReturnValue = E_NOT_OK;
  }
  else
  {
    /* dummy else */
  }

  if (ReturnValue != E_NOT_OK)
  #endif /*End of FEE_DEV_ERROR_DETECT == STD_ON */

  {
    /* Execute only if there is no pending request */
    if( StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_NO_REQ )
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        Fee_BlockConfig[Number of configured blocks] to efficiently access its 
        elements*/
      BlockPtr = FEE_CONFIG_PTR->FeeBlockConfigPtr + UserBlockIndex;

      StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus = FEE_ONE;
      Fee_lRegisterWriteRequest(StateDataPtr->FeeReadWriteBuffer,BlockPtr);
    }
    else
    {
      #if (FEE_DEV_ERROR_DETECT == STD_ON)
      /* FEE is busy with another request; raise BUSY DET */
      Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,
                      FEE_SID_INVALIDATEBLOCK,
                      FEE_E_BUSY
                     );/* Report to DET */
      #endif
      ReturnValue = E_NOT_OK;
    }
  }

  return (ReturnValue);

}/*End of Fee_InvalidateBlock */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE094]
**                                                                            **
** Syntax : Std_ReturnType Fee_EraseImmediateBlock( uint16 BlockNumber )      **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x09                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber - Logical Block Number                      **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : Since double-sector algorithm is used with threshold limit   **
** for triggering GC, immediate block's write requests during GC can be       **
** accommodated within the pre-erased threshold area of the active FEE sector.**
** Hence, this API is implemented as a dummy function returning E_NOT_OK.     **
**                                                                            **
*******************************************************************************/
Std_ReturnType Fee_EraseImmediateBlock(uint16 BlockNumber)
{
  UNUSED_PARAMETER (BlockNumber)
  return (E_NOT_OK);
}

/* Memory Map of the FEE Code */
#define FEE_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


#define FEE_START_SEC_JOBENDNOTIF_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
** Traceability: [cover parentID=DS_AS_FEE098,DS_AS_FEE095,
                  DS_AS_FEE052_FEE055_FEE142,DS_AS3XX_FEE051]                 **
**                                                                            **
** Syntax : void Fee_JobEndNotification(void)                                 **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x10                                                           **
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
** Description : This routine shall be called by the underlying flash         **
**           driver to report the successful end of an asynchronous operation **
*******************************************************************************/
void Fee_JobEndNotification(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint16             DataByteCount;
  uint16             BlockOffset;
  uint16             Length;
  uint16             LoopIndex;
  Fls_17_Pmu_Job_Type Job;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled != (unsigned_int)FEE_ZERO)
  {
    Job = Fls_17_Pmu_GetNotifCaller();

    if(Job == StateDataPtr->FeeIntrJob)
    {
      /* If job is internal job */
      StateDataPtr->FeeIntrJobStatus = FEE_JOB_DONE;
      StateDataPtr->FeeIntrJobResult = FEE_JOB_PASS;
      StateDataPtr->FeeIntrJob = FEE_NO_JOB;
    }
    else if( Fee_lGetUserJob(StateDataPtr->FeePendReqStatus) == Job)
    {
      /* if job is user job */
      StateDataPtr->FeeUserJobStatus = FEE_JOB_IDLE;
      StateDataPtr->FeeUserJobResult = FEE_JOB_PASS;

      if(Job == FLS_READ_JOB)
      {
        /* if job is read job */
        if(StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_READ_NOTIF)
        {
          Length = StateDataPtr->FeePendReqInfo.Length;
          BlockOffset = StateDataPtr->FeePendReqInfo.BlockOffset;
          DataByteCount = FEE_ONE;
          /* Copy read data to user buffer */
          if(BlockOffset > FEE_ZERO)
          {
            while((BlockOffset < FEE_SEVEN) && (Length > FEE_ZERO))
            {
              /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
                access user data buffer efficiently. User ensures the 
                availability of sufficient buffer data space for the requested
                operation. 
                Pointer arithmetic is used to efficiently access the data 
                within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
              StateDataPtr->FeePendReqInfo.DataBufferPtr[0] =
                  StateDataPtr->FeeReadWriteBuffer[DataByteCount + BlockOffset];
              BlockOffset++;
              /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
                access user data buffer efficiently. User ensures the 
                availability of sufficient buffer data space for the requested
                operation*/
              StateDataPtr->FeePendReqInfo.DataBufferPtr++;
              Length--;
            }
            DataByteCount += FEE_EIGHT;
          }
          while(DataByteCount < StateDataPtr->FeeLastReadSize)
          {
            for(LoopIndex = FEE_ZERO; 
                (LoopIndex < FEE_SEVEN) && (Length > FEE_ZERO); 
                LoopIndex++)
            {
              /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
                access user data buffer efficiently. User ensures the 
                availability of sufficient buffer data space for the requested
                operation. 
                Pointer arithmetic is used to efficiently access the data 
                within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
              StateDataPtr->FeePendReqInfo.DataBufferPtr[LoopIndex] =
                  StateDataPtr->FeeReadWriteBuffer[DataByteCount + LoopIndex];
              Length--;
            }
            /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
              access user data buffer efficiently. User ensures the 
              availability of sufficient buffer data space for the requested
              operation*/
            StateDataPtr->FeePendReqInfo.DataBufferPtr += FEE_SEVEN;
            DataByteCount += FEE_EIGHT;
          }
          /*************   RESET JOB VARIABLES ******************/
          StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_NO_REQ;
          StateDataPtr->FeeJobResult = MEMIF_JOB_OK;
          #if (FEE_GET_PREV_DATA_API == STD_ON)
          StateDataPtr->FeePendReqInfo.GetPrevCopy = FEE_ZERO;
          #endif
          /*****************************************************/

          /* Call user configured job End notification */
          if ( FEE_CONFIG_PTR->FeeNvmJobEndNotification != NULL_PTR )
          {
            (FEE_CONFIG_PTR->FeeNvmJobEndNotification)();
          }
        }
      }
      else 
      {
        /* FLS_WRITE_JOB: Handling last page write is done as part of
           Write State Machine itself */
      }
    }/* End of User Job Handling */
    else
    {
      /* do nothing */
    }
  }
}/*End of Fee_JobEndNotification */
#define FEE_STOP_SEC_JOBENDNOTIF_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#define FEE_START_SEC_JOBERRNOTIF_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
** Traceability: [cover parentID=DS_AS_FEE096,DS_AS_FEE054_FEE056_FEE143,
                                 DS_AS3XX_FEE053]                             **
**                                                                            **
** Syntax : void Fee_JobErrorNotification(void)                               **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x11                                                           **
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
** Description : This routine shall be called by the underlying flash         **
**                  driver to report the failure of an asynchronous operation **
*******************************************************************************/
void Fee_JobErrorNotification(void)
{
  Fee_StateDataType  *StateDataPtr;
  Fee_CacheType      *BlkPtr;
  #if (FEE_GET_PREV_DATA_API == STD_ON)
  Fee_PendReqBufType *PendReqPtr;
  #endif
  uint16              BlockIndex;
  Fls_17_Pmu_Job_Type Job;
  uint8               Error;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  #if (FEE_GET_PREV_DATA_API == STD_ON)
  PendReqPtr = &(StateDataPtr->FeePendReqInfo);
  #endif

  /* Check if the Fee_Init API is called */
  if(StateDataPtr->FeeStatusFlags.FeeInitAPICalled != (unsigned_int)FEE_ZERO)
  {
    Job = Fls_17_Pmu_GetNotifCaller();

    if(Job == FLS_CANCEL_JOB)
    {
      /* if job is cancel job */
      StateDataPtr->FeeUserJobStatus = FEE_JOB_IDLE;
      StateDataPtr->FeeUserJobResult = FEE_JOB_FAIL;
    }
    else if(Job == StateDataPtr->FeeIntrJob)
    {
      /* if job is internal job */
      StateDataPtr->FeeIntrJobStatus = FEE_JOB_DONE;
      StateDataPtr->FeeIntrJobResult = FEE_JOB_FAIL;
      StateDataPtr->FeeIntrJob = FEE_NO_JOB;
    }
    else if( Job == Fee_lGetUserJob(StateDataPtr->FeePendReqStatus))
    {
      /* Internal Write activity has failed */
      if( StateDataPtr->FeePendReqStatus == FEE_PEND_INTR_COMPARE)
      {
        Fee_lHandleWriteFail();
      }
      /* User Write / Read activity has failed */
      else
      {
        if(Job == FLS_WRITE_JOB)
        {
          /* if job is write job */
          if(StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus == 
                                          (unsigned_int)FEE_ONE)
          {
             StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus = FEE_ZERO;
             Error = FEE_INVALIDATE;
          }
          else
          {
             Error = FEE_WRITE;
          }

          /* Update Next Free Page Address */
          StateDataPtr->FeeNextFreePageAddr += StateDataPtr->FeeLastWriteSize;
        }
        else /*if(job == FLS_READ_JOB)*/
        {
          /* job is read job */
          /* Update block state as Inconsistent in Cache */
          BlockIndex =
              Fee_lGetUsrBlockIndex(StateDataPtr->FeeCurrReqBlockNum);
          BlkPtr = &(StateDataPtr->FeeBlockInfo[BlockIndex]);
          #if (FEE_GET_PREV_DATA_API == STD_ON)
          if(PendReqPtr->GetPrevCopy == FEE_ONE)
          {
            PendReqPtr->GetPrevCopy = FEE_ZERO;
            BlkPtr->Status.PrevCopyConsistent = FEE_ZERO;
          }
          else
          #endif
          {
            BlkPtr->Status.Consistent = FEE_ZERO;
            if(StateDataPtr->FeeCurrReqBlockNum ==
               StateDataPtr->FeeLastWrittenBlkInfo.BlockNumber)
            {
              StateDataPtr->FeeLastWrittenBlkInfo.Status.Consistent = FEE_ZERO;
            }
          }
          Error = FEE_READ;
        }
        Fee_lHandleUsrJobFailure(Error);
      }
    } /* End of User Job */
    else
    {
      /* do nothing, in case if FLS is used as standalone and this notification
         function being used */
    }
  }
}/*End of Fee_JobErrorNotification */
#define FEE_STOP_SEC_JOBERRNOTIF_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#define FEE_START_SEC_ILLEGALNOTIF_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
** Syntax : void Fee_17_IllegalStateNotification(void)                        **
**                                                                            **
** Service ID: NA                                                             **
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
** Description : This routine shall be called by the underlying flash         **
**                  driver when it reaches the illegal state                  **
*******************************************************************************/
void Fee_17_IllegalStateNotification(void)
{
  if(FEE_CONFIG_PTR->FeeIllegalStateNotification != NULL_PTR)
  {
    /* Call the Notification function */
    FEE_CONFIG_PTR->FeeIllegalStateNotification();
  }
}/*End of Fee_17_IllegalStateNotification */
#define FEE_STOP_SEC_ILLEGALNOTIF_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

/* Memory Map of the FEE Code */
#define FEE_START_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#if (FEE_GET_CYCLE_COUNT_API == STD_ON)
/*******************************************************************************
** Syntax : Std_ReturnType Fee_17_GetCycleCount(uint16 BlockNumber,           **
**                                              uint32 * const CountPtr)      **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in):  BlockNumber - Logical Block Number                       **
**                                                                            **
** Parameters (out) :  CountPtr - Pointer to the variable to which the Erase  **
**                               cycle count is to be updated                 **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : When called with BlockNumber = 0, this routine delivers the  **
** FEE sector erase cycle count. When called with a configured BlockNumber,   **
** the write cycle count of the given block is returned.                      **
**                                                                            **
*******************************************************************************/
Std_ReturnType Fee_17_GetCycleCount(uint16 BlockNumber,
                                    uint32 * const CountPtr)
{
  uint32               RawCnt;
  Fee_StateDataType   *StateDataPtr;
  uint16               BlockIndex;
  Std_ReturnType       ReturnValue;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReturnValue = E_NOT_OK;

  /* if the cache is not updated, or if GC is on going, return.
     else, proceed with Get block cycle count */
  if ( (StateDataPtr->FeeCacheState == FEE_CACHE_UPDATE_COMPLETE) &&
       (StateDataPtr->FeeGcState == FEE_GC_IDLE) )
  {

    if ( BlockNumber == FEE_ZERO )
    {
      /* Erase Cycle count is derived from the latest state count
         State Count is incremented at every state page write.
         At the start / After preparation of DFLASH, the state count is 3.
         Since then the state count is incremented twice during GC
         (once for VALID SP programming and again for ERASED SP programming).
         Hence the Erase Cycle Count shall be (StateCount - 3)/2 */
      RawCnt = ((StateDataPtr->FeeStateCount - FEE_THREE) >> FEE_ONE);
      *CountPtr = RawCnt;
      ReturnValue = E_OK;
    }
    else
    {
      /* Get index of the block info stored in cache */
      BlockIndex = Fee_lGetUsrBlockIndex(BlockNumber);

      /* Read block cycle count */
      RawCnt = Fee_lGetBlkCycleCount(BlockIndex);

      /* Check if the read was performed */
      if(RawCnt != FEE_32_BIT_ALL_ONES)
      {
        *CountPtr = RawCnt;
        ReturnValue = E_OK;
      }
    } /* end of if ( BlockNumber == FEE_ZERO ) */
  } /* end of if (StateDataPtr->FeeCacheState == FEE_CACHE_UPDATE_COMPLETE)..*/

  return (ReturnValue);

}/*End of Fee_GetCycleCount */
#endif
/* End of FEE_GET_CYCLE_COUNT_API == STD_ON */



/*******************************************************************************
** Syntax : void Fee_17_EnableGcStart( void )                                 **
**                                                                            **
** Service ID:                                                                **
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
** Description : This service allows enabling the trigger of GC, if GC was    **
** disabled earlier by calling the API Fee_17_DisableGcStart. This service    **
** allows the FEE to trigger the GC if the sector is filled up to the         **
** threshold level                                                            **
**                                                                            **
*******************************************************************************/
void Fee_17_EnableGcStart(void)
{
  Fee_StateDataType  *StateDataPtr;
  
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Enable the GC start functionality */
  StateDataPtr->FeeDisableGCStart = (boolean)FALSE;
}

/*******************************************************************************
** Syntax : void Fee_17_DisableGcStart( void )                                **
**                                                                            **
** Service ID:                                                                **
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
** Description : This API prevents the GC from being started in case the      **
** threshold is crossed in the active FEE sector. This API does not stop an   **
** ongoing GC but only prevents the GC from being triggered by the            **
** Write/Invalidate request issued by the user.                               **
** Note: This API will disable the GC from starting only if,                  **
** 1. GC is not ongoing                                                       **
** 2. Initialisation activities are complete (i.e. the state pages are read   **
** and the GC state is determined)                                            **
**                                                                            **
*******************************************************************************/
void Fee_17_DisableGcStart(void)
{
  Fee_StateDataType  *StateDataPtr;
  
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if InitGC is COMPLETE */
  if(StateDataPtr->FeeInitGCState == FEE_INITGC_COMPLETE)
  {
    /* Check if the GC is not started (= IDLE) */
    if(StateDataPtr->FeeGcState == FEE_GC_IDLE)
    {
      /* Disable the GC start functionality */
      StateDataPtr->FeeDisableGCStart = (boolean)TRUE;
    }
  }
}/*End of Fee_17_DisableGcStart*/

/*******************************************************************************
** Traceability: [cover parentID=DS_AS_FEE097,DS_AS_FEE026,
                  DS_AS_FEE057_FEE179_FEE180_FEE181]                          **
**                                                                            **
** Syntax : void Fee_MainFunction(void)                                       **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: 0x12                                                           **
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
** Description : This function asynchronously handles the requested read      **
** / write / invalidate jobs and their internal management operations.        **
** It determines the job to be executed and executes the same.                **
**                                                                            **
*******************************************************************************/
void Fee_MainFunction(void)
{
  Fee_StateDataType  *StateDataPtr;
  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  uint32              BankNumber;
  #endif
  
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  BankNumber = (uint32)StateDataPtr->FeeStatusFlags.FeeCurrSector ^ FEE_ONE;
  #endif
  
  /* 1. Determine & switch Main Job,if required, based on input conditions */
  Fee_lDecideMainJob();

  /* 2. Execute the Main Job */
  switch(StateDataPtr->FeeMainJob)
  {
    case FEE_MAIN_INITGC:
    {
      if(StateDataPtr->FeeIntrJobStatus != FEE_JOB_REQSTD)
      {
        Fee_lInitGC();
      }
      break;
    }

    case FEE_MAIN_CACHEUPDATE:
    {
      Fee_lUpdateCache();
      break;
    }

    case FEE_MAIN_WRITE:
    {
      if(StateDataPtr->FeeUserJobStatus != FEE_JOB_REQSTD)
      {
        Fee_lLocalWrite();
      }
      break;
    }

    case FEE_MAIN_READ:
    {
      if(StateDataPtr->FeeUserJobStatus != FEE_JOB_REQSTD)
      {
        Fee_lLocalRead();
      }
      break;
    }

    case FEE_MAIN_GC:
    {
      #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
      if(FEE_ERASE_NOT_SUSPENDED == StateDataPtr->FeeEraseSuspendStatus)
      {
      #endif
        if(StateDataPtr->FeeIntrJobStatus != FEE_JOB_REQSTD)
        {
          Fee_lGarbageCollection();
        }
      #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
      }
      else
      {
        if(StateDataPtr->FeeEraseSuspendStatus < FEE_RESUME_ERASE_LIMIT)
        {
          if(E_OK == Fls_17_Pmu_ResumeErase(BankNumber))
          {
            StateDataPtr->FeeEraseSuspendStatus = FEE_ERASE_NOT_SUSPENDED;
          }
          else
          {
            StateDataPtr->FeeEraseSuspendStatus++;
            if(FEE_ZERO == StateDataPtr->FeeEraseResumeDemReported)
            {
              if(FEE_RESUME_ERASE_LIMIT == StateDataPtr->FeeEraseSuspendStatus)
              {
                /* Report to DEM */
                #if (FEE_GC_ERASE_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
                Dem_ReportErrorStatus(FEE_E_GC_ERASE, DEM_EVENT_STATUS_FAILED);
                #endif
                StateDataPtr->FeeEraseResumeDemReported = FEE_ONE;
              }
            }
          }
        }
      }
      #endif
      break;
    }

    /*case FEE_MAIN_NONE, FEE_MAIN_UNINIT belong here */
    default:
    {
      break;
    }
  }
}/*  End of Fee_MainFunction */

#if (FEE_GET_PREV_DATA_API == STD_ON)
/*******************************************************************************
** Syntax : Std_ReturnType Fee_17_GetPrevData(uint16 BlockNumber,             **
**                                            uint16 BlockOffset,             **
**                                            uint8 *DataBufferPtr,           **
**                                            uint16 Length)                  **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async: Asynchronous                                                   **
**                                                                            **
** Reentrancy: Non reentrant                                                  **
**                                                                            **
** Parameters (in) :  BlockNumber - Logical Block Number                      **
**                    BlockOffset - Read address offset inside the block      **
**                    Length - Number of bytes to read                        **
**                                                                            **
** Parameters (out):  DataBufferPtr - Pointer to data buffer                  **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : This API shall read one occurance previous version of the    **
** given logical block                                                        **
**                                                                            **
*******************************************************************************/
Std_ReturnType Fee_17_GetPrevData(uint16 BlockNumber,
                                  uint16 BlockOffset,
                                  uint8 *DataBufferPtr,
                                  uint16 Length)
{

  Fee_StateDataType    *StateDataPtr;
  Fee_PendReqBufType   *PendReqPtr;
  Std_ReturnType        RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  PendReqPtr = &(StateDataPtr->FeePendReqInfo);

  RetVal = E_NOT_OK;

  /* Accept this request if there is NO PENDING request  and
  * CACHE UPDATE is complete */
  if ((StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_NO_REQ) &&
      (StateDataPtr->FeeCacheState == FEE_CACHE_UPDATE_COMPLETE) )
  {
    RetVal = E_OK;

    StateDataPtr->FeePendReqStatus = FEE_REQ_READ;

    /* Store the read request parameters in the Pending buffer structure */
    PendReqPtr->BlockNumber = BlockNumber;
    PendReqPtr->BlockOffset = BlockOffset;
    PendReqPtr->DataBufferPtr = DataBufferPtr;
    PendReqPtr->Length = Length;
    PendReqPtr->GetPrevCopy = FEE_ONE;

    StateDataPtr->FeeCurrReqBlockNum = BlockNumber;
    StateDataPtr->FeeJobResult = MEMIF_JOB_PENDING;
  } /* Check for pending request, cache update complete */

  return (RetVal);

}
#endif /* #if (FEE_GET_PREV_DATA_API == STD_ON) */

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE boolean Fee_lIsUserReadReqstd(                    **
**                                            const Fee_StateDataType *s_ptr) **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : s_ptr - Pointer to state data                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks if user read is requested                **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lIsUserReadReqstd(const Fee_StateDataType *s_ptr)
{
  boolean RetVal;
  
  if (s_ptr->FeePendReqStatus == FEE_REQ_READ)
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
** Syntax: IFX_LOCAL_INLINE boolean Fee_lIsUserWriteInvldReqstd               **
**                                       (const Fee_StateDataType *s_ptr)     **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : s_ptr - Pointer to state data                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks if user write/invalidate is requested    **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lIsUserWriteInvldReqstd
                                              (const Fee_StateDataType *s_ptr)
{
  boolean RetVal;
  
  if(((s_ptr)->FeePendReqStatus == FEE_REQ_NORM_WRITE) ||
     ((s_ptr)->FeePendReqStatus == FEE_REQ_IMM_WRITE))
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
** Syntax: IFX_LOCAL_INLINE boolean Fee_lIsSectorEmpty(uint8 state, uint8 used)*
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : state - Sector state                                     **
**                   used - Sector used status                                **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks if sector is empty                       **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lIsSectorEmpty(uint8 state, uint8 used)
{
  boolean RetVal;
  
  if((state == FEE_STATELESS) && (used == FEE_ZERO))
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
** Syntax: IFX_LOCAL_INLINE uint32 Fee_lIsWLBeginning(uint32 address)         **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : address - address to be checked                          **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : uint32                                                   **
**                                                                            **
** Description : This routine returns the masked value of address w.r.t. WL   **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fee_lIsWLBeginning(uint32 address)
{
  return (address & ((uint32)FEE_DFLASH_WORDLINE_SIZE_1));
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint32 Fee_lGetWLBeginning(uint32 address)        **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : address - address to be processed                        **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : uint32                                                   **
**                                                                            **
** Description : This routine returns the start address of the WL             **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fee_lGetWLBeginning(uint32 address)
{
  return (address & ((uint32)(~((uint32)FEE_DFLASH_WORDLINE_SIZE_1))));
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE uint32 Fee_lGetNextWLAddr(uint32 address)         **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : address - address to be processed                        **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : uint32                                                   **
**                                                                            **
** Description : This routine returns the start address of the next WL        **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fee_lGetNextWLAddr(uint32 address)
{
  return (Fee_lGetWLBeginning(address + FEE_DFLASH_WORDLINE_SIZE));
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE boolean Fee_lGcDonotTrigger(const Fee_StateDataType*
**                                                            *StateDataPtr)  **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : StateDataPtr - Pointer to state data                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks whether GC was never triggered.          **
**               This is done by checking whether Fee_Write was never called  **
**               in the FEE_GC_RESTART_WRITE configuration                    **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lGcDonotTrigger(const Fee_StateDataType 
                                                 *StateDataPtr)
{
  boolean RetVal;
  
  if ((FEE_CONFIG_PTR->FeeGCConfigSetting.FeeGcRestartPoint ==
                              (unsigned_int)FEE_GC_RESTART_WRITE)
        && (StateDataPtr->FeeStatusFlags.FeeWriteInvldAPICalled ==
                              (unsigned_int)FEE_ZERO))
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
** Syntax: IFX_LOCAL_INLINE boolean Fee_lIsSectorThresholdCrossed(            **
**                                                          uint32 FreePages, **
**                                                          uint32 PageCount) **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : FreePages - Number of free pages                         **
**                   PageCount - Page count                                   **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks whether sector threshold will be crossed **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lIsSectorThresholdCrossed(uint32 FreePages, 
                                                      uint32 PageCount)
{
  boolean RetVal;
  
  if ((sint32)(((sint32)FreePages - (sint32)PageCount)
                                  * (sint32)FEE_DFLASH_PAGE_SIZE)
              < (sint32)(FEE_CONFIG_PTR->FeeThresholdLimit))
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
** Syntax: IFX_LOCAL_INLINE boolean Fee_lIsSectorEndCrossed(uint32 FreePages, **
**                                                         uint32 PageCount)  **
**                                                                            **
** Service ID: None                                                           **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non Re-entrant                                                **
**                                                                            **
** Parameters (in) : FreePages - Number of free pages                         **
**                   PageCount - Page count                                   **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : boolean                                                  **
**                                                                            **
** Description : This routine checks whether sector end will be crossed       **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lIsSectorEndCrossed(uint32 FreePages, 
                                                uint32 PageCount)
{
  boolean RetVal;
  
  if (FreePages < PageCount)
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
** Traceability: [cover parentID=DS_AS_FEE099]                                **
**                                                                            **
** Syntax : static void Fee_lHandleUsrJobFailure(uint8 Error)                 **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : Error : Type of user job failure                         **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function handles the user job failure occuring due to   **
**               FLS operation failure                                        **
*******************************************************************************/
static void Fee_lHandleUsrJobFailure(uint8 Error)
{
  Fee_StateDataType     *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Set StateDataPtr->FeeJobResult as Failed */
  StateDataPtr->FeeJobResult = MEMIF_JOB_FAILED;

  StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_NO_REQ;

  StateDataPtr->FeeUserJobStatus = FEE_JOB_IDLE;
  StateDataPtr->FeeUserJobResult = FEE_JOB_FAIL;
  StateDataPtr->FeeUserJobFailCount = FEE_ZERO;
  StateDataPtr->FeeBlkPageCount = FEE_ZERO;

  /* Raise Job Failed DEM */
  #if (FEE_WRITE_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
  if (Error == FEE_WRITE)
  {
    Dem_ReportErrorStatus(FEE_E_WRITE, DEM_EVENT_STATUS_FAILED);
  }
  #endif

  #if (FEE_INVALIDATE_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
  if (Error == FEE_INVALIDATE)
  {
    Dem_ReportErrorStatus(FEE_E_INVALIDATE, DEM_EVENT_STATUS_FAILED);
  }
  #endif

  #if (FEE_READ_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
  if (Error == FEE_READ)
  {
    Dem_ReportErrorStatus(FEE_E_READ, DEM_EVENT_STATUS_FAILED);
  }
  #endif

  #if (FEE_WRITE_DEM_REPORT != FEE_ENABLE_DEM_REPORT)
  #if (FEE_INVALIDATE_DEM_REPORT != FEE_ENABLE_DEM_REPORT)
  #if (FEE_READ_DEM_REPORT != FEE_ENABLE_DEM_REPORT)
  /* To avoid compiler warning */
  UNUSED_PARAMETER (Error)
  #endif
  #endif
  #endif

  /* Report job Error notification */
  if (FEE_CONFIG_PTR->FeeNvmJobErrorNotification != NULL_PTR )
  {
    (FEE_CONFIG_PTR->FeeNvmJobErrorNotification)();
  }
}

/*******************************************************************************
** Syntax : static MemIf_StatusType Fee_lGetStatus(void)                      **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  MemIf_StatusType                                        **
**                                                                            **
** Description : This function returns the status of the FEE driver           **
*******************************************************************************/
static MemIf_StatusType Fee_lGetStatus(void)
{
  Fee_StateDataType  *StateDataPtr;
  MemIf_StatusType   ReturnStatus;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  ReturnStatus = MEMIF_IDLE;

  if(StateDataPtr->FeePendReqStatus == FEE_WRITE_FAIL)
  {
    /* ReturnStatus is MEMIF_IDLE */
  }
  /* If there is a request pending */
  else if(StateDataPtr->FeePendReqStatus != FEE_PEND_PREV_NO_REQ)
  {
    ReturnStatus = MEMIF_BUSY;
  }
  /* Init activities NOT triggered */
  else if(StateDataPtr->FeeStatusFlags.FeeStartInitGC == (unsigned_int)FEE_ZERO)
  {
    /* ReturnStatus is MEMIF_IDLE */
  }
  /* If the GC/InitGC has failed */
  else if((StateDataPtr->FeeGcState == FEE_GC_FAIL) ||
          (StateDataPtr->FeeInitGCState == FEE_INITGC_FAIL))
  {
    /* ReturnStatus is MEMIF_IDLE */
  }
  /* if Cache update activities ongoing */
  else if(StateDataPtr->FeeCacheState != FEE_CACHE_UPDATE_COMPLETE)
  {
    ReturnStatus = MEMIF_BUSY_INTERNAL;
  }
  else if(StateDataPtr->FeeGcState != FEE_GC_IDLE)
  {
    /* Scenario: First Fee write is not called but Fee_Read  */
    /* is already called                                     */
    if((boolean)FALSE != Fee_lGcDonotTrigger(StateDataPtr))
    {
      /* ReturnStatus is MEMIF_IDLE */
    }
    else
    {
      ReturnStatus = MEMIF_BUSY_INTERNAL;
    }
  }
  else
  {
    /* ReturnStatus is MEMIF_IDLE */
  }

  return (ReturnStatus);

}/*End of Fee_GetStatus */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE boolean Fee_lGcCheckUnconfigBlkCopy(void)        **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  TRUE : At least one un-configured block is to be copied **
**                    FALSE: All un-configured blocks are copied              **
**                                                                            **
** Description : This function checks whether any un-configured block needs   **
**               to be copied                                                 **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lGcCheckUnconfigBlkCopy(void)
{
  Fee_StateDataType     *StateDataPtr;
  boolean               RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* if any unconfigured block is yet to be copied then, return TRUE */
  if(StateDataPtr->FeeGCUnconfigBlkCopyIndex < 
                                          StateDataPtr->FeeUnConfigBlockCount)
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
** Syntax : static Fls_17_Pmu_Job_Type Fee_lGetUserJob(uint8 PendReqType)     **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :PendReqType : Type of pending request                 **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Fls_17_Pmu_Job_Type                                     **
**                                                                            **
** Description : This function returns the ongoing user job                   **
*******************************************************************************/
static Fls_17_Pmu_Job_Type Fee_lGetUserJob(uint8 PendReqType)
{
  Fls_17_Pmu_Job_Type RetVal;

  switch(PendReqType)
  {
    /* These states mean that the write job is going on */
    case FEE_PEND_PREV_COMPARE:
    case FEE_PEND_INTR_COMPARE:
    {
      RetVal = FLS_WRITE_JOB;
      break;
    }

    /* The following are the states for read job */
    case FEE_PEND_PREV_READ_REQ:
    case FEE_PEND_PREV_READ_NOTIF:
    {
      RetVal = FLS_READ_JOB;
      break;
    }

    default:
    {
      RetVal = FLS_NO_JOB;
      break;
    }
  }

  return (RetVal);

}/*End of Fee_lGetUserJob */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_NAS_FEE_PR1064]
**                                                                            **
** Syntax : static boolean Fee_lIsReadAllowedbyGC(void )                      **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Bool                                                    **
**                                                                            **
** Description : This function checks if the user read is allowed by GC       **
**                                                                            **
*******************************************************************************/
static boolean Fee_lIsReadAllowedbyGC(void)
{
  Fee_StateDataType *StateDataPtr;
  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  uint32             BankNumber;
  Std_ReturnType     FlsRetVal;
  #endif
  boolean            RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  RetVal = (boolean)FALSE;
  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  BankNumber = (uint32)StateDataPtr->FeeStatusFlags.FeeCurrSector ^ FEE_ONE;
  #endif
  
  /* GC is requested and not yet started */
  if(StateDataPtr->FeeGcState == FEE_GC_REQUESTED)
  {
    RetVal = (boolean)TRUE;
  }
  /* Copying of an unconfig blks is done.
     Copy of next unconfig blk has not yet started
  */
  else if((StateDataPtr->FeeGcState == FEE_GC_UNCFG_START_COPY) &&
          (StateDataPtr->FeeIntrJobStatus == FEE_JOB_IDLE))
  {
    RetVal = (boolean)TRUE;
  }
  /* Copying of a config blks is done.
     Copy of next config blk has not yet started
  */
  else if((StateDataPtr->FeeGcState == FEE_GC_START_COPY) &&
          (StateDataPtr->FeeIntrJobStatus == FEE_JOB_IDLE))
  {
    RetVal = (boolean)TRUE;
  }

  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  /* Erase is ongoing */
  else if((StateDataPtr->FeeGcState == FEE_GC_ERASE_PREV_SECTOR) ||
          (StateDataPtr->FeeGcState == FEE_GC_INIT_ERASE_PREV_SECTOR))
  {
    /* call fls erase suspend function if supported by hardware */
    if(FEE_CONFIG_PTR->FeeGCConfigSetting.FeeUseEraseSuspend ==
                              (unsigned_int)FEE_ERASE_SUSPEND_ENABLED)
    {
      if(FEE_ERASE_NOT_SUSPENDED == StateDataPtr->FeeEraseSuspendStatus)
      {
        FlsRetVal = Fls_17_Pmu_SuspendErase(BankNumber);
        if(E_OK == FlsRetVal)
        {
          RetVal = (boolean)TRUE;
          StateDataPtr->FeeEraseSuspendStatus = FEE_ERASE_SUSPENDED;
        }
      }
      else
      {
        RetVal = (boolean)TRUE;
        /* In the case of a suspend request, when previous attempt of resume
           had failed at least once, then the count shall be 
           reset back to 1, as it is a new request for suspend. */
        StateDataPtr->FeeEraseSuspendStatus = FEE_ERASE_SUSPENDED;
      }
    }
  }
  #endif

  else
  {
    /* dummy else */
  }

  return (RetVal);

}/* End of Fee_lIsReadAllowedbyGC */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_NAS_FEE_PR1063,DS_AS_FEE009]
**                                                                            **
** Syntax : static boolean Fee_lIsWriteInvldAllowedbyGC(void)                 **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Bool                                                    **
**                                                                            **
** Description : Checks if the Write/Invalidate request execution is allowed  **
**               by GC                                                        **
*******************************************************************************/
static boolean Fee_lIsWriteInvldAllowedbyGC(void)
{
  Fee_StateDataType *StateDataPtr;
  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  const Fee_BlockType  *PendReqBlkPtr;
  uint32             BankNumber;
  uint16             PendReqBlkNum;
  uint16             PendReqBlkIndex;
  Std_ReturnType     PendBlkWritable;
  Std_ReturnType     FlsRetVal;
  #endif
  boolean            RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  RetVal = (boolean)FALSE;
  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  BankNumber = (uint32)StateDataPtr->FeeStatusFlags.FeeCurrSector ^ FEE_ONE;
  #endif
  
  /* Immediate write has been requested */
  if((StateDataPtr->FeePendReqStatus == (uint8)FEE_REQ_IMM_WRITE) &&
     (StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus == 
                                   (unsigned_int)FEE_ZERO))
  {
    /* Copying of an unconfig blks is done.
       Copy of next unconfig blk has not yet started */
    if ((StateDataPtr->FeeGcState == FEE_GC_UNCFG_START_COPY) &&
             (StateDataPtr->FeeIntrJobStatus == FEE_JOB_IDLE))
    {
      RetVal = (boolean)TRUE;
    }
    /* Copying of a config blks is done.
       Copy of next config blk has not yet started */
    else if ((StateDataPtr->FeeGcState == FEE_GC_START_COPY) &&
             (StateDataPtr->FeeIntrJobStatus == FEE_JOB_IDLE))
    {
      RetVal = (boolean)TRUE;
    }
    else
    {
      /* dummy else */
    }
  }

  #if (FEE_FLS_SUPPORTS_ERASE_SUSPEND == STD_ON)
  /* Check if Erase is ongoing */
  if((StateDataPtr->FeeGcState == FEE_GC_ERASE_PREV_SECTOR) ||
     (StateDataPtr->FeeGcState == FEE_GC_INIT_ERASE_PREV_SECTOR))
  {
    /* call fls erase suspend function if supported by hardware */
    if(FEE_CONFIG_PTR->FeeGCConfigSetting.FeeUseEraseSuspend ==
                            (unsigned_int)FEE_ERASE_SUSPEND_ENABLED)
    {
      PendReqBlkNum = StateDataPtr->FeePendReqInfo.BlockNumber;
      PendReqBlkIndex = Fee_lGetUsrBlockIndex(PendReqBlkNum);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
        Fee_BlockConfig[Number of configured blocks] to efficiently access its 
        elements*/
      PendReqBlkPtr = FEE_CONFIG_PTR->FeeBlockConfigPtr + PendReqBlkIndex;
      
      PendBlkWritable = Fee_lPerformCurrBlkCheck(PendReqBlkPtr);
      
      if(PendBlkWritable == E_OK)
      {
        if(FEE_ERASE_NOT_SUSPENDED == StateDataPtr->FeeEraseSuspendStatus)
        {
          FlsRetVal = Fls_17_Pmu_SuspendErase(BankNumber);
          if(E_OK == FlsRetVal)
          {
            RetVal = (boolean)TRUE;
            StateDataPtr->FeeEraseSuspendStatus = FEE_ERASE_SUSPENDED;
          }
        }
        else
        {
          RetVal = (boolean)TRUE;
          /* In the case of a suspend request, when previous attempt of resume
             had failed at least once, then the count shall be 
             reset back to 1, as it is a new request for suspend. */
          StateDataPtr->FeeEraseSuspendStatus = FEE_ERASE_SUSPENDED;
        }
      }
    }
  }
  #endif

  return (RetVal);

}/* End of Fee_lIsWriteInvldAllowedbyGC */


/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lMainGCTransHdlr(void)                  **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) : None                                                 **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function determines and performs the state transition   **
** for the main function when in FEE_MAIN_GC state                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lMainGCTransHdlr(void)
{
  Fee_StateDataType *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  
  /* Check if GC is completed */
  if((FEE_MAIN_GC == StateDataPtr->FeeMainJob) &&
     (FEE_GC_IDLE == StateDataPtr->FeeGcState))
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_NONE;
  }  

  /* Check if the Read request is pending & can be executed */
  else if((boolean)TRUE == Fee_lIsUserReadReqstd(StateDataPtr))
  {
    if(Fee_lIsReadAllowedbyGC() == (boolean)TRUE)
    {
      StateDataPtr->FeeMainJob = FEE_MAIN_READ;
    }
  }
  /* Check if the Write request is pending & can be executed */
  else if(((boolean)TRUE == Fee_lIsUserWriteInvldReqstd(StateDataPtr)) &&
       (FEE_MAIN_GC == StateDataPtr->FeeMainJob))
  {
    if(Fee_lIsWriteInvldAllowedbyGC() == (boolean)TRUE)
    {
      StateDataPtr->FeeMainJob = FEE_MAIN_WRITE;
    }
  }
  else
  {
    /* dummy for MISRA */
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lMainNoneTransHdlr(void)                **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function determines and performs the state transition   **
** for the main function when in FEE_MAIN_NONE state                          **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lMainNoneTransHdlr(void)
{
  Fee_StateDataType *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if User Read request is registered */
  if( (boolean)TRUE == Fee_lIsUserReadReqstd(StateDataPtr) )
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_READ;
  }
  /* Check if User Write, Invalidate request is registered */
  else if( (boolean)TRUE == Fee_lIsUserWriteInvldReqstd(StateDataPtr) )
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_WRITE;
  }
  else
  {
    /* Fee_MainStateVar will be FEE_MAIN_NONE */
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lMainCacheUpdtTransHdlr(void)           **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function determines and performs the state transition   **
** for the main function when in FEE_MAIN_CACHEUPDATE state                   **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lMainCacheUpdtTransHdlr(void)
{
  Fee_StateDataType *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if User Read request is registered */
  if((boolean)TRUE == Fee_lIsUserReadReqstd(StateDataPtr))
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_READ;
  }
  /* Check if User Write, Invalidate request is registered
    and the write can be accomodated */
  else if((boolean)TRUE == Fee_lIsUserWriteInvldReqstd(StateDataPtr))
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_WRITE;
  }
  else if(StateDataPtr->FeeGcState != FEE_GC_IDLE)
  /* Check for !Fee_lGcDonotTrigger() is not required as it will be true
     else FEE would not be in Cache Update state */
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_GC;
  }
  else
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_NONE;
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lMainReadTransHdlr(void)                **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function determines and performs the state transition   **
** for the main function when in FEE_MAIN_READ state                          **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lMainReadTransHdlr(void)
{
  Fee_StateDataType *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if the Write/Read/Invalidate job is finished */
  if(StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_NO_REQ)
  {
    /* Check if GC job is ongoing/requested */
    if(((boolean)FALSE == Fee_lGcDonotTrigger(StateDataPtr)) && 
        (StateDataPtr->FeeGcState != FEE_GC_IDLE))
    {
      StateDataPtr->FeeMainJob = FEE_MAIN_GC;
    }
    else
    {
      StateDataPtr->FeeMainJob = FEE_MAIN_NONE;
    }
  }
  /* Check if user write/invalidate is requested */
  else if((boolean)TRUE == Fee_lIsUserWriteInvldReqstd(StateDataPtr))
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_WRITE;
  }
  else
  {
    /* dummy else */
  }
}

/*******************************************************************************
** Syntax: IFX_LOCAL_INLINE void Fee_lMainWriteTransHdlr(void)                **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function determines and performs the state transition   **
** for the main function when in FEE_MAIN_WRITE state                         **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lMainWriteTransHdlr(void)
{
  Fee_StateDataType *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if the Write/Read/Invalidate job is finished */
  if(StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_NO_REQ)
  {
    /* Check if GC job is ongoing/requested */
    if (StateDataPtr->FeeGcState != FEE_GC_IDLE)
    /* Check for !Fee_lGcDonotTrigger() is not required as it is True
       when the main job is WRITE */
    {
      StateDataPtr->FeeMainJob = FEE_MAIN_GC;
    }
    else
    {
      StateDataPtr->FeeMainJob = FEE_MAIN_NONE;
    }
  }
  else if( (StateDataPtr->FeePendReqStatus == FEE_REQ_NORM_WRITE ) ||
           (StateDataPtr->FeePendReqStatus == FEE_REQ_IMM_WRITE)
         )
  {
    if ((StateDataPtr->FeeGcState != FEE_GC_IDLE) &&
       (StateDataPtr->FeeReadLen == 0U))
    /* Check for !Fee_lGcDonotTrigger() is not required as it is always
       true when FeePendReqStatus = FEE_REQ_NORM_WRITE / FEE_REQ_IMM_WRITE */
    {
      StateDataPtr->FeeMainJob = FEE_MAIN_GC;
    }
  }
  /* Check if user read is requested */
  else if((boolean)TRUE == Fee_lIsUserReadReqstd(StateDataPtr))
  {
    StateDataPtr->FeeMainJob = FEE_MAIN_READ;
  }
  else
  {
    /* Remain in FEE_MAIN_WRITE */
    /* i.e.
    FEE_PEND_PREV_WRITE_REQ : Current block write is not yet completed
    */
  }
}

/*******************************************************************************
** Syntax : static void Fee_lDecideMainJob(void)                              **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function determines and performs the state transition   **
** for the main function                                                      **
*******************************************************************************/
static void Fee_lDecideMainJob(void)
{
  Fee_StateDataType     *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  switch(StateDataPtr->FeeMainJob)
  {
    case FEE_MAIN_INITGC:
    {
      /* Check if InitGC activity is ongoing */
      if(StateDataPtr->FeeInitGCState == FEE_INITGC_COMPLETE)
      {
        StateDataPtr->FeeMainJob = FEE_MAIN_CACHEUPDATE;
      }
      break;
    }

    case FEE_MAIN_NONE:
    {
      Fee_lMainNoneTransHdlr();
      break;
    }

    case FEE_MAIN_CACHEUPDATE:
    {
      Fee_lMainCacheUpdtTransHdlr();
      break;
    }

    case FEE_MAIN_READ:
    {
      Fee_lMainReadTransHdlr();
      break;
    }

    case FEE_MAIN_WRITE:
    {
      Fee_lMainWriteTransHdlr();
      break;

    }

    case FEE_MAIN_GC:
    {
      Fee_lMainGCTransHdlr();
      break;
    }

    /* remaining case - FEE_MAIN_UNINIT */
    default:
    {
      /* Check if InitGC activity is requested */
      if(StateDataPtr->FeeStatusFlags.FeeStartInitGC == (unsigned_int)FEE_ONE)
      {
        StateDataPtr->FeeMainJob = FEE_MAIN_INITGC;
      }
      break;
    }
  }
}/*End of Fee_lDecideMainJob */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleErasePrepDFLASH(void)            **
**                                                                            **
** Service ID:                                                                **
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
** Description : This function handles the erase operations for DFLASH        **
**               preparation                                                  **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleErasePrepDFLASH(void)
{
  Fee_StateDataType   *StateDataPtr;
  Fls_AddressType      DestAddr;
  Std_ReturnType       ReturnValueFls;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  DestAddr = StateDataPtr->FeeCurrSectSrcAddr;
  StateDataPtr->FeeIntrJobStatus = FEE_JOB_REQSTD;
  StateDataPtr->FeeIntrJob = FEE_ERASE_JOB;

  /* erase current sector */
  ReturnValueFls = Fls_17_Pmu_Erase(DestAddr,
                                    (Fls_LengthType)FEE_DFLASH_SECTOR_SIZE);
  if(ReturnValueFls == E_NOT_OK)
  {
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    StateDataPtr->FeeIntrJob = FEE_NO_JOB;
    Fee_lHandleInitGcFail();
  }
}

/*******************************************************************************
** Syntax : static void Fee_lHandleWritePrepDFLASH(const uint8 *SrcPtr)       **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  SrcPtr:  Pointer to the buffer with state page data     **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    : None                                                     **
**                                                                            **
** Description : This function handles the write operations for DFLASH        **
**               preparation                                                  **
*******************************************************************************/
static void Fee_lHandleWritePrepDFLASH(const uint8 *SrcPtr)
{
  Fee_StateDataType   *StateDataPtr;
  Fls_AddressType      DestAddr;
  Std_ReturnType       ReturnValueFls;
  uint8                PrepDFlashState;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  DestAddr = StateDataPtr->FeeCurrSectSrcAddr;
  if(Fee_lIsUnerasableAddr(DestAddr))
  {
    DestAddr += FEE_DFLASH_WORDLINE_SIZE;
  }
  StateDataPtr->FeeCurrSectSrcAddr = DestAddr;
  StateDataPtr->FeeIntrJobStatus = FEE_JOB_REQSTD;
  StateDataPtr->FeeIntrJob = FEE_WRITE_JOB;

  PrepDFlashState = StateDataPtr->FeePrepDFLASHState;

  switch (PrepDFlashState)
  {
    case FEE_PREPDF_MARK_S0_ERASED_SP:
    case FEE_PREPDF_MARK_S0_VALID_SP:
    case FEE_PREPDF_MARK_S1_ERASED_SP:
    {
      ReturnValueFls = Fls_17_Pmu_Write(DestAddr, SrcPtr,
                                  (Fls_LengthType)FEE_STATEPAGE_PROGRAM_SIZE);
    }
    break;

    /*
      Remaining cases:
        FEE_PREPDF_MARK_S0_ERASED_M
        FEE_PREPDF_MARK_S0_VALID_M
        FEE_PREPDF_MARK_S1_ERASED_M
    */
    default:
    {
      DestAddr += FEE_STATEPAGE_DATA_SIZE;
      ReturnValueFls = Fls_17_Pmu_Write(DestAddr, SrcPtr,
                                  (Fls_LengthType)FEE_STATEPAGE_MARKER_SIZE);
    }
    break;
  }

  if(ReturnValueFls == E_NOT_OK)
  {
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    StateDataPtr->FeeIntrJob = FEE_NO_JOB;
    Fee_lHandleInitGcFail();
  }
}/*End of Fee_lHandleWritePrepDFLASH */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lIDSM(void)                             **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function implements the Init Dirty State Machine which  **
** repairs the identified state pages before handing over to the INSM         **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lIDSM(void)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_SectorInfoType    *SectorInfoPtr;
  uint8                 *ReadWriteBuffPtr;
  Fls_AddressType        DestWriteAddress;
  uint32                 BankNumber;
  uint32                 WriteLength;
  Std_ReturnType         ReturnValueFls;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  BankNumber =  StateDataPtr->FeeStatusFlags.FeeCurrSector;
  SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
  if(SectorInfoPtr->Status.Dirty == (unsigned_int)FEE_ZERO)
  {
    BankNumber = BankNumber ^ FEE_ONE;
    SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
  }

  switch(StateDataPtr->FeeRepairStep)
  {
    case FEE_PROGRAM_MARKER:
    {
      StateDataPtr->FeeCurrSectSrcAddr = SectorInfoPtr->StatePageAddr;
      if (StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
      {
        Fee_lClearFeeGcReadWriteBuffer();
        ReadWriteBuffPtr = StateDataPtr->FeeGcReadWriteBuffer;
        (void) Fls_17_Pmu_ReadWordsSync(
                          (Fls_AddressType)StateDataPtr->FeeCurrSectSrcAddr,
                          (uint32 *)(void *)ReadWriteBuffPtr,
                          (Fls_LengthType)(FEE_STATEPAGE_DATA_SIZE/FEE_FOUR));
      }
      Fee_lPrepareMarkerBuffer();
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      ReadWriteBuffPtr = StateDataPtr->FeeGcReadWriteBuffer +  
                                         FEE_STATEPAGE_DATA_SIZE;
      WriteLength = FEE_DFLASH_PAGE_SIZE;
      DestWriteAddress = StateDataPtr->FeeCurrSectSrcAddr + 
                                         FEE_STATEPAGE_DATA_SIZE;
      break;
    }

    /* remaining case FEE_PROGRAM_STATEPAGE */
    default:
    {
      if(Fee_lIsUnerasableAddr(SectorInfoPtr->NextFreeWLAddr))
      {
        (SectorInfoPtr->NextFreeWLAddr) += FEE_DFLASH_WORDLINE_SIZE;
      }
      StateDataPtr->FeeCurrSectSrcAddr = SectorInfoPtr->NextFreeWLAddr;
      ReadWriteBuffPtr = StateDataPtr->FeeGcReadWriteBuffer;
      /* Update the new state page address and the next free WL addr */
      SectorInfoPtr->StatePageAddr = StateDataPtr->FeeCurrSectSrcAddr;
      SectorInfoPtr->NextFreeWLAddr =
                   Fee_lGetNextWLAddr(StateDataPtr->FeeCurrSectSrcAddr);
      WriteLength = FEE_STATEPAGE_PROGRAM_SIZE;
      DestWriteAddress = StateDataPtr->FeeCurrSectSrcAddr;
      break;
    }
  }/* END OF SWITCH */
  StateDataPtr->FeeIntrJob = FEE_WRITE_JOB;
  StateDataPtr->FeeIntrJobStatus = FEE_JOB_REQSTD;
  ReturnValueFls=Fls_17_Pmu_Write(DestWriteAddress,
                                  ReadWriteBuffPtr,
                                  (Fls_LengthType)WriteLength);
  if(ReturnValueFls == E_NOT_OK)
  {
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    StateDataPtr->FeeIntrJob = FEE_NO_JOB;
    Fee_lHandleInitGcFail();
  }
}


/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lINSM(void)                             **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function implements the Init Normal State Machine which **
** detects the resumption point of the previously interrupted GC              **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lINSM(void)
{
  Fee_StateDataType  *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  switch (StateDataPtr->FeeGcResumeState)
  {
    /* Copy of Sector1 to Sector0 was ongoing */
    case FEE_INITGC_DATA_COPY_S1_S0:
    {
      StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK1_BASE;
      StateDataPtr->FeeGcSrcAddr = FEE_DFLASH_BANK1_BASE;
      StateDataPtr->FeeGcDestAddr = FEE_DFLASH_BANK0_BASE;
      StateDataPtr->FeeGcState = FEE_GC_INIT_ERASE_PREV_SECTOR;

      break;
    }

    /* Copy of Sector0 to Sector1 was ongoing */
    case FEE_INITGC_DATA_COPY_S0_S1:
    {
      StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK0_BASE;
      StateDataPtr->FeeGcSrcAddr = FEE_DFLASH_BANK0_BASE;
      StateDataPtr->FeeGcDestAddr = FEE_DFLASH_BANK1_BASE;
      StateDataPtr->FeeGcState = FEE_GC_INIT_ERASE_PREV_SECTOR;

      break;
    }

    /* Erase of Sector0 was ongoing */
    case FEE_INITGC_ERASE_S0:
    {
      StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK1_BASE;
      StateDataPtr->FeeGcSrcAddr = FEE_DFLASH_BANK1_BASE;
      StateDataPtr->FeeGcDestAddr = FEE_DFLASH_BANK0_BASE;
      StateDataPtr->FeeGcState = FEE_GC_ERASE_PREV_SECTOR;

      break;
    }

    /* Erase of Sector1 was ongoing */
    case FEE_INITGC_ERASE_S1:
    {
      StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK0_BASE;
      StateDataPtr->FeeGcSrcAddr = FEE_DFLASH_BANK0_BASE;
      StateDataPtr->FeeGcDestAddr = FEE_DFLASH_BANK1_BASE;
      StateDataPtr->FeeGcState = FEE_GC_ERASE_PREV_SECTOR;

      break;
    }

    /* 
      Remaining normal cases:
        case FEE_INITGC_NORMAL_S1
        case FEE_INITGC_NORMAL_S0
    */
    default:
    {
      /* Set Garbage Collection state to Idle */
      StateDataPtr->FeeGcState = FEE_GC_IDLE;

      /* Update the FeeCurrSectSrcAddr */
      if(StateDataPtr->FeeStatusFlags.FeeCurrSector == (unsigned_int)FEE_ZERO)
      {
        StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK0_BASE;
      }
      else
      {
        StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK1_BASE;
      }

      break;
    }
  }/* End Switch */

  /* Mark end of initialization */
  StateDataPtr->FeeInitGCState = FEE_INITGC_COMPLETE;
}


/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lIPrepDFlashSM(void)                    **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function implements the DFlash preparation State        **
** Machine which prepares an empty DFlash                                     **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lIPrepDFlashSM(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint8              *BufferPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  switch(StateDataPtr->FeePrepDFLASHState)
  {
    case FEE_PREPDF_ERASE_S0:
    case FEE_PREPDF_ERASE_S1:
    {
      Fee_lHandleErasePrepDFLASH();
      break;
    }
    case FEE_PREPDF_MARK_S0_ERASED_SP:
    {
      /* Step1: Program first 120bytes of state page */
      if (StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
      {
        Fee_lPrepareStatePageBuffer(FEE_SECTOR0, FEE_ERASED);
      }
      else
      {
        /* The state page is being reattempted after one compare failure
           Clear the last 8bytes of GcReadWriteBuffer for comparison of SP */
        Fee_lClearMarkerBuffer();
      }
      /* State page data is prepared in the first 128 pages of RWBuffer */
      BufferPtr = StateDataPtr->FeeGcReadWriteBuffer;
      Fee_lHandleWritePrepDFLASH(BufferPtr);
      break;
    }
    case FEE_PREPDF_MARK_S0_VALID_SP:
    {
      /* Step1: Program first 120bytes of state page */
      if (StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
      {
        Fee_lPrepareStatePageBuffer(FEE_SECTOR0, FEE_VALID);
      }
      else
      {
        /* The state page is being reattempted after one compare failure
           Clear the last 8bytes of GcReadWriteBuffer for comparison of SP */
        Fee_lClearMarkerBuffer();
      }
      /* State page data is prepared in the first 128 pages of RWBuffer */
      BufferPtr = StateDataPtr->FeeGcReadWriteBuffer;
      Fee_lHandleWritePrepDFLASH(BufferPtr);
      break;
    }
    case FEE_PREPDF_MARK_S1_ERASED_SP:
    {
      /* Step1: Program first 120bytes of state page */
      if (StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
      {
        Fee_lPrepareStatePageBuffer(FEE_SECTOR1, FEE_ERASED);
      }
      else
      {
        /* The state page is being reattempted after one compare failure
           Clear the last 8bytes of GcReadWriteBuffer for comparison of SP */
        Fee_lClearMarkerBuffer();
      }
      /* State page data is prepared in the first 128 pages of RWBuffer */
      BufferPtr = StateDataPtr->FeeGcReadWriteBuffer;
      Fee_lHandleWritePrepDFLASH(BufferPtr);
      break;
    }
    /* 
      Remaining cases:
        FEE_PREPDF_MARK_S0_ERASED_M
        FEE_PREPDF_MARK_S0_VALID_M
        FEE_PREPDF_MARK_S1_ERASED_M
    */
    default:
    {
      /* Step1: Program marker of state page */
      Fee_lPrepareMarkerBuffer();
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      BufferPtr = StateDataPtr->FeeGcReadWriteBuffer +
                  FEE_STATEPAGE_DATA_SIZE;
      Fee_lHandleWritePrepDFLASH(BufferPtr);
      break;
    }
  }/* END OF SWITCH */
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lInitGCDSMTransHandler(void)            **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function shall handle the transitions required by the   **
** Init Dirty State Machine                                                   **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lInitGCDSMTransHandler(void)
{
  Fee_StateDataType  *StateDataPtr;
  Fee_SectorInfoType *SectorInfoPtr;
  uint8              *ReadWriteBuffPtr;
  uint32              BankNumber;
  uint32              EndAddr;
  Fls_AddressType     DestAddr;
  Std_ReturnType      ReturnVal1;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  DestAddr = (Fls_AddressType)StateDataPtr->FeeCurrSectSrcAddr;
  /* Get the active sector number */
  BankNumber =  StateDataPtr->FeeStatusFlags.FeeCurrSector;
  SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
  /* Is active sector state page dirty and being repaired?
     If yes - BankNumber = active sector,
     else - Bank Number = other sector */
  if(SectorInfoPtr->Status.Dirty == (unsigned_int)FEE_ZERO)
  {
    BankNumber = BankNumber ^ FEE_ONE;
    SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
  }

  switch(StateDataPtr->FeeRepairStep)
  {
    case FEE_PROGRAM_MARKER:
    {
      ReadWriteBuffPtr = StateDataPtr->FeeGcReadWriteBuffer;
      ReturnVal1 = Fls_17_Pmu_CompareWordsSync(
                              (Fls_AddressType)DestAddr,
                              (uint32 *)(void *)ReadWriteBuffPtr,
                              (Fls_LengthType)FEE_STATEPAGE_SIZE/FEE_FOUR);

      if ( (ReturnVal1 == E_NOT_OK))
      {
        /* Compare failed for the first time */
        if(StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
        {
          /* Check if there is space in the sector for state page */
          if(BankNumber == FEE_ZERO)
          {
            EndAddr = FEE_DFLASH_BANK0_BASE;
          }
          else
          {
            EndAddr = FEE_DFLASH_BANK1_BASE;
          }
          EndAddr = EndAddr +
                (FEE_DFLASH_SECTOR_SIZE - FEE_DFLASH_WORDLINE_SIZE);
          if (SectorInfoPtr->NextFreeWLAddr > EndAddr)
          {
            /* No space - skip writing State Page and proceed */
            SectorInfoPtr->Status.Dirty = FEE_ZERO;
            if ((StateDataPtr->FeeSectorInfo[0].Status.Dirty == 
                                        (unsigned_int)FEE_ZERO) &&
                (StateDataPtr->FeeSectorInfo[1].Status.Dirty == 
                                        (unsigned_int)FEE_ZERO))
            {
              /* Go to Init Normal State Machine */
              StateDataPtr->FeeInitGCState = FEE_INITGC_NORMAL;
            }
            else
            {
              /* Other sector state page needs repair -
                 remain in the same state */
            }
          }
          else
          {
            StateDataPtr->FeeIntrJobFailCount++;
            StateDataPtr->FeeRepairStep = FEE_PROGRAM_STATEPAGE;
          }
        }
        /* Compare failed twice - enter Fail state*/
        else
        {
          Fee_lHandleInitGcFail();
        }
      }
      /* Repair successful */
      else
      {
        StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;
        SectorInfoPtr->Status.Dirty = FEE_ZERO;
        if((StateDataPtr->FeeSectorInfo[0].Status.Dirty 
                                    == (unsigned_int)FEE_ZERO) &&
           (StateDataPtr->FeeSectorInfo[1].Status.Dirty 
                                    == (unsigned_int)FEE_ZERO))
        {
          /* Go to Init Normal State Machine */
          StateDataPtr->FeeInitGCState = FEE_INITGC_NORMAL;
        }
        else
        {
          /* Other sector state page needs repair -
             remain in the same state */
        }
      }
      break;
    }

    /* Remaining case FEE_PROGRAM_STATEPAGE */
    default:
    {
      ReadWriteBuffPtr = StateDataPtr->FeeGcReadWriteBuffer;
      ReturnVal1 = Fls_17_Pmu_CompareWordsSync(
                              (Fls_AddressType)DestAddr,
                              (uint32 *)(void *)ReadWriteBuffPtr,
                              (Fls_LengthType)FEE_STATEPAGE_DATA_SIZE/FEE_FOUR);

      /* Check if compare failed */
      if(ReturnVal1 == E_NOT_OK)
      {
        /* This state is entered because compare has
              already failed once during repair */
        /* So go to fail state */
        Fee_lHandleInitGcFail();
      }
      else
      {
        StateDataPtr->FeeRepairStep = FEE_PROGRAM_MARKER;
      }
      break;
    }
  }/* END OF SWITCH */
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lInitGCPrepDFlSTransHandler(void)       **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function shall handle the transitions required by the   **
** InitGC state machine during preparation of DFlash                          **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lInitGCPrepDFlSTransHandler(void)
{
  Fee_StateDataType  *StateDataPtr;
  Fee_SectorInfoType *SectorInfoPtr0;
  Fee_SectorInfoType *SectorInfoPtr1;
  uint8              *SrcAddr;
  uint32              DestAddr;
  uint8               PrepDFlashState;
  Std_ReturnType      ReturnVal1;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  StateDataPtr->FeeUnErasableWLCountTemp = FEE_ZERO;
  StateDataPtr->FeeUnErasableWLAddrTemp = FEE_ZERO;
  SectorInfoPtr0 = &(StateDataPtr->FeeSectorInfo[0]);
  SectorInfoPtr1 = &(StateDataPtr->FeeSectorInfo[1]);

  PrepDFlashState = StateDataPtr->FeePrepDFLASHState;
  DestAddr = (Fls_AddressType)(StateDataPtr->FeeCurrSectSrcAddr);

  /****** CASE FEE_PREPDF_ERASE_S0 ******************/
  if (PrepDFlashState == FEE_PREPDF_ERASE_S0)
  {
    ReturnVal1 = Fls_17_Pmu_VerifyErase(FEE_ZERO, 
                            &(StateDataPtr->FeeUnErasableWLAddrTemp),
                            &(StateDataPtr->FeeUnErasableWLCountTemp));

    /* Check if compare failed */
    if(ReturnVal1 == E_NOT_OK)
    {
      Fee_lHandleInitGcFail();
    }
    else
    {
      SectorInfoPtr0->UnerasableWLCount =StateDataPtr->FeeUnErasableWLCountTemp;
      SectorInfoPtr0->UnerasableWLAddr = StateDataPtr->FeeUnErasableWLAddrTemp;
      StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK1_BASE;
      PrepDFlashState = FEE_PREPDF_ERASE_S1;
    }
  }

  /****** CASE FEE_PREPDF_ERASE_S1 ******************/
  else if (PrepDFlashState == FEE_PREPDF_ERASE_S1)
  {
    ReturnVal1 = Fls_17_Pmu_VerifyErase(FEE_ONE, 
                            &(StateDataPtr->FeeUnErasableWLAddrTemp),
                            &(StateDataPtr->FeeUnErasableWLCountTemp));
    /* Check if compare failed */
    if(ReturnVal1 == E_NOT_OK)
    {
      Fee_lHandleInitGcFail();
    }
    else
    {
      SectorInfoPtr1->UnerasableWLCount =StateDataPtr->FeeUnErasableWLCountTemp;
      SectorInfoPtr1->UnerasableWLAddr = StateDataPtr->FeeUnErasableWLAddrTemp;
      StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK0_BASE;
      PrepDFlashState = FEE_PREPDF_MARK_S0_ERASED_SP;
    }
  }

  /****** All other state page program casess******************/
  else
  {
    SrcAddr = StateDataPtr->FeeGcReadWriteBuffer;
    ReturnVal1 = Fls_17_Pmu_CompareWordsSync(
                            (Fls_AddressType)DestAddr,
                            (uint32 *)(void *)SrcAddr,
                            (Fls_LengthType)FEE_STATEPAGE_SIZE/FEE_FOUR);

    if ( (ReturnVal1 == E_NOT_OK)  )
    {
      /* Compare failed the first time */
      if(StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
      {
        StateDataPtr->FeeIntrJobFailCount++;
        DestAddr = Fee_lGetNextWLAddr(DestAddr);
        StateDataPtr->FeeCurrSectSrcAddr = DestAddr;

        switch (PrepDFlashState)
        {
          case FEE_PREPDF_MARK_S0_ERASED_SP:
          case FEE_PREPDF_MARK_S0_ERASED_M:
          {
            PrepDFlashState = FEE_PREPDF_MARK_S0_ERASED_SP;
            break;
          }
          case FEE_PREPDF_MARK_S0_VALID_SP:
          case FEE_PREPDF_MARK_S0_VALID_M:
          {
            PrepDFlashState = FEE_PREPDF_MARK_S0_VALID_SP;
            break;
          }

          /*
            Remaining cases:
              FEE_PREPDF_MARK_S1_ERASED_SP
              FEE_PREPDF_MARK_S1_ERASED_M
          */
          default:
          {
            PrepDFlashState = FEE_PREPDF_MARK_S1_ERASED_SP;
            break;
          }
        }
      }
      /* Compare failed twice */
      else
      {
        Fee_lHandleInitGcFail();
      } /* End of if if(StateDataPtr->FeeIntrJobFailCount == FEE_ZERO) */
    }
    /* Compare successful */
    else
    {
      switch (PrepDFlashState)
      {
        case FEE_PREPDF_MARK_S0_ERASED_SP:
        {
          PrepDFlashState = FEE_PREPDF_MARK_S0_ERASED_M;
          break;
        }

        case FEE_PREPDF_MARK_S0_ERASED_M:
        {
          StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;
          DestAddr = Fee_lGetNextWLAddr(DestAddr);
          StateDataPtr->FeeCurrSectSrcAddr = DestAddr;
          PrepDFlashState = FEE_PREPDF_MARK_S0_VALID_SP;
          break;
        }

        case FEE_PREPDF_MARK_S0_VALID_SP:
        {
          PrepDFlashState = FEE_PREPDF_MARK_S0_VALID_M;
          break;
        }

        case FEE_PREPDF_MARK_S0_VALID_M:
        {
          StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;
          SectorInfoPtr0->StatePageAddr = DestAddr;
          DestAddr = Fee_lGetNextWLAddr(DestAddr);
          StateDataPtr->FeeNextFreePageAddr = DestAddr;
          SectorInfoPtr0->NextFreeWLAddr = DestAddr;
          StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK1_BASE;
          PrepDFlashState = FEE_PREPDF_MARK_S1_ERASED_SP;
          break;
        }

        case FEE_PREPDF_MARK_S1_ERASED_SP:
        {
          PrepDFlashState = FEE_PREPDF_MARK_S1_ERASED_M;
          break;
        }

        /*
          Remaining case FEE_PREPDF_MARK_S1_ERASED_M
        */
        default:
        {
          StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;
          SectorInfoPtr1->StatePageAddr = DestAddr;
          DestAddr = Fee_lGetNextWLAddr(DestAddr);
          SectorInfoPtr1->NextFreeWLAddr = DestAddr;
          /* Set Garbage Collection state to Idle */
          StateDataPtr->FeeGcState = FEE_GC_IDLE;
          /* Mark end of initialization */
          StateDataPtr->FeeInitGCState = FEE_INITGC_COMPLETE;
          break;
        }
      }/* End of SWITCH */
    }/* End of if ( (ReturnVal1 == E_NOT_OK) || (ReturnVal1 == E_NOT_OK) )*/
  }

  StateDataPtr->FeePrepDFLASHState = PrepDFlashState;
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lInitGCTransHandler(void)               **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function shall handle the transitions required by the   **
** InitGC state machine                                                       **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lInitGCTransHandler(void)
{
  Fee_StateDataType  *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /**********************************************************************/
  /*                1.AYSNC STATE MACHINE TRANSITIONS                   */
  /**********************************************************************/

  /* Is the Job finished i.e. Should the State machine Transition be made */
  if(StateDataPtr->FeeIntrJobStatus == FEE_JOB_DONE)
  {
    /* If the Job has failed */
    if(StateDataPtr->FeeIntrJobResult == FEE_JOB_FAIL)
    {
     Fee_lHandleInitGcFail();
    }
    /* Job has passed */
    else
    {
      /***************** CASE 2: INITGC DIRTY STATE MACHINE ****************/
      if( StateDataPtr->FeeInitGCState == FEE_INITGC_HANDLE_DIRTY)
      {
        Fee_lInitGCDSMTransHandler();
      }/* End of FEE_INITGC_HANDLE_DIRTY */


      /***************** CASE 4: DFLASH PREPARATION STATE MACHINE *********/
      if(StateDataPtr->FeeInitGCState == FEE_INITGC_PREPARE_DFLASH)
      {
        Fee_lInitGCPrepDFlSTransHandler();
      } /* END OF IF (PREPARE_DFLASH) */
    }/* END OF JOB PASS check */
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
  }
  else
  {
    /* State Transition not required as no job was requested */
    /* Nothing to do */
  }
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_HE2_FEE_PR2964,DS_NAS_EP_FEE_PR2964]
                  [/cover]
********************************************************************************
** Syntax : static void Fee_lInitGC(void)                                     **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function shall detect if there was any interruption in  **
** the ongoing Garbage Collection procedure. If yes, then this routine shall  **
** get the DFLASH contents in the normal state.                               **
*******************************************************************************/
static void Fee_lInitGC(void)
{
  Fee_StateDataType  *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /**********************************************************************/
  /*                1.AYSNC STATE MACHINE TRANSITIONS                   */
  /**********************************************************************/

  Fee_lInitGCTransHandler();

  /**********************************************************************/
  /*                       2.STATE MACHINE EXECUTION                    */
  /**********************************************************************/

  /***************** CASE 1: INITGC INVALID STATE MACHINE ********************/

  if(StateDataPtr->FeeInitGCState == FEE_INITGC_HANDLE_INVALID)
  {
    if(FEE_CONFIG_PTR->FeeIllegalStateNotification != NULL_PTR)
    {
      /* Call the Notification function */
      FEE_CONFIG_PTR->FeeIllegalStateNotification();
    }

    /* Erase the sectors */
    if(FEE_CONFIG_PTR->FeeEraseAllEnable == (boolean)TRUE)
    {
      StateDataPtr->FeeInitGCState = FEE_INITGC_PREPARE_DFLASH;
      StateDataPtr->FeePrepDFLASHState = FEE_PREPDF_ERASE_S0;
      StateDataPtr->FeeCurrSectSrcAddr = FEE_DFLASH_BANK0_BASE;
      StateDataPtr->FeeStateCount = FEE_ZERO;
    }
    else
    {
      StateDataPtr->FeeInitGCState = FEE_INITGC_FAIL;
    }
  }

  /***************** CASE 2: INITGC DIRTY STATE MACHINE ********************/

  if(StateDataPtr->FeeInitGCState == FEE_INITGC_HANDLE_DIRTY)
  {
    Fee_lIDSM();
  }

  /***************** CASE 3: INITGC NORMAL STATE MACHINE *******************/

  if(StateDataPtr->FeeInitGCState == FEE_INITGC_NORMAL)
  {
    Fee_lINSM();
  }

  /*********** CASE 4: DFLASH PREPARATION STATE MACHINE ********************/

  if( StateDataPtr->FeeInitGCState == FEE_INITGC_PREPARE_DFLASH)
  {
    #if (FEE_VIRGIN_FLASH_ILLEGAL_STATE == STD_ON)
    if(FEE_CONFIG_PTR->FeeEraseAllEnable == (boolean)TRUE)
    {
      Fee_lIPrepDFlashSM();
    }
    else
    {
      if(FEE_CONFIG_PTR->FeeIllegalStateNotification != NULL_PTR)
      {
        /* Call the illegal state notification function */
        FEE_CONFIG_PTR->FeeIllegalStateNotification();
      }
      StateDataPtr->FeeInitGCState = FEE_INITGC_FAIL;
    }
    #else
    Fee_lIPrepDFlashSM();
    #endif
  }

}/*End of Fee_lInitGC */


/*******************************************************************************
** Syntax : static void Fee_lLastPgHandler(void)                              **
**                                                                            **
** Service ID:                                                                **
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
** Description : This routine will perform end of job handling activities     **
**    for Write, Invalidate requests                    .                     **
*******************************************************************************/
static void Fee_lLastPgHandler(void)
{
  Fee_StateDataType           *StateDataPtr;
  Fee_LastWrittenBlkInfoType  *LastBlkInfoPtr;
  Fee_CacheType               *BlkPtr;
  uint32                       LWBMarkerAddress;
  uint32                       NextFreePageWLAddr;
  uint16                       BlockIndex;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  BlockIndex = Fee_lGetUsrBlockIndex(StateDataPtr->FeeCurrReqBlockNum);
  BlkPtr = &(StateDataPtr->FeeBlockInfo[BlockIndex]);
  /*****************   UPDATE THE CACHE TABLE *********************/

  /* Update latest copy information in the cache table */
  BlkPtr->Address = StateDataPtr->FeeBlockStartAddr;
  BlkPtr->Status.Consistent = FEE_ONE;
  /* If Invalidate request was ongoing then reset the flag */
  if(StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus == (unsigned_int)FEE_ONE)
  {
    BlkPtr->Status.Valid = FEE_ZERO;
  }
  else
  {
    BlkPtr->Status.Valid = FEE_ONE;
  }

  /* if Garbage Collection is pending */
  if(StateDataPtr->FeeGcState != FEE_GC_IDLE)
  {
    if(BlkPtr->Status.Copied != FEE_ZERO)
    {
      #if (FEE_GET_PREV_DATA_API == STD_ON)
      BlkPtr->PrevCopyAddress = BlkPtr->Address;
      #endif
      BlkPtr->Status.PrevCopyValid = BlkPtr->Status.Valid;
      BlkPtr->Status.PrevCopyConsistent = BlkPtr->Status.Consistent;
      BlkPtr->Status.PrevCopyCopied = FEE_ONE;
    }
    else
    {
      BlkPtr->Status.PrevCopyCopied = FEE_ZERO;
    }
    BlkPtr->Status.Copied = FEE_ZERO;
    StateDataPtr->FeeStatusFlags.FeeBlkModified = FEE_ONE; /*BLK IS MODIFIED*/
  }

  /* Update Last written block information */
  LWBMarkerAddress = StateDataPtr->FeeNextFreePageAddr - FEE_DFLASH_PAGE_SIZE;
  NextFreePageWLAddr = Fee_lGetWLBeginning(StateDataPtr->FeeNextFreePageAddr);
  if(((StateDataPtr->FeeBlockStartAddr) < NextFreePageWLAddr) &&
      (LWBMarkerAddress >= NextFreePageWLAddr))
  {
    LastBlkInfoPtr = &(StateDataPtr->FeeLastWrittenBlkInfo);
    LastBlkInfoPtr->BlockNumber = StateDataPtr->FeeCurrReqBlockNum;
    LastBlkInfoPtr->PageCount = StateDataPtr->FeeBlkPageCount;
    LastBlkInfoPtr->Addr = StateDataPtr->FeeBlockStartAddr;
    LastBlkInfoPtr->Status.Consistent = FEE_ONE; /*Consistent = 1 */
  }

  /************** RESET/UPDATE GLOBAL VARIABLES USED *******************/
  StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_NO_REQ;
  StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus = FEE_ZERO;
  StateDataPtr->FeeJobResult = MEMIF_JOB_OK;
  StateDataPtr->FeeUserJobFailCount = FEE_ZERO;
  StateDataPtr->FeeBlkPageCount = FEE_ZERO;

  /* Call user configured job End notification */
  if ( FEE_CONFIG_PTR->FeeNvmJobEndNotification != NULL_PTR )
  {
    (FEE_CONFIG_PTR->FeeNvmJobEndNotification)();
  }
}/* End of Fee_lLastPgHandler */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lPrepareInvldReqBuff(                   **
**                                          uint32 BlockCycleCount,           **
**                                          uint8 *ReadWriteBuffPtr           **
**                                          uint16 BlockIndex                 **
**                                       )                                    **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockCycleCount : Blk count of block to be written      **
**                    ReadWriteBuffPtr : Buffer pointer used for invalidate   **
**                    BlockIndex : Index at which block details are           **
**                       stored in the Configuration Block Array              **
**                                                                            **
** Parameters (in-out) :StateDataPtr : Pointer to structure of                **
**                                     FEE Global variables                   **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine will fill the readwrite buffer with data        **
** for invalidation.                                                          **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lPrepareInvldReqBuff(uint32 BlockCycleCount,
                                              uint8 *ReadWriteBuffPtr,
                                              uint16 BlockIndex)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *BlkPtr;
  uint16                 BlockNumber;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  BlockNumber = StateDataPtr->FeePendReqInfo.BlockNumber;
  StateDataPtr->FeeCurrReqBlockNum = BlockNumber;
  BlockCycleCount = BlockCycleCount + FEE_ONE;

 /* Compose the write buffer with page ID and other details */
 if(StateDataPtr->FeeBlkPageCount == FEE_ZERO)
 {
    /* First Page write */
    *(ReadWriteBuffPtr) = FEE_DATA_FIRST_PAGE_ID;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 1U) = (uint8)(BlockNumber);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 2U) = (uint8)(BlockNumber >> FEE_EIGHT);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 3U) = (uint8)(BlockCycleCount);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 4U) = (uint8)(BlockCycleCount >> FEE_EIGHT);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 5U) = (uint8)(BlockCycleCount >> FEE_SIXTEEN);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 6U) = FEE_ONE; /* Page length = 1 */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 7U) = FEE_ZERO; /* Page length = 1, Valid bit zero */

    /* Push latest copy details of the block into Previous copy */
    /* Update previous copy information */
    BlkPtr = &(StateDataPtr->FeeBlockInfo[BlockIndex]);
    BlkPtr->Status.PrevCopyValid = BlkPtr->Status.Valid;
    BlkPtr->Status.PrevCopyConsistent = BlkPtr->Status.Consistent;
    BlkPtr->Status.PrevCopyCopied = BlkPtr->Status.Copied;
    #if (FEE_GET_PREV_DATA_API == STD_ON)
    BlkPtr->PrevCopyAddress = BlkPtr->Address;
    #endif

    /* Mark the block as inconsistent */
    BlkPtr->Status.Consistent = FEE_ZERO;

    StateDataPtr->FeeBlkPageCount = FEE_ONE;
  }
  else
  {
    *(ReadWriteBuffPtr)     = FEE_DATA_MARKER_PAGE_ID;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 1) = FEE_DP_MARKER_BYTE_2;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 2) = FEE_DP_MARKER_BYTE_3;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 3) = FEE_DP_MARKER_BYTE_4;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 4) = (uint8)(BlockNumber);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 5) = (uint8)(BlockNumber >> FEE_EIGHT);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 6U) = FEE_ONE; /* Page length = 1 */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((ReadWriteBuffPtr) + 7U) = FEE_ZERO; /* Page length = 1, Valid bit zero */

    StateDataPtr->FeeBlkPageCount = FEE_TWO;
  }
  StateDataPtr->FeeReadLen = FEE_EIGHT;
  StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_COMPARE;
}

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_NAS_FEE_PR1067,DS_AS_FEE154,DS_AS_FEE153]
**                                                                            **
** Syntax : IFX_LOCAL_INLINE void Fee_lPrepareWriteReqBuff                    **
**                                        (                                   **
**                                           uint32 BlockCycleCount,          **
**                                           uint8 *ReadWriteBuffPtr          **
**                                           uint16  BlockIndex,              **
**                                           uint16 BlockSize                 **
**                                        )                                   **
** [/cover]                                                                   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockCycleCount : Blk count of block to be written      **
**                    ReadWriteBuffPtr : Buffer Pointer to be filled for write**
**                    BlockIndex : Index at which block details are           **
**                       stored in the Configuration Block Array              **
**                    BlockSize : Size of the block to be written             **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine will fill the read write buffer with data       **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lPrepareWriteReqBuff(uint32 BlockCycleCount,
                                              uint8 *ReadWriteBuffPtr,
                                              uint16 BlockIndex,
                                              uint16 BlockSize)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *BlkPtr;
  uint8*                 DataBufferPtr;
  Fee_PageType           BlockPages_1;
  uint16                 DataByteCnt;
  uint16                 BlockNumber;
  uint8                  EndOfWLReached;

  EndOfWLReached = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  BlockNumber = StateDataPtr->FeePendReqInfo.BlockNumber;
  DataBufferPtr = StateDataPtr->FeePendReqInfo.DataBufferPtr;
  BlockCycleCount = BlockCycleCount + FEE_ONE;
  BlockPages_1 = Fee_lGetDFLASHPages(BlockSize) - FEE_ONE;
  /* Update the block number */
  StateDataPtr->FeeCurrReqBlockNum = BlockNumber;

  /* Compose the write buffer with page ID and other details */
  if(StateDataPtr->FeeBlkPageCount == BlockPages_1)
  {
    *(ReadWriteBuffPtr)     = FEE_DATA_MARKER_PAGE_ID;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 1) = FEE_DP_MARKER_BYTE_2;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 2) = FEE_DP_MARKER_BYTE_3;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 3) = FEE_DP_MARKER_BYTE_4;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 4) = (uint8)(BlockNumber);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 5) = (uint8)(BlockNumber >> FEE_EIGHT);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 6) = (uint8)(BlockPages_1);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + 7) = (uint8)((BlockPages_1 >> FEE_EIGHT) |
                                                 (FEE_VALID_BIT));
    StateDataPtr->FeeBlkPageCount++;

    StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_COMPARE;
    StateDataPtr->FeeReadLen = FEE_PAGE1_DATA_OFFSET;
  }
  else
  {
    if(StateDataPtr->FeeBlkPageCount == FEE_ZERO)
    {
      /* First Page write */
      *(ReadWriteBuffPtr) = FEE_DATA_FIRST_PAGE_ID;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      *((ReadWriteBuffPtr) + 1U) = (uint8)(BlockNumber);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      *((ReadWriteBuffPtr) + 2U) = (uint8)(BlockNumber >> FEE_EIGHT);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      *((ReadWriteBuffPtr) + 3U) = (uint8)(BlockCycleCount);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      *((ReadWriteBuffPtr) + 4U) = (uint8)(BlockCycleCount >> FEE_EIGHT);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      *((ReadWriteBuffPtr) + 5U) = (uint8)(BlockCycleCount >> FEE_SIXTEEN);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      *((ReadWriteBuffPtr) + 6U) = (uint8)(BlockPages_1);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      *((ReadWriteBuffPtr) + 7U) = (uint8)((BlockPages_1 >> FEE_EIGHT) |
                                           (FEE_VALID_BIT));

      StateDataPtr->FeeReadLen = FEE_PAGE1_DATA_OFFSET;
      /* Push latest copy details of the block into Previous copy */
      /* Update previous copy information */
      BlkPtr = &(StateDataPtr->FeeBlockInfo[BlockIndex]);
      BlkPtr->Status.PrevCopyValid = BlkPtr->Status.Valid;
      BlkPtr->Status.PrevCopyConsistent = BlkPtr->Status.Consistent;
      BlkPtr->Status.PrevCopyCopied = BlkPtr->Status.Copied;
      #if (FEE_GET_PREV_DATA_API == STD_ON)
      BlkPtr->PrevCopyAddress = BlkPtr->Address;
      #endif

      /* Mark the block as inconsistent */
      BlkPtr->Status.Consistent = FEE_ZERO;

      StateDataPtr->FeeBlkPageCount++;

      if(Fee_lIsWLBeginning(StateDataPtr->FeeNextFreePageAddr + 
         StateDataPtr->FeeReadLen) == 
         FEE_ZERO)
      {
        EndOfWLReached = FEE_ONE;
        StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_COMPARE;        

      }
    }

    if (EndOfWLReached == FEE_ZERO)
    {
      /* Add data to the write buffer from the user specified pointer */
      DataByteCnt = StateDataPtr->FeeReadLen;
      do
      {
        if((StateDataPtr->FeeReadLen & FEE_SEVEN) == FEE_ZERO)
        {
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          *(ReadWriteBuffPtr + StateDataPtr->FeeReadLen) =
          FEE_DATA_NEXT_PAGE_ID;
          StateDataPtr->FeeBlkPageCount++;

          StateDataPtr->FeeReadLen++;
        }
        else
        {
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to
          efficiently access the data within 
          FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          *(ReadWriteBuffPtr + StateDataPtr->FeeReadLen) = *(DataBufferPtr);
          StateDataPtr->FeeReadLen++;
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            access user data buffer efficiently. User ensures the 
            availability of sufficient buffer data space for the requested
            operation*/
          DataBufferPtr++;
          StateDataPtr->FeeUserWriteBytesCount++;
       }
      }while((Fee_lIsWLBeginning(StateDataPtr->FeeNextFreePageAddr + \
              StateDataPtr->FeeReadLen)!= FEE_ZERO) && \
              (StateDataPtr->FeeUserWriteBytesCount != BlockSize) && \
              ((StateDataPtr->FeeReadLen - DataByteCnt)< \
              ((uint16)(FEE_WRITE_LEN_MAX))));
 
                        
      /* If this is the last page being written for the write request */
      if(StateDataPtr->FeeUserWriteBytesCount == BlockSize)
      {
        /* If there is a scope for stuff bits(all-one) */
        while((StateDataPtr->FeeReadLen & FEE_SEVEN) != FEE_ZERO)
        {
          /*Stuff all-one for remaining bits of FeeReadWriteBuffer*/
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          *(ReadWriteBuffPtr + StateDataPtr->FeeReadLen) = FEE_8BIT_ALL_ONE;
          StateDataPtr->FeeReadLen++;
        }
        StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_COMPARE;

      }
      /* If this is beginning of next word line */
      else if(Fee_lIsWLBeginning(StateDataPtr->FeeNextFreePageAddr +
              StateDataPtr->FeeReadLen) == FEE_ZERO)
      {
        StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_COMPARE;
      }
      else
      {
        /* dummy else to avoid MISRA */
      }

      /* Store the request parameters in last request buffer */
      StateDataPtr->FeePendReqInfo.DataBufferPtr = DataBufferPtr;
    }
  }

} /* End of User Write req handling */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE Std_ReturnType Fee_lCheckBlkCycleCnt(            **
**                                            const Fee_BlockType *BlockPtr   **
**                                            uint32 BlockCycleCount)         **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockPtr                                                **
**                    BlockCycleCount                                         **
**                                                                            **
** Parameters (out):  RetVal: E_NOT_OK / E_OK                                 **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine checks if the block cycle count of the          **
** requested user block has reached the configured maximum                    **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fee_lCheckBlkCycleCnt(
                                            const Fee_BlockType *BlockPtr,
                                            uint32 BlockCycleCount)
{
  Fee_StateDataType     *StateDataPtr;
  uint32                 CycleCountCfg;
  Std_ReturnType         RetVal;

  RetVal = E_OK;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  CycleCountCfg = BlockPtr->CycleCountLimit;

  /* If the cycle count limit is reached then
     report FEE_E_WRITE_CYCLES_EXHAUSTED to DEM  */
  if( (CycleCountCfg != FEE_ZERO) && (BlockCycleCount == CycleCountCfg ))
  {
    #if (FEE_WRITE_CYCLES_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
    Dem_ReportErrorStatus(FEE_E_WRITE_CYCLES_EXHAUSTED,
                          DEM_EVENT_STATUS_FAILED);
    #endif

    StateDataPtr->FeeJobResult = MEMIF_JOB_FAILED;
    StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_NO_REQ;

    /* Clear the DFLASH access for FEE */

    if ( FEE_CONFIG_PTR->FeeNvmJobErrorNotification != NULL_PTR )
    {
      (FEE_CONFIG_PTR->FeeNvmJobErrorNotification)();
    }
    RetVal = E_NOT_OK;
  }

  return (RetVal);

}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleUserWriteReq(                    **
**                                                     uint16 UserBlockIndex) **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  UserBlockIndex: Index of the requested block            **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine handles the functionality of FEE_REQ_NORM_WRITE **
** / FEE_REQ_IMM_WRITE state of Write state machine                           **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleUserWriteReq(uint16 UserBlockIndex)
{
  Fee_StateDataType           *StateDataPtr;
  const Fee_BlockType         *BlockPtr;
  uint8                       *PendReqStatePtr;
  Fee_LastWrittenBlkInfoType  *LastWrittenBlkPtr;
  uint32                       BlockCycleCount;
  uint32                       DestAddr;
  uint32                       TempAddr;
  uint32                       Index;
  uint16                       Offset;
  Fls_LengthType               Length;
  Std_ReturnType               ReturnValue;

  BlockCycleCount = FEE_ZERO;
  ReturnValue = E_OK;
  Length = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    Fee_BlockConfig[Number of configured blocks] to efficiently access its 
    elements*/
  BlockPtr = FEE_CONFIG_PTR->FeeBlockConfigPtr + UserBlockIndex;
  PendReqStatePtr = &(StateDataPtr->FeePendReqStatus);
  LastWrittenBlkPtr = &(StateDataPtr->FeeLastWrittenBlkInfo);
  if(StateDataPtr->FeeReadLen == (uint16)0)
  {
    /* Read block cycle count */
    if (StateDataPtr->FeeBlockInfo[UserBlockIndex].Address != FEE_ZERO)
    {
      BlockCycleCount = Fee_lGetBlkCycleCount(UserBlockIndex);
    }
    /* Check if Block Cycle Count has reached the maximum */
    if (StateDataPtr->FeeUserJobFailCount == FEE_ZERO)
    {
      ReturnValue = Fee_lCheckBlkCycleCnt(BlockPtr, BlockCycleCount);
    }
  
    if (ReturnValue != E_NOT_OK) /* If block cycle count not exceeded */
    {
      /* Check if request can be performed */
      ReturnValue = Fee_lPerformWriteCheck(BlockPtr);
    }
  }
  /* If write is to be started */
  if( ReturnValue == E_OK )
  {
    DestAddr = StateDataPtr->FeeNextFreePageAddr;

    /* If it is not a new wordline and if previous data is consistent */
    if(Fee_lIsWLBeginning(DestAddr) != FEE_ZERO)
    {
      /* Read the previous page content to LastWrittenBlkBuff.
      This is used for comparison after the 1st page of curr blk is written */
      TempAddr = Fee_lGetWLBeginning(DestAddr);
      Length = (Fls_LengthType)(DestAddr & FEE_DFLASH_WORDLINE_SIZE_1);
      Length -= StateDataPtr->FeeReadLen;
      if(Length > (Fls_LengthType)FEE_WRITE_LEN_MAX)
      {
        Length = (Fls_LengthType)FEE_WRITE_LEN_MAX;
      }
      if(Length != (Fls_LengthType)0)
      {
        TempAddr += StateDataPtr->FeeReadLen;
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
          efficiently access the data within 
          FeeLastWrittenBlkBuffer + StateDataPtr->FeeReadLen*/
        ReturnValue = Fls_17_Pmu_ReadWordsSync(
                (Fls_AddressType)TempAddr,
                (uint32 *)(void *)(StateDataPtr->FeeLastWrittenBlkBuffer + 
                StateDataPtr->FeeReadLen),
                Length/FEE_DFLASH_WORD_SIZE);
                
        if(ReturnValue == E_NOT_OK)
        {
          StateDataPtr->FeeReadLen = FEE_ZERO;
          Length = FEE_ZERO;
          /* Update the Copied status so that the last page of the prev block
             is skipped from comparison */
          LastWrittenBlkPtr->Status.Consistent = FEE_ZERO;
          LastWrittenBlkPtr->BlockNumber = FEE_ZERO;
          DestAddr = Fee_lGetNextWLAddr(DestAddr);
          while (Fee_lIsUnerasableAddr(DestAddr))
          {
            DestAddr += FEE_DFLASH_WORDLINE_SIZE;
          }
        }
        else
        {
          
          Offset = (uint16)(Length);
          Offset += StateDataPtr->FeeReadLen;
          /* Copy FeeLastWrittenBlkBuffer to ReadWriteBuffer
             for WL compare after first page write */
          for(Index=StateDataPtr->FeeReadLen;Index<Offset;Index++)
          {
            StateDataPtr->FeeReadWriteBuffer[Index] =
                  StateDataPtr->FeeLastWrittenBlkBuffer[Index];
          } /* End of for(Index =0... */
          StateDataPtr->FeeReadLen += (uint16)(Length);
        } /* end of if(ReturnValue == E_NOT_OK) */
     }
    } /* end of if ( (Fee_lIsWLBeginning(DestAddr) != FEE_ZERO).. */

    /* initialize variables */
    if(Length == (Fls_LengthType)0U)
    {
    StateDataPtr->FeeReadLen = 0U;
    StateDataPtr->FeeBlkPageCount = FEE_ZERO;
    StateDataPtr->FeeUserWriteBytesCount = FEE_ZERO;
    StateDataPtr->FeeBlockStartAddr = DestAddr;
    StateDataPtr->FeeNextFreePageAddr = DestAddr;
    *PendReqStatePtr = FEE_PEND_PREV_WRITE_REQ;
    }
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleUserWriteCompare                 **
**                                                   (uint16 UserBlockIndex)  **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  UserBlockIndex: Index of the requested block            **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine handles the functionality of                    **
** FEE_PEND_PREV_COMPARE state of Write state machine                         **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleUserWriteCompare(uint16 UserBlockIndex)
{
  Fee_StateDataType           *StateDataPtr;
  uint8                       *PendReqStatePtr;
  const Fee_BlockType         *BlockPtr;
  Fee_LastWrittenBlkInfoType  *LastWrittenBlkPtr;
  uint32                       DestAddr;
  uint32                       TempAddr;
  Fls_LengthType               CompareLength;
  Fls_LengthType               TmpCompareLength;
  Std_ReturnType               ReturnValueFls;
  uint8                        Error;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    Fee_BlockConfig[Number of configured blocks] to efficiently access its 
    elements*/
  BlockPtr = FEE_CONFIG_PTR->FeeBlockConfigPtr + UserBlockIndex;
  LastWrittenBlkPtr = &(StateDataPtr->FeeLastWrittenBlkInfo);
  PendReqStatePtr = &(StateDataPtr->FeePendReqStatus);

  DestAddr = StateDataPtr->FeeNextFreePageAddr;
  /* Compare entire WL content */
  TempAddr = Fee_lGetWLBeginning(DestAddr);
  CompareLength = DestAddr - TempAddr;
  CompareLength += StateDataPtr->FeeLastWriteSize;
  TmpCompareLength = CompareLength;
  CompareLength -=  StateDataPtr->FeeComparedLen;
  if(CompareLength > (Fls_LengthType)FEE_COMPARE_LEN_MAX)
  {
    CompareLength = (Fls_LengthType)FEE_COMPARE_LEN_MAX;
  }
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
    efficiently access the data within 
    FeeReadWriteBuffer + StateDataPtr->FeeComparedLen*/
  ReturnValueFls = Fls_17_Pmu_CompareWordsSync(
                              (Fls_AddressType)(TempAddr + 
                              StateDataPtr->FeeComparedLen),
  (uint32 *)(void *)(StateDataPtr->FeeReadWriteBuffer +
  StateDataPtr->FeeComparedLen), (Fls_LengthType)(CompareLength / FEE_FOUR));
                              
  StateDataPtr->FeeComparedLen += (uint16)CompareLength;
  /* Compare failed */
  if(ReturnValueFls == E_NOT_OK)
  {
    StateDataPtr->FeeComparedLen = FEE_ZERO;
    /* Compare failed first time */
    if(StateDataPtr->FeeUserJobFailCount == FEE_ZERO)
    {
      StateDataPtr->FeeUserJobFailCount++;
      StateDataPtr->FeeWriteAffectedAddr = StateDataPtr->FeeBlockStartAddr;
      TempAddr = Fee_lGetNextWLAddr(DestAddr);
      StateDataPtr->FeeNextFreePageAddr = TempAddr;
      /* Point to the start of the user data buffer to re-write user block */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
        access user data buffer efficiently. User ensures the 
        availability of sufficient buffer data space for the requested
        operation*/
      StateDataPtr->FeePendReqInfo.DataBufferPtr -=
                           StateDataPtr->FeeUserWriteBytesCount;
      /* Last Written Block affected? */
      if(LastWrittenBlkPtr->Status.Consistent == (unsigned_int)FEE_ONE)
      {
        StateDataPtr->FeeWLAffectedType = FEE_LWB_ACROSS_WL_AFFECTED;
        /* Point to addr of the affected prev block */
        StateDataPtr->FeeCurrSectSrcAddr = LastWrittenBlkPtr->Addr;
        StateDataPtr->FeeCurrReqBlockNum = LastWrittenBlkPtr->BlockNumber;
        /* Initialise variables for next write operation */
        StateDataPtr->FeeBlkPageCount = FEE_ZERO;
        StateDataPtr->FeeBlockStartAddr = TempAddr;
        /* Move to Internal read */
        *PendReqStatePtr = FEE_PEND_INTR_READ;
      }
      else if(Fee_lIsLastWrittenWLAffected(FEE_ZERO) == (boolean)TRUE)
      {
        StateDataPtr->FeeWLAffectedType = FEE_LWB_WITHIN_WL_AFFECTED;
        /* Affected data is within the last written block buffer */
        /* Initialise variables */
        StateDataPtr->FeeBlkPageCount = FEE_ZERO;
        StateDataPtr->FeeBlockStartAddr = TempAddr;
        /* Move to Internal read */
        *PendReqStatePtr = FEE_PEND_INTR_READ;
      }
      else
      {
        /* Re-write the current block */
        Fee_lRegisterWriteRequest(StateDataPtr->FeePendReqInfo.DataBufferPtr,
                                  BlockPtr);
      }
    }
    /* Compare failed twice */
    else
    {
      /* Update Next Free Page Address */
      StateDataPtr->FeeNextFreePageAddr = Fee_lGetNextWLAddr(DestAddr);

      /* Handle job failure: Raise DEM and Job error notification */
      if(StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus 
                                               == (unsigned_int)FEE_ONE)
      {
        StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus = FEE_ZERO;
        Error = FEE_INVALIDATE;
      }
      else
      {
        Error = FEE_WRITE;
      }
      Fee_lHandleUsrJobFailure(Error); /* DEM & Job Error Notification */
    }
  }
  /* Compare Successful */
  else
  {
    if(StateDataPtr->FeeComparedLen == TmpCompareLength)
    {
      StateDataPtr->FeeComparedLen = FEE_ZERO;
      StateDataPtr->FeeNextFreePageAddr += StateDataPtr->FeeLastWriteSize;
      /* Is it Invalidate request? */
      if((StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus 
                        == (unsigned_int)FEE_ONE) &&
         (StateDataPtr->FeeBlkPageCount == FEE_TWO))
      {
        Fee_lLastPgHandler();
      }
      /* Write request - All pages written? */
      else if(StateDataPtr->FeeBlkPageCount == 
              Fee_lGetDFLASHPages((uint16)(BlockPtr->Size)))
      {
        Fee_lLastPgHandler();
      }
      else
      {
        *PendReqStatePtr = FEE_PEND_PREV_WRITE_REQ;
      }
    }
  }
}

/*******************************************************************************
** Syntax : static boolean Fee_lIsLastWrittenWLAffected(uint16 StartOffset)   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  StartOffset - Offset                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  boolean                                                 **
**                                                                            **
** Description : This function checks if the last written word-line is        **
**               affected                                                     **
*******************************************************************************/
static boolean Fee_lIsLastWrittenWLAffected(uint16 StartOffset)
{
  Fee_StateDataType  *StateDataPtr;
  uint8              *LWBBufferPtr;
  uint32              WLAddr;
  uint16              EndOffset;
  uint16              BlockNumber;
  boolean             RetVal;

  RetVal = (boolean)FALSE;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  LWBBufferPtr = &(StateDataPtr->FeeLastWrittenBlkBuffer[0]);
  EndOffset = (uint16)((StateDataPtr->FeeWriteAffectedAddr) & 
                        FEE_DFLASH_WORDLINE_SIZE_1);

  /* Check if any block header is present in the last written word-line */
  if(EndOffset > (uint16)FEE_DFLASH_PAGE_SIZE)
  {
    WLAddr = Fee_lGetWLBeginning(StateDataPtr->FeeWriteAffectedAddr);
    EndOffset -= FEE_DFLASH_PAGE_SIZE;
    do
    {
      /* if any block header is found */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeLastWrittenBlkBuffer[]*/
      if(LWBBufferPtr[StartOffset] == (uint8)FEE_DATA_FIRST_PAGE_ID)
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
          access the data within FeeLastWrittenBlkBuffer[]*/
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
          access the data within FeeLastWrittenBlkBuffer[]*/
        BlockNumber = ((uint16)(((uint16)LWBBufferPtr[StartOffset + FEE_TWO]) <<
                          FEE_EIGHT) | ((uint16)LWBBufferPtr[StartOffset +1U]));
        /* check if the block and its address is in cache table */
        RetVal = Fee_lIsBlockAffected(BlockNumber, (WLAddr + StartOffset));
      }
      StartOffset += FEE_DFLASH_PAGE_SIZE;
    }while((StartOffset < EndOffset) && (RetVal == (boolean)FALSE));
  }

  return (RetVal);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE boolean Fee_lIsBlockAffected(uint16 BlockNumber, **
**                                                       uint32 BlockAddr)    **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber - Block Number                              **
**                    BlockAddr   - Block Address                             **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  boolean                                                 **
**                                                                            **
** Description : This function checks if the block is affected                **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE boolean Fee_lIsBlockAffected(uint16 BlockNumber,
                                             uint32 BlockAddr)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *CachePtr;
  Fee_CacheType         *UnconfigBlockPtr;
  uint16 TempIndex;
  uint16 BlockIndex;
  boolean RetVal;

  RetVal = (boolean)FALSE;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  BlockIndex = Fee_lGetUsrBlockIndex(BlockNumber);
  if(BlockIndex == FEE_LOG_BLOCK_NOT_FOUND)
  {
    /* un-configured block */
    TempIndex = FEE_ZERO;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) + 
                         (FEE_CONFIG_PTR->FeeBlkCnt) + TempIndex;    
    while((TempIndex < (StateDataPtr->FeeUnConfigBlockCount)) &&
          (RetVal == (boolean)FALSE))
    {

      /* check if the block and its address is in cache table */
      if((UnconfigBlockPtr->BlockNumber == BlockNumber) &&
         (UnconfigBlockPtr->Address == BlockAddr))
      {
        RetVal = (boolean)TRUE;
      }
      else
      {
        TempIndex++;
      }
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
        access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
      UnconfigBlockPtr++;
    }
  }
  else
  {
    CachePtr = &(StateDataPtr->FeeBlockInfo[BlockIndex]);
    /* check if the block and its address is in cache table */
    if(CachePtr->Address == BlockAddr)
    {
      RetVal = (boolean)TRUE;
    }
  }
  if(RetVal == (boolean)TRUE)
  {
    /* if the block and its address exists in cache table then, update the 
       current request */
    StateDataPtr->FeeCurrReqBlockNum = BlockNumber;
    StateDataPtr->FeeCurrSectSrcAddr = BlockAddr;
  }

  return (RetVal);
}

/*******************************************************************************
**  TRACEABILITY: [cover parentID=DS_AS_FEE024_FEE036]
**                                                                            **
** Syntax : static void Fee_lHandleUserWrite(uint16 UserBlockIndex)           **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  UserBlockIndex: Index of the requested block            **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine handles the functionality of                    **
** FEE_PEND_PREV_WRITE_REQ state of Write state machine                       **
*******************************************************************************/
static void Fee_lHandleUserWrite(uint16 UserBlockIndex)
{
  Fee_StateDataType     *StateDataPtr;
  const Fee_BlockType   *BlockPtr;
  uint8                 *ReadWriteBuffPtr;
  uint32                 DestAddr;
  uint32                 BlockCycleCount;
  uint16                 BlockSize;
  uint16                 Offset;
  Std_ReturnType         RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    Fee_BlockConfig[Number of configured blocks] to efficiently access its 
    elements*/
  BlockPtr = FEE_CONFIG_PTR->FeeBlockConfigPtr + UserBlockIndex;
  BlockSize = (uint16)BlockPtr->Size;
  BlockCycleCount = FEE_ZERO;

  /* Read block cycle count */
  if (StateDataPtr->FeeBlockInfo[UserBlockIndex].Address != FEE_ZERO)
  {
    BlockCycleCount = Fee_lGetBlkCycleCount(UserBlockIndex);
  }

  DestAddr = StateDataPtr->FeeNextFreePageAddr;
  Offset = (uint16) (DestAddr & (uint32)FEE_DFLASH_WORDLINE_SIZE_1);
  if (Offset == FEE_ZERO)
  {

    /* Check against unersable WL*/
    while (Fee_lIsUnerasableAddr(DestAddr))
    {
      DestAddr += FEE_DFLASH_WORDLINE_SIZE;
    }
    StateDataPtr->FeeNextFreePageAddr = DestAddr;
    StateDataPtr->FeeLastWrittenBlkInfo.BlockNumber = FEE_ZERO;
    StateDataPtr->FeeLastWrittenBlkInfo.Status.Consistent = FEE_ZERO;
  }

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  ReadWriteBuffPtr = StateDataPtr->FeeReadWriteBuffer + Offset;

  /* if block invalidation is ongoing */
  if(StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus == (unsigned_int)FEE_ONE)
  {
    Fee_lPrepareInvldReqBuff(BlockCycleCount, ReadWriteBuffPtr, UserBlockIndex);
  }
  else
  {
    Fee_lPrepareWriteReqBuff(BlockCycleCount, ReadWriteBuffPtr,
                        UserBlockIndex, BlockSize);
  }
  if(StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_COMPARE)
  {

    StateDataPtr->FeeLastWriteSize = ((Fls_LengthType)StateDataPtr->FeeReadLen);
  
    StateDataPtr->FeeUserJobStatus = FEE_JOB_REQSTD;
  
    /* Call Fls_17_Pmu_Write to write data in the DFLASH */
    RetVal = Fls_17_Pmu_Write((Fls_AddressType)DestAddr,
                              (ReadWriteBuffPtr),
                              (Fls_LengthType)(StateDataPtr->FeeLastWriteSize));
    StateDataPtr->FeeReadLen = FEE_ZERO;
    /* DET from FLS is not expected here */
    /* Hence, only check for Flash Sequence Error, Protection Error */
    /* In case of error, Fee_JobErrorNotification will be called */
    /* which would un-ncessarily update FeeNextFreePageAddr */
    /* Hence the below code reverts FeeNextFreePageAddr back to its */
    /* original value */
    if(E_NOT_OK == RetVal)
    {
      StateDataPtr->FeeNextFreePageAddr -= StateDataPtr->FeeLastWriteSize;
    }
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleIntrRead(void)                   **
**                                                                            **
** Service ID:                                                                **
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
** Description : This routine handles the functionality of                    **
** FEE_PEND_INTR_READ state of Write state machine                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleIntrRead(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint8              *ReadWriteBuffPtr;
  uint8              *LWBBufferPtr;
  uint32              SrcAddr;
  uint32              AffectedWLAddr;
  uint32              SrcAddrNextWLAddr;
  uint16              Offset;
  uint16              ReadLength;
  uint16              ReadLengthInWL;
  uint16              TempIndex;
  uint16              RemainingPageCount;
  uint16              LWBPageCount;
  uint16              ReadLengthMax;
  Std_ReturnType      ReturnValueFls;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  AffectedWLAddr = Fee_lGetWLBeginning(StateDataPtr->FeeWriteAffectedAddr);
  SrcAddr = StateDataPtr->FeeCurrSectSrcAddr;
  Offset = (uint16)((StateDataPtr->FeeNextFreePageAddr) & 
                    (uint32)FEE_DFLASH_WORDLINE_SIZE_1);
  if (Offset == FEE_ZERO)
  {
    Fee_lClearFeeReadWriteBuffer();
  }
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  ReadWriteBuffPtr = StateDataPtr->FeeReadWriteBuffer + Offset;
  ReadLengthMax = (uint16)FEE_DFLASH_WORDLINE_SIZE - Offset;

  /* if block(s) across the last written word line is affected */
  if(StateDataPtr->FeeWLAffectedType == FEE_LWB_ACROSS_WL_AFFECTED)
  {
    /* if current address is within the last written word line */
    if(SrcAddr >= AffectedWLAddr)
    {
      Offset = (uint16)(SrcAddr & (uint32)FEE_DFLASH_WORDLINE_SIZE_1);
      LWBBufferPtr = &(StateDataPtr->FeeLastWrittenBlkBuffer[0]);
      LWBPageCount = (uint16)(StateDataPtr->FeeLastWrittenBlkInfo.PageCount);
      RemainingPageCount = LWBPageCount - (uint16)StateDataPtr->FeeBlkPageCount;
      if(RemainingPageCount == FEE_ONE)
      {
        /* After marker write, change affected type to within word line */
        ReadLength = FEE_DFLASH_PAGE_SIZE;
        StateDataPtr->FeeWLAffectedType = FEE_LWB_WITHIN_WL_AFFECTED;
      }
      else
      {
        ReadLength = ((RemainingPageCount - FEE_ONE) << FEE_THREE);
        if(ReadLengthMax < ReadLength)
        {
          ReadLength = ReadLengthMax;
        }
      }
      for(TempIndex = FEE_ZERO; TempIndex < ReadLength; TempIndex++)
      {
        /* copy from last written block buffer to read-write buffer */
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeLastWrittenBlkBuffer[]*/
        ReadWriteBuffPtr[TempIndex] = LWBBufferPtr[TempIndex + Offset];
      }
      StateDataPtr->FeeCurrSectSrcAddr += ReadLength;
      StateDataPtr->FeePendReqStatus = FEE_PEND_INTR_WRITE;
    }
    else
    {
      /* if current address is before the last written word line */
      ReadLength = (uint16)FEE_DFLASH_WORDLINE_SIZE - Offset;
      SrcAddrNextWLAddr = Fee_lGetNextWLAddr(SrcAddr);
      ReadLengthInWL = (uint16)(SrcAddrNextWLAddr - SrcAddr);
      if(ReadLength > ReadLengthInWL)
      {
        if (((boolean)FALSE != Fee_lIsUnerasableAddr(SrcAddrNextWLAddr)) ||
            (SrcAddrNextWLAddr >= AffectedWLAddr))
        {
          ReadLength = ReadLengthInWL;
        }
      }

      /* copy from flash to read-write buffer */
      ReturnValueFls = Fls_17_Pmu_ReadWordsSync(
                       (Fls_AddressType)SrcAddr,
                       (uint32 *)(void *)ReadWriteBuffPtr,
                       (Fls_LengthType)ReadLength/FEE_DFLASH_WORD_SIZE);

      SrcAddr += ReadLength;

      while (Fee_lIsUnerasableAddr(SrcAddr))
      {
        SrcAddr = Fee_lGetNextWLAddr(SrcAddr);
      }
      StateDataPtr->FeeCurrSectSrcAddr = SrcAddr;

      if(ReturnValueFls == E_OK)
      {
        /* proceed to internal write */
        StateDataPtr->FeePendReqStatus = FEE_PEND_INTR_WRITE;
      }
      else
      {
        /* handle error */
        Fee_lHandleWriteFail();
      }
    }
  }
  else
  {
    /* if block(s) within the last written word line is affected */
    
    Offset = (uint16)(SrcAddr & (uint32)FEE_DFLASH_WORDLINE_SIZE_1);
    LWBBufferPtr = &(StateDataPtr->FeeLastWrittenBlkBuffer[0]);

    /* If last written block's marker copy pending */
    if(StateDataPtr->FeeWLAffectedType == FEE_LWB_MARKER_COPY_PENDING)
    {
      ReadLength = FEE_DFLASH_PAGE_SIZE;
      StateDataPtr->FeeWLAffectedType = FEE_LWB_WITHIN_WL_AFFECTED;
    }
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
      access the data within FeeLastWrittenBlkBuffer[]*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
      access the data within FeeLastWrittenBlkBuffer[]*/
    else if(StateDataPtr->FeeWLAffectedType == FEE_LWB_WITHIN_WL_AFFECTED)
    {
     /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
      access the data within FeeLastWrittenBlkBuffer[]*/
     /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
      access the data within FeeLastWrittenBlkBuffer[]*/
      StateDataPtr->FeeIntrCurrReqPageCount = (uint16)\
       ((uint16)(LWBBufferPtr[Offset+FEE_SEVEN]) << FEE_EIGHT) |\
                                                (LWBBufferPtr[Offset+FEE_SIX]);
      (StateDataPtr->FeeIntrCurrReqPageCount) &= ((uint16)FEE_PAGE_COUNT_MASK);
      ReadLength = (StateDataPtr->FeeIntrCurrReqPageCount) << FEE_THREE;
      if(ReadLength > ReadLengthMax)
      {
        ReadLength = ReadLengthMax;
        StateDataPtr->FeeWLAffectedType = FEE_LWB_WITHIN_WL_AFFECTED_CONT;
      }
      else
      {
        StateDataPtr->FeeWLAffectedType = FEE_LWB_MARKER_COPY_PENDING;
      }
    }
    else /* FeeWLAffectedType == FEE_LWB_WITHIN_WL_AFFECTED_CONT */
    {
      ReadLength = (uint16)((StateDataPtr->FeeIntrCurrReqPageCount) -
                      (StateDataPtr->FeeBlkPageCount));
      ReadLength = (ReadLength << FEE_THREE);

      StateDataPtr->FeeWLAffectedType = FEE_LWB_MARKER_COPY_PENDING;
    }

    /* copy from last written block buffer to read-write buffer */
    for(TempIndex = FEE_ZERO; TempIndex < ReadLength; TempIndex++)
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
        access the data within FeeLastWrittenBlkBuffer[]*/
      ReadWriteBuffPtr[TempIndex] = LWBBufferPtr[TempIndex + Offset];
    }
    StateDataPtr->FeeCurrSectSrcAddr = SrcAddr + ReadLength;
    StateDataPtr->FeePendReqStatus = FEE_PEND_INTR_WRITE;
  }

  StateDataPtr->FeeLastReadSize = ReadLength;
}

/*******************************************************************************
** Syntax : static void Fee_lHandleIntrWrite(void)                            **
**                                                                            **
** Service ID:                                                                **
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
** Description : This routine handles the functionality of                    **
** FEE_PEND_INTR_WRITE state of Write state machine                           **
*******************************************************************************/
static void Fee_lHandleIntrWrite(void)
{
  Fee_StateDataType     *StateDataPtr;
  uint8                 *ReadWriteBuffPtr;
  uint32                 DestAddr;
  uint16                 Offset;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  StateDataPtr->FeeLastWriteSize = StateDataPtr->FeeLastReadSize;
  DestAddr = StateDataPtr->FeeNextFreePageAddr;
  Offset = (uint16) (DestAddr & (uint32)FEE_DFLASH_WORDLINE_SIZE_1);
  
  /* Skip un-erasable word line, if any */
  if (Offset == FEE_ZERO)
  {
    while (Fee_lIsUnerasableAddr(DestAddr))
    {
      DestAddr = Fee_lGetNextWLAddr(DestAddr);
    }
    StateDataPtr->FeeNextFreePageAddr = DestAddr;
  }
  if(StateDataPtr->FeeBlkPageCount == FEE_ZERO)
  {
    StateDataPtr->FeeBlockStartAddr = DestAddr;
  }

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  ReadWriteBuffPtr = StateDataPtr->FeeReadWriteBuffer + Offset;
  StateDataPtr->FeeBlkPageCount +=
                   (Fee_PageType)((StateDataPtr->FeeLastWriteSize)>>FEE_THREE);
  StateDataPtr->FeeUserJobStatus = FEE_JOB_REQSTD;
  StateDataPtr->FeePendReqStatus = FEE_PEND_INTR_COMPARE;

  /* initiate FLS write */
  (void) Fls_17_Pmu_Write((Fls_AddressType)DestAddr,
                          ReadWriteBuffPtr,
                          (Fls_LengthType)(StateDataPtr->FeeLastWriteSize));
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleIntrWriteCompare                 **
**                                                   (uint16 UserBlockIndex)  **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  UserBlockIndex: Index of the requested block            **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine handles the functionality of                    **
** FEE_PEND_INTR_COMPARE state of Write state machine                         **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleIntrWriteCompare(uint16 UserBlockIndex)
{
  Fee_StateDataType     *StateDataPtr;
  uint8                 *PendReqStatePtr;
  const Fee_BlockType   *BlockPtr;
  Fee_CacheType         *UnconfigBlockPtr;
  uint32                DestAddr;
  uint32                TempAddr;
  uint16                BlockNumber;
  uint16                CompareLength;
  uint16                TmpCompareLength;
  uint16                StartOffset;
  uint16                UnconfigBlockIndex;
  Std_ReturnType        ReturnValueFls;
  boolean               RetVal;

  RetVal = (boolean)FALSE;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    Fee_BlockConfig[Number of configured blocks] to efficiently access its 
    elements*/
  BlockPtr = FEE_CONFIG_PTR->FeeBlockConfigPtr + UserBlockIndex;
  PendReqStatePtr = &(StateDataPtr->FeePendReqStatus);

  DestAddr = StateDataPtr->FeeNextFreePageAddr;
  TempAddr = Fee_lGetWLBeginning(DestAddr);
  CompareLength = (uint16)((DestAddr + (StateDataPtr->FeeLastWriteSize)) - 
                            TempAddr);
  TmpCompareLength = CompareLength;
  CompareLength -=  StateDataPtr->FeeComparedLen;
  if(CompareLength > (uint16)FEE_COMPARE_LEN_MAX)
  {
    CompareLength = (uint16)FEE_COMPARE_LEN_MAX;
  }                            
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    Fee_BlockConfig[Number of configured blocks] to efficiently access its 
    elements*/
  ReturnValueFls = Fls_17_Pmu_CompareWordsSync(
             (Fls_AddressType)(TempAddr+StateDataPtr->FeeComparedLen),
             (uint32 *)(void *)(StateDataPtr->FeeReadWriteBuffer +
             StateDataPtr->FeeComparedLen),
             ((Fls_LengthType)CompareLength/FEE_FOUR));
  
  StateDataPtr->FeeComparedLen += CompareLength;
  /* Compare failed */
  if(ReturnValueFls == E_NOT_OK)
  {
    StateDataPtr->FeeComparedLen = FEE_ZERO;
    Fee_lHandleWriteFail();
  }
  else if(StateDataPtr->FeeComparedLen != TmpCompareLength)
  {
    /*dummy else if to skip else for inbetween loops */
  }
  /* Compare successful */
  else
  {
    StateDataPtr->FeeComparedLen = FEE_ZERO;
    /* Update cache table for previous block - both configured and unconfigured 
       blocks needs only address update, the status remains the same */
    StartOffset = (uint16)DestAddr & FEE_DFLASH_WORDLINE_SIZE_1;
    if((StateDataPtr->FeeLastWriteSize == FEE_EIGHT) &&
       ((StateDataPtr->FeeReadWriteBuffer[StartOffset]) == 
                                                       FEE_DATA_MARKER_PAGE_ID))
    {
      BlockNumber = StateDataPtr->FeeCurrReqBlockNum;
      UserBlockIndex = Fee_lGetUsrBlockIndex(BlockNumber);
      if(UserBlockIndex != FEE_LOG_BLOCK_NOT_FOUND)
      {
        StateDataPtr->FeeBlockInfo[UserBlockIndex].Address =
                         StateDataPtr->FeeBlockStartAddr;
      }
      else
      {
        UnconfigBlockIndex = FEE_ZERO;
        while((UnconfigBlockIndex < (StateDataPtr->FeeUnConfigBlockCount)) &&
              (RetVal == (boolean)FALSE))
        {
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
          UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) +
                             (FEE_CONFIG_PTR->FeeBlkCnt) +
                             UnconfigBlockIndex;
          if((UnconfigBlockPtr->BlockNumber) == BlockNumber)
          {
            UnconfigBlockPtr->Address = StateDataPtr->FeeBlockStartAddr;
            RetVal = (boolean)TRUE;
          }
          else
          {
            UnconfigBlockIndex++;
          }
        }
      }
    }

    StateDataPtr->FeeNextFreePageAddr += (StateDataPtr->FeeLastWriteSize);
    *PendReqStatePtr = FEE_PEND_INTR_READ;

    /* if affected type is within the last written word line */
    if(StateDataPtr->FeeWLAffectedType == FEE_LWB_WITHIN_WL_AFFECTED)
    {
      StateDataPtr->FeeBlkPageCount = FEE_ZERO;
      StartOffset = (uint16)StateDataPtr->FeeCurrSectSrcAddr & 
                                                  FEE_DFLASH_WORDLINE_SIZE_1;
      if(Fee_lIsLastWrittenWLAffected(StartOffset) == (boolean)FALSE)
      {
        /* Prev block is written, now write the curr user requested blk*/
        if(Fee_lIsWLBeginning(StateDataPtr->FeeNextFreePageAddr) != FEE_ZERO)
        {
          StateDataPtr->FeeNextFreePageAddr =
                      Fee_lGetNextWLAddr(StateDataPtr->FeeNextFreePageAddr);
        }
        while (Fee_lIsUnerasableAddr(StateDataPtr->FeeNextFreePageAddr))
        {
          StateDataPtr->FeeNextFreePageAddr =
                      Fee_lGetNextWLAddr(StateDataPtr->FeeNextFreePageAddr);
        }
        /* register write request */
        Fee_lRegisterWriteRequest(StateDataPtr->FeePendReqInfo.DataBufferPtr,
                                  BlockPtr);
        StateDataPtr->FeeBlockStartAddr = StateDataPtr->FeeNextFreePageAddr;
      }
    }
  }
}

/*******************************************************************************
** Syntax : static void Fee_lLocalWrite(void)                                 **
**                                                                            **
** Service ID:                                                                **
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
** Description : This routine will perform the write operation on DFLASH. It  **
** is called by Fee_MainFunction                                              **
*******************************************************************************/
static void Fee_lLocalWrite(void)
{
  Fee_StateDataType     *StateDataPtr;
  uint8                 *PendReqStatePtr;
  uint16                 BlockNumber;
  uint16                 UserBlockIndex;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  BlockNumber = StateDataPtr->FeePendReqInfo.BlockNumber;
  UserBlockIndex = Fee_lGetUsrBlockIndex(BlockNumber);

  PendReqStatePtr = &(StateDataPtr->FeePendReqStatus);

  /**********************************************************************/
  /*                       2.STATE MACHINE EXECUTION                    */
  /**********************************************************************/

  /*********** CASE 1: USER WRITE REQSTD STATE *******************/
  if ((boolean)TRUE == Fee_lIsUserWriteInvldReqstd(StateDataPtr))
  {
    Fee_lHandleUserWriteReq(UserBlockIndex);
  }


  /*********** CASE: USER WRITE COMPARE STATE *******************/
  if(*PendReqStatePtr == FEE_PEND_PREV_COMPARE)
  {
    Fee_lHandleUserWriteCompare(UserBlockIndex);
  }

  /*********** CASE: USER WRITE ONGOING STATE *******************/
  if(*PendReqStatePtr == FEE_PEND_PREV_WRITE_REQ)
  {
    Fee_lHandleUserWrite(UserBlockIndex);
  }


  /*********** CASE: INTERNAL WRITE COMPARE STATE *******************/
  if (*PendReqStatePtr == FEE_PEND_INTR_COMPARE)
  {
    Fee_lHandleIntrWriteCompare(UserBlockIndex);
  }

  /*********** CASE: INTERNAL WRITE BLOCK READ STATE *******************/
  if (*PendReqStatePtr == FEE_PEND_INTR_READ)
  {
    Fee_lHandleIntrRead();
  }


  /*********** CASE: INTERNAL WRITE ONGOING STATE *******************/
  if (*PendReqStatePtr == FEE_PEND_INTR_WRITE)
  {
    Fee_lHandleIntrWrite();
  }
}/* End of Fee_lLocalWrite */


/*******************************************************************************
** Syntax : static void Fee_lRegisterWriteRequest( uint8 *DataBufferPtr,      **
**                                         const Fee_BlockType * BlockPtr)    **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (inout) :  None                                                 **
**                                                                            **
** Parameters (out):  DataBufferPtr -  Pointer to Data buffer                 **
**                    BlockPtr - Pointer to block data structure              **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine will register the write request in the pending  **
** request buffer.                                                            **
*******************************************************************************/
static void Fee_lRegisterWriteRequest(uint8 *DataBufferPtr,
                                      const Fee_BlockType * BlockPtr)
{
  Fee_StateDataType     *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  StateDataPtr->FeeStatusFlags.FeeWriteInvldAPICalled = FEE_ONE;
  StateDataPtr->FeeStatusFlags.FeeStartInitGC = FEE_ONE; /*TRUE*/

  /* if the logical block is of immediate in nature */
  if ( ( BlockPtr->FeeImmediateData == (unsigned_int)FEE_IMMEDIATE_DATA ) &&
      /* This request is not for invalidation    */
     ( StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus 
                                              == (unsigned_int)FEE_ZERO )
    )
  {
    StateDataPtr->FeePendReqStatus = FEE_REQ_IMM_WRITE;
  }
  else
  {
    StateDataPtr->FeePendReqStatus = FEE_REQ_NORM_WRITE;
  }

  StateDataPtr->FeeJobResult = MEMIF_JOB_PENDING;

  /* Store the pending request parameters in the pending request buffer*/
  StateDataPtr->FeePendReqInfo.BlockNumber  = BlockPtr->BlockNumber;
  StateDataPtr->FeePendReqInfo.DataBufferPtr = DataBufferPtr;

  /* Update the current requested logical block number */
  StateDataPtr->FeeCurrReqBlockNum = BlockPtr->BlockNumber;
}/* End of Fee_lRegisterWriteRequest */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE021_FEE023_FEE073_FEE075]
**                                                                            **
** Syntax : static void Fee_lLocalRead(void)                                  **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine will perform the read operation on DFLASH. It   **
** is called by Fee_MainFunction                                              **
*******************************************************************************/
static void Fee_lLocalRead(void)
{
  Fee_PendReqBufType   *PendReqPtr;
  Fee_StateDataType    *StateDataPtr;
  uint8                *DataBufferPtr;
  uint32                ActualAddr;
  uint32                TempAddress;
  uint16                ReadLimit;
  uint16                SizeInCurrentWL;
  uint16                ReadLength;
  uint16                BlockNumber;
  uint16                BlockOffset;
  uint16                Length;
  uint16                UserBlockIndex;
  uint16                DataByteCount;
  Std_ReturnType        ReturnValue;
  uint8                 ReadLengthPages;
  uint8                 LoopIndex;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  PendReqPtr = &StateDataPtr->FeePendReqInfo;

  BlockNumber = PendReqPtr->BlockNumber;
  BlockOffset = PendReqPtr->BlockOffset;
  DataBufferPtr = PendReqPtr->DataBufferPtr;
  Length = PendReqPtr->Length;
  DataByteCount = FEE_ONE;

  ReturnValue = E_OK;
  UserBlockIndex = Fee_lGetUsrBlockIndex(BlockNumber);

  /**********************************************************************/
  /*                1.AYSNC STATE MACHINE TRANSITIONS                   */
  /**********************************************************************/
  /* These transitions are handled in Fee_JobEndNotification and
    Fee_JobErrorNotification as specified by AUTOSAR */


  /**********************************************************************/
  /*                       2.STATE MACHINE EXECUTION                    */
  /**********************************************************************/

  /*********** CASE 1: READ REQSTD STATE *******************/
  if((boolean)TRUE == Fee_lIsUserReadReqstd(StateDataPtr))
  {
    /*********************** PERFORM CHECKS START **********************/
    ReturnValue = Fee_lPerformReadCheck(UserBlockIndex);

    BlockOffset = Fee_lLocalReadStart(ReturnValue, UserBlockIndex);
  }

  /*********** CASE 2: READ REQSTD STATE *******************/

  StateDataPtr->FeeUserReadDestPtr = DataBufferPtr;

  /* PERFORM READ START */
  if (StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_READ_REQ)
  {
    if(BlockOffset > FEE_ZERO)
    {
      while(BlockOffset < FEE_SEVEN)
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
        StateDataPtr->FeeUserReadDestPtr[0] =
            StateDataPtr->FeeReadWriteBuffer[DataByteCount + BlockOffset];
        BlockOffset++;
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
          access user data buffer efficiently. User ensures the 
          availability of sufficient buffer data space for the requested
          operation*/
        StateDataPtr->FeeUserReadDestPtr++;
      }
      BlockOffset = FEE_ZERO;
      DataByteCount += FEE_EIGHT;
    }
    while(DataByteCount < StateDataPtr->FeeLastReadSize)
    {
      for(LoopIndex = FEE_ZERO; LoopIndex < FEE_SEVEN; LoopIndex++)
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
        StateDataPtr->FeeUserReadDestPtr[LoopIndex] =
            StateDataPtr->FeeReadWriteBuffer[DataByteCount + LoopIndex];
      }
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
        access user data buffer efficiently. User ensures the 
        availability of sufficient buffer data space for the requested
        operation*/
      StateDataPtr->FeeUserReadDestPtr += FEE_SEVEN;
      DataByteCount += FEE_EIGHT;
    }
  }

  if ( (ReturnValue == E_OK) ||
       (StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_READ_REQ)
     )
  {
    /* calculate actual address where data is needed to be read */
    ActualAddr = StateDataPtr->FeeCurrSectSrcAddr;

    if(Fee_lIsWLBeginning(ActualAddr) == FEE_ZERO)
    {
      ReadLimit = FEE_DATA_BYTES_IN_WORDLINE;
    }
    else
    {
      SizeInCurrentWL = (uint16)FEE_DFLASH_WORDLINE_SIZE - 
                             (uint16)(ActualAddr & FEE_DFLASH_WORDLINE_SIZE_1);
      ReadLimit = (SizeInCurrentWL >> FEE_THREE) * FEE_SEVEN;
    }

    if (Length > (ReadLimit - BlockOffset))
    {
      if(Fee_lIsWLBeginning(ActualAddr) != FEE_ZERO)
      {
        TempAddress = Fee_lGetNextWLAddr(ActualAddr);
        if((boolean)FALSE == Fee_lIsUnerasableAddr(TempAddress))
        {
          ReadLimit = FEE_DATA_BYTES_IN_WORDLINE;
        }
      }
    }

    ReadLimit = ReadLimit - BlockOffset;
    if (Length > ReadLimit)
    {
      ReadLength = ReadLimit;
      Length = Length - ReadLimit;
      StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_READ_REQ;
    }
    else
    {
      ReadLength = Length;
      StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_READ_NOTIF;
    }

    /* Call Fls_Read to read the data from DFLASH */
    StateDataPtr->FeeUserJobStatus = FEE_JOB_REQSTD;

    if((ReadLength + BlockOffset) > FEE_SEVEN)
    {
      ReadLength = ReadLength - (FEE_SEVEN - BlockOffset);
      ReadLengthPages = (uint8)Fee_lGetDFLASHPages(ReadLength) - FEE_ONE;
    }
    else
    {
      ReadLengthPages = (uint8)Fee_lGetDFLASHPages(ReadLength) - FEE_TWO;
    }
    ReadLength = (uint16)ReadLengthPages << FEE_THREE;

    Fee_lClearFeeReadWriteBuffer();

    StateDataPtr->FeeLastReadSize = (Fls_LengthType)ReadLength;

    (void) Fls_17_Pmu_Read((Fls_AddressType)ActualAddr,
                           StateDataPtr->FeeReadWriteBuffer,
                           (Fls_LengthType)ReadLength);
    /* Return value above is ignored */

    StateDataPtr->FeeCurrReqBlockNum = BlockNumber;

    /* If the current request is set to previous pending */
    if(StateDataPtr->FeePendReqStatus == FEE_PEND_PREV_READ_REQ)
    {
      /* Increment FeeCurrSectSrcAddr to point to next page */
      ActualAddr += ReadLength;
      while (Fee_lIsUnerasableAddr(ActualAddr))
      {
        ActualAddr = Fee_lGetNextWLAddr(ActualAddr);
      }
      StateDataPtr->FeeCurrSectSrcAddr = ActualAddr;
    }

    /* Store the read request parameters in the Pending buffer structure */
    PendReqPtr->BlockOffset = BlockOffset;
    PendReqPtr->Length = Length;
    PendReqPtr->DataBufferPtr = StateDataPtr->FeeUserReadDestPtr;
    /* PERFORM READ ENDS */
  }
}/* End of Fee_lLocalRead */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint16 Fee_lLocalReadStart                       **
**                                           (Std_ReturnType ReturnValue,     **
**                                                    uint16 UserBlockIndex)  **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  ReturnValue - Return value of Fee_lPerformReadCheck     **
**                    UserBlockIndex - User block index                       **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint16 - Block Offset                                   **
**                                                                            **
** Description : This routine will perform the initial operations of          **
**               Fee_lLocalRead()                                             **
*******************************************************************************/
IFX_LOCAL_INLINE uint16 Fee_lLocalReadStart(Std_ReturnType ReturnValue,
                                           uint16 UserBlockIndex)
{
  Fee_PendReqBufType   *PendReqPtr;
  Fee_StateDataType    *StateDataPtr;
  uint32                StartAddr;
  uint16                BlockOffset;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  PendReqPtr = &StateDataPtr->FeePendReqInfo;
  BlockOffset = PendReqPtr->BlockOffset;

  if(ReturnValue == E_NOT_OK )
  {
    StateDataPtr->FeePendReqStatus = FEE_PEND_PREV_NO_REQ;

    if ( FEE_CONFIG_PTR->FeeNvmJobErrorNotification != NULL_PTR )
    {
       (FEE_CONFIG_PTR->FeeNvmJobErrorNotification)();
    }
  }
  /* ReturnValue is E_OK here */
  else
  {
    /* Load the start address of the block */
    #if (FEE_GET_PREV_DATA_API == STD_ON)
    if(PendReqPtr->GetPrevCopy == FEE_ONE)
    {
      StartAddr = (StateDataPtr->FeeBlockInfo[UserBlockIndex].PrevCopyAddress)
                  + FEE_DFLASH_PAGE_SIZE;
    }
    else
    #endif
    {
      StartAddr = (StateDataPtr->FeeBlockInfo[UserBlockIndex].Address)
                  + FEE_DFLASH_PAGE_SIZE;
    }

    do
    {
      if(Fee_lIsWLBeginning(StartAddr) == FEE_ZERO)
      {
        while (Fee_lIsUnerasableAddr(StartAddr))
        {
          StartAddr = Fee_lGetNextWLAddr(StartAddr);
        }
      }
      if (BlockOffset >= FEE_SEVEN)
      {
        StartAddr += FEE_DFLASH_PAGE_SIZE;
        BlockOffset -= FEE_SEVEN;
      }
    }while(BlockOffset >= FEE_SEVEN);

    StateDataPtr->FeeCurrSectSrcAddr = StartAddr;
  }
  return (BlockOffset);
}

/*******************************************************************************
** Syntax : static void Fee_lGCErrorHandler(uint8 Error)                      **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  uint8                                                   **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine is responsible for handling GC transitions      **
**               incase of errors                                             **
*******************************************************************************/
static void Fee_lGCErrorHandler(uint8 Error)
{
  Fee_StateDataType  *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  switch(Error)
  {
    /* Write Failure */
    case FEE_GC_WRITE:
    {
      /* Report Dem */
      #if (FEE_GC_WRITE_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
      Dem_ReportErrorStatus(FEE_E_GC_WRITE, DEM_EVENT_STATUS_FAILED);
      #endif

      break;
    }

    /* Read Failure */
    case FEE_GC_READ:
    {
      /* Report Dem */
      #if (FEE_GC_READ_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
      Dem_ReportErrorStatus(FEE_E_GC_READ, DEM_EVENT_STATUS_FAILED);
      #endif

      break;
    }

    /* Erase Failure */
    case FEE_GC_ERASE:
    {
      /* Report Dem */
      #if (FEE_GC_ERASE_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
      Dem_ReportErrorStatus(FEE_E_GC_ERASE, DEM_EVENT_STATUS_FAILED);
      #endif

      break;
    }

    /* GC Trig failure - a case where the Gc triggers another Gc */
    default:
    {
      /* Report Dem */
      #if (FEE_GC_TRIG_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
      Dem_ReportErrorStatus(FEE_E_GC_TRIG, DEM_EVENT_STATUS_FAILED);
      #endif

      break;
    }
  }

  if((StateDataPtr->FeeGcState == FEE_GC_UNCFG_START_COPY) ||
     (StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_WRITE) ||
     (StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_READ_NXTPAGE))
  {
    StateDataPtr->FeeGcState = FEE_GC_UNCFG_START_COPY;
    Fee_lGcDiscardUnconfigBlk();
  }
  else
  {
    Fee_lHandleGcFail();
  }
}/*End of Fee_lGCErrorHandler */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleGcFail(void)                     **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This function shall handle the severe error case or          **
** internal job fail case during Garbage collection                           **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleGcFail(void)
{
  Fee_StateDataType     *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Enter GC Fail state */
  StateDataPtr->FeeGcState = FEE_GC_FAIL;

  /* Raise Illegal state notification if configured */
  if(FEE_CONFIG_PTR->FeeIllegalStateNotification != NULL_PTR)
  {
    /* Call the Notification function */
    FEE_CONFIG_PTR->FeeIllegalStateNotification();
  }
}

/*******************************************************************************
** Syntax : static void Fee_lGcRead(Fls_AddressType Position, uint16 Offset)  **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  Position: Dflash Addr to read from                      **
**                    Offset: Offset of read write buffer (page0 / page1)     **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : Gives a reads request during Gc                              **
*******************************************************************************/
static void Fee_lGcRead(Fls_AddressType Position, uint16 Offset)
{
  Fee_StateDataType    *StateDataPtr;
  uint8                *GcReadWriteBufferPtr;
  Fee_PageType          PendingPageCount;
  Fls_LengthType        ReadLength;
  uint16                PendingBlockSize;
  uint16                GcSrcAddrOffset;

  ReadLength = FEE_DFLASH_PAGE_SIZE;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  PendingPageCount = StateDataPtr->FeeGcCurrBlkInfo.PageCount -
              StateDataPtr->FeeBlkPageCount;
  PendingBlockSize = (uint16)PendingPageCount << FEE_THREE;

  if(PendingPageCount > FEE_ZERO)
  {
    if(PendingBlockSize > (FEE_DFLASH_WORDLINE_SIZE - Offset))
    {
      ReadLength = (Fls_LengthType)FEE_DFLASH_WORDLINE_SIZE - Offset;
    }
    else
    {
      ReadLength = (Fls_LengthType)PendingBlockSize;
    }
    GcSrcAddrOffset = (uint16)(Position & FEE_DFLASH_WORDLINE_SIZE_1);
    if(ReadLength >((Fls_LengthType)FEE_DFLASH_WORDLINE_SIZE - GcSrcAddrOffset))
    {
      if(Fee_lIsUnerasableAddr(Position + FEE_DFLASH_WORDLINE_SIZE))
      {
        ReadLength = (Fls_LengthType)FEE_DFLASH_WORDLINE_SIZE - GcSrcAddrOffset;
      }
    }
  }

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  GcReadWriteBufferPtr = StateDataPtr->FeeGcReadWriteBuffer + Offset;

  /* initiate FLS read */
  (void) Fls_17_Pmu_Read(Position, GcReadWriteBufferPtr, ReadLength);

  StateDataPtr->FeeLastReadSize = ReadLength;
  StateDataPtr->FeeIntrJobStatus = FEE_JOB_REQSTD;
  StateDataPtr->FeeIntrJob = FEE_READ_JOB;
}

/*******************************************************************************
** Syntax : static uint8 Fee_lGcWrite(Fls_AddressType Position, uint16 Offset)**
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  Position: Dflash Addr to write to                       **
**                    Offset: Offset of read write buffer (page0 / page1)     **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint8                                                   **
**                                                                            **
** Description : Gives a write request and sets the error flag                **
**               in case of an error.                                         **
*******************************************************************************/
static uint8 Fee_lGcWrite(Fls_AddressType Position, uint16 Offset)
{
  Std_ReturnType      ReturnValueFls;
  Fee_StateDataType  *StateDataPtr;
  uint8              *GcReadWriteBufferPtr;
  uint8               Error;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  Error = FEE_ZERO;

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  GcReadWriteBufferPtr = StateDataPtr->FeeGcReadWriteBuffer + Offset;
  StateDataPtr->FeeIntrJobStatus = FEE_JOB_REQSTD;
  StateDataPtr->FeeIntrJob = FEE_WRITE_JOB;
  StateDataPtr->FeeLastWriteSize = StateDataPtr->FeeLastReadSize;

  ReturnValueFls = Fls_17_Pmu_Write(Position,
                                    GcReadWriteBufferPtr,
                                    StateDataPtr->FeeLastWriteSize);
  /* if the Fls routine did not accept the request         */
  /* then report Dem error and Set GC state to FEE_GC_IDLE */
  if(ReturnValueFls == E_NOT_OK)
  {
    /* Any failure from FLS (both DET,DEM) will trigger Illegal notification
       and FEE DEM. Note Fee_JobErrorNotification will NOT raise any
       illegal notification / DEM incase of internal job failures */
    Error = FEE_GC_WRITE;
  }

  return (Error);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lGcRequested(void)                      **
**                                                                            **
** Service ID:  NA                                                            **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Dem_EvenIdType                                          **
**                                                                            **
** Description : This routine is responsible for handling Garbage Collection  **
**                GC REQUESTED state                                          **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lGcRequested(void)
{
  Fee_CacheType         *CachePtr;
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *UnconfigBlockPtr;
  uint16                 BlockCount;
  uint8                  GcState;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
   /* Initialize the copied status for all the logical blocks */
  CachePtr = StateDataPtr->FeeBlockInfo;
  BlockCount = FEE_ZERO;

  do
  {
    CachePtr->Status.Copied = FEE_ZERO;
    CachePtr->Status.PrevCopyCopied = FEE_ZERO;
    BlockCount++;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    CachePtr++;
  }while(BlockCount != FEE_CONFIG_PTR->FeeBlkCnt);

  /* Update the src and dest pointers based on the current sector */
  if(StateDataPtr->FeeStatusFlags.FeeCurrSector == (unsigned_int)FEE_ZERO)
  {
    StateDataPtr->FeeGcDestAddr = StateDataPtr->FeeSectorInfo[1].NextFreeWLAddr;
    StateDataPtr->FeeGcSrcAddr = FEE_DFLASH_BANK0_BASE;
  }
  else
  {
    StateDataPtr->FeeGcDestAddr = StateDataPtr->FeeSectorInfo[0].NextFreeWLAddr;
    StateDataPtr->FeeGcSrcAddr = FEE_DFLASH_BANK1_BASE;
  }
  StateDataPtr->FeeBlkPageCount = FEE_ZERO;
  StateDataPtr->FeeStatusFlags.FeeBlkModified = FEE_ZERO;
  StateDataPtr->FeeGCCopyIndex = FEE_ZERO;
  /* Functionality to take care of unconfigured blocks starts ******/

  GcState = FEE_GC_START_COPY;

  StateDataPtr->FeeGcBlkIndexInWL = FEE_ZERO;

  /* Clear the copied status of un-configured blocks */
  if ((FEE_CONFIG_PTR->FeeGCConfigSetting.FeeUnconfigBlock ==
                                                       FEE_UNCONFIG_BLOCK_KEEP)
     && (StateDataPtr->FeeUnConfigBlockCount != FEE_ZERO))
  {
    BlockCount = FEE_ZERO;

    while(BlockCount < StateDataPtr->FeeUnConfigBlockCount)
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
      UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) +
                         (FEE_CONFIG_PTR->FeeBlkCnt) +
                         BlockCount;
      UnconfigBlockPtr->Status.Copied = FEE_ZERO;
      BlockCount++;
    }

    StateDataPtr->FeeGCUnconfigBlkCopyIndex = FEE_ZERO;
    GcState = FEE_GC_UNCFG_START_COPY;
  }

  StateDataPtr->FeeGcState = GcState;
}

/*******************************************************************************
** Syntax : static void Fee_lGcPrologue(void)                                 **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine is responsible for handling transitions from    **
**         FEE_GC_UNCONFIG_BLOCK_READ and  FEE_GC_COPY_READ_NXTPAGE states.   **
** The transitions from the above states are handled as Prologue and not as   **
** part of the state itself. This is to avoid one main cycle delay in         **
** in executing the next pass state (FEE_GC_UNCONFIG_BLOCK_WRITE or           **
** FEE_GC_START_COPY_WRITE)                                                   **
*******************************************************************************/
static void Fee_lGcPrologue(void)
{
  Fee_StateDataType     *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  if (StateDataPtr->FeeIntrJobStatus == FEE_JOB_DONE)
  {
    if (StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_READ_NXTPAGE)
    {
      /* Reset the job status */
      StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
      if(StateDataPtr->FeeIntrJobResult == FEE_JOB_FAIL)
      {
        /* Raise GC Read DEM */
        #if (FEE_GC_READ_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
        Dem_ReportErrorStatus(FEE_E_GC_READ, DEM_EVENT_STATUS_FAILED);
        #endif

        /* When unconfigblk read fails, Fee_lGCErrorHandler need not be called.
        Ignore the entire UnconfigBlk so that it is not copied to new sector */
        Fee_lGcDiscardUnconfigBlk();
        /* move to Unconfig block start to check for next unconfig block */
        StateDataPtr->FeeGcState = FEE_GC_UNCFG_START_COPY;
      }
      else
      {
        StateDataPtr->FeeGcState = FEE_GC_UNCFG_COPY_WRITE;
      }
    }

    else if (StateDataPtr->FeeGcState == FEE_GC_COPY_READ_NXTPAGE)
    {
      StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
      if(StateDataPtr->FeeIntrJobResult == FEE_JOB_FAIL)
      {
         Fee_lGCErrorHandler(FEE_GC_READ);
      }
      else
      {
        StateDataPtr->FeeGcState = FEE_GC_COPY_WRITE;
      }
    }
    else
    {
      /* dummy else */
    }
  }
}

/*******************************************************************************
** Syntax : static void Fee_lGcDiscardUnconfigBlk(void)                       **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This function discards the un-configured block               **
**                                                                            **
*******************************************************************************/
static void Fee_lGcDiscardUnconfigBlk(void)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *UnconfigBlockPtr;
  Fee_CacheType         *NextUnconfigBlockPtr;
  uint16                 BlockIndex;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
  UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) + 
                     (FEE_CONFIG_PTR->FeeBlkCnt);
  BlockIndex = FEE_ZERO;

  /* Move all the un-configured block infos in cache table by one index */
  while((BlockIndex < (StateDataPtr->FeeUnConfigBlockCount)) &&
        (StateDataPtr->FeeGcCurrBlkInfo.BlockNumber != 
                                                 UnconfigBlockPtr->BlockNumber))
  {
    BlockIndex++;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    UnconfigBlockPtr++;
  }
  /* Decrement the un-configured block count */
  (StateDataPtr->FeeUnConfigBlockCount)--;
  while(BlockIndex < (StateDataPtr->FeeUnConfigBlockCount))
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    NextUnconfigBlockPtr = UnconfigBlockPtr + FEE_ONE;
    UnconfigBlockPtr->BlockNumber = NextUnconfigBlockPtr->BlockNumber;
    UnconfigBlockPtr->Address = NextUnconfigBlockPtr->Address;
    BlockIndex++;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    UnconfigBlockPtr++;
  }
}

/*******************************************************************************
** Syntax : static boolean Fee_lGcCheckBlkCopy(void)                          **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  boolean RetVal: 1 if there is a block to be copied      **
**                                  0 if no block is to be copied             **
**                                                                            **
** Description : This routine checks the cache table starting from the        **
**  FeeGCCopyIndex and finds if any block needs to be copied                  **
*******************************************************************************/
static boolean Fee_lGcCheckBlkCopy(void)
{
  Fee_StateDataType    *StateDataPtr;
  Fee_CacheType        *CachePtr;
  uint16                BlockCount;
  uint8                 RetVal;

  RetVal = FEE_ZERO;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  BlockCount = StateDataPtr->FeeGCCopyIndex;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
  CachePtr = StateDataPtr->FeeBlockInfo + BlockCount;

  while ((RetVal == FEE_ZERO) && (BlockCount != FEE_CONFIG_PTR->FeeBlkCnt))
  {
    /* If block is consistent and not copied */
    if ((CachePtr->Status.Consistent == (unsigned_int)FEE_ONE) &&
        (CachePtr->Status.Copied == (unsigned_int)FEE_ZERO))
    {
      RetVal = FEE_ONE;
    }
    else
    {
      BlockCount++;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
      CachePtr++;
    }
  }
  StateDataPtr->FeeGCCopyIndex = BlockCount;

  return (RetVal);
}

/*******************************************************************************
** Syntax : static void Fee_lGcStartCopy(void)                                **
**                                                                            **
** Service ID:  NA                                                            **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine is responsible for handling Garbage Collection  **
**                GC START COPY state                                         **
*******************************************************************************/
static void Fee_lGcStartCopy(void)
{
  Fee_CacheType         *CachePtr;
  Fee_CacheType         *UnconfigBlockPtr;
  Fee_StateDataType     *StateDataPtr;
  uint32                 DestAddr;
  uint16                 Offset;
  uint16                 Index;
  boolean                StateFlag;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  if (StateDataPtr->FeeIntrJobStatus == FEE_JOB_DONE)
  {
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    if(StateDataPtr->FeeIntrJobResult == FEE_JOB_FAIL)
    {
      Fee_lGCErrorHandler(FEE_GC_READ);
    }
    else
    {
      if(StateDataPtr->FeeGcState == FEE_GC_START_COPY)
      {
        StateDataPtr->FeeGcState = FEE_GC_COPY_WRITE;
      }
      else
      {
        StateDataPtr->FeeGcState = FEE_GC_UNCFG_COPY_WRITE;
      }
    }
  }
  else
  {
    /* i.e. StateDataPtr->FeeGcState == FEE_GC_START_COPY */
    DestAddr = (uint32)StateDataPtr->FeeGcDestAddr;

    /* No more blocks to be copied in the current pass -
       check if any block is modified during GC */
    if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_START_COPY)
    {
      StateFlag = Fee_lGcCheckUnconfigBlkCopy();
    }
    else /* if(StateDataPtr->FeeGcState == FEE_GC_START_COPY) */
    {
      /* Check if there are any more blocks to be copied */
      StateFlag = Fee_lGcCheckBlkCopy();
      if (StateFlag == (boolean)FALSE)
      {
        /* if any of the logical blocks were modified */
        if(StateDataPtr->FeeStatusFlags.FeeBlkModified == (unsigned_int)FEE_ONE)
        {
          StateDataPtr->FeeStatusFlags.FeeBlkModified = FEE_ZERO;
          StateDataPtr->FeeGCCopyIndex = FEE_ZERO;
          StateFlag = Fee_lGcCheckBlkCopy();
        }
      }
    }

    /* Block copying not yet completed */
    if(StateFlag == (boolean)TRUE)
    {
      if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_START_COPY)
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
        UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) +
                           (FEE_CONFIG_PTR->FeeBlkCnt) +
                           (StateDataPtr->FeeGCUnconfigBlkCopyIndex);
        StateDataPtr->FeeGcSrcAddr = UnconfigBlockPtr->Address;
      }
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
        access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
      else /* FeeGcState == FEE_GC_START_COPY */
      {
        CachePtr = StateDataPtr->FeeBlockInfo + StateDataPtr->FeeGCCopyIndex;
        StateDataPtr->FeeGcSrcAddr = CachePtr->Address;
      }
      StateDataPtr->FeeBlkPageCount = FEE_ZERO;
      Offset = (uint16) (DestAddr & (uint32)FEE_DFLASH_WORDLINE_SIZE_1);
      if (Offset == FEE_ZERO)
      {
        Fee_lClearFeeGcReadWriteBuffer();
      }
      /* Fill Curr Block information */
      /* Read the header (first 8bytes) of the block to update the
        Gc Curr Blk information */
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      (void) Fls_17_Pmu_ReadWordsSync(
                 (Fls_AddressType)StateDataPtr->FeeGcSrcAddr,
               (uint32 *)(void *)(StateDataPtr->FeeGcReadWriteBuffer + Offset),
                 (Fls_LengthType)FEE_TWO);
      Fee_lUpdateGcCurrBlockInfo();
      Fee_lGcRead((Fls_AddressType)(StateDataPtr->FeeGcSrcAddr), Offset);
    }
    /* Finished copying all the configured OR unconfigured blocks */
    else
    {
      if (Fee_lIsWLBeginning(DestAddr) != FEE_ZERO)
      {
        DestAddr = Fee_lGetNextWLAddr(DestAddr);
      }
      Fee_lClearFeeGcReadWriteBuffer();
      StateDataPtr->FeeGcDestAddr = DestAddr;

      /* Clear the compare fail count*/
      StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;

      if(StateDataPtr->FeeGcState == FEE_GC_START_COPY)
      {
        /* Finished copying all the configured blocks */
        /* Mark the prev copy of blocks as inconsistent for the blocks which
           are not updated during GC - this means that only the latest data
           is copied during GC and there is no prev copy in the new sector */
        Index = FEE_ZERO;
        do
        {
          CachePtr = &(StateDataPtr->FeeBlockInfo[Index]);
          if (CachePtr->Status.PrevCopyCopied == (unsigned_int)FEE_ZERO)
          {
            #if (FEE_GET_PREV_DATA_API == STD_ON)
            CachePtr->PrevCopyAddress = FEE_ZERO;
            #endif
            CachePtr->Status.PrevCopyValid = FEE_ZERO;
            CachePtr->Status.PrevCopyConsistent = FEE_ZERO;
            CachePtr->Status.PrevCopyCopied = FEE_ZERO;
          }
          Index++;
        }while(Index < FEE_CONFIG_PTR->FeeBlkCnt);

        /* Move to next state */
        StateDataPtr->FeeGcState = FEE_GC_MARK_SECTOR1_V_SP;
      }
      else
      {
        /* Finished copying all the unconfigured blocks */
        /* Now copy all the configured blocks */
        StateDataPtr->FeeGcState = FEE_GC_START_COPY;
      }
    }
  }
}

/*******************************************************************************
** Syntax : static uint8 Fee_lGcStartCopyWrite(void)                          **
**                                                                            **
** Service ID:  NA                                                            **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint8                                                   **
**                                                                            **
** Description : This routine is responsible for handling Garbage Collection  **
**                GC START COPY WRITE state                                   **
*******************************************************************************/
static uint8 Fee_lGcStartCopyWrite(void)
{
  Fee_StateDataType    *StateDataPtr;
  Fee_GcBlkInfoType    *GcCurrBlkPtr;
  uint32                DestAddr;
  uint32                PageCount;
  uint32                FreePages;
  uint16                Offset;
  uint8                 BankNumber;
  uint8                 Error;

  Error = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  GcCurrBlkPtr = &(StateDataPtr->FeeGcCurrBlkInfo);

  if (StateDataPtr->FeeIntrJobStatus == FEE_JOB_DONE)
  {
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    if(StateDataPtr->FeeIntrJobResult == FEE_JOB_FAIL)
    {
      Fee_lGCErrorHandler(FEE_GC_WRITE);
    }
    else
    {
      Fee_lGcBlkCompare();
    }
  }
  else if(StateDataPtr->FeeComparedLen != (uint16)0)
  {
    Fee_lGcBlkCompare();
  }
  else
  {
    /* i.e. StateDataPtr->FeeGcState == FEE_GC_COPY_WRITE */
    DestAddr = (uint32)StateDataPtr->FeeGcDestAddr;
    if(Fee_lIsUnerasableAddr(DestAddr))
    {
      DestAddr = Fee_lGetNextWLAddr(DestAddr);
    }
    StateDataPtr->FeeGcDestAddr = DestAddr;

    /*************** CHECK for the case of GC TRIGGERING GC *****************/
    if (StateDataPtr->FeeBlkPageCount == FEE_ZERO)
    {
      /* Get the Destination sector - It is the sector other than curr sector */
      BankNumber = (uint8)StateDataPtr->FeeStatusFlags.FeeCurrSector ^ FEE_ONE;
      /* Find the free pages in the destination sector */
      FreePages = Fee_lGetFreeDFLASHPages(BankNumber, DestAddr);
      /* Get the block page count */
      PageCount = (uint32)StateDataPtr->FeeGcCurrBlkInfo.PageCount;
      /* Check if the block is crossing the threshold */
      if ((boolean)TRUE == Fee_lIsSectorThresholdCrossed(FreePages, PageCount))
      {
        Error = FEE_GC_TRIG;
      }
    }
    if (Error == FEE_ZERO)
    {
      Offset = (uint16) (DestAddr & FEE_DFLASH_WORDLINE_SIZE_1);
      if(Offset == FEE_ZERO)
      {
        StateDataPtr->FeeGcFirstBlkNumInWL = GcCurrBlkPtr->BlockNumber;
        StateDataPtr->FeeGcBlkIndexInWL = FEE_ZERO;
      }
      if(StateDataPtr->FeeBlkPageCount == FEE_ZERO)
      {
        StateDataPtr->FeeBlockStartAddr = DestAddr;
      }
      Error = Fee_lGcWrite((Fls_AddressType)DestAddr, Offset);
    }
  }

  return (Error);
}

/*******************************************************************************
** Syntax : static void Fee_lGcCopyReadNxtPage(void)                          **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine is responsible for handling Garbage Collection  **
**                FEE_GC_COPY_READ_NXTPAGE state                              **
*******************************************************************************/
static void Fee_lGcCopyReadNxtPage(void)
{
  uint32                 SrcAddr;
  Fee_StateDataType     *StateDataPtr;
  uint16                 Offset;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* The transitions from this state are handled as part of Fee_lGcPrologue
     Hence only state actions are done here */

  Offset = (uint16) ((StateDataPtr->FeeGcDestAddr) & 
                                            (uint32)FEE_DFLASH_WORDLINE_SIZE_1);
  SrcAddr = StateDataPtr->FeeGcSrcAddr;
  if (Fee_lIsUnerasableAddr(SrcAddr))
  {
    SrcAddr = Fee_lGetNextWLAddr(SrcAddr);
  }
  StateDataPtr->FeeGcSrcAddr = SrcAddr;
  Fee_lGcRead((Fls_AddressType)(StateDataPtr->FeeGcSrcAddr), Offset);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lGcBlkCompare(void)                     **
**                                                                            **
** Service ID: NA                                                             **
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
** Description : Performs compare actions on FEE_GC_START_COPY_WRITE          **
**               during Garbage collection                                    **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lGcBlkCompare(void)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_GcBlkInfoType     *GcCurrBlkPtr;
  Fee_CacheType         *CachePtr;
  Fee_CacheType         *UnconfigBlockPtr;
  uint32                 DestAddr;
  Fls_AddressType        CompAddr;
  Fls_LengthType         CompLength;
  Fls_LengthType         TmpCompareLength;
  uint16                 BlockNumber;
  uint16                 Index;
  Std_ReturnType         ReturnVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  CachePtr = StateDataPtr->FeeBlockInfo;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
  UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) +
                     (FEE_CONFIG_PTR->FeeBlkCnt);
  GcCurrBlkPtr = &(StateDataPtr->FeeGcCurrBlkInfo);

  DestAddr = StateDataPtr->FeeGcDestAddr;
  /* Compare entire WL */
  CompAddr = (Fls_AddressType)Fee_lGetWLBeginning(DestAddr);

  CompLength = (StateDataPtr->FeeGcDestAddr) & 
                                     (Fls_LengthType)FEE_DFLASH_WORDLINE_SIZE_1;
  CompLength += (StateDataPtr->FeeLastWriteSize);
  TmpCompareLength = CompLength;
  
  CompLength -=  StateDataPtr->FeeComparedLen;
  if(CompLength > (Fls_LengthType)FEE_COMPARE_LEN_MAX)
  {
    CompLength = (Fls_LengthType)FEE_COMPARE_LEN_MAX;
  }
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer + StateDataPtr->FeeComparedLen*/
  ReturnVal = Fls_17_Pmu_CompareWordsSync(
                     (CompAddr+StateDataPtr->FeeComparedLen),
                     (uint32 *)(void *)(StateDataPtr->FeeGcReadWriteBuffer +
                     StateDataPtr->FeeComparedLen),
                     CompLength/FEE_DFLASH_WORD_SIZE);
                     
  StateDataPtr->FeeComparedLen += (uint16)CompLength;

  if(ReturnVal == E_NOT_OK)
  {
    StateDataPtr->FeeComparedLen = FEE_ZERO;
    if(StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
    {
      StateDataPtr->FeeIntrJobFailCount++;

      if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_WRITE)
      {
        if(StateDataPtr->FeeGcBlkIndexInWL != FEE_ZERO)
        {
          Fee_lHandleUnConfigGcCopyError();
        }
        StateDataPtr->FeeGcState = FEE_GC_UNCFG_START_COPY;
      }
      else /* FeeGcState == FEE_GC_COPY_WRITE */
      {
        if(StateDataPtr->FeeGcBlkIndexInWL != FEE_ZERO)
        {
          Fee_lHandleConfigGcCopyError();
        }
        StateDataPtr->FeeGcState = FEE_GC_START_COPY;
      }
      DestAddr = Fee_lGetNextWLAddr(DestAddr);
      StateDataPtr->FeeGcDestAddr = DestAddr;
    }
    else
    {
      if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_WRITE)
      {
        Fee_lGcDiscardUnconfigBlk();
        StateDataPtr->FeeGcState = FEE_GC_UNCFG_START_COPY;
      }
      else /* FeeGcState == FEE_GC_COPY_WRITE */
      {
        Fee_lGCErrorHandler(FEE_GC_WRITE);
      }
    }
  }
  else if(StateDataPtr->FeeComparedLen != (uint16)TmpCompareLength)
  {
    /* dummy else if to skip else for inbetween loops */
  }
  /* Compare successful */
  else
  {
    StateDataPtr->FeeComparedLen = FEE_ZERO;
    StateDataPtr->FeeGcSrcAddr += StateDataPtr->FeeLastWriteSize;
    StateDataPtr->FeeGcDestAddr += StateDataPtr->FeeLastWriteSize;

    StateDataPtr->FeeBlkPageCount += 
                   (Fee_PageType)(StateDataPtr->FeeLastWriteSize >> FEE_THREE);
    if (StateDataPtr->FeeBlkPageCount == (GcCurrBlkPtr->PageCount + FEE_ONE))
    {
      /* Clear the compare fail count if the block write is compelte */
      /* Count shall not be cleared upon completion of previous block write */
      if (GcCurrBlkPtr->BlockNumber != StateDataPtr->FeeGcPrevBlockNumber)
      {
        StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;
      }
      /* Update the cache table for curr block */
      BlockNumber = GcCurrBlkPtr->BlockNumber;

      if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_WRITE)
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
          efficiently access the data within 
          FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
        UnconfigBlockPtr += (StateDataPtr->FeeGCUnconfigBlkCopyIndex);
        UnconfigBlockPtr->Status.Copied = FEE_ONE;
        StateDataPtr->FeeGcLWBGcSrcAddr[StateDataPtr->FeeGcBlkIndexInWL] =
                                      UnconfigBlockPtr->Address;
        UnconfigBlockPtr->Address = StateDataPtr->FeeBlockStartAddr;
        StateDataPtr->FeeGCUnconfigBlkCopyIndex++;

        /* Move to next state */
        StateDataPtr->FeeGcState = FEE_GC_UNCFG_START_COPY;
      }
      else /* FEE_GC_COPY_WRITE */
      {
        Index = Fee_lGetUsrBlockIndex(BlockNumber);
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
          efficiently access the data within 
          FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
        CachePtr = CachePtr + Index;
        CachePtr->Status.Copied = FEE_ONE;
        StateDataPtr->FeeGcLWBGcSrcAddr[StateDataPtr->FeeGcBlkIndexInWL] =
                                                           CachePtr->Address;
        CachePtr->Address = StateDataPtr->FeeBlockStartAddr;
        StateDataPtr->FeeGCCopyIndex++;

        /* Move to next state */
        StateDataPtr->FeeGcState = FEE_GC_START_COPY;
      }

      /* Update LastBlock with Curr block info */
      StateDataPtr->FeeGcPrevBlockNumber = GcCurrBlkPtr->BlockNumber;

      (StateDataPtr->FeeGcBlkIndexInWL)++;
    }
    else
    {
      if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_WRITE)
      {
        StateDataPtr->FeeGcState = FEE_GC_UNCFG_COPY_READ_NXTPAGE;
      }
      else /* FEE_GC_COPY_WRITE */
      {
        StateDataPtr->FeeGcState = FEE_GC_COPY_READ_NXTPAGE;
      }
    }
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleUnConfigGcCopyError(void)        **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This function handles un-configured block's copy error       **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleUnConfigGcCopyError(void)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *UnconfigBlockPtr;
  uint16                 BlockIndex;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
  UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) +
                     (FEE_CONFIG_PTR->FeeBlkCnt);
  BlockIndex = FEE_ZERO;

  /* Find the index of the first affected un-configured block */
  while((BlockIndex < (StateDataPtr->FeeUnConfigBlockCount)) &&
        ((UnconfigBlockPtr->BlockNumber) != StateDataPtr->FeeGcFirstBlkNumInWL))
  {
    BlockIndex++;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    UnconfigBlockPtr++;
  }
  StateDataPtr->FeeGCUnconfigBlkCopyIndex = BlockIndex;

  /* Clear the Copied status of all the affected un-configured blocks */
  for(BlockIndex = FEE_ZERO;
      BlockIndex < StateDataPtr->FeeGcBlkIndexInWL;
      BlockIndex++)
  {
    UnconfigBlockPtr->Status.Copied = FEE_ZERO;
    UnconfigBlockPtr->Address = StateDataPtr->FeeGcLWBGcSrcAddr[BlockIndex];
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    UnconfigBlockPtr++;
  }
  StateDataPtr->FeeGcBlkIndexInWL = FEE_ZERO;
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleConfigGcCopyError(void)          **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This function handles configured block's copy error          **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleConfigGcCopyError(void)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *CachePtr;
  uint16                 BlockIndex;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  BlockIndex = Fee_lGetUsrBlockIndex(StateDataPtr->FeeGcFirstBlkNumInWL);
  CachePtr = &(StateDataPtr->FeeBlockInfo[BlockIndex]);

  StateDataPtr->FeeGCCopyIndex = BlockIndex;
  for(BlockIndex = FEE_ZERO;
      BlockIndex < StateDataPtr->FeeGcBlkIndexInWL;
      BlockIndex++)
  {
    if(CachePtr->Status.Copied == (unsigned_int)FEE_ONE)
    {
      /* Latest copy not available. Move GcLogBlkCount to
              prev block Index and switch to Start Copy state */
      CachePtr->Status.Copied = FEE_ZERO;
      CachePtr->Address = StateDataPtr->FeeGcLWBGcSrcAddr[BlockIndex];
    }
    else
    {
      /* Latest copy of the block is available: no need to copy the
         affected previous block.
         Just make the PrevCopyStatus as zero. This is needed to mark
         the affected previous copy as inconsistent */
      CachePtr->Status.PrevCopyCopied = FEE_ZERO;
    }
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    CachePtr++;
  }
  StateDataPtr->FeeGcBlkIndexInWL = FEE_ZERO;
}

/*******************************************************************************
** Syntax : static uint8 Fee_lGcErasePrevSector(uint8 GcState)                **
**                                                                            **
** Service ID:  NA                                                            **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  GcState: Curr Gc State                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint8                                                   **
**                                                                            **
** Description : This routine is responsible for handling Garbage Collection  **
**    FEE_GC_ERASE_PREV_SECTOR / FEE_GC_INIT_ERASE_PREV_SECTOR state          **
*******************************************************************************/
static uint8 Fee_lGcErasePrevSector(uint8 GcState)
{
  Fee_StateDataType   *StateDataPtr;
  Std_ReturnType       ReturnValueFls;
  uint8                Error;

  Error = FEE_ZERO;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  if (StateDataPtr->FeeIntrJobStatus == FEE_JOB_DONE)
  {
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    if(StateDataPtr->FeeIntrJobResult == FEE_JOB_FAIL)
    {
       Fee_lGCErrorHandler(FEE_GC_ERASE);
    }
    else
    {
      Fee_lGcEraseCompare(GcState);
    }
  }
  else if((StateDataPtr->FeeSectorCount < FLS_ERASE_SECTORS) &&
         (StateDataPtr->FeeSectorCount > (uint8)0))
  {
    Fee_lGcEraseCompare(GcState);
  }
  else
  {
    /* i.e. StateDataPtr->FeeGcState == FEE_GC_ERASE_PREV_SECTOR or
                                      FEE_GC_INIT_ERASE_PREV_SECTOR  */
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_REQSTD;
    StateDataPtr->FeeIntrJob = FEE_ERASE_JOB;

    ReturnValueFls = Fls_17_Pmu_Erase(
                                (Fls_AddressType)StateDataPtr->FeeGcDestAddr,
                                (Fls_LengthType)FEE_DFLASH_SECTOR_SIZE);

        /* if the Fls routine did not accept the request */
    if(ReturnValueFls == E_NOT_OK)
    {
      /* Any failure from FLS (both DET,DEM) will trigger Illegal notification
         and FEE DEM */
        Error = FEE_GC_ERASE;
    }
  }

  return (Error);
}


/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lGcEraseCompare(uint8 GcState)          **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  GcState: Curr Gc State                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : Performs state transitions from FEE_GC_ERASE_PREV_SECT or    **
**  FEE_GC_INIT_ERASE_PREV_SECTOR during Garbage collection                   **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lGcEraseCompare(uint8 GcState)
{
  Fee_StateDataType    *StateDataPtr;
  uint32                BankNumber;
  Std_ReturnType        ReturnVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  if(StateDataPtr->FeeSectorCount == 0U)
  {
    StateDataPtr->FeeUnErasableWLAddrTemp = FEE_ZERO;
    StateDataPtr->FeeUnErasableWLCountTemp = FEE_ZERO;
  }

  if(StateDataPtr->FeeGcDestAddr == FEE_DFLASH_BANK1_BASE)
  {
   BankNumber = FEE_ONE;
  }
  else
  {
   BankNumber = FEE_ZERO;
  }

  ReturnVal = Fls_17_Pmu_VerifySectorErase(BankNumber, 
                         &(StateDataPtr->FeeUnErasableWLAddrTemp),
                         &(StateDataPtr->FeeUnErasableWLCountTemp),
                         StateDataPtr->FeeSectorCount);
  if(ReturnVal == E_NOT_OK)
  {
    Fee_lGCErrorHandler(FEE_GC_ERASE);
  }
  else
  { 
    StateDataPtr->FeeSectorCount++;
    if(StateDataPtr->FeeSectorCount == FLS_ERASE_SECTORS)
    {
      StateDataPtr->FeeSectorInfo[BankNumber].UnerasableWLCount = 
                                  StateDataPtr->FeeUnErasableWLCountTemp;
      StateDataPtr->FeeSectorInfo[BankNumber].UnerasableWLAddr = 
                                  StateDataPtr->FeeUnErasableWLAddrTemp;
  
      if(GcState == FEE_GC_ERASE_PREV_SECTOR)
      {
        GcState = FEE_GC_MARK_SECTOR0_E_SP;
      }
      /*  Case FEE_GC_INIT_ERASE_PREV_SECTOR */
      else
      {
        /* Erase of prev sector is happening as part of INIT GC.
           The FeeStateCount shall be incremented by 2 to mark one sector 
           erase. Increment FeeStateCount by 1 here. FeeStateCount is 
           incremented again while preparing the state page buffer in
           FEE_GC_INIT_MARK_SECTOR0_E_SP */
        StateDataPtr->FeeStateCount += FEE_ONE;
        GcState = FEE_GC_INIT_MARK_SECTOR0_E_SP;
      }
      /* Move to next state */
      StateDataPtr->FeeGcState = GcState;
      StateDataPtr->FeeSectorCount = FEE_ZERO;
    }  
  }
}

/*******************************************************************************
** Syntax : static uint8 Fee_lGcProgramStatePage(uint8 GcState)               **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  GcState: Curr Gc State                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint8                                                   **
**                                                                            **
** Description : This routine is responsible for marking state page during    **
**               Garbage Collection                                           **
*******************************************************************************/
static uint8 Fee_lGcProgramStatePage(uint8 GcState)
{
  uint32                 DestAddr;
  uint32                 FreePages;
  uint32                 PageCount;
  Fee_StateDataType     *StateDataPtr;
  uint8                  BankNumber;
  uint8                  Error;

  Error = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  if (StateDataPtr->FeeIntrJobStatus == FEE_JOB_DONE)
  {
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    if(StateDataPtr->FeeIntrJobResult == FEE_JOB_FAIL)
    {
       Fee_lGCErrorHandler(FEE_GC_WRITE);
    }
    else
    {
      Fee_lGcStatePageCompare(GcState);
    }
  }
  else
  {
    DestAddr = StateDataPtr->FeeGcDestAddr;
    if(DestAddr >= FEE_DFLASH_BANK1_BASE)
    {
      BankNumber = FEE_ONE;
    }
    else
    {
      BankNumber = FEE_ZERO;
    }

    switch (GcState)
    {
      case FEE_GC_MARK_SECTOR1_V_SP:
      case FEE_GC_MARK_SECTOR0_E_SP:
      case FEE_GC_INIT_MARK_SECTOR0_E_SP:
      {
        if (StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
        {
          if (GcState == FEE_GC_MARK_SECTOR1_V_SP)
          {
            Fee_lPrepareStatePageBuffer(BankNumber, FEE_VALID);
          }
          else
          {
            Fee_lPrepareStatePageBuffer(BankNumber, FEE_ERASED);
          }
        }
        else
        {
          /* The state page is being reattempted after one compare failure
             Clear the last 8bytes of GcReadWriteBuffer for comparison of SP */
          Fee_lClearMarkerBuffer();
        }
        if(Fee_lIsUnerasableAddr(DestAddr))
        {
          DestAddr = Fee_lGetNextWLAddr(DestAddr);
        }
        StateDataPtr->FeeGcDestAddr = DestAddr;

        /************* CHECK for the case of GC TRIGGERING GC *************/
        /* Get the Destination sector - the sector other than curr sector */
        BankNumber= (uint8)StateDataPtr->FeeStatusFlags.FeeCurrSector ^ FEE_ONE;
        /* Find the free pages in the destination sector */
        FreePages = Fee_lGetFreeDFLASHPages(BankNumber, DestAddr);
        /* Get the state page count */
        PageCount = FEE_DFLASH_WORDLINE_SIZE/FEE_DFLASH_PAGE_SIZE;
        /* Check if there is space in the new sector for state page */
        if ((boolean)TRUE == Fee_lIsSectorEndCrossed(FreePages, PageCount))
        {
          Error = FEE_GC_TRIG;
        }
        else
        {
          StateDataPtr->FeeLastReadSize = FEE_STATEPAGE_PROGRAM_SIZE;
          Error = Fee_lGcWrite((Fls_AddressType)DestAddr, FEE_ZERO);
        }
        break;
      }

      /*
        Remaining cases:
          FEE_GC_MARK_SECTOR1_V_M
          FEE_GC_MARK_SECTOR0_E_M
          FEE_GC_INIT_MARK_SECTOR0_E_M
      */
      default:
      {
        Fee_lPrepareMarkerBuffer();
        StateDataPtr->FeeLastReadSize = FEE_STATEPAGE_MARKER_SIZE;
        Error= Fee_lGcWrite((Fls_AddressType)(DestAddr+FEE_STATEPAGE_DATA_SIZE),
                             FEE_STATEPAGE_DATA_SIZE);
        break;
      }
    }
  }

  return (Error);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lGcStatePageCompare(uint8 GcState)      **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  GcState: Curr Gc State                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : Performs compare actions on state page programming           **
**               during Garbage collection                                    **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lGcStatePageCompare(uint8 GcState)
{
  Fee_StateDataType    *StateDataPtr;
  Fee_SectorInfoType   *SectorInfoPtr;
  uint8                *SrcAddr;
  uint32                DestAddr;
  uint32                BankNumber;
  Std_ReturnType        ReturnVal1;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  DestAddr = StateDataPtr->FeeGcDestAddr;
  SrcAddr = StateDataPtr->FeeGcReadWriteBuffer;
  ReturnVal1 = Fls_17_Pmu_CompareWordsSync(
                                 (Fls_AddressType)DestAddr,
                                 (uint32 *)(void *)SrcAddr,
                                 (Fls_LengthType)FEE_STATEPAGE_SIZE/FEE_FOUR);

  if (ReturnVal1 == E_NOT_OK)
  {
    if(StateDataPtr->FeeIntrJobFailCount == FEE_ZERO)
    {
      StateDataPtr->FeeIntrJobFailCount++;
      DestAddr = Fee_lGetNextWLAddr(DestAddr);
      StateDataPtr->FeeGcDestAddr = DestAddr;

      switch (GcState)
      {
        case FEE_GC_MARK_SECTOR1_V_SP:
        case FEE_GC_MARK_SECTOR1_V_M:
        {
          GcState = FEE_GC_MARK_SECTOR1_V_SP;
          break;
        }

        case FEE_GC_MARK_SECTOR0_E_SP:
        case FEE_GC_MARK_SECTOR0_E_M:
        {
          GcState = FEE_GC_MARK_SECTOR0_E_SP;
          break;
        }

        /*
          Remaining cases:
            FEE_GC_INIT_MARK_SECTOR0_E_SP
            FEE_GC_INIT_MARK_SECTOR0_E_M
        */
        default:
        {
          GcState = FEE_GC_INIT_MARK_SECTOR0_E_SP;
          break;
        }
      }
      StateDataPtr->FeeGcState = GcState;
    }
    else
    {
      Fee_lGCErrorHandler(FEE_GC_WRITE);
    }
  }
  else
  {
    switch (GcState)
    {
      case FEE_GC_MARK_SECTOR1_V_SP:
      {
        GcState = FEE_GC_MARK_SECTOR1_V_M;
        break;
      }
      case FEE_GC_MARK_SECTOR1_V_M:
      {
        StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;
        /* Update the current sector to next sector */
        StateDataPtr->FeeStatusFlags.FeeCurrSector ^= FEE_ONE;

        /* Update FreeWLAddr and StatePage Addr and NextFreePageAddr */
        BankNumber = StateDataPtr->FeeStatusFlags.FeeCurrSector;
        SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
        SectorInfoPtr->StatePageAddr = DestAddr;
        SectorInfoPtr->NextFreeWLAddr = Fee_lGetNextWLAddr(DestAddr);
        /* Update NextFreePageAddr */
        StateDataPtr->FeeNextFreePageAddr = SectorInfoPtr->NextFreeWLAddr;

        StateDataPtr->FeeLastWrittenBlkInfo.BlockNumber = FEE_ZERO;
        StateDataPtr->FeeLastWrittenBlkInfo.Status.Consistent = FEE_ZERO;

        if(BankNumber == FEE_ZERO)
        {
          StateDataPtr->FeeGcDestAddr = FEE_DFLASH_BANK1_BASE;
        }
        else
        {
          StateDataPtr->FeeGcDestAddr = FEE_DFLASH_BANK0_BASE;
        }

        /* Move to next state */
        GcState = FEE_GC_ERASE_PREV_SECTOR;
        break;
      }
      case FEE_GC_MARK_SECTOR0_E_SP:
      {
        GcState = FEE_GC_MARK_SECTOR0_E_M;
        break;
      }
      case FEE_GC_INIT_MARK_SECTOR0_E_SP:
      {
        GcState = FEE_GC_INIT_MARK_SECTOR0_E_M;
        break;
      }
      /*
        Remaining cases:
          FEE_GC_MARK_SECTOR0_E_M
          FEE_GC_INIT_MARK_SECTOR0_E_M
      */
      default:
      {
        StateDataPtr->FeeIntrJobFailCount = FEE_ZERO;
        /* Get the curr active sector number */
        BankNumber = StateDataPtr->FeeStatusFlags.FeeCurrSector;
        /* calculate the prev sector number */
        BankNumber = BankNumber ^ FEE_ONE;
        /* Update FreeWLAddr and StatePage Addr */
        SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
        SectorInfoPtr->StatePageAddr = DestAddr;
        SectorInfoPtr->NextFreeWLAddr = Fee_lGetNextWLAddr(DestAddr);
        if (GcState == FEE_GC_MARK_SECTOR0_E_M)
        {
          GcState = FEE_GC_IDLE;
        }
        else
        {
          GcState = FEE_GC_REQUESTED;
        }
        break;
      }
    }
    StateDataPtr->FeeGcState = GcState;
  }
}

/*******************************************************************************
** Syntax : static void Fee_lGarbageCollection(void)                          **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine is responsible for handling Garbage Collection  **
** which is done once the Sector reaches the threshold limit.                 **
*******************************************************************************/
static void Fee_lGarbageCollection(void)
{
  Fee_StateDataType     *StateDataPtr;
  uint8                  Error;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  Error = FEE_ZERO;

  /* Asynch Transitions based ONLY on IntrJobStatus are done here */
  Fee_lGcPrologue();

  /**********************************************************************/
  /*         STATE MACHINE EXECUTION and STATE TRANSITIONS              */
  /**********************************************************************/

  /*********** GC REQSTD STATE *******************/
  /*  Initialisation of variables is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_REQUESTED)
  {
    #if (FEE_UNCFG_BLK_OVERFLOW_HANDLE == FEE_STOP_AT_GC)
    if(StateDataPtr->FeeUncfgBlksExceeded != FEE_UNCONFIG_BLK_LIMIT_CROSSED)
    #endif
    {
      Fee_lGcRequested();
    }
  }


  /********* FEE_GC_UNCFG_START_COPY *******/
  /*  Reading the first page of an Unconfig block is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_START_COPY)
  {
    Fee_lGcStartCopy();
  }

  /*********** FEE_GC_UNCFG_COPY_WRITE *******************/
  /*  Writing a page of an unconfig block is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_WRITE)
  {
    Error = Fee_lGcStartCopyWrite();
  }

  /*********** FEE_GC_UNCFG_COPY_READ_NXTPAGE *******************/
  /*  Reading the subsequent pages of an Unconfig block is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_UNCFG_COPY_READ_NXTPAGE)
  {
    Fee_lGcCopyReadNxtPage();
  }


  /*********** FEE_GC_START_COPY *******************/
  /*  Reading the first page of a Config block is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_START_COPY)
  {
    Fee_lGcStartCopy();
  }


  /*********** FEE_GC_COPY_WRITE *******************/
  /*  Writing a page of a Config block is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_COPY_WRITE)
  {
    Error = Fee_lGcStartCopyWrite();
  }


  /*********** FEE_GC_COPY_READ_NXTPAGE *******************/
  /*  Reading the subsequent pages of a Config block is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_COPY_READ_NXTPAGE)
  {
    Fee_lGcCopyReadNxtPage();
  }


  /*********** FEE_GC_MARK_SECTOR1_V_SP *******************/
  /*  Writing the valid state page content is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_MARK_SECTOR1_V_SP)
  {
    Error = Fee_lGcProgramStatePage(FEE_GC_MARK_SECTOR1_V_SP);
  }


  /*********** FEE_GC_MARK_SECTOR1_V_M *******************/
  /*  Writing the valid state page marker is done here */
  if(StateDataPtr->FeeGcState == FEE_GC_MARK_SECTOR1_V_M)
  {
    Error = Fee_lGcProgramStatePage(FEE_GC_MARK_SECTOR1_V_M);
  }


  /***** FEE_GC_INIT_ERASE_PREV_SECTOR / FEE_GC_ERASE_PREV_SECTOR *********/
  /*  Erasing the previous sector is done here */
  if (StateDataPtr->FeeGcState == FEE_GC_ERASE_PREV_SECTOR)
  {
    Error = Fee_lGcErasePrevSector(FEE_GC_ERASE_PREV_SECTOR);
  }
  if (StateDataPtr->FeeGcState == FEE_GC_INIT_ERASE_PREV_SECTOR)
  {
    Error = Fee_lGcErasePrevSector(FEE_GC_INIT_ERASE_PREV_SECTOR);
  }


  /***** FEE_GC_INIT_MARK_SECTOR0_E_SP /
        FEE_GC_MARK_SECTOR0_E_SP *********/
  /*  Writing the erased state page content is done here */
  if (StateDataPtr->FeeGcState == FEE_GC_MARK_SECTOR0_E_SP)
  {
    Error = Fee_lGcProgramStatePage(FEE_GC_MARK_SECTOR0_E_SP);
  }
  if (StateDataPtr->FeeGcState == FEE_GC_INIT_MARK_SECTOR0_E_SP)
  {
    Error = Fee_lGcProgramStatePage(FEE_GC_INIT_MARK_SECTOR0_E_SP);
  }

  /***** FEE_GC_INIT_MARK_SECTOR0_E_M /
        FEE_GC_MARK_SECTOR0_E_M *********/
  /*  Writing the erased state page marker is done here */
  if (StateDataPtr->FeeGcState == FEE_GC_MARK_SECTOR0_E_M)
  {
    Error = Fee_lGcProgramStatePage(FEE_GC_MARK_SECTOR0_E_M);
  }

  if (StateDataPtr->FeeGcState == FEE_GC_INIT_MARK_SECTOR0_E_M)
  {
    Error = Fee_lGcProgramStatePage(FEE_GC_INIT_MARK_SECTOR0_E_M);
  }

  /* Report Dem, if Error occured */
  if (Error != FEE_ZERO)
  {
    /* Clear all variables */
    StateDataPtr->FeeIntrJobStatus = FEE_JOB_IDLE;
    StateDataPtr->FeeIntrJobResult = FEE_JOB_FAIL;
    StateDataPtr->FeeIntrJob = FEE_NO_JOB;
    /* Fee_lGCErrorHandler(Error); */
    Fee_lGCErrorHandler(Error);
  }
}/*End of Fee_lGarbageCollection */

/*******************************************************************************
** Syntax : static Fee_PageType Fee_lGetDFLASHPages(uint16 Size)              **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  size                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Fee_PageType                                            **
**                                                                            **
** Description : This routine will calculate and return the number of DFLASH  **
** pages required for a logical block size.                                   **
*******************************************************************************/
static Fee_PageType Fee_lGetDFLASHPages(uint16 Size)
{
  Fee_PageType  PhysicalPages;
  
  PhysicalPages = FEE_TWO;

  while (Size >= FEE_SEVEN)
  {
    PhysicalPages++;
    Size -= FEE_SEVEN;
  }
  if (Size > FEE_ZERO)
  {
    PhysicalPages++;
  }

  return (PhysicalPages);

}/*End of Fee_lGetDFLASHPages */


/*******************************************************************************
** Syntax : static Fee_PageType Fee_lGetFreeDFLASHPages(uint8 BankNumber      **
**                                               uint32 StartAddr)            **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :BankNumber : Bank Number                                  **
**                  StartAddr  : Current address                              **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Fee_PageType                                            **
**                                                                            **
** Description : This routine will calculate the number of empty DFLASH  pages**
** remaining in the sector.                                                   **
*******************************************************************************/
static Fee_PageType Fee_lGetFreeDFLASHPages(uint8 BankNumber, uint32 StartAddr)
{
  Fee_StateDataType *StateDataPtr;
  Fee_PageType       FreePages;
  uint32             EndAddr;
  uint32             WLAddr;
  sint32             RemainingPages;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  
  /* Get the sector end address */
  if (BankNumber == FEE_ZERO)
  {
    EndAddr = FEE_DFLASH_BANK0_BASE + FEE_DFLASH_SECTOR_SIZE;
  }
  else
  {
    EndAddr = FEE_DFLASH_BANK1_BASE + FEE_DFLASH_SECTOR_SIZE;
  }

  RemainingPages = ( ((sint32)EndAddr - (sint32)StartAddr) /
                         (sint32)FEE_DFLASH_PAGE_SIZE );

  /* Subtract the pages of un-erasable word line, if any */
  if (StateDataPtr->FeeSectorInfo[BankNumber].UnerasableWLCount != FEE_ZERO)
  {
    WLAddr = StateDataPtr->FeeSectorInfo[BankNumber].UnerasableWLAddr;
    if ( (StartAddr <= WLAddr) && (WLAddr <= EndAddr) )
    {
      RemainingPages = RemainingPages -
                      (sint32)(FEE_DFLASH_WORDLINE_SIZE / FEE_DFLASH_PAGE_SIZE);
    }
  }

  if (RemainingPages > (sint32)FEE_ZERO)
  {
    FreePages = (Fee_PageType)RemainingPages;
  }
  else
  {
    FreePages = FEE_ZERO;
  }

  return (FreePages);

}/*End of Fee_lGetFreeDFLASHPages */


/*******************************************************************************
** Syntax : static uint16 Fee_lGetUsrBlockIndex(uint16 BlockNumber)           **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber - Number of logical block                   **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint8                                                   **
**                                                                            **
** Description : This utility function will return the Index (location)of the **
** block in the user configuration.                                           **
*******************************************************************************/
static uint16 Fee_lGetUsrBlockIndex(uint16 BlockNumber)
{
  const Fee_BlockType  *BlockPtr;
  uint16                Index;
  uint16                BlockNumberRef;

  Index = FEE_ZERO;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
    Fee_BlockConfig[Number of configured blocks] to efficiently access its 
    elements*/
  BlockPtr = &(FEE_CONFIG_PTR->FeeBlockConfigPtr[Index]);
  BlockNumberRef = BlockPtr->BlockNumber;

  while( (Index < FEE_CONFIG_PTR->FeeBlkCnt) &&
         (BlockNumberRef != BlockNumber)
       )
  {
    Index++;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      Fee_BlockConfig[Number of configured blocks] to efficiently access its 
      elements*/
    BlockPtr++;
    BlockNumberRef = BlockPtr->BlockNumber;
  }

  /* if block is not configured, return invalid block index */
  if( Index == FEE_CONFIG_PTR->FeeBlkCnt )
  {
    Index = FEE_LOG_BLOCK_NOT_FOUND;
  }

  return (Index);

}/* End of Fee_lGetUsrBlockIndex */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lComputeNextFreePageAddr                **
**                                                  (uint32 BankNumber)       **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :BankNumber                                            **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function computes the first free page addr      **
** of the sector based on NextFreeWLAddr                                      **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lComputeNextFreePageAddr(uint32 BankNumber)
{
  Fee_StateDataType  *StateDataPtr;
  Fee_SectorInfoType *SectorInfoPtr;
  uint32             *TempDataPtr;
  uint32              TempAddr;
  uint32              TempAddrPrevWL;
  Std_ReturnType      RetVal;
  uint8               LoopEnd;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
  LoopEnd = FEE_ZERO;

  /* Get the WL before the Next Free WL of the sector */
  StateDataPtr->FeeNextFreePageAddr = SectorInfoPtr->NextFreeWLAddr;

  /* Check if the WL before the free WL is state page */
  TempAddrPrevWL = SectorInfoPtr->NextFreeWLAddr - FEE_DFLASH_WORDLINE_SIZE;
  
  /* If previous word line contains a state page OR
     if it is an un-erasable word line, then next free page address is same
     as next free word line address; otherwise proceed to indentify the 
     next free page address */
  if ((TempAddrPrevWL != SectorInfoPtr->StatePageAddr) &&
      (!((SectorInfoPtr->UnerasableWLCount != FEE_ZERO) &&
         (SectorInfoPtr->UnerasableWLAddr  == TempAddrPrevWL))))
  {
    TempAddr = SectorInfoPtr->NextFreeWLAddr;
    TempAddrPrevWL += FEE_DFLASH_PAGE_SIZE;

    do
    {
      TempAddr -= FEE_DFLASH_PAGE_SIZE;
      /* Read the header (2 Words) */
      RetVal = Fls_17_Pmu_ReadWordsSync((Fls_AddressType)TempAddr,
                           (uint32 *)(void *)StateDataPtr->FeeReadWriteBuffer,
                                    (Fls_LengthType)FEE_TWO);
      TempDataPtr = (uint32 *)(void *)StateDataPtr->FeeReadWriteBuffer;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      if((RetVal == E_OK) &&
         (*(TempDataPtr)     == (uint32)FEE_ZERO) &&
         (*(TempDataPtr + 1) == (uint32)FEE_ZERO))
      {
        StateDataPtr->FeeNextFreePageAddr = TempAddr;
      }
      else
      {
        LoopEnd = FEE_ONE;
        if ((RetVal == E_OK) &&
            ((StateDataPtr->FeeReadWriteBuffer[0] == FEE_DATA_NEXT_PAGE_ID) ||
             (StateDataPtr->FeeReadWriteBuffer[0] == FEE_DATA_FIRST_PAGE_ID)))
        {
          StateDataPtr->FeeLastWrittenBlockDirty = FEE_LWB_CHECK_FOR_DIRTY;
        }
        else if (RetVal != E_OK)
        {
          StateDataPtr->FeeNextFreePageAddr = SectorInfoPtr->NextFreeWLAddr;
        }
        else
        {
          /* dummy else */
        }
      }
    }while ((TempAddr > TempAddrPrevWL) && (LoopEnd == FEE_ZERO));
  }
}

/*******************************************************************************
** Syntax : static void Fee_lUpdateCache(void)                                **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function  will update the cache which contains  **
** the latest information about the DFLASH contents.                          **
*******************************************************************************/
static void Fee_lUpdateCache(void)
{
  Fee_StateDataType      *StateDataPtr;
  Fee_SectorInfoType     *SectorInfoPtr;
  uint32                  EndAddr;
  uint32                  BankNumber;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  if(StateDataPtr->FeeStatusFlags.FeeCurrSector == (unsigned_int)FEE_ZERO)
  {
    EndAddr = FEE_DFLASH_BANK0_BASE;
    BankNumber = FEE_SECTOR0;
  }
  else
  {
    EndAddr = FEE_DFLASH_BANK1_BASE;
    BankNumber = FEE_SECTOR1;
  }

  SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];
  
  /* Identifies next free page */
  Fee_lComputeNextFreePageAddr(BankNumber);

  /* Point to last written page of the valid sector */
  StateDataPtr->FeeCurrSectSrcAddr = StateDataPtr->FeeNextFreePageAddr -
                FEE_DFLASH_PAGE_SIZE;

  /* Reset the number of pages in the block */
  StateDataPtr->FeeBlkPageCount = FEE_ZERO;

  /* Start the Cache Update state machine */
  StateDataPtr->FeeCacheState = FEE_CACHE_FIND_NEXT_BLK;

  Fee_lUpdateCacheTable(BankNumber, EndAddr);

  /* check if marker is missing for the last written block */
  if(StateDataPtr->FeeLastWrittenBlockDirty == FEE_LWB_DIRTY)
  {
    /* move next free page address to next page */
    /* to avoid any further write attempts on the missing marker location */
    StateDataPtr->FeeNextFreePageAddr += FEE_DFLASH_PAGE_SIZE;
    /* If next free page is the beginning of a word line */
    if(Fee_lIsWLBeginning(StateDataPtr->FeeNextFreePageAddr) == FEE_ZERO)
    {
      /* clear last written block info */
      StateDataPtr->FeeLastWrittenBlkInfo.BlockNumber = FEE_ZERO;
      StateDataPtr->FeeLastWrittenBlkInfo.Status.Consistent = FEE_ZERO;
    }
    /* if next free page address is beyond next free word line address */
    if(StateDataPtr->FeeNextFreePageAddr > SectorInfoPtr->NextFreeWLAddr)
    {
      /* move next free word line to its next word line */
      SectorInfoPtr->NextFreeWLAddr += FEE_DFLASH_WORDLINE_SIZE;
    }
  }

}/* End of Fee_lUpdateCache */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lComputeNextWLAddr                      **
**                                     (Fee_SectorInfoType *SectorInfoPtr)    **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) : SectorInfoPtr - Pointer to Sector Info               **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function will confirm the next free WL address  **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lComputeNextWLAddr(Fee_SectorInfoType *SectorInfoPtr)
{
  uint32 index;
  uint32 *SrcPtr;
  Fee_StateDataType  *StateDataPtr;
  Std_ReturnType      ReturnValueFls;
  uint16 Length;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  SrcPtr = (uint32 *)(void *)(StateDataPtr->FeeReadWriteBuffer);
  Length = FEE_DFLASH_WORDLINE_SIZE/FEE_FOUR;

  ReturnValueFls = Fls_17_Pmu_ReadWordsSync(
                   (Fls_AddressType)SectorInfoPtr->NextFreeWLAddr,
                   (uint32 *)(void *)(StateDataPtr->FeeReadWriteBuffer),
                   (Fls_LengthType)(Length));

  /* If read error -> word line is not empty */
  if(ReturnValueFls == E_NOT_OK)
  {
    SectorInfoPtr->NextFreeWLAddr = 
                       Fee_lGetNextWLAddr(SectorInfoPtr->NextFreeWLAddr);
  }
  else
  {
    /* check if word line is empty */
    index = 0U;
    while((index < Length) && (*SrcPtr == (uint32)0))
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
      SrcPtr++;
      index++;
    }
    /* if word line is not empty, move to next word line */
    if(index < Length)
    {
      SectorInfoPtr->NextFreeWLAddr = 
                         Fee_lGetNextWLAddr(SectorInfoPtr->NextFreeWLAddr);
    }
  }
} /* End of Fee_lComputeNextWLAddr */

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE049]                                **
**                                                                            **
** Syntax : IFX_LOCAL_INLINE void Fee_lUpdateCacheTable(                      **
**                                      uint32 BankNumber,                    **
**                                      uint32 EndAddr                        **
**                                    )                                       **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :BankNumber                                            **
**                      EndAddr                                               **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function  will search through the DFLASH from   **
**  the freepage of the sector until the start page of a data block is found  **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lUpdateCacheTable(uint32 BankNumber,
                                           uint32 EndAddr)
{
  Fee_StateDataType       *StateDataPtr;
  uint8                   *ReadWriteBufferPtr;
  uint32                   WordValue[2];
  uint32                   LWBHeaderAddress = 0x00U;
  uint32                   LWBMarkerAddress;
  uint32                   DataNextPageCount;
  uint16                   PageCount;
  uint16                   PageCount1;
  uint16                   BlockNumber;
  uint8                    PageId;
  uint8                    PotentialMarkerPage;
  Std_ReturnType           RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReadWriteBufferPtr = StateDataPtr->FeeReadWriteBuffer;
  PotentialMarkerPage = FEE_ZERO;
  DataNextPageCount = FEE_ZERO;

  do
  {
    /* Check for unerasable WL */
    while(Fee_lIsUnerasableAddr(StateDataPtr->FeeCurrSectSrcAddr))
    {
      StateDataPtr->FeeCurrSectSrcAddr -= FEE_DFLASH_PAGE_SIZE;
      if (StateDataPtr->FeeCurrSectSrcAddr == EndAddr)
      {
        StateDataPtr->FeeCacheState = FEE_CACHE_UPDATE_COMPLETE;
      }
    }

    if (StateDataPtr->FeeCacheState != FEE_CACHE_UPDATE_COMPLETE)
    {
      /* Read the header (2 Words) */
      RetVal = Fls_17_Pmu_ReadWordsSync(
                              (Fls_AddressType)StateDataPtr->FeeCurrSectSrcAddr,
                              (uint32 *)(void *)ReadWriteBufferPtr,
                              (Fls_LengthType)FEE_TWO);
      
      if(StateDataPtr->FeeLastWrittenBlockDirty == FEE_LWB_CHECK_FOR_DIRTY)
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
        if((RetVal == E_NOT_OK) ||
          ((ReadWriteBufferPtr[0] != FEE_DATA_FIRST_PAGE_ID) &&
           (ReadWriteBufferPtr[0] != FEE_DATA_NEXT_PAGE_ID)))
        {
          StateDataPtr->FeeLastWrittenBlockDirty = FEE_LWB_NOT_DIRTY;
        }
      }
      
      if(RetVal == E_NOT_OK)
      {
        PotentialMarkerPage = FEE_ONE;
        DataNextPageCount = FEE_ZERO;
      }
      else
      {
        /* Evaluate the header read */
        PageId = *ReadWriteBufferPtr;
        WordValue[0] = *((uint32 *)(void *)ReadWriteBufferPtr);
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
          access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
        WordValue[1] = *((uint32 *)(void *)(ReadWriteBufferPtr + FEE_FOUR));
      
        /* Check for data continuation page */
        if (WordValue[0] == FEE_DP_MARKER_WORD)
        {
          PotentialMarkerPage = FEE_ONE;
          DataNextPageCount = FEE_ZERO;
          
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          PageCount1= ((uint16)((uint16)(ReadWriteBufferPtr[7]) << FEE_EIGHT) |
                       ReadWriteBufferPtr[6]);
          PageCount = (PageCount1 & (uint16)FEE_PAGE_COUNT_MASK);
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          BlockNumber= ((uint16)((uint16)(ReadWriteBufferPtr[5]) << FEE_EIGHT) |
                       ReadWriteBufferPtr[4]);
          LWBMarkerAddress = StateDataPtr->FeeCurrSectSrcAddr;
          if((StateDataPtr->FeeCurrSectSrcAddr - EndAddr) < (uint32)((uint32)\
                                                       PageCount << FEE_THREE))
          {
            RetVal = E_NOT_OK;
          }           
          else if(StateDataPtr->FeeSectorInfo[BankNumber].UnerasableWLCount == 
                                                                       FEE_ZERO)
          {
            LWBHeaderAddress= LWBMarkerAddress-((uint32)PageCount << FEE_THREE);
          }
          else
          {
            LWBHeaderAddress = Fee_lGetBlockHeaderAddress(LWBMarkerAddress, 
                                                                     PageCount);
          }
          if( RetVal == E_OK )
          {
          

            RetVal = Fls_17_Pmu_ReadWordsSync(
                     (Fls_AddressType)LWBHeaderAddress,
                     (uint32 *)(void *)ReadWriteBufferPtr,
                     (Fls_LengthType)FEE_TWO);
    
            if(RetVal == E_OK)
            {
  
              RetVal = Fee_lEvaluateBlkHeader(BlockNumber, PageCount1);
              if(RetVal == E_OK)
              {
                Fee_lHandleBlkMarkerFound(LWBHeaderAddress, LWBMarkerAddress, 
                                          PageCount);
                PotentialMarkerPage = FEE_ZERO;
              }
            }
          }
          else
          {
            PotentialMarkerPage = FEE_ONE;
            DataNextPageCount = FEE_ZERO;
          }
        }
        else if (PageId == FEE_DATA_NEXT_PAGE_ID)
        {
          DataNextPageCount++;
        }
        /* Check for data first page */
        else if (PageId == FEE_DATA_FIRST_PAGE_ID)
        {
          Fee_lHandleBlkHeaderFound(PotentialMarkerPage, DataNextPageCount);
          DataNextPageCount = FEE_ZERO;
          PotentialMarkerPage = FEE_ZERO;
        }
        else
        {
          DataNextPageCount = FEE_ZERO;
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          if(((WordValue[0] == (uint32)0) && (WordValue[1] == (uint32)0)) ||
             (ReadWriteBufferPtr[0] == FEE_STATE_FIRST_PAGE_ID))
          {
            PotentialMarkerPage = FEE_ZERO;
          }
          else
          {
            PotentialMarkerPage = FEE_ONE;
          }
        }
      }
      Fee_lCheckPrevPage(EndAddr);
    }
  }while(StateDataPtr->FeeCacheState == FEE_CACHE_FIND_NEXT_BLK);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE Std_ReturnType Fee_lEvaluateBlkHeader            **
**                                               (uint16 BlockNumber,         **
**                                                uint16 BlockPageCount)      **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber - Logical block number                      **
**                    BlockPageCount - Page Count of the corresponding block  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description : This function evaluates block header page with respect to    **
**               block marker page                                            **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fee_lEvaluateBlkHeader(uint16 BlockNumber, 
                                                      uint16 BlockPageCount)
{
  Fee_StateDataType          *StateDataPtr;
  uint8                      *ReadWriteBufferPtr;
  uint16                      BlockNumber1;
  uint16                      PageCount1;
  Std_ReturnType              RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReadWriteBufferPtr = StateDataPtr->FeeReadWriteBuffer;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  BlockNumber1 = (uint16)((uint16)ReadWriteBufferPtr[2] << FEE_EIGHT) | 
                                 ReadWriteBufferPtr[1];
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  PageCount1   = (uint16)((uint16)ReadWriteBufferPtr[7] << FEE_EIGHT) |
                                 ReadWriteBufferPtr[6];

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  if((BlockNumber == BlockNumber1) && 
     (BlockPageCount == PageCount1) &&
     (ReadWriteBufferPtr[0] == FEE_DATA_FIRST_PAGE_ID))
  {
    RetVal = E_OK;
  }
  else
  {
    RetVal = E_NOT_OK;
  }
  
  return (RetVal);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint32 Fee_lGetBlockHeaderAddress                **
**                                           (uint32 LWBMarkerAddress,        **
**                                            uint16 PageCount)               **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  LWBMarkerAddress - Last Written Block's Marker Address  **
**                    PageCount - Page Count of the corresponding block       **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint32 - Block header address                           **
**                                                                            **
** Description : This function returns the address of the block's header      **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fee_lGetBlockHeaderAddress(uint32 LWBMarkerAddress,
                                                  uint16 PageCount)
{
  uint32 BlockSize;

  BlockSize = (uint32)PageCount << FEE_THREE;

  do
  {
    LWBMarkerAddress -= FEE_DFLASH_PAGE_SIZE;
    if(Fee_lIsUnerasableAddr(LWBMarkerAddress) == (boolean)FALSE)
    {
      BlockSize -= FEE_DFLASH_PAGE_SIZE;
    }
  }while(BlockSize > FEE_ZERO);

  return (LWBMarkerAddress);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleBlkMarkerFound                   **
**                                        (uint32 LWBHeaderAddress,           **
**                                         uint32 LWBMarkerAddress,           **
**                                         uint16 PageCount)                  **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  LWBHeaderAddress - Last Written Block's Header Address  **
**                    LWBMarkerAddress - Last Written Block's Marker Address  **
**                    PageCount - Page Count of the corresponding block       **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This function does the cache table update and other necessary**
**               handling when the block's marker is found during cache build **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleBlkMarkerFound(uint32 LWBHeaderAddress,
                                               uint32 LWBMarkerAddress,
                                               uint16 PageCount)
{
  Fee_StateDataType          *StateDataPtr;
  Fee_LastWrittenBlkInfoType *LastWrittenBlkPtr;
  uint8                      *ReadWriteBufferPtr;
  Fee_CacheStatusType         Status;
  uint32                      NextFreePageWLAddr;
  uint16                      BlockNumber;
  uint16                      Index;

  Status.Valid = FEE_ZERO;
  Status.Consistent = FEE_ZERO;
  Status.Copied = FEE_ZERO;
  Status.PrevCopyValid = FEE_ZERO;
  Status.PrevCopyConsistent = FEE_ZERO;
  Status.PrevCopyCopied = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReadWriteBufferPtr = StateDataPtr->FeeReadWriteBuffer;
  LastWrittenBlkPtr = &(StateDataPtr->FeeLastWrittenBlkInfo);
  StateDataPtr->FeeCurrSectSrcAddr = LWBHeaderAddress;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  BlockNumber = (uint16)((uint16)ReadWriteBufferPtr[2] << FEE_EIGHT) | 
                                                         ReadWriteBufferPtr[1];
  Index = Fee_lGetUsrBlockIndex(BlockNumber);

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  if((ReadWriteBufferPtr[7] & FEE_VALID_MASK) != FEE_ZERO)
  {
    Status.Valid = FEE_ONE;
  }
  else
  {
    Status.Valid = FEE_ZERO;
  }
  Status.Consistent = FEE_ONE;
  
  /* Updated cache table */
  if ((Index != FEE_LOG_BLOCK_NOT_FOUND))
  {
    if ((StateDataPtr->FeeBlockInfo[Index].Address == FEE_ZERO) 
        #if (FEE_GET_PREV_DATA_API == STD_ON)
        || (StateDataPtr->FeeBlockInfo[Index].PrevCopyAddress == FEE_ZERO)
        #endif
       )
    {
      Fee_lUpdateBlockInfo(Status, Index);
    }
  }
  else
  {
    Fee_lUpdateUnconfigBlockInfo(BlockNumber);
  }
  
  /* Updated last written block info, if not done */
  if(LastWrittenBlkPtr->BlockNumber == FEE_ZERO)
  {
    NextFreePageWLAddr = Fee_lGetWLBeginning(StateDataPtr->FeeNextFreePageAddr);
    if((LWBHeaderAddress < NextFreePageWLAddr) && 
       (LWBMarkerAddress >= NextFreePageWLAddr))
    {
      LastWrittenBlkPtr->PageCount = PageCount + FEE_ONE;
      LastWrittenBlkPtr->Addr = LWBHeaderAddress;
      LastWrittenBlkPtr->BlockNumber = BlockNumber;
      LastWrittenBlkPtr->Status.Consistent = FEE_ONE;
    }
    else if (LWBMarkerAddress < NextFreePageWLAddr)
    {
      LastWrittenBlkPtr->BlockNumber = FEE_16_BIT_ALL_ONES;
    }
    else
    {
      /* dummy else */
    }
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lHandleBlkHeaderFound                   **
**                                        (uint8 PotentialMarkerPage,         **
**                                         uint32 DataNextPageCount)          **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : PotentialMarkerPage - Potential Marker Page was found or **
**                                         not                                **
**                   DataNextPageCount - Count of Data block's continuation   **
**                                       page                                 **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This function does the cache table update and other necessary**
**               handling when the block's header is found during cache build **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lHandleBlkHeaderFound(uint8 PotentialMarkerPage,
                                               uint32 DataNextPageCount)
{
  Fee_StateDataType          *StateDataPtr;
  Fee_LastWrittenBlkInfoType *LastWrittenBlkPtr;
  uint8                      *ReadWriteBufferPtr;
  Fee_CacheStatusType         Status;
  uint16                      BlockNumber;
  uint16                      Index;
  uint16                      PageCount;

  Status.Valid = FEE_ZERO;
  Status.Consistent = FEE_ZERO;
  Status.Copied = FEE_ZERO;
  Status.PrevCopyValid = FEE_ZERO;
  Status.PrevCopyConsistent = FEE_ZERO;
  Status.PrevCopyCopied = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReadWriteBufferPtr = StateDataPtr->FeeReadWriteBuffer;
  LastWrittenBlkPtr = &(StateDataPtr->FeeLastWrittenBlkInfo);
  
  /* extract page count */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  PageCount = (uint16)((uint16)ReadWriteBufferPtr[7] << FEE_EIGHT) | 
                                                         ReadWriteBufferPtr[6];
  PageCount = (PageCount & (uint16)FEE_PAGE_COUNT_MASK) - FEE_ONE;
  
  /* extract block number */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  BlockNumber = (uint16)((uint16)ReadWriteBufferPtr[2] << FEE_EIGHT) | 
                                                         ReadWriteBufferPtr[1];
  Index = Fee_lGetUsrBlockIndex(BlockNumber);
  
  /* If potential marker page was found and page count matches then, update
     cache table */
  if((PotentialMarkerPage != FEE_ZERO) && (PageCount == DataNextPageCount))
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    if((ReadWriteBufferPtr[7] & FEE_VALID_MASK) != FEE_ZERO)
    {
      Status.Consistent = FEE_ONE;
      Status.Valid = FEE_ONE;
    }
    else
    {
      Status.Consistent = FEE_ONE;
      Status.Valid = FEE_ZERO;
    }
    if ((Index != FEE_LOG_BLOCK_NOT_FOUND))
    {
      if ((StateDataPtr->FeeBlockInfo[Index].Address == FEE_ZERO) 
          #if (FEE_GET_PREV_DATA_API == STD_ON)
          || (StateDataPtr->FeeBlockInfo[Index].PrevCopyAddress == FEE_ZERO)
          #endif
         )
      {
        Fee_lUpdateBlockInfo(Status, Index);
      }
    }
    else
    {
      Fee_lUpdateUnconfigBlockInfo(BlockNumber);
    }
    /* Update last written block info, if not done before */
    if(LastWrittenBlkPtr->BlockNumber == FEE_ZERO)
    {
      LastWrittenBlkPtr->BlockNumber = FEE_16_BIT_ALL_ONES;
    }
  }
  else
  {
    if(StateDataPtr->FeeLastWrittenBlockDirty == FEE_LWB_CHECK_FOR_DIRTY)
    {
      if(PageCount == DataNextPageCount)
      {
        StateDataPtr->FeeLastWrittenBlockDirty = FEE_LWB_DIRTY;
      }
      else
      {
        StateDataPtr->FeeLastWrittenBlockDirty = FEE_LWB_NOT_DIRTY;
      }
    }
    /* AI00194692 - Do not update cache table as block is inconsistent */
    /*            - Continue the search for consistent copy of the block */
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lCheckPrevPage(uint32 EndAddr)          **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :EndAddr - BaseAddr of the sector                      **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This utility function will check if the CurrSectSrcAddr      **
**  has reached the base address. If not, it decrements the CurrSectSrcAddr   **
**  by page size                                                              **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lCheckPrevPage(uint32 EndAddr)
{
  Fee_StateDataType       *StateDataPtr;
  
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Check if all pages have been read */
  if( StateDataPtr->FeeCurrSectSrcAddr != EndAddr )
  {
    /* Move to the next page */
    StateDataPtr->FeeCurrSectSrcAddr -= FEE_DFLASH_PAGE_SIZE;
  }
  else
  {
    /* Goto the next state */
    StateDataPtr->FeeCacheState = FEE_CACHE_UPDATE_COMPLETE;    
  }
}

/*******************************************************************************
** Syntax : static uint32 Fee_lGetBlkCycleCount(uint16 Index)                 **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  Index      Cache Index for targeted logical block       **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Return value    :  uint32     Fetches the cycle count for the logical block**
**                                                                            **
** Description : This utility function fetches the cycle count for a          **
**               logical block from DFLASH.                                   **
*******************************************************************************/
static uint32 Fee_lGetBlkCycleCount(uint16 Index)
{
  Fee_StateDataType  *StateDataPtr;
  uint8              *TempArrayPtr;
  uint32              Address;
  uint32              BlockCycleCount;
  uint32              TempVal;
  Std_ReturnType      RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  BlockCycleCount = FEE_ZERO;

  /* Initialize the temp array */
  StateDataPtr->FeeTempArray[0U] = (uint32)FEE_ZERO;
  StateDataPtr->FeeTempArray[1U] = (uint32)FEE_ZERO;

  /* Read block cycle count */
  Address = (StateDataPtr->FeeBlockInfo[Index]).Address;
  if( Address != FEE_ZERO )
  {
    /* Read the header (2 Words) */
    RetVal = Fls_17_Pmu_ReadWordsSync((Fls_AddressType)Address,
                            (uint32 *)(void *)(StateDataPtr->FeeTempArray),
                            (Fls_LengthType)FEE_TWO);
    if( RetVal != E_OK )
    {
      BlockCycleCount = FEE_32_BIT_ALL_ONES;
    }
    else
    {
      /* Block Cycle count is 24 bits starting from 3rd byte of FeeTempArray.
         Read the word from second byte and extract the upper 24bits */
      TempArrayPtr = (uint8 *)(void *)(StateDataPtr->FeeTempArray);
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
        access the data within FeeTempArray[2U]*/
      TempArrayPtr += FEE_TWO;
      TempVal = *((uint32 *)(void *)TempArrayPtr);
      BlockCycleCount = (TempVal >> FEE_EIGHT);
    }
  }

  return (BlockCycleCount);

}/*End of Fee_lGetBlkCycleCount */


/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lCheckPrevWordline(                    **
**                                       Fee_SectorInfoType *SectorInfoPtr,   **
**                                       uint32 EndAddr                       **
**                                      )                                     **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :SectorInfoPtr: Pointer to Sector Info                 **
**                     :EndAddr: Sector base addr                             **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint8                                                   **
**                                                                            **
** Description : This utility function will check if the CurrSectSrcAddr      **
**  has reached the end address. If not, it decrements the CurrSectSrcAddr    **
**  by wordline size. If the end addr is reached, this function computes      **
**  the first free WL address and the sector used status.                     **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fee_lCheckPrevWordline(Fee_SectorInfoType *SectorInfoPtr,
                                    uint32 EndAddr)
{
  Fee_StateDataType   *StateDataPtr;
  uint32               UnerasableWlAddr;
  uint32               NonZeroWlAddr[2];
  uint32               StatePageAddr;
  uint32               NonZeroWlCount;
  uint8                UnerasableWlCount;
  uint8                SectorState;
  uint8                RetVal;

  RetVal = FEE_SP_INIT;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  UnerasableWlAddr = SectorInfoPtr->UnerasableWLAddr;
  UnerasableWlCount = SectorInfoPtr->UnerasableWLCount;
  NonZeroWlAddr[0] = SectorInfoPtr->NonZeroWLAddr[0];
  NonZeroWlAddr[1] = SectorInfoPtr->NonZeroWLAddr[1];
  NonZeroWlCount = SectorInfoPtr->NonZeroWLCount;
  StatePageAddr = SectorInfoPtr->StatePageAddr;
  SectorState = SectorInfoPtr->State;

  /* Check if all pages have been read */
  if ( StateDataPtr->FeeCurrSectSrcAddr == EndAddr )
  {
    /* EndAddr reached - State page search complete */
    RetVal = FEE_SP_DONE;

    /************ Compile NextFreeWL Addr and Sector used state *************/
    if (NonZeroWlCount == FEE_ZERO)
    {
      /* The sector is empty -
         NextFreeWlAddr = BaseAddr, Sector status = Unused (default value) */
      SectorInfoPtr->NextFreeWLAddr = EndAddr;
    }
    else
    {
      /* Updating NextFreeWLAddr of the sector */
      if ( (NonZeroWlAddr[0] == UnerasableWlAddr) &&
           (UnerasableWlCount != FEE_ZERO) )
      {
        SectorInfoPtr->NextFreeWLAddr = Fee_lGetNextWLAddr(NonZeroWlAddr[1]);
        if(SectorInfoPtr->NextFreeWLAddr == UnerasableWlAddr)
        {
          SectorInfoPtr->NextFreeWLAddr = 
                           Fee_lGetNextWLAddr(SectorInfoPtr->NextFreeWLAddr);
        }
      }
      else
      {
        SectorInfoPtr->NextFreeWLAddr = Fee_lGetNextWLAddr(NonZeroWlAddr[0]);
      }

      /* Refer UTP AI00238899 - Don't verify if sector end is reached */
      if((SectorInfoPtr->NextFreeWLAddr) < (EndAddr + FEE_DFLASH_SECTOR_SIZE))
      {
        /* Refer UTP AI00194703 - Identify/confirm next empty word line */
        Fee_lComputeNextWLAddr(SectorInfoPtr);
      }

      /* Determine Sector Used state */
      if (SectorState == FEE_ERASED)
      {
        if ( (NonZeroWlAddr[0] == StatePageAddr) ||
             ((NonZeroWlAddr[1] == StatePageAddr) &&
              (NonZeroWlAddr[0] == UnerasableWlAddr)
             )
           )
        {
          /* Sector is unused (Default value) */
        }
        else
        {
          SectorInfoPtr->Status.Used = FEE_ONE;
        } /* if ( (NonZeroWlAddr[0] == StatePageAddr) || .... */
      } /* if (SectorState == FEE_ERASED) */
      else
      {
          SectorInfoPtr->Status.Used = FEE_ONE;
      }
    } /* if (NonZeroWlCount == FEE_ZERO) */
  }
  else
  {
    /* Move to the previos wordline */
    StateDataPtr->FeeCurrSectSrcAddr -= FEE_DFLASH_WORDLINE_SIZE;
  }

  return (RetVal);

}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lEvaluateStatePageFormat(void)         **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  FEE_STATE_OLD_FORMAT: if the state page is of Old Format**
**                    FEE_STATE_NEW_FORMAT: if the state page is of New Format**
**                    FEE_STATE_INVALID_FORMAT: if the state page is unknown  **
**                                              Format                        **
**                                                                            **
** Description : This utility function will evaluate the state page content   **
**  and returns the state page status                                         **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fee_lEvaluateStatePageFormat(void)
{
  Fee_StateDataType *StateDataPtr;
  uint32             Format[2];

  uint32             StatePageFormat;
  uint8             *BufferPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
    efficiently access the data within 
    FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  BufferPtr = StateDataPtr->FeeReadWriteBuffer + FEE_STATE_ALLONES_OFFSET;
  
  Format[0] = *((uint32*)(void *)(BufferPtr));
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
    efficiently access the data within 
    FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  Format[1] = *((uint32*)(void *)(BufferPtr + FEE_FOUR));
  
  if( (Format[0] == FEE_SP_ALL_ONES_C6) &&
           (Format[1] == FEE_32_BIT_ALL_ONES))
  {
    StatePageFormat = FEE_STATE_NEW_FORMAT;
  }
  else if( (Format[0] == FEE_ZERO) && (Format[1] == FEE_ZERO))
  {
    StatePageFormat = FEE_STATE_OLD_FORMAT;
  }  
  else
  {
    StatePageFormat = FEE_STATE_INVALID_FORMAT;
  }
  return(StatePageFormat);
}
/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lEvaluateStatePageMarker(void)         **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  FEE_SP_CLEAN: if the State Page Marker is Valid         **
**                    FEE_SP_DIRTY: if the State Page Marker is Non Zero      **
**                    FEE_SP_INVALID: if the State Page Marker is Zero        **
**                                                                            **
** Description : This utility function will evaluate the state page Marker    **
**  content and returns the status                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fee_lEvaluateStatePageMarker(void)
{
  Fee_StateDataType *StateDataPtr;
  uint8             *BufferPtr;
  uint32             Marker[2];
  Std_ReturnType     FlsRetVal;
  uint8              ReturnVal;
  
  ReturnVal = FEE_SP_CLEAN;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  
  /* Read the marker (last 8 bytes) of the current page */
  /* Offset will be 120 */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
    efficiently access the data within 
    FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  BufferPtr = StateDataPtr->FeeReadWriteBuffer + FEE_SP_MARKERFIELD_START;
  FlsRetVal = Fls_17_Pmu_ReadWordsSync(
              (Fls_AddressType)(StateDataPtr->FeeCurrSectSrcAddr + \
               FEE_SP_MARKERFIELD_START),
              (uint32 *)(void *)(BufferPtr),
      (Fls_LengthType)(FEE_STATEPAGE_MARKER_SIZE/FEE_DFLASH_WORD_SIZE));
  
  if(FlsRetVal == E_NOT_OK)
  {
    ReturnVal = FEE_SP_DIRTY;
  }
  else
  {
    Marker[0] = *((uint32*)(void *)(BufferPtr));
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
      efficiently access the data within 
      FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    Marker[1] = *((uint32*)(void *)(BufferPtr + FEE_DFLASH_WORD_SIZE));
    if((Marker[0] == FEE_ZERO) && (Marker[1] == FEE_ZERO))
    {
      ReturnVal = FEE_SP_INVALID;
    }
    else
    {
      /* if ReturnVal is FEE_SP_CLEAN */
      if ((Marker[0] != FEE_SP_MARKER_WORD_1) ||
          (Marker[1] != FEE_SP_MARKER_WORD_2))
      {
        ReturnVal = FEE_SP_DIRTY;
      }
    }
  }
  return (ReturnVal);
}
/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lEvaluateOldStatePage(void)            **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  FEE_SP_CLEAN: if the state page is of Old Format        **
**                    FEE_SP_INVALID: if the state page is invalid            **
**                                                                            **
** Description : This utility function will evaluate the state page content   **
** with respect to old format and returns the status                          **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fee_lEvaluateOldStatePage(void)
{
  Fee_StateDataType *StateDataPtr;
  uint8             *BufferPtr;
  uint16             Offset;
  uint8              ReturnVal;
  
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  ReturnVal = FEE_SP_CLEAN;
  BufferPtr = StateDataPtr->FeeReadWriteBuffer;
  
  
  /* Check the first zero field */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  if(*(BufferPtr + FEE_TWO) == FEE_ZERO)
  {
    Offset = FEE_SP_ZEROFIELD1_START;
  }
  else
  {
    Offset = FEE_SP_ZEROFIELD2_START;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    if(*((uint32 *)(void *)(BufferPtr + FEE_EIGHT)) != FEE_SP_PAGE2_WORD_1)
    {
      ReturnVal = FEE_SP_INVALID;
    }
  }
  if(FEE_SP_INVALID != ReturnVal)
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    if( *(BufferPtr + FEE_THREE) != FEE_ZERO)
    {
      ReturnVal = FEE_SP_INVALID;
    }
    else
    {
      while ((ReturnVal == FEE_SP_CLEAN) && \
            (Offset < FEE_SP_ZEROFIELD1_END))
      {
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
          efficiently access the data within 
          FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
        if( *((uint32 *)(void *)(BufferPtr + Offset)) != FEE_ZERO)
        {
          ReturnVal = FEE_SP_INVALID;
        }
        else
        {
          Offset += FEE_FOUR;
        }
      }
    }
  }
  return(ReturnVal);
}
/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lEvaluateNewStatePage(void)            **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  FEE_SP_CLEAN: if the state page is of New Format        **
**                    FEE_SP_INVALID: if the state page is invalid            **
**                                                                            **
** Description : This utility function will evaluate the state page content   **
**  with respect to New Format and returns the status                         **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fee_lEvaluateNewStatePage(void)
{
  Fee_StateDataType *StateDataPtr;
  uint32             *BufferPtr;
  uint32             *BufferPtr2;
  uint32             Data;
  uint32             Index;
  uint32             Length;
  uint8              ReturnVal;
  
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  ReturnVal = FEE_SP_CLEAN;
  BufferPtr = (uint32*)(void*)(StateDataPtr->FeeReadWriteBuffer);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
    access user data buffer efficiently. User ensures the 
    availability of sufficient buffer data space for the requested
    operation. 
    Pointer arithmetic is used to efficiently access the data 
    within FeeReadWriteBuffer[FEE_STATE_FIRSTFOUR_COPY_OFFSET]*/
  BufferPtr2 = (uint32*)(void*)(StateDataPtr->FeeReadWriteBuffer + \
               (FEE_STATE_FIRSTFOUR_COPY_OFFSET));

  /* check that the first word content is redundant below except page id */
  if((*BufferPtr & FEE_STATE_WLCOUNT_MASK) != \
     (*BufferPtr2 & FEE_STATE_WLCOUNT_MASK))
  {
    ReturnVal = FEE_SP_INVALID;
  }
  else
  {
    /* first four page content should be duplicated. Below check is for that */
    for(Index = FEE_ZERO; Index < FEE_SEVEN; Index++)
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
        access user data buffer efficiently. User ensures the 
        availability of sufficient buffer data space for the requested
        operation. 
        Pointer arithmetic is used to efficiently access the data 
        within FeeReadWriteBuffer*/
      BufferPtr++;
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
        access user data buffer efficiently. User ensures the 
        availability of sufficient buffer data space for the requested
        operation. 
        Pointer arithmetic is used to efficiently access the data 
        within FeeReadWriteBuffer*/      
      BufferPtr2++; 
            
      if(*BufferPtr != *BufferPtr2)
      {
        ReturnVal = FEE_SP_INVALID;
        break; /* break */
      }
   
    }
    /* Check for second half of state page */
    if(ReturnVal != FEE_SP_INVALID)
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
        access ReadWrite buffer efficiently. User ensures the 
        availability of sufficient buffer data space for the requested
        operation. 
        Pointer arithmetic is used to efficiently access the data 
        within FeeReadWriteBuffer[FEE_STATE_ALLONES_OFFSET]*/      
      BufferPtr = (uint32*)(void*)(StateDataPtr->FeeReadWriteBuffer + 
                            FEE_STATE_ALLONES_OFFSET);
      /* get the length excluding CRC */
      Length = ( (sizeof(Fee_StateHalf))/(sizeof(Fee_StateHalf[0])) ) - 1U;
      
      for(Index = FEE_ZERO;Index < Length;Index++)
      {
        if(*BufferPtr != Fee_StateHalf[Index])
        {
          ReturnVal = FEE_SP_INVALID;
          break; /* break */
        }
        /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
          access user data buffer efficiently. User ensures the 
          availability of sufficient buffer data space for the requested
          operation. 
          Pointer arithmetic is used to efficiently access the data 
          within FeeReadWriteBuffer*/        
        BufferPtr++;
      }
    }
    /* verify the CRC */
    if(ReturnVal != FEE_SP_INVALID)
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
        access user data buffer efficiently. User ensures the 
        availability of sufficient buffer data space for the requested
        operation. 
        Pointer arithmetic is used to efficiently access the data 
        within FeeReadWriteBuffer*/
      BufferPtr = (uint32*)(void*)(StateDataPtr->FeeReadWriteBuffer + \
                  FEE_STATE_CRC_OFFSET);
      /* Length will give the number of pages to Generate CRC */
      Length = FEE_STATE_CRC_OFFSET/FEE_DFLASH_WORD_SIZE;                  
      Data = Fee_lEvaluateStatePageCrc(
             (uint32*)(void*)(StateDataPtr->FeeReadWriteBuffer),Length);
      if( Data != *BufferPtr)
      {
        ReturnVal = FEE_SP_INVALID;
      }
        
    }
  }
  return(ReturnVal);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint32 Fee_lEvaluateStatePageCrc                 **
**            (uint32 * DataPtr, uint32 Length)                               **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  DataPtr : Pointer to Data                               **
**                    Length : Data Length in Word                            **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  32 bit CRC value                                        **
**                                                                            **
** Description : This utility function will evaluate 32 bit CRC value over    **
**               data pointed by DataPtr for the requested Length             **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Fee_lEvaluateStatePageCrc(uint32 * DataPtr,
                 uint32 Length)
{
  uint32             Index;
  uint32             Crc32;
  uint32             *BufferPtr;

  
  BufferPtr = DataPtr;
  Crc32 = FEE_CRC_START_VAL;

  
  for(Index = FEE_ZERO;Index < Length;Index++)
  {
    Crc32 = CRC32(Crc32, *BufferPtr);
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
      access user data buffer efficiently. Fee ensures the 
      availability of sufficient buffer data space for the requested
      operation. 
      Pointer arithmetic is used to efficiently access the data 
      within DataPtr */
    BufferPtr++;
  }
  return(Crc32);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lEvaluateStatePage(void)               **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  FEE_SP_CLEAN: if the state page is valid                **
**                    FEE_SP_DIRTY: if the state page is dirty                **
**                    FEE_SP_INVALID: if the state page is invalid            **
**                                                                            **
** Description : This utility function will evaluate the state page content   **
**  and returns the state page status                                         **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fee_lEvaluateStatePage(void)
{
  Fee_StateDataType *StateDataPtr;
  uint8             *BufferPtr;
  Std_ReturnType     FlsRetVal;
  uint8              State;
  uint8              ReturnVal;
  uint32             StatePageFormat;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Read the state page content (first 120 bytes) of the current page */
  FlsRetVal = Fls_17_Pmu_ReadWordsSync(
                (Fls_AddressType)StateDataPtr->FeeCurrSectSrcAddr,
                (uint32 *)(void *)StateDataPtr->FeeReadWriteBuffer,
                (Fls_LengthType)(FEE_STATEPAGE_DATA_SIZE/FEE_DFLASH_WORD_SIZE));
  BufferPtr = StateDataPtr->FeeReadWriteBuffer;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  State = *(BufferPtr + FEE_ONE);

  if ( (FlsRetVal == E_NOT_OK) ||
       (!((State == FEE_VALID)||(State == FEE_ERASED))) )
  {
    ReturnVal = FEE_SP_INVALID;
  }
  else
  {
    StatePageFormat = Fee_lEvaluateStatePageFormat();
    
    if(StatePageFormat == FEE_STATE_NEW_FORMAT)
    {
      ReturnVal = Fee_lEvaluateNewStatePage();
    }
    else if(StatePageFormat == FEE_STATE_OLD_FORMAT)
    {
      ReturnVal = Fee_lEvaluateOldStatePage();
      
    }
    else
    {
      ReturnVal = FEE_SP_INVALID;     
    }   
  
    if(FEE_SP_INVALID != ReturnVal)
    {
      /* Check the marker field */
      ReturnVal = Fee_lEvaluateStatePageMarker();      
    }
        
  }        
  return (ReturnVal);

}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lSetInitGc(uint8 DFlashStatus)          **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :DFlashStatus: State of DFlash sectors                 **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function sets the InitGc state based on         **
**  the DFlashStatus                                                          **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lSetInitGc(uint8 DFlashStatus)
{

  Fee_StateDataType  *StateDataPtr;
  Fee_SectorInfoType *SectorInfoPtr0;
  Fee_SectorInfoType *SectorInfoPtr1;
  uint32              S0StateCnt;
  uint32              S1StateCnt;
  uint8               S0Used;
  uint8               S1Used;
  uint8               S0Dirty;
  uint8               S1Dirty;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  SectorInfoPtr0 = &StateDataPtr->FeeSectorInfo[0];
  SectorInfoPtr1 = &StateDataPtr->FeeSectorInfo[1];

  StateDataPtr->FeeInitGCState = FEE_INITGC_NORMAL;

  S0StateCnt = SectorInfoPtr0->StateCount;
  S1StateCnt = SectorInfoPtr1->StateCount;
  S0Used = SectorInfoPtr0->Status.Used;
  S1Used = SectorInfoPtr1->Status.Used;
  S0Dirty = SectorInfoPtr0->Status.Dirty;
  S1Dirty = SectorInfoPtr1->Status.Dirty;

  /* Update the state count */
  if (S0StateCnt > S1StateCnt)
  {
    StateDataPtr->FeeStateCount = S0StateCnt;
  }
  else
  {
    StateDataPtr->FeeStateCount = S1StateCnt;
  }

  switch (DFlashStatus)
  {
    /* Both the sectors empty */
    case FEE_S0_S1_EMPTY:
    {
      StateDataPtr->FeeInitGCState = FEE_INITGC_PREPARE_DFLASH;
      StateDataPtr->FeePrepDFLASHState = FEE_PREPDF_MARK_S0_ERASED_SP;
      StateDataPtr->FeeCurrSectSrcAddr =
              StateDataPtr->FeeSectorInfo[0].NextFreeWLAddr;
      S0Dirty = FEE_ZERO;
      S1Dirty = FEE_ZERO;
      break;
    }
    /* S0 is valid */
    case FEE_S0_VALID:
    {
      /* Check if copy was interrupted */
      if(S1Used == FEE_ONE)
      {
        StateDataPtr->FeeGcResumeState = FEE_INITGC_DATA_COPY_S0_S1;
        S1Dirty = FEE_ZERO;
      }
      else
      {
        StateDataPtr->FeeGcResumeState = FEE_INITGC_NORMAL_S0;
      }
      break;
    }
    /* S1 is valid */
    case FEE_S1_VALID:
    {
      /* Change the current sector to SECTOR1 */
      StateDataPtr->FeeStatusFlags.FeeCurrSector = FEE_ONE;
      /* Check if copy was interrupted */
      if(S0Used == FEE_ONE)
      {
        StateDataPtr->FeeGcResumeState = FEE_INITGC_DATA_COPY_S1_S0;
        S0Dirty = FEE_ZERO;
      }
      else
      {
        StateDataPtr->FeeGcResumeState = FEE_INITGC_NORMAL_S1;
      }
      break;
    }
    /* Erase interrupted */
    case FEE_S0_S1_VALID:
    {
      if (S0StateCnt > S1StateCnt)
      {
        StateDataPtr->FeeGcResumeState = FEE_INITGC_ERASE_S1;
        S1Dirty = FEE_ZERO;
      }
      else
      {
        /* Change the current sector to SECTOR1 */
        StateDataPtr->FeeStatusFlags.FeeCurrSector = FEE_ONE;
        StateDataPtr->FeeGcResumeState = FEE_INITGC_ERASE_S0;
        S0Dirty = FEE_ZERO;
      }
      break;
    }
    default:
    {
      S0Dirty = FEE_ZERO;
      S1Dirty = FEE_ZERO;
      StateDataPtr->FeeInitGCState = FEE_INITGC_HANDLE_INVALID;
      break;
    }
  }

  if ( (S0Dirty != FEE_ZERO) || (S1Dirty != FEE_ZERO) )
  {
    StateDataPtr->FeeInitGCState = FEE_INITGC_HANDLE_DIRTY;
    StateDataPtr->FeeRepairStep = FEE_PROGRAM_MARKER;
  }

  SectorInfoPtr0->Status.Dirty = S0Dirty;
  SectorInfoPtr1->Status.Dirty = S1Dirty;

}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lBuildDFlashStatus(void)               **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  DFlash status                                           **
**                                                                            **
** Description : This utility function compiles the DFLASH status based on    **
**  sector0 and sector1 state contents                                        **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fee_lBuildDFlashStatus(void)
{
  Fee_StateDataType *StateDataPtr;
  uint8              S0State;
  uint8              S1State;
  uint8              S0Used;
  uint8              S1Used;
  uint8              RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  S0State = StateDataPtr->FeeSectorInfo[0].State;
  S1State = StateDataPtr->FeeSectorInfo[1].State;
  S0Used = StateDataPtr->FeeSectorInfo[0].Status.Used;
  S1Used = StateDataPtr->FeeSectorInfo[1].Status.Used;

  if ( (S0State == FEE_VALID) && (S1State == FEE_ERASED) )
  {
    RetVal = FEE_S0_VALID;
  }
  else if ( (S0State == FEE_ERASED) && (S1State == FEE_VALID) )
  {
    RetVal = FEE_S1_VALID;
  }
  else if ( (S0State == FEE_VALID) && (S1State == FEE_VALID) )
  {
    /* This happens when Erase of S0 / S1 is interrupted just before the start
       of Erase */
    RetVal = FEE_S0_S1_VALID;
  }
  else if ( (S0State == FEE_VALID) && (S1State == FEE_STATELESS) )
  {
    /* This happens when Erase of S1 is interrupted. Handle it the same way as
       S0_S1_VALID so that S1 can be erased as part of INIT GC */
    RetVal = FEE_S0_S1_VALID;
  }
  else if ( (S0State == FEE_STATELESS) && (S1State == FEE_VALID) )
  {
    /* This happens when Erase of S0 is interrupted. Handle it the same way as
       S0_S1_VALID so that S0 can be erased as part of INIT GC */
    RetVal = FEE_S0_S1_VALID;
  }
  else if ( ((boolean)TRUE == Fee_lIsSectorEmpty(S0State, S0Used)) &&
            ((boolean)TRUE == Fee_lIsSectorEmpty(S1State, S1Used)) )
  {
    RetVal = FEE_S0_S1_EMPTY;
  }
  /* Any other combination leads to invalid state */
  else
  {
    RetVal = FEE_S0_S1_INVALID;
  }

  return (RetVal);

}

/*******************************************************************************
** Syntax : static void Fee_lUpdateNonZeroWLInfo(                             **
**                                         Fee_SectorInfoType *SectorInfoPtr, **
**                                         uint32              SrcAddr        **
**                                       )                                    **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :SectInfoPtr: Pointer to Sector Info                   **
**                     :SrcAddr: Current non zero page addr                   **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function will update the NonZeroWLAddr and      **
**  NonZeroWLCount                                                            **
*******************************************************************************/
static void Fee_lUpdateNonZeroWLInfo(Fee_SectorInfoType *SectorInfoPtr,
                                                  uint32 SrcAddr)
{
  uint32 NonZeroWlCount;
  NonZeroWlCount = SectorInfoPtr->NonZeroWLCount;
  /* Update the non-zero WL location if not updated */
  if (NonZeroWlCount <= FEE_ONE)
  {
    SectorInfoPtr->NonZeroWLAddr[NonZeroWlCount] = SrcAddr;
  }

  /* Increment non-zero WL count */
  SectorInfoPtr->NonZeroWLCount++;
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint8 Fee_lGetSectorInfo (                       **
**                                     uint32 BankNumber,                     **
**                                     uint32 EndAddr                         **
**                                   )                                        **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :BankNumber: Sector number                             **
**                      EndAddr: Base of Sector                               **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  uint8                                                   **
**                                                                            **
** Description : This utility function will search through the DFLASH from    **
**  the bottom till base of the sector and gets sector state information      **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Fee_lGetSectorInfo(uint32 BankNumber,
                                         uint32 EndAddr)
{
  Fee_StateDataType   *StateDataPtr;
  Fee_SectorInfoType  *SectorInfoPtr;
  uint32              *BuffPtr;
  uint32               SrcAddr;
  uint32               StateCount1;
  uint32               StateCount2;
  Std_ReturnType       RetVal;
  uint8                ReturnVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  SectorInfoPtr = &StateDataPtr->FeeSectorInfo[BankNumber];

  SrcAddr = StateDataPtr->FeeCurrSectSrcAddr;

  /* Read the header (2 Words) */
  RetVal = Fls_17_Pmu_ReadWordsSync((Fls_AddressType)SrcAddr,
                           (uint32 *)(void *)StateDataPtr->FeeReadWriteBuffer,
                                    (Fls_LengthType)FEE_FOUR);

  /* Check if DBER is found in the header */
  /* DBER is NOT found */
  if(RetVal == E_OK)
  {
    BuffPtr = (uint32 *)(void *)StateDataPtr->FeeReadWriteBuffer;
    /* Check if the header is zero */
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    if ((*BuffPtr == FEE_ZERO) && (*(BuffPtr + FEE_ONE) == FEE_ZERO) && 
        (*(BuffPtr + FEE_TWO)==FEE_ZERO) && (*(BuffPtr + FEE_THREE)==FEE_ZERO))
    {
      /* Zero page */
      /* Move to previous wordline */
    }
    /* Non-zero page */
    else
    {
      Fee_lUpdateNonZeroWLInfo(SectorInfoPtr, SrcAddr);
      /* Check if it is state page */
      if ( StateDataPtr->FeeReadWriteBuffer[0] == FEE_STATE_FIRST_PAGE_ID )
      {
        RetVal = Fee_lEvaluateStatePage();
        if ( (RetVal == FEE_SP_CLEAN) || (RetVal == FEE_SP_DIRTY))
        {
          /* State Count is at byte 4 of the buffer - move Ptr by one word */
          /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
            efficiently access the data within 
            FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
          StateCount2 = *(BuffPtr + FEE_ONE);
          StateCount1 = SectorInfoPtr->StateCount;
          if (StateCount2 > StateCount1)
          {
            SectorInfoPtr->Status.Dirty = RetVal;
            SectorInfoPtr->State =
                     StateDataPtr->FeeReadWriteBuffer[1];
            SectorInfoPtr->UnerasableWLCount =
                     StateDataPtr->FeeReadWriteBuffer[2];
            SectorInfoPtr->StateCount = StateCount2;
            /* Unerasable WL Addr is at byte 12 of the buffer -
               move Ptr by three words */
            /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to 
              efficiently access the data within 
              FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
            SectorInfoPtr->UnerasableWLAddr = *(BuffPtr + FEE_THREE);
            SectorInfoPtr->StatePageAddr = SrcAddr;
          }/* if(StateCount2 > StateCount1) */
        }/* if ((RetVal == FEE_SP_CLEAN) || (RetVal == FEE_SP_DIRTY)) */
      }/* if(StateDataPtr->FeeReadWriteBuffer[0] == FEE_STATE_FIRST_PAGE_ID)*/
    }/* if( (*BuffPtr == FEE_ZERO) && (*(BuffPtr + 1U) == FEE_ZERO) ) */
  }
  else
  {
    Fee_lUpdateNonZeroWLInfo(SectorInfoPtr, SrcAddr);
  }   
    
  ReturnVal = Fee_lCheckPrevWordline(SectorInfoPtr, EndAddr);

  return (ReturnVal);

}

/*******************************************************************************
** Syntax : static boolean Fee_lIsUnerasableAddr(uint32 Addr)                 **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :Addr                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  TRUE: if Addr is part of unerasable WL                  **
**                    FALSE: if Addr is not part of unerasable WL             **
**                                                                            **
** Description : This utility function checks if the Addr is part of          **
** unerasable Wordline                                                        **
*******************************************************************************/
static boolean Fee_lIsUnerasableAddr(uint32 Addr)
{
  Fee_StateDataType *StateDataPtr;
  boolean            RetVal;
  uint8              BankNumber;

  RetVal = (boolean)FALSE;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  
  /* Get the FEE sector index */
  if(Addr >= FEE_DFLASH_BANK1_BASE)
  {
    BankNumber = FEE_ONE;
  }
  else
  {
    BankNumber = FEE_ZERO;
  }
  
  /* Get word line beginning address of the address to be checked */
  Addr = (uint32)(Fee_lGetWLBeginning(Addr));
  
  /* Check if the word line address is un-erasable word line address */
  if ((StateDataPtr->FeeSectorInfo[BankNumber].UnerasableWLCount != FEE_ZERO) &&
      (StateDataPtr->FeeSectorInfo[BankNumber].UnerasableWLAddr == Addr))
  {
    RetVal = (boolean)TRUE;
  }

  return (RetVal);

}

/*******************************************************************************
** Syntax : static void Fee_lPrepareStatePageBuffer(uint8 Bank,               **
**                                                  uint8 SectorState)        **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  Bank: Sector Number                                     **
**                    SectorState: Erased / Valid                             **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This routine will update the write buffer for state page     **
** programming of a sector                                                    **
*******************************************************************************/
static void Fee_lPrepareStatePageBuffer(uint8 Bank,
                                        uint8 SectorState)
{
  uint32              StateCnt;
  uint32              WordlineAddr;
  uint32              Index;
  uint32              Length;
  Fee_StateDataType  *StateDataPtr;
  uint8              *ArrPtr;
  uint8               WordlineCnt;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  Fee_lClearFeeGcReadWriteBuffer();
  ArrPtr = StateDataPtr->FeeGcReadWriteBuffer;

  /* Get the State count */
  StateCnt = StateDataPtr->FeeStateCount;
  StateCnt = StateCnt + FEE_ONE;

  /* Get the UnerasableWL count */
  WordlineCnt = StateDataPtr->FeeSectorInfo[Bank].UnerasableWLCount;

  /* Get the UnerasableWL Addr */
  WordlineAddr = StateDataPtr->FeeSectorInfo[Bank].UnerasableWLAddr;

  /* Update the page id of the 1st page of state page */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  *ArrPtr = FEE_STATE_FIRST_PAGE_ID;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  *(ArrPtr + FEE_ONE) = SectorState;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within
    FeeGcReadWriteBuffer[FEE_STATE_FIRSTFOUR_COPY_OFFSET]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within
    FeeGcReadWriteBuffer[FEE_STATE_FIRSTFOUR_COPY_OFFSET]*/
  *(ArrPtr + FEE_ONE + FEE_STATE_FIRSTFOUR_COPY_OFFSET) = SectorState;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  *(ArrPtr + FEE_TWO) = WordlineCnt;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within
    FeeGcReadWriteBuffer[FEE_STATE_FIRSTFOUR_COPY_OFFSET]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within
    FeeGcReadWriteBuffer[FEE_STATE_FIRSTFOUR_COPY_OFFSET]*/
  *(ArrPtr + FEE_TWO + FEE_STATE_FIRSTFOUR_COPY_OFFSET) = WordlineCnt;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  *((uint32 *)(void *)(ArrPtr + FEE_FOUR)) = StateCnt;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within
    FeeGcReadWriteBuffer[FEE_STATE_FIRSTFOUR_COPY_OFFSET]*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within
    FeeGcReadWriteBuffer[FEE_STATE_FIRSTFOUR_COPY_OFFSET]*/
  *((uint32 *)(void *)(ArrPtr + FEE_FOUR + FEE_STATE_FIRSTFOUR_COPY_OFFSET)) \
   = StateCnt;
   /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
     access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
 
  if(WordlineCnt > FEE_ZERO)
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *((uint32 *)(void *)(ArrPtr + FEE_TWELVE)) = WordlineAddr;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
      access the data within FeeGcReadWriteBuffer*/
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
      access the data within FeeGcReadWriteBuffer*/
    *((uint32 *)(void *)(ArrPtr + FEE_TWELVE + \
    FEE_STATE_FIRSTFOUR_COPY_OFFSET)) = WordlineAddr;
  }
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer*/
  ArrPtr = StateDataPtr->FeeGcReadWriteBuffer + FEE_DFLASH_PAGE_SIZE;
  /* Write all subsequent state page id's */
  for(Index = FEE_ONE;Index < FEE_EIGHT;Index++)
  {
    *ArrPtr = FEE_STATE_NEXT_PAGE_ID;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer*/
    ArrPtr += FEE_EIGHT;
  }
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer*/
  ArrPtr = StateDataPtr->FeeGcReadWriteBuffer + FEE_STATE_ALLONES_OFFSET;
  /* get the length excluding crc from All 1's field */
  Length = FEE_STATE_HALF_WORD_SIZE - 1U;
  
  /* Write second half state page */
  for(Index = FEE_ZERO;Index < Length;Index++)
  {
    *(uint32 *)(void *)(ArrPtr) = Fee_StateHalf[Index];
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer*/
    ArrPtr += FEE_DFLASH_WORD_SIZE;
  }

  /* prepare the CRC field */
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
    access the data within FeeGcReadWriteBuffer*/
  ArrPtr = StateDataPtr->FeeGcReadWriteBuffer + FEE_STATE_CRC_OFFSET;
  /* Length will give the number of pages to Generate CRC */
  Length = FEE_STATE_CRC_OFFSET/FEE_DFLASH_WORD_SIZE;
  *(uint32 *)(void *)(ArrPtr) =
   Fee_lEvaluateStatePageCrc(
   (uint32 *)(void *)StateDataPtr->FeeGcReadWriteBuffer, Length);

  StateDataPtr->FeeStateCount = StateCnt;

}/* End of Fee_lPrepareStatePageBuffer */

/*******************************************************************************
** Syntax : static void Fee_lPrepareMarkerBuffer(void)                        **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) : None                                                 **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function prepares the Write buffer array for    **
**  marker programming of the state page                                      **
*******************************************************************************/
static void Fee_lPrepareMarkerBuffer(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint8              *BufferPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  BufferPtr = StateDataPtr->FeeGcReadWriteBuffer + FEE_STATEPAGE_DATA_SIZE;

  *((uint32*)(void *)(BufferPtr)) = FEE_SP_MARKER_WORD_1;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  *((uint32*)(void *)(BufferPtr+4U)) = FEE_SP_MARKER_WORD_2;
}/* End of Fee_lPrepareMarkerBuffer */

/*******************************************************************************
** Syntax : static void Fee_lClearMarkerBuffer(void)                          **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) : None                                                 **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function clears the marker field of             **
**  write buffer for comparison                                               **
*******************************************************************************/
static void Fee_lClearMarkerBuffer(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint8              *BufferPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  BufferPtr = StateDataPtr->FeeGcReadWriteBuffer + FEE_STATEPAGE_DATA_SIZE;

  *((uint32*)(void *)(BufferPtr)) = (uint32)FEE_ZERO;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  *((uint32*)(void *)(BufferPtr + 4U)) = (uint32)FEE_ZERO;
}/* End of Fee_lClearMarkerBuffer */

/*******************************************************************************
** Syntax : static void Fee_lClearFeeReadWriteBuffer(void)                    **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function clears the user read write buffer      **
*******************************************************************************/
static void Fee_lClearFeeReadWriteBuffer(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint32*             ReadWriteBuffPtr;
  uint16              DataByteCnt;
  uint16              Length;

  DataByteCnt = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  ReadWriteBuffPtr = (uint32 *)(void *)StateDataPtr->FeeReadWriteBuffer;
  Length = FEE_DFLASH_WORDLINE_SIZE/FEE_DFLASH_WORD_SIZE;
  /* Clear the fee read write buffer */
  do
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(ReadWriteBuffPtr + DataByteCnt) = FEE_ZERO;
    DataByteCnt++;
  }while (DataByteCnt != Length);
}

/*******************************************************************************
** Syntax : static void Fee_lClearFeeGcReadWriteBuffer(void)                  **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : This utility function clears the Gc read write buffer        **
*******************************************************************************/
static void Fee_lClearFeeGcReadWriteBuffer(void)
{
  Fee_StateDataType  *StateDataPtr;
  uint32*             GcReadWriteBuffPtr;
  uint16              DataByteCnt;
  uint16              Length;

  DataByteCnt = FEE_ZERO;
  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  GcReadWriteBuffPtr = (uint32 *)(void *)StateDataPtr->FeeGcReadWriteBuffer;
  Length = FEE_DFLASH_WORDLINE_SIZE/FEE_DFLASH_WORD_SIZE;

  /* Clear the fee GC read write buffer */
  do
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
    *(GcReadWriteBuffPtr + DataByteCnt) = FEE_ZERO;
    DataByteCnt++;
  }while (DataByteCnt != Length);
}

/*******************************************************************************
** Syntax : static void Fee_lHandleInitGcFail(void)                           **
**                                                                            **
** Service ID:                                                                **
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
** Description : This function shall handle the fail cases during             **
** InitGC state machine                                                       **
*******************************************************************************/
static void Fee_lHandleInitGcFail(void)
{
  Fee_StateDataType     *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Enter InitGc fail state */
  StateDataPtr->FeeInitGCState = FEE_INITGC_FAIL;

  /* Raise DEM */
  #if (FEE_GC_INIT_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
  Dem_ReportErrorStatus(FEE_E_GC_INIT, DEM_EVENT_STATUS_FAILED);
  #endif

  /* Raise Illegal state notification if configured */
  if(FEE_CONFIG_PTR->FeeIllegalStateNotification != NULL_PTR)
  {
    /* Call the Notification function */
    FEE_CONFIG_PTR->FeeIllegalStateNotification();
  }
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lPerformPrevBlkCheck (void)             **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (in-out) : None                                                 **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description :  Checks if there is enough space to accommodate the          **
** Last Written Block in case there is a wordline issue                       **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lPerformPrevBlkCheck(void)
{
  Fee_StateDataType *StateDataPtr;
  uint32             DestAddr;
  Fee_DataType       LWBType;
  Fee_PageType       LWBPageCount;
  Fee_PageType       RemainingPages;
  uint16             LWBNum;
  uint16             LWBIndex;
  uint16             Offset;
  uint8              BankNumber;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  BankNumber = StateDataPtr->FeeStatusFlags.FeeCurrSector;
  DestAddr = StateDataPtr->FeeNextFreePageAddr;

  LWBNum = StateDataPtr->FeeLastWrittenBlkInfo.BlockNumber;
  LWBPageCount = StateDataPtr->FeeLastWrittenBlkInfo.PageCount;
  LWBIndex = Fee_lGetUsrBlockIndex(LWBNum);

  RemainingPages = Fee_lGetFreeDFLASHPages(BankNumber, 
                                                  Fee_lGetNextWLAddr(DestAddr));
  Offset = (uint16)DestAddr & FEE_DFLASH_WORDLINE_SIZE_1;

  /* Unconfig blocks are treated as Normal blocks */
  if(LWBIndex == FEE_LOG_BLOCK_NOT_FOUND)
  {
    LWBType = FEE_NORMAL_DATA;
  }
  else
  {
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used within 
      Fee_BlockConfig[Number of configured blocks] to efficiently access its 
      elements*/
    LWBType =
     (Fee_DataType)FEE_CONFIG_PTR->FeeBlockConfigPtr[LWBIndex].FeeImmediateData;
  }

  /* Affected page count (in the worst-case) = Page Count of the block which
     crossed the current word-line boundary + filled pages in the current WL */
  LWBPageCount = LWBPageCount + (Offset >> FEE_THREE);

  if ((LWBType == FEE_NORMAL_DATA) &&
      ((boolean)TRUE == Fee_lIsSectorThresholdCrossed((uint32)RemainingPages, 
                                                      (uint32)LWBPageCount)))
  {
    /* In case of a compare failure, the affected previously written blocks
       cannot be accommodated before the threshold boundary of the current
       sector. Hence, start the current request at next free WL */
    DestAddr = Fee_lGetNextWLAddr(DestAddr);
  }
  else if ((LWBType == FEE_IMMEDIATE_DATA) &&
           (RemainingPages < LWBPageCount))
  {
    /* Prev Blk cannot be accommodated if there is a compare failure
       Start the current request at new WL */
    DestAddr = Fee_lGetNextWLAddr(DestAddr);
  }
  else
  {
    /* Prev Blk can be accommodated if there is a compare failure */
  }

  while (Fee_lIsUnerasableAddr(DestAddr))
  {
    DestAddr = Fee_lGetNextWLAddr(DestAddr);
  }
  StateDataPtr->FeeNextFreePageAddr = DestAddr;
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformCurrBlkCheck          **
**                                           (const Fee_BlockType * BlockPtr) **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockPtr                                                **
**                                                                            **
** Parameters (in-out) : None                                                 **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description :  Checks if there is enough space to accommodate the          **
** Current requested Block                                                    **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformCurrBlkCheck(
                                               const Fee_BlockType * BlockPtr)
{
  Fee_StateDataType *StateDataPtr;
  uint32             DestAddr;
  Fee_PageType       BlkPages;
  Fee_PageType       RemainingPages;
  uint8              BankNumber;
  Std_ReturnType     ReturnValue;

  ReturnValue = E_NOT_OK;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  BankNumber = StateDataPtr->FeeStatusFlags.FeeCurrSector;
  DestAddr = StateDataPtr->FeeNextFreePageAddr;

  if (StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus == (unsigned_int)FEE_ONE)
  {
    BlkPages = FEE_TWO;
  }
  else
  {
    BlkPages = Fee_lGetDFLASHPages((uint16)(BlockPtr->Size));
  }

  RemainingPages = Fee_lGetFreeDFLASHPages(BankNumber, DestAddr);

  /* if threshold crossed */
  if ((boolean)TRUE == Fee_lIsSectorThresholdCrossed((uint32)RemainingPages, 
                                                     (uint32)BlkPages))
  {
    /****** Check if it is a NORMAL REQUEST *******/
    if( (BlockPtr->FeeImmediateData == (unsigned_int)FEE_NORMAL_DATA) ||
        (StateDataPtr->FeeStatusFlags.FeeBlkInvalidStatus 
                                            == (unsigned_int)FEE_ONE)
      )
    {
      /* Do nothing */
    }
    /****** Check if it is a IMMEDIATE REQUEST *******/
    else
    {
      /* if this block cannot be accomodated in the DFLASH */
      if(RemainingPages < BlkPages)
      {
        /* Do nothing */
      }
      /**** Block can be accomodated within the current DFLASH Sector */
      else
      {
        ReturnValue = E_OK;
      }
    }/* End of immediate write check */

    if (StateDataPtr->FeeGcState == FEE_GC_IDLE)
    {
      /* GC should be triggered only if DisableGCStart is FALSE */
      if(StateDataPtr->FeeDisableGCStart == (boolean)FALSE)
      {
        /* Request for Garbage Collection and continue */
        StateDataPtr->FeeGcState = FEE_GC_REQUESTED;
      }
    }
  }
  /* Threshold NOT Crossed*/
  else
  {
    ReturnValue = E_OK;
  }

  return (ReturnValue);
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformWriteCheck            **
**                                           (const Fee_BlockType * BlockPtr) **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockPtr                                                **
**                                                                            **
** Parameters (in-out) : None                                                 **
**                                                                            **
** Return value    :  E_NOT_OK / E_OK                                         **
**                                                                            **
** Description :  Checks if there is enough space to accommodate the request  **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformWriteCheck(
                                                const Fee_BlockType * BlockPtr)
{
  uint32             DestAddr;
  Fee_StateDataType *StateDataPtr;
  Std_ReturnType     ReturnValue;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  DestAddr = StateDataPtr->FeeNextFreePageAddr;
  while(Fee_lIsUnerasableAddr(DestAddr))
  {
    DestAddr = Fee_lGetNextWLAddr(DestAddr);
  }
  StateDataPtr->FeeNextFreePageAddr = DestAddr;

  /* Check if Prev Blk can be accommodated if there is a compare failure */
  if( (Fee_lIsWLBeginning(DestAddr) != FEE_ZERO) &&
      (StateDataPtr->FeeLastWrittenBlkInfo.Status.Consistent == 
                                                 (unsigned_int)FEE_ONE))
  {
    Fee_lPerformPrevBlkCheck();
  }

  /* Check if Current Blk can be accommodated */
  ReturnValue = Fee_lPerformCurrBlkCheck(BlockPtr);

  return (ReturnValue);
}

/*******************************************************************************
** Syntax : static void Fee_lHandleWriteFail(void)                            **
**                                                                            **
** Service ID:                                                                **
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
** Description : This function shall handle the severe error                  **
** (compare failed twice or internal write fail) case during                  **
** write state machine                                                        **
*******************************************************************************/
static void Fee_lHandleWriteFail(void)
{
  Fee_StateDataType  *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;

  /* Enter Write Fail State */
  StateDataPtr->FeePendReqStatus = FEE_WRITE_FAIL;

  /* Raise Job Failed DEM */
  #if (FEE_WRITE_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
  Dem_ReportErrorStatus(FEE_E_WRITE, DEM_EVENT_STATUS_FAILED);
  #endif

  /* Raise Illegal state notification if configured */
  if(FEE_CONFIG_PTR->FeeIllegalStateNotification != NULL_PTR)
  {
    /* Call the Notification function */
    FEE_CONFIG_PTR->FeeIllegalStateNotification();
  }
}

/*******************************************************************************
** TRACEABILITY: [cover parentID=DS_AS_FEE159_FEE160]
**                                                                            **
** Syntax : IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformReadCheck             **
**                                               (uint16 UserBlockIndex)      **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  UserBlockIndex       block index                        **
**                                                                            **
** Parameters (in-out) :None                                                  **
**                                                                            **
** Return value    :  E_NOT_OK / E_OK                                         **
**                                                                            **
** Description :  This utility function checks if the requested copy of the   **
** logical data block is consistent and valid                                 **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Fee_lPerformReadCheck(uint16 UserBlockIndex)
{
  Fee_StateDataType     *StateDataPtr;
  Fee_CacheType         *BlockPtr;
  Std_ReturnType         RetVal;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  BlockPtr = &StateDataPtr->FeeBlockInfo[UserBlockIndex];

  #if (FEE_GET_PREV_DATA_API == STD_ON)
  if (StateDataPtr->FeePendReqInfo.GetPrevCopy == FEE_ONE)
  {
    /* If the logical block is inconsistent */
    if (BlockPtr->Status.PrevCopyConsistent == (unsigned_int)FEE_ZERO)
    {
      StateDataPtr->FeeJobResult = MEMIF_BLOCK_INCONSISTENT;
      RetVal = E_NOT_OK;
    }
    /* If the logical block is invalid */
    else if (BlockPtr->Status.PrevCopyValid == (unsigned_int)FEE_ZERO)
    {
      StateDataPtr->FeeJobResult = MEMIF_BLOCK_INVALID;
      RetVal = E_NOT_OK;
    }
    else
    {
      RetVal = E_OK;
    }
  }
  else
  #endif
  {
    /* If the logical block is inconsistent */
    if (BlockPtr->Status.Consistent == (unsigned_int)FEE_ZERO)
    {
      StateDataPtr->FeeJobResult = MEMIF_BLOCK_INCONSISTENT;
      RetVal = E_NOT_OK;
    }
    /* If the logical block is invalid */
    else if (BlockPtr->Status.Valid == (unsigned_int)FEE_ZERO)
    {
      StateDataPtr->FeeJobResult = MEMIF_BLOCK_INVALID;
      RetVal = E_NOT_OK;
    }
    else
    {
      RetVal = E_OK;
    }
  }

  return (RetVal);
}

/*******************************************************************************
** Syntax : static void Fee_lUpdateBlockInfo(Fee_CacheStatusType Status,      **
**                                                        uint16 Index)       **
**                                                                            **
** Service ID: NA                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in-out) :Status: State of data block                           **
**                      Index: Block Index in cache table                     **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This utility function updates the cache table with the       **
** block info                                                                 **
*******************************************************************************/
static void Fee_lUpdateBlockInfo(Fee_CacheStatusType Status,
                                 uint16 Index)
{
  Fee_CacheType           *CachePtr;
  Fee_StateDataType       *StateDataPtr;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  CachePtr = StateDataPtr->FeeBlockInfo;

  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
  CachePtr = CachePtr + Index;

  if(CachePtr->Address == FEE_ZERO)
  {
    /* Update the cache with block Address */
    CachePtr->Address = StateDataPtr->FeeCurrSectSrcAddr;
    CachePtr->Status.Valid = Status.Valid;
    CachePtr->Status.Consistent = Status.Consistent;
    CachePtr->Status.Copied = Status.Copied;
  }
  else
  {
    /* Update the cache with block Address of previous copy */
    #if (FEE_GET_PREV_DATA_API == STD_ON)
    CachePtr->PrevCopyAddress = StateDataPtr->FeeCurrSectSrcAddr;
    #endif
    CachePtr->Status.PrevCopyValid = Status.Valid;
    CachePtr->Status.PrevCopyConsistent = Status.Consistent;
    CachePtr->Status.PrevCopyCopied = Status.Copied;
  }
}

/*******************************************************************************
** Syntax : static void Fee_lUpdateUnconfigBlockInfo(uint16 BlockNumber)      **
**                                                                            **
** Service ID:                                                                **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  BlockNumber - Block Number                              **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description : This function does the cache table update of the             **
**               un-configured block                                          **
*******************************************************************************/
static void Fee_lUpdateUnconfigBlockInfo(uint16 BlockNumber)
{
  Fee_StateDataType       *StateDataPtr;
  Fee_CacheType           *UnconfigBlockPtr;
  uint16                   UnconfigBlockIndex;
  uint16                   TempUnConfigBlockCount;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  TempUnConfigBlockCount = StateDataPtr->FeeUnConfigBlockCount;

  UnconfigBlockIndex = FEE_ZERO;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
  UnconfigBlockPtr = (StateDataPtr->FeeBlockInfo) +
                     (FEE_CONFIG_PTR->FeeBlkCnt);
 
  while((UnconfigBlockIndex < TempUnConfigBlockCount) &&
        (UnconfigBlockPtr->BlockNumber != BlockNumber))
  {
    UnconfigBlockIndex++;
    /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
      access the data within FeeBlockInfo[FEE_TOTAL_BLOCK_COUNT]*/
    UnconfigBlockPtr++;
  }
  if(UnconfigBlockIndex == TempUnConfigBlockCount)
  {
    /* check if the cache table has space */
    if(((FEE_CONFIG_PTR->FeeBlkCnt) + TempUnConfigBlockCount) < 
                                                         FEE_TOTAL_BLOCK_COUNT)
    {
      #if (FEE_UNCFG_BLK_DEM_REPORT == FEE_ENABLE_DEM_REPORT)
      if(((FEE_CONFIG_PTR->FeeBlkCnt) + TempUnConfigBlockCount) == 
                                                           FEE_MAX_BLOCK_COUNT)
      {
        Dem_ReportErrorStatus(FEE_E_UNCONFIG_BLK_EXCEEDED, 
                              DEM_EVENT_STATUS_FAILED);
      }
      #endif
  
      UnconfigBlockPtr->Address = StateDataPtr->FeeCurrSectSrcAddr;
      UnconfigBlockPtr->BlockNumber = BlockNumber;
      TempUnConfigBlockCount++;
    }
    else
    {
      /* Un-configured blocks cache table is already full */
      StateDataPtr->FeeUncfgBlksExceeded = FEE_UNCONFIG_BLK_LIMIT_CROSSED;
    }
  }
  StateDataPtr->FeeUnConfigBlockCount = TempUnConfigBlockCount;
}

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Fee_lUpdateGcCurrBlockInfo(void)            **
**                                                                            **
** Service ID: NA                                                             **
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
** Description : Updates the block information of the block being written     **
**               during Garbage collection                                    **
*******************************************************************************/
IFX_LOCAL_INLINE void Fee_lUpdateGcCurrBlockInfo(void)
{
  Fee_StateDataType    *StateDataPtr;
  Fee_GcBlkInfoType    *GcBlkPtr;
  uint8                *ReadWriteBufferPtr;
  Fee_PageType          PageCount;
  uint32                WordValue;
  uint16                BlockNumber;
  uint16                Offset;

  StateDataPtr = FEE_CONFIG_PTR->FeeStatePtr;
  GcBlkPtr = &(StateDataPtr->FeeGcCurrBlkInfo);
  Offset= (uint16) ((StateDataPtr->FeeGcDestAddr) & FEE_DFLASH_WORDLINE_SIZE_1);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  ReadWriteBufferPtr = StateDataPtr->FeeGcReadWriteBuffer + Offset;
  
  /* Extract block number and page count */
  WordValue = *(uint32 *)(void *)ReadWriteBufferPtr;
  BlockNumber = (uint16)(WordValue >> FEE_EIGHT);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently 
    access the data within FeeGcReadWriteBuffer[FEE_DFLASH_WORDLINE_SIZE]*/
  PageCount = *((uint16 *)(void *)(ReadWriteBufferPtr+FEE_SIX));
  PageCount = (PageCount & (uint16)FEE_PAGE_COUNT_MASK);
  
  /* Update FeeGcCurrBlkInfo */
  GcBlkPtr->Addr = StateDataPtr->FeeGcSrcAddr;
  GcBlkPtr->BlockNumber = BlockNumber;
  GcBlkPtr->PageCount = PageCount;
  GcBlkPtr->Consistent = FEE_ONE;
}

/* Memory Map of the FEE Code */
#define FEE_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

