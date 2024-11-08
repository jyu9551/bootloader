
/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2014)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. Passing on and copying of this document, and communication   **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**   $FILENAME   : Irq_Cfg.h $                                                **
**                                                                            **
**   $CC VERSION : \main\dev_tc23x\13 $                                       **
**                                                                            **
**   DATE, TIME: 2024-08-13, 09:38:46                                         **
**                                                                            **
**   GENERATOR : Build b141014-0350                                           **
**                                                                            **
**   AUTHOR    : DL-AUTOSAR-Engineering                                       **
**                                                                            **
**   VENDOR    : Infineon Technologies                                        **
**                                                                            **
**   DESCRIPTION  : Irq configuration generated out of ECU configuration      **
**                 file                                                       **
**                                                                            **
**   MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                            **
*******************************************************************************/
#ifndef IRQ_CFG_H 
#define IRQ_CFG_H 


/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/

/************************* interrupt CATEGORY *********************************/
#define IRQ_CAT1                    (0)
#define IRQ_CAT23                   (1)
/************************* interrupt Type of Service **************************/
#define IRQ_TOS_CPU0                (0x0000)
#define IRQ_TOS_DMA                 (0x0800)
/*
                     Container : IrqGeneralConfiguration
*/

/*
Configuration: IRQ_OSEK_ENABLE 
- if TRUE, OSEK RTOS is used, 
  Both CAT1 and CAT23 interrupt categories can be configured 
  Any CAT23 interrupt priority should be greater than CAT1 interrupt priority
- if FALSE,OSEK RTOS is not used
  Only CAT1 interrupt category can be configured 
*/

#define IRQ_OSEK_ENABLE             (STD_OFF)


/*
                     Container : IrqAscLin interruptConfiguration
*/
/* ASC Lin Tx interrupt Category Setting*/
#define IRQ_ASCLIN0_TX_CAT          (IRQ_CAT1)
#define IRQ_ASCLIN1_TX_CAT          (IRQ_CAT1)

/* ASC Lin Tx interrupt Priority Setting*/
#define IRQ_ASCLIN0_TX_PRIO         0x0
#define IRQ_ASCLIN1_TX_PRIO         0x0

/* ASC Lin Tx interrupt type of service Setting*/
#define IRQ_ASCLIN0_TX_TOS          (IRQ_TOS_CPU0)
#define IRQ_ASCLIN1_TX_TOS          (IRQ_TOS_CPU0)

/* ASC Lin Rx interrupt Category Setting*/
#define IRQ_ASCLIN0_RX_CAT          (IRQ_CAT1)
#define IRQ_ASCLIN1_RX_CAT          (IRQ_CAT1)

/* ASC Lin Rx interrupt Priority Setting*/
#define IRQ_ASCLIN0_RX_PRIO         0x0
#define IRQ_ASCLIN1_RX_PRIO         0x0

/* ASC Lin Rx interrupt type of service Setting*/
#define IRQ_ASCLIN0_RX_TOS          (IRQ_TOS_CPU0)
#define IRQ_ASCLIN1_RX_TOS          (IRQ_TOS_CPU0)

/* ASC Lin Err interrupt Category Setting*/
#define IRQ_ASCLIN0_ERR_CAT         (IRQ_CAT1)
#define IRQ_ASCLIN1_ERR_CAT         (IRQ_CAT1)

/* ASC Lin Err interrupt Priority Setting*/
#define IRQ_ASCLIN0_ERR_PRIO        0x0
#define IRQ_ASCLIN1_ERR_PRIO        0x0

/* ASC Lin Err interrupt type of service Setting*/
#define IRQ_ASCLIN0_ERR_TOS         (IRQ_TOS_CPU0)
#define IRQ_ASCLIN1_ERR_TOS         (IRQ_TOS_CPU0)

/*
                     Container : IrqQspi interruptConfiguration
*/
/* Qspi Tx interrupt Category Setting*/
#define IRQ_QSPI0_TX_CAT            (IRQ_CAT1)
#define IRQ_QSPI1_TX_CAT            (IRQ_CAT1)
#define IRQ_QSPI2_TX_CAT            (IRQ_CAT1)
#define IRQ_QSPI3_TX_CAT            (IRQ_CAT1)

/* Qspi Tx interrupt Priority Setting*/
#define IRQ_QSPI0_TX_PRIO           0x0
#define IRQ_QSPI1_TX_PRIO           0x0
#define IRQ_QSPI2_TX_PRIO           0x0
#define IRQ_QSPI3_TX_PRIO           0x0

/* Qspi Tx interrupt type of service Setting*/
#define IRQ_QSPI0_TX_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI1_TX_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI2_TX_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI3_TX_TOS            (IRQ_TOS_CPU0)

/* Qspi Rx interrupt Category Setting*/
#define IRQ_QSPI0_RX_CAT            (IRQ_CAT1)
#define IRQ_QSPI1_RX_CAT            (IRQ_CAT1)
#define IRQ_QSPI2_RX_CAT            (IRQ_CAT1)
#define IRQ_QSPI3_RX_CAT            (IRQ_CAT1)

/* Qspi Rx interrupt Priority Setting*/
#define IRQ_QSPI0_RX_PRIO           0x0
#define IRQ_QSPI1_RX_PRIO           0x0
#define IRQ_QSPI2_RX_PRIO           0x0
#define IRQ_QSPI3_RX_PRIO           0x0

/* Qspi Rx interrupt type of service Setting*/
#define IRQ_QSPI0_RX_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI1_RX_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI2_RX_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI3_RX_TOS            (IRQ_TOS_CPU0)

/* Qspi Err interrupt Category Setting*/
#define IRQ_QSPI0_ERR_CAT           (IRQ_CAT1)
#define IRQ_QSPI1_ERR_CAT           (IRQ_CAT1)
#define IRQ_QSPI2_ERR_CAT           (IRQ_CAT1)
#define IRQ_QSPI3_ERR_CAT           (IRQ_CAT1)

