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
**   $FILENAME   : Adc_PBCfg.c $                                              **
**                                                                            **
**   $CC VERSION : \main\dev_tc23x_as4.0.3\16 $                               **
**                                                                            **
**   DATE, TIME: 2021-11-10, 10:43:08                                         **
**                                                                            **
**   GENERATOR : Build b141014-0350                                           **
**                                                                            **
**   AUTHOR    : DL-AUTOSAR-Engineering                                       **
**                                                                            **
**   VENDOR    : Infineon Technologies                                        **
**                                                                            **
**   DESCRIPTION  : Adc configuration generated out of ECU configuration      **
**                 file                                                       **
**                                                                            **
**   MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Inclution of module header file */
#include "Adc.h"
/* Import DMA channel information */
#include "Mcal_DmaLib.h"


/*******************************************************************************
**                      Private Macro definition                              **
*******************************************************************************/

/*******************************************************************************
**                      Configuration Options                                 **
*******************************************************************************/

/*
  Macros for ADC HW unit number definition
*/
#define HWUNIT_ADC0          (0U)
#define HWUNIT_ADC1          (1U)
#define HWUNIT_ADC2          (2U)
#define HWUNIT_ADC3          (3U)
#define HWUNIT_ADC4          (4U)
#define HWUNIT_ADC5          (5U)
#define HWUNIT_ADC6          (6U)
#define HWUNIT_ADC7          (7U)
#define HWUNIT_ADC8          (8U)
#define HWUNIT_ADC9          (9U)
#define HWUNIT_ADC10         (10U)

/*
  Macros for BNDSELX bitfields values definition
  Used for Standard mode limit checking
*/
#define BNDSELU_BNDSELL      (0U)
/*
  Configuration Options: ADC_ARBITRATION_ROUND_LENGTH
  Number of arbitration slots per arbitration round 
  (arbitration round length = tARB).
  00B - An arbitration round contains 4 arbitration slots
  (tARB = 4 / fADCD).
  01B - An arbitration round contains 8 arbitration slots
  (tARB = 8 / fADCD).
  10B - An arbitration round contains 16 arbitration
  slots (tARB = 16 / fADCD).
  11B - An arbitration round contains 20 arbitration
  slots (tARB = 20 / fADCD).
  Defines the number of arbitration slots per arbitration round.
  only 4 slots (i.e. ARBITRATION_SLOTS_4) are required for first 3 request 
  sources (i.e. RS0, RS1, RS2).
*/
#define ADC_ARBITRATION_SLOTS_4       (0U)
#define ADC_ARBITRATION_SLOTS_8       (1U)
#define ADC_ARBITRATION_SLOTS_16      (2U)
#define ADC_ARBITRATION_SLOTS_20      (3U)

/*
  Configuration Options: ADC_REQUEST_SOURCE_PRIO
  Priority of each request source 
  00B - Lowest Priority
  ...
  11B - Highest Priority
*/
#define ADC_REQSRC_PRIORITY_0_LOWEST       (0U)
#define ADC_REQSRC_PRIORITY_1              (1U)
#define ADC_REQSRC_PRIORITY_2              (2U)
#define ADC_REQSRC_PRIORITY_3_HIGHEST      (3U)

/*
  Configuration Options: ADC_HW_TRIG_TYPE
  Type of HW trigger : External request or Timer 
*/
#define ADC_HW_TRIGG_EXT_REQ          (0U)
#define ADC_HW_TRIGG_TIMER            (1U)

/*
  Configuration Options: Reference to MCU/ERU module
  ERU module selection for IOUT signal to form an ADC External request  
*/
#define ADC_HW_ERU0                   (0U)
#define ADC_HW_ERU1                   (1U)
#define ADC_HW_ERU2                   (2U)
#define ADC_HW_ERU3                   (3U)
#define ADC_HW_ERU4                   (4U)
#define ADC_HW_ERU5                   (5U)
#define ADC_HW_ERU6                   (6U)
#define ADC_HW_ERU7                   (7U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU0 Input lines  
*/
#define ERU_INPUT00_PORT15_PIN4       (0U)
#define ERU_INPUT01_CCU60_COUT60      (1U)
#define ERU_INPUT02_PORT10_PIN7       (2U)
#define ERU_INPUT03_MSC0_FCLP         (3U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU1 Input lines  
*/
#define ERU_INPUT10_PORT14_PIN3       (0U)
#define ERU_INPUT11_CCU61_COUT60      (1U)
#define ERU_INPUT12_PORT10_PIN8       (2U)
#define ERU_INPUT13_STM0_STMIR0       (3U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU2 Input lines  
*/
#define ERU_INPUT20_PORT10_PIN2       (0U)
#define ERU_INPUT21_PORT02_PIN1       (1U)
#define ERU_INPUT22_PORT00_PIN4       (2U)
#define ERU_INPUT23_ERAY0_MT          (3U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU3 Input lines  
*/
#define ERU_INPUT30_PORT10_PIN3       (0U)
#define ERU_INPUT31_PORT14_PIN1       (1U)
#define ERU_INPUT32_PORT02_PIN0       (2U)
#define ERU_INPUT33_STM1_STMIR0       (3U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU4 Input lines  
*/
#define ERU_INPUT40_PORT33_PIN7       (0U)
#define ERU_INPUT41_GTM_TOM0_12       (1U)
#define ERU_INPUT42_GPT120_T3OUT      (2U)
#define ERU_INPUT43_PORT15_PIN5       (3U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU5 Input lines  
*/
#define ERU_INPUT50_PORT15_PIN8       (0U)
#define ERU_INPUT51_GTM_TOM1_12       (1U)
#define ERU_INPUT52_GPT120_T6OUT      (2U)
#define ERU_INPUT53_STM2_STMIR0       (3U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU6 Input lines  
*/
#define ERU_INPUT60_PORT20_PIN0       (0U)
#define ERU_INPUT61_ESR0              (1U)
#define ERU_INPUT62_PORT33_PIN11      (2U)
#define ERU_INPUT63_PORT11_PIN10      (3U)

