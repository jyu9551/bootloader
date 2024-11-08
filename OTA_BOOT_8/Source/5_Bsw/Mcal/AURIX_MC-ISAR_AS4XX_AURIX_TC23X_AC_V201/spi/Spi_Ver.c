/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2015)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. Passing on and copying of this document, and communication   **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**   $FILENAME   : Spi_Ver.c $                                                **
**                                                                            **
**   $CC VERSION : \main\29 $                                                 **
**                                                                            **
**   $DATE       : 2016-09-08 $                                               **
**                                                                            **
**   AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                            **
**   VENDOR      : Infineon Technologies                                      **
**                                                                            **
**   DESCRIPTION : This file contains AUTOSAR version specific functionality  **
**                 of SPI Handler driver.                                     **
**                                                                            **
**   MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Own header file, this includes own configuration file also */
#include "Spi.h"

#include "Spi_Local.h"

#if(SPI_SLAVE_ENABLE == STD_ON)
#include "SpiSlave.h"
#endif

/* Include AS version specific header file */
#include "Spi_Ver.h"

/* Inclusion from Diagnostic Error Manager File */
/* The module shall include the Dem.h file.*/
/* Fix for AI00252645*/
#if ( (SPI_HW_ERROR_DEM_REPORT == ENABLE_DEM_REPORT) ||  \
      (SPI_DMA_ERROR_DEM_REPORT == ENABLE_DEM_REPORT) )
#include "Dem.h"
#endif

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef SPI_SW_MAJOR_VERSION
  #error "SPI_SW_MAJOR_VERSION is not defined. "
#endif

#ifndef SPI_SW_MINOR_VERSION
  #error "SPI_SW_MINOR_VERSION is not defined. "
#endif

#ifndef SPI_SW_PATCH_VERSION
  #error "SPI_SW_PATCH_VERSION is not defined. "
#endif
/* [cover parentID=DS_AS_SPI069] //ASW:1530 */

/* [cover parentID=DS_AS_SPI069] //AUTOSAR:11117 */
#if (SPI_SW_MAJOR_VERSION != 4U)
  #error "SPI_SW_MAJOR_VERSION does not match. "
#endif

#if (SPI_SW_MINOR_VERSION != 8U)
  #error "SPI_SW_MINOR_VERSION does not match. "
#endif
/* [/cover] */

/* AUTOSAR Secification File Version Check */
#ifndef SPI_AR_RELEASE_MAJOR_VERSION
  #error "SPI_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif

#ifndef SPI_AR_RELEASE_MINOR_VERSION
  #error "SPI_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

#ifndef SPI_AR_RELEASE_REVISION_VERSION
  #error "SPI_AR_RELEASE_REVISION_VERSION is not defined. "
#endif

#if (SPI_AR_RELEASE_MAJOR_VERSION != 4U)
  #error "SPI_AR_RELEASE_MAJOR_VERSION does not match. "
#endif

#if (SPI_AR_RELEASE_MINOR_VERSION != 0U)
  #error "SPI_AR_RELEASE_MINOR_VERSION does not match. "
#endif

#if (SPI_AR_RELEASE_REVISION_VERSION != 3U)
  #error "SPI_AR_RELEASE_REVISION_VERSION does not match. "
#endif
/* [/cover] */

/* Inter Module Check */
/* Check for the correct version usage in the used modules */
/* [cover parentID=DS_AS403_SPI369] */

/*
  VERSION CHECK FOR DET MODULE INCLUSION
*/
#if ( SPI_DEV_ERROR_DETECT == STD_ON)

#ifndef DET_AR_RELEASE_MAJOR_VERSION
  #error "DET_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif

#ifndef DET_AR_RELEASE_MINOR_VERSION
  #error "DET_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

/* Updated for AI00251674 IFX_DET_VERSION_CHECK and IFX_DEM_VERSION_CHECK
   are defined in Mcal_Options.h */