/* Qspi Err interrupt Priority Setting*/
#define IRQ_QSPI0_ERR_PRIO          0x0
#define IRQ_QSPI1_ERR_PRIO          0x0
#define IRQ_QSPI2_ERR_PRIO          0x0
#define IRQ_QSPI3_ERR_PRIO          0x0

/* Qspi Err interrupt type of service Setting*/
#define IRQ_QSPI0_ERR_TOS           (IRQ_TOS_CPU0)
#define IRQ_QSPI1_ERR_TOS           (IRQ_TOS_CPU0)
#define IRQ_QSPI2_ERR_TOS           (IRQ_TOS_CPU0)
#define IRQ_QSPI3_ERR_TOS           (IRQ_TOS_CPU0)

/* Qspi PT interrupt Category Setting*/
#define IRQ_QSPI0_PT_CAT            (IRQ_CAT1)
#define IRQ_QSPI1_PT_CAT            (IRQ_CAT1)
#define IRQ_QSPI2_PT_CAT            (IRQ_CAT1)
#define IRQ_QSPI3_PT_CAT            (IRQ_CAT1)

/* Qspi PT interrupt Priority Setting*/
#define IRQ_QSPI0_PT_PRIO           0x0
#define IRQ_QSPI1_PT_PRIO           0x0
#define IRQ_QSPI2_PT_PRIO           0x0
#define IRQ_QSPI3_PT_PRIO           0x0

/* Qspi PT interrupt type of service Setting*/
#define IRQ_QSPI0_PT_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI1_PT_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI2_PT_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI3_PT_TOS            (IRQ_TOS_CPU0)

/* Qspi HC interrupt Category Setting*/
#define IRQ_QSPI2_HC_CAT            (IRQ_CAT1)
#define IRQ_QSPI3_HC_CAT            (IRQ_CAT1)

/* Qspi HC interrupt Priority Setting*/
#define IRQ_QSPI2_HC_PRIO           0x0
#define IRQ_QSPI3_HC_PRIO           0x0

/* Qspi HC interrupt type of service Setting*/
#define IRQ_QSPI2_HC_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI3_HC_TOS            (IRQ_TOS_CPU0)

/* Qspi UD interrupt Category Setting*/
#define IRQ_QSPI0_UD_CAT            (IRQ_CAT1)
#define IRQ_QSPI1_UD_CAT            (IRQ_CAT1)
#define IRQ_QSPI2_UD_CAT            (IRQ_CAT1)
#define IRQ_QSPI3_UD_CAT            (IRQ_CAT1)

/* Qspi UD interrupt Priority Setting*/
#define IRQ_QSPI0_UD_PRIO           0x0
#define IRQ_QSPI1_UD_PRIO           0x0
#define IRQ_QSPI2_UD_PRIO           0x0
#define IRQ_QSPI3_UD_PRIO           0x0

/* Qspi UD interrupt type of service Setting*/
#define IRQ_QSPI0_UD_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI1_UD_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI2_UD_TOS            (IRQ_TOS_CPU0)
#define IRQ_QSPI3_UD_TOS            (IRQ_TOS_CPU0)



/*
                     Container : IrqCCU6 interruptConfiguration
*/
/*CCU60 interrupt Category setting */
#define IRQ_CCU60_SR0_CAT           (IRQ_CAT1)
#define IRQ_CCU60_SR1_CAT           (IRQ_CAT1)
#define IRQ_CCU60_SR2_CAT           (IRQ_CAT1)
#define IRQ_CCU60_SR3_CAT           (IRQ_CAT1)
                                                                                                           
/*CCU60 interrupt Priority setting */ 
#define IRQ_CCU60_SR0_PRIO          0x0
#define IRQ_CCU60_SR1_PRIO          0x0
#define IRQ_CCU60_SR2_PRIO          0x0
#define IRQ_CCU60_SR3_PRIO          0x0

/*CCU60 interrupt type of service setting */
#define IRQ_CCU60_SR0_TOS           (IRQ_TOS_CPU0)
#define IRQ_CCU60_SR1_TOS           (IRQ_TOS_CPU0)
#define IRQ_CCU60_SR2_TOS           (IRQ_TOS_CPU0)
#define IRQ_CCU60_SR3_TOS           (IRQ_TOS_CPU0)

/*CCU61 interrupt Category setting */
#define IRQ_CCU61_SR0_CAT           (IRQ_CAT1)
#define IRQ_CCU61_SR1_CAT           (IRQ_CAT1)
#define IRQ_CCU61_SR2_CAT           (IRQ_CAT1)
#define IRQ_CCU61_SR3_CAT           (IRQ_CAT1)
                                                                                                           
/*CCU61 interrupt Priority setting */ 
#define IRQ_CCU61_SR0_PRIO          0x0
#define IRQ_CCU61_SR1_PRIO          0x0
#define IRQ_CCU61_SR2_PRIO          0x0
#define IRQ_CCU61_SR3_PRIO          0x0

/*CCU61 interrupt type of service setting */
#define IRQ_CCU61_SR0_TOS           (IRQ_TOS_CPU0)
#define IRQ_CCU61_SR1_TOS           (IRQ_TOS_CPU0)
#define IRQ_CCU61_SR2_TOS           (IRQ_TOS_CPU0)
#define IRQ_CCU61_SR3_TOS           (IRQ_TOS_CPU0)

/*
                     Container : IrqGPTinterruptConfiguration
*/
/* GPT120 interrupt Category setting */
#define IRQ_GPT120_CARPEL_CAT       (IRQ_CAT1)
#define IRQ_GPT120_T2_CAT           (IRQ_CAT1)
#define IRQ_GPT120_T3_CAT           (IRQ_CAT1)
#define IRQ_GPT120_T4_CAT           (IRQ_CAT1)
#define IRQ_GPT120_T5_CAT           (IRQ_CAT1)
#define IRQ_GPT120_T6_CAT           (IRQ_CAT1)

