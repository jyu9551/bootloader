/******************************************************************************
**                                                                           **
** Copyright (C) Infineon Technologies (2014)                                **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to Infineon      **
** Technologies. Passing on and copying of this document, and communication  **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : Irq.c $                                                    **
**                                                                           **
**  $CC VERSION : \main\dev_tc23x\16 $                                       **
**                                                                           **
**  $DATE       : 2016-03-30 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains initisalization of interrupt priority   **
**                and interruptframe based on interrupt category.            **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of Tasking sfr file */
#include "IfxSrc_reg.h"

/*Include Irq Module header file*/
#include "Irq.h"

/* Inclusion of Global Header File */
#include "Mcal.h"

#include "Mcal_Options.h"

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/* Set CLRR to clear SRR bit and disable SRE bit */
#define IRQ_DISABLE_CLEAR_SRC     (0x02000000U)

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#if (IFX_MCAL_USED == STD_ON)
#define IRQ_START_SEC_CODE
#include "MemMap.h"
#else
#define IFX_IRQ_START_SEC_CODE_ASIL_B
#include "Ifx_MemMap.h"
#endif

#if (IFX_MCAL_USED == STD_ON)

#if (IRQ_ASCLIN_EXIST == STD_ON)
static void Irq_ClearAsclinIntFlags(void);
#endif

#if (IRQ_CCU6_EXIST == STD_ON)
static void Irq_ClearCcu6IntFlags (void);
#endif

#if (IRQ_GPT_EXIST == STD_ON)
static void Irq_ClearGptIntFlags (void);
#endif

#if (IRQ_GTM_EXIST == STD_ON)
static void Irq_ClearGtmIntFlags (void);
#endif

#if (IRQ_CAN_EXIST == STD_ON)
static void Irq_ClearCanIntFlags (void);
#endif

#if (IRQ_GPSRGROUP_EXIST == STD_ON)
static void Irq_ClearGpsrGroupIntFlags (void);
#endif

#if (IRQ_QSPI_EXIST == STD_ON)
static void Irq_ClearSpiIntFlags (void);
#endif

#if (IRQ_ADC_EXIST == STD_ON)
static void Irq_ClearAdcIntFlags (void);
#endif

#if (IRQ_FLEXRAY_EXIST == STD_ON)
static void Irq_ClearFlexrayIntFlags (void);
#endif

#if (IRQ_ETH_EXIST == STD_ON)
static void Irq_ClearEthernetIntFlags (void);
#endif

#if (IRQ_DMA_EXIST == STD_ON)
static void Irq_ClearDmaIntFlags (void);
#endif

#if (IRQ_STM_EXIST == STD_ON)
static void Irq_ClearStmIntFlags (void);
#endif

#if (IRQ_SCU_EXIST == STD_ON)
static void Irq_ClearScuIntFlags(void);
#endif

#if (IRQ_PMU0_EXIST == STD_ON)
static void Irq_ClearPmuIntFlags (void);
#endif

#if (IRQ_SENT_EXIST == STD_ON)
static void Irq_ClearSentIntFlags (void);
#endif

#if ((IRQ_HSM0_EXIST == STD_ON) || (IRQ_HSM1_EXIST == STD_ON))
static void Irq_ClearHsmIntFlags (void);
#endif

#endif /* (IFX_MCAL_USED == STD_ON) */

#if (IFX_MCAL_USED == STD_ON)
#define IRQ_STOP_SEC_CODE
#include "MemMap.h"
#else
#define IFX_IRQ_STOP_SEC_CODE_ASIL_B
#include "Ifx_MemMap.h"
#endif
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
#if (IFX_MCAL_USED == STD_ON)
#define IRQ_START_SEC_CODE
#include "MemMap.h"
#else
#define IFX_IRQ_START_SEC_CODE_ASIL_B
#include "Ifx_MemMap.h"
#endif
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#if (IFX_MCAL_USED == STD_ON)

#if (IRQ_ASCLIN_EXIST == STD_ON)
static void Irq_ClearAsclinIntFlags(void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_ASCLIN0_EXIST == STD_ON)
  SRC_ASCLIN0TX.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ASCLIN0RX.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ASCLIN0ERR.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_ASCLIN1_EXIST == STD_ON)
  SRC_ASCLIN1TX.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ASCLIN1RX.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ASCLIN1ERR.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

}
#endif

#if (IRQ_CCU6_EXIST == STD_ON)
static void Irq_ClearCcu6IntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_CCU60_EXIST == STD_ON)
  SRC_CCU60SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_CCU60SR1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_CCU60SR2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_CCU60SR3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CCU61_EXIST == STD_ON)
  SRC_CCU61SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_CCU61SR1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_CCU61SR2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_CCU61SR3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
}
#endif

#if (IRQ_GPT_EXIST == STD_ON)
static void Irq_ClearGptIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_GPT120_EXIST == STD_ON)
  SRC_GPT120CIRQ.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GPT120T2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GPT120T3.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GPT120T4.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GPT120T5.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GPT120T6.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
}
#endif

#if (IRQ_GTM_EXIST == STD_ON)
static void Irq_ClearGtmIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_GTM_AEI_EXIST == STD_ON)
  SRC_GTMAEIIRQ.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_GTM_ERR_EXIST == STD_ON)
  SRC_GTMERR.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_GTM_TIM0_EXIST == STD_ON)
  SRC_GTMTIM00.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTIM01.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTIM02.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTIM03.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTIM04.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTIM05.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTIM06.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTIM07.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_GTM_TOM0_EXIST == STD_ON)
  SRC_GTMTOM00.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM01.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM02.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM03.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM04.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM05.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM06.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM07.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_GTM_TOM1_EXIST == STD_ON)
  SRC_GTMTOM10.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM11.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM12.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM13.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM14.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM15.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM16.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_GTMTOM17.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

}
#endif

