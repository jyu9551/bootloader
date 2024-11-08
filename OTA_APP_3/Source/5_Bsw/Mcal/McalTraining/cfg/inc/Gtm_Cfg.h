
/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2011)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. Passing on and copying of this document, and communication   **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME  : Gtm_Cfg.h                                                    **
**                                                                            **
**  VERSION   : 0.0.6                                                         **
**                                                                            **
**  DATE, TIME: 2021-11-10, 10:43:07                                          **
**                                                                            **
**  GENERATOR : Build b141014-0350                                            **
**                                                                            **
**  BSW MODULE DECRIPTION : Mcu.bmd                                           **
**                                                                            **
**  VARIANT   : VariantPB                                                     **
**                                                                            **
**  PLATFORM  : Infineon Aurix                                                **
**                                                                            **
**  COMPILER  : Tasking/GNU/Diab                                              **
**                                                                            **
**  AUTHOR    : DL-AUTOSAR-Engineering                                        **
**                                                                            **
**  VENDOR    : Infineon Technologies                                         **
**                                                                            **
**  DESCRIPTION  : GTM configuration generated out of ECU configuration      **
**                 file                                                       **
**                                                                            **
**  SPECIFICATION(S) : complex driver                                         **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/
#define GTM_CONFIG_COUNT  (1U)
#ifndef GTMCFG_H
#define GTMCFG_H
#define GTM_MCUINITCHECK_API (STD_OFF)

#define GTM_MCUSAFETY_ENABLE (STD_OFF)


#define GTM_MCU_MODULE_INSTANCE ((uint8)0)

/*
Configuration: GTM_RUNNING_IN_USER_0_MODE_ENABLE 
- if STD_ON, enable User0 mode 
- if STD_OFF, enable User1 mode 
*/
#define GTM_RUNNING_IN_USER_0_MODE_ENABLE (STD_OFF)

/*
Configuration: GTM_USER_MODE_INIT_API_ENABLE
- if STD_ON, Enable Protected Mode(user mode) in Init API 
- if STD_OFF, Enable Supervisor mode in Init API  
*/
#define GTM_USER_MODE_INIT_API_ENABLE (STD_OFF)

/*Configuration: GTM_USER_MODE_DEINIT_API_ENABLE
- if STD_ON, Enable Protected mode in DeInit API 
- if STD_OFF, Disable Protected mode in DeInit API
*/
#define GTM_USER_MODE_DEINIT_API_ENABLE (STD_OFF)

/*Configuration: GTM_MODE_RUNTIME_API_ENABLE
- if STD_ON, Enable Protected mode in APIs other than Init/Deinit APIs
- if STD_OFF, Disable Protected mode in APIs other than Init/Deinit APIs
*/
#define GTM_USER_MODE_RUNTIME_API_ENABLE (STD_OFF)

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/


/*****************************************************************************
**                                 GPT                                      **
******************************************************************************/

#define  GTM_GPT_MODULE_USED                        (STD_ON)
/*****************************************************************************
**                                 ICU                                      **
******************************************************************************/
#define GTM_ICU_MODULE_USED                          (STD_OFF)
/*****************************************************************************
**                                 PWM                                      **
******************************************************************************/
#define GTM_PWM_MODULE_USED                          (STD_OFF)

/* TIM and TOM usage */
#define GTM_TOM_USED_BY_OTHER_DRIVERS    (STD_ON)
#define GTM_TIM_USED_BY_OTHER_DRIVERS    (STD_OFF)



/* Resource Availability  */
#define GTM_TIM_MODULE  (0U)
#define GTM_NO_OF_TIM_MODULES   (1U)
#define GTM_CHANNELS_PER_TIM_MODULE    (8U)
#define GTM_MAX_TIM_CHANNELS                                                  \
                          (GTM_NO_OF_TIM_MODULES * GTM_CHANNELS_PER_TIM_MODULE)

#define GTM_TOM_MODULE  (1U)
#define GTM_NO_OF_TOM_MODULES   (2U)
#define GTM_NO_OF_TGC_PER_MODULE  (2U)
#define GTM_CHANNELS_PER_TOM_MODULE    (16U)
#define GTM_TOM_CHANNELS_PER_TGC   (8U)
#define GTM_MAX_TOM_CHANNELS                                                   \
                          (GTM_NO_OF_TOM_MODULES *  GTM_CHANNELS_PER_TOM_MODULE)

 


#define GTM_NO_OF_TBU_CHANNELS  (3U)

#define GTM_NO_OF_TOUTSEL_REGISTERS  (8U)
#define GTM_NO_OF_ADC_MODULES (4U)
#define GTM_ADC_CONN_PER_REGISTER (4U)
#define GTM_NO_OF_TTCAN_TRIGGERS (4U)

/* Gtm SFR resetting at Initialization is enabled */
#define GTM_SFR_RESET_ENABLE (STD_OFF)
/* Gtm De-Init API enabled */
#define GTM_DEINIT_API_ENABLE (STD_ON)




/*Number of TOM and  TIM channels configured for notifications*/
#define GTM_NO_OF_TOM_CH_CONF_NOTIF (1U)
#define GTM_NO_OF_TIM_CH_CONF_NOTIF (0U)

/* Generating defines for Global Channel Numbers */
#define GTM_TIM0_CHANNEL0   (0U)
#define GTM_TIM0_CHANNEL1   (1U)
#define GTM_TIM0_CHANNEL2   (2U)
#define GTM_TIM0_CHANNEL3   (3U)
#define GTM_TIM0_CHANNEL4   (4U)
#define GTM_TIM0_CHANNEL5   (5U)
#define GTM_TIM0_CHANNEL6   (6U)
#define GTM_TIM0_CHANNEL7   (7U)
#define GTM_TOM0_CHANNEL0   (8U)
#define GTM_TOM0_CHANNEL1   (9U)
#define GTM_TOM0_CHANNEL2   (10U)
#define GTM_TOM0_CHANNEL3   (11U)
#define GTM_TOM0_CHANNEL4   (12U)
#define GTM_TOM0_CHANNEL5   (13U)
#define GTM_TOM0_CHANNEL6   (14U)
#define GTM_TOM0_CHANNEL7   (15U)
#define GTM_TOM0_CHANNEL8   (16U)
#define GTM_TOM0_CHANNEL9   (17U)
#define GTM_TOM0_CHANNEL10   (18U)
#define GTM_TOM0_CHANNEL11   (19U)
#define GTM_TOM0_CHANNEL12   (20U)
#define GTM_TOM0_CHANNEL13   (21U)
#define GTM_TOM0_CHANNEL14   (22U)
#define GTM_TOM0_CHANNEL15   (23U)
#define GTM_TOM1_CHANNEL0   (24U)
#define GTM_TOM1_CHANNEL1   (25U)
#define GTM_TOM1_CHANNEL2   (26U)
#define GTM_TOM1_CHANNEL3   (27U)
#define GTM_TOM1_CHANNEL4   (28U)
#define GTM_TOM1_CHANNEL5   (29U)
#define GTM_TOM1_CHANNEL6   (30U)
#define GTM_TOM1_CHANNEL7   (31U)
#define GTM_TOM1_CHANNEL8   (32U)
#define GTM_TOM1_CHANNEL9   (33U)
#define GTM_TOM1_CHANNEL10   (34U)
#define GTM_TOM1_CHANNEL11   (35U)
#define GTM_TOM1_CHANNEL12   (36U)
#define GTM_TOM1_CHANNEL13   (37U)
#define GTM_TOM1_CHANNEL14   (38U)
#define GTM_TOM1_CHANNEL15   (39U)

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
#endif