/*  GPT120 interrupt Priority setting  */
#define IRQ_GPT120_CARPEL_PRIO      0x0
#define IRQ_GPT120_T2_PRIO          0xf
#define IRQ_GPT120_T3_PRIO          0x0
#define IRQ_GPT120_T4_PRIO          0x0
#define IRQ_GPT120_T5_PRIO          0x0
#define IRQ_GPT120_T6_PRIO          0x0

/* GPT120 interrupt type of service setting */
#define IRQ_GPT120_CARPEL_TOS       (IRQ_TOS_CPU0)
#define IRQ_GPT120_T2_TOS           (IRQ_TOS_CPU0)
#define IRQ_GPT120_T3_TOS           (IRQ_TOS_CPU0)
#define IRQ_GPT120_T4_TOS           (IRQ_TOS_CPU0)
#define IRQ_GPT120_T5_TOS           (IRQ_TOS_CPU0)
#define IRQ_GPT120_T6_TOS           (IRQ_TOS_CPU0)


/*
                     Container : IrqSTMinterruptConfiguration
*/
/* STM0 interrupt Category Setting */ 
#define IRQ_STM0_SR0_CAT            (IRQ_CAT1)
#define IRQ_STM0_SR1_CAT            (IRQ_CAT1)
                                                                                                           
/* STM0 interrupt Priority Setting */  
#define IRQ_STM0_SR0_PRIO           0x0
#define IRQ_STM0_SR1_PRIO           0x0

/* STM0 interrupt type of service Setting */ 
#define IRQ_STM0_SR0_TOS            (IRQ_TOS_CPU0)
#define IRQ_STM0_SR1_TOS            (IRQ_TOS_CPU0)



/*
                     Container : IrqDMAinterruptConfiguration
*/
/* DMA interrupt Category settings */
#define IRQ_DMA_ERR_SR_CAT           (IRQ_CAT1)
#define IRQ_DMA_CHANNEL0_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL1_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL2_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL3_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL4_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL5_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL6_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL7_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL8_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL9_SR_CAT      (IRQ_CAT1)
#define IRQ_DMA_CHANNEL10_SR_CAT     (IRQ_CAT1)
#define IRQ_DMA_CHANNEL11_SR_CAT     (IRQ_CAT1)
#define IRQ_DMA_CHANNEL12_SR_CAT     (IRQ_CAT1)
#define IRQ_DMA_CHANNEL13_SR_CAT     (IRQ_CAT1)
#define IRQ_DMA_CHANNEL14_SR_CAT     (IRQ_CAT1)
#define IRQ_DMA_CHANNEL15_SR_CAT     (IRQ_CAT1)

/* DMA interrupt Priority settings */
#define IRQ_DMA_ERR_SR_PRIO          0x0
#define IRQ_DMA_CHANNEL0_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL1_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL2_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL3_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL4_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL5_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL6_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL7_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL8_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL9_SR_PRIO     0x0
#define IRQ_DMA_CHANNEL10_SR_PRIO    0x0
#define IRQ_DMA_CHANNEL11_SR_PRIO    0x0
#define IRQ_DMA_CHANNEL12_SR_PRIO    0x0
#define IRQ_DMA_CHANNEL13_SR_PRIO    0x0
#define IRQ_DMA_CHANNEL14_SR_PRIO    0x0
#define IRQ_DMA_CHANNEL15_SR_PRIO    0x0

/* DMA interrupt type of service settings */
#define IRQ_DMA_ERR_SR_TOS           (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL0_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL1_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL2_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL3_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL4_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL5_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL6_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL7_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL8_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL9_SR_TOS      (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL10_SR_TOS     (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL11_SR_TOS     (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL12_SR_TOS     (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL13_SR_TOS     (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL14_SR_TOS     (IRQ_TOS_CPU0)
#define IRQ_DMA_CHANNEL15_SR_TOS     (IRQ_TOS_CPU0)




/* Macro for Dma_Irq.c*/
#define IRQ_DMA_CHANNEL0_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL0_PARAM       (255U)
#define IRQ_DMA_CHANNEL1_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL1_PARAM       (255U)
#define IRQ_DMA_CHANNEL2_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL2_PARAM       (255U)
#define IRQ_DMA_CHANNEL3_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL3_PARAM       (255U)
#define IRQ_DMA_CHANNEL4_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL4_PARAM       (255U)
#define IRQ_DMA_CHANNEL5_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL5_PARAM       (255U)
#define IRQ_DMA_CHANNEL6_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL6_PARAM       (255U)
#define IRQ_DMA_CHANNEL7_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL7_PARAM       (255U)
#define IRQ_DMA_CHANNEL8_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL8_PARAM       (255U)
#define IRQ_DMA_CHANNEL9_USED        (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL9_PARAM       (255U)
#define IRQ_DMA_CHANNEL10_USED       (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL10_PARAM      (255U)
#define IRQ_DMA_CHANNEL11_USED       (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL11_PARAM      (255U)
#define IRQ_DMA_CHANNEL12_USED       (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL12_PARAM      (255U)
#define IRQ_DMA_CHANNEL13_USED       (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL13_PARAM      (255U)
#define IRQ_DMA_CHANNEL14_USED       (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL14_PARAM      (255U)
#define IRQ_DMA_CHANNEL15_USED       (IRQ_DMA_AVAILABLE)
#define IRQ_DMA_CHANNEL15_PARAM      (255U)

/* Macro for AscLin_Irq.c*/
#define IRQ_ASCLIN_CHANNEL0_USED      (IRQ_ASCLIN_AVAILABLE)
#define IRQ_ASCLIN_CHANNEL1_USED      (IRQ_ASCLIN_AVAILABLE)