#if (IRQ_CAN_EXIST == STD_ON)
static void Irq_ClearCanIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_CAN0_EXIST == STD_ON)
  SRC_CANINT0.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN1_EXIST == STD_ON)
  SRC_CANINT1.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN2_EXIST == STD_ON)
  SRC_CANINT2.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN3_EXIST == STD_ON)
  SRC_CANINT3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN4_EXIST == STD_ON)
  SRC_CANINT4.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN5_EXIST == STD_ON)
  SRC_CANINT5.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN6_EXIST == STD_ON)
  SRC_CANINT6.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN7_EXIST == STD_ON)
  SRC_CANINT7.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN8_EXIST == STD_ON)
  SRC_CANINT8.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN9_EXIST == STD_ON)
  SRC_CANINT9.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN10_EXIST == STD_ON)
  SRC_CANINT10.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN11_EXIST == STD_ON)
  SRC_CANINT11.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN12_EXIST == STD_ON)
  SRC_CANINT12.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN13_EXIST == STD_ON)
  SRC_CANINT13.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN14_EXIST == STD_ON)
  SRC_CANINT14.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN15_EXIST == STD_ON)
  SRC_CANINT15.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN16_EXIST == STD_ON)
  SRC_CAN1INT0.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN17_EXIST == STD_ON)
  SRC_CAN1INT1.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN18_EXIST == STD_ON)
  SRC_CAN1INT2.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN19_EXIST == STD_ON)
  SRC_CAN1INT3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN20_EXIST == STD_ON)
  SRC_CAN1INT4.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN21_EXIST == STD_ON)
  SRC_CAN1INT5.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN22_EXIST == STD_ON)
  SRC_CAN1INT6.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_CAN23_EXIST == STD_ON)
  SRC_CAN1INT7.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

}
#endif

#if (IRQ_GPSRGROUP_EXIST == STD_ON)
static void Irq_ClearGpsrGroupIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_GPSRGROUP0_EXIST == STD_ON)
  SRC_GPSR00.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
}
#endif

#if (IRQ_QSPI_EXIST == STD_ON)
static void Irq_ClearSpiIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_QSPI0_EXIST == STD_ON)
  SRC_QSPI0TX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI0RX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI0ERR.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI0PT.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI0U.U   = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_QSPI1_EXIST == STD_ON)
  SRC_QSPI1TX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI1RX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI1ERR.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI1PT.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI1U.U   = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_QSPI2_EXIST == STD_ON)
  SRC_QSPI2TX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI2RX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI2ERR.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI2PT.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI2HC.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI2U.U   = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_QSPI3_EXIST == STD_ON)
  SRC_QSPI3TX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI3RX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI3ERR.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI3PT.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI3HC.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI3U.U   = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_QSPI4_EXIST == STD_ON)
  SRC_QSPI4TX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI4RX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI4ERR.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI4PT.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI4HC.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI4U.U   = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_QSPI5_EXIST == STD_ON)
  SRC_QSPI5TX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI5RX.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI5ERR.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI5PT.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI5HC.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_QSPI5U.U   = IRQ_DISABLE_CLEAR_SRC;
  #endif

}
#endif

#if (IRQ_ADC_EXIST == STD_ON)
static void Irq_ClearAdcIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_ADC0_EXIST == STD_ON)
  SRC_VADCG0SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG0SR1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG0SR2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG0SR3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_ADC1_EXIST == STD_ON)
  SRC_VADCG1SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG1SR1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG1SR2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG1SR3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_ADC2_EXIST == STD_ON)
  SRC_VADCG2SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG2SR1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG2SR2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG2SR3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_ADC3_EXIST == STD_ON)
  SRC_VADCG3SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG3SR1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG3SR2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCG3SR3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_ADCCG0_EXIST == STD_ON)
  SRC_VADCCG0SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCCG0SR1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCCG0SR2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_VADCCG0SR3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

}
#endif

#if (IRQ_FLEXRAY_EXIST == STD_ON)
static void Irq_ClearFlexrayIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_FLEXRAY0_EXIST == STD_ON)
  SRC_ERAY_ERAY0_INT0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_INT1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_TINT0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_TINT1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_NDAT0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_NDAT1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_MBSC0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_MBSC1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_OBUSY.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_ERAY_ERAY0_IBUSY.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
}
#endif

#if (IRQ_ETH_EXIST == STD_ON)
static void Irq_ClearEthernetIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  SRC_ETH.U = IRQ_DISABLE_CLEAR_SRC;
}
#endif

#if (IRQ_DMA_EXIST == STD_ON)
static void Irq_ClearDmaIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  SRC_DMAERR.U = IRQ_DISABLE_CLEAR_SRC;

  #if (IRQ_DMA_CH0TO15_EXIST == STD_ON)
  SRC_DMACH0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH3.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH4.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH5.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH6.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH7.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH8.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH9.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH10.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH11.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH12.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH13.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH14.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_DMACH15.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

}
#endif

#if (IRQ_STM_EXIST == STD_ON)
static void Irq_ClearStmIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_STM0_EXIST == STD_ON)
  SRC_STM0SR0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_STM0SR1.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

}
#endif

#if (IRQ_SCU_EXIST == STD_ON)
static void Irq_ClearScuIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  SRC_SCUDTS.U  = IRQ_DISABLE_CLEAR_SRC;
  SRC_SCUERU0.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_SCUERU1.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_SCUERU2.U = IRQ_DISABLE_CLEAR_SRC;
  SRC_SCUERU3.U = IRQ_DISABLE_CLEAR_SRC;
}
#endif