/*
  Configuration Options: Selection of ERU input line
  Selection of Input signal/pin to the selected ERU unit.
  The output of this ERU unit will trigger ADC
  ERU7 Input lines  
*/
#define ERU_INPUT70_PORT20_PIN9       (0U)
#define ERU_INPUT71_ESR1              (1U)
#define ERU_INPUT72_PORT15_PIN1       (2U)
#define ERU_INPUT73_ERAY1_MT          (3U)

/* Trigger selection from ERU IOUT signal */
#define ADC_TRSEL_ERU_IOUT            (7U)


/*
  Configuration Options: ADC_HW_TRIG_PIN
  Type of trigger: External request from ERU
*/
#define ADC0_TRSEL_RS0_RS1_IOUT0         (ADC_TRSEL_ERU_IOUT)
#define ADC1_TRSEL_RS0_RS1_IOUT1         (ADC_TRSEL_ERU_IOUT)
#define ADC2_TRSEL_RS0_RS1_IOUT2         (ADC_TRSEL_ERU_IOUT)
#define ADC3_TRSEL_RS0_RS1_IOUT3         (ADC_TRSEL_ERU_IOUT)
#define ADC4_TRSEL_RS0_RS1_IOUT4         (ADC_TRSEL_ERU_IOUT)
#define ADC5_TRSEL_RS0_RS1_IOUT5         (ADC_TRSEL_ERU_IOUT)
#define ADC6_TRSEL_RS0_RS1_IOUT6         (ADC_TRSEL_ERU_IOUT)
#define ADC7_TRSEL_RS0_RS1_IOUT7         (ADC_TRSEL_ERU_IOUT)
#define ADC8_TRSEL_RS0_RS1_IOUT0         (ADC_TRSEL_ERU_IOUT)
#define ADC9_TRSEL_RS0_RS1_IOUT1         (ADC_TRSEL_ERU_IOUT)
#define ADC10_TRSEL_RS0_RS1_IOUT2        (ADC_TRSEL_ERU_IOUT)
#define ADC0_TRSEL_RS0_RS1_RS3_IOUT0     (ADC_TRSEL_ERU_IOUT)
#define ADC1_TRSEL_RS0_RS1_RS3_IOUT1     (ADC_TRSEL_ERU_IOUT)
#define ADC2_TRSEL_RS0_RS1_RS3_IOUT2     (ADC_TRSEL_ERU_IOUT)
#define ADC3_TRSEL_RS0_RS1_RS3_IOUT3     (ADC_TRSEL_ERU_IOUT)
#define ADCx_TRSEL_RS2_IOUT3             (6U)
#define ADCx_TRSEL_RS2_IOUT4             (7U)
#define ADCx_TRSEL_RS2_IOUT6             (8U)
#define ADCx_TRSEL_RS2_IOUT7             (9U)

/* ERU OGU numbers */
#define ADC_ERU_OGU0                     (0U)
#define ADC_ERU_OGU1                     (1U)
#define ADC_ERU_OGU2                     (2U)
#define ADC_ERU_OGU3                     (3U)
#define ADC_ERU_OGU4                     (4U)
#define ADC_ERU_OGU5                     (5U)
#define ADC_ERU_OGU6                     (6U)
#define ADC_ERU_OGU7                     (7U)

/* Gate selection from GTM TRIG0 */
#define ADC_GTSEL_GTM_TRIG0              (0U)

/* Gate selection from GTM TRIG1 */
#define ADC_GTSEL_GTM_TRIG1              (1U)

/*
  Configuration Options: ADC_HW_GATE_PIN
  Type of trigger: External request/Timer 
*/
#define ADC0_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC0_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC1_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC1_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC2_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC2_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC3_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC3_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC4_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC4_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC5_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC5_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC6_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC6_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC7_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC7_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)

#define ADC8_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC8_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)

#define ADC9_GTSEL_RS0_RS1_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC9_GTSEL_RS0_RS1_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)

#define ADC10_GTSEL_RS0_RS1_TIM_TRIG0    (ADC_GTSEL_GTM_TRIG0)
#define ADC10_GTSEL_RS0_RS1_TIM_TRIG1    (ADC_GTSEL_GTM_TRIG1)

#define ADCx_GTSEL_RS2_TIM_TRIG0         (ADC_GTSEL_GTM_TRIG0)
#define ADCx_GTSEL_RS2_TIM_TRIG1         (ADC_GTSEL_GTM_TRIG1)

#define ADC0_GTSEL_RS0_RS1_RS3_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC0_GTSEL_RS0_RS1_RS3_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC1_GTSEL_RS0_RS1_RS3_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC1_GTSEL_RS0_RS1_RS3_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC2_GTSEL_RS0_RS1_RS3_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC2_GTSEL_RS0_RS1_RS3_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
#define ADC3_GTSEL_RS0_RS1_RS3_TIM_TRIG0     (ADC_GTSEL_GTM_TRIG0)
#define ADC3_GTSEL_RS0_RS1_RS3_TIM_TRIG1     (ADC_GTSEL_GTM_TRIG1)
                                         