/*
                     Container : Ethernet interrupt Configuration
*/
/* Ethernet interrupt Category Setting*/
#define IRQ_ETH_SR_CAT              (IRQ_CAT1)

/* Ethernet interrupt Category Setting*/
#define IRQ_ETH_SR_PRIO             0x0

/* Ethernet interrupt type of service Setting*/
#define IRQ_ETH_SR_TOS              (IRQ_TOS_CPU0)

/*
                     Container : IrqCaninterruptConfiguration
*/
/* CAN interrupt Category setting */
#define IRQ_CAN_SR0_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR1_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR2_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR3_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR4_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR5_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR6_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR7_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR8_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR9_CAT             (IRQ_CAT1)
#define IRQ_CAN_SR10_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR11_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR12_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR13_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR14_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR15_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR16_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR17_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR18_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR19_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR20_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR21_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR22_CAT            (IRQ_CAT1)
#define IRQ_CAN_SR23_CAT            (IRQ_CAT1)

/*  CAN interrupt Priority setting  */
#define IRQ_CAN_SR0_PRIO            0x6e
#define IRQ_CAN_SR1_PRIO            0x6d
#define IRQ_CAN_SR2_PRIO            0x6c
#define IRQ_CAN_SR3_PRIO            0x6b
#define IRQ_CAN_SR4_PRIO            0x6a
#define IRQ_CAN_SR5_PRIO            0x69
#define IRQ_CAN_SR6_PRIO            0x68
#define IRQ_CAN_SR7_PRIO            0x67
#define IRQ_CAN_SR8_PRIO            0x66
#define IRQ_CAN_SR9_PRIO            0x65
#define IRQ_CAN_SR10_PRIO           0x64
#define IRQ_CAN_SR11_PRIO           0x63
#define IRQ_CAN_SR12_PRIO           0x0
#define IRQ_CAN_SR13_PRIO           0x0
#define IRQ_CAN_SR14_PRIO           0x0
#define IRQ_CAN_SR15_PRIO           0x0
#define IRQ_CAN_SR16_PRIO           0x0
#define IRQ_CAN_SR17_PRIO           0x0
#define IRQ_CAN_SR18_PRIO           0x0
#define IRQ_CAN_SR19_PRIO           0x0
#define IRQ_CAN_SR20_PRIO           0x0
#define IRQ_CAN_SR21_PRIO           0x0
#define IRQ_CAN_SR22_PRIO           0x0
#define IRQ_CAN_SR23_PRIO           0x0


/* CAN interrupt type of service setting */
#define IRQ_CAN_SR0_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR1_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR2_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR3_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR4_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR5_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR6_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR7_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR8_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR9_TOS             (IRQ_TOS_CPU0)
#define IRQ_CAN_SR10_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR11_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR12_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR13_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR14_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR15_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR16_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR17_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR18_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR19_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR20_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR21_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR22_TOS            (IRQ_TOS_CPU0)
#define IRQ_CAN_SR23_TOS            (IRQ_TOS_CPU0)

/*
                     Container : IrqHSMinterruptConfiguration
*/
/* HSM interrupt Category setting */
#define IRQ_HSM_SR0_CAT             (IRQ_CAT1)
#define IRQ_HSM_SR1_CAT             (IRQ_CAT1)

/*  HSM interrupt Priority setting  */
#define IRQ_HSM_SR0_PRIO            0x1
#define IRQ_HSM_SR1_PRIO            0x10


/* HSM interrupt type of service setting */
#define IRQ_HSM_SR0_TOS             (IRQ_TOS_CPU0)
#define IRQ_HSM_SR1_TOS             (IRQ_TOS_CPU0)



/*
                     Container : Irq ADC configuration
*/
/*ADC0 interrupt Category setting */
#define IRQ_ADC0_SR0_CAT            (IRQ_CAT1)
#define IRQ_ADC0_SR1_CAT            (IRQ_CAT1)
#define IRQ_ADC0_SR2_CAT            (IRQ_CAT1)
#define IRQ_ADC0_SR3_CAT            (IRQ_CAT1)

/*ADC0 interrupt Priority setting */
#define IRQ_ADC0_SR0_PRIO           0xd6
#define IRQ_ADC0_SR1_PRIO           0x3d
#define IRQ_ADC0_SR2_PRIO           0x3e
#define IRQ_ADC0_SR3_PRIO           0x3f

/*ADC0 interrupt type of service setting */
#define IRQ_ADC0_SR0_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC0_SR1_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC0_SR2_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC0_SR3_TOS            (IRQ_TOS_CPU0)

/*ADC1 interrupt Category setting */
#define IRQ_ADC1_SR0_CAT            (IRQ_CAT1)
#define IRQ_ADC1_SR1_CAT            (IRQ_CAT1)
#define IRQ_ADC1_SR2_CAT            (IRQ_CAT1)
#define IRQ_ADC1_SR3_CAT            (IRQ_CAT1)

/*ADC1 interrupt Priority setting */
#define IRQ_ADC1_SR0_PRIO           0x59
#define IRQ_ADC1_SR1_PRIO           0x5a
#define IRQ_ADC1_SR2_PRIO           0x5b
#define IRQ_ADC1_SR3_PRIO           0x5c

/*ADC1 interrupt type of service setting */
#define IRQ_ADC1_SR0_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC1_SR1_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC1_SR2_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC1_SR3_TOS            (IRQ_TOS_CPU0)

/*ADC2 interrupt Category setting */
#define IRQ_ADC2_SR0_CAT            (IRQ_CAT1)
#define IRQ_ADC2_SR1_CAT            (IRQ_CAT1)
#define IRQ_ADC2_SR2_CAT            (IRQ_CAT1)
#define IRQ_ADC2_SR3_CAT            (IRQ_CAT1)

