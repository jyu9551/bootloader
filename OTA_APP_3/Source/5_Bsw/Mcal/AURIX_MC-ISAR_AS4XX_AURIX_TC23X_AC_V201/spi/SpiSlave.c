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
**   $FILENAME   : SpiSlave.c $                                               **
**                                                                            **
**   $CC VERSION : \main\93 $                                                 **
**                                                                            **
**   $DATE       : 2016-07-22 $                                               **
**                                                                            **
**   AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                            **
**   VENDOR      : Infineon Technologies                                      **
**                                                                            **
**   DESCRIPTION : This file contains                                         **
**                 - functionality of SPISlave driver.                        **
**                                                                            **
**   MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**  TRACEABILITY: [cover parentID= DS_MCAL_SPI_9813_01,DS_MCAL_SPI_9810_01,
DS_MCAL_SPI_9818_01,DS_MCAL_SPI_9808_01,SAS_MCAL_SPI_9844,
DS_NAS_HE2_SPI_PR2981_2,DS_NAS_EP_SPI_PR2981_2, ]
**  [/cover]                                                                  **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include "IfxQspi_reg.h"
#include "IfxQspi_bf.h"
#include "IfxDma_reg.h"
#include "IfxSrc_reg.h"
#include "IfxPort_reg.h"

/* Own header file, this includes own configuration file also */
#include "Spi_Local.h"

/* Include AS version specific header file */
#include "Spi_Ver.h"

#if(SPI_SLAVE_ENABLE == STD_ON)
/* Inclusion from Diagnostic Error Manager File */
/* The module shall include the Dem.h file.*/
#if ( (SPI_HW_ERROR_DEM_REPORT == ENABLE_DEM_REPORT) ||  \
      (SPI_DMA_ERROR_DEM_REPORT == ENABLE_DEM_REPORT) )
#include "Dem.h"
#endif

/*
  Exports specific definitions and services of the BSW Scheduler for SPI
*/
#include "SchM_Spi.h"

/* Inclusion from Safety Error */
/* The module shall include the SafetyReport.h file.*/
/*AI00247913 Fixed*/
#if(SPI_SAFETY_ENABLE == STD_ON)
#include "SafetyReport.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/* Value to indicate that the SPI Bus is Idle */
#define SPI_BUS_IDLE ((uint8)0xFFU)

/* Value to indicate that the Sequence is non active */
#define SPI_SEQ_IDLE_ID (0xFFU)
/* Value to indicate that the Job is non active */
#define SPI_JOB_IDLE_ID (0xFFFFU)

/* Defines for QSPI GLOBALCON Register */
#define SPI_QSPI_SLAVE_SELECT (0x02U)
#define SPI_QSPI_LOOPBACK_DISABLE (0x0U)

#define SPISLAVE_QSPI_CLOCKPHASE     (0x01U)
#define SPISLAVE_QSPI_CLOCKPOL_SHIFT (0x00U)

/* Defines for Dem errors */
#define SPI_DEM_HW_ERROR (1U)
#define SPI_DEM_DMA_TRANSFER_ERROR (2U)

/* DMA Interrupt Category in Asyn Mode with Interrupts
  Interrupt trigger is generated and bit CHSRxz.ICH
  is set on changing the TCOUNT value and TCOUNT equals IRDV*/
#define SPI_DMA_INTCT         (2U)

/* DMA Channel Control Setting for 16 Bit data Width */
#define SPI_DMA_CHDW_16BIT    (0x1U )

/* DMA Channel Control Setting for 32 Bit data Width */
#define SPI_DMA_CHDW_32BIT    (0x2U )

/* DMA Channel Control Setting for 8 Bit data Width */
#define SPI_DMA_CHDW_8BIT    (0x0U)

/* DMA Interrupt Category in Asyn Mode with Interrupts
  Interrupt trigger is generated and bit CHSRxz.ICH
  is set on changing the TCOUNT value and TCOUNT equals IRDV*/
#define SPI_DMA_INTCT         (2U)

/* DMA RX Channel Address Control Reg Value :
   Dest Address  Increment : 1*CHDW
   Src  Address  Increment : 0
   CBLD = 0xF; INCD =1; SMF = DMF = 0 i.e 1*CHDW (i.e.CHCRxz.CHDW)
*/
#define SPI_DMA_RX_ADRCR_NORML_TRANSFER (0x08B0F080U)

/* Dma SRC register addresses */
/* #define SRC_DMACH0ADDR ((Ifx_SRC_SRCR_Bits volatile *)
                                                     (void *)&(SRC_DMA_CH0)) */

/* Macros for receive count shift. */
#define SPI_RECEIVECOUNT_SHIFT_16BIT (1U)
#define SPI_RECEIVECOUNT_SHIFT_32BIT (2U)
#define SPI_RECEIVECOUNT_ONE (1U)

/* Macros for receive completion status  */
#define SPI_RX_IN_PROGRESS (1U)
#define SPI_RX_COMPLETED (0U)

/* Defines for status register */
#define SPI_QSPI_ERRORSTATUS_MASK (0x079U)
/* Defines for FLAGSCLEAR register */
#define SPI_QSPI_FLAGSCLEAR_VALUE (0x0FFFU)
#define SPI_QSPI_RXC_FLAGSCLEAR_VALUE (0x00000400U)
#define SPI_QSPI_ERR_FLAGSCLEAR_VALUE (0x000001FFU)

/* Baudrate config parameter masks */
#define SPI_BAUDRATECONFIG_TQ_MASK (0x00FFU)
#define SPI_BAUDRATECONFIG_Q_MASK (0x003FU)
#define SPI_BAUDRATECONFIG_Q_SHIFT (8U)
#define SPI_BAUDRATECONFIG_A_MASK (0x0003U)
#define SPI_BAUDRATECONFIG_A_SHIFT (16U)
#define SPI_BAUDRATECONFIG_B_MASK (0x0003U)
#define SPI_BAUDRATECONFIG_B_SHIFT (18U)
#define SPI_BAUDRATECONFIG_C_MASK (0x0003U)
#define SPI_BAUDRATECONFIG_C_SHIFT (20U)

  /* HW channels vary from 0 - 15
     but the ECON maps 8 - 15 to 0 to 7.   */
#define SPI_CONFIG_HWCHANNELNO_MASK (0x0FU)
#define SPI_CONFIG_HWCHANNELNO_SHIFT (19U)
#define SPI_CONFIG_MSB_MASK (0x1U)
#define SPI_CONFIG_MSB_SHIFT (8U)
#define SPI_CONFIG_DATALENGTH_MASK (0x1FU)
#define SPI_CONFIG_TIMEDELAY_MASK (0x0007FFFFU)
#define SPI_SLAVE_TRANSMIT_DATA (0xf0f0e0e0U)

/* Defines for QSPI GLOBALCON1 Register */
/*
#define SPI_QSPI_ERRORENS_VALUE (0x17FU)*/
/* Transmitter FIFO errors are disabled as slave shall only receive data
 baud rate error shall be enabled for slave this detects baud rate error
 in shift clock input */
#define SPI_QSPI_ERRORENS_VALUE (0x1E7U)
/* If the TXFIFO filling level is equal or less than this
threshold, than each move of an element from the
TXFIFO to the shift register triggers a transmit interrupt */
#define SPI_QSPI_TXFIFOINT_VALUE (0x0U)
/* If the RXFIFO filling level is equal or greater than this
threshold, than each move of an element from the shift
register to the FIFO triggers a receive interrupt */
#define SPI_QSPI_RXFIFOINT_VALUE (0x0U)
/* Enable the Interrupt */
#define SPI_QSPI_INT_ENABLE (0x1U)
/* Disable the interrupt */
#define SPI_QSPI_INT_DISABLE (0x0U)

/* Defines for QSPI BACONENTRY Register */
#define SPI_QSPI_TIMEDELAY_MASK (0x0007FFFEU)
#define SPI_QSPI_DL_BIT_SELECT (0U)

/*Macro to handle 16 bit data */
#define SPI_16BIT_DATA_ACCESS  (2U)
/*Macro to handle 32 bit data */
#define SPI_32BIT_DATA_ACCESS  (4U)
/*Macro to handle 8 bit data */
#define SPI_8BIT_DATA_ACCESS  (1U)

#define SPI_HWUNIT_MASK (0x0FU)
#define SPI_HWUNIT_CH_SHIFT (0x04U)

/* Defines for QSPI GLOBALCON Register */
#define SPI_QSPI_MASTER_SELECT (0x0U)
#define SPI_QSPI_LOOPBACK_DISABLE (0x0U)
#define SPI_QSPI_EXPECT_VALUE (15U)
#define SPI_QSPI_SI_DISABLE (0U)
#define SPI_QSPI_SRF_DISABLE (0U)
#define SPI_QSPI_MODULE_ENABLE (1U)

#if(SPI_RESET_SFR_INIT == STD_ON)
#define SPI_KERNEL_RESET_VALUE  (0x00000001U)
#define SPI_KERNEL_RESET_STATUS (0x00000002U)
#define SPI_DMA_RESET_VALUE     (0x00000000U)
#endif

/* Defines for QSPI ECON Register */
#define SPI_QSPI_CLOCKPHASE_MASK (0x01U)
#define SPI_QSPI_CLOCKPOL_MASK (0x01U)
#define SPI_QSPI_CLOCKPOL_SHIFT (0x01U)

  /* HW channels vary from 0 - 15
     but the ECON maps 8 - 15 to 0 to 7.   */
#define SPI_QSPI_HWCHANNELNO_MASK (0x7U)

/* Mask for 16 Bit Data in the Channel's Data Configuration variable.
i.e to determine if data width > 8 */
#define SPI_16BIT_DATAMASK ((uint16)(0x0008U))
/* Mask for 32 Bit Data in the Channel's Data Configuration variable.
i.e to determine if data width > 16 */
#define SPI_32BIT_DATAMASK ((uint16)(0x0010U))

/* Define to reset the QSPI module by setting GLOBALCON.RESETS bits */
#define SPI_QSPI_RESET_MODULE (0xF0000000U)
/*Define reset value for GLOBALCON register   */
#define SLAVE_QSPI_GLOBCON_RSTVAL (0x040F30FFU)

#define SLAVE_QSPI_ECON_RSTVAL (0x00001450U)

#define SPI_QSPI_RST_STATEMCHNEANDFIFO (0x7U)

#define SPI_QSPI_CLC_DISR_DISABLE (0x00000001U)

/* SPI Register Start Address Mapping */
#define SPI_HW_MODULE  ((volatile Ifx_QSPI *)(volatile void *) &(MODULE_QSPI0))

#define SPI_HWUNIT_MASK (0x0FU)
#define SPI_HWUNIT_CH_SHIFT (0x04U)

/* This macro is used to find the position of the sequence in the array
SeqStatus */
#define SPI_SEQUENCE_BIT_POSITION  ((uint8)15)
/* This macro to find the index for sequence in the array SeqStatus. */
#define SPI_SEQUENCE_INDEX   ((uint8)4)

/*
  This macro extracts the status of the seq from two bits
*/
#define SPI_SEQUENCE_STATUS_EXTRACT  (3U)
#define SPI_SEQUENCE_STATUS_BITMASK  (0x03U)

/* This macro to find the index for Job in the array Spi_JobResult. */
#define SPI_JOB_INDEX   ((uint8)4)
/*
  This macro represents the number of bits required to hold the staus of jobs
*/
#define SPI_JOB_STATUS_BITS  ((uint8)2U)
/*
  This macro extracts the status of the job from two bits
*/
#define SPI_JOB_STATUS_EXTRACT  (3U)

/*
  This macro represents the number of bits required to hold the staus of seq
*/
#define SPI_SEQUENCE_STATUS_BITS  (2U)
/*
This macro is used to find the position of the Job in the array Spi_JobResult
*/
#define SPI_JOB_BIT_POSITION  ((uint8)15)

#if ( SPI_INITCHECK_API == STD_ON )
/* Compare value of the init check */
#define SPI_INITCHK_COMP_VALUE  (0xFFFFFFFFU)
#endif
/* ( SPI_INITCHECK_API == STD_ON ) */

#define SPI_CLC_CLEARMASK                    (0x00000002U)
#define SPI_GLOBALCONREG_CLEARMASK           (0xF9FFFFFFU)
#define SPI_QSPI_RX_CLEARMASK                (0x7FFFFFFFU)
#define SPI_QSPI_RX_CLEARMASK1               (0x7FFFFBFFU)
#define SPI_QSPI_RX_CLEARMASK3               (0x52000000U)
#define SPI_QSPI_RX_SETMASK                  (0x52000000U)
#define SPI_QSPI_RX_SETMASK1                 (0x00000400U)
#define SPI_QSPI_ERR_CLEARMASK               (0x7FFFFFFFU)
#define SPI_QSPI_ERR_SETMASK                 (0x50000000U)
#define SPI_QSPI_ERR_SETMASK1                (0x00000400U)
#define SPI_DMACHOADDR_CLEARMASK             (0x7FFFFFFFU)
#define SPI_DMAERRINTR_CLEARMASK             (0xFFFFFFFFU)
#define SPI_DMACHCSR_CLEARMASK               (0xFFFFFFFFU)
#define SPI_DMACHCSR_CLEARMASK1              (0xFFFFFFFFU)
#define SPI_DMAADICR_CLEARMASK               (0xFFFFFFFFU)
#define SPI_DMAADICR_CLEARMASK1              (0xF3FFFFFFU)
#define SPI_DMACHCSR_CLEARMASK2              (0xF7FFFFFFU)
#define SPI_DMATSR_CLEARMASK                 (0xFFFFFFFFU)
#define SPI_GLOBALCONREG_CLEARMASK1          (0x0FFFFFFFU)
#define SPI_GLOBALCONREG_CLEARMASK17         (0x7FFFFFFFU)
#define SPI_GLOBALCONREG_SETMASK1            (0x70000000U)
#define SPI_GLOBALCONREG_CLEARMASK2          (0xFFFFBFFFU)
#define SPI_GLOBALCONREG_CLEARMASK3          (0xFFFFFDFFU)
#define SPI_GLOBALCONREG_CLEARMASK4          (0xFFFFC3FFU)
#define SPI_GLOBALCONREG_CLEARMASK5          (0xFFDFFFFFU)
#define SPI_GLOBALCONREG_CLEARMASK6          (0xFFFFFFFFU)
#define SPI_GLOBALCONREG_CLEARMASK7          (0xFFFFFF00U)
#define SPI_GLOBALCONREG_CLEARMASK8          (0x0FFFFFFFU)
#define SPI_GLOBALCONREG_CLEARMASK9          (0xFFFFFE00U)
#define SPI_GLOBALCONREG_CLEARMASK10         (0xFFF3FFFFU)
#define SPI_GLOBALCONREG_CLEARMASK11         (0xFFFFEFFFU)
#define SPI_GLOBALCONREG_CLEARMASK12         (0xFFFFDFFFU)
#define SPI_GLOBALCONREG_CLEARMASK13         (0xFFFFFFC0U)
#define SPI_GLOBALCONREG_CLEARMASK14         (0xFFFFFF3FU)
#define SPI_GLOBALCONREG_CLEARMASK15         (0xFFFFFCFFU)
#define SPI_GLOBALCONREG_CLEARMASK16         (0xFFFFF3FFU)
#define SPI_BACONREG_CLEARMASK               (0x0FFFFFFFU)
#define SPI_BACONREG_CLEARMASK1              (0xFFDFFFFFU)
#define SPI_BACONREG_CLEARMASK2              (0xFFBFFFFFU)
#define SPI_BACONREG_CLEARMASK3              (0xF07FFFFFU)
#define SPI_BACONREG_CLEARMASK4              (0xFFF7FFFFU)
#define SPI_DMAADICR_CLEARMASK2              (0x00800000U)
#define SPI_DMAADICR_CLEARMASK3              (0x0C000000U)
#define SPI_QSPI_RX_CLEARMASK2               (0x00000400U)
#define SPI_QSPI_RX_SETMASK2                 (0x02000000U)
#define SPI_QSPI_RX_SETMASK3                 (0x40000000U)
#define SPI_QSPI_RX_SETMASK4                 (0x10000000U)
#define SPI_DMACHOADDR_SETMASK               (0x02000000U)
#define SPI_DMAADICR_SETMASK                 (0x00800000U)
#define SPI_DMACHCSR_SETMASK                 (0x08000000U)
#define SPI_DMATSR_SETMASK                   (0x00040000U)
#define SPI_GLOBALCONREG_SETMASK             (0x000000FFU)
#define SPI_GLOBALCONREG_SETMASK2            (0x04000000U)
#define SPI_FLAGSCLEAR_SETMASK               (0x00000FFFU)
#define SPI_FLAGSCLEAR_CLEARMASK             (0x00000FFFU)
#define SPI_CLC_DISS_BITPOS                  1U
#define SPI_SLAVE_SRIS_BITPOS                4U
#define SPI_SLAVE_SCIS_BITPOS                8U
#define SPI_QSPI_RX_BITPOS                   10U
#define SPI_DMA_ADICR_INTCT_BITPOS           26U
#define SPI_QSPI_RX_SRE_BITPOS               10U
#define SPI_DMA_ADICR_ETRL_BITPOS            23U


/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/*
Type:Spi_SlaveEBBuffer.
The EB Buffer Parameters.
*/
typedef struct Spi_SlaveEBBuffer
{
  Spi_DataType* DestPtr;        /* Destination pointer */
  Spi_NumberOfDataType Length;  /* Number of the data to be transmitted */
}Spi_SlaveEBBufferType;

/*******************************************************************************
**                 Private Function Declarations:
*******************************************************************************/
/*Memory Map of the SPI Code*/
#define SPI_START_SEC_CODE
/*
  Allows to map variables, constants and code of modules to individual
  memory sections.
*/
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"
/* This function configures the Slave hardware  for the reception of data */
static void Spi_lSlaveStartJob(uint32 Module, Spi_JobType JobId,\
                                                             uint8 ApiAccessId);

/*returns value 1, on finding the jobs shared by the sequences*/
static uint8 Spi_lSlaveSeqSharingJobStatus
(
  const Spi_SequenceConfigType* SeqConfigPtr
);

/* This function configures the QSPI and Dma hardware for
      the channel reception      */
static void Spi_lSlaveStartChannel(uint32 Module, Spi_ChannelType Channel,
       uint32 DelayAndHwChNo, uint8 ParityConfig,uint8 ApiAccessId);

/* Function to Enable Spi Slave Hw Rx Interrupt*/
IFX_LOCAL_INLINE void  Spi_lSlaveHwEnableQspiRxIntr\
                                              (uint32 Module,uint8 ApiAccessId);

/* Function to Disable Spi Slave Hw Rx Interrupt */
IFX_LOCAL_INLINE void  Spi_lSlaveHwDisableQspiRxIntr(uint32 Module);

