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
**  FILENAME  : Gtm_PBCfg.c                                                   **
**                                                                            **
**  $CC VERSION : \main\dev_tc23x\9 $                                        **
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
**  SPECIFICATION(S) : complex driver implementation                          **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/
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
**  FILENAME  : Gtm.m                                                        **
**                                                                           **
**  $CC VERSION : \main\dev_tc23x\8 $                                        **
**                                                                           **
**  DATE, TIME: 2021-11-10, 10:43:07                                         **
**                                                                           **
**  GENERATOR : Build b141014-0350                                           **
**                                                                           **
**  AUTHOR    : DL-AUTOSAR-Engineering                                       **
**                                                                           **
**  VENDOR    : Infineon Technologies                                        **
**                                                                           **
**  DESCRIPTION  : GTM configuration generated out of ECU configuration      **
**                 file (Mcu.bmd/.xdm) for Gtm.m file for TC23x              **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                                                                            **
*******************************************************************************/



/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Own header file, this includes own configuration file also */
#include "Gtm.h"



/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/* Note:
The user can configure the parameters with the tag Configuration:
The user should not change anything under the tag Configuration Options:
*/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define GTM_INTERRUPT_LEVEL_MODE          (0U)
#define GTM_INTERRUPT_PULSE_MODE          (1U)
#define GTM_INTERRUPT_PULSE_NOTIFY_MODE   (2U) 
#define GTM_INTERRUPT_SINGLE_PULSE_MODE   (3U)


#define TSPP1_SUBUNIT_OUTPUT 5

/* For Tbu */
#define BITS_0_TO_23   (0U)
#define BITS_3_TO_26   (1U)
#define FREE_RUNNING_COUNTER_MODE      (0)
#define FORWARD_BACKWARD_COUNTER_MODE  (1)

#define Gtm_lTbuBuildControl(Bit0, Bit123, Bit5)                              \
 (0x00U | (uint8)(Bit0) | (uint8)((uint8)(Bit123) << 1) |                     \
                                                 (uint8)((uint8)(Bit5) << 5))
#define Gtm_lTimerCtrlValue(Word, ClockValue)                                 \
                        ((uint32)(Word) | (uint32)((uint32)(ClockValue) << 12))
#define Gtm_TimbuildReg(Word, ClockValue)                                     \
                         ((uint32)(Word)| (uint32)((uint32)(ClockValue) << 24))
#define Gtm_TimTduBuildReg(Word, ClockValue)                                  \
                        ((uint32)(Word) | (uint32)((uint32)(ClockValue) << 28))
/* For Interrupt Mode Appending */
#define Gtm_lIncludeIntMode8Bit(Bytevalue, IrqMode)                           \
                          ((uint8)(Bytevalue) | (uint8)((uint8)(IrqMode) << 6))
#define Gtm_lIncludeIntMode16Bit(Intvalue, IrqMode)                           \
                       ((uint16)(Intvalue) | (uint16)((uint16)(IrqMode) << 14))
#define Gtm_lIncludeIntMode32Bit(Wordvalue, IrqMode)                          \
                      ((uint32)(Wordvalue) | (uint32)((uint32)(IrqMode) << 30))

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/


/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/