/* Gate selection from GTM TRIG1 */
#define ADC_GTSEL_ERU_PDOUT              (12U)
/*
  Configuration Options: ADC_HW_GATE_PIN
  Type of Gate: External request from ERU
*/
#define ADC0_GTSEL_RS0_RS1_PDOUT0        (ADC_GTSEL_ERU_PDOUT)
#define ADC1_GTSEL_RS0_RS1_PDOUT1        (ADC_GTSEL_ERU_PDOUT)
#define ADC2_GTSEL_RS0_RS1_PDOUT2        (ADC_GTSEL_ERU_PDOUT)
#define ADC3_GTSEL_RS0_RS1_PDOUT3        (ADC_GTSEL_ERU_PDOUT)
#define ADC4_GTSEL_RS0_RS1_PDOUT4        (ADC_GTSEL_ERU_PDOUT)
#define ADC5_GTSEL_RS0_RS1_PDOUT5        (ADC_GTSEL_ERU_PDOUT)
#define ADC6_GTSEL_RS0_RS1_PDOUT6        (ADC_GTSEL_ERU_PDOUT)
#define ADC7_GTSEL_RS0_RS1_PDOUT7        (ADC_GTSEL_ERU_PDOUT)
#define ADC8_GTSEL_RS0_RS1_PDOUT0        (ADC_GTSEL_ERU_PDOUT)
#define ADC9_GTSEL_RS0_RS1_PDOUT1        (ADC_GTSEL_ERU_PDOUT)
#define ADC10_GTSEL_RS0_RS1_PDOUT2       (ADC_GTSEL_ERU_PDOUT)
#define ADC0_GTSEL_RS0_RS1_RS3_PDOUT0    (ADC_GTSEL_ERU_PDOUT)
#define ADC1_GTSEL_RS0_RS1_RS3_PDOUT1    (ADC_GTSEL_ERU_PDOUT)
#define ADC2_GTSEL_RS0_RS1_RS3_PDOUT2    (ADC_GTSEL_ERU_PDOUT)
#define ADC3_GTSEL_RS0_RS1_RS3_PDOUT3    (ADC_GTSEL_ERU_PDOUT)

/*
  Configuration Options: ADC_HW_GATE_PIN
  Type of Gate: External request from CCU6
*/
#define ADCx_GTSEL_RS0_RS1_RS2_CCU6061_TRIG0       (0x2U)
#define ADCx_GTSEL_RS0_RS1_RS2_CCU6061_TRIG1       (0x3U)
#define ADCx_GTSEL_RS0_RS1_RS2_CCU6061_TRIG2       (0x4U)
#define ADCx_GTSEL_RS0_RS1_RS2_RS3_CCU6061_TRIG0   (0x2U)
#define ADCx_GTSEL_RS0_RS1_RS2_RS3_CCU6061_TRIG1   (0x3U)
#define ADCx_GTSEL_RS0_RS1_RS2_RS3_CCU6061_TRIG2   (0x4U)

/*
  Configuration Options: ADC_HW_GATE_SIGNAL
  Gating signal High or Low 
*/
#define ADC_GATE_SIGNAL_HIGH             (2U)
#define ADC_GATE_SIGNAL_LOW              (3U)
#define ADC_GATE_SIGNAL_NONE             (1U)

/* Macro to use the Gate pin as Trigger */
#define ADC_USE_GATE_PIN_TO_TRIG         (0xFU)

/* Macro to use the Internal Trigger fo RS3 */
#define ADC_USE_INTERNAL_TRIG_TO_TRIG_RS3          (0xFU)

/*
  Configuration Options: AdcHwExtTrigSelect
*/
#define ADC_GROUP_SR1_TRIG               (12U)

#define ADC0_TRSEL_RS0_RS1_G0_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC1_TRSEL_RS0_RS1_G1_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC2_TRSEL_RS0_RS1_G2_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC3_TRSEL_RS0_RS1_G3_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC4_TRSEL_RS0_RS1_G4_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC5_TRSEL_RS0_RS1_G5_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC6_TRSEL_RS0_RS1_G6_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC7_TRSEL_RS0_RS1_G7_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC8_TRSEL_RS0_RS1_G8_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC9_TRSEL_RS0_RS1_G9_SR1        (ADC_GROUP_SR1_TRIG)
#define ADC10_TRSEL_RS0_RS1_G10_SR1      (ADC_GROUP_SR1_TRIG)
#define ADC0_TRSEL_RS0_RS1_RS3_G0_SR1    (ADC_GROUP_SR1_TRIG)
#define ADC1_TRSEL_RS0_RS1_RS3_G1_SR1    (ADC_GROUP_SR1_TRIG)
#define ADC2_TRSEL_RS0_RS1_RS3_G2_SR1    (ADC_GROUP_SR1_TRIG)
#define ADC3_TRSEL_RS0_RS1_RS3_G3_SR1    (ADC_GROUP_SR1_TRIG)

#define ADCx_TRSEL_RS0_RS1_RS2_CCU60_SR3     (0x0U)
#define ADCx_TRSEL_RS0_RS1_RS2_CCU61_SR3     (0x1U)
#define ADCx_TRSEL_RS0_RS1_RS2_RS3_CCU60_SR3 (0x0U)
#define ADCx_TRSEL_RS0_RS1_RS2_RS3_CCU61_SR3 (0x1U)

#define ADCx_TRSEL_RS0_RS1_RS2_C0_SR1        (13U)
#define ADCx_TRSEL_RS0_RS1_RS2_C1_SR1        (14U)
#define ADCx_TRSEL_RS0_RS1_RS2_RS3_C0_SR1    (13U)
#define ADCx_TRSEL_RS0_RS1_RS2_RS3_C1_SR1    (14U)