/* Function to Enable Spi Slave Hw Rx Interrupt */
IFX_LOCAL_INLINE void  Spi_lSlaveHwEnableQspiErrIntr(uint32 Module,\
                                                            uint8 ApiAccessId);

/* Function to Clear Dma Hw Rx channel Interrupt */
IFX_LOCAL_INLINE void  Spi_lSlaveHwDmaClearIntr(Dma_ChannelType Channel,\
                                                            uint8 ApiAccessId);

/*Disable the Module at the end of Job transmission */
IFX_INLINE void Spi_lSlaveHwDisableModule(uint32 Module);

/* Function to Start transfer of Dma Rx Channels */
IFX_LOCAL_INLINE void Spi_lSlaveHwDmaStartReception(
Spi_DataType* DmaRxDestPtr,
uint32 DmaReceiveCount,uint16 DataWidth,uint8 ApiAccessId);

/*This function returns the id of the completed job */
IFX_LOCAL_INLINE uint16 Spi_lSlaveCompletedJobId(void);

/*  Function to Initialise Spi Slave Hw Interrupts during
    Hw initialisation */
IFX_LOCAL_INLINE void  Spi_lSlaveHwInitIntr(uint32 Module);

/* Function to Initialise Dma Hw used for Spi Slave during
   Hw initialisation */
static void Spi_lSlaveHwDmaInit(void);

/* Function to Initialise Dma Hw Interrupts */
IFX_LOCAL_INLINE void  Spi_lSlaveHwInitDmaIntr(void);

/* Function to deinitialize DMA module */
static void Spi_lSlaveHwDmaDeInit(void);

/* The function shall configure the spi slave channel
   parameters in the hardware */
static void Spi_lSlaveHwSetChannelConfig(volatile uint32 Module,
 uint16 DataConfig,uint32 DelayAndHwChNo, uint8 ParityConfig,uint8 ApiAccessId);

/*  Function to Set Slave Job Configuration Parameters  */
static void Spi_lSlaveHwSetJobConfig(volatile Ifx_QSPI* ModulePtr,
                       const Spi_JobConfigType* JobConfigPtr,uint8 ApiAccessId);

#if(SPI_BAUDRATE_AT_RUNTIME == STD_OFF)

IFX_LOCAL_INLINE void Spi_lSlaveHwSetConfigExtn
                                           (const Spi_ConfigType* ConfigPtr);

#if (SPI_INITCHECK_API == STD_ON)
IFX_LOCAL_INLINE Std_ReturnType Spi_lSlaveHwCheckConfigExtn
                                           (const Spi_ConfigType* ConfigPtr);
#endif

#endif /* SPI_BAUDRATE_AT_RUNTIME == STD_OFF */

/* Function to write into slave transmission buffer  */
static void Spi_lSlaveWriteTransmit(uint8 ApiAccessId);

IFX_LOCAL_INLINE uint32 Spi_lChkEBBufCont(Spi_ChannelType Channel,
                uint16 EBDatCfg);

/* Function to clear Job Result status(SPI_JOB_OK) */
IFX_LOCAL_INLINE void Spi_lSlaveClearJobResultStatus(void);

/* Function to clear Sequence Status (SPI_SEQ_OK) */
IFX_LOCAL_INLINE void Spi_lSlaveClearSequenceStatus(void);

#if ((SPI_CANCEL_API == STD_ON) && (SPI_LEVEL_DELIVERED != 0U))
/*Clear Sequence Canceled State*/
IFX_LOCAL_INLINE void  Spi_lSlaveClrSeqCancelStatus(Spi_SequenceType Seq);

/* Function to set sequence cancel status */
IFX_LOCAL_INLINE void  Spi_lSlaveSetSeqCancelStatus(Spi_SequenceType Seq);
#endif

#if (SPI_CANCEL_API == STD_ON)
/* Function to set the status of sequence cancelled to cancel */
IFX_LOCAL_INLINE void Spi_lSlaveSeqCancelled(Spi_JobAndSeqAccessType \
                                                           *JobSeqDataPtr);
#endif
/*(SPI_CANCEL_API == STD_ON)*/

/*Returns the sequence status*/
IFX_LOCAL_INLINE Spi_SeqResultType Spi_lSlaveGetSeqStatus(Spi_SequenceType Seq);

/*Set the sequence status */
IFX_LOCAL_INLINE void Spi_lSlaveSetSeqStatus
(
Spi_SequenceType Seq,
Spi_SeqResultType Status
);

/*set the Sequence result atomically*/
IFX_LOCAL_INLINE void Spi_lSlaveSetSeqStatusAtomic(Spi_SequenceType Seq,
                              Spi_SeqResultType Status);

IFX_LOCAL_INLINE void Spi_lSlaveSetJobStatusAtomic
(
  Spi_JobType Job,
  Spi_JobResultType Result
);

#if ( SPI_INITCHECK_API == STD_ON )
static Std_ReturnType Spi_lSlaveHwDmaSfrInitCheck(void);
static Std_ReturnType Spi_lSlaveInitGlobalVarCheck(void);
#endif

IFX_LOCAL_INLINE void Spi_lSlaveDmaCfgMeErrIntr(uint8 Enable);
static void Spi_lSlaveDmaSetSourceAddr(Dma_ChannelType Channel,\
                                              uint32 Address,uint8 ApiAccessId);
static void Spi_lSlaveDmaChClrCtlCfg(Dma_ChannelType Channel,uint8 ApiAccessId);
static void Spi_lSlaveDmaChClrIntrFlags(Dma_ChannelType Channel,\
                                                             uint8 ApiAccessId);
IFX_LOCAL_INLINE void Spi_lSlaveDmaEnTransReqLostIntr(Dma_ChannelType Channel);
IFX_LOCAL_INLINE void Spi_lSlaveDmaCfgIntControl(Dma_ChannelType Channel,
                                                  uint32 value);
IFX_LOCAL_INLINE void Spi_lSlaveDmaEnableChIntrTrigger(Dma_ChannelType Channel);
IFX_LOCAL_INLINE void Spi_lSlaveDmaSetDestAddr(Dma_ChannelType Channel,   \
                                                        uint32 Address);
IFX_LOCAL_INLINE void Spi_lSlaveDmaClrAdcr(Dma_ChannelType Channel);
IFX_LOCAL_INLINE void Spi_lSlaveDmaChClrIntctEtrlFlags(Dma_ChannelType    \
                                                               Channel);
IFX_LOCAL_INLINE void Spi_lSlaveDmaDisableChIntrTrig(Dma_ChannelType   \
                                                               Channel);
IFX_LOCAL_INLINE void Spi_lSlaveDmaClrErrFlags(Dma_ChannelType Channel);

#define SPI_STOP_SEC_CODE
/*
  Allows to map variables, constants and code of modules to individual
  memory sections.
*/
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
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*
 * To be used for all global or static variables (unspecified size )
 * that are never  initialized.
 */
#define SPI_START_SEC_ASIL1_VAR_UNSPECIFIED
/*
  Allows to map variables, constants and code of modules to individual
  memory sections.
*/
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

static Spi_JobAndSeqAccessType Spi_SlaveJobAndSeqAccess;

#if (SPI_SLAVE_ENABLE == STD_ON)
Spi_SlaveChnlAccessType Spi_SlaveChnlAccess[SPI_NUM_ASIL_SLAVE_MODULES];
#endif

/* EB Buffer Parameters are stored */
static Spi_SlaveEBBufferType Spi_SlaveEBBuffer[SPI_MAXIMUM_CHANNEL];
static volatile Spi_StatusType Spi_SlaveBusStatus;
static Spi_AsilSeqStatusType Spi_SlaveSeqStatus[SPI_SEQUENCE_ASIL_ARRAY_INDEX];

#define SPI_STOP_SEC_ASIL1_VAR_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

#define SPI_START_SEC_ASIL1_VAR_FAST_32BIT
/*
  Allows to map variables, constants and code of modules to individual
  memory sections.
*/
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

/* Variable Holds the Configuration Pointer given in Spi_lSlaveInit */
static const Spi_ConfigType* Spi_kSlaveConfigPtr;

/* Variable to Hold Job Result. */
uint32 Spi_SlaveJobResult[SPI_JOB_ASIL_ARRAY_INDEX];

#define SPI_STOP_SEC_ASIL1_VAR_FAST_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*Memory Map of the SPI Code*/
#define SPI_START_SEC_CODE
/*
  Allows to map variables, constants and code of modules to individual
  memory sections.
*/
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
  allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
** Traceability :                                                             **
** Syntax : Std_ReturnType Spi_lSlaveInit(const Spi_ConfigType* ConfigPtr)    **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  None                                                          **
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
** Description :Initialization Service for SPI SLAVE .                        **
** The Initialization service initializes all SPI SLAVE relevant              **
** registers and global variables with the values of the structure            **
** referenced by the parameter ConfigPtr.                                     **
*******************************************************************************/
Std_ReturnType Spi_lSlaveInit(const Spi_ConfigType* ConfigPtr)
{
  uint32 Index;
  #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
  uint32 JobReIndex;
  #endif
  uint32 ReadBack;
  Spi_SlaveEBBufferType* EBBufferPtr;
  const Spi_HWModuleConfigType* HWModuleConfigPtr;
  Ifx_QSPI_GLOBALCON GlobalConReg;
  Ifx_QSPI* ModulePtr;
  #if(SPI_RESET_SFR_INIT == STD_ON)
  uint32 LoopCounter;
  #endif
  Std_ReturnType RetVal = E_OK;

  /* Enter Critical Section */
  SchM_Enter_Spi_Init();

  /* Get QSPIx Configuration Pointer */
  HWModuleConfigPtr = (ConfigPtr->HWModuleConfigPtr[SPI_SLAVE_MODULE_INDEX]) ;

  /* Reset End Init Protection to access regsiters */
  SPI_SFR_INIT_RESETENDINIT();

  /* SPI QSPIx Module Initialisation within end init protection */
  /* Initialize CLC Register - Enable QSPIx Module, Set Sleep Settings */
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_SFR_INIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].CLC.U,\
                                    HWModuleConfigPtr->HWClkSetting);

  /* Reset End Init Protection to access regsiters */
  SPI_SFR_INIT_SETENDINIT();

  /* Fix for CPU Bug CPU_TC.H002 */
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  ReadBack = (uint32)((SPI_SFR_INIT_USER_MODE_READ32\
             (SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].CLC.U) & SPI_CLC_CLEARMASK)\
                                                         >>SPI_CLC_DISS_BITPOS);

  #if(IFX_SPI_DEBUG_CLC == STD_ON)
  ReadBack |= Spi_IfxDebugClcSlave;
  #endif

  if(ReadBack == 1U)
  {
     RetVal = E_NOT_OK;
  }

  if(RetVal == E_OK)
  {

    /*  ------------------------------------------------------------------ **
    **                 SPI Slave Channel Initialization                    **
    **  ------------------------------------------------------------------ */

    Index = (uint32)(ConfigPtr->NoOfChannels) - (uint32)(Spi_NoOfIBChannels);
    do
    {
      Index--;
      /* Get Slave EB_Buffer Pointer */
      EBBufferPtr = &(Spi_SlaveEBBuffer[Index]);
      EBBufferPtr->DestPtr = NULL_PTR;  /* Set EB Destination Pointer to NULL*/
      EBBufferPtr->Length  = (Spi_NumberOfDataType)0U;/* Set EB Length to 0 */
    }while (Index > 0U);

    /*  ------------------------------------------------------------------ **
    **                 Global Variable Initialization                      **
    **  ------------------------------------------------------------------ */

    Spi_kSlaveConfigPtr = ConfigPtr;  /* Store ConfigPtr for use by APIs*/
    Spi_SlaveBusStatus = SPI_IDLE; /* Set SPI Slave Bus Status to Idle */

    /* Rx Dma Channel no's are copied to Global variable from Configuration*/
    Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel =
                       ConfigPtr->HWModuleConfigPtr[SPI_SLAVE_MODULE_INDEX]-> \
                        SpiDmaConfigPtr->RxDmaChannel;
    #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
    Index = ConfigPtr->NoOfSequences; /* Set Index to the last Sequence */
    do
    {
      Index--;
      /* Set each Sequence Status to SPI_SEQ_OK */
      Spi_SlaveJobAndSeqAccess.CurrentJobIndex[Index] = 0U;
    }while (Index > 0U);

    JobReIndex = 0U;
    #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
    do
    {
    #endif
      Spi_SlaveJobAndSeqAccess.JobQueueRearrange[JobReIndex]          = 0U;
      Spi_SlaveJobAndSeqAccess.JobLinkedSequenceRearrange[JobReIndex] = 0U;
    #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
      JobReIndex++;
    }while(JobReIndex < SPI_MAX_JOB_TRIG_Q_LENGTH);
    #endif
    #else
    Spi_SlaveJobAndSeqAccess.CurrentJobIndex = 0U;
    #endif

    Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId = (Spi_JobType)0U;

    Spi_SlaveJobAndSeqAccess.EndIndex = (Spi_JobType)0U;

    /* Set Sequence Status to not active in the Sequence Queue */
    Index = 0U;
    #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
    do
    {
    #endif
      Spi_SlaveJobAndSeqAccess.JobLinkedSeq[Index] = SPI_SEQ_IDLE_ID;
      Spi_SlaveJobAndSeqAccess.JobQueue[Index] = SPI_JOB_IDLE_ID;
    #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
      Index++;
    }while (Index < SPI_MAX_JOB_TRIG_Q_LENGTH);
    #endif

    /* Job Result status to SPI_JOB_OK */
    Spi_lSlaveClearJobResultStatus();
    /* Sequence status to SPI_SEQ_OK */
    Spi_lSlaveClearSequenceStatus();

    /*  ------------------------------------------------------------------ **
    **                 SPI Slave HW Module and Dma initilisation           **
    **  ------------------------------------------------------------------ */

   #if(SPI_RESET_SFR_INIT == STD_ON)


    /* Reset End Init Protection to access regsiters */
    SPI_SFR_INIT_RESETENDINIT();

    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    SPI_SFR_INIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].KRST0.U,\
                                                        SPI_KERNEL_RESET_VALUE);

    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    ReadBack = SPI_SFR_INIT_USER_MODE_READ32\
                                (SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].KRST0.U);
    UNUSED_PARAMETER(ReadBack)

    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    SPI_SFR_INIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].\
                                                KRST1.U,SPI_KERNEL_RESET_VALUE);

    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    ReadBack = SPI_SFR_INIT_USER_MODE_READ32(SPI_HW_MODULE\
                                              [SPI_SLAVE_MODULE_INDEX].KRST1.U);

   /* Set End Init Protection */
    SPI_SFR_INIT_SETENDINIT();

    UNUSED_PARAMETER(ReadBack)

    LoopCounter = SPI_KERNEL_RESET_WAIT_COUNT;

    do
    {
      LoopCounter--;
      #if (IFX_SPI_DEBUG_RST == STD_ON)
      ReadBack = Spi_IfxDebugStatus;
      #else
      /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic
        is used to efficiently access the SFRs of multiple QSPI Modules.*/
      ReadBack = SPI_SFR_INIT_USER_MODE_READ32(SPI_HW_MODULE\
                                             [SPI_SLAVE_MODULE_INDEX].KRST0.U);
      #endif
    }while((ReadBack != SPI_KERNEL_RESET_STATUS) && (LoopCounter > 0U));

    /* Reset End Init Protection to access regsiters */
    SPI_SFR_INIT_RESETENDINIT();

  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    SPI_SFR_INIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].KRSTCLR.U,\
                                                        SPI_KERNEL_RESET_VALUE);
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    ReadBack = SPI_SFR_INIT_USER_MODE_READ32(SPI_HW_MODULE\
                                            [SPI_SLAVE_MODULE_INDEX].KRSTCLR.U);

    /* Set End Init Protection */
    SPI_SFR_INIT_SETENDINIT();

    UNUSED_PARAMETER(ReadBack)

   #endif /* SPI_RESET_SFR_INIT == STD_ON */

    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    SPI_SFR_INIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].\
                                            GLOBALCON.U,SPI_QSPI_RESET_MODULE);

    /* Select Lines (A/B) for MOSI */
    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    SPI_SFR_INIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].PISEL.U,\
                      (((uint32)SPI_SLAVE_SRIS_SEL << SPI_SLAVE_SRIS_BITPOS) | \
                        ((uint32)SPI_SLAVE_SCIS_SEL << SPI_SLAVE_SCIS_BITPOS)));

   #if(SPI_BAUDRATE_AT_RUNTIME == STD_OFF)
    /*Set the ECON register values */
    Spi_lSlaveHwSetConfigExtn(ConfigPtr);
   #endif /* SPI_BAUDRATE_AT_RUNTIME == STD_OFF */

    Spi_lSlaveHwInitIntr(SPI_SLAVE_MODULE_INDEX);

    Spi_lSlaveHwDmaInit();

    /*  ------------------------------------------------------------------ **
    **              Enable QSPI as SLAVE                                   **
    **  ------------------------------------------------------------------ */

    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    ModulePtr =  &(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX]);

    GlobalConReg.U = SPI_SFR_INIT_USER_MODE_READ32(ModulePtr->GLOBALCON.U);

    GlobalConReg.B.MS = SPI_QSPI_SLAVE_SELECT;

    SPI_SFR_INIT_WRITE32(ModulePtr->GLOBALCON.U,GlobalConReg.U);
  }

  /* Exit Critical Section */
  SchM_Exit_Spi_Init();

  return RetVal;
}
/*******************************************************************************
** Syntax : void Spi_lSlaveDeInit (void)                                      **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  1                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : None                                                     **
**                                                                            **
** Description : Service For SPI SLAVE Deinitialization                       **
**                                                                            **
*******************************************************************************/
void Spi_lSlaveDeInit(void)
{
  #if ((SPI_LEVEL_DELIVERED == 1U) || (SPI_LEVEL_DELIVERED == 2U))
  uint32 Index;
  #endif
  volatile uint32 ReadBack;

  Ifx_QSPI* ModulePtr;
  uint8 HwChnlIdx;

  /*  ------------------------------------------------------------------ **
  **                 Global Variable Initialization                      **
  **  ------------------------------------------------------------------ */

  Spi_SlaveBusStatus = SPI_IDLE; /* Set SPI Slave Bus Status to Idle */

  #if ((SPI_LEVEL_DELIVERED == 1U) || (SPI_LEVEL_DELIVERED == 2U))

  #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
  /* Set Index to the last Sequence */
  Index = Spi_kSlaveConfigPtr->NoOfSequences;
  do
  {
    Index--;
    /* Set each Sequence Status to SPI_SEQ_OK */
    Spi_SlaveJobAndSeqAccess.CurrentJobIndex[Index] = 0U;
  }while (Index > 0U);
  #else
  Spi_SlaveJobAndSeqAccess.CurrentJobIndex = 0U;
  #endif

  Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId = (Spi_JobType)0U;

  Spi_SlaveJobAndSeqAccess.EndIndex = (Spi_JobType)0U;

  /* Set Sequence Status to not active in the Sequence Queue */
  Index = 0U;
  #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
  do
  {
  #endif
    Spi_SlaveJobAndSeqAccess.JobLinkedSeq[Index] = SPI_SEQ_IDLE_ID;
    Spi_SlaveJobAndSeqAccess.JobQueue[Index] = SPI_JOB_IDLE_ID;
  #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
    Index++;
  }while (Index < SPI_MAX_JOB_TRIG_Q_LENGTH);
  #endif

  #endif

  /* Reset Global Config Pointer */
  Spi_kSlaveConfigPtr = NULL_PTR;

  /*  ------------------------------------------------------------------ **
  **                 SPI Slave HW Module and Dma DeInitilisation         **
  **  ------------------------------------------------------------------ */

  /* Enter Critical Section */
  SchM_Enter_Spi_DeInit();

  /* Reset End Init Protection to access regsiters */
  SPI_SFR_DEINIT_RESETENDINIT();

  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_SFR_DEINIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].GLOBALCON.U,\
                                                         SPI_QSPI_RESET_MODULE);

  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used
    to efficiently access the SFRs of multiple QSPI Modules.*/
  ModulePtr =  &(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX]);
  for(HwChnlIdx = 0U; HwChnlIdx < 8U; HwChnlIdx++)
  {
    SPI_SFR_DEINIT_WRITE32(ModulePtr->ECON[HwChnlIdx].U,SLAVE_QSPI_ECON_RSTVAL);
  }

  /* Set DISR to Disable the Module */
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_SFR_DEINIT_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].CLC.U,\
                                  SPI_QSPI_CLC_DISR_DISABLE);

  /* Fix for CPU Bug CPU_TC.H002 */
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  ReadBack = (uint32)SPI_SFR_DEINIT_USER_MODE_READ32\
                                 (SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].CLC.U);
  UNUSED_PARAMETER(ReadBack)

  /* Set End Init Protection */
  SPI_SFR_DEINIT_SETENDINIT();
  /*Reserve bit access is ensured for SRC_QSPImRX register*/
  SPI_SFR_DEINIT_MODIFY32(MODULE_SRC.QSPI.QSPI\
                    [SPI_SLAVE_MODULE_INDEX].RX.U,SPI_QSPI_RX_CLEARMASK,\
                                                          SPI_QSPI_RX_SETMASK2)
  /*  Disable intr */
  /*Reserve bit access is ensured for SRC_QSPImRX register*/
  SPI_SFR_DEINIT_MODIFY32(MODULE_SRC.QSPI.QSPI\
                        [SPI_SLAVE_MODULE_INDEX].RX.U,SPI_QSPI_RX_CLEARMASK1,0U)

  Spi_lSlaveHwDmaDeInit();

  Spi_SlaveChnlAccess[0].DmaChannelIdx.RxDmaChannel = DMA_CHANNEL_INVALID;

  /* Exit Critical Section */
  SchM_Exit_Spi_DeInit();
} /* End of Function: Spi_DeInit */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Spi_lSlaveClearJobResultStatus(void)        **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : None.                                                    **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Function to Set Job Result status to SPI_JOB_OK.             **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveClearJobResultStatus(void)
{
  uint16 Index;
  Index = 0U;

  #if (SPI_JOB_ASIL_ARRAY_INDEX > 1U)
  do
  {
  #endif
    Spi_SlaveJobResult[Index] = 0U; /*Set Job Result status to SPI_JOB_OK */
    #if (SPI_JOB_ASIL_ARRAY_INDEX > 1U)
        Index++;
  }while (Index < SPI_JOB_ASIL_ARRAY_INDEX);
    #endif

}
#if ((SPI_CANCEL_API == STD_ON) && (SPI_LEVEL_DELIVERED != 0U))
/*******************************************************************************
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveClrSeqCancelStatus     **
** (                                                                          **
**   Spi_SequenceType Seq                                                     **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Non-Reentrant                                           **
**                                                                            **
** Parameters (in)  : Seq - Sequence Id                                       **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Clear Sequence Canceled State                           **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveClrSeqCancelStatus(Spi_SequenceType Seq)
{
  uint16 Index;

  /* ASIL or QM sequence index from map configuration*/
  Index = (uint16)((uint16)Spi_kSlaveConfigPtr->SpiMapConfigPtr->SeqIndex[Seq]
            >> SPI_SEQUENCE_INDEX);

  Spi_SlaveSeqStatus[Index].AsilSeqCancelStatus =
        Spi_SlaveSeqStatus[Index].AsilSeqCancelStatus & \
        (~((uint32)SPI_SEQ_CANCELED << (SPI_SEQUENCE_STATUS_BITS * \
        (Seq & SPI_SEQUENCE_BIT_POSITION))));

}
#endif
/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Spi_lSlaveClearSequenceStatus(void)         **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : None.                                                    **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
** Description : Function to Set Job Result status to SPI_SEQ_OK.             **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveClearSequenceStatus(void)
{
  uint32 IndexValue;
  IndexValue = 0U;

  #if (SPI_SEQUENCE_ASIL_ARRAY_INDEX > 1U)
  do
  {
  #endif
    /* Set Sequence Status SPI_SEQ_OK */
    Spi_SlaveSeqStatus[IndexValue].AsilSeqStatus = 0U;
    #if ((SPI_CANCEL_API == STD_ON) && (SPI_LEVEL_DELIVERED != 0U))
    /* Clear the Status Variable */
    Spi_SlaveSeqStatus[IndexValue].AsilSeqCancelStatus = 0U;
    #endif
    /* ((SPI_CANCEL_API == STD_ON) && (SPI_LEVEL_DELIVERED != 0)) */
    #if (SPI_SEQUENCE_ASIL_ARRAY_INDEX > 1U)
    IndexValue++;
  }while (IndexValue < SPI_SEQUENCE_ASIL_ARRAY_INDEX);
  #endif
}
/*******************************************************************************
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveSetSeqStatus           **
** (                                                                          **
**   Spi_SequenceType Seq, Spi_SeqResultType Status                           **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Non-Reentrant                                           **
**                                                                            **
** Parameters (in)  : Seq - Seq Id                                            **
**                Status - Sequence status                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Set the sequence status                                 **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveSetSeqStatus
(
Spi_SequenceType Seq,
Spi_SeqResultType Status
)
{
  uint8 Index;

  /* ASIL or QM sequence index from map configuration*/
  Index = (uint8)(Spi_kSlaveConfigPtr->SpiMapConfigPtr->SeqIndex[Seq]
            >> SPI_SEQUENCE_INDEX);

  Spi_SlaveSeqStatus[Index].AsilSeqStatus =     \
         (Spi_SlaveSeqStatus[Index].AsilSeqStatus & \
          ~((uint32)SPI_SEQUENCE_STATUS_EXTRACT <<
           (SPI_SEQUENCE_STATUS_BITS * ((Seq) & SPI_SEQUENCE_BIT_POSITION)))) |
           ((uint32)((uint32)Status & SPI_SEQUENCE_STATUS_BITMASK) <<
            (SPI_SEQUENCE_STATUS_BITS * ((Seq) & SPI_SEQUENCE_BIT_POSITION)));

}
/*******************************************************************************
** Syntax          : IFX_LOCAL_INLINE Spi_SeqResultType Spi_lSlaveGetSeqStatus**
** (                                                                          **
**   Spi_SequenceType Seq                                                     **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Seq - Seq Id                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : sequence result.                                        **
**                                                                            **
** Description      : Returns the sequence status                             **
*******************************************************************************/
IFX_LOCAL_INLINE Spi_SeqResultType Spi_lSlaveGetSeqStatus(Spi_SequenceType Seq)
{
  Spi_SeqResultType RetVal = SPI_SEQ_OK;
  uint8 Index;

  /* ASIL or QM sequence index from map configuration*/
  Index = (uint8)(Spi_kSlaveConfigPtr->SpiMapConfigPtr->SeqIndex[Seq]
            >> SPI_SEQUENCE_INDEX);

  RetVal = (Spi_SeqResultType)((Spi_SlaveSeqStatus[Index].AsilSeqStatus >> \
    (SPI_SEQUENCE_STATUS_BITS * ((uint32)Seq &  SPI_SEQUENCE_BIT_POSITION))) \
        & SPI_SEQUENCE_STATUS_EXTRACT);

   return(RetVal);
}

