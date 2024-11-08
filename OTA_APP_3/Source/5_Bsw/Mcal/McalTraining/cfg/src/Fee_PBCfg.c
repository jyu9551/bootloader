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
**  $FILENAME   : Fee_PBCfg.c $                                              **
**                                                                           **
**  $CC VERSION : \main\18 $                                                 **
**                                                                           **
**  DATE, TIME: 2021-11-10, 10:43:08                                         **
**                                                                           **
**  GENERATOR : Build b141014-0350                                           **
**                                                                           **
**  AUTHOR    : DL-AUTOSAR-Engineering                                       **
**                                                                           **
**  VENDOR    : Infineon Technologies                                        **
**                                                                           **
**  DESCRIPTION  : FEE configuration generated out of ECU configuration      **
**                   file (Fee.bmd)                                          **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Include Fee Module Header File */
#include "Fee.h"

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
**                      Global Function Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/* Allocate space for state data variables in RAM */
#define FEE_START_SEC_VAR_FAST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

/* Fee State Variable structure */
static Fee_StateDataType Fee_StateVar;

#define FEE_STOP_SEC_VAR_FAST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


/* User configured logical block initialization structure */
#define FEE_START_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

static const Fee_BlockType Fee_BlockConfig[] =
{
  {
    0U, /* Block Cycle Count */
    (uint8)FEE_IMMEDIATE_DATA,  /* Block type is Immediate */
    1U, /* Block number */
    2U /* Fee Block Size */
  },
  {
    0U, /* Block Cycle Count */
    (uint8)FEE_IMMEDIATE_DATA,  /* Block type is Immediate */
    2U, /* Block number */
    4U /* Fee Block Size */
  }
};

/* Fee Global initialization structure. */
const Fee_ConfigType Fee_ConfigRoot[] =
{
  {
    /* Fee State Data Structure */
    &Fee_StateVar,
    /* Pointer to logical block configurations */
    &Fee_BlockConfig[0],
    /* Fee Job end notification API */
    (Fee_NotifFunctionPtrType)NULL_PTR,
    /* Fee Job error notification API */
    (Fee_NotifFunctionPtrType)NULL_PTR,
    /* Fee threshold value */
    10240U,
    /* Number of blocks configured */
    2U,

    {
      /* Keep the unconfigured blocks */
      FEE_UNCONFIG_BLOCK_KEEP,
      /* Restart Garbage Collection when user job is requested */
      FEE_GC_RESTART_WRITE,
      /* Erase Suspend feature is disabled */
      FEE_ERASE_SUSPEND_DISABLED,
      /* Reserved */
      0U
    },

    /* Fee Illegal State notification */
    (Fee_NotifFunctionPtrType)NULL_PTR,
    /* Erase All feature is enabled */
    (boolean)TRUE
  }
};

#define FEE_STOP_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#define FEE_START_SEC_SPL_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
const Fee_ConfigType * Fee_CfgPtr = &Fee_ConfigRoot[0];
#define FEE_STOP_SEC_SPL_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