/* 
  Configuration Options: Sleep Mode
  values for Adc_ModuleCfgType.SleepMode
*/
/* ADC module responds to the sleep mode request from MCU driver */
#define ADC_SLEEP_MODE_ACCEPT            (0x00000000U)
/* Sleep mode request from MCU driver is rejected, module remains in its current
   state */
#define ADC_SLEEP_MODE_REJECT            (0x00000001U)

/*
  Configuration Options: post calibration control
*/
/* Define to enable post calibration */
#define ADC_POST_CAL_ENABLE              (0U)
/* Define to disable post calibration */
#define ADC_POST_CAL_DISABLE             (1U)

#if (ADC_ENABLE_LIMIT_CHECK == STD_ON)
/* Channel Limit check enabled */
#define ADC_CHANNEL_LIMIT_CHECK          (0x1U)
#endif

#if (ADC_MASTER_SLAVE_SYNC == STD_ON)
/* Master / Slave selection in GxSYNCTR.STSEL bit field */
#define ADC_SLAVE_CI1                    (1U)
#define ADC_SLAVE_CI2                    (2U)
#define ADC_SLAVE_CI3                    (3U)
#define ADC_EVAL_R1                      (0x10U)
#define ADC_EVAL_R2                      (0x20U)
#define ADC_EVAL_R3                      (0x40U)
#endif
#define ADC_STANDALONE                   (0U)

/* Macros for Alternate reference mode */
#define ADC_REF_LOCAL_MODE               (0U)
#define ADC_REF_GLOBAL_MODE              (1U)

#if (ADC_ALTERNATE_REF_MODE == ADC_REF_GLOBAL_MODE)
/*
  Macros for Sample and Hold unit number definition
*/
#define ADC_SAMPLE_HOLD_UNIT0          (0U)
#define ADC_SAMPLE_HOLD_UNIT1          (1U)
#define ADC_SAMPLE_HOLD_UNIT2          (2U)
#define ADC_SAMPLE_HOLD_UNIT3          (3U)
#define ADC_SAMPLE_HOLD_UNIT4          (4U)
#define ADC_SAMPLE_HOLD_UNIT5          (5U)
#define ADC_SAMPLE_HOLD_UNIT6          (6U)
#define ADC_SAMPLE_HOLD_UNIT7          (7U)
#define ADC_SAMPLE_HOLD_UNIT8          (8U)
#define ADC_SAMPLE_HOLD_UNIT9          (9U)
#define ADC_SAMPLE_HOLD_UNIT10         (10U)
#endif /* (ADC_ALTERNATE_REF_MODE == ADC_REF_GLOBAL_MODE) */

/*
  Macros for Operation mode of the converter
*/
#define ADC_NORMAL_MODE                (3U)
#define ADC_FAST_STANDBY_MODE          (2U)
#define ADC_SLOW_STANDBY_MODE          (1U)

/*
  Macros for Calibration order of the converter
*/
#define ADC_CONVERT_THEN_CAL           (0U)
#define ADC_CAL_THEN_CONVERT           (1U)

/*
  Macros for enhanced conversion timing scheme
*/
#define ADC_ACCELERATED_TIMING_ENABLE  (1U)
#define ADC_ACCELERATED_TIMING_DISABLE (0U)

/* Macros to define the Adc Global control parameters */
#if (ADC_MASTER_SLAVE_SYNC == STD_ON)
/* ARBM = 0, if Synchronization of conversion is enabled */
#define ADC_GxARBCFG_ARBM           (0U) 
#else
#define ADC_GxARBCFG_ARBM           (1U) 
#endif /* (ADC_MASTER_SLAVE_SYNC == STD_ON) */

/* All request sources are configured for CANCEL_INJECT_REPEAT mode */
#define ADC_RS_CANCEL_INJECT_REPEAT ((uint32)(0x00008888U))

/* Macro to enable the detection of input trigger in ERU */
#define ADC_ERU_EIEN_ENABLE         (1U)


#define ADC_POWER_SUPPLY_5V         ((uint32)(0x00000000))
#define ADC_POWER_SUPPLY_3_3V       ((uint32)(0x00004000))

#define ADC_USE_VDDM_PRECH_VAREF_ADJ ((uint32)(0x00000000))
#define ADC_USE_VAREF_CONVERSION     ((uint32)(0x00001000))

#if (ADC_CCU6_SYNCHRONIZATION == STD_ON)
#define ADC_CCU6_MODULE_CCU60        ((uint8)0x0)
#define ADC_CCU6_MODULE_CCU61        ((uint8)0x1)
#endif /* (ADC_CCU6_SYNCHRONIZATION == STD_ON) */
/*******************************************************************************
**                     Utility Macros                                         **
*******************************************************************************/