/*ADC2 interrupt Priority setting */
#define IRQ_ADC2_SR0_PRIO           0x0
#define IRQ_ADC2_SR1_PRIO           0x0
#define IRQ_ADC2_SR2_PRIO           0x0
#define IRQ_ADC2_SR3_PRIO           0x0

/*ADC2 interrupt type of service setting */
#define IRQ_ADC2_SR0_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC2_SR1_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC2_SR2_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC2_SR3_TOS            (IRQ_TOS_CPU0)

/*ADC3 interrupt Category setting */
#define IRQ_ADC3_SR0_CAT            (IRQ_CAT1)
#define IRQ_ADC3_SR1_CAT            (IRQ_CAT1)
#define IRQ_ADC3_SR2_CAT            (IRQ_CAT1)
#define IRQ_ADC3_SR3_CAT            (IRQ_CAT1)

/*ADC3 interrupt Priority setting */
#define IRQ_ADC3_SR0_PRIO           0x0
#define IRQ_ADC3_SR1_PRIO           0x0
#define IRQ_ADC3_SR2_PRIO           0x0
#define IRQ_ADC3_SR3_PRIO           0x0

/*ADC3 interrupt type of service setting */
#define IRQ_ADC3_SR0_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC3_SR1_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC3_SR2_TOS            (IRQ_TOS_CPU0)
#define IRQ_ADC3_SR3_TOS            (IRQ_TOS_CPU0)








/*ADC CG0 interrupt Category setting */
#define IRQ_ADCCG0_SR0_CAT          (IRQ_CAT1)
#define IRQ_ADCCG0_SR1_CAT          (IRQ_CAT1)
#define IRQ_ADCCG0_SR2_CAT          (IRQ_CAT1)
#define IRQ_ADCCG0_SR3_CAT          (IRQ_CAT1)

/*ADC CG0 interrupt Priority setting */
#define IRQ_ADCCG0_SR0_PRIO         0x28
#define IRQ_ADCCG0_SR1_PRIO         0x29
#define IRQ_ADCCG0_SR2_PRIO         0x2a
#define IRQ_ADCCG0_SR3_PRIO         0x2b

/*ADC CG0 interrupt type of service setting */
#define IRQ_ADCCG0_SR0_TOS          (IRQ_TOS_CPU0)
#define IRQ_ADCCG0_SR1_TOS          (IRQ_TOS_CPU0)
#define IRQ_ADCCG0_SR2_TOS          (IRQ_TOS_CPU0)
#define IRQ_ADCCG0_SR3_TOS          (IRQ_TOS_CPU0)




/*
                     Container : IrqFLEXRAYinterruptConfiguration
*/
/* FLEXRAY0 interrupt Category Setting */
#define IRQ_FLEXRAY0_SR0_CAT         (IRQ_CAT1)
#define IRQ_FLEXRAY0_SR1_CAT         (IRQ_CAT1)
#define IRQ_FLEXRAY0_TIMER_INT0_CAT  (IRQ_CAT1)
#define IRQ_FLEXRAY0_TIMER_INT1_CAT  (IRQ_CAT1)
#define IRQ_FLEXRAY0_NEW_DATA0_CAT   (IRQ_CAT1)
#define IRQ_FLEXRAY0_NEW_DATA1_CAT   (IRQ_CAT1)
#define IRQ_FLEXRAY0_MBSC0_CAT       (IRQ_CAT1)
#define IRQ_FLEXRAY0_MBSC1_CAT       (IRQ_CAT1)
#define IRQ_FLEXRAY0_OB_BUSY_CAT     (IRQ_CAT1)
#define IRQ_FLEXRAY0_IB_BUSY_CAT     (IRQ_CAT1)

/* FLEXRAY0 interrupt Priority Setting */
#define IRQ_FLEXRAY0_SR0_PRIO        0x0
#define IRQ_FLEXRAY0_SR1_PRIO        0x0
#define IRQ_FLEXRAY0_TIMER_INT0_PRIO 0x0
#define IRQ_FLEXRAY0_TIMER_INT1_PRIO 0x0
#define IRQ_FLEXRAY0_NEW_DATA0_PRIO  0x0
#define IRQ_FLEXRAY0_NEW_DATA1_PRIO  0x0
#define IRQ_FLEXRAY0_MBSC0_PRIO      0x0
#define IRQ_FLEXRAY0_MBSC1_PRIO      0x0
#define IRQ_FLEXRAY0_OB_BUSY_PRIO    0x0
#define IRQ_FLEXRAY0_IB_BUSY_PRIO    0x0

/* FLEXRAY0 interrupt type of service Setting */
#define IRQ_FLEXRAY0_SR0_TOS         (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_SR1_TOS         (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_TIMER_INT0_TOS  (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_TIMER_INT1_TOS  (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_NEW_DATA0_TOS   (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_NEW_DATA1_TOS   (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_MBSC0_TOS       (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_MBSC1_TOS       (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_OB_BUSY_TOS     (IRQ_TOS_CPU0)
#define IRQ_FLEXRAY0_IB_BUSY_TOS     (IRQ_TOS_CPU0)


/*
                     Container : PMU0 interrupt Configuration
*/
/* PMU0 interrupt Category Setting*/
#define IRQ_PMU0_SR0_CAT            (IRQ_CAT1)
#define IRQ_PMU0_SR1_CAT            (IRQ_CAT1)

/* PMU0 interrupt Category Setting*/
#define IRQ_PMU0_SR0_PRIO           0x0
#define IRQ_PMU0_SR1_PRIO           0x0

/* PMU0 interrupt type of service Setting*/
#define IRQ_PMU0_SR0_TOS            (IRQ_TOS_CPU0)
#define IRQ_PMU0_SR1_TOS            (IRQ_TOS_CPU0)