/*******************************************************************************
**                      Global Funtion Declarations                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/
#define MCU_START_SEC_POSTBUILDCFG
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

static const Gtm_ClockSettingType Gtm_kClockSetting0 = 
{
  0x00955559U,
  {
    0x0000004fU,
    0x00000004U,
    0x0000000fU,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
  },

    0x0U,

  {
    {  0x00000001U,  0x00000001U  },
    {  0x00000001U,  0x00000001U  },
    {  0x00000001U,  0x00000001U  }
  }
};

static const Gtm_GeneralConfigType Gtm_kGeneralConfig0 =
{
  0x0001U,
  Gtm_lIncludeIntMode8Bit(0U,GTM_INTERRUPT_PULSE_NOTIFY_MODE)
  };


static const Gtm_TomTgcConfigGroupType Gtm_kTomTgcConfigGroup0[] =
{
  {

      0x0000U,
      0x0000U,
      0x0008U,
      0x0000U,
      0x0000U,
      0x00000000U,      
  },

};

static const Gtm_TomChannelConfigType Gtm_kTomChannelConfig0[]=
{
  {
    0x3U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
  },     


};
static const Gtm_TomTgcConfigType Gtm_kTomTgcConfig0[] =
{
  {
    0x0U,
    0x0U,
    &Gtm_kTomTgcConfigGroup0[0]
  },

};

static const Gtm_TomConfigType Gtm_kTomConfig0[] = 
{
  {
    GTM_DRIVER_GPT,
    GTM_INTERRUPT_LEVEL_MODE,
    Gtm_lTimerCtrlValue(0x0U,GTM_FIXED_CLOCK_1),
    NULL_PTR

  },


  {
    GTM_DRIVER_COMPLEX,
    GTM_INTERRUPT_PULSE_NOTIFY_MODE,
    Gtm_lTimerCtrlValue(0x0U,GTM_FIXED_CLOCK_1),
    &Gtm_kTomChannelConfig0[0]
  },



};

static const Gtm_ModUsageConfigType Gtm_kModUsage0 =
{  
   /*TIM Module Usage */
       {
      GTM_TIM_CH_NOT_USEDBY_ICU,
      GTM_TIM_CH_NOT_USEDBY_ICU,
      GTM_TIM_CH_NOT_USEDBY_ICU,
      GTM_TIM_CH_NOT_USEDBY_ICU,
      GTM_TIM_CH_NOT_USEDBY_ICU,
      GTM_TIM_CH_NOT_USEDBY_ICU,
      GTM_TIM_CH_NOT_USEDBY_ICU,
      GTM_TIM_CH_NOT_USEDBY_ICU,
   },
    {   /*TOM module Usage */
     /*TOM Module 0 Usage*/    {
      0x0U, /*GPT channel no*/
      0x00U, /* GTM configured channel*/
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
    },
     /*TOM Module 1 Usage*/
    {
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
      GTM_TOM_CH_NOT_USEDBY_GPT_PWM,
    },
  }

};




static const uint8 Gtm_kAdcConnections0[GTM_NO_OF_ADC_MODULES] = 
{
  0x00U,  0x00U,  0x00U,  0x00U,
};


static const Gtm_PortConfigType Gtm_kPortConfig0 =
{
  {
    0x00000000U,
  },
  {
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
  }
};

static const Gtm_ModuleConfigType Gtm_kModuleConfig0 =
{

  GTM_SLEEP_ENABLE,  /* Module Sleep Mode */
  1U,  /* Global Clock Configuration - Numerator */
  1U,/* Global Clock Configuration - Denominator */
  
  0xFFFFFFFFU,  /* Access Enable 0 */
  0UL,  /* Access Enable 1 */
  {0U},
  {0U},  /* TIM Usage */  
  NULL_PTR,  /* TIM Configuration Pointer */
  {0x01U},  /* TOM TGC Usage */
  &Gtm_kTomTgcConfig0[0],  /* TOM TGC Configuration Pointer */
  {0x0000000eU,
   0x00000000U,
   },
  {0x00000003U
  },  /* TOM Usage */
  &Gtm_kTomConfig0[0],  /* TOM Configuration Pointer */

  &Gtm_kModUsage0, /* Configuration for GTM Usage by other modules */
  &Gtm_kGeneralConfig0,  /* GTM General Configuration */
  NULL_PTR,  /* TBU Configuration Pointer */

  &Gtm_kAdcConnections0[0],  /* Adc Connections Configuration Pointer*/
  {
    0x0000U,  /* Ttcan Connections Configuration*/
  },
};

const Gtm_ConfigType Gtm_ConfigRoot[GTM_CONFIG_COUNT]  =
{
  {
    /*  GTM Module Clock Settings  */
    &Gtm_kClockSetting0,
    /*  Pointer to Gtm Configuration structure  */
    &Gtm_kPortConfig0,
    &Gtm_kModuleConfig0,
  },
};
#define MCU_STOP_SEC_POSTBUILDCFG
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