/*******************************************************************************
** Name             : Adc_lConfigChannel                                      **
**                                                                            **
** Description      : Macro to configure ADC Channel parameters               **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lConfigChannel is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lConfigChannel(AdcChannelRefVoltsrcHigh, \
                           AdcInputClassSelection, \
                           AnalogInputChannelNumber \
                          ) \
(                                                \
    ((uint32)((AnalogInputChannelNumber) & ADC_AN_CH_MASK) <<  16U) | \
    ((uint32)(AdcChannelRefVoltsrcHigh) << 11U)  | \
    ((uint32)(AdcInputClassSelection)) \
)

#if (ADC_ENABLE_LIMIT_CHECK == STD_ON)
/*******************************************************************************
** Name             : Adc_lCfgChBoundry                                       **
**                                                                            **
** Description      : Macro to configure ADC Channel parameters               **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lCfgChBoundry is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lCfgChBoundry(AdcChannelBound0,AdcChannelBound1)                   \
(                                                                              \
    ((uint32)(AdcChannelBound0)) |                                             \
    ((uint32)(AdcChannelBound1) << (uint32)16)                                 \
)
#endif /* (ADC_ENABLE_LIMIT_CHECK == STD_ON) */
/*******************************************************************************
** Name             : Adc_lArbLvlCtrl                                         **
**                                                                            **
** Description      :  Arbitration Level Control Register                     **
**                    Data compution                                          **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lArbLvlCtrl is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lArbLvlCtrl(AdcRS0Prio,AdcRS1Prio,AdcRS2Prio,AdcRS3Prio) \
                                ( (AdcRS0Prio)              |  \
                                 ((uint32)(AdcRS1Prio) << 4U)       |  \
                                 ((uint32)(AdcRS2Prio) << 8U)       |  \
                                 ((uint32)(AdcRS3Prio) << 12U)      |  \
                                 (ADC_RS_CANCEL_INJECT_REPEAT) \
                                )

/*******************************************************************************
** Name             : Adc_lInpClass                                           **
**                                                                            **
** Description      :  ADC Channel resolution and sampling time               **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lInpClass is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lInpClass(AdcChSampTime,AdcChRes,AdcEmuxChSampTime) \
                                ( (AdcChSampTime)      | \
                                 ((AdcChRes) << 0x08U) | \
                                 ((AdcEmuxChSampTime) << 0x10U) | \
                                 ((AdcChRes) << 0x18U) \
                                )
/*******************************************************************************
** Name             : Adc_lConfigPrescale                                     **
**                                                                            **
** Description      : Macro to configure ADC clock prescale                   **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lConfigPrescale is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lConfigPrescale(AdcAnClkDiv,AdcDigClkDiv) \
(                                                  \
  ((uint32)(AdcDigClkDiv) << 8U)      | \
  (AdcAnClkDiv)                 \
)
/*******************************************************************************
** Name             : Adc_lConfigArbitration                                  **
**                                                                            **
** Description      : Macro to configure ADC Arbitration                      **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lConfigArbitration is declared as a function
 like macro to avoid function call in configuration data structures*/
#define Adc_lConfigArbitration(AdcArbRndLength)                                \
(                                                                              \
  ((uint32)(ADC_GxARBCFG_ARBM) << 7U) |                                        \
  ((uint32)(AdcArbRndLength)  << 4U)                                           \
)
#if (ADC_USE_EMUX == STD_ON)
/*******************************************************************************
** Name             : Adc_lCfgEmuxSelectGrp                                   **
**                                                                            **
** Description      : Macro to configure ADC Emux Group interface line        **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lCfgEmuxSelectGrp is declared as a function
 like macro to avoid function call in configuration data structures*/
#define Adc_lCfgEmuxSelectGrp(AdcEmuxGrp0,AdcEmuxGrp1) \
(                                                  \
  ((uint32)(AdcEmuxGrp1) << 4U)      | \
  (AdcEmuxGrp0)                 \
)
#endif /* (ADC_USE_EMUX == STD_ON) */

/*******************************************************************************
** Name             : Adc_lConfigHwReqSrc                                     **
**                                                                            **
** Description      : Macro to configure External trigger source for Request  **
**                    line selection and trigger edge selection               **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lConfigHwReqSrc is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lConfigHwReqSrc(GatePin, \
                            TriggType,GateSignal,TriggPin,TriggSignal, \
                            EruUnit, EruInput,EruOutput,EruSelect)\
(                                             \
  ((uint32)(GatePin)) |                       \
  ((uint32)(TriggType) << 4U ) |              \
  ((uint32)(GateSignal) << 5U ) |             \
  ((uint32)(TriggPin) << 8U ) |               \
  ((uint32)(TriggSignal) << 12U ) |           \
  ((uint32)(EruUnit) << 16U ) |               \
  ((uint32)(EruInput) << 20U) |               \
  ((uint32)ADC_ERU_EIEN_ENABLE << 27U) |      \
  ((uint32)(EruOutput) << 28U) |              \
  ((uint32)(EruSelect) << 31U)                \
)

#if (ADC_ERU_GATING == STD_ON)
/*******************************************************************************
** Name             : Adc_lConfigEruGate                                      **
**                                                                            **
** Description      : Macro to configure External Gate source for Request     **
**                    line selection and Gate edge selection                  **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lConfigEruGate is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lConfigEruGate(GatePin, GateSignal, EruGateUnit, \
                           EruGateInput,EruGateOutput,EruGateSelect)\
(                                             \
  ((uint32)(GatePin)) |                       \
  ((uint32)(GateSignal) << 5U ) |             \
  ((uint32)(EruGateUnit) << 16U ) |           \
  ((uint32)(EruGateInput) << 20U) |           \
  ((uint32)(EruGateOutput) << 28U) |          \
  ((uint32)(EruGateSelect) << 31U)            \
)
#endif /* (ADC_ERU_GATING == STD_ON) */

#if (ADC_USE_EMUX == STD_ON)
/*******************************************************************************
** Name             : Adc_lGroupEmuxData                                      **
**                                                                            **
** Description      : Macro to configure EMUX scan bit and Emux select value  **
**                                                                            **
*******************************************************************************/
/*IFX_MISRA_RULE_19_07_STATUS=Adc_lGroupEmuxData is declared as a function like 
 macro to avoid function call in configuration data structures*/
#define Adc_lGroupEmuxData(EmuxSelect, EmuxScan, EmuxEnable)   \
             ((uint8)((EmuxSelect) | (EmuxScan) | ((uint32)(EmuxEnable) << 3U)))

