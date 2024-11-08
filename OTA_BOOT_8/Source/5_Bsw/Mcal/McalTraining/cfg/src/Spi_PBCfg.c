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
**  FILENAME  : Spi_PBCfg.c                                                   **
**                                                                            **
**  $CC VERSION : \main\88 $                                                 **
**                                                                            **
**  DATE, TIME: 2024-08-13, 09:38:47                                         **
**                                                                            **
**  GENERATOR : Build b141014-0350                                            **
**                                                                            **
**  AUTHOR    : DL-AUTOSAR-Engineering                                        **
**                                                                            **
**  VENDOR    : Infineon Technologies                                         **
**                                                                            **
**  DESCRIPTION  : SPI configuration generated out of ECU configuration       **
**                 file                                                       **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Include SPI Module File */
/* [cover parentID=DS_NAS_SPI_PR699,DS_NAS_SPI_PR709] */

#include "Spi.h"
/* Inclusion of Mcal Specific Global Header File */
#include "Mcal.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
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
/* MISRA RULE 87 VIOLATION: Inclusion of MemMap.h in between the code can't
   be avoided as it is required for mapping global variables, constants
   and code
*/
/* Violates MISRA Required Rule 16.9,
            function identifier used without '&' or parenthisized parameter list
           when using function pointer in configurations
*/

/*
                     Container: SpiChannelConfiguration
*/
#define SPI_START_SEC_POSTBUILDCFG
/*
 * To be used for global or static constants (unspecified size)
*/
#include "MemMap.h"
/*
Configuration : Channel Configuration Constant Structure.
The IB Channels are configured first followed by EB.
*/
static const Spi_ChannelConfigType Spi_kChannelConfig0[] =
{
/* EB Channel: SpiChannel_0 Configuration */
  {
    /* Default Data, SPI_DEFAULT_DATA */
    (uint32)0x00000000U,
    /* Data Configuration */
    Spi_DataConfig(16U, /* Data Width */
                   SPI_DATA_MSB_FIRST), /* Transfer Start */
        /* EB Channels : SPI112: Max EB Buffers  */
    (Spi_NumberOfDataType)511U,
    /* Buffer Type, SPI_CHANNEL_TYPE */
    (uint8)SPI_EB_BUFFER,
  },
};

/*
                    Container: SpiJobConfiguration
*/
/* Notification Function of SpiJob_0 is NULL_PTR */


/*
Configuration: Channel Assignment
*/
/*Channel Assignment of Job: SpiJob_0*/
static const Spi_ChannelType SpiJob_0_ChannelLinkPtr[] =
{
  SpiConf_SpiChannel_SpiChannel_0,
  SPI_CHANNEL_LINK_DELIMITER
};

/*
Configuration: Job Configuration Constant Structure.
*/
static const Spi_JobConfigType Spi_kJobConfig0[] =
{

/* Job: SpiJob_0 Configuration */
  {
    /* User given name outside the naming convention */
    /* Spi_ChannelLinkPtr */
    SpiJob_0_ChannelLinkPtr,
    /* Baud Rate (10000000 Hz) Hw configuration Parameters */
    Spi_BaudRateParams(/*TQ*/(0x1U), /*Q*/(0x1U),
              /*A*/(0x0U), /*B*/(0x0U), /*C*/(0x1U)),

    /* Time Delay Configuration */
      (uint32)Spi_DelayParams(/*IPRE*/0x01U,/*IDLE*/ 0x01U,
        /*LPRE*/0x01U, /*LEAD*/0x01U,
        /*TPRE*/0x01U, /*TRAIL*/0x01U),

    SPI_CS_HW, /* Hw CS Pin is Selected */

    /*CS Active level Polarity*/
    SPI_CS_POLARITY_LOW,

 /* Shift Clock Configuration : Clock Idle Polarity: SPI_SHIFT_CLOCK_IDLE_LEVEL,
                                  Clock Phase: SPI_DATA_SHIFT_EDGE */
    Spi_ShiftClkConfig(SPI_CLK_IDLE_LOW,
                      SPI_DATA_SHIFT_TRAIL),
   /* Job Priority escalated to Maximum as it is mapped
      to one or more non-interruptible sequence */
    SPI_JOB_PRIORITY_3,

    /* Spi HW Unit. bit[7:4]: Channel no, bit[3:0]: hw module no */
      (uint8)((uint8)SPI_QSPI_CHANNEL2 << 4U) | (SPI_QSPI0_INDEX),

    /* Channel Based Chip Select */
    (uint8)0U,
    
    /* Spi Parity Selection */
    (uint8)SPI_QSPI_PARITY_UNUSED,
    
  }
};