#if (IRQ_PMU0_EXIST == STD_ON)
static void Irq_ClearPmuIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_PMU0_SR0_EXIST == STD_ON)
  SRC_PMU00.U = IRQ_DISABLE_CLEAR_SRC;
  #endif

  #if (IRQ_PMU0_SR1_EXIST == STD_ON)
  SRC_PMU01.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
}
#endif

#if (IRQ_SENT_EXIST == STD_ON)
static void Irq_ClearSentIntFlags (void)
{
  /* Set CLRR to clear SRR bit and disable SRE bit */
  #if (IRQ_SENT0_EXIST == STD_ON)
  SRC_SENT0.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
  #if (IRQ_SENT1_EXIST == STD_ON)
  SRC_SENT1.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
  #if (IRQ_SENT2_EXIST == STD_ON)
  SRC_SENT2.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
  #if (IRQ_SENT3_EXIST == STD_ON)
  SRC_SENT3.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
}
#endif

#if ((IRQ_HSM0_EXIST == STD_ON) || (IRQ_HSM1_EXIST == STD_ON))
static void Irq_ClearHsmIntFlags (void)
{
  #if (IRQ_HSM0_EXIST == STD_ON)
  SRC_HSM0.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
  #if (IRQ_HSM1_EXIST == STD_ON)
  SRC_HSM1.U = IRQ_DISABLE_CLEAR_SRC;
  #endif
}
#endif

#endif /*(IFX_MCAL_USED == STD_ON) */


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#if (IFX_MCAL_USED == STD_ON)
/*******************************************************************************
** Syntax :  void IrqAscLin_Init(void)                                        **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqAscLin_Init(void)
{
  #if (IRQ_ASCLIN_EXIST == STD_ON)

  #if (IRQ_ASCLIN0_EXIST == STD_ON)
  /* Reserve bit access is ensured for SRC register at all places accessed*/
  IRQ_SFR_MODIFY32(SRC_ASCLIN0TX.U, IRQ_CLEAR_MASK , \
                                   (IRQ_ASCLIN0_TX_TOS | IRQ_ASCLIN0_TX_PRIO));
  IRQ_SFR_MODIFY32(SRC_ASCLIN0RX.U, IRQ_CLEAR_MASK ,  \
                                   (IRQ_ASCLIN0_RX_TOS | IRQ_ASCLIN0_RX_PRIO));
  IRQ_SFR_MODIFY32(SRC_ASCLIN0ERR.U, IRQ_CLEAR_MASK ,  \
                                  (IRQ_ASCLIN0_ERR_TOS | IRQ_ASCLIN0_ERR_PRIO));
  #endif

  #if (IRQ_ASCLIN1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32(SRC_ASCLIN1TX.U, IRQ_CLEAR_MASK , \
                                    (IRQ_ASCLIN1_TX_TOS | IRQ_ASCLIN1_TX_PRIO));
  IRQ_SFR_MODIFY32(SRC_ASCLIN1RX.U, IRQ_CLEAR_MASK ,  \
                                    (IRQ_ASCLIN1_RX_TOS | IRQ_ASCLIN1_RX_PRIO));
  IRQ_SFR_MODIFY32(SRC_ASCLIN1ERR.U, IRQ_CLEAR_MASK , \
                                  (IRQ_ASCLIN1_ERR_TOS | IRQ_ASCLIN1_ERR_PRIO));
  #endif

  #endif

}