#endif /* (ADC_USE_EMUX == STD_ON) */
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/


/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/


/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Funtion Declarations                           **
*******************************************************************************/


/* AdcHwUnit0_AdcGroup0 Group notification function declartion */
extern void AdcNotification_AdcHwUnit0_AdcGroup0(void);
/* AdcHwUnit1_AdcGroup0 Group notification function declartion */
extern void AdcNotification_AdcHwUnit1_AdcGroup0(void);
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/
/* Mapping of unspecified size constant */
#define ADC_START_SEC_POSTBUILDCFG
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#if (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW)
/*
  Constant for extracting the Groups configured for a particular trigger source
  Used in Scheduler function in Adc.c
*/
/* ADC0 configuration */
static const uint32 Adc_kKernel0GroupMask0[] =
{
  0x0U, /* For Request source 0 */
  0x1U, /* For Request source 1 */
  0x0U,  /* For Request source 2 */
  0x0U  /* For Request source 3 */
};
/* ADC1 configuration */
static const uint32 Adc_kKernel1GroupMask0[] =
{
  0x0U, /* For Request source 0 */
  0x1U, /* For Request source 1 */
  0x0U,  /* For Request source 2 */
  0x0U  /* For Request source 3 */
};
#endif /* (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW) */

/*
                     Container: AdcGlobalConfiguration
*/
/*Configuration set 1 */
static const Adc_GlobalCfgType Adc_kGlobCfg0 =
{
  /* ADC088: Module Clock control 
     Divider Factor for Analog Internal Clock (fADCI)
     Divider Factor for Digital Arbiter Clock (fADCD)
  */
  Adc_lConfigPrescale( 
    (Adc_PrescaleType)0xa,
    (Adc_PrescaleType)0x0
    ),
  /*Global Input class configuration */
  {
    Adc_lInpClass
    ( (uint32)0,
      (uint32)ADC_CH_RES_12BIT,
      (uint32)0
    ),
    Adc_lInpClass
    ( (uint32)0,
      (uint32)ADC_CH_RES_12BIT,
      (uint32)0
    )

  },
  /* Control of post calibration */
  (uint32)( 
             ((uint32)ADC_POST_CAL_DISABLE << 16U) |  
             ((uint32)ADC_POST_CAL_DISABLE << 17U)   
           ),
  ADC_POWER_SUPPLY_5V,
  ADC_USE_VDDM_PRECH_VAREF_ADJ,
  ADC_NORMAL_MODE
};

/*
                     Container: AdcHWUnitConfiguration
*/
/* 
Configuration: ADC HW unit configuration structure
*/
/*Configuration set 1 */
/* ADC 0 hardware unit configuration */
static const Adc_HwUnitCfgType Adc_kKernel0HwCfg0 =
{
  /*  
     Arbitration Round Length: 00b - 11b
     ANONC : 11b
     ARBM  : 1b
  */
  Adc_lConfigArbitration((uint32)ADC_ARBITRATION_SLOTS_4),
   
  /* Trigger source arbitration level control */
  /* HW trigger sources participate in arbitration only when 
     priority mechanism PRIORITY_HW or PRIORITY_HW_SW considered
     Default value for each trigger source is : REQSRC_PRIORITY_0_LOWEST 
     In No priority, the priority levels of trigger source data shall be set 
     to default value.
  */
  Adc_lArbLvlCtrl(                           
                   ADC_REQSRC_PRIORITY_0_LOWEST,
                   ADC_REQSRC_PRIORITY_0_LOWEST,
                   ADC_REQSRC_PRIORITY_0_LOWEST,
                                       ADC_REQSRC_PRIORITY_0_LOWEST
                                     ),
 
  /*Adc Kernel Input class configuration */
  {
    Adc_lInpClass
    ( (uint32)0,
      (uint32)ADC_CH_RES_12BIT,
      (uint32)0
    ),
    Adc_lInpClass
    ( (uint32)0,
      (uint32)ADC_CH_RES_12BIT,
      (uint32)0
    )

  },
  
  /* Kernel Mode for Parallel sampling */
  ADC_STANDALONE,
  (0x00U), /* Evaluate ready input Rx is 0 for Standalone kernel */ 

  (uint8)(DMA_CHANNEL_INVALID)

};/* Adc HwCfg */
/* ADC 1 hardware unit configuration */
static const Adc_HwUnitCfgType Adc_kKernel1HwCfg0 =
{
  /*  
     Arbitration Round Length: 00b - 11b
     ANONC : 11b
     ARBM  : 1b
  */
  Adc_lConfigArbitration((uint32)ADC_ARBITRATION_SLOTS_4),
   
  /* Trigger source arbitration level control */
  /* HW trigger sources participate in arbitration only when 
     priority mechanism PRIORITY_HW or PRIORITY_HW_SW considered
     Default value for each trigger source is : REQSRC_PRIORITY_0_LOWEST 
     In No priority, the priority levels of trigger source data shall be set 
     to default value.
  */
  Adc_lArbLvlCtrl(                           
                   ADC_REQSRC_PRIORITY_3_HIGHEST,
                   ADC_REQSRC_PRIORITY_0_LOWEST,
                   ADC_REQSRC_PRIORITY_0_LOWEST,
                                       ADC_REQSRC_PRIORITY_0_LOWEST
                                     ),
 
  /*Adc Kernel Input class configuration */
  {
    Adc_lInpClass
    ( (uint32)0,
      (uint32)ADC_CH_RES_12BIT,
      (uint32)0
    ),
    Adc_lInpClass
    ( (uint32)0,
      (uint32)ADC_CH_RES_12BIT,
      (uint32)0
    )

  },
  
  /* Kernel Mode for Parallel sampling */
  ADC_STANDALONE,
  (0x00U), /* Evaluate ready input Rx is 0 for Standalone kernel */ 

  (uint8)(DMA_CHANNEL_INVALID)

};/* Adc HwCfg */

