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
**  $FILENAME   : Dma_Irq.c $                                                **
**                                                                           **
**  $CC VERSION : \main\dev_tc23x\3 $                                        **
**                                                                           **
**  $DATE       : 2016-06-27 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains                                         **
**                 - functionality of DMA driver.                            **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Global functions like Set/Reset END INIT protection bit,
  Enable/Disable interrupts, Automic write function */
#include "Mcal.h"

/*Include Irq Module*/
#include "Irq.h"

#include "EcuM.h"

#if defined (ECUM_USES_SPI)
#include "Spi.h"
#endif

#if defined (ECUM_USES_ADC)
#include "Adc.h"
#endif
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

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define IRQ_START_SEC_CODE
#include "MemMap.h"
/******************************************************************************
** Syntax : void DMAERRSR_ISR(void)                                          **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA error interrupts which can be due to Source **
**               ,destination, TCS, RAM or safelinked list checksum error.   **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_ERR_SR_PRIO > 0U) || (IRQ_DMA_ERR_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_ERR_SR_PRIO > 0U) && (IRQ_DMA_ERR_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMAERRSR_ISR, 0, IRQ_DMA_ERR_SR_PRIO)
#elif IRQ_DMA_ERR_SR_CAT == IRQ_CAT23
ISR(DMAERRSR_ISR)
#endif
{
  /* Enable Global Interrupts */
#if (IRQ_DMA_ERR_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif
}
#endif
#if (IRQ_DMA_CH0TO15_EXIST == STD_ON)
/******************************************************************************
** Syntax : void DMACH0SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel0 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL0_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL0_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL0_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL0_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH0SR_ISR, 0, IRQ_DMA_CHANNEL0_SR_PRIO)
#elif IRQ_DMA_CHANNEL0_SR_CAT == IRQ_CAT23
ISR(DMACH0SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL0_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL0_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL0_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL0_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL0_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL0_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL0_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL0_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL0_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL0_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL0_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL0_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL0_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL0_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH1SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel1 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL1_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL1_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL1_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL1_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH1SR_ISR, 0, IRQ_DMA_CHANNEL1_SR_PRIO)
#elif IRQ_DMA_CHANNEL1_SR_CAT == IRQ_CAT23
ISR(DMACH1SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL1_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL1_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL1_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL1_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL1_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL1_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL1_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL1_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL1_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL1_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL1_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL1_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL1_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL1_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH2SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel2 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL2_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL2_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL2_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL2_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH2SR_ISR, 0, IRQ_DMA_CHANNEL2_SR_PRIO)
#elif IRQ_DMA_CHANNEL2_SR_CAT == IRQ_CAT23
ISR(DMACH2SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL2_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL2_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL2_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL2_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL2_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL2_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL2_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL2_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL2_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL2_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL2_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL2_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL2_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL2_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH3SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel3 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL3_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL3_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL3_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL3_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH3SR_ISR, 0, IRQ_DMA_CHANNEL3_SR_PRIO)
#elif IRQ_DMA_CHANNEL3_SR_CAT == IRQ_CAT23
ISR(DMACH3SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL3_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL3_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL3_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL3_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL3_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL3_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL3_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL3_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL3_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL3_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL3_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL3_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL3_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL3_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH4SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel4 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL4_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL4_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL4_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL4_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH4SR_ISR, 0, IRQ_DMA_CHANNEL4_SR_PRIO)
#elif IRQ_DMA_CHANNEL4_SR_CAT == IRQ_CAT23
ISR(DMACH4SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL4_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL4_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL4_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL4_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL4_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL4_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL4_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL4_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL4_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL4_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL4_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL4_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL4_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL4_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH5SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel5 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL5_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL5_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL5_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL5_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH5SR_ISR, 0, IRQ_DMA_CHANNEL5_SR_PRIO)
#elif IRQ_DMA_CHANNEL5_SR_CAT == IRQ_CAT23
ISR(DMACH5SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL5_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL5_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL5_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL5_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL5_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL5_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL5_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL5_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL5_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL5_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL5_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL5_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL5_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL5_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH6SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel6 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL6_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL6_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL6_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL6_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH6SR_ISR, 0, IRQ_DMA_CHANNEL6_SR_PRIO)
#elif IRQ_DMA_CHANNEL6_SR_CAT == IRQ_CAT23
ISR(DMACH6SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL6_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL6_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL6_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL6_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL6_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL6_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL6_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL6_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL6_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL6_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL6_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL6_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL6_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL6_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH7SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel7 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL7_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL7_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL7_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL7_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH7SR_ISR, 0, IRQ_DMA_CHANNEL7_SR_PRIO)
#elif IRQ_DMA_CHANNEL7_SR_CAT == IRQ_CAT23
ISR(DMACH7SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL7_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL7_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL7_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL7_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL7_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL7_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL7_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL7_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL7_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL7_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL7_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL7_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL7_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL7_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH8SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel8 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL8_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL8_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL8_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL8_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH8SR_ISR, 0, IRQ_DMA_CHANNEL8_SR_PRIO)
#elif IRQ_DMA_CHANNEL8_SR_CAT == IRQ_CAT23
ISR(DMACH8SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL8_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL8_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL8_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL8_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL8_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL8_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL8_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL8_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL8_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL8_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL8_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL8_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL8_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL8_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH9SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel9 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL9_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL9_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL9_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL9_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH9SR_ISR, 0, IRQ_DMA_CHANNEL9_SR_PRIO)
#elif IRQ_DMA_CHANNEL9_SR_CAT == IRQ_CAT23
ISR(DMACH9SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL9_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL9_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL9_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL9_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL9_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL9_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL9_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL9_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL9_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL9_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL9_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL9_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL9_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL9_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH10SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel10 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL10_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL10_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL10_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL10_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH10SR_ISR, 0, IRQ_DMA_CHANNEL10_SR_PRIO)
#elif IRQ_DMA_CHANNEL10_SR_CAT == IRQ_CAT23
ISR(DMACH10SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL10_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL10_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL10_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL10_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL10_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL10_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL10_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL10_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL10_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL10_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL10_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL10_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL10_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL10_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH11SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel11 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL11_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL11_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL11_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL11_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH11SR_ISR, 0, IRQ_DMA_CHANNEL11_SR_PRIO)
#elif IRQ_DMA_CHANNEL11_SR_CAT == IRQ_CAT23
ISR(DMACH11SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL11_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL11_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL11_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL11_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL11_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL11_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL11_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL11_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL11_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL11_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL11_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL11_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL11_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL11_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH12SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel12 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL12_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL12_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL12_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL12_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH12SR_ISR, 0, IRQ_DMA_CHANNEL12_SR_PRIO)
#elif IRQ_DMA_CHANNEL12_SR_CAT == IRQ_CAT23
ISR(DMACH12SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL12_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL12_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL12_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL12_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL12_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL12_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL12_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL12_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL12_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL12_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL12_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL12_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL12_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL12_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH13SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel13 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL13_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL13_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL13_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL13_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH13SR_ISR, 0, IRQ_DMA_CHANNEL13_SR_PRIO)
#elif IRQ_DMA_CHANNEL13_SR_CAT == IRQ_CAT23
ISR(DMACH13SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL13_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL13_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL13_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL13_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL13_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL13_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL13_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL13_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL13_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL13_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL13_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL13_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL13_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL13_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH14SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel14 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL14_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL14_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL14_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL14_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH14SR_ISR, 0, IRQ_DMA_CHANNEL14_SR_PRIO)
#elif IRQ_DMA_CHANNEL14_SR_CAT == IRQ_CAT23
ISR(DMACH14SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL14_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL14_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL14_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL14_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL14_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL14_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL14_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL14_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL14_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL14_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL14_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL14_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL14_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL14_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif
/******************************************************************************
** Syntax : void DMACH15SR_ISR(void)                                        **
**                                                                           **
** Service ID:       NA                                                      **
**                                                                           **
** Sync/Async:       Synchronous                                             **
**                                                                           **
** Reentrancy:       reentrant                                               **
**                                                                           **
** Parameters (in):  none                                                    **
**                                                                           **
** Parameters (out): none                                                    **
**                                                                           **
** Return value:     none                                                    **
**                                                                           **
** Description : Service for DMA Channel15 for service a node on Channel    **
**               interrupt,Transaction lost Transmit interrupt, Move engine  **
**               interrupts,Wrap buffer interrupts                           **
**                                                                           **
******************************************************************************/
#if((IRQ_DMA_CHANNEL15_SR_PRIO > 0U) || (IRQ_DMA_CHANNEL15_SR_CAT == IRQ_CAT23))
#if((IRQ_DMA_CHANNEL15_SR_PRIO > 0U) && (IRQ_DMA_CHANNEL15_SR_CAT == IRQ_CAT1))
IFX_INTERRUPT(DMACH15SR_ISR, 0, IRQ_DMA_CHANNEL15_SR_PRIO)
#elif IRQ_DMA_CHANNEL15_SR_CAT == IRQ_CAT23
ISR(DMACH15SR_ISR)
#endif
{
  #if ((IRQ_DMA_CHANNEL15_USED == IRQ_DMA_USED_MCALSPI_TX) || \
           (IRQ_DMA_CHANNEL15_USED == IRQ_DMA_USED_MCALSPI_RX))
  uint32 DmaErrorstatus;
  #endif
  /* Enable Global Interrupts */