/*******************************************************************************
** Syntax :  void IrqCcu6_Init(void)                                          **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqCcu6_Init(void)
{
  #if (IRQ_CCU6_EXIST == STD_ON)
  #if (IRQ_CCU60_EXIST == STD_ON)
  IRQ_SFR_MODIFY32(SRC_CCU60SR0.U,IRQ_CLEAR_MASK, \
                                      (IRQ_CCU60_SR0_TOS | IRQ_CCU60_SR0_PRIO));
  IRQ_SFR_MODIFY32(SRC_CCU60SR1.U,IRQ_CLEAR_MASK, \
                                      (IRQ_CCU60_SR1_TOS | IRQ_CCU60_SR1_PRIO));
  IRQ_SFR_MODIFY32(SRC_CCU60SR2.U,IRQ_CLEAR_MASK, \
                                      (IRQ_CCU60_SR2_TOS | IRQ_CCU60_SR2_PRIO));
  IRQ_SFR_MODIFY32(SRC_CCU60SR3.U,IRQ_CLEAR_MASK,  \
                                      (IRQ_CCU60_SR3_TOS | IRQ_CCU60_SR3_PRIO));
  #endif

  #if (IRQ_CCU61_EXIST == STD_ON)
  IRQ_SFR_MODIFY32(SRC_CCU61SR0.U, IRQ_CLEAR_MASK , \
                                     (IRQ_CCU61_SR0_TOS | IRQ_CCU61_SR0_PRIO));
  IRQ_SFR_MODIFY32(SRC_CCU61SR1.U, IRQ_CLEAR_MASK , \
                                     (IRQ_CCU61_SR1_TOS | IRQ_CCU61_SR1_PRIO));
  IRQ_SFR_MODIFY32(SRC_CCU61SR2.U, IRQ_CLEAR_MASK , \
                                     (IRQ_CCU61_SR2_TOS | IRQ_CCU61_SR2_PRIO));
  IRQ_SFR_MODIFY32(SRC_CCU61SR3.U, IRQ_CLEAR_MASK , \
                                     (IRQ_CCU61_SR3_TOS | IRQ_CCU61_SR3_PRIO));
  #endif

  #endif
}
/*******************************************************************************
** Syntax :  void IrqGpt_Init(void)                                         **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqGpt_Init(void)
{
  #if (IRQ_GPT_EXIST == STD_ON)

  #if (IRQ_GPT120_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_GPT120CIRQ.U, IRQ_CLEAR_MASK , \
                              (IRQ_GPT120_CARPEL_TOS | IRQ_GPT120_CARPEL_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GPT120T2.U, IRQ_CLEAR_MASK , \
                                (IRQ_GPT120_T2_TOS | IRQ_GPT120_T2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GPT120T3.U, IRQ_CLEAR_MASK , \
                                (IRQ_GPT120_T3_TOS | IRQ_GPT120_T3_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GPT120T4.U, IRQ_CLEAR_MASK , \
                                (IRQ_GPT120_T4_TOS | IRQ_GPT120_T4_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GPT120T5.U, IRQ_CLEAR_MASK , \
                                (IRQ_GPT120_T5_TOS | IRQ_GPT120_T5_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GPT120T6.U, IRQ_CLEAR_MASK , \
                                (IRQ_GPT120_T6_TOS | IRQ_GPT120_T6_PRIO));
  #endif

  #endif
}
/*******************************************************************************
** Syntax :  void IrqGtm_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/
void IrqGtm_Init(void)
{
  #if (IRQ_GTM_EXIST == STD_ON)

  #if (IRQ_GTM_AEI_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_GTMAEIIRQ.U, IRQ_CLEAR_MASK , \
                                      (IRQ_GTM_AEI_TOS | IRQ_GTM_AEI_PRIO));
  #endif


  #if (IRQ_GTM_ERR_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_GTMERR.U, IRQ_CLEAR_MASK ,  \
                                 (IRQ_GTM_ERR_SR_TOS | IRQ_GTM_ERR_SR_PRIO));
  #endif

  #if (IRQ_GTM_TIM0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_GTMTIM00.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR0_TOS | IRQ_GTM_TIM0_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTIM01.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR1_TOS | IRQ_GTM_TIM0_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTIM02.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR2_TOS | IRQ_GTM_TIM0_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTIM03.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR3_TOS | IRQ_GTM_TIM0_SR3_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTIM04.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR4_TOS | IRQ_GTM_TIM0_SR4_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTIM05.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR5_TOS | IRQ_GTM_TIM0_SR5_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTIM06.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR6_TOS | IRQ_GTM_TIM0_SR6_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTIM07.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TIM0_SR7_TOS | IRQ_GTM_TIM0_SR7_PRIO));
  #endif


  #if (IRQ_GTM_TOM0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_GTMTOM00.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR0_TOS | IRQ_GTM_TOM0_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM01.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR1_TOS | IRQ_GTM_TOM0_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM02.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR2_TOS | IRQ_GTM_TOM0_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM03.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR3_TOS | IRQ_GTM_TOM0_SR3_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM04.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR4_TOS | IRQ_GTM_TOM0_SR4_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM05.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR5_TOS | IRQ_GTM_TOM0_SR5_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM06.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR6_TOS | IRQ_GTM_TOM0_SR6_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM07.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM0_SR7_TOS | IRQ_GTM_TOM0_SR7_PRIO));
  #endif

  #if (IRQ_GTM_TOM1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_GTMTOM10.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM1_SR0_TOS | IRQ_GTM_TOM1_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM11.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM1_SR1_TOS | IRQ_GTM_TOM1_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM12.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM1_SR2_TOS | IRQ_GTM_TOM1_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM13.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM1_SR3_TOS | IRQ_GTM_TOM1_SR3_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM14.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM1_SR4_TOS | IRQ_GTM_TOM1_SR4_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM15.U, IRQ_CLEAR_MASK , \
                                (IRQ_GTM_TOM1_SR5_TOS | IRQ_GTM_TOM1_SR5_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM16.U, IRQ_CLEAR_MASK ,  \
                                (IRQ_GTM_TOM1_SR6_TOS | IRQ_GTM_TOM1_SR6_PRIO));
  IRQ_SFR_MODIFY32 (SRC_GTMTOM17.U, IRQ_CLEAR_MASK ,  \
                                (IRQ_GTM_TOM1_SR7_TOS | IRQ_GTM_TOM1_SR7_PRIO));

  #endif

  #endif
}

/*******************************************************************************
** Syntax :  void IrqCan_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/
void IrqCan_Init(void)
{

  #if (IRQ_CAN_EXIST == STD_ON)
  /* Interrupt Priority is written to the SRC registers*/
  #if (IRQ_CAN0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT0.U, IRQ_CLEAR_MASK ,  \
                                          (IRQ_CAN_SR0_TOS | IRQ_CAN_SR0_PRIO));
  #endif

  #if (IRQ_CAN1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT1.U, IRQ_CLEAR_MASK , \
                                          (IRQ_CAN_SR1_TOS | IRQ_CAN_SR1_PRIO));
  #endif

  #if (IRQ_CAN2_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT2.U, IRQ_CLEAR_MASK , \
                                          (IRQ_CAN_SR2_TOS | IRQ_CAN_SR2_PRIO));
  #endif

  #if (IRQ_CAN3_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT3.U, IRQ_CLEAR_MASK , \
                                          (IRQ_CAN_SR3_TOS | IRQ_CAN_SR3_PRIO));
  #endif

  #if (IRQ_CAN4_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT4.U, IRQ_CLEAR_MASK , \
                                          (IRQ_CAN_SR4_TOS | IRQ_CAN_SR4_PRIO));
  #endif

  #if (IRQ_CAN5_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT5.U, IRQ_CLEAR_MASK , \
                                         (IRQ_CAN_SR5_TOS | IRQ_CAN_SR5_PRIO));
  #endif

  #if (IRQ_CAN6_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT6.U, IRQ_CLEAR_MASK , \
                                          (IRQ_CAN_SR6_TOS | IRQ_CAN_SR6_PRIO));
  #endif

  #if (IRQ_CAN7_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT7.U, IRQ_CLEAR_MASK , \
                                          (IRQ_CAN_SR7_TOS | IRQ_CAN_SR7_PRIO));
  #endif

  #if (IRQ_CAN8_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT8.U, IRQ_CLEAR_MASK , \
                                          (IRQ_CAN_SR8_TOS | IRQ_CAN_SR8_PRIO));
  #endif

  #if (IRQ_CAN9_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT9.U, IRQ_CLEAR_MASK , \
                                         (IRQ_CAN_SR9_TOS | IRQ_CAN_SR9_PRIO));
  #endif

  #if (IRQ_CAN10_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT10.U, IRQ_CLEAR_MASK , \
                                       (IRQ_CAN_SR10_TOS | IRQ_CAN_SR10_PRIO));
  #endif

  #if (IRQ_CAN11_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT11.U, IRQ_CLEAR_MASK , \
                                        (IRQ_CAN_SR11_TOS | IRQ_CAN_SR11_PRIO));
  #endif

  #if (IRQ_CAN12_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT12.U, IRQ_CLEAR_MASK , \
                                    (IRQ_CAN_SR12_TOS | IRQ_CAN_SR12_PRIO));
  #endif

  #if (IRQ_CAN13_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT13.U, IRQ_CLEAR_MASK , \
                                       (IRQ_CAN_SR13_TOS | IRQ_CAN_SR13_PRIO));
  #endif

  #if (IRQ_CAN14_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT14.U, IRQ_CLEAR_MASK , \
                                       (IRQ_CAN_SR14_TOS | IRQ_CAN_SR14_PRIO));
  #endif

  #if (IRQ_CAN15_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CANINT15.U, IRQ_CLEAR_MASK , \
                                        (IRQ_CAN_SR15_TOS | IRQ_CAN_SR15_PRIO));
  #endif

  #if (IRQ_CAN16_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT0.U, IRQ_CLEAR_MASK , \
                                      (IRQ_CAN_SR16_TOS | IRQ_CAN_SR16_PRIO));
  #endif

  #if (IRQ_CAN17_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT1.U, IRQ_CLEAR_MASK , \
                                       (IRQ_CAN_SR17_TOS | IRQ_CAN_SR17_PRIO));
  #endif

  #if (IRQ_CAN18_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT2.U, IRQ_CLEAR_MASK ,  \
                                        (IRQ_CAN_SR18_TOS | IRQ_CAN_SR18_PRIO));
  #endif

  #if (IRQ_CAN19_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT3.U, IRQ_CLEAR_MASK ,  \
                                        (IRQ_CAN_SR19_TOS | IRQ_CAN_SR19_PRIO));
  #endif

  #if (IRQ_CAN20_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT4.U, IRQ_CLEAR_MASK ,  \
                                  (IRQ_CAN_SR20_TOS | IRQ_CAN_SR20_PRIO));
  #endif

  #if (IRQ_CAN21_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT5.U, IRQ_CLEAR_MASK ,  \
                                      (IRQ_CAN_SR21_TOS | IRQ_CAN_SR21_PRIO));
  #endif

  #if (IRQ_CAN22_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT6.U, IRQ_CLEAR_MASK ,  \
                                        (IRQ_CAN_SR22_TOS | IRQ_CAN_SR22_PRIO));
  #endif

  #if (IRQ_CAN23_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_CAN1INT7.U, IRQ_CLEAR_MASK ,  \
                                      (IRQ_CAN_SR23_TOS | IRQ_CAN_SR23_PRIO));
  #endif

  #endif
 }