/*
                     Container: AdcChannelConfiguration
*/
/* 
Configuration: ADC Channel configuration structure
*/
/*Configuration set 1 */
/* ADC 0 Channel configuration */
static const Adc_ChannelCfgType Adc_kKernel0ChCfg0[]=
{

 {
  /* HwVersionResistance  configuration */
  Adc_lConfigChannel(
                      ADC_REF_VOLTAGE_VAREF,
                      (2U),
                      (0xbU)
                    ),
  ADC_RES_ACCUMULATION_NONE,
  0U,  /* Limit Check not enable for the channel */
  0U,  /* No channel event on Limit Check */
  0U,  /* Boundary values */
  0U,  /* value of BNDSELX bitfields */
  0U   /* Channel not enabled for synchronization */
 }
}; /* Adc_ChCfg0 */
/* ADC 1 Channel configuration */
static const Adc_ChannelCfgType Adc_kKernel1ChCfg0[]=
{

 {
  /* IgnitionVoltage  configuration */
  Adc_lConfigChannel(
                      ADC_REF_VOLTAGE_VAREF,
                      (2U),
                      (0x19U)
                    ),
  ADC_RES_ACCUMULATION_NONE,
  0U,  /* Limit Check not enable for the channel */
  0U,  /* No channel event on Limit Check */
  0U,  /* Boundary values */
  0U,  /* value of BNDSELX bitfields */
  0U   /* Channel not enabled for synchronization */
 },


 {
  /* BatteryVoltage  configuration */
  Adc_lConfigChannel(
                      ADC_REF_VOLTAGE_VAREF,
                      (2U),
                      (0x1bU)
                    ),
  ADC_RES_ACCUMULATION_NONE,
  0U,  /* Limit Check not enable for the channel */
  0U,  /* No channel event on Limit Check */
  0U,  /* Boundary values */
  0U,  /* value of BNDSELX bitfields */
  0U   /* Channel not enabled for synchronization */
 }
}; /* Adc_ChCfg0 */

/*
                     Container: AdcGroupConfiguration
*/
/*
Configuration: ADC Group Definitions
Array of channel Id (i.e. Channel Index of channel configuration structure )
ending with Delimiter for each Adc Channel Group.
Maximum number of channels in a group shall be 16.
Channels under the same group can not share the same CHCTR register and hence 
configuration of Channel(x) and Channel(x+n16) under the same group is not
allowed.
*/
/*Configset 1 */
/* ADC 0 unit configuration */



static const Adc_GroupDefType Adc_kCfg0Kernel0AdcHwUnit0_AdcGroup0[]=
{  
  1U,

  0U 
};



/* ADC 1 unit configuration */



static const Adc_GroupDefType Adc_kCfg0Kernel1AdcHwUnit1_AdcGroup0[]=
{  
  2U,

  0U ,
  1U 
};