/*******************************************************************************
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveSetSeqStatusAtomic     **
** (                                                                          **
**   Spi_SequenceType Seq, Spi_SeqResultType Status                           **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Seq - Sequence Id                                       **
**                Result - Sequence result                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : sequence result.                                        **
**                                                                            **
** Description      : set the Sequence result atomically                      **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveSetSeqStatusAtomic(Spi_SequenceType Seq,
                              Spi_SeqResultType Status)
{
  uint16 Index;
  uint32 Temp;
  sint32 *Ptr;

  Index = (uint16)((uint16)Spi_kSlaveConfigPtr->SpiMapConfigPtr->SeqIndex[Seq]
            >> SPI_SEQUENCE_INDEX);
  Ptr = (sint32*)(void*) &Spi_SlaveSeqStatus[Index].AsilSeqStatus;
  Temp = ((uint32)SPI_SEQUENCE_STATUS_BITS *
                  (uint32)((uint32)(Seq) & (uint32)SPI_SEQUENCE_BIT_POSITION));
  Mcal_SetAtomic(
       Ptr,
       (uint32)(Status),
       (sint32)Temp,
       2);
}

/*******************************************************************************
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveSetJobStatusAtomic     **
** (                                                                          **
**   Spi_JobType Job, Spi_JobResultType Result                                **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Job - Job Id                                            **
**                Result - job result                                         **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : sequence result.                                        **
**                                                                            **
** Description      : set the Job result atomically                           **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveSetJobStatusAtomic
(
  Spi_JobType Job,
  Spi_JobResultType Result
)
{
  uint16 Index;
  uint32 Temp;
  sint32 *Ptr;

  /* ASIL or QM Job index from map configuration*/
  Index = (uint16)(Spi_kSlaveConfigPtr->SpiMapConfigPtr->JobIndex[Job]
            >> SPI_JOB_INDEX);

  Ptr = (sint32*)(void*) &Spi_SlaveJobResult[Index];
  Temp = ((uint32)SPI_JOB_STATUS_BITS *
                       (uint32)((uint32)(Job) & (uint32)SPI_JOB_BIT_POSITION));

  Mcal_SetAtomic(
           Ptr,\
           (uint32)(Result),\
           (sint32)Temp,\
           2);
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwInitIntr              **
** (                                                                          **
**   uint32 Module                                                            **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : synchronous                                             **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Hw module no.                                  **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Initialise Spi Slave Hw Interrupts during   **
**                    Hw initialisation                                       **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveHwInitIntr(uint32 Module)
{
  /*Reserve bit access is ensured for SRC_QSPImRX register*/
  SPI_SFR_INIT_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].RX.U,\
                                                SPI_QSPI_RX_CLEARMASK,\
                                                           SPI_QSPI_RX_SETMASK2)
  SPI_SFR_INIT_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].RX.U,\
                                     SPI_QSPI_RX_CLEARMASK,SPI_QSPI_RX_SETMASK3)
  SPI_SFR_INIT_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].RX.U,\
                                     SPI_QSPI_RX_CLEARMASK,SPI_QSPI_RX_SETMASK4)
  SPI_SFR_INIT_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].RX.U,\
                                                      SPI_QSPI_RX_CLEARMASK1,0U)
   /*  Enable intr */

}