/*
                     Container : Scu interrupt Configuration
*/
/* Scu interrupt Category Setting*/
#define IRQ_SCU_DTS_BUSY_SR_CAT     (IRQ_CAT1)
#define IRQ_SCU_ERU_SR0_CAT         (IRQ_CAT1)
#define IRQ_SCU_ERU_SR1_CAT         (IRQ_CAT1)
#define IRQ_SCU_ERU_SR2_CAT         (IRQ_CAT1)
#define IRQ_SCU_ERU_SR3_CAT         (IRQ_CAT1)

/* Scu interrupt Category Setting*/
#define IRQ_SCU_DTS_BUSY_SR_PRIO    0x0
#define IRQ_SCU_ERU_SR0_PRIO        0x0
#define IRQ_SCU_ERU_SR1_PRIO        0x0
#define IRQ_SCU_ERU_SR2_PRIO        0x0
#define IRQ_SCU_ERU_SR3_PRIO        0x0

/* Scu interrupt type of service Setting*/
#define IRQ_SCU_DTS_BUSY_SR_TOS     (IRQ_TOS_CPU0)
#define IRQ_SCU_ERU_SR0_TOS         (IRQ_TOS_CPU0)
#define IRQ_SCU_ERU_SR1_TOS         (IRQ_TOS_CPU0)
#define IRQ_SCU_ERU_SR2_TOS         (IRQ_TOS_CPU0)
#define IRQ_SCU_ERU_SR3_TOS         (IRQ_TOS_CPU0)

/*
                     Container : IrqGPSRGroupinterruptConfiguration
*/
/* GPSRGROUP0 interrupt Category Setting */ 
#define IRQ_GPSRGROUP0_SR0_CAT      (IRQ_CAT1)
#define IRQ_GPSRGROUP0_SR1_CAT      (IRQ_CAT1)
#define IRQ_GPSRGROUP0_SR2_CAT      (IRQ_CAT1)
#define IRQ_GPSRGROUP0_SR3_CAT      (IRQ_CAT1)
                                                                                                           
/* GPSRGROUP0 interrupt Priority Setting */  
#define IRQ_GPSRGROUP0_SR0_PRIO     0x0
#define IRQ_GPSRGROUP0_SR1_PRIO     0x0
#define IRQ_GPSRGROUP0_SR2_PRIO     0x0
#define IRQ_GPSRGROUP0_SR3_PRIO     0x0

/* GPSRGROUP0 interrupt type of service Setting */ 
#define IRQ_GPSRGROUP0_SR0_TOS      (IRQ_TOS_CPU0)
#define IRQ_GPSRGROUP0_SR1_TOS      (IRQ_TOS_CPU0)
#define IRQ_GPSRGROUP0_SR2_TOS      (IRQ_TOS_CPU0)
#define IRQ_GPSRGROUP0_SR3_TOS      (IRQ_TOS_CPU0)


/*
                     Container : Irq GTM interruptConfiguration
*/
/* GTM AEI interrupt Category Setting */ 
#define IRQ_GTM_AEI_CAT             (IRQ_CAT1)
                                                                                                           
/* GTM AEI interrupt Priority Setting */  
#define IRQ_GTM_AEI_PRIO            0x0

/* GTM AEI interrupt type of service Setting */ 
#define IRQ_GTM_AEI_TOS             (IRQ_TOS_CPU0)

/* GTM ERR interrupt Category Setting */ 
#define IRQ_GTM_ERR_SR_CAT          (IRQ_CAT1)
                                                                                                           
/* GTM ERR interrupt Priority Setting */  
#define IRQ_GTM_ERR_SR_PRIO         0x0

/* GTM ERR interrupt type of service Setting */ 
#define IRQ_GTM_ERR_SR_TOS          (IRQ_TOS_CPU0)

/* GTM TIM0 interrupt Category Setting */ 
#define IRQ_GTM_TIM0_SR0_CAT        (IRQ_CAT1)
#define IRQ_GTM_TIM0_SR1_CAT        (IRQ_CAT1)
#define IRQ_GTM_TIM0_SR2_CAT        (IRQ_CAT1)
#define IRQ_GTM_TIM0_SR3_CAT        (IRQ_CAT1)
#define IRQ_GTM_TIM0_SR4_CAT        (IRQ_CAT1)
#define IRQ_GTM_TIM0_SR5_CAT        (IRQ_CAT1)
#define IRQ_GTM_TIM0_SR6_CAT        (IRQ_CAT1)
#define IRQ_GTM_TIM0_SR7_CAT        (IRQ_CAT1)
                                                                                                           
/* GTM TIM0 interrupt Priority Setting */  
#define IRQ_GTM_TIM0_SR0_PRIO       0x0
#define IRQ_GTM_TIM0_SR1_PRIO       0x0
#define IRQ_GTM_TIM0_SR2_PRIO       0x0
#define IRQ_GTM_TIM0_SR3_PRIO       0x0
#define IRQ_GTM_TIM0_SR4_PRIO       0x0
#define IRQ_GTM_TIM0_SR5_PRIO       0x0
#define IRQ_GTM_TIM0_SR6_PRIO       0x0
#define IRQ_GTM_TIM0_SR7_PRIO       0x0

/* GTM TIM0 interrupt type of service Setting */ 
#define IRQ_GTM_TIM0_SR0_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TIM0_SR1_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TIM0_SR2_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TIM0_SR3_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TIM0_SR4_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TIM0_SR5_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TIM0_SR6_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TIM0_SR7_TOS        (IRQ_TOS_CPU0)

/* GTM TOM0 interrupt Category Setting */ 
#define IRQ_GTM_TOM0_SR0_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM0_SR1_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM0_SR2_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM0_SR3_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM0_SR4_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM0_SR5_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM0_SR6_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM0_SR7_CAT        (IRQ_CAT1)
                                                                                                           