/*******************************************************************************
** Syntax :  void IrqGpsrGroup_Init(void)                                     **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for Fls                        **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/
void IrqGpsrGroup_Init(void)
{
  #if (IRQ_GPSRGROUP_EXIST == STD_ON)

  #if (IRQ_GPSRGROUP0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_GPSR00.U, IRQ_CLEAR_MASK , \
                           (IRQ_GPSRGROUP0_SR0_TOS | IRQ_GPSRGROUP0_SR0_PRIO));
  #endif

  #endif
}

/********************************************************************************
** Syntax :  void IrqSpi_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqSpi_Init(void)
{

  #if (IRQ_QSPI_EXIST == STD_ON)

  #if (IRQ_QSPI0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_QSPI0TX.U, IRQ_CLEAR_MASK ,  \
                                       (IRQ_QSPI0_TX_TOS | IRQ_QSPI0_TX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI0RX.U, IRQ_CLEAR_MASK ,  \
                                       (IRQ_QSPI0_RX_TOS | IRQ_QSPI0_RX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI0ERR.U, IRQ_CLEAR_MASK , \
                                     (IRQ_QSPI0_ERR_TOS | IRQ_QSPI0_ERR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI0PT.U, IRQ_CLEAR_MASK , \
                                      (IRQ_QSPI0_PT_TOS | IRQ_QSPI0_PT_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI0U.U, IRQ_CLEAR_MASK ,  \
                                       (IRQ_QSPI0_UD_TOS | IRQ_QSPI0_UD_PRIO));
  #endif /*IRQ_QSPI0_EXIST == STD_ON*/

  #if (IRQ_QSPI1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_QSPI1TX.U, IRQ_CLEAR_MASK , \
                                        (IRQ_QSPI1_TX_TOS | IRQ_QSPI1_TX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI1RX.U, IRQ_CLEAR_MASK , \
                                       (IRQ_QSPI1_RX_TOS | IRQ_QSPI1_RX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI1ERR.U, IRQ_CLEAR_MASK , \
                                      (IRQ_QSPI1_ERR_TOS | IRQ_QSPI1_ERR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI1PT.U, IRQ_CLEAR_MASK ,  \
                                        (IRQ_QSPI1_PT_TOS | IRQ_QSPI1_PT_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI1U.U, IRQ_CLEAR_MASK ,  \
                                        (IRQ_QSPI1_UD_TOS | IRQ_QSPI1_UD_PRIO));
  #endif

  #if (IRQ_QSPI2_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_QSPI2TX.U, IRQ_CLEAR_MASK , \
                                     (IRQ_QSPI2_TX_TOS | IRQ_QSPI2_TX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI2RX.U, IRQ_CLEAR_MASK ,  \
                                       (IRQ_QSPI2_RX_TOS | IRQ_QSPI2_RX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI2ERR.U, IRQ_CLEAR_MASK , \
                                     (IRQ_QSPI2_ERR_TOS | IRQ_QSPI2_ERR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI2PT.U, IRQ_CLEAR_MASK , \
                                     (IRQ_QSPI2_PT_TOS | IRQ_QSPI2_PT_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI2HC.U, IRQ_CLEAR_MASK , \
                                       (IRQ_QSPI2_HC_TOS | IRQ_QSPI2_HC_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI2U.U, IRQ_CLEAR_MASK ,  \
                                     (IRQ_QSPI2_UD_TOS | IRQ_QSPI2_UD_PRIO));
  #endif

  #if (IRQ_QSPI3_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_QSPI3TX.U, IRQ_CLEAR_MASK , \
                                      (IRQ_QSPI3_TX_TOS | IRQ_QSPI3_TX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI3RX.U, IRQ_CLEAR_MASK , \
                                    (IRQ_QSPI3_RX_TOS | IRQ_QSPI3_RX_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI3ERR.U, IRQ_CLEAR_MASK , \
                                      (IRQ_QSPI3_ERR_TOS | IRQ_QSPI3_ERR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI3PT.U, IRQ_CLEAR_MASK ,  \
                                        (IRQ_QSPI3_PT_TOS | IRQ_QSPI3_PT_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI3HC.U, IRQ_CLEAR_MASK ,  \
                                        (IRQ_QSPI3_HC_TOS | IRQ_QSPI3_HC_PRIO));
  IRQ_SFR_MODIFY32 (SRC_QSPI3U.U, IRQ_CLEAR_MASK ,  \
                                     (IRQ_QSPI3_UD_TOS | IRQ_QSPI3_UD_PRIO));
  #endif

  #endif
}

/*******************************************************************************
** Syntax :  void IrqAdc_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/
void IrqAdc_Init(void)
{

  #if (IRQ_ADC_EXIST == STD_ON)

  #if (IRQ_ADC0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_VADCG0SR0.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC0_SR0_TOS | IRQ_ADC0_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG0SR1.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC0_SR1_TOS | IRQ_ADC0_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG0SR2.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC0_SR2_TOS | IRQ_ADC0_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG0SR3.U, IRQ_CLEAR_MASK , \
                                       (IRQ_ADC0_SR3_TOS | IRQ_ADC0_SR3_PRIO));
  #endif

  #if (IRQ_ADC1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_VADCG1SR0.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC1_SR0_TOS | IRQ_ADC1_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG1SR1.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC1_SR1_TOS | IRQ_ADC1_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG1SR2.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC1_SR2_TOS | IRQ_ADC1_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG1SR3.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC1_SR3_TOS | IRQ_ADC1_SR3_PRIO));
  #endif

  #if (IRQ_ADC2_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_VADCG2SR0.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC2_SR0_TOS | IRQ_ADC2_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG2SR1.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC2_SR1_TOS | IRQ_ADC2_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG2SR2.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC2_SR2_TOS | IRQ_ADC2_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG2SR3.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC2_SR3_TOS | IRQ_ADC2_SR3_PRIO));
  #endif

  #if (IRQ_ADC3_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_VADCG3SR0.U, IRQ_CLEAR_MASK , \
                                       (IRQ_ADC3_SR0_TOS | IRQ_ADC3_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG3SR1.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC3_SR1_TOS | IRQ_ADC3_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG3SR2.U, IRQ_CLEAR_MASK , \
                                        (IRQ_ADC3_SR2_TOS | IRQ_ADC3_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCG3SR3.U, IRQ_CLEAR_MASK , \
                                       (IRQ_ADC3_SR3_TOS | IRQ_ADC3_SR3_PRIO));
  #endif

  #if (IRQ_ADCCG0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_VADCCG0SR0.U, IRQ_CLEAR_MASK , \
                                    (IRQ_ADCCG0_SR0_TOS | IRQ_ADCCG0_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCCG0SR1.U, IRQ_CLEAR_MASK , \
                                    (IRQ_ADCCG0_SR1_TOS | IRQ_ADCCG0_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCCG0SR2.U, IRQ_CLEAR_MASK , \
                                    (IRQ_ADCCG0_SR2_TOS | IRQ_ADCCG0_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_VADCCG0SR3.U, IRQ_CLEAR_MASK , \
                                    (IRQ_ADCCG0_SR3_TOS | IRQ_ADCCG0_SR3_PRIO));
  #endif

  #endif /* (IRQ_ADC_EXIST == STD_ON) */
}





/*******************************************************************************
** Syntax :  void IrqEray_Init(void)                                          **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for                            **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqFlexray_Init(void)
{
  #if (IRQ_FLEXRAY_EXIST == STD_ON)

  #if (IRQ_FLEXRAY0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_INT0.U, IRQ_CLEAR_MASK , \
                               (IRQ_FLEXRAY0_SR0_TOS | IRQ_FLEXRAY0_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_INT1.U, IRQ_CLEAR_MASK , \
                               (IRQ_FLEXRAY0_SR1_TOS | IRQ_FLEXRAY0_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_TINT0.U, IRQ_CLEAR_MASK , \
                  (IRQ_FLEXRAY0_TIMER_INT0_TOS | IRQ_FLEXRAY0_TIMER_INT0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_TINT1.U, IRQ_CLEAR_MASK , \
                  (IRQ_FLEXRAY0_TIMER_INT1_TOS | IRQ_FLEXRAY0_TIMER_INT1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_NDAT0.U, IRQ_CLEAR_MASK , \
                   (IRQ_FLEXRAY0_NEW_DATA0_TOS | IRQ_FLEXRAY0_NEW_DATA0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_NDAT1.U, IRQ_CLEAR_MASK , \
                   (IRQ_FLEXRAY0_NEW_DATA1_TOS | IRQ_FLEXRAY0_NEW_DATA1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_MBSC0.U, IRQ_CLEAR_MASK , \
                           (IRQ_FLEXRAY0_MBSC0_TOS | IRQ_FLEXRAY0_MBSC0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_MBSC1.U, IRQ_CLEAR_MASK , \
                           (IRQ_FLEXRAY0_MBSC1_TOS | IRQ_FLEXRAY0_MBSC1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_OBUSY.U, IRQ_CLEAR_MASK , \
                       (IRQ_FLEXRAY0_OB_BUSY_TOS | IRQ_FLEXRAY0_OB_BUSY_PRIO));
  IRQ_SFR_MODIFY32 (SRC_ERAY_ERAY0_IBUSY.U, IRQ_CLEAR_MASK , \
                        (IRQ_FLEXRAY0_IB_BUSY_TOS | IRQ_FLEXRAY0_IB_BUSY_PRIO));
  #endif

  #endif
}

/*******************************************************************************
** Syntax :  void IrqEthernet_Init(void)                                      **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for                            **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqEthernet_Init(void)
{
  #if (IRQ_ETH_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_ETH.U, IRQ_CLEAR_MASK , \
                                            (IRQ_ETH_SR_TOS |IRQ_ETH_SR_PRIO));
  #endif
}

/*******************************************************************************
** Syntax :  void IrqDma_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for                            **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqDma_Init(void)
{
  #if (IRQ_DMA_EXIST == STD_ON)

  SRC_DMAERR.U |= (IRQ_DMA_ERR_SR_TOS | IRQ_DMA_ERR_SR_PRIO);

  #if (IRQ_DMA_CH0TO15_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_DMACH0.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL0_SR_TOS |IRQ_DMA_CHANNEL0_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH1.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL1_SR_TOS |IRQ_DMA_CHANNEL1_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH2.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL2_SR_TOS |IRQ_DMA_CHANNEL2_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH3.U, IRQ_CLEAR_MASK ,\
                         (IRQ_DMA_CHANNEL3_SR_TOS |IRQ_DMA_CHANNEL3_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH4.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL4_SR_TOS |IRQ_DMA_CHANNEL4_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH5.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL5_SR_TOS |IRQ_DMA_CHANNEL5_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH6.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL6_SR_TOS |IRQ_DMA_CHANNEL6_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH7.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL7_SR_TOS |IRQ_DMA_CHANNEL7_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH8.U, IRQ_CLEAR_MASK ,\
                          (IRQ_DMA_CHANNEL8_SR_TOS |IRQ_DMA_CHANNEL8_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH9.U, IRQ_CLEAR_MASK ,\
                           (IRQ_DMA_CHANNEL9_SR_TOS |IRQ_DMA_CHANNEL9_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH10.U, IRQ_CLEAR_MASK ,\
                         (IRQ_DMA_CHANNEL10_SR_TOS |IRQ_DMA_CHANNEL10_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH11.U, IRQ_CLEAR_MASK ,\
                         (IRQ_DMA_CHANNEL11_SR_TOS |IRQ_DMA_CHANNEL11_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH12.U, IRQ_CLEAR_MASK ,\
                         (IRQ_DMA_CHANNEL12_SR_TOS |IRQ_DMA_CHANNEL12_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH13.U, IRQ_CLEAR_MASK ,\
                        (IRQ_DMA_CHANNEL13_SR_TOS |IRQ_DMA_CHANNEL13_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH14.U, IRQ_CLEAR_MASK ,\
                         (IRQ_DMA_CHANNEL14_SR_TOS |IRQ_DMA_CHANNEL14_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_DMACH15.U, IRQ_CLEAR_MASK ,\
                        (IRQ_DMA_CHANNEL15_SR_TOS |IRQ_DMA_CHANNEL15_SR_PRIO));
  #endif

  #endif/*End of IRQ_DMA_EXIST*/
}
#endif /* (IFX_MCAL_USED == STD_ON) */
/*******************************************************************************
** Syntax :  void IrqStm_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for                            **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqStm_Init(void)
{
  #if (IRQ_STM_EXIST == STD_ON)

  #if (IRQ_STM0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_STM0SR0.U, IRQ_CLEAR_MASK ,\
                                       (IRQ_STM0_SR0_TOS |IRQ_STM0_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_STM0SR1.U, IRQ_CLEAR_MASK ,\
                                       (IRQ_STM0_SR1_TOS |IRQ_STM0_SR1_PRIO));
  #endif

  #endif
}

#if (IFX_MCAL_USED == STD_ON)
/*******************************************************************************
** Syntax :  void IrqScu_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for                            **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqScu_Init(void)
{
  #if (IRQ_SCU_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_SCUDTS.U, IRQ_CLEAR_MASK ,\
                         (IRQ_SCU_DTS_BUSY_SR_TOS |IRQ_SCU_DTS_BUSY_SR_PRIO));
  IRQ_SFR_MODIFY32 (SRC_SCUERU0.U, IRQ_CLEAR_MASK ,\
                                  (IRQ_SCU_ERU_SR0_TOS |IRQ_SCU_ERU_SR0_PRIO));
  IRQ_SFR_MODIFY32 (SRC_SCUERU1.U, IRQ_CLEAR_MASK ,\
                                  (IRQ_SCU_ERU_SR1_TOS |IRQ_SCU_ERU_SR1_PRIO));
  IRQ_SFR_MODIFY32 (SRC_SCUERU2.U, IRQ_CLEAR_MASK ,\
                                   (IRQ_SCU_ERU_SR2_TOS |IRQ_SCU_ERU_SR2_PRIO));
  IRQ_SFR_MODIFY32 (SRC_SCUERU3.U, IRQ_CLEAR_MASK ,\
                                   (IRQ_SCU_ERU_SR3_TOS |IRQ_SCU_ERU_SR3_PRIO));
  #endif


}

/*******************************************************************************
** Syntax :  void IrqPmu_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for                            **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/
void IrqPmu_Init(void)
{
  #if (IRQ_PMU0_EXIST == STD_ON)

  #if (IRQ_PMU0_SR0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_PMU00.U, IRQ_CLEAR_MASK ,\
                                     (IRQ_PMU0_SR0_TOS |IRQ_PMU0_SR0_PRIO));
  #endif

  #if (IRQ_PMU0_SR1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_PMU01.U, IRQ_CLEAR_MASK ,\
                                     (IRQ_PMU0_SR1_TOS |IRQ_PMU0_SR1_PRIO));
  #endif

  #endif
}


/*******************************************************************************
** Syntax :  void IrqSent_Init(void)                                          **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for                            **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/

void IrqSent_Init(void)
{
  #if (IRQ_SENT_EXIST == STD_ON)

  #if (IRQ_SENT0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_SENT0.U, IRQ_CLEAR_MASK ,\
                                       (IRQ_SENT_SR0_TOS |IRQ_SENT_SR0_PRIO));
  #endif

  #if (IRQ_SENT1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_SENT1.U, IRQ_CLEAR_MASK ,\
                                       (IRQ_SENT_SR1_TOS |IRQ_SENT_SR1_PRIO));
  #endif

  #if (IRQ_SENT2_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_SENT2.U, IRQ_CLEAR_MASK ,\
                                       (IRQ_SENT_SR2_TOS |IRQ_SENT_SR2_PRIO));
  #endif

  #if (IRQ_SENT3_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_SENT3.U, IRQ_CLEAR_MASK ,\
                                       (IRQ_SENT_SR3_TOS |IRQ_SENT_SR3_PRIO));
  #endif

  #endif
}

/*******************************************************************************
** Syntax :  void IrqHsm_Init(void)                                           **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To set the interrupt priority for various                    **
**               service nodes according to priority configurartion.          **
**                                                                            **
*******************************************************************************/
void IrqHsm_Init(void)
{
  /* Interrupt Priority is written to the SRC registers*/

  #if (IRQ_HSM0_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_HSM0.U, IRQ_CLEAR_MASK ,\
                                          (IRQ_HSM_SR0_TOS |IRQ_HSM_SR0_PRIO));
  #endif

  #if (IRQ_HSM1_EXIST == STD_ON)
  IRQ_SFR_MODIFY32 (SRC_HSM1.U, IRQ_CLEAR_MASK ,\
                                           (IRQ_HSM_SR1_TOS |IRQ_HSM_SR1_PRIO));
  #endif
}

/*******************************************************************************
** Syntax :  void Irq_ClearAllInterruptFlags(void)                            **
**                                                                            **
** Service ID:  none                                                          **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  non reentrant                                                 **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : To clear all SRR and corresponding SRE bits.This ensures     **
** Standby mode can be entered if no pending interrupts are available.        **
**                                                                            **
*******************************************************************************/
void Irq_ClearAllInterruptFlags(void)
{
  Mcal_DisableAllInterrupts();

  #if (IRQ_ASCLIN_EXIST == STD_ON)
  Irq_ClearAsclinIntFlags();
  #endif

  #if (IRQ_CCU6_EXIST == STD_ON)
  Irq_ClearCcu6IntFlags();
  #endif

  #if (IRQ_GPT_EXIST == STD_ON)
  Irq_ClearGptIntFlags();
  #endif

  #if (IRQ_GTM_EXIST == STD_ON)
  Irq_ClearGtmIntFlags();
  #endif

  #if (IRQ_CAN_EXIST == STD_ON)
  Irq_ClearCanIntFlags();
  #endif

  #if (IRQ_GPSRGROUP_EXIST == STD_ON)
  Irq_ClearGpsrGroupIntFlags();
  #endif

  #if (IRQ_QSPI_EXIST == STD_ON)
  Irq_ClearSpiIntFlags();
  #endif

  #if (IRQ_ADC_EXIST == STD_ON)
  Irq_ClearAdcIntFlags();
  #endif

  #if (IRQ_FLEXRAY_EXIST == STD_ON)
  Irq_ClearFlexrayIntFlags();
  #endif

  #if (IRQ_ETH_EXIST == STD_ON)
  Irq_ClearEthernetIntFlags();
  #endif

  #if (IRQ_DMA_EXIST == STD_ON)
  Irq_ClearDmaIntFlags();
  #endif

  #if (IRQ_STM_EXIST == STD_ON)
  Irq_ClearStmIntFlags();
  #endif

  #if (IRQ_SCU_EXIST == STD_ON)
  Irq_ClearScuIntFlags();
  #endif

  #if (IRQ_PMU0_EXIST == STD_ON)
  Irq_ClearPmuIntFlags();
  #endif

  #if (IRQ_SENT_EXIST == STD_ON)
  Irq_ClearSentIntFlags();
  #endif

  #if ((IRQ_HSM0_EXIST == STD_ON) || (IRQ_HSM1_EXIST == STD_ON))
  Irq_ClearHsmIntFlags();
  #endif

}
#endif /* (IFX_MCAL_USED == STD_ON) */

#if (IFX_MCAL_USED == STD_ON)
#define IRQ_STOP_SEC_CODE
#include "MemMap.h"
#else
#define IFX_IRQ_STOP_SEC_CODE_ASIL_B
#include "Ifx_MemMap.h"
#endif