#if (IRQ_DMA_CHANNEL15_SR_CAT == IRQ_CAT1)
  Mcal_EnableAllInterrupts();
#endif

  /* SPI TX interrupt */
  #if (IRQ_DMA_CHANNEL15_USED == IRQ_DMA_USED_MCALSPI_TX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL15_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiTx(IRQ_DMA_CHANNEL15_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL15_PARAM);
  }
  /* SPI RX interrupt */
  #elif (IRQ_DMA_CHANNEL15_USED == IRQ_DMA_USED_MCALSPI_RX)

  DmaErrorstatus  = Spi_IsrCheckDmaError(IRQ_DMA_CHANNEL15_PARAM);
  if (DmaErrorstatus == 0U)
  {
    Spi_IsrDmaQspiRx(IRQ_DMA_CHANNEL15_PARAM);
  }
  else
  {
    Spi_IsrDmaError(IRQ_DMA_CHANNEL15_PARAM);
  }
  /* ADC interrupt */
  #elif (IRQ_DMA_CHANNEL15_USED == IRQ_DMA_USED_MCALADC)

  #if (ADC_GROUP_EMUX_SCAN == STD_ON)
  Adc_IsrDmaSrn(IRQ_DMA_CHANNEL15_PARAM);
  #endif /*End of ADC_GROUP_EMUX_SCAN == STD_ON*/

  /* DMA CDD interrupt */
  #elif (IRQ_DMA_CHANNEL15_USED == IRQ_DMA_AVAILABLE)
  /* User Code Begins */
  /* Complex DMA driver usage */
  #endif
}
#endif

#endif/*end of IRQ_DMA_CH0TO15_EXIST*/

#define IRQ_STOP_SEC_CODE
#include "MemMap.h"