#if (IFX_DET_VERSION_CHECK == STD_ON)
#if ( DET_AR_RELEASE_MAJOR_VERSION != SPI_AR_RELEASE_MAJOR_VERSION )
  #error "DET_AR_RELEASE_MAJOR_VERSION does not match. "
#endif
#if ( DET_AR_RELEASE_MINOR_VERSION != SPI_AR_RELEASE_MINOR_VERSION )
  #error "DET_AR_RELEASE_MINOR_VERSION does not match. "
#endif
#endif /* ( SPI_DEV_ERROR_DETECT == STD_ON) */
/* End Of SPI_DEV_ERROR_DETECT */
#endif /* (IFX_DET_VERSION_CHECK  == STD_ON) */

#if ( (SPI_HW_ERROR_DEM_REPORT == ENABLE_DEM_REPORT) ||  \
      (SPI_DMA_ERROR_DEM_REPORT == ENABLE_DEM_REPORT) )
#ifndef DEM_AR_RELEASE_MAJOR_VERSION
  #error "DEM_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif

#ifndef DEM_AR_RELEASE_MINOR_VERSION
  #error "DEM_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

#if (IFX_DEM_VERSION_CHECK == STD_ON)
#if ( DEM_AR_RELEASE_MAJOR_VERSION != SPI_AR_RELEASE_MAJOR_VERSION )
  #error "DEM_AR_RELEASE_MAJOR_VERSION does not match. "
#endif
#if ( DEM_AR_RELEASE_MINOR_VERSION != SPI_AR_RELEASE_MINOR_VERSION )
  #error "DEM_AR_RELEASE_MINOR_VERSION does not match. "
#endif
#endif /* (IFX_DEM_VERSION_CHECK  == STD_ON) */
#endif
/* [/cover] */

#define SPI_START_SEC_CODE
#include "MemMap.h"
/*******************************************************************************
** Syntax           :  Spi_JobResultType Spi_lGetJobStatus                    **
** (                                                                          **
**   Spi_JobType Job                                                          **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Job - Job Id                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Job result.                                             **
**                                                                            **
** Description      : Get the Job Result                                      **
** Traceability : [cover parentID=DS_AS403_SPI038]                 [/cover]   **
*******************************************************************************/
Spi_JobResultType Spi_lGetJobStatus(Spi_JobType Job)
{
  uint16 Index;
  uint32 Temp = 0U;
  Spi_JobResultType RetVal;

  #if (SPI_SAFETY_ENABLE == STD_ON)
  {
    /* ASIL or QM Job index from map configuration*/
    Index = (uint16)(Spi_kConfigPtr->SpiMapConfigPtr->JobIndex[Job]
            >> SPI_JOB_INDEX);
  }
  #else
  {
    /* if safety is OFF then map config is absent */
    Index = (uint16)(Job
            >> SPI_JOB_INDEX);
  }
  #endif

  #if (SPI_SAFETY_ENABLE == STD_ON)
    if( SPI_ASIL_JOB == Spi_lGetJobKind(Job) )
    {
      Temp = Spi_AsilJobResult[Index] >>
                   (SPI_JOB_STATUS_BITS * ((uint32)Job & SPI_JOB_BIT_POSITION));
    }
    else
    {
      #if (SPI_QM_MASTER_MODULES_USED != 0U)
      Temp = Spi_QmJobResult[Index] >>
                   (SPI_JOB_STATUS_BITS * ((uint32)Job & SPI_JOB_BIT_POSITION));
      #endif
      /* Do nothing QM modules are NOT Used */
    }
  #else
  {
      Temp = Spi_QmJobResult[Index] >>
                   (SPI_JOB_STATUS_BITS * ((uint32)Job & SPI_JOB_BIT_POSITION));
  }
  #endif

  RetVal = (Spi_JobResultType)(Temp & (uint32)SPI_JOB_STATUS_EXTRACT);

  return (RetVal);
}

#define SPI_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
allowed only for MemMap.h*/
#include "MemMap.h"