/*******************************************************************************
** Syntax           : static void Spi_lSlaveHwDmaInit                         **
** (                                                                          **
**   void                                                                     **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : None.                                                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Initialise Dma Hw used for Spi Slave during **
**                    Hw initialisation                                       **
*******************************************************************************/
static void Spi_lSlaveHwDmaInit(void)
{
  Dma_ChannelType DmaChannel;
  uint32 Address;

  DmaChannel = Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;

#if(SPI_RESET_SFR_INIT == STD_ON)
  /* Reset for Slave Rx Dma channel */
  SPI_SFR_INIT_WRITE32(MODULE_DMA.ERRINTR.U,SPI_DMA_RESET_VALUE);
  SPI_SFR_INIT_WRITE32(MODULE_DMA.CH[DmaChannel].ADICR.U,\
                                                        SPI_DMA_RESET_VALUE);
  SPI_SFR_INIT_WRITE32(MODULE_DMA.CH[DmaChannel].CHCSR.U,\
                                                        SPI_DMA_RESET_VALUE);
  /*Reserve bit access is ensured for DMA_TSRz register*/
  SPI_SFR_INIT_WRITE32(MODULE_DMA.TSR[DmaChannel].U,\
                                                        SPI_DMA_RESET_VALUE);
  SPI_SFR_INIT_WRITE32(MODULE_DMA.CH[DmaChannel].SADR.U,\
                                                        SPI_DMA_RESET_VALUE);
  SPI_SFR_INIT_WRITE32(MODULE_DMA.CH[DmaChannel].CHCFGR.U,\
                                                        SPI_DMA_RESET_VALUE);
  SPI_SFR_INIT_WRITE32(MODULE_DMA.CH[DmaChannel].DADR.U,\
                                                        SPI_DMA_RESET_VALUE);
#endif /* Reset at Init ON */

  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  Address = ((uint32)&(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].RXEXIT.U));

  /* Enable Dma Interrupt */
  Spi_lSlaveDmaCfgMeErrIntr(1U);

  /* Set Source Address */
  Spi_lSlaveDmaSetSourceAddr(DmaChannel,Address,SPI_INIT_ACCESS);

  /* Init channel control register */
  Spi_lSlaveDmaChClrCtlCfg(Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel,\
                                                               SPI_INIT_ACCESS);

  Spi_lSlaveDmaChClrIntrFlags(Spi_SlaveChnlAccess[0U].                    \
                                   DmaChannelIdx.RxDmaChannel,SPI_INIT_ACCESS);

  /* DMA Error Initialization */
  Spi_lSlaveDmaEnTransReqLostIntr(Spi_SlaveChnlAccess[0U].DmaChannelIdx.  \
                                                          RxDmaChannel);

  /* Set Interrupt Control  */
  Spi_lSlaveDmaCfgIntControl(                   \
            Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel,SPI_DMA_INTCT);

  /* Enable RX DMA Ch Interrupt Only */
  Spi_lSlaveDmaEnableChIntrTrigger(Spi_SlaveChnlAccess[0U].               \
                                                  DmaChannelIdx.RxDmaChannel);

  Spi_lSlaveHwInitDmaIntr();
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwInitDmaIntr           **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Initialise Dma Hw Interrupts                **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveHwInitDmaIntr(void)
{
  Ifx_SRC_SRCR IntrSrc;
  uint32 RxChannelOffset;
  volatile Ifx_SRC_SRCR_Bits *MoveEngCh0BaseAddr;

  RxChannelOffset = (uint32)Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;
  MoveEngCh0BaseAddr = SRC_DMACH0ADDR;

  /*IFX_MISRA_RULE_17_04_STATUS="MoveEngCh0BaseAddr" Pointer arithmetic is used
    to efficiently access the SFRs of multiple QSPI Modules.*/
  IntrSrc.U = (unsigned_int)SPI_SFR_INIT_USER_MODE_READ32\
                                               ((*((volatile Ifx_SRC_SRCR*)\
                     (volatile void*)MoveEngCh0BaseAddr + RxChannelOffset)).U);
  IntrSrc.B.CLRR = 1U;
  IntrSrc.B.SWSCLR = 1U;
  IntrSrc.B.IOVCLR = 1U;
  IntrSrc.B.SRE = 1U;    /* Enable intr */
  IntrSrc.U &= SPI_DMACHOADDR_CLEARMASK;

  /*IFX_MISRA_RULE_17_04_STATUS="MoveEngCh0BaseAddr" Pointer arithmetic is used
    to efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_SFR_INIT_WRITE32((*((volatile Ifx_SRC_SRCR*)(volatile void *)\
                            MoveEngCh0BaseAddr + RxChannelOffset)).U,IntrSrc.U);
}
/*******************************************************************************
** Syntax           : static void Spi_lSlaveHwDmaDeInit                       **
** (                                                                          **
**   void                                                                     **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : None.                                                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to DeInitialise Dma Hw used for Spi            **
**                     Slave                                                  **
**                                                                            **
*******************************************************************************/
static void Spi_lSlaveHwDmaDeInit(void)
{
  Dma_ChannelType DmaChannel_rx = DMA_CHANNEL_INVALID;
  Ifx_SRC_SRCR IntrSrc;
  uint32 RxChannelOffset;
  volatile Ifx_SRC_SRCR_Bits *MoveEngCh0BaseAddr;

  DmaChannel_rx = Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;

  Spi_lSlaveDmaChClrCtlCfg(DmaChannel_rx,SPI_DEINIT_ACCESS);
  Spi_lSlaveDmaSetSourceAddr(DmaChannel_rx,0U,SPI_DEINIT_ACCESS);
  Spi_lSlaveDmaSetDestAddr(DmaChannel_rx,0U);
  Spi_lSlaveDmaClrAdcr(DmaChannel_rx);

  Spi_lSlaveDmaChClrIntctEtrlFlags(DmaChannel_rx);
  Spi_lSlaveDmaDisableChIntrTrig(DmaChannel_rx);
  Spi_lSlaveDmaChClrIntrFlags(DmaChannel_rx,SPI_DEINIT_ACCESS);

  /* Clear DMA Error Status Reg for Transaction Lost Error */
  Spi_lSlaveDmaClrErrFlags(DmaChannel_rx);

  RxChannelOffset = (uint32)Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;
  MoveEngCh0BaseAddr = SRC_DMACH0ADDR;

  /*IFX_MISRA_RULE_17_04_STATUS="MoveEngCh0BaseAddr" Pointer arithmetic is used
    to efficiently access the SFRs of multiple QSPI Modules.*/
  IntrSrc.U = (unsigned_int)SPI_SFR_DEINIT_USER_MODE_READ32\
                                               ((*((volatile Ifx_SRC_SRCR*)\
                      (volatile void*)MoveEngCh0BaseAddr + RxChannelOffset)).U);
  IntrSrc.B.CLRR = 1U;
  IntrSrc.B.SRE = 0U;    /* Enable intr */
  IntrSrc.U &= SPI_DMACHOADDR_CLEARMASK;
  /*IFX_MISRA_RULE_17_04_STATUS="MoveEngCh0BaseAddr" Pointer arithmetic is used
    to efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_SFR_DEINIT_WRITE32((*((volatile Ifx_SRC_SRCR*)(volatile void*)\
                           MoveEngCh0BaseAddr + RxChannelOffset)).U,IntrSrc.U);
}

/*******************************************************************************
** Syntax : Std_ReturnType Spi_SlaveSetupEB                                   **
** (                                                                          **
**   Spi_ChannelType Channel,                                                 **
**   Spi_DataType* DesDataBufferPtr,                                          **
**   Spi_NumberOfDataType DataLength                                          **
** )                                                                          **
** [/cover]                                                                   **
** Service ID:  0x11                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : Channel - Slave Channel ID.                              **
**                   DesDataBufferPtr - Pointer to Destination data           **
**                   buffer to store recieved data from SPI Master            **
**                   DataLength - Length of the data to be recieved           **
**                   on this channel                                          **
**                   Min.: 1                                                  **
**                   Max.: Max of data specified at configuration for         **
**                   this channel                                             **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : E_OK - Setup command has been accepted.                  **
**                   E_NOT_OK - Setup command has not been accepted.          **
**                                                                            **
** Description : Service to setup the buffers and the length of data          **
** for the External Buffer of SPI SLAVE Channel.                              **
** This service shall update the buffer pointers and lengh attributes         **
*******************************************************************************/
Std_ReturnType Spi_SlaveSetupEB
(
  Spi_ChannelType Channel,
  Spi_DataType* DesDataBufferPtr,
  Spi_NumberOfDataType DataLength
)
{

  uint32 RetVal;
  Spi_SlaveEBBufferType* EBBufferPtr;
  uint16 DataConfig;
  uint32 ReceiveCount;
  uint16 BufferLength;

  RetVal = E_OK;
  DataConfig = 0U;

  if ((Channel >= Spi_kSlaveConfigPtr->NoOfChannels) ||
     (Channel < (Spi_ChannelType)Spi_NoOfIBChannels))
  {
     /* Report to Safety error */
    SafeMcal_ReportError((uint16)SPI_MODULE_ID,SPI_INSTANCE_ID,
                    SPI_SLAVE_SID_SETUPEB,SPI_E_PARAM_CHANNEL);

    RetVal = E_NOT_OK;
  }
  else
  {
    /*IFX_MISRA_RULE_17_04_STATUS="SpiChannelConfigPtr" Pointer arithmetic used
      due to PBConfigStructure and is within allowed range.*/
    DataConfig = Spi_kSlaveConfigPtr->SpiChannelConfigPtr[Channel].DataConfig;

    ReceiveCount = Spi_lChkEBBufCont(Channel,DataConfig);

    if ((DataLength > ReceiveCount)
         ||(DataLength < (Spi_NumberOfDataType)1U))
    {
      /* Report to Safety error */
      SafeMcal_ReportError((uint16)SPI_MODULE_ID,SPI_INSTANCE_ID,
                       SPI_SLAVE_SID_SETUPEB,SPI_E_PARAM_LENGTH);
      RetVal = E_NOT_OK;
    }
  }

  if (RetVal == E_OK)
  {
    if (DataConfig & SPI_32BIT_DATAMASK)
    {
      /* Length/4U */
      BufferLength = (Spi_NumberOfDataType)(
                                   DataLength << SPI_RECEIVECOUNT_SHIFT_32BIT);
    }
    else if (DataConfig & SPI_16BIT_DATAMASK)
    {
      /* Length/2U */
      BufferLength = (Spi_NumberOfDataType)(
                                   DataLength << SPI_RECEIVECOUNT_SHIFT_16BIT);
    }
    else
    {
      BufferLength = DataLength;
    }

   /*IFX_MISRA_RULE_17_04_STATUS="DesDataBufferPtr" Pointer arithmetic used due
      to BufferStructure and is within allowed range.*/
    /*IFX_MISRA_RULE_17_04_STATUS="SpiChannelConfigPtr" Pointer arithmetic used
      due to PBConfigStructure and is within allowed range.*/
    if((DesDataBufferPtr == NULL_PTR)
         || (DesDataBufferPtr[BufferLength] !=
         Spi_kSlaveConfigPtr->SpiChannelConfigPtr[Channel].ChnlBufferMarker))
    {

      /* Report to Safety error */
      SafeMcal_ReportError((uint16)SPI_MODULE_ID,SPI_INSTANCE_ID,
                 SPI_SLAVE_SID_SETUPEB,SPI_E_PARAM_DEST_POINTER);

      RetVal = E_NOT_OK;
    }
  }

  if (RetVal == E_OK)
  {
    /* Get Slave EB Buffer Pointer */
    EBBufferPtr = &(Spi_SlaveEBBuffer[Channel]);

    EBBufferPtr->DestPtr = DesDataBufferPtr;

    if (DataConfig & SPI_32BIT_DATAMASK)
    {
      /* Length/4U */
      DataLength = (Spi_NumberOfDataType)(
                                   DataLength << SPI_RECEIVECOUNT_SHIFT_32BIT);
    }
    else if (DataConfig & SPI_16BIT_DATAMASK)
    {
      /* Length/2U */
      DataLength = (Spi_NumberOfDataType)(
                                   DataLength << SPI_RECEIVECOUNT_SHIFT_16BIT);
    }
    else
    {
      /*Do Nothing*/
    }

    EBBufferPtr->Length  = DataLength;

  }
  return ((Std_ReturnType)RetVal);

} /* End of Function: SpiSlave_SetUpEB */

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint32 Spi_lChkEBBufCont                         **
** (                                                                          **
**   Spi_ChannelType Channel,                                                 **
**   uint16 EBDatCfg                                                          **
** )                                                                          **
** [/cover]                                                                   **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : Channel - Slave Channel ID.                              **
**                   EBDatCfg -datawidth                                      **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : Count                                                    **
**                                                                            **
**                                                                            **
** Description : Service return count of buffer                               **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Spi_lChkEBBufCont(Spi_ChannelType Channel,
                                         uint16 EBDatCfg)
{

  uint32 Count;

  /* Data Width > 16 bits */
  if (EBDatCfg   & SPI_32BIT_DATAMASK)
  {
    /* Length/4U */
    /*IFX_MISRA_RULE_17_04_STATUS="SpiChannelConfigPtr" Pointer arithmetic used
      due to PBConfigStructure and is within allowed range.*/
    Count =
      ((uint32)Spi_kSlaveConfigPtr->SpiChannelConfigPtr[Channel].NoOfBuffers >>
                                SPI_RECEIVECOUNT_SHIFT_32BIT);
  }
  else if (EBDatCfg & SPI_16BIT_DATAMASK)
  {
    /* Length/2U */
    /*IFX_MISRA_RULE_17_04_STATUS="SpiChannelConfigPtr" Pointer arithmetic used
      due to PBConfigStructure and is within allowed range.*/
    Count =\
      ((uint32)Spi_kSlaveConfigPtr->SpiChannelConfigPtr[Channel].NoOfBuffers >>\
                                    SPI_RECEIVECOUNT_SHIFT_16BIT);
  }
  else
  {
    /*IFX_MISRA_RULE_17_04_STATUS="SpiChannelConfigPtr" Pointer arithmetic used
      due to PBConfigStructure and is within allowed range.*/
    Count = (uint32)Spi_kSlaveConfigPtr->SpiChannelConfigPtr[Channel].\
                                                                   NoOfBuffers;
  }

  return(Count);

}

/*******************************************************************************
** Syntax : static void Spi_lSlaveWriteTransmit (uint8 ApiAccessId)           **
                                                                              **
** [/cover]                                                                   **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) :  void                                                    **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : void                                                     **
**                                                                            **
**                                                                            **
** Description : Service to write into slave transmit buffer                  **
*******************************************************************************/
static void Spi_lSlaveWriteTransmit(uint8 ApiAccessId)
{
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_SFR_RUNTIME_USER_MODE_WRITE32(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].\
                                       DATAENTRY[0U].U,SPI_SLAVE_TRANSMIT_DATA);
  }
  else
  {
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].DATAENTRY[0U].U = \
                                             SPI_SLAVE_TRANSMIT_DATA;
  }

}