/*
                     Container: Spi_SequenceConfiguration
*/
/* Notification Function of Sequence: SpiSequence_0 is NULL_PTR */

/*
Configuration: Job Assignment
*/
/* Job Assignment of Sequence: SpiSequence_0 */
static const Spi_JobType SpiSequence_0_JobLinkPtr[] =
{
  SpiConf_SpiJob_SpiJob_0,
  SPI_JOB_LINK_DELIMITER
};
/*
Configuration: Sequence Configuration Constant Structure.
*/
static const Spi_SequenceConfigType Spi_kSequenceConfig0[] =
{   /* Sequence: SpiSequence_0 Configuration */
  {
    /* User given name outside the naming convention */    /* Spi_JobLinkPtr */
    SpiSequence_0_JobLinkPtr,
     /* This holds the total number of jobs linked to this sequence */
    1U,
  }
};


static const Spi_HWModuleConfigType Spi_kModuleConfig0[]=
{
  /* QSPI0 Module */
  {
      /*Clock Settings:Sleep Control Disabled*/
      SPI_CLK_SLEEP_DISABLE,
      SPI_QSPI0_MRIS_SEL,
      NULL_PTR,
  },
  /* QSPI1 Module */
  {
    SPI_1_NOT_CONFIGURED,
    SPI_1_NOT_CONFIGURED,
    NULL_PTR,
  },
  /* QSPI2 Module */
  {
    SPI_2_NOT_CONFIGURED,
    SPI_2_NOT_CONFIGURED,
    NULL_PTR,
  },
  /* QSPI3 Module */
  {
    SPI_3_NOT_CONFIGURED,
    SPI_3_NOT_CONFIGURED,
    NULL_PTR,
  },
};


static const Spi_BaudrateEconType Spi_kBaudrateEcon0[]=
{
  {
    Spi_BaudRateECON(0x1U, 0x0U, 0x0U, 0x1U,
    SPI_DATA_SHIFT_TRAIL,
    SPI_CLK_IDLE_LOW,
    SPI_QSPI_PARITY_DISABLE),
    (uint8)((uint8)SPI_QSPI_CHANNEL2 << 4U) | (SPI_QSPI0_INDEX)
  }
};

const Spi_ConfigType Spi_ConfigRoot[1U] =
{
  {
    Spi_kChannelConfig0,
    Spi_kJobConfig0,
    Spi_kSequenceConfig0,
    {
      &Spi_kModuleConfig0[0U],
      &Spi_kModuleConfig0[1U],
      &Spi_kModuleConfig0[2U],
      &Spi_kModuleConfig0[3U],
    },
    Spi_kBaudrateEcon0,
    (Spi_JobType)(sizeof(Spi_kJobConfig0) / sizeof(Spi_JobConfigType)),
    (Spi_ChannelType)(sizeof(Spi_kChannelConfig0) / \
                      sizeof(Spi_ChannelConfigType)),
    (Spi_SequenceType)(sizeof(Spi_kSequenceConfig0) / \
                                        sizeof(Spi_SequenceConfigType)),
    (uint8)(sizeof(Spi_kBaudrateEcon0) / sizeof(Spi_BaudrateEconType)),
  }
};


#define SPI_STOP_SEC_POSTBUILDCFG
/* Allows to map variables, constants and code of modules to individual
  memory sections.*/
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
allowed only for MemMap.h*/  
#include "MemMap.h"

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
**                      Global Function Definitions                           **
*******************************************************************************/
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/* General Notes */
/*
SPI095: The code file structure shall not be defined within this specification
completely. At this point it shall be pointed out that the code-file structure
shall include the following file named:
- Spi_Lcfg.c ?for link time and for post-build configurable parameters and
- Spi_PBcfg.c ?for post build time configurable parameters.
These files shall contain all link time and post-build time configurable
parameters.
This file shall contain all link time and post-build time configurable
parameters.
For the implementation of VariantPC, the implementation stores the
pre compile time parameters that have to be defined as constants in this file.

SPI123: In this configuration, all Sequences declared are considered as Non
Interruptible Sequences. That means, their dedicated parameter
SPI_INTERRUPTIBLE_SEQUENCE (see SPI064 & SPI106) could be omitted or the
FALSE value should be used as default.

*/