/* 
Configuration: ADC Group configuration structure
 - ADC028 : ADC_GROUP_ID refered by Array Index
            ADC_GROUP_PRIORITY if ADC_PRIORITY_HW_SW considered, 
            ADC_GROUP_REPLACEMENT if ADC_PRIORITY_HW_SW considered
            ADC_GROUP_DEFINITION
            ADC_GROUP_TRIGG_SRC
            ADC_GROUP_CONV_MODE
            ADC_STREAMING_NUM_SAMPLES
            ADC_STREAMING_BUFFER_MODE
            ADC_HW_TRIG_SIGNAL if EXT TR Source selected
            ADC_HW_TRIG_TIMER if TIM TR source selected
*/
/* ADC Groups are arranged in the order of their trigger type ( HW/SW ) and 
   request sources (RS0 .. RS2) starting from SW trigger RS0 to 
   SW trigger RS2 and then HW trigger RS0 to HW trigger RS2.
*/
/*Configset 1 */
/* ADC 0 Group configuration */
static const Adc_GroupCfgType Adc_kKernel0GrpCfg0[] =
{
  {
    /*AdcHwUnit0_AdcGroup0*/
    #if (ADC_GRP_NOTIF_CAPABILITY == STD_ON)
    /* ADC079: ADC081: ADC084: ADC085: ADC104: Notification function pointer */
    &AdcNotification_AdcHwUnit0_AdcGroup0,
    #endif /* (ADC_GRP_NOTIF_CAPABILITY == STD_ON) */

    /* Assignment of channels to a channel group */
    /* First element is the number of configured channels in the group */
    /* From Second element will give the channel ID */
    &Adc_kCfg0Kernel0AdcHwUnit0_AdcGroup0[0U],

#if (ADC_HW_TRIGGER_API == STD_ON)
    (uint32)0, /* no timer cfg values for SW trigg group*/
    (uint32)0,
    (uint32)0,
    (uint32)0,
    (uint32)0,
    (uint32)0,
#endif

    /* Internal channel mask from group defination - derived from the tool */
    (uint16)0x800,

    /* Group trigger source : SW / HW */
    ADC_TRIGG_SRC_SW,

    /* Group HW trigger source : RS0 - RS2 */
    ADC_REQSRC1_NCH_SCAN,

    /* Group conversion mode */
    ADC_CONV_MODE_ONESHOT,

#if (ADC_RESULT_HANDLING_MODE == ADC_AUTOSAR)
    /* Number of ADC values to be acquired in streaming access mode */
    (Adc_StreamNumSampleType)1,

    /* ADC316: Buffer mode type - Configure streaming buffer as 
       "linear buffer" or "ring buffer" */
    ADC_STREAM_BUFFER_LINEAR,

#if (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW)

    /* Priority level of the group */
    (Adc_GroupPriorityType)253,
    /* Group level Replacement mechanism - 
       Vaild only when Group is interruptable
    */
    ADC_GROUP_REPL_ABORT_RESTART
#endif /* (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW) */

#endif /* (ADC_RESULT_HANDLING_MODE == ADC_AUTOSAR) */

  }/*AdcHwUnit0_AdcGroup0*/ 
};
/* ADC 1 Group configuration */
static const Adc_GroupCfgType Adc_kKernel1GrpCfg0[] =
{
  {
    /*AdcHwUnit1_AdcGroup0*/
    #if (ADC_GRP_NOTIF_CAPABILITY == STD_ON)
    /* ADC079: ADC081: ADC084: ADC085: ADC104: Notification function pointer */
    &AdcNotification_AdcHwUnit1_AdcGroup0,
    #endif /* (ADC_GRP_NOTIF_CAPABILITY == STD_ON) */

    /* Assignment of channels to a channel group */
    /* First element is the number of configured channels in the group */
    /* From Second element will give the channel ID */
    &Adc_kCfg0Kernel1AdcHwUnit1_AdcGroup0[0U],

#if (ADC_HW_TRIGGER_API == STD_ON)
    (uint32)0, /* no timer cfg values for SW trigg group*/
    (uint32)0,
    (uint32)0,
    (uint32)0,
    (uint32)0,
    (uint32)0,
#endif

    /* Internal channel mask from group defination - derived from the tool */
    (uint16)0xa00,

    /* Group trigger source : SW / HW */
    ADC_TRIGG_SRC_SW,

    /* Group HW trigger source : RS0 - RS2 */
    ADC_REQSRC1_NCH_SCAN,

    /* Group conversion mode */
    ADC_CONV_MODE_ONESHOT,

#if (ADC_RESULT_HANDLING_MODE == ADC_AUTOSAR)
    /* Number of ADC values to be acquired in streaming access mode */
    (Adc_StreamNumSampleType)1,

    /* ADC316: Buffer mode type - Configure streaming buffer as 
       "linear buffer" or "ring buffer" */
    ADC_STREAM_BUFFER_LINEAR,

#if (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW)

    /* Priority level of the group */
    (Adc_GroupPriorityType)254,
    /* Group level Replacement mechanism - 
       Vaild only when Group is interruptable
    */
    ADC_GROUP_REPL_ABORT_RESTART
#endif /* (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW) */

#endif /* (ADC_RESULT_HANDLING_MODE == ADC_AUTOSAR) */

  }/*AdcHwUnit1_AdcGroup0*/ 
};

/*
                     Container: Adc Kernel Configuration
*/
/*
Configuration : Adc_KernelConfigType
Configuration of ADC: 
Pointer to Adc Hw Unit configuration
Pointer to the array of channel configuration
Pointer to the array of group configuration
Pointer to the array of Group mask in each trigger source
Total number of configured groups
*/
/*Configset 1 */
/* ADC 0  configuration */
static const Adc_KernelConfigType  Adc_kKernel0Config0 =
{
  /* Pointer to Adc Hw Unit configuration */
  &Adc_kKernel0HwCfg0,

  /* Pointer to the array of channel configuration */
  &Adc_kKernel0ChCfg0[0U],

  /* Pointer to the array of group configuration */
  &Adc_kKernel0GrpCfg0[0U],

#if (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW)
  /* Pointer to the array of Group mask for SW, AS, CHIN trigger sources */
  &Adc_kKernel0GroupMask0[0U],
#endif /* (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW) */

  /* Total number of configured groups */
  1U
};/* Adcx Configuration */
 
/* ADC 1  configuration */
static const Adc_KernelConfigType  Adc_kKernel1Config0 =
{
  /* Pointer to Adc Hw Unit configuration */
  &Adc_kKernel1HwCfg0,

  /* Pointer to the array of channel configuration */
  &Adc_kKernel1ChCfg0[0U],

  /* Pointer to the array of group configuration */
  &Adc_kKernel1GrpCfg0[0U],

#if (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW)
  /* Pointer to the array of Group mask for SW, AS, CHIN trigger sources */
  &Adc_kKernel1GroupMask0[0U],
#endif /* (ADC_PRIORITY_IMPLEMENTATION == ADC_PRIORITY_HW_SW) */

  /* Total number of configured groups */
  1U
};/* Adcx Configuration */
 
 

/*
                     Container: AdcConfiguration
*/
/*
Configuration : Adc_ConfigType
Configuration of ADC: 
Pointer to Adc Hw Unit configuration
Pointer to the array of channel configuration
Pointer to the array of group configuration
Pointer to the array of Group mask in each trigger source
Total number of configured groups
*/
const Adc_ConfigType  Adc_ConfigRoot[1] =
{
/*Configset 1 */
  {
    {
      /* Pointer to Adc0 configuration */
      &Adc_kKernel0Config0,
      /* Pointer to Adc1 configuration */
      &Adc_kKernel1Config0
    },

    /* Pointer to Adc Global configuration value */
    &Adc_kGlobCfg0,
  
    /* Module sleep mode control */
    ADC_SLEEP_MODE_ACCEPT
  } 
};/* AdcConfiguration */



#define ADC_STOP_SEC_POSTBUILDCFG
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