/* GTM TOM0 interrupt Priority Setting */  
#define IRQ_GTM_TOM0_SR0_PRIO       0x13
#define IRQ_GTM_TOM0_SR1_PRIO       0x14
#define IRQ_GTM_TOM0_SR2_PRIO       0x15
#define IRQ_GTM_TOM0_SR3_PRIO       0x16
#define IRQ_GTM_TOM0_SR4_PRIO       0x0
#define IRQ_GTM_TOM0_SR5_PRIO       0x0
#define IRQ_GTM_TOM0_SR6_PRIO       0x0
#define IRQ_GTM_TOM0_SR7_PRIO       0x0

/* GTM TOM0 interrupt type of service Setting */ 
#define IRQ_GTM_TOM0_SR0_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM0_SR1_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM0_SR2_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM0_SR3_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM0_SR4_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM0_SR5_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM0_SR6_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM0_SR7_TOS        (IRQ_TOS_CPU0)

/* GTM TOM1 interrupt Category Setting */ 
#define IRQ_GTM_TOM1_SR0_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM1_SR1_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM1_SR2_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM1_SR3_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM1_SR4_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM1_SR5_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM1_SR6_CAT        (IRQ_CAT1)
#define IRQ_GTM_TOM1_SR7_CAT        (IRQ_CAT1)
                                                                                                           
/* GTM TOM1 interrupt Priority Setting */  
#define IRQ_GTM_TOM1_SR0_PRIO       0x17
#define IRQ_GTM_TOM1_SR1_PRIO       0x1b
#define IRQ_GTM_TOM1_SR2_PRIO       0x18
#define IRQ_GTM_TOM1_SR3_PRIO       0x19
#define IRQ_GTM_TOM1_SR4_PRIO       0x0
#define IRQ_GTM_TOM1_SR5_PRIO       0x0
#define IRQ_GTM_TOM1_SR6_PRIO       0x1a
#define IRQ_GTM_TOM1_SR7_PRIO       0x0

/* GTM TOM1 interrupt type of service Setting */ 
#define IRQ_GTM_TOM1_SR0_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM1_SR1_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM1_SR2_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM1_SR3_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM1_SR4_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM1_SR5_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM1_SR6_TOS        (IRQ_TOS_CPU0)
#define IRQ_GTM_TOM1_SR7_TOS        (IRQ_TOS_CPU0)

/*
                     Container : IrqSentInterruptConfiguration
*/
/* SENT Interrupt Category setting */
#define IRQ_SENT_SR0_CAT            (IRQ_CAT1)
#define IRQ_SENT_SR1_CAT            (IRQ_CAT1)
#define IRQ_SENT_SR2_CAT            (IRQ_CAT1)
#define IRQ_SENT_SR3_CAT            (IRQ_CAT1)

/*  SENT Interrupt Priority setting  */
#define IRQ_SENT_SR0_PRIO           0x0
#define IRQ_SENT_SR1_PRIO           0x0
#define IRQ_SENT_SR2_PRIO           0x0
#define IRQ_SENT_SR3_PRIO           0x0


/* SENT Interrupt type of service setting */
#define IRQ_SENT_SR0_TOS            (IRQ_TOS_CPU0)
#define IRQ_SENT_SR1_TOS            (IRQ_TOS_CPU0)
#define IRQ_SENT_SR2_TOS            (IRQ_TOS_CPU0)
#define IRQ_SENT_SR3_TOS            (IRQ_TOS_CPU0)


/* Global macros that determines whether a module / SRN is in use or not
*/
#define IRQ_ASCLIN_EXIST            (STD_OFF)
#define IRQ_ASCLIN0_EXIST           (STD_OFF)
#define IRQ_ASCLIN1_EXIST           (STD_OFF)
#define IRQ_ASCLIN2_EXIST           (STD_OFF)
#define IRQ_ASCLIN3_EXIST           (STD_OFF)

#define IRQ_QSPI_EXIST              (STD_ON)
#define IRQ_QSPI0_EXIST             (STD_ON)
#define IRQ_QSPI1_EXIST             (STD_ON)
#define IRQ_QSPI2_EXIST             (STD_ON)
#define IRQ_QSPI3_EXIST             (STD_ON)
#define IRQ_QSPI4_EXIST             (STD_OFF)
#define IRQ_QSPI5_EXIST             (STD_OFF)



#define IRQ_CCU6_EXIST              (STD_OFF)
#define IRQ_CCU60_EXIST             (STD_OFF)
#define IRQ_CCU61_EXIST             (STD_OFF)

#define IRQ_GPT_EXIST               (STD_ON)
#define IRQ_GPT120_EXIST            (STD_ON)

#define IRQ_STM_EXIST               (STD_ON)
#define IRQ_STM0_EXIST              (STD_ON)
#define IRQ_STM1_EXIST              (STD_OFF)
#define IRQ_STM2_EXIST              (STD_OFF)

#define IRQ_DMA_EXIST               (STD_ON)
#define IRQ_DMA_CH0TO15_EXIST       (STD_ON)

#define IRQ_ETH_EXIST               (STD_OFF)

#define IRQ_CAN_EXIST               (STD_ON)
#define IRQ_CAN0_EXIST              (STD_ON)
#define IRQ_CAN1_EXIST              (STD_ON)
#define IRQ_CAN2_EXIST              (STD_ON)
#define IRQ_CAN3_EXIST              (STD_ON)
#define IRQ_CAN4_EXIST              (STD_ON)
#define IRQ_CAN5_EXIST              (STD_ON)
#define IRQ_CAN6_EXIST              (STD_ON)
#define IRQ_CAN7_EXIST              (STD_ON)
#define IRQ_CAN8_EXIST              (STD_ON)
#define IRQ_CAN9_EXIST              (STD_ON)
#define IRQ_CAN10_EXIST             (STD_ON)
#define IRQ_CAN11_EXIST             (STD_ON)
#define IRQ_CAN12_EXIST             (STD_ON)
#define IRQ_CAN13_EXIST             (STD_ON)
#define IRQ_CAN14_EXIST             (STD_ON)
#define IRQ_CAN15_EXIST             (STD_ON)
#define IRQ_CAN16_EXIST             (STD_ON)
#define IRQ_CAN17_EXIST             (STD_ON)
#define IRQ_CAN18_EXIST             (STD_ON)
#define IRQ_CAN19_EXIST             (STD_ON)
#define IRQ_CAN20_EXIST             (STD_ON)
#define IRQ_CAN21_EXIST             (STD_ON)
#define IRQ_CAN22_EXIST             (STD_ON)
#define IRQ_CAN23_EXIST             (STD_ON)