/*******************************************************************************
** Syntax : Std_ReturnType Spi_SlaveReadPrepare                               **
** (                                                                          **
**   Spi_SequenceType Sequence                                                **
** )                                                                          **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  0x12                                                          **
**                                                                            **
** Sync/Async:  Asynchronous                                                  **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : Sequence - Sequnece ID.                                  **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : E_OK - Reception command has been accepted.              **
**                   E_NOT_OK - Reception command has not been accepted.      **
**                                                                            **
** Description : Spi_SlaveReadPrepare to read/receive data on the SPI bus.    **
** This service takes over the given parameter 'Sequence', Queues jobs and    **
** Initializes QSPIx (connected to each job) to Slave Mode and starts         **
** receiving data on bus transmitted by QSPIy,configured in Master Mode.      **
**                                                                            **
** Jobs in each sequence are queued based on priority.                        **
**                                                                            **
** If Spi_SlaveReadPrepare service is called with new sequence when current   **
** sequence is ongoing then depending on configured parameter                 **
** 'Interruptible seq' or 'non-interruptible sequence' Jobs of each seq is    **
**  queued by calling local function Spi_JobSeqQueueUpdate.                   **
**                                                                            **
** This function is pre compile time selectable by the config parameter:      **
** SPI_LEVEL_DELIVERED. This function is only relevant for LEVEL 1 and  2.    **
**                                                                            **
** This service also updates slave bus status to SPI_BUSY, set the sequence   **
** result to SPI_SEQ_PENDING, set the first job result to                     **
** SPI_JOB_PENDING and return.                                                **
*******************************************************************************/
Std_ReturnType Spi_SlaveReadPrepare(Spi_SequenceType Sequence)
{
  uint32 RetVal;
  const Spi_SequenceConfigType* ParamSeqConfigPtr;
  uint32 HWUnit;
  uint32 SeqJobSharingStatus;
  uint16 JobId;
  uint16 FirstJobInQueue;
  uint8 IsQAvail;
  #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_OFF)
  uint16 Index;
  #endif
  /*(SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_OFF)*/
  uint32 JobCounter;

  JobCounter = 0U;
  RetVal = E_OK;

  /* Seq shall be within the specified range of values */
  if ((Sequence >= Spi_kSlaveConfigPtr->NoOfSequences))
  {

    /* Report to Safety error */
    SafeMcal_ReportError((uint16)SPI_MODULE_ID,SPI_INSTANCE_ID,
                  SPI_SLAVE_SID_READPREPARE,SPI_E_PARAM_SEQ);

    RetVal = E_NOT_OK;
  }

  if(RetVal == E_OK)
  {
   /*IFX_MISRA_RULE_17_04_STATUS="SpiSequenceConfigPtr" Pointer arithmetic used
      due to PBConfigStructure and is within allowed range.*/
    ParamSeqConfigPtr = &(Spi_kSlaveConfigPtr->SpiSequenceConfigPtr[Sequence]);
    IsQAvail = Spi_lIsQueueAvailable(ParamSeqConfigPtr->\
                                     JobsInParamSeq,&Spi_SlaveJobAndSeqAccess);

    if (IsQAvail == E_OK)
    {
      /* Get SPI SLAVE Bus */
      HWUnit = SPI_SLAVE_MODULE_INDEX;

     /* If the requested Sequence is already in the state SPI_SEQ_PENDING
     the SPI Slave does not take in account this new request and
     this service returns value E_NOT_OK. According to [SPI100],
     the SPI Handler/Driver shall report the SPI_E_SEQ_PENDING error.
     If the requested Sequence shares Jobs with another sequence
     that is in the state SPI_SEQ_PENDING, the SPI Handler/Driver does not
     take in account this new request and this service returns value E_NOT_OK.
     The SPI Slave shall report the SPI_E_SEQ_PENDNG error.*/

     /* ParamSeqConfigPtr->SeqSharingJobs : Holds for each sequence all the
     other sequences that shares a job with this sequence. The current
     sequence is also set.Spi_SeqStatus : Holds all pending sequences */

     if (SPI_SEQ_PENDING != (Spi_lSlaveGetSeqStatus(Sequence)))
     {
       SeqJobSharingStatus = Spi_lSlaveSeqSharingJobStatus(ParamSeqConfigPtr);
     }
     else
     {
       SeqJobSharingStatus = 1U;
     }

     if (SeqJobSharingStatus == 0U)
     {
       RetVal = E_OK;
       /* Set Sequence Status to Pending */
       /* set the sequence result to SPI_SEQ_PENDING */

       Spi_lSlaveSetSeqStatus(Sequence, SPI_SEQ_PENDING);

       /* If the bus attached to the lead job is free, start transmission
       immediately */

    #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_OFF)
       Index = Spi_SlaveJobAndSeqAccess.EndIndex;
       do
       {
         /* extract each job id from the sequence */
         /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due
           to PBConfigStructure and is within allowed range.*/
         JobId = ParamSeqConfigPtr->JobLinkPtr[JobCounter];
         Spi_SlaveJobAndSeqAccess.JobQueue[Index] = JobId;
         Spi_SlaveJobAndSeqAccess.JobLinkedSeq[Index] = Sequence;

         Spi_lSlaveSetJobStatusAtomic(JobId,SPI_JOB_QUEUED );
         JobCounter++;
         Index = Spi_lIncrementAndRoundIndex(Index);
      /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due to
         PBConfigStructure and is within allowed range.*/
       }while (ParamSeqConfigPtr->JobLinkPtr[JobCounter] != \
                                                 SPI_JOB_LINK_DELIMITER);
       Spi_SlaveJobAndSeqAccess.EndIndex = Index;
    #else
      do
      {
        /* extract each job id from the sequence */
        /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due
          to PBConfigStructure and is within allowed range.*/
        JobId = ParamSeqConfigPtr->JobLinkPtr[JobCounter];

        Spi_lSlaveSetJobStatusAtomic(JobId, SPI_JOB_QUEUED );
        JobCounter++;
      /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due to
        PBConfigStructure and is within allowed range.*/
      }while (ParamSeqConfigPtr->JobLinkPtr[JobCounter] != \
                                                 SPI_JOB_LINK_DELIMITER);

      Spi_lIntSeqAsyncTransmit(Sequence,&Spi_SlaveJobAndSeqAccess);
    #endif
     /*(SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_OFF)*/

     if (Spi_SlaveBusStatus == SPI_IDLE)
      {
        /* Store the pending Sequence and its first Job in the SPI Bus */
        /* set the SPI Handler/Driver status to SPI_BUSY */
        Spi_SlaveBusStatus = SPI_BUSY;

        /* To start the first job transmission */
        FirstJobInQueue = Spi_SlaveJobAndSeqAccess.JobQueue[0U];

        /* Set First Job Index in the Status Array */
        #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_OFF)
        Spi_SlaveJobAndSeqAccess.CurrentJobIndex = 0U;
        /* Set First Job Index in the Status Array */
        #else /* (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON) */
        Spi_SlaveJobAndSeqAccess.CurrentJobIndex[Sequence]= 0U;
        #endif

        #if ((SPI_CANCEL_API == STD_ON) && (SPI_LEVEL_DELIVERED != 0U))
        Spi_lSlaveClrSeqCancelStatus(Sequence);
        #endif

        /* Id should be incremented first and Job should be started.
             Reason:
             For an empty Queue, a request comes with no of jobs that is
             equal to the Queue size, in this scenario , the Queue should
             be assigned to full status before starting a job.
        */

        Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId++;
        Spi_lSlaveStartJob(HWUnit ,FirstJobInQueue,SPI_RUNTIME_ACCESS);
      }
      else
      {
        /* Set First Job Index in the Status Array */
        #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
        Spi_SlaveJobAndSeqAccess.CurrentJobIndex[Sequence] = 0U;
        #else
        Spi_SlaveJobAndSeqAccess.CurrentJobIndex = 0U;
        #endif
      }
     }
     else
     {
       RetVal = E_NOT_OK;
     }

    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  else
  {

    RetVal = E_NOT_OK;

  }

  Spi_lSlaveWriteTransmit(SPI_RUNTIME_ACCESS);

  return ((Std_ReturnType)RetVal);
}

/*******************************************************************************
** Syntax : static uint8 Spi_lSlaveSeqSharingJobStatus                        **
** (                                                                          **
**   Spi_SequenceConfigType* SeqConfigPtr                                     **
** )                                                                          **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : SeqConfigPtr - sequence configuration                    **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : return 1: jobs are shared across sequences               **
**                                                                            **
**                                                                            **
** Description : returns value 1, on finding the jobs shared by the sequences **
*******************************************************************************/
static uint8 Spi_lSlaveSeqSharingJobStatus
(
  const Spi_SequenceConfigType* SeqConfigPtr
)
{

  uint32 Index;
  Spi_SequenceType SeqId;
  uint8 JobSharingStatus;

  Index = 0U;
  JobSharingStatus = 0U;
  if (SeqConfigPtr->SeqSharingJobs != NULL_PTR)
  {
    do
    {
     /*IFX_MISRA_RULE_17_04_STATUS="SeqSharingJobs" Pointer arithmetic used due
        to PBConfigStructure and is within allowed range.*/
      SeqId = (Spi_SequenceType)SeqConfigPtr->SeqSharingJobs[Index];
      if (SPI_SEQ_PENDING == Spi_lSlaveGetSeqStatus(SeqId))
      {
        JobSharingStatus = 1U;
      }
      Index++;
    /*IFX_MISRA_RULE_17_04_STATUS="SeqSharingJobs" Pointer arithmetic used due
      to PBConfigStructure and is within allowed range.*/
    }while ((SeqConfigPtr->SeqSharingJobs[Index] != \
                   SPI_SHARED_JOBS_DELIMITER) && (JobSharingStatus == 0U));
  }/*(ParamSeqConfigPtr->SeqSharingJobs != NULL_PTR)*/
  return (JobSharingStatus);
}
/*******************************************************************************
** Syntax : static void Spi_lSlaveStartJob                                    **
** (                                                                          **
**   uint32 Module,                                                           **
**   Spi_JobType JobId                                                        **
** )                                                                          **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant  (for different JobIds)                            **
**                                                                            **
** Parameters (in) : Module - ModuleId                                        **
**                   Job    - JobId                                           **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : None                                                     **
**                                                                            **
**                                                                            **
** Description : Sets up the SPI Slave HW According to Job Parameters.        **
**               Call to Start Channel                                        **
**                                                                            **
** This function configures the Slave hardware (job related                   **
** configuration like timing parameters and selection of QSPI MODE i.e,       **
** Master or Slave) for the reception of data and calls the start channel     **
** function to configure data width.                                          **
*******************************************************************************/
static void Spi_lSlaveStartJob(uint32 Module, Spi_JobType JobId,\
                                                              uint8 ApiAccessId)
{
  const Spi_JobConfigType* JobConfigPtr;
  Spi_ChannelType Channel;
  Ifx_QSPI* ModulePtr;
  uint32 DelayAndHwChNo;

 /*IFX_MISRA_RULE_17_04_STATUS="SpiJobConfigPtr" Pointer arithmetic used due to
    PBConfigStructure and is within allowed range.*/
  JobConfigPtr = &(Spi_kSlaveConfigPtr->SpiJobConfigPtr[JobId]);
  Spi_SlaveChnlAccess[0U].CurrentChannelIndex = 0U;
  /*IFX_MISRA_RULE_17_04_STATUS="ChannelLinkPtr" Pointer arithmetic used due to
    PBConfigStructure and is within allowed range.*/
  Channel = JobConfigPtr->ChannelLinkPtr[0U];
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  ModulePtr =  &(SPI_HW_MODULE[Module]);

  /* set the first job result to SPI_JOB_PENDING */
  Spi_lSlaveSetJobStatusAtomic(JobId,SPI_JOB_PENDING);

  /* Configure the Job parameters like Mod En, baudrate (TQ, Q, A, B, C),
     Event enable (Error enable, TxEn, Rxen,
     EXPECT  = max_value, clock Phase & polarity
     Delay Parameters (Lead,Trail,Idle)
  */
  Spi_lSlaveHwSetJobConfig(ModulePtr,JobConfigPtr,ApiAccessId);

  Spi_lSlaveHwEnableQspiRxIntr(Module,ApiAccessId);

  Spi_lSlaveHwEnableQspiErrIntr(Module,ApiAccessId);

  DelayAndHwChNo = JobConfigPtr->TimeDelayConfig |
              (( ((uint32)JobConfigPtr->HwUnit >> SPI_HWUNIT_CH_SHIFT)
              &  SPI_HWUNIT_MASK) <<   SPI_CONFIG_HWCHANNELNO_SHIFT);
  Spi_lSlaveStartChannel(Module, Channel,DelayAndHwChNo,
                          ((uint8)(JobConfigPtr->ParitySelection)),ApiAccessId);
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwEnableQspiRxIntr      **
** (                                                                          **
**   uint32 Module                                                            **
**   uint8 ApiAccessId                                                        **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Hw module no.                                  **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Enable Spi Slave Hw Rx Interrupt            **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveHwEnableQspiRxIntr(uint32 Module,\
                                                              uint8 ApiAccessId)
{
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
    /*Reserve bit access is ensured for SRC_QSPImRX register*/
    /* Set SRC.IOVCLR, SRC.SWSCLR, SRC.CLRR */
    SPI_SFR_RUNTIME_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].RX.U,\
                                    SPI_QSPI_RX_CLEARMASK,SPI_QSPI_RX_SETMASK)

    SPI_SFR_RUNTIME_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].RX.U,\
                                  SPI_QSPI_RX_CLEARMASK,SPI_QSPI_RX_SETMASK1)
  }
  else
  {
    /*Reserve bit access is ensured for SRC_QSPImRX register*/
    /* Set SRC.IOVCLR, SRC.SWSCLR, SRC.CLRR */
    MODULE_SRC.QSPI.QSPI[Module].RX.U = (MODULE_SRC.QSPI.QSPI[Module].RX.U & \
                                    SPI_QSPI_RX_CLEARMASK) |SPI_QSPI_RX_SETMASK;

    MODULE_SRC.QSPI.QSPI[Module].RX.U = (MODULE_SRC.QSPI.QSPI[Module].RX.U & \
                                  SPI_QSPI_RX_CLEARMASK) | SPI_QSPI_RX_SETMASK1;
  }
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwDisableQspiRxIntr     **
** (                                                                          **
**   uint32 Module                                                            **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Hw module no.                                  **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Disable Spi Slave Hw Rx Interrupt           **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveHwDisableQspiRxIntr(uint32 Module)
{
  if((((MODULE_SRC.QSPI.QSPI[Module].RX.U) & \
                              SPI_QSPI_RX_SETMASK1)>>SPI_QSPI_RX_BITPOS)  == 1U)
  {
    /* Set SRC.IOVCLR, SRC.SWSCLR, SRC.CLRR */
    /* Clear SRC.SRE */
    /*Reserve bit access is ensured for SRC_QSPImRX register*/
    MODULE_SRC.QSPI.QSPI[Module].RX.U = (MODULE_SRC.QSPI.QSPI[Module].RX.U & \
                                  SPI_QSPI_RX_CLEARMASK1) |SPI_QSPI_RX_SETMASK;
  }
}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwEnableQspiErrIntr     **
** (                                                                          **
**   uint32 Module                                                            **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Hw module no.                                  **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Enable Spi Slave Hw Rx Interrupt            **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveHwEnableQspiErrIntr(uint32 Module,\
                                                              uint8 ApiAccessId)
{
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
    /* Set SRC.IOVCLR, SRC.SWSCLR, SRC.CLRR */
    /*Reserve bit access is ensured for SRC_QSPImERR register*/
    SPI_SFR_RUNTIME_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].ERR.U,\
                                    SPI_QSPI_ERR_CLEARMASK,SPI_QSPI_ERR_SETMASK)

    SPI_SFR_RUNTIME_MODIFY32(MODULE_SRC.QSPI.QSPI[Module].ERR.U,\
                                   SPI_QSPI_ERR_CLEARMASK,SPI_QSPI_ERR_SETMASK1)
  }
  else
  {
    /* Set SRC.IOVCLR, SRC.SWSCLR, SRC.CLRR */
    /*Reserve bit access is ensured for SRC_QSPImERR register*/
    MODULE_SRC.QSPI.QSPI[Module].ERR.U = (MODULE_SRC.QSPI.QSPI[Module].ERR.U & \
                                  SPI_QSPI_ERR_CLEARMASK) |SPI_QSPI_ERR_SETMASK;

    MODULE_SRC.QSPI.QSPI[Module].ERR.U = (MODULE_SRC.QSPI.QSPI[Module].ERR.U &
                                 SPI_QSPI_ERR_CLEARMASK)| SPI_QSPI_ERR_SETMASK1;
  }
}
/*******************************************************************************
** Syntax : static void Spi_lSlaveStartChannel                                **
** (                                                                          **
**   uint32 Module,                                                           **
**   Spi_ChannelType Channel, uint32 DelayAndHwChNo, uint8 ParityConfig       **
** )                                                                          **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant (for different channel Ids)                        **
**                                                                            **
** Parameters (in) :   Module - ModuleId                                      **
**                     ChannelType  - Channel                                 **
**                     DelayAndHwChNo - configured delay and channel          **
**                     ParityConfig - Parity selection                        **
**                                                                            **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    :                                                          **
**                                                                            **
**                                                                            **
** Description :This function configures the QSPI and Dma hardware for        **
**              the channel reception                                         **
*******************************************************************************/
static void Spi_lSlaveStartChannel(uint32 Module, Spi_ChannelType Channel,
       uint32 DelayAndHwChNo, uint8 ParityConfig,uint8 ApiAccessId)
{
  const Spi_ChannelConfigType* ChannelConfigPtr;
  Spi_DataType *DmaDestPtr;
  uint32 DmaReceiveCount;
  Spi_SlaveEBBufferType* EBBufferPtr;

  /*IFX_MISRA_RULE_17_04_STATUS="SpiChannelConfigPtr" Pointer arithmetic used
    due to PBConfigStructure and is within allowed range.*/
  ChannelConfigPtr = &(Spi_kSlaveConfigPtr->SpiChannelConfigPtr[Channel]);
  EBBufferPtr = &(Spi_SlaveEBBuffer[Channel]);
  DmaDestPtr = EBBufferPtr->DestPtr;
  DmaReceiveCount  = (uint32)EBBufferPtr->Length;

  /*Data Width > 16 bits*/
  if (ChannelConfigPtr->DataConfig & SPI_32BIT_DATAMASK)
  {
    /* DmaReceiveCount is div by 4 to convert transfers as 32 bit from 8 bit */
    DmaReceiveCount = (DmaReceiveCount >> SPI_RECEIVECOUNT_SHIFT_32BIT) ;
  }
  /*Data Width > 8 bits*/
  else if (ChannelConfigPtr->DataConfig & SPI_16BIT_DATAMASK)
  {
    /* DmaReceiveCount is div by 2 to convert transfers as 16 bit from 8 bit */
    DmaReceiveCount = (DmaReceiveCount >> SPI_RECEIVECOUNT_SHIFT_16BIT) ;
  }
  else
  {
      /* Data Width is equal to 8 bits */
  }

  Spi_lSlaveHwSetChannelConfig(Module,ChannelConfigPtr->DataConfig,
                           DelayAndHwChNo,ParityConfig,ApiAccessId);

  /* Start Dma Reception */
  Spi_lSlaveHwDmaStartReception(DmaDestPtr, DmaReceiveCount,
                          ChannelConfigPtr->DataConfig,ApiAccessId);

}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwDmaStartReception     **
** (                                                                          **
**   uint32 Module,                                                           **
**   Spi_DataType* DmaRxDestPtr,                                              **
**   uint32 DmaReceiveCount,                                                  **
**   uint16 DataWidth,                                                        **
** )                                                                          **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Hw Module.                                     **
**                    DmaRxDestPtr - Rx-Dma Channel Destination Pointer       **
**                    DmaReceiveCount - Dma Receive Count                     **
**                    DataWidth - DataWidth of Rx Dma channels                **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Start transfer of Dma Rx Channels.          **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveHwDmaStartReception
(
Spi_DataType* DmaRxDestPtr,
uint32 DmaReceiveCount,
uint16 DataWidth,
uint8 ApiAccessId
)
{
  Dma_ChannelType DmaRxChannel;
  uint32 DmaRxAddressControlRegVal;
  uint32 DmaDataWidth;

  DmaRxChannel = Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;

  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  /*IFX_MISRA_RULE_11_01_STATUS=Typecasting DmaRxDestPtr to uint32 is unvoidable
    DmaRxDestPtr is later used for SFR assignment.*/
  /*IFX_MISRA_RULE_11_03_STATUS=Type cast to uint32 is unavoidable*/
  SPI_SFR_RUNTIME_MCAL_DMASETDESTADDR(DmaRxChannel,(uint32)DmaRxDestPtr);
  }
  else
  {
  /*IFX_MISRA_RULE_11_01_STATUS=Typecasting DmaRxDestPtr to uint32 is unvoidable
    DmaRxDestPtr is later used for SFR assignment.*/
  /*IFX_MISRA_RULE_11_03_STATUS=Type cast to uint32 is unavoidable*/
  Mcal_DmaSetDestAddr(DmaRxChannel,(uint32)DmaRxDestPtr);
  }

  DmaRxAddressControlRegVal = SPI_DMA_RX_ADRCR_NORML_TRANSFER;

  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  /* Set Address Ctr Reg RX*/
  SPI_SFR_RUNTIME_MCAL_DMASETADICR(DmaRxChannel,DmaRxAddressControlRegVal);
  }
  else
  {
  Mcal_DmaSetAdicr(DmaRxChannel, DmaRxAddressControlRegVal);
  }

  /* Clear Pending Interrupt */
  Spi_lSlaveHwDmaClearIntr(DmaRxChannel,ApiAccessId);

  if ( DataWidth & SPI_32BIT_DATAMASK)
  {
    DmaDataWidth = SPI_DMA_CHDW_32BIT;
  }
  else if ( DataWidth & SPI_16BIT_DATAMASK)
  {
    DmaDataWidth = SPI_DMA_CHDW_16BIT;
  }
  else
  {
    DmaDataWidth = SPI_DMA_CHDW_8BIT;
  }
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
    /* SetChannel Control Reg : RX */
    SPI_SFR_RUNTIME_MCAL_DMASETTXCOUNT(DmaRxChannel,DmaReceiveCount);
    SPI_SFR_RUNTIME_MCAL_DMASETDATAWIDTH(DmaRxChannel,DmaDataWidth);

    /* Set Hardware Trigger Request on the Channels */
    SPI_SFR_RUNTIME_MCAL_DMAENABLEHWTRANSFER(DmaRxChannel);
  }
  else
  {
    Mcal_DmaSetTxCount(DmaRxChannel,DmaReceiveCount);
    Mcal_DmaSetDataWidth(DmaRxChannel,DmaDataWidth);
    /* Set Hardware Trigger Request on the Channels */
    Mcal_DmaEnableHwTransfer(DmaRxChannel);
  }
}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwDmaClearIntr          **
** (                                                                          **
**   uint32 Module                                                            **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Hw module no.                                  **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Clear Dma Hw channel Interrupt              **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveHwDmaClearIntr(Dma_ChannelType Channel,\
                                                              uint8 ApiAccessId)
{
  volatile Ifx_SRC_SRCR_Bits *MoveEngCh0BaseAddr;
  volatile uint32 * RegAddress;

  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  SPI_SFR_RUNTIME_MCAL_DMACHCLRINTCTETRLFLAGS(Channel);
  SPI_SFR_RUNTIME_MCAL_DMACHCLRINTRFLAGS(Channel);
  /* Set Interrupt Control */
  SPI_SFR_RUNTIME_MCAL_DMACFGINTCONTROL(Channel,SPI_DMA_INTCT);
  }
  else
  {
  Mcal_DmaChClrIntctEtrlFlags(Channel);
  Mcal_DmaChClrIntrFlags(Channel);
  Mcal_DmaCfgIntControl(Channel,SPI_DMA_INTCT);
  }
  MoveEngCh0BaseAddr = SRC_DMACH0ADDR;

  /* IntrSrc.CLRR = 1U; */
  /*IFX_MISRA_RULE_17_04_STATUS="MoveEngCh0BaseAddr" Pointer arithmetic
    is used to efficiently access the SFRs of multiple QSPI Modules.*/
  RegAddress =
    ((volatile uint32*)(volatile void *)MoveEngCh0BaseAddr + \
                                                       (uint8)Channel);
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
    SPI_SFR_RUNTIME_MODIFY32(*RegAddress,\
                                SPI_DMACHOADDR_CLEARMASK,SPI_DMACHOADDR_SETMASK)
  }
  else
  {
    *RegAddress =  *RegAddress | SPI_DMACHOADDR_SETMASK;
  }
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveDmaCfgMeErrIntr        **
** (                                                                          **
**   uint32 Enable                                                            **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Enable :  1 - Enable ME Error Interrupt.  0 - Disable   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to enable/Disable the Dma ME Error interrupt.  **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaCfgMeErrIntr(uint8 Enable)
{
  /* Errata :
     Byte and Half-word Write Accesses to specific Registers not supported
  */
  SPI_SFR_INIT_MODIFY32(MODULE_DMA.ERRINTR.U,SPI_DMAERRINTR_CLEARMASK,Enable)
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           : static void Spi_lSlaveDmaSetSourceAddr                  **
** (                                                                          **
**   Dma_ChannelType Channel, uint32 Address                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                    Address - Source address                                **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Update the Dma Source address register.                 **
*******************************************************************************/
static void Spi_lSlaveDmaSetSourceAddr(Dma_ChannelType Channel,
                                               uint32 Address,uint8 ApiAccessId)
{
  uint32 RemapAddress;
  if(ApiAccessId == SPI_INIT_ACCESS)
  {
    RemapAddress = SPI_SFR_INIT_USER_MODE_MCAL_GETDSPRREMAPADDRESS(Address);
    SPI_SFR_INIT_WRITE32(MODULE_DMA.CH[Channel].SADR.U,(uint32)RemapAddress);
  }
  else
  {
    RemapAddress = SPI_SFR_DEINIT_USER_MODE_MCAL_GETDSPRREMAPADDRESS(Address);
    SPI_SFR_DEINIT_WRITE32(MODULE_DMA.CH[Channel].SADR.U,(uint32)RemapAddress);
  }
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  static void Spi_lSlaveDmaChClrCtlCfg                   **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Clear the bitfields pertaining to different Channel     **
**                    control registers.                                      **
*******************************************************************************/
static void Spi_lSlaveDmaChClrCtlCfg(Dma_ChannelType Channel,\
                                                              uint8 ApiAccessId)
{
  uint32 ltemp;

  if(ApiAccessId == SPI_INIT_ACCESS)
  {
  SPI_SFR_INIT_WRITE32(MODULE_DMA.CH[Channel].CHCFGR.U,\
                                                      SPI_DMA_CHCFGR_RESETVAL);
   /*Reserve bit access is ensured for DMA_CHCSRz register*/
  SPI_SFR_INIT_MODIFY32(MODULE_DMA.CH[Channel].CHCSR.U,\
                                                 SPI_DMA_CHCSR_SCH_CLR_MASK,0U)

  ltemp = SPI_SFR_INIT_USER_MODE_READ32(MODULE_DMA.TSR[Channel].U);
  }
  else
  {
  SPI_SFR_DEINIT_WRITE32(MODULE_DMA.CH[Channel].CHCFGR.U,\
                                                      SPI_DMA_CHCFGR_RESETVAL);
   /*Reserve bit access is ensured for DMA_CHCSRz register*/
  SPI_SFR_DEINIT_MODIFY32(MODULE_DMA.CH[Channel].CHCSR.U,\
                                                 SPI_DMA_CHCSR_SCH_CLR_MASK,0U)

  ltemp = SPI_SFR_DEINIT_USER_MODE_READ32(MODULE_DMA.TSR[Channel].U);
  }
  /* Clear ECH bit, Set DCH bit */
  ltemp &= (uint32)(~((uint32)1U << IFX_DMA_TSR_ECH_OFF));
  ltemp |= ((uint32)1U << IFX_DMA_TSR_DCH_OFF);
  /* Reset */
  /*Reserve bit access is ensured for DMA_TSRz register*/
  if(ApiAccessId == SPI_INIT_ACCESS)
  {
  SPI_SFR_INIT_WRITE32(MODULE_DMA.TSR[Channel].U,ltemp);
  }
  else
  {
  SPI_SFR_DEINIT_WRITE32(MODULE_DMA.TSR[Channel].U,ltemp);
  }
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  static void Spi_lSlaveDmaChClrIntrFlags                **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Clear WRP Buffer Interrupt, Dma Channel Interrupt and   **
**                    TRL flags.                                              **
*******************************************************************************/
static void Spi_lSlaveDmaChClrIntrFlags(Dma_ChannelType Channel,\
                                                              uint8 ApiAccessId)
{
  if(ApiAccessId == SPI_INIT_ACCESS)
  {
  SPI_SFR_INIT_MODIFY32(MODULE_DMA.CH[Channel].CHCSR.U,SPI_DMACHCSR_CLEARMASK,\
                                                    SPI_DMA_CHCSR_CLR_INTFLAGS)
  /*Reserve bit access is ensured for DMA_TSRz register*/
  SPI_SFR_INIT_MODIFY32(MODULE_DMA.TSR[Channel].U,SPI_DMACHCSR_CLEARMASK1,\
                                                      SPI_DMA_TSR_CLR_INTFLAGS)
  }
  else
  {
  SPI_SFR_DEINIT_MODIFY32(MODULE_DMA.CH[Channel].CHCSR.U,\
                              SPI_DMACHCSR_CLEARMASK,SPI_DMA_CHCSR_CLR_INTFLAGS)
  /*Reserve bit access is ensured for DMA_TSRz register*/
  SPI_SFR_DEINIT_MODIFY32(MODULE_DMA.TSR[Channel].U,SPI_DMACHCSR_CLEARMASK1,\
                                                       SPI_DMA_TSR_CLR_INTFLAGS)
  }
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveDmaEnTransReqLostIntr  **
** (                                                                          **
**   uint32 DmaChannel                                                        **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : DmaChannel - Dma channel no.                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to enable the Dma Error interrupt.             **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaEnTransReqLostIntr(Dma_ChannelType Channel)
{
  /* DMA Error (Transaction req lost intr) enable  */
  SPI_SFR_INIT_MODIFY32(MODULE_DMA.CH[Channel].ADICR.U,\
                                    SPI_DMAADICR_CLEARMASK,SPI_DMAADICR_SETMASK)
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveDmaCfgIntControl       **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Configures Channel Interrupt Control with the
**                    given value                                             **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaCfgIntControl(Dma_ChannelType Channel,
                                                  uint32 value)
{
  value = value & IFX_DMA_BLK_ME_ADICR_INTCT_MSK;
  SPI_SFR_INIT_MODIFY32(MODULE_DMA.CH[Channel].ADICR.U,SPI_DMAADICR_CLEARMASK1,\
                                 ((uint32)value&3U)<<SPI_DMA_ADICR_INTCT_BITPOS)
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveDmaEnableChIntrTrigger **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Enable the Channel Interrupt Trigger.                   **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaEnableChIntrTrigger(Dma_ChannelType Channel)
{
  SPI_SFR_INIT_MODIFY32(MODULE_DMA.CH[Channel].CHCSR.U,\
                                   SPI_DMACHCSR_CLEARMASK1,SPI_DMACHCSR_SETMASK)
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           : uint32 Spi_lSlaveDmaGetTxIntStatus                      **
** (                                                                          **
**   void                                                                     **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : RetVal                                                  **
**                                                                            **
** Description      : return the Interrupt flag(ICH)          .               **
*******************************************************************************/
uint32 Spi_lSlaveDmaGetTxIntStatus(void)
{
  uint32 RetVal;
  Dma_ChannelType DmaRxChannel;

  DmaRxChannel = Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;

  RetVal = Mcal_DmaGetTransferIntStatus(DmaRxChannel);

  return RetVal;
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           : void Spi_lSlaveDmaClrIntrFlag                           **
** (                                                                          **
**   void                                                                     **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Clear the DMa ChannelInterrupt flag(Set CICH)           **
*******************************************************************************/
void Spi_lSlaveDmaClrIntrFlag(void)
{
  Dma_ChannelType DmaRxChannel;

  DmaRxChannel = Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;
  Mcal_DmaChClrIntrFlags(DmaRxChannel);
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveDmaSetDestAddr          **
** (                                                                          **
**   Dma_ChannelType Channel, uint32 Address                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                    Address - Destination address                           **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Update the Dma Destination address register.            **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaSetDestAddr(Dma_ChannelType Channel,
                                                      uint32 Address)
{
  uint32 RemapAddress;
  RemapAddress = SPI_SFR_DEINIT_USER_MODE_MCAL_GETDSPRREMAPADDRESS(Address);
  SPI_SFR_DEINIT_WRITE32(MODULE_DMA.CH[Channel].DADR.U,(uint32)RemapAddress);
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  void Spi_lSlaveDmaClrAdcr                              **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Clear the Address control part of ADICR .               **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaClrAdcr(Dma_ChannelType Channel)
{
  SPI_SFR_DEINIT_MODIFY32(MODULE_DMA.CH[Channel].ADICR.U,\
                                                      SPI_ADDR_CTRL_RESETVAL,0U)
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveDmaChClrIntctEtrlFlags  **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Clear the Channel Interrupt control flags.              **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaChClrIntctEtrlFlags(Dma_ChannelType Channel)
{
  SPI_SFR_DEINIT_MODIFY32(MODULE_DMA.CH[Channel].ADICR.U,\
                                              SPI_ADICR_INTCT_ETRL_CLR_MASK,0U)
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveDmaDisableChIntrTrig    **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Disables the Channel Interrupt Trigger.                 **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaDisableChIntrTrig
(
  Dma_ChannelType Channel
)
{
  SPI_SFR_DEINIT_MODIFY32(MODULE_DMA.CH[Channel].CHCSR.U,\
                                                     SPI_DMACHCSR_CLEARMASK2,0U)
}

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           :  IFX_LOCAL_INLINE void Spi_lSlaveDmaClrErrFlags         **
** (                                                                          **
**   Dma_ChannelType Channel                                                  **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Channel - Dma channel no.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Clear the Transaction request flag.                     **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveDmaClrErrFlags(Dma_ChannelType Channel)
{
    /*Reserve bit access is ensured for DMA_TSRz register*/
  SPI_SFR_DEINIT_MODIFY32(MODULE_DMA.TSR[Channel].U,\
                                        SPI_DMATSR_CLEARMASK,SPI_DMATSR_SETMASK)
}

/*******************************************************************************
** Syntax           : void Spi_lSlaveBusHandler                               **
**                    (                                                       **
**                      uint32 Module                                         **
**                    )                                                       **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-Reentrant                                           **
**                                                                            **
** Parameters (in)  : Module- SPI Slave Bus Id.                               **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : The Slave Bus Handler should be called from Isr of      **
** DMA RX when received data count is equal to DmaReceiveCount.               **
**                                                                            **
** The Slave Bus handler performs the following:                              **
** 1.Starts next Channel Reception                                            **
** 2.If the last channel is reached starts next Slave Job.                    **
**   (Schedule function is called)                                            **
** 3.If the last job is reached starts next pending sequence                  **
**   (Schedule function is called)                                            **
**                                                                            **
*******************************************************************************/
void Spi_lSlaveBusHandler(uint32 Module,uint8 ApiAccessId)
{
  uint32 HWUnit;
  uint16 JobId;
  uint16 SICompletedJob;
  Spi_SequenceType CompletedJobSeqId;
  Spi_ChannelType ChannelId;
  uint16 ExtractJobId;
  uint16 EndIndex;
  const Spi_JobConfigType* JobConfigPtr;
  const Spi_SequenceConfigType* SeqConfigPtr;
  uint32 DelayAndHwChNo;
  #if (SPI_CANCEL_API == STD_ON)
  Spi_JobAndSeqAccessType  *JobSeqDataPtr;
  #endif

  SICompletedJob = Spi_lSlaveCompletedJobId();
  /* last job completed */
  /* extract the completed Job Id */
  JobId = Spi_SlaveJobAndSeqAccess.JobQueue[SICompletedJob];
  /* Increment Channel Index to the next Channel in the Job Link */
  Spi_SlaveChnlAccess[0U].CurrentChannelIndex++;
 /*IFX_MISRA_RULE_17_04_STATUS="SpiJobConfigPtr" Pointer arithmetic used due to
    PBConfigStructure and is within allowed range.*/
  JobConfigPtr = &(Spi_kSlaveConfigPtr->SpiJobConfigPtr[JobId]);
  /*IFX_MISRA_RULE_17_04_STATUS="ChannelLinkPtr" Pointer arithmetic used due to
    PBConfigStructure and is within allowed range.*/
  ChannelId = JobConfigPtr->ChannelLinkPtr[Spi_SlaveChnlAccess[0U].\
                                                          CurrentChannelIndex];

  /*Last Channel already transmitted. i.e end of job/seq*/
  if (ChannelId == SPI_CHANNEL_LINK_DELIMITER)
  {
    #if (SPI_CANCEL_API == STD_ON)
    JobSeqDataPtr = &Spi_SlaveJobAndSeqAccess;
    #endif

    Spi_lHwDisableQspiErrIntr(Module);

    Spi_lSlaveHwDisableQspiRxIntr(Module);

    Spi_lSlaveSetJobStatusAtomic(JobId, SPI_JOB_OK);
    CompletedJobSeqId = Spi_SlaveJobAndSeqAccess.JobLinkedSeq[SICompletedJob];
   /*IFX_MISRA_RULE_17_04_STATUS="SpiSequenceConfigPtr" Pointer arithmetic used
      due to PBConfigStructure and is within allowed range.*/
    SeqConfigPtr = &(Spi_kSlaveConfigPtr->\
                                      SpiSequenceConfigPtr[CompletedJobSeqId]);

    #if (SPI_CANCEL_API == STD_ON)
    Spi_lSlaveSeqCancelled(JobSeqDataPtr);
    #endif

    #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
    /* Increment Job Index to the next Job in the Job Link */
    Spi_SlaveJobAndSeqAccess.CurrentJobIndex[CompletedJobSeqId]++ ;
    #else
    Spi_SlaveJobAndSeqAccess.CurrentJobIndex++;
    #endif
    /*(SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)*/

    #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
    /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due to
      PBConfigStructure and is within allowed range.*/
    if (SeqConfigPtr->JobLinkPtr[Spi_SlaveJobAndSeqAccess.\
        CurrentJobIndex[CompletedJobSeqId]] == SPI_JOB_LINK_DELIMITER)
    #else
    /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due to
      PBConfigStructure and is within allowed range.*/
    if (SeqConfigPtr->JobLinkPtr[Spi_SlaveJobAndSeqAccess.CurrentJobIndex] == \
                                                      SPI_JOB_LINK_DELIMITER)
    #endif
    /*(SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)*/
    {
      #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
      Spi_SlaveJobAndSeqAccess.CurrentJobIndex[CompletedJobSeqId] = 0U;
      #else
      Spi_SlaveJobAndSeqAccess.CurrentJobIndex = 0U;
      #endif
      /*(SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)*/
      /* Set Sequence to Idle to indicate that another Transmission can be
         Started on any other sequence that shared a job with this sequence */
      Spi_lSlaveSetSeqStatusAtomic(CompletedJobSeqId, SPI_SEQ_OK);
    }

    /*Fix for AI00254397 */
    SchM_Enter_Spi_AsyncTransmit(); /* Enter Critical Section: EnterKey-A */
    /* The variables are read into local variables to remove Misra 1.2 */
    ExtractJobId = Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId;
    EndIndex = Spi_SlaveJobAndSeqAccess.EndIndex;

    if (ExtractJobId != EndIndex)
    {
      JobId = Spi_SlaveJobAndSeqAccess.\
                     JobQueue[Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId];
      /* Get SPI Bus. The SPI bus is extracted from each job. */
      HWUnit = SPI_SLAVE_MODULE_INDEX;
      Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId = \
          Spi_lIncrementAndRoundIndex(Spi_SlaveJobAndSeqAccess. \
                                              StartIndexExtractJobId);
      /* Exit Critical Section asap for minimal blocking time: ExitKey-A1*/
      SchM_Exit_Spi_AsyncTransmit();
      Spi_lSlaveStartJob(HWUnit,JobId,ApiAccessId);
    }
    else
    {
      Spi_SlaveBusStatus = SPI_IDLE;
      Spi_SlaveJobAndSeqAccess.EndIndex = 0U;
      Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId = 0U;
      /* Exit Critical Section asap for minimal blocking time: ExitKey-A2*/
      SchM_Exit_Spi_AsyncTransmit();
    }/*(Spi_StartIndexExtractJobId != Spi_EndIndex)*/
  }/*(ChannelId == SPI_CHANNEL_LINK_DELIMITER)*/
  else
  {
    DelayAndHwChNo = JobConfigPtr->TimeDelayConfig|
         ((((uint32)JobConfigPtr->HwUnit >> SPI_HWUNIT_CH_SHIFT) &
         SPI_HWUNIT_MASK) <<  SPI_CONFIG_HWCHANNELNO_SHIFT);

    Spi_lSlaveStartChannel(Module,ChannelId, DelayAndHwChNo,
                          ((uint8)(JobConfigPtr->ParitySelection)),ApiAccessId);
  }
}
/*******************************************************************************
** Syntax           : void Spi_lSlaveErrorHandler                             **
**                    (                                                       **
**                      uint32 Module,                                        **
**                      uint32 DemError                                       **
**                    )                                                       **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-Reentrant                                           **
**                                                                            **
** Parameters (in)  : Module- SPI Slave Bus Id.                               **
**                    DemError-Dem Error ID                                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : The Slave Error Handler should be called from Isr of    **
** DMA RX when received data count is equal to                                **
** DmaReceiveCount and Error as occured                                       **
*******************************************************************************/
void Spi_lSlaveErrorHandler(uint32 Module, uint32 DemError,uint8 ApiAccessId)
{
  Spi_JobAndSeqAccessType *JobSeqDataPtr;
  Spi_JobType JobId;
  uint16 CompletedJobSeqIndex;
  Spi_SequenceType SeqId;
  uint16 ExtractJobId;
  uint16 EndIndex;
  uint32 HWUnit;
  uint8 Seqbool;
  uint8 NextSeqId;
  uint16 StartIndex;

  JobSeqDataPtr = &Spi_SlaveJobAndSeqAccess;

  /* Production relevant errors shall be reported to the Diagnostic
     Event Manager (DEM). They shall not be used as the return value of the
     called function.*/
  if (DemError == (uint32)SPI_DEM_HW_ERROR)
  {
    #if (SPI_HW_ERROR_DEM_REPORT == ENABLE_DEM_REPORT)
    /* Dem for receive error*/
      Dem_ReportErrorStatus(SPI_E_HARDWARE_ERROR,
               DEM_EVENT_STATUS_FAILED);
    #endif
  }

  CompletedJobSeqIndex = Spi_lSlaveCompletedJobId();
  /* last job completed */
  /* extract the completed Job Id */
  JobId = JobSeqDataPtr->JobQueue[CompletedJobSeqIndex];
  SeqId = (uint8)JobSeqDataPtr->JobLinkedSeq[CompletedJobSeqIndex];

  /* Indicate Failure in Job */
  Spi_lSlaveSetJobStatusAtomic(JobId, SPI_JOB_FAILED);
  /* Set Sequence to Idle to indicate that another Transmission can be
     Started on any other sequence that shared a job with this sequence */
  Spi_lSlaveSetSeqStatusAtomic(SeqId, SPI_SEQ_FAILED);

  Spi_lSlaveHwDisableModule(Module);

  Spi_lSlaveHwDisableQspiRxIntr(Module);

  Spi_lHwDisableQspiErrIntr(Module);

  /*Fix for AI00254397 */
  SchM_Enter_Spi_AsyncTransmit(); /* Enter Critical Section: EnterKey-B */
  StartIndex = JobSeqDataPtr->StartIndexExtractJobId ;
  Seqbool = 0U;
  while ((StartIndex != JobSeqDataPtr->EndIndex) && (Seqbool == 0U))
  {
    NextSeqId = JobSeqDataPtr->JobLinkedSeq[StartIndex];
    if (Spi_lSlaveGetSeqStatus(NextSeqId) == SPI_SEQ_FAILED)
    {

      StartIndex = Spi_lIncrementAndRoundIndex(StartIndex);
    }
    else
    {
      Seqbool = 1U;
    }
  }
  JobSeqDataPtr->StartIndexExtractJobId = StartIndex;

  #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
  JobSeqDataPtr->CurrentJobIndex[SeqId] = 0U;
  #else
  JobSeqDataPtr->CurrentJobIndex = 0U;
  #endif
  /*(SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)*/

  /* The variables are read into local variables to remove Misra 1.2 */
  ExtractJobId = JobSeqDataPtr->StartIndexExtractJobId;
  EndIndex = JobSeqDataPtr->EndIndex;

  if (ExtractJobId != EndIndex)
  {
    JobId = JobSeqDataPtr->JobQueue[JobSeqDataPtr->StartIndexExtractJobId];
    /*       Get SPI Bus. The SPI bus is extracted from each job .*/
    HWUnit = SPI_SLAVE_MODULE_INDEX;

    JobSeqDataPtr->StartIndexExtractJobId = \
       Spi_lIncrementAndRoundIndex(JobSeqDataPtr->StartIndexExtractJobId);
    /* Exit Critical Section asap for minimal blocking time: ExitKey-B1*/
    SchM_Exit_Spi_AsyncTransmit();
    Spi_lSlaveStartJob(HWUnit,JobId,ApiAccessId);
  }
  else
  {

    Spi_SlaveBusStatus = SPI_IDLE;
    JobSeqDataPtr->EndIndex = 0U;
    JobSeqDataPtr->StartIndexExtractJobId = 0U;
    /* Exit Critical Section asap for minimal blocking time: ExitKey-B2*/
    SchM_Exit_Spi_AsyncTransmit();
  }/*(ExtractJobId != EndIndex)*/

}

/*******************************************************************************
** Syntax           :  IFX_INLINE void Spi_lSlaveHwDisableModule              **
** (                                                                          **
**   uint32 Module                                                            **
** )                                                                          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Module no                                      **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Disable the Module at the end of Job transmission.      **
*******************************************************************************/
IFX_INLINE void Spi_lSlaveHwDisableModule(uint32 Module)
{
  /*This function is called from aparticular function which is called
    only from ISR*/
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic
    is used to efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_HW_MODULE[(uint8)Module].GLOBALCON.U &=  SPI_GLOBALCONREG_CLEARMASK17;
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic
    is used to efficiently access the SFRs of multiple QSPI Modules.*/
  SPI_HW_MODULE[(uint8)Module].GLOBALCON.U |=  SPI_GLOBALCONREG_SETMASK1;
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE uint16 Spi_lSlaveCompletedJobId(void)  **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : None.                                                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : This function returns the id of the completed job       **
*******************************************************************************/
IFX_LOCAL_INLINE uint16 Spi_lSlaveCompletedJobId(void)
{
  uint16 CompletedJobId;

  if (Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId == 0U)
  {
    CompletedJobId = (SPI_MAX_JOB_TRIG_Q_LENGTH - 1U);
  }
  else
  {
    CompletedJobId = (Spi_SlaveJobAndSeqAccess.StartIndexExtractJobId - 1U);
  }

  return (CompletedJobId);
}

/*******************************************************************************
** Syntax       : static void Spi_lSlaveHwSetJobConfig                        **
**                (                                                           **
**                  Ifx_QSPI* ModulePtr,                                      **
**                  const Spi_JobConfigType* JobConfigPtr                     **
**                                                                            **
**                )                                                           **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : ModulePtr - Slave Hw module pointer.                    **
**                    BaudRateConfig - Packed variable for Baud Rate          **
                                  Configuration Parameters TQ, Q, A, B, C     **
**                    Config  -   Data Config parameters & Qspi Channel no    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Set Slave Job Configuration Parameters      **
*******************************************************************************/
static void Spi_lSlaveHwSetJobConfig(volatile Ifx_QSPI* ModulePtr,
                        const Spi_JobConfigType* JobConfigPtr,uint8 ApiAccessId)
{
  /*
  Packed variable for Baud Rate Configuration Parameters TQ, Q, A, B, C
  bit 0-7: TQ;bit 8-13 : Q;bit 16-17 : A;bit 18-19 : B;bit 20-21 : C
  */

  Ifx_QSPI_GLOBALCON GlobalConReg;
  Ifx_QSPI_GLOBALCON1 GlobalCon1Reg;
  uint32 BaudRateConfigVal;
  uint16 Tqval;

#if(SPI_BAUDRATE_AT_RUNTIME == STD_ON)
  Ifx_QSPI_ECON EconReg;
  uint16 Qval;
  uint8 Aval, Bval, Cval;
  uint8 HwChannelno;
#endif /* SPI_BAUDRATE_AT_RUNTIME == STD_ON */

  /*
  Baudrate or SCLK = 1 /( (1/fQSPI) * GLOBALCON.TQ * ECONz.Q *[A+B+C]) bps
  GLOBALCON.TQ : Global Time Quantum - Common n-divider scaling the baud rates
           of all channels in direction of higher or lower baud rates
           0 = div by 1, 255 = div by 256
  ECONz.Q : Time Quantum - Defines the time quantum length used by A, B, and C
            to define the baud rate and duty cycle
            6b'000000 =  1, 6b'111111 = 64

  BaudRateConfig :
  bit 0-7: TQ
  bit 8-13 : Q
  bit 16-17 : A
  bit 18-19 : B
  bit 20-21 : C
  */

  BaudRateConfigVal = JobConfigPtr->BaudRateConfig;

  /*AI00249920: ECON is setup in Spi_lSlaveInit() */
  Tqval = (uint16)((uint16)BaudRateConfigVal & SPI_BAUDRATECONFIG_TQ_MASK);

  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  GlobalConReg.U = SPI_SFR_RUNTIME_USER_MODE_READ32(ModulePtr->GLOBALCON.U);
  GlobalCon1Reg.U = SPI_SFR_RUNTIME_USER_MODE_READ32(ModulePtr->GLOBALCON1.U);
  }
  else
  {
  GlobalConReg.U = ModulePtr->GLOBALCON.U;
  GlobalCon1Reg.U = ModulePtr->GLOBALCON1.U;
  }

  GlobalConReg.B.LB = SPI_QSPI_LOOPBACK_DISABLE;   /* loop back disabled */

  GlobalConReg.B.SI = SPI_QSPI_SI_DISABLE;   /* Status Injection disabled. */
  GlobalConReg.B.EXPECT = SPI_QSPI_EXPECT_VALUE;   /*  expect time to Max */
  GlobalConReg.B.SRF = SPI_QSPI_SRF_DISABLE;    /* dont Stop on RXFIFO full. */
  GlobalConReg.B.EN = SPI_QSPI_MODULE_ENABLE;
  GlobalConReg.B.TQ = Tqval;

  /* Ensuring no resets */
  GlobalConReg.B.RESETS = 0U;
  GlobalCon1Reg.B.ERRORENS = SPI_QSPI_ERRORENS_VALUE;
 /* GlobalCon1Reg.B.TXFIFOINT = SPI_QSPI_RXFIFOINT_VALUE;*/
  GlobalCon1Reg.B.RXFIFOINT = 0U;
  GlobalCon1Reg.B.TXEN = SPI_QSPI_INT_DISABLE;
  GlobalCon1Reg.B.RXEN = SPI_QSPI_INT_ENABLE;
  GlobalCon1Reg.B.RXFM = 1U; /* Single Move Mode */
  GlobalCon1Reg.B.TXFM = 1U; /* Single Move Mode */

#if(SPI_BAUDRATE_AT_RUNTIME == STD_ON)

  Qval = ((uint16)(BaudRateConfigVal >> SPI_BAUDRATECONFIG_Q_SHIFT)
                            & SPI_BAUDRATECONFIG_Q_MASK);
  Aval = ((uint8)(BaudRateConfigVal >> SPI_BAUDRATECONFIG_A_SHIFT)
                            & SPI_BAUDRATECONFIG_A_MASK);
  Bval = ((uint8)(BaudRateConfigVal >> SPI_BAUDRATECONFIG_B_SHIFT)
                            & SPI_BAUDRATECONFIG_B_MASK);
  Cval = ((uint8)(BaudRateConfigVal >> SPI_BAUDRATECONFIG_C_SHIFT)
                            & SPI_BAUDRATECONFIG_C_MASK);
  /* HW channels vary from 0 - 15
     but the ECON maps 8 - 15 to 0 to 7.   */
  HwChannelno = ((uint8)(JobConfigPtr->HwUnit) >> SPI_HWUNIT_CH_SHIFT);

  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  EconReg.U = SPI_SFR_RUNTIME_USER_MODE_READ32\
                  (ModulePtr->ECON[HwChannelno & SPI_QSPI_HWCHANNELNO_MASK].U);
  }
  else
  {
  EconReg.U = ModulePtr->ECON[HwChannelno & SPI_QSPI_HWCHANNELNO_MASK].U;
  }

  EconReg.B.CPH = (((uint8)(JobConfigPtr->ShiftClkConfig))
                                             & SPI_QSPI_CLOCKPHASE_MASK);
  EconReg.B.CPOL = ((uint8)((JobConfigPtr->ShiftClkConfig)
                   >> SPI_QSPI_CLOCKPOL_SHIFT) & SPI_QSPI_CLOCKPOL_MASK);

  if(((uint8)JobConfigPtr->ParitySelection) == SPI_QSPI_PARITY_UNUSED)
  {
    /* Transmit/Receive parity Disabled */
    EconReg.B.PAREN = SPI_QSPI_PARITY_DISABLE;
  }
  else
  {
    /* Transmit/Receive parity Enabled */
    EconReg.B.PAREN = SPI_QSPI_PARITY_ENABLE;
  }

  /* Configure the ECON Baudrate params */
  EconReg.B.Q = (uint8)Qval;
  EconReg.B.A = (uint8)Aval;
  EconReg.B.B = (uint8)Bval;
  EconReg.B.C = (uint8)Cval;

  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
    SPI_SFR_RUNTIME_MODIFY32(ModulePtr->ECON[HwChannelno & \
           SPI_QSPI_HWCHANNELNO_MASK].U,((uint32)EconReg.U),((uint32)EconReg.U))
  }
  else
  {
    ModulePtr->ECON[HwChannelno & SPI_QSPI_HWCHANNELNO_MASK].U = EconReg.U;
  }

#endif /* SPI_BAUDRATE_AT_RUNTIME == STD_ON */
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  SPI_SFR_RUNTIME_MODIFY32(ModulePtr->GLOBALCON.U,\
                                (uint32)GlobalConReg.U,(uint32)GlobalConReg.U)

  SPI_SFR_RUNTIME_MODIFY32(ModulePtr->GLOBALCON1.U,\
                    (uint32)GlobalCon1Reg.U,(uint32)GlobalCon1Reg.U)
  /*Reserve bit access is ensured for FLAGSCLEAR register*/
  SPI_SFR_RUNTIME_USER_MODE_WRITE32(ModulePtr->FLAGSCLEAR.U,\
                                                  (SPI_QSPI_FLAGSCLEAR_VALUE));
  }
  else
  {
  ModulePtr->GLOBALCON.U = GlobalConReg.U;
  ModulePtr->GLOBALCON1.U = GlobalCon1Reg.U;
  ModulePtr->FLAGSCLEAR.U = SPI_QSPI_FLAGSCLEAR_VALUE;
  }
}

/*******************************************************************************
** Traceability : []
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveHwSetConfigExtn         **
** (                                                                          **
**   const Spi_ConfigType* ConfigPtr                                          **
** )                                                                          **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : ConfigPtr  -   init config pointer                      **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to Set the ECON register Parameters            **
*******************************************************************************/
#if(SPI_BAUDRATE_AT_RUNTIME == STD_OFF)
IFX_LOCAL_INLINE void Spi_lSlaveHwSetConfigExtn(const Spi_ConfigType* ConfigPtr)
{
    Ifx_QSPI* ModulePtr;
    uint8 HwChannelno;
    uint8 AsilModule;
    uint8 EconCntr;

    EconCntr = 0U;
    do
    {
      /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
      AsilModule = ((ConfigPtr->SpiBaudrateEconPtr)[EconCntr].QSPIHwUnit
                                                           & SPI_HWUNIT_MASK);
      if(
         ((uint32)SPI_ASIL_MASTER_MODULE_USED & \
         ((uint32)0x01U << AsilModule)) == (uint32)SPI_ASIL_MASTER_MODULE_USED
        )
      {
 /*IFX_MISRA_RULE_17_04_STATUS="SpiBaudrateEconPtr" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
        HwChannelno = ((((ConfigPtr->SpiBaudrateEconPtr)[EconCntr].QSPIHwUnit)\
               >> SPI_HWUNIT_CH_SHIFT) & SPI_QSPI_HWCHANNELNO_MASK);

     /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
        ModulePtr =  &(SPI_HW_MODULE[(uint8)SPI_SLAVE_MODULE_INDEX]);


        /* Write parameters to ECON register */
/*IFX_MISRA_RULE_17_04_STATUS="SpiBaudrateEconPtr" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
        SPI_SFR_INIT_WRITE32(ModulePtr->ECON[HwChannelno].U,
                           (ConfigPtr->SpiBaudrateEconPtr)[EconCntr].EconVal);
      }
      EconCntr++;
    }while(EconCntr < ConfigPtr->NoOfEconReg);
}

#if (SPI_INITCHECK_API == STD_ON)
/*******************************************************************************
** Traceability : []
** Syntax       : IFX_LOCAL_INLINE Std_ReturnType Spi_lSlaveHwCheckConfigExtn **
** (                                                                          **
**   const Spi_ConfigType* ConfigPtr                                          **
** )                                                                          **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : ConfigPtr  -   init config pointer                      **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : E_OK if ECON registers match else E_NOT_OK.             **
**                                                                            **
** Description      : Function to Check the ECON register Parameters          **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Spi_lSlaveHwCheckConfigExtn
(
  const Spi_ConfigType* ConfigPtr
)
{

    Ifx_QSPI* ModulePtr;
    Ifx_QSPI_ECON EconReg;
    uint8 HwChannelno;
    Std_ReturnType RetVal;
    uint32 ReadValue;
    volatile uint32 CompareValue;
    uint8 AsilModule;
    uint8 EconCntr;

    CompareValue = SPI_INITCHK_COMP_VALUE;
    RetVal = E_OK;
    EconCntr = 0U;

    do
    {
      /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
      AsilModule = ((ConfigPtr->SpiBaudrateEconPtr)[EconCntr].QSPIHwUnit
                                                           & SPI_HWUNIT_MASK);

     if(
        ((uint32)SPI_ASIL_MASTER_MODULE_USED & \
        ((uint32)0x01U << AsilModule)) == (uint32)SPI_ASIL_MASTER_MODULE_USED
       )
     {
 /*IFX_MISRA_RULE_17_04_STATUS="SpiBaudrateEconPtr" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
        HwChannelno = ((((ConfigPtr->SpiBaudrateEconPtr)[EconCntr].QSPIHwUnit)\
                          >> SPI_HWUNIT_CH_SHIFT) & SPI_QSPI_HWCHANNELNO_MASK);

/*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
        ModulePtr =  &(SPI_HW_MODULE[(uint8)SPI_SLAVE_MODULE_INDEX]);

        /*check if respective ECONz sfr value matches*/
      ReadValue = SPI_SFR_INIT_USER_MODE_READ32(ModulePtr->ECON[HwChannelno].U);

        /* Read parameters of ECON register */
/*IFX_MISRA_RULE_17_04_STATUS="SpiBaudrateEconPtr" Pointer arithmetic is used
       to efficiently access the SFRs of multiple QSPI Modules.*/
        EconReg.U = (ConfigPtr->SpiBaudrateEconPtr)[EconCntr].EconVal;

        CompareValue &= (ReadValue ^ ~((uint32)EconReg.U));

        if(CompareValue != SPI_INITCHK_COMP_VALUE)
        {
          RetVal = E_NOT_OK;
          break;
        }
     }
     EconCntr++;
    }while(EconCntr < ConfigPtr->NoOfEconReg);

    return(RetVal);
}
#endif /* SPI_INITCHECK_API == STD_ON */
#endif /* SPI_BAUDRATE_AT_RUNTIME == STD_OFF */

/*******************************************************************************
** Syntax           : static void Spi_lSlaveHwSetChannelConfig                **
** (                                                                          **
**   uint32 Module,                                                           **
**   uint16 DataConfig,                                                       **
**   uint32 DelayAndHwChNo                                                    **
**   uint8 ParityConfig                                                       **
** )                                                                          **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Asynchronous                                            **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : Module - Hw module .                                    **
**                    DataConfig  -   Data Config parameters                  **
**                    DelayAndHwChNo  -   Delay                               **
**                    ParityConfig  -   Parity Configuration                  **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : The function shall configure the spi slave              **
**                    channel parameters in the hardware                      **
*******************************************************************************/
static void Spi_lSlaveHwSetChannelConfig(volatile uint32 Module,
  uint16 DataConfig,uint32 DelayAndHwChNo, uint8 ParityConfig,uint8 ApiAccessId)
{
  /* DataConfig contains MSB, DL,Byte, */
  Ifx_QSPI_BACON BaconReg;
  volatile Ifx_QSPI* ModulePtr;
  uint32 TimeDelay;
  uint32 Temp;

  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  ModulePtr = &(SPI_HW_MODULE[Module]);
   /*Reserve bit access is ensured for FLAGSCLEAR register*/
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  SPI_SFR_RUNTIME_USER_MODE_MODIFY32(ModulePtr->FLAGSCLEAR.U,\
                                SPI_FLAGSCLEAR_CLEARMASK,SPI_FLAGSCLEAR_SETMASK)
  BaconReg.U = SPI_SFR_RUNTIME_USER_MODE_READ32(ModulePtr->BACON.U);
  }
  else
  {
  ModulePtr->FLAGSCLEAR.U = SPI_QSPI_FLAGSCLEAR_VALUE;
  BaconReg.U = ModulePtr->BACON.U;
  }
  TimeDelay = DelayAndHwChNo & SPI_CONFIG_TIMEDELAY_MASK;

  Temp = BaconReg.U ;
  Temp = Temp & (~SPI_QSPI_TIMEDELAY_MASK) ;
  Temp = Temp | TimeDelay;
  BaconReg.U = Temp;

  /* Channel select */
  BaconReg.B.CS  = (uint8)(DelayAndHwChNo >> SPI_CONFIG_HWCHANNELNO_SHIFT);

  #if(SPI_SLAVE_FLIP_TRANSFERSTART == STD_ON)
  BaconReg.B.MSB = (uint8)(~(uint8)((DataConfig >> SPI_CONFIG_MSB_SHIFT) &
                      SPI_CONFIG_MSB_MASK)) ;  /* 1 = MSB or 0 = LSB */
  #else
  BaconReg.B.MSB = (uint8)(DataConfig >> SPI_CONFIG_MSB_SHIFT) &
                      SPI_CONFIG_MSB_MASK ; /* 1 = MSB or 0 = LSB */

  #endif

  BaconReg.B.BYTE = SPI_QSPI_DL_BIT_SELECT; /* 1 = bytes or  0 = bits */

  /* data length in bits  */
  BaconReg.B.DL = ((uint8)DataConfig & SPI_CONFIG_DATALENGTH_MASK) ;

  if ( ParityConfig != SPI_QSPI_PARITY_UNUSED )
  {
    BaconReg.B.PARTYP = ParityConfig;
  }
  else
  {
    /*Do Nothing since parity is not enabled in ECONz register*/
  }
  if(ApiAccessId == SPI_RUNTIME_ACCESS)
  {
  SPI_SFR_RUNTIME_USER_MODE_WRITE32(ModulePtr->BACONENTRY.U,BaconReg.U);
  }
  else
  {
  ModulePtr->BACONENTRY.U = BaconReg.U;
  }
  Spi_lSlaveWriteTransmit(ApiAccessId);
}

#if (SPI_CANCEL_API == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=         ]                                  **
** Syntax:           void Spi_lSlaveCancel                                    **
**                   (                                                        **
**                     Spi_SequenceType        Sequence                       **
**                   )                                                        **
** [/cover]                                                                   **
**                                                                            **
**                                                                            **
** Service ID:       0x0                                                      **
**                                                                            **
** Sync/Async:       Asynchronous                                             **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  Sequence                                                 **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value:     none                                                     **
**                                                                            **
** Description : This service cancel the specified on-going                   **
** sequence transmission without cancelling any Job transmission and the SPI  **
** Handler/Driver will set the sequence result to SPI_SEQ_CANCELLED.          **
** The SPI Handler/Driver is not responsible on external devices damages or   **
** undefined state due to cancelling a sequence transmission. It is up to the **
** user to be aware of what user is doing!                                    **
*******************************************************************************/
void Spi_lSlaveCancel(Spi_SequenceType Sequence)
{
  #if (SPI_LEVEL_DELIVERED != 0U)
  Spi_JobAndSeqAccessType   *JobSeqDataPtr;

  const Spi_SequenceConfigType* ParamSeqConfigPtr;
  uint32 JobCounter;
  Spi_JobType JobId;

  JobSeqDataPtr = &Spi_SlaveJobAndSeqAccess;
  #endif
  /* Semaphore to protect SeqCancelStatus */
  SchM_Enter_Spi_Cancel();
  /* Set Sequence Cancelled */
  Spi_lSlaveSetSeqStatus(Sequence, SPI_SEQ_CANCELED);
  /* If the Sequence is not the current running sequence, then set the
     sequence to OK so that it can be started again */
  #if (SPI_LEVEL_DELIVERED != 0U) /*Used Only in Async Mode*/
  #if (SPI_LEVEL_DELIVERED == 2U)
  if (   (Sequence !=
           JobSeqDataPtr->JobLinkedSeq[JobSeqDataPtr->StartIndexExtractJobId])
        && (Sequence >= Spi_kConfigPtr->NoOfSyncSequences)
      )
  {
    Spi_lSlaveSetSeqStatus(Sequence, SPI_SEQ_OK);
    /* Set the Shadow Flag to indicate that the sequence is cancelled for the
       status functions.
       This is need because the main status variable SeqStatus is set
       to SPI_SEQ_OK so that the cancelled sequence can be started again.
       Also the cancelled sequence does not block other sequences that
       shares a job with this sequence */
    Spi_lSlaveSetSeqCancelStatus(Sequence);
  }
  #else
  if ( (Sequence !=
          JobSeqDataPtr->JobLinkedSeq[JobSeqDataPtr->StartIndexExtractJobId])
      )
  {
      Spi_lSlaveSetSeqStatus(Sequence, SPI_SEQ_OK);
      /* Set the Shadow Flag to indicate that the sequence is cancelled for the
       status functions.
       This is need because the main status variable SeqStatus is set
       to SPI_SEQ_OK so that the cancelled sequence can be started again.
       Also the cancelled sequence does not block other sequences that
       shares a job with this sequence */
      Spi_lSlaveSetSeqCancelStatus(Sequence);
    }
  #endif
      /* Set the status of All queued Jobs in that seq to ok */
      /*IFX_MISRA_RULE_17_04_STATUS="SpiSequenceConfigPtr" Pointer arithmetic
        used due to PBConfigStructure and is within allowed range.*/
      ParamSeqConfigPtr= &(Spi_kSlaveConfigPtr->SpiSequenceConfigPtr[Sequence]);
      JobCounter = 0U;

      /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due to
        PBConfigStructure and is within allowed range.*/
      while (
      JobSeqDataPtr->JobQueue[JobSeqDataPtr->StartIndexExtractJobId] !=
                   ParamSeqConfigPtr->JobLinkPtr[JobCounter])
      {
        /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due
          to PBConfigStructure and is within allowed range.*/
        if (ParamSeqConfigPtr->JobLinkPtr[JobCounter] == \
                                                 SPI_JOB_LINK_DELIMITER)
        {
          break;
        }
        JobCounter++;
      }

        /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due
          to PBConfigStructure and is within allowed range.*/
        while (ParamSeqConfigPtr->JobLinkPtr[JobCounter] != \
                                                 SPI_JOB_LINK_DELIMITER)
        {
          /* extract each job id from the sequence */
         /*IFX_MISRA_RULE_17_04_STATUS="JobLinkPtr" Pointer arithmetic used due
            to PBConfigStructure and is within allowed range.*/
          JobId = ParamSeqConfigPtr->JobLinkPtr[JobCounter];

          Spi_lSlaveSetJobStatusAtomic(JobId,SPI_JOB_OK );
          JobCounter++;
        }
   #endif

    /* Exit Critical Section */
    SchM_Exit_Spi_Cancel();
  }

#endif

#if ((SPI_CANCEL_API == STD_ON) && (SPI_LEVEL_DELIVERED != 0U))
/*******************************************************************************
** Traceability : [cover parentID=     ]                                      **
**                                                                            **
** Syntax : IFX_LOCAL_INLINE void Spi_lSlaveSetSeqCancelStatus                **
**                                      (Spi_SequenceType Seq)                **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  NA                                                            **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  Seq - Sequence Id                                       **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  void                                                    **
**                                                                            **
** Description      : To set the sequence to cancelled state                  **
*******************************************************************************/
IFX_LOCAL_INLINE void Spi_lSlaveSetSeqCancelStatus(Spi_SequenceType Seq)
{
  uint16 Index;
  /* ASIL or QM sequence index from map configuration*/
  Index = (uint16)((uint16)Spi_kSlaveConfigPtr->SpiMapConfigPtr->SeqIndex[Seq]
          >> SPI_SEQUENCE_INDEX);
  Spi_SlaveSeqStatus[Index].AsilSeqCancelStatus = \
      Spi_SlaveSeqStatus[Index].AsilSeqCancelStatus |\
     ((uint32)SPI_SEQ_CANCELED << (SPI_SEQUENCE_STATUS_BITS * \
                          (Seq & SPI_SEQUENCE_BIT_POSITION)));
}
#endif

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Spi_lSlaveSeqCancelled(           **
                           Spi_JobAndSeqAccessType *JobSeqDataPtr)            **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Reentrant                                               **
**                                                                            **
** Parameters (in)  : None.                                                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None.                                                   **
**                                                                            **
** Description      : Function to set the status of sequence cancelled to     **
**                    cancel                                                  **
*******************************************************************************/
#if (SPI_CANCEL_API == STD_ON)
IFX_LOCAL_INLINE void Spi_lSlaveSeqCancelled(Spi_JobAndSeqAccessType \
                                                            *JobSeqDataPtr)
{
  uint8 Seqbool;
  uint8 NextSeqId;
  uint16 StartIndex;

  /*Fix for AI00254397 */
  SchM_Enter_Spi_Cancel();
  StartIndex = JobSeqDataPtr->StartIndexExtractJobId;
  Seqbool = 0U;
  while ((StartIndex != JobSeqDataPtr->EndIndex) && (Seqbool == 0U))
  {
    NextSeqId = JobSeqDataPtr->JobLinkedSeq[StartIndex];
    if (Spi_lSlaveGetSeqStatus(NextSeqId) == SPI_SEQ_CANCELED)
    {
      #if(SPI_LEVEL_DELIVERED != 0U)
      Spi_lSlaveSetSeqCancelStatus(NextSeqId);
      #endif
      StartIndex = Spi_lIncrementAndRoundIndex(StartIndex);
    }
    else
    {
      Seqbool = 1U;
    }
  }
  JobSeqDataPtr->StartIndexExtractJobId = StartIndex;
  SchM_Exit_Spi_Cancel();
}
#endif
 /*(SPI_CANCEL_API == STD_ON) */

#if ( SPI_INITCHECK_API == STD_ON )
/*******************************************************************************
** Traceability : [cover parentID=DS_MCAL_SPI_9802,DS_MCAL_SPI_9803,          **
DS_MCAL_SPI_9846,SAS_MCAL_SPI_9804]                                           **
**                                                                            **
** Syntax : Std_ReturnType Spi_lSlaveInitCheck                                **
                                        (const Spi_ConfigType* ConfigPtr)     **
** [/cover]                                                                   **
**                                                                            **
** Service ID:  NA                                                            **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  ConfigPtr - Pointer to configuration set                **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  Std_ReturnType                                          **
**                                                                            **
** Description      : This function checks is the initialization of the       **
**                    SPI hardware unit and driver done according to the      **
**                    configuration set referenced by ConfigPtr.              **
*******************************************************************************/
Std_ReturnType Spi_lSlaveInitCheck(const Spi_ConfigType* ConfigPtr)
{
  const Spi_HWModuleConfigType* HWModuleConfigPtr;
  uint32                        ReadValue;
  volatile uint32               CompareValue;
  Std_ReturnType                RetVal;
  /* initialize compare value */
  CompareValue = SPI_INITCHK_COMP_VALUE;
  RetVal = E_OK;
  /*  Get Hardware pointer for slave QSPI*/
  HWModuleConfigPtr = (ConfigPtr->HWModuleConfigPtr[SPI_SLAVE_MODULE_INDEX]);
  /* check for clock initialization */
  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                (SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].CLC.U);
  CompareValue &= (ReadValue ^ ~((uint32)
                                       HWModuleConfigPtr->HWClkSetting));

  if(CompareValue == SPI_INITCHK_COMP_VALUE)
  {
    /* Check for GLOBALCON register */
    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                            (SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].GLOBALCON.U);
    CompareValue &= (ReadValue ^ ~((uint32)SLAVE_QSPI_GLOBCON_RSTVAL));
    /* Check for PISEL register (clock input select and slave receive signal) */
    /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
      efficiently access the SFRs of multiple QSPI Modules.*/
    ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                (SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].PISEL.U);
    CompareValue &= (ReadValue ^ ~((uint32)HWModuleConfigPtr->HWPinSetting));

    #if(SPI_BAUDRATE_AT_RUNTIME == STD_OFF)
    /*Check the ECON register values */
    RetVal = RetVal | Spi_lSlaveHwCheckConfigExtn(ConfigPtr);
    #endif /* SPI_BAUDRATE_AT_RUNTIME == STD_OFF */

    /* Checking DMA specific registers */
    RetVal = RetVal | Spi_lSlaveHwDmaSfrInitCheck();

    /* write only - hence ignored
      IntrSrc.CLRR,SWSCLR and IOVCLR are write only bits */
    ReadValue = (uint32)((SPI_SFR_INIT_USER_MODE_READ32\
                          (MODULE_SRC.QSPI.QSPI[SPI_SLAVE_MODULE_INDEX].RX.U) &\
                               SPI_QSPI_RX_CLEARMASK2)>>SPI_QSPI_RX_SRE_BITPOS);
    CompareValue &= (ReadValue ^ ~((uint32)0U));
    /* Check for all the global variables */
    RetVal = RetVal | Spi_lSlaveInitGlobalVarCheck();
  }

  if ( (CompareValue != SPI_INITCHK_COMP_VALUE) || (RetVal != E_OK) )
  {
    RetVal = E_NOT_OK;
  }
  return RetVal;
}

/*******************************************************************************
** Syntax : static Std_ReturnType Spi_lSlaveHwDmaSfrInitCheck(void)           **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : Std_ReturnType                                           **
**                                                                            **
**                                                                            **
** Description :  Returns E_OK if all the sfr values are as initialised       **
**                else returns E_NOT_OK                                       **
**                                                                            **
*******************************************************************************/
static Std_ReturnType Spi_lSlaveHwDmaSfrInitCheck(void)
{
  Ifx_SRC_SRCR IntrSrc;
  volatile Ifx_SRC_SRCR_Bits *MoveEngCh0BaseAddr;
  Dma_ChannelType   DmaReceiveChannel = DMA_CHANNEL_INVALID;
  uint32            Address,RemapAddress;
  uint32            ReadValue;
  volatile uint32   CompareValue;

  Std_ReturnType    RetVal;

  MoveEngCh0BaseAddr = SRC_DMACH0ADDR;
  CompareValue = SPI_INITCHK_COMP_VALUE;

  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32(MODULE_DMA.CLC.U);
  CompareValue &= (ReadValue ^ ~((uint32)8U));

  RetVal = E_NOT_OK;
  CompareValue = SPI_INITCHK_COMP_VALUE;

  DmaReceiveChannel  = Spi_SlaveChnlAccess[0U].DmaChannelIdx.RxDmaChannel;

  /*IFX_MISRA_RULE_17_04_STATUS="SPI_HW_MODULE" Pointer arithmetic is used to
    efficiently access the SFRs of multiple QSPI Modules.*/
  Address = ((uint32)&(SPI_HW_MODULE[SPI_SLAVE_MODULE_INDEX].RXEXIT.U));

  /* Check Source Address */
  RemapAddress = SPI_SFR_INIT_USER_MODE_MCAL_GETDSPRREMAPADDRESS(Address);
  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                      (MODULE_DMA.CH[DmaReceiveChannel].SADR.U);
  CompareValue &= (ReadValue ^ ~(RemapAddress));

  /* Check channel control register */
  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                    (MODULE_DMA.CH[DmaReceiveChannel].CHCFGR.U);
  CompareValue &= (ReadValue ^ ~((uint32)DMA_CHCFGR_RESETVAL));

  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                     (MODULE_DMA.CH[DmaReceiveChannel].CHCSR.U);
  CompareValue &= (ReadValue ^ ~((uint32)
                                   ReadValue & (SPI_DMA_CHCSR_SCH_CLR_MASK)));

  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                          (MODULE_DMA.TSR[DmaReceiveChannel].U);
  CompareValue &= (ReadValue ^ ~((uint32)
                                    (ReadValue &
                                ((~((uint32)1U << IFX_DMA_TSR_ECH_OFF))
                                | ( (uint32)1U << IFX_DMA_TSR_DCH_OFF)))));

  /*Check for Mcal_DmaChClrIntrFlags(DmaReceiveChannel) ;*/
  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                     (MODULE_DMA.CH[DmaReceiveChannel].CHCSR.U);
  CompareValue &= (ReadValue ^ ~((uint32) ReadValue  | 0U));

  ReadValue = (uint32)SPI_SFR_INIT_USER_MODE_READ32\
                                          (MODULE_DMA.TSR[DmaReceiveChannel].U);
  CompareValue &= (ReadValue ^ ~((uint32)ReadValue | 0U));

  /* DMA Error Initialization */
  ReadValue = (uint32)((SPI_SFR_INIT_USER_MODE_READ32\
                (MODULE_DMA.CH[DmaReceiveChannel].ADICR.U) & \
                           SPI_DMAADICR_CLEARMASK2)>>SPI_DMA_ADICR_ETRL_BITPOS);
  CompareValue &= (ReadValue ^ ~((uint32)1U));

  /* Check Interrupt Control  */
  ReadValue = (uint32)((SPI_SFR_INIT_USER_MODE_READ32\
                (MODULE_DMA.CH[DmaReceiveChannel].ADICR.U) & \
                          SPI_DMAADICR_CLEARMASK3)>>SPI_DMA_ADICR_INTCT_BITPOS);
  CompareValue &= (ReadValue ^ ~((uint32)SPI_DMA_INTCT));

  /* the following registers are updated for INTERRUPT mode only and not
  applicable for POLLING MODE. However when safety is ON it is mandated
  that the user switches to INTERRUPT mode immediately after initalisation
  and before Spi_lSlaveInitCheck is called */

  /*IFX_MISRA_RULE_17_04_STATUS="MoveEngCh0BaseAddr" Pointer arithmetic is used
    to efficiently access the SFRs of multiple QSPI Modules.*/
  IntrSrc.U = SPI_SFR_INIT_USER_MODE_READ32((*((volatile Ifx_SRC_SRCR*)\
                    (volatile void*)MoveEngCh0BaseAddr + DmaReceiveChannel)).U);
  ReadValue = (uint32)IntrSrc.B.SRE;
  CompareValue &= (ReadValue ^ ~((uint32)1U));

  if (CompareValue == SPI_INITCHK_COMP_VALUE)
  {
    RetVal = E_OK;
  }
  return (RetVal);
}

/*******************************************************************************
** Syntax : static Std_ReturnType Spi_lSlaveInitGlobalVarCheck(void)          **
**                                                                            **
** Service ID:  None                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Re-entrant                                                    **
**                                                                            **
** Parameters (in) : None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value    : Std_ReturnType                                           **
**                                                                            **
**                                                                            **
** Description :  Returns E_OK if all the global variable values are          **
**                as initialised else returns E_NOT_OK                        **
**                                                                            **
*******************************************************************************/
static Std_ReturnType Spi_lSlaveInitGlobalVarCheck(void)
{
  uint32            Index;
  uint32            ReadValue;
  volatile uint32   CompareValue;
  Std_ReturnType    RetVal;

  CompareValue = SPI_INITCHK_COMP_VALUE;
  RetVal = E_NOT_OK;

  ReadValue = (uint32)Spi_SlaveBusStatus;
  CompareValue &= (ReadValue ^ ~((uint32)SPI_IDLE));

  #if ((SPI_LEVEL_DELIVERED == 1U) || (SPI_LEVEL_DELIVERED == 2U))

  #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
  Index = 0U;
  #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
  do
  {
  #endif
    ReadValue = (uint32)0U;
    CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveJobAndSeqAccess.\
                                                    JobQueueRearrange[Index]));

    ReadValue = (uint32)0U;
    CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveJobAndSeqAccess.\
                                           JobLinkedSequenceRearrange[Index]));
  #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
    Index++;
  }while(Index < SPI_MAX_JOB_TRIG_Q_LENGTH);
  #endif
  #endif

  ReadValue = (uint32)(Spi_kSlaveConfigPtr->\
     HWModuleConfigPtr[SPI_SLAVE_MODULE_INDEX]->SpiDmaConfigPtr->RxDmaChannel);
  CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveChnlAccess[0U].\
                                                  DmaChannelIdx.RxDmaChannel));

  #if (SPI_INTERRUPTABLE_SEQUENCE_ALLOWED == STD_ON)
  /* Set Index to the last Sequence */
  Index = Spi_kSlaveConfigPtr->NoOfSequences;
  do
  {
    Index--;

    ReadValue = (uint32)0U;
    CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveJobAndSeqAccess.\
                                                      CurrentJobIndex[Index]));
  }while (Index > 0U);
  #else
  ReadValue = (uint32)0U;
  CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveJobAndSeqAccess.\
                                                             CurrentJobIndex));
  #endif
  #endif
  /* ((SPI_LEVEL_DELIVERED == 1U) || (SPI_LEVEL_DELIVERED == 2U))  */

  /* The following global variables are common to all levels */
  Index = 0U;
  #if (SPI_JOB_ASIL_ARRAY_INDEX > 1U)
  do
  {
  #endif
    ReadValue = (uint32)0U;
    CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveJobResult[Index]));
  #if (SPI_JOB_ASIL_ARRAY_INDEX > 1U)
    Index++;
  }while (Index < SPI_JOB_ASIL_ARRAY_INDEX);
  #endif

  Index = 0U;
  #if (SPI_SEQUENCE_ASIL_ARRAY_INDEX > 1U)
  do
  {
  #endif
    ReadValue = (uint32)0U;
    CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveSeqStatus[Index].\
                                                               AsilSeqStatus));
  #if (SPI_SEQUENCE_ASIL_ARRAY_INDEX > 1U)
    Index++;
  }while (Index < SPI_SEQUENCE_ASIL_ARRAY_INDEX);
  #endif

  #if  (SPI_LEVEL_DELIVERED != 0U)
  Index = 0U;
  #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
  do
  {
  #endif
    ReadValue = (uint32)SPI_SEQ_IDLE_ID;
    CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveJobAndSeqAccess.\
                                                         JobLinkedSeq[Index]));

    ReadValue = (uint32)SPI_JOB_IDLE_ID;
    CompareValue &= (ReadValue ^ ~((uint32)Spi_SlaveJobAndSeqAccess.\
                                                             JobQueue[Index]));
  #if(SPI_MAX_JOB_TRIG_Q_LENGTH > 1U)
    Index++;
  }while (Index < SPI_MAX_JOB_TRIG_Q_LENGTH);
  #endif
  #endif

  if (CompareValue == SPI_INITCHK_COMP_VALUE)
  {
    RetVal = E_OK;
  }
  return (RetVal);
}
#endif
/* End of Function: Spi_lSlaveInitCheck */
/* (SPI_INITCHECK_API == STD_ON) */

/*Memory Map of the SPI SLAVE Code*/
#define SPI_STOP_SEC_CODE
/*
  Allows to map variables, constants and code of modules to individual
  memory sections.
*/
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
allowed only for MemMap.h*/
#include "MemMap.h"

#endif
/* (SPI_SLAVE_ENABLE == STD_ON)*/
/*******************************************************************************
**                            General Notes                                   **
*******************************************************************************/