#define IRQ_HSM0_EXIST              (STD_ON)
#define IRQ_HSM1_EXIST              (STD_ON)

#define IRQ_ADC_EXIST               (STD_ON)
#define IRQ_ADC0_EXIST              (STD_ON)
#define IRQ_ADC1_EXIST              (STD_ON)
#define IRQ_ADC2_EXIST              (STD_ON)
#define IRQ_ADC3_EXIST              (STD_ON)
#define IRQ_ADC4_EXIST              (STD_OFF)
#define IRQ_ADC5_EXIST              (STD_OFF)
#define IRQ_ADC6_EXIST              (STD_OFF)
#define IRQ_ADC7_EXIST              (STD_OFF)
#define IRQ_ADC8_EXIST              (STD_OFF)
#define IRQ_ADC9_EXIST              (STD_OFF)
#define IRQ_ADC10_EXIST             (STD_OFF)
#define IRQ_ADCCG0_EXIST            (STD_ON)
#define IRQ_ADCCG1_EXIST            (STD_ON)



#define IRQ_FLEXRAY_EXIST           (STD_OFF)
#define IRQ_FLEXRAY0_EXIST          (STD_OFF)
#define IRQ_FLEXRAY1_EXIST          (STD_OFF)

#define IRQ_PMU0_EXIST              (STD_ON)
#define IRQ_PMU0_SR0_EXIST          (STD_ON)
#define IRQ_PMU0_SR1_EXIST          (STD_ON)

#define IRQ_SCU_EXIST               (STD_OFF)

#define IRQ_GPSRGROUP_EXIST         (STD_ON)
#define IRQ_GPSRGROUP0_EXIST        (STD_ON)
#define IRQ_GPSRGROUP1_EXIST        (STD_OFF)
#define IRQ_GPSRGROUP2_EXIST        (STD_OFF)


#define IRQ_GTM_EXIST               (STD_ON)
#define IRQ_GTM_AEI_EXIST           (STD_ON)
#define IRQ_GTM_ARU_EXIST           (STD_OFF)
#define IRQ_GTM_BRC_EXIST           (STD_OFF)
#define IRQ_GTM_CMP_EXIST           (STD_OFF)
#define IRQ_GTM_SPE_EXIST           (STD_OFF)
#define IRQ_GTM_PSM0_EXIST          (STD_OFF)
#define IRQ_GTM_PSM1_EXIST          (STD_OFF)
#define IRQ_GTM_DPLL_EXIST          (STD_OFF)
#define IRQ_GTM_ERR_EXIST           (STD_ON)
#define IRQ_GTM_TIM0_EXIST          (STD_ON)
#define IRQ_GTM_TIM1_EXIST          (STD_OFF)
#define IRQ_GTM_TIM2_EXIST          (STD_OFF)
#define IRQ_GTM_TIM3_EXIST          (STD_OFF)
#define IRQ_GTM_TIM4_EXIST          (STD_OFF)
#define IRQ_GTM_TIM5_EXIST          (STD_OFF)
#define IRQ_GTM_MCS0_EXIST          (STD_OFF)
#define IRQ_GTM_MCS1_EXIST          (STD_OFF)
#define IRQ_GTM_MCS2_EXIST          (STD_OFF)
#define IRQ_GTM_MCS3_EXIST          (STD_OFF)
#define IRQ_GTM_MCS4_EXIST          (STD_OFF)
#define IRQ_GTM_MCS5_EXIST          (STD_OFF)
#define IRQ_GTM_TOM0_EXIST          (STD_ON)
#define IRQ_GTM_TOM1_EXIST          (STD_ON)
#define IRQ_GTM_TOM2_EXIST          (STD_OFF)
#define IRQ_GTM_TOM3_EXIST          (STD_OFF)
#define IRQ_GTM_TOM4_EXIST          (STD_OFF)
#define IRQ_GTM_ATOM0_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM1_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM2_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM3_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM4_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM5_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM6_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM7_EXIST         (STD_OFF)
#define IRQ_GTM_ATOM8_EXIST         (STD_OFF)

#define IRQ_SENT_EXIST              (STD_OFF)
#define IRQ_SENT0_EXIST             (STD_OFF)
#define IRQ_SENT1_EXIST             (STD_OFF)
#define IRQ_SENT2_EXIST             (STD_OFF)
#define IRQ_SENT3_EXIST             (STD_OFF)
#define IRQ_SENT4_EXIST             (STD_OFF)
#define IRQ_SENT5_EXIST             (STD_OFF)
#define IRQ_SENT6_EXIST             (STD_OFF)
#define IRQ_SENT7_EXIST             (STD_OFF)
#define IRQ_SENT8_EXIST             (STD_OFF)
#define IRQ_SENT9_EXIST             (STD_OFF)
#define IRQ_SENT10_EXIST            (STD_OFF)
#define IRQ_SENT11_EXIST            (STD_OFF)
#define IRQ_SENT12_EXIST            (STD_OFF)
#define IRQ_SENT13_EXIST            (STD_OFF)
#define IRQ_SENT14_EXIST            (STD_OFF)

/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/

/* IRQ_CFG_H */
#endif
