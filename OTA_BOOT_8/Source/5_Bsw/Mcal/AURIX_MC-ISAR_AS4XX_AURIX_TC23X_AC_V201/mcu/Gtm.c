
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
**  $FILENAME   : Gtm.c $                                                    **
**                                                                           **
**  $CC VERSION : \main\dev_tc23x\34 $                                       **
**                                                                           **
**  $DATE       : 2017-04-04 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains                                         **
**                functionality of <> driver.                                **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/

/*  TRACEABILITY: [cover parentID=DS_NAS_GTM_PR1086,
DS_NAS_GTM_PR73,DS_NAS_GTM_PR228,SAS_NAS_ALL_PR70,DS_NAS_GTM_PR128,
SAS_NAS_ALL_PR470,SAS_NAS_ALL_PR630_PR631,SAS_NAS_ALL_PR749,
SAS_NAS_ALL_PR1652,DS_NAS_GTM_PR115]
[/cover]
*/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Own header file, this includes own configuration file also */
#include "Gtm.h"
#include "Gtm_Local.h"
/* Include Irq definitions for IRQ */
#include "IfxSrc_reg.h"


#if (GTM_MCUSAFETY_ENABLE == STD_ON)
#include "SafetyReport.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

#define GTM_VALUE_ZERO (0U)
#define GTM_ONE (1)
#define GTM_ZERO (0)

#define GTM_ZERO_U (0U)
#define GTM_ONE_U (1U)
#define GTM_TWO_U (2U)
#define GTM_SIXTEEN_U (16U)
#define GTM_EIGHT_U (8U)
#define GTM_THREE_UL (3UL)
#define GTM_GET_IRQ_VAL_8 (0x3FUL)


/* Get the IRQ Mode Information from a 8bit variable */
#define GTM_GET_MODE_VAL_8 (0xC0UL)
#define GTM_GET_EIRQ_VAL_8 (0x1E00UL)

/* Bits to shift to get the mode information to LSB for a 8 bit variable */
#define GTM_SHIFT_TO_LSB_8 (6)
#define GTM_SHIFT_EIRQ_TO_LSB_8 (9U)
/* Get the IRQ Enable Information from a 32 bit variable */
#define GTM_GET_IRQ_VAL_32 (0x3FFFFFFFUL)
/* Get the IRQ Mode Information from a 32 bit variable */
#define GTM_GET_MODE_VAL_32 (0xC0000000UL)
/* Bits to shift to get the mode information to LSB for a 32 bit variable */
#define GTM_SHIFT_TO_LSB_32 (30U)


#define GTM_TOM_CHAN_PER_SRN (2U)
#define GTM_GET_INT_STATUS (3UL)
#define GTM_GET_TIM_INT_STATUS (1UL)


#define GTM_CLEAR_INTERRUPT (3UL)
#define GTM_CLEAR_TIM_INTERRUPT (0x3FUL)

#define GTM_TOM_TGC_GET_FUPD (0xFFFFUL)
#define GTM_TOM_TGC_CLEAR_FUPD (0x5555UL)
#define GTM_TOM_CHAN_ENABLE (3UL)

#define GTM_CCU1 (1U)

#define GTM_TIM_GLITCH_POS (5U)

#define GTM_CMU_CLK_ENABLE (2UL)
#define GTM_CMU_REG_CLK_ENABLE (3UL)
#define GTM_CMU_START_FROM_EXTCLK (16U)

#define GTM_DISABLE_ALL_CLOCKS (0x00555555U)

#define GTM_TBU_CONFIGURED (5U)

#define GTM_BITS_IN_U32 (32U)
#define GTM_TBU_GET_LOWER_NIBBLE  (0x0FUL)

/* Get the CMU EIRQ Enable Information from a 32 bit variable */
#define GTM_CMU_GET_EIRQ_VAL_32 (0x00FFFFFFUL)
/* Get the CMU Fixed clock control line Info from a 32 bit variable */
#define GTM_GET_FIXED_CLK_VAL (0x0FUL)

#define GTM_CMU_NO_OF_CLOCKS (8U)
#define GTM_CMU_NO_OF_EXT_CLOCKS (3U)
#define GTM_CMU_TOTAL_CLOCKS (12U)


/* Address of first register of ICM from where the corresponding Sub-Modules
   status are reported */
#define GTM_TIM_ICM_BASE_ADDRESS (&GTM_ICM_IRQG_2)
#define GTM_TOM_ICM_BASE_ADDRESS (&GTM_ICM_IRQG_6)

/* Number of Modules status that are reported per ICM register */
#define GTM_TOM_MODULES_IN_ICM_REG  (2U)
#define GTM_TIM_MODULES_IN_ICM_REG  (4U)

/* This signifies that number of TOM/TIM module definitions present in a
   single element of GtmTomUsage/GtmTimUsage parameter */
#define GTM_TOM_MODS_IN_UINT32 (GTM_BITS_IN_U32/GTM_CHANNELS_PER_TOM_MODULE)
#define GTM_TIM_MODS_IN_UINT32 (GTM_BITS_IN_U32/GTM_CHANNELS_PER_TIM_MODULE)

#define GTM_MCAL_TIMEOUT_VAL (150000U)
#define GTM_INVALID_NUMBER (0xFFU)

/*Xor compare value for initcheck*/
#define GTM_32_XOR_COMPARE_VAL (0xFFFFFFFFUL)
#define GTM_GET_EN_REG_BIT (0x00555555UL)
#define GTM_ENDIS_CLR_MASK          (0x0000FFFFUL)
#define GTM_OUTEN_CLR_MASK          (0x0000FFFFUL)
#define GTM_FUPD_CLR_MASK          (0x0000FFFFUL)
#define GTM_KERNEL_RESET_DELAY (0x100U)

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/** CMU object */
typedef Ifx_GTM_CMU_CLK0_5 Gtm_CmuClkCtrl;
typedef Ifx_GTM_CMU_ECLK_NUM Gtm_CmuEclkNumType;
typedef Ifx_GTM_CMU_ECLK_DEN Gtm_CmuEclkDenType;

typedef struct
{
  Gtm_CmuEclkNumType CmuEclkNum;
  Gtm_CmuEclkDenType CmuEclkDen;
}Gtm_CmuEclkType;


typedef volatile struct
{
  /* CMU Clock Enable Register */
  Ifx_GTM_CMU_CLK_EN CLK_EN;
  /* CMU Global Clock Control Numerator Register */
  Ifx_GTM_CMU_GCLK_NUM GCLK_NUM;
  /* CMU Global Clock Control Denominator Register */
  Ifx_GTM_CMU_GCLK_DEN GCLK_DEN;
  /* CMU Control For Clock Register */
  Gtm_CmuClkCtrl ClkCtrl[GTM_CMU_NO_OF_CLOCKS];
  /* CMU External Clock Control Registers */
  Gtm_CmuEclkType CmuEclk[GTM_CMU_NO_OF_EXT_CLOCKS];
  /* Fixed clock divider value  */
  uint32 GtmFxdClkControl;
} Gtm_CmuType;


#define GTM_START_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#if (defined(GTM_TOM_MODULE_USED)) 
/* To get the Source that has triggered Interrupt */
IFX_LOCAL_INLINE uint32 Gtm_lGetIntSource(uint32 Value, uint8 Index)
{
  return((uint32)(Value & (GTM_BIT_SET << Index )));
}
#endif
/* for #if (defined(GTM_TOM_MODULE_USED))  */

/* To identify if a sub-module is configured or not */
IFX_LOCAL_INLINE boolean Gtm_lGetUnused8(uint8 CtrlVal)
{
  return(((CtrlVal & GTM_SETUNUSED8) > GTM_VALUE_ZERO)?\
                                                  (boolean)TRUE:(boolean)FALSE);
}


/* To get the value at bitposition of the input "Value" */
IFX_LOCAL_INLINE uint8 Gtm_lGetBit8(uint8 Value, uint8 BitPosition)
{
  return((uint8)(((Value & (uint8)(GTM_ONE_U << BitPosition))> GTM_VALUE_ZERO)
                                                            ?GTM_ONE:GTM_ZERO));
}

/* Get the Driver that uses the particular channel */
IFX_LOCAL_INLINE uint8 Gtm_lGetModuleIndex(uint32 ModUsage, uint8 Channel)
{
  return((uint8)((ModUsage & (GTM_THREE_UL << (GTM_TWO_U * Channel))) >>
                                                        (GTM_TWO_U * Channel)));
}

IFX_LOCAL_INLINE void Gtm_lCmuClockConfigure(void);

#ifdef GTM_TIM_MODULE_USED

IFX_LOCAL_INLINE uint32 Gtm_lGetTimIrqStatus(uint8 ModuleNo,\
                                                          uint8 ChannelNumber);
#endif
IFX_LOCAL_INLINE void Gtm_lCmuExtClkConfigure(void);

IFX_LOCAL_INLINE void  Gtm_lCmuConfClkConfigure(void);

IFX_LOCAL_INLINE void Gtm_lSaveTgcStatus(Ifx_GTM_TOM_TGC_TYPE *TomTgcRegPtr);

IFX_LOCAL_INLINE void Gtm_lTomHostTrigger(void);

IFX_LOCAL_INLINE void Gtm_lTomTgcConfigure(void);

IFX_LOCAL_INLINE void Gtm_lPortConfig(void);

IFX_LOCAL_INLINE void Gtm_lTbuConfig(void);

IFX_LOCAL_INLINE void Gtm_lTtcanConnectionsConfig(void);

IFX_LOCAL_INLINE Std_ReturnType Gtm_lEnableGtm(void);


#if (GTM_SFR_RESET_ENABLE == STD_ON)
IFX_LOCAL_INLINE void Gtm_lResetKernelInit(void);

#endif

#if (GTM_DEINIT_API_ENABLE == STD_ON)

IFX_LOCAL_INLINE void Gtm_lResetKernelDeInit(void);

#endif

IFX_LOCAL_INLINE void Gtm_lTomConfigure(void);


#if (defined(GTM_TOM_MODULE_USED))
IFX_LOCAL_INLINE uint32 Gtm_lGetTomIrqStatus(uint8 ModuleNo,
                                            uint8 ChannelNumber);
#endif



#define GTM_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*
 * To be used for global or static variables (32 bits) that are initialized
 * after every reset (the normal case)
   Inclusion of MemMap.h
*/

#define GTM_START_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

const Gtm_ConfigType *Gtm_kConfigPtr;

#define GTM_STOP_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

#if ((GTM_NO_OF_TIM_CH_CONF_NOTIF > 0U)  ||(GTM_NO_OF_TOM_CH_CONF_NOTIF > 0U))

#define GTM_START_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"


/* Declaration of GTM Notification Link time configuration */
/*IFX_MISRA_RULE_08_07_STATUS= Reports Misra violation 8.7 under specific 
configuration.
If only TOM  or only TIM notification is configured then 
Gtm_kNotifConfig0 will be used only in one function body (ISR).*/
extern const Gtm_NotificationConfigType Gtm_kNotifConfig0;

#ifdef IFX_GTM_DEBUG

extern volatile uint32 TestGtm_DebugMask01;
extern volatile uint32 TestGtm_DebugMask02;

#endif
#define GTM_STOP_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

#endif


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
**                      Global Function Declarations                          **
*******************************************************************************/
#if (GTM_GPT_MODULE_USED == STD_ON)

#define GPT_START_SEC_CALLOUT_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
** Syntax : void Gpt_Isr (uint8 ModChannelNo)                                 **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  ModChannelNo- Gpt channel number which called the        **
**            TOM channel interrupt                                           **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     This ISR services the interrupt generated from GTM to GPT**
*******************************************************************************/
extern void Gpt_Isr(uint8 ModChannelNo);

#define GPT_STOP_SEC_CALLOUT_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

#endif

#if (GTM_PWM_MODULE_USED == STD_ON)

#define PWM_17_GTM_START_SEC_CALLOUT_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
** Syntax : void Pwm_17_Gtm_Isr (uint8 ModChannelNo)                          **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  ModChannelNo- PWM channel number which called the        **
**            TOM channel interrupt                                           **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     This ISR services the interrupt generated from GTM to PWM**
*******************************************************************************/
extern void Pwm_17_Gtm_Isr(uint8 ModChannelNo);

#define PWM_17_GTM_STOP_SEC_CALLOUT_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

#endif


#if (GTM_ICU_MODULE_USED == STD_ON)

#define ICU_17_GTMCCU6_START_SEC_CALLOUT_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"
/*******************************************************************************
** Syntax : void Icu_17_GtmCcu6_Eru_Isr (uint8 ModChannelNo)                  **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  ModChannelNo- Icu channel number which called the        **
**            TOM channel interrupt                                           **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     This ISR services the interrupt generated from GTM to ICU**
*******************************************************************************/
extern void Icu_17_GtmCcu6_Isr(uint8 ModChannelNo);

#define ICU_17_GTMCCU6_STOP_SEC_CALLOUT_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

#endif

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*Memory Map of the GTM Code*/
#define GTM_START_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_GTM_PR1088,DS_NAS_GTM_PR1089,
  DS_NAS_GTM_PR1090,DS_NAS_GTM_PR1091,DS_MCAL_GTM_0002,DS_NAS_EP_GTM_PR3018]
** Syntax : Std_ReturnType  Gtm_Init(const Gtm_ConfigType *ConfigPtr)         **
** [/cover]                                                                   **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non reentrant                                            **
**                                                                            **
** Parameters (in):  ConfigPtr - Pointer to configuration set                 **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value:     E_OK if GTM is initialized successfully                  **
**                   E_NOT_OK if GTM is not initialized successfully          **
**                                                                            **
** Description :     This routine initializes the GTM module.                 **
**                   This routine must be called before executing ICU / PWM / **
**                   GPT driver.                                              **
**                   The intention of this function is to make the            **
**                   configuration settings for GTM driver                    **
**                   also enable Interrupt Request for all resources that is  **
**                   used across ICU/PWM/GPT and GTM modules.                 **
**                   Clock Setting for GTM module clock and clock bus         **
**                   configuration will be done in this                       **
**                   function.                                                **
**                                                                            **
*******************************************************************************/
Std_ReturnType Gtm_Init(const Gtm_ConfigType *ConfigPtr)
{
  Std_ReturnType GtmEnableStatus= E_NOT_OK;
  const Gtm_GeneralConfigType *GeneralConfigPtr; /* Pointer to General Config */
  uint32 TimeOut = GTM_MCAL_TIMEOUT_VAL;

  Gtm_kConfigPtr = ConfigPtr;

  /*************************** Enable GTM *************************************/
  GtmEnableStatus = Gtm_lEnableGtm();
  /* Check if Initialization was successful*/
  if(GtmEnableStatus == E_OK)
  {
    /********************* Enable GTM Global Clock ****************************/

    Gtm_lCmuClockConfigure();

    /********************* General Initialization***************** ************/
    /* Get the general configuration */
    GeneralConfigPtr = ConfigPtr->GtmModuleConfigPtr->GtmGeneralConfigPtr;

    GTM_SFR_INIT_USER_MODE_WRITE32((GTM_CTRL.U),\
       ((uint32)(GeneralConfigPtr->GtmCtrlValue)));

    GTM_SFR_INIT_USER_MODE_WRITE32((GTM_IRQ_EN.U), \
       ((uint32)((uint32)GeneralConfigPtr->GtmIrqEnable & \
         GTM_GET_IRQ_VAL_8)));

    GTM_SFR_INIT_USER_MODE_WRITE32((GTM_IRQ_MODE.U),\
       ((uint32)(((uint32) (GeneralConfigPtr->GtmIrqEnable)& \
         GTM_GET_MODE_VAL_8) >> GTM_SHIFT_TO_LSB_8)));

    GTM_SFR_INIT_USER_MODE_WRITE32((GTM_EIRQ_EN.U),\
       ((uint32)(((uint32)(GeneralConfigPtr->GtmIrqEnable) & \
          GTM_GET_EIRQ_VAL_8) >> GTM_SHIFT_EIRQ_TO_LSB_8)));


    /* SRN Enable */
    GTM_SFR_INIT_MODIFY32((SRC_GTMAEIIRQ.U),GTM_SRC_CLEAR_MASK,\
                            GTM_BIT_SRE_MASK)

    /****************** TIM Initialization ************************************/
    Gtm_lTimConfigure();

    /****************** TOM Initialization ************************************/

    Gtm_lTomConfigure();

    /****************** PORT Initialization ***********************************/
    Gtm_lPortConfig();

    /****************** TBU Initialization ************************************/
    Gtm_lTbuConfig();

    /******************** ADC Connections Initialization **********************/
    Gtm_lAdcConnectionsConfig();

    /******************** TTCAN Connections Initialization ********************/
    Gtm_lTtcanConnectionsConfig();

    /******************** Access Enable Initialization ************************/

    GTM_SFR_INIT_RESETSAFETYENDINIT_TIMED(TimeOut);
    GTM_SFR_INIT_WRITE32((GTM_ACCEN0.U),\
                          (ConfigPtr->GtmModuleConfigPtr->GtmAccessEnable0));
    GTM_SFR_INIT_SETSAFETYENDINIT_TIMED();
 }

 return (GtmEnableStatus);

}/* End of Gtm_Init */

#if (GTM_DEINIT_API_ENABLE == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID = DS_NAS_HE2_GTM_PR3021,
                                                        DS_NAS_EP_GTM_PR3021] **
** Syntax : void Gtm_DeInit (void)                                            **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID:       None                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       Non Reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :  This service shall de-initialize GTM hardware peripheral    **
** and global variables                                                       **
**                                                                            **
*******************************************************************************/
void Gtm_DeInit(void)
{
  volatile uint32  ReadBack;
  /* De-Init all SRC functions*/
  Gtm_lResetGtmSRCReg(); 

  /* Reset GTM kernel */
  Gtm_lResetKernelDeInit();

  /* Disable GTM Module */
  GTM_SFR_DEINIT_RESETENDINIT();

  GTM_SFR_DEINIT_MODIFY32((MODULE_GTM.CLC.U),GTM_CLC_CLR_MASK,GTM_ONE_U)

  /* Read back the CLC register */
  ReadBack = GTM_SFR_DEINIT_USER_MODE_READ32((MODULE_GTM.CLC.U));

  GTM_SFR_DEINIT_SETENDINIT();

  /* Reset the global config pointer to NULL */
  Gtm_kConfigPtr = NULL_PTR;

  UNUSED_PARAMETER(ReadBack)

}/* End of Gtm_DeInit() */

#endif
/* #if (GTM_DEINIT_API_ENABLE == STD_ON) */

/*******************************************************************************
** Traceability : [cover parentID = DS_MCAL_GTM_0403_01, DS_MCAL_GTM_0402_01] **
** Syntax : void Gtm_IsrTomModule(uint8 ModuleNo, uint8 ChannelNumber)        **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  ModuleNo - TOM Module Number                             **
**                   ChannelNumber - Channel number (0,2,4,6,8,10,12,14)      **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     All TOM Interrupt sources are mapped to this function.   **
**                   On an interrupt request from TOM isr, channel number and **
**                   module number are passed as parameter. Since 2 interrupt **
**                   sources are mapped  to one Interrupt node, the parameter **
**                   to this function will be an even number only.            **
*******************************************************************************/
void Gtm_IsrTomModule(uint8 ModuleNo, uint8 ChannelNumber)
{
#ifdef GTM_TOM_MODULE_USED

  uint8 ChanIndex;          /* Maintains an index to the channel to check     */
  uint8 ModChNum; /*Holds the GPT/PWM channel for ModuleNo and ChannelNumber */
  uint16 NotifRegVal; /* Holds the value of IRQ NOTIF reg before clearing reg */
  uint32 IrqStatus;       /* Conveys which channel has triggered the Interrupt*/
  uint32 TomModuleUsage;  /*Holds the value of TOM module usage for ModuleNo*/
  #if (GTM_NO_OF_TOM_CH_CONF_NOTIF > 0U)
  Gtm_NotificationPtrType NotifFn;  /* Notification function */
  #endif

  Ifx_GTM_TOM_CH_TYPE *ChannelPtr; /* Pointer to the TOM channel Register  */
  /* Module usage configuration pointer*/
  const Gtm_ModUsageConfigType *ModUsageConfigPtr;

  /* Get the GtmModUsageConfigPtr config pointer*/
  ModUsageConfigPtr =(Gtm_kConfigPtr->
                                  GtmModuleConfigPtr->GtmModUsageConfigPtr);
  /* Get the module channel no corresponding to ModuleNo and ChannelNumber
  for GTM compex driver - Notification index in array Gtm_kNotifConfig0[]
  for GPT - Gpt channel no
  for PWM - PWM channel no
  */

  /* Get the usage information for TOM module*/
  TomModuleUsage = (uint32)Gtm_kConfigPtr->GtmModuleConfigPtr->                \
                                                  GtmTomModuleUsage[ModuleNo];

    /* Initialize ChanIndex to the number of channels mapped to one
       Interrupt Node */
    ChanIndex = GTM_TOM_CHAN_PER_SRN;
    /* Get the Irq Group status to detect the channel that has
    triggered the ISR*/
    IrqStatus = Gtm_lGetTomIrqStatus(ModuleNo, ChannelNumber);
    /* Loop through both the channels */
    do
    {
      ChanIndex--;
      /* Check which the channel has triggered interrupt
                                         ( 2 TOM channel shares the same SRN)*/
      if(Gtm_lGetIntSource(IrqStatus, ChanIndex))
      {
        /* Get the Channel Register Pointer */
        ChannelPtr = &((*(Ifx_GTM_TOMx*)(volatile void *)(MODULE_GTM.TOM)).
                                TOM_CH[ModuleNo].CH[ChannelNumber + ChanIndex]);
        /* Clear the status for receiving other interrupts */
        NotifRegVal = (uint16) ChannelPtr->IRQ_NOTIFY.U;
        ChannelPtr->IRQ_NOTIFY.U = GTM_CLEAR_INTERRUPT;

        ModChNum = ModUsageConfigPtr->                                         \
                              Gtm_TomUsage[ModuleNo][ChannelNumber + ChanIndex];

        #if (GTM_NO_OF_TOM_CH_CONF_NOTIF > 0U)

        if (Gtm_lGetModuleIndex(TomModuleUsage,ChannelNumber + ChanIndex) ==   \
                                                        GTM_TOM_DRIVER_COMPLEX)
        {
          /*Get notification number from Lcfg */
          NotifFn = Gtm_kNotifConfig0.TomNotifUsage[ModChNum];
          /* Call the notification function if configured */
          NotifFn(GTM_TOM_MODULE, ModuleNo,                                \
                                        ChannelNumber + ChanIndex,NotifRegVal);
        }
        #else
          UNUSED_PARAMETER(NotifRegVal)
        #endif

        #if (GTM_GPT_MODULE_USED == STD_ON)
         if(Gtm_lGetModuleIndex(TomModuleUsage,ChannelNumber + ChanIndex) == \
                                                                 GTM_DRIVER_GPT)
         {
           Gpt_Isr(ModChNum);
         }
        #endif
        #if (GTM_PWM_MODULE_USED == STD_ON)
         if(Gtm_lGetModuleIndex(TomModuleUsage,ChannelNumber + ChanIndex) ==  \
                                                             GTM_DRIVER_PWM_MOD)
         {
           Pwm_17_Gtm_Isr(ModChNum);
         }
        #endif
      }
    /* Span through both the channels */
    }while(ChanIndex > GTM_ZERO_U);
#else /* included to remove GNU compilation warning */
  UNUSED_PARAMETER(ModuleNo)
  UNUSED_PARAMETER(ChannelNumber)
#endif
/*#ifdef GTM_TOM_MODULE_USED*/
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_HE2_GTM_PR2935_1, DS_MCAL_GTM_0403_02,
                                                         DS_MCAL_GTM_0402_02] **
** Traceability : [cover parentID = DS_MCAL_GTM_0403_03, DS_MCAL_GTM_0402_03] **
** Syntax : void Gtm_IsrTimModule(uint8 ModuleNo, uint8 ChannelNumber)        **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  ModuleNo: TIM Module Number                              **
**                   ChannelNumber - Channel number (0 to 7)                  **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     All TIM Interrupt sources are mapped to this function.   **
**                   On an interrupt request from TIM isr, channel number and **
**                   module number are passed as parameter. Notification      **
**                   function if configured, will be called                   **
*******************************************************************************/
void Gtm_IsrTimModule(uint8 ModuleNo, uint8 ChannelNumber)
{
#ifdef GTM_TIM_MODULE_USED

  /* Following Holds the GPT/PWM channel for ModuleNo and ChannelNumber */
  uint8 ModUsageChno;
  uint8 ModChNum;  /*Holds the GPT/PWM channel for ModuleNo and ChannelNumber */
  uint16 NotifRegVal; /* Holds the value of IRQ NOTIF reg before clearing reg */
  uint32 TimModuleUsage;  /*Holds the value of TIM module usage for ModuleNo*/
  Ifx_GTM_TIM_CH_TYPE *TimChannelRegPtr; /* Pointer to TIM channel Register */
  /*Module usage configuration pointer*/
  const Gtm_ModUsageConfigType *ModUsageConfigPtr;
  #if (GTM_NO_OF_TIM_CH_CONF_NOTIF > 0U)
  Gtm_NotificationPtrType NotifFn;  /* Notification function */
  #endif

  /* Check if current interrupt is valid interrupt or spurious interrupt */
  if(Gtm_lGetTimIrqStatus( ModuleNo, ChannelNumber) == GTM_ONE_U)

  {
    /* Get the location of TIM channel registers */
    /*IFX_MISRA_RULE_11_05_STATUS=volatile in terms of pointer access.
    Permitted for special function registers.*/
    TimChannelRegPtr = &((*(Ifx_GTM_TIMx*)(volatile void *)(MODULE_GTM.TIM)).\
                                          CH_TIM[ModuleNo].CH[ChannelNumber]);

    /* Clear the Notify for receiving new interrupt requests */
    NotifRegVal = (uint16) TimChannelRegPtr->CH_IRQ_NOTIFY.U;
    TimChannelRegPtr->CH_IRQ_NOTIFY.U = GTM_CLEAR_TIM_INTERRUPT;
  /* Get the TIM module usage*/
  TimModuleUsage = (uint32)Gtm_kConfigPtr->GtmModuleConfigPtr->                \
                                          GtmTimModuleUsage[ModuleNo];
  /* Get the module usage config pointer */
  ModUsageConfigPtr = (Gtm_kConfigPtr->                                        \
                                     GtmModuleConfigPtr->GtmModUsageConfigPtr);
  /* Retrieve the correspoding module ch no. i.e. for ICU - ICU channel no
   and for GTM complex driver Notificaiton no*/
  ModChNum = ModUsageConfigPtr->Gtm_TimUsage[ChannelNumber];
  ModUsageChno = ChannelNumber;

  #if (GTM_NO_OF_TIM_CH_CONF_NOTIF > 0U)

  if(Gtm_lGetModuleIndex(TimModuleUsage ,ModUsageChno) ==                      \
                                                        GTM_TIM_DRIVER_COMPLEX)
  {
    /* If interrupt has been raised for an unconfigured channel then 0xFF
       will be returned */
    NotifFn = Gtm_kNotifConfig0.TimNotifUsage[ModChNum];
    /* Call the notification function if configured */
    NotifFn(GTM_TIM_MODULE, ModuleNo, ChannelNumber, NotifRegVal);
  }
  #else
    UNUSED_PARAMETER(NotifRegVal)
  #endif

    #if (GTM_ICU_MODULE_USED == STD_ON)
    /* Check of the interrupt is configured by ICU driver */
    if(Gtm_lGetModuleIndex(TimModuleUsage,ModUsageChno) == GTM_DRIVER_ICU)
    {
      Icu_17_GtmCcu6_Isr(ModChNum);
    }
    #endif
  }
  #if (GTM_MCUSAFETY_ENABLE == STD_ON)
  else
  {
    /* Current interrupt is spurious intterrupt */
    SafeMcal_ReportError ((uint16)GTM_MCU_MODULE_ID,GTM_MCU_MODULE_INSTANCE,
                     GTM_SID_TIM_ISR,GTM_E_SPURIOUS_INTRPT);
  }
  #endif
  /*  #if (GTM_MCUSAFETY_ENABLE == STD_ON)*/
  #else /* included to remove GNU compilation warning */
    UNUSED_PARAMETER(ModuleNo)
    UNUSED_PARAMETER(ChannelNumber)
  #endif
/*#ifdef GTM_TIM_MODULE_USED*/

}

#if (defined(GTM_TOM_MODULE_USED))

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint32 Gtm_lGetTomIrqStatus(uint8 ModuleNo,      **
**                                                       uint8 ChannelNumber) **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  ModuleNo - TOM Module Number                             **
**                   ChannelNumber - Channel number (0,2,4,6,8,10,12,14)      **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     IrqStatus - Interrupt status of the channels             **
**                                                                            **
** Description :     This function fetches the interrupt status of the channel**
**                   number and the next channel number                       **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Gtm_lGetTomIrqStatus(uint8 ModuleNo,uint8 ChannelNumber)
{
  volatile uint32 RegVal;
  uint32 IrqStatus;
  uint8 RegPos;

  /* Ascertain the position of Interrupt status for the corresponding module
     and channel*/
  RegPos = ((ModuleNo % GTM_TOM_MODULES_IN_ICM_REG) * \
                                   GTM_CHANNELS_PER_TOM_MODULE) + ChannelNumber;

  /* Identify the ICM register corresponding to the Module number */
  /* Identify the ICM register corresponding to the Module number */
  /* MISRA Rule Violation 11.4 and 17.4 Pointer to Pointer conversion*/
  /* Pointer to pointer conversion is performed to simplify the code. Since
     each of the ICM register has a different name, to make this available in a
     single functions, this conversion is performed. This pointer arithmetic
     is desired */


    /*IFX_MISRA_RULE_11_05_STATUS=volatile in terms of pointer access.
    Permitted for special function registers.*/
  /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic is used to efficiently 
   access the SFRs of multiple GTM ICM registers.*/
  RegVal = *(((uint32*)(volatile void *)GTM_TOM_ICM_BASE_ADDRESS) + \
                                         (ModuleNo/GTM_TOM_MODULES_IN_ICM_REG));


  /* Retrieve the IRQ status of the channel number and the next channel */
  IrqStatus = (RegVal & (GTM_GET_INT_STATUS << RegPos)) >> (RegPos);
  return(IrqStatus);
}

#endif
/*  for #if (defined(GTM_TOM_MODULE_USED))   */

/*******************************************************************************
** Syntax : void Gtm_lCmuConfClkConfigure(void)                               **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to initialize CMU module                        **
*******************************************************************************/
IFX_LOCAL_INLINE void  Gtm_lCmuConfClkConfigure(void)
{
  const Gtm_ClockSettingType *ClockSettingPtr;   /* Pointer to Clock Settings */
  uint8  Count;             /* Variable to maintain Loop Count                */



  /* Get Clock Configuration */
  ClockSettingPtr = Gtm_kConfigPtr->GtmClockSettingPtr;


  /* Configure all clocks */
  for(Count = GTM_ZERO_U; Count < GTM_CMU_NO_OF_CLOCKS; Count++)
  {
    if(ClockSettingPtr->GtmClockEnable & \
                 (uint32)(GTM_CMU_CLK_ENABLE << (Count * GTM_CMU_BITS_PER_CLK)))
    {
      /* The clock structure provided by Sfr will cause increase in code size.
         Hence a type Gtm_CmuType has been created and used to provide an
         array approach for CMU clocks */


      /* The CMU structure as per SFR does not support for arrayed approach. To
         reduce the code size, a new type Gtm_CmuType is introduced and is
         typecasted in this case */
      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      GTM_SFR_INIT_USER_MODE_WRITE32((((Gtm_CmuType*)(void*)\
           &(MODULE_GTM.CMU))->ClkCtrl[Count].CTRL.U),\
           (ClockSettingPtr->GtmCmuClkCnt[Count]));

    }

  }
}

/*******************************************************************************
** Syntax : void Gtm_lCmuExtClkConfigure(void)                                **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to initialize CMU module                        **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lCmuExtClkConfigure(void)
{
  const Gtm_ClockSettingType *ClockSettingPtr;   /* Pointer to Clock Settings */
  uint8  Count;             /* Variable to maintain Loop Count                */
  uint32 Regval;

  /* Get Clock Configuration */
  ClockSettingPtr = Gtm_kConfigPtr->GtmClockSettingPtr;

  /* Configure External Clocks         */
  for(Count = GTM_ZERO_U; Count < GTM_CMU_NO_OF_EXT_CLOCKS; Count++)
  {
    if(ClockSettingPtr->GtmClockEnable & \
      (uint32)(GTM_CMU_CLK_ENABLE <<((Count * GTM_CMU_BITS_PER_CLK) + \
      GTM_CMU_START_FROM_EXTCLK)))
    {
      /* The clock structure provided by Sfr will cause increase in code size.
         Hence a type Gtm_CmuType has been created and used to provide an
         array approach for CMU clocks */
      /* MISRA rule violation 11.4 and 1.2 */

      /* The CMU structure as per SFR does not support for arrayed approach. To
         reduce the code size, a new type Gtm_CmuType is introduced and is
         typecasted in this case */

      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      GTM_SFR_INIT_USER_MODE_WRITE32((\
      ((Gtm_CmuType*)(void*)&(MODULE_GTM.CMU))->CmuEclk[Count].CmuEclkNum.U ),\
                              ClockSettingPtr->GtmEclk[Count].CmuEclkNum);
      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      GTM_SFR_INIT_USER_MODE_WRITE32((\
      ((Gtm_CmuType*)(void*)&(MODULE_GTM.CMU))->CmuEclk[Count].CmuEclkNum.U),\
                              ClockSettingPtr->GtmEclk[Count].CmuEclkNum);
      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      GTM_SFR_INIT_USER_MODE_WRITE32((\
      ((Gtm_CmuType*)(void*)&(MODULE_GTM.CMU))->CmuEclk[Count].CmuEclkDen.U),\
                              ClockSettingPtr->GtmEclk[Count].CmuEclkDen);


    }
  }
  
  Regval = ((uint32)(ClockSettingPtr->GtmFxdClkControl) & \
          GTM_GET_FIXED_CLK_VAL);
  
  /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
  GTM_SFR_INIT_USER_MODE_WRITE32((\
  ((Gtm_CmuType*)(void*)&(MODULE_GTM.CMU))->GtmFxdClkControl),Regval);
}

/*******************************************************************************
** Syntax : void Gtm_lCmuClockConfigure(void)                                 **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to initialize CMU module                        **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lCmuClockConfigure(void)
{
  const Gtm_ClockSettingType *ClockSettingPtr;   /* Pointer to Clock Settings */
  uint32 RegTemp;           /* Temporary variable to store the register value */
  uint32 RegTemp1;          /* Temporary variable to store the register value */
  uint32 RegTemp3;          /* Temporary variable to store the register value */
  uint32 RegTemp4;          /* Temporary variable to store the register value */
  uint8  Count;             /* Variable to maintain Loop Count                */
  uint8  EnableClock;       /* To Indicate if clock need to be enabled        */

  /* Get Clock Configuration */
  ClockSettingPtr = Gtm_kConfigPtr->GtmClockSettingPtr;

  /* Check if any new value configured for Numerator or Denominator of the
     Global Clock */

  /* The content of Numerator and Denominator are temporarily taken in local
     variables and used in the if statement below for Misra reasons.          */
  RegTemp3 = GTM_SFR_INIT_USER_MODE_READ32(GTM_CMU_GCLK_NUM.U);
  RegTemp4 = GTM_SFR_INIT_USER_MODE_READ32(GTM_CMU_GCLK_DEN.U);
  RegTemp1 = GTM_ZERO_UL;
  /* Indicate that Clock has been enabled prior */
  EnableClock = GTM_ZERO_U;

  if( (RegTemp3 != Gtm_kConfigPtr->GtmModuleConfigPtr->GtmGclkNum) ||          \
      (RegTemp4 != Gtm_kConfigPtr->GtmModuleConfigPtr->GtmGclkDen))
  {
    RegTemp = GTM_SFR_INIT_USER_MODE_READ32(GTM_CMU_CLK_EN.U);

    /* Clock dividers can be initialized only when the corresponding clocks are
       disabled */
    GTM_SFR_INIT_USER_MODE_WRITE32(GTM_CMU_CLK_EN.U,GTM_DISABLE_ALL_CLOCKS);
    /* Indicate that Clock has been disabled and need to be enabled */
    EnableClock = GTM_ONE_U;
    /* For reliable clock setting, it is recommended to have 2 initializations
       of Numerator followed by a single initialization of Denominator        */
    GTM_SFR_INIT_USER_MODE_WRITE32(GTM_CMU_GCLK_NUM.U ,\
                        Gtm_kConfigPtr->GtmModuleConfigPtr->GtmGclkNum);
    GTM_SFR_INIT_USER_MODE_WRITE32(GTM_CMU_GCLK_NUM.U ,\
                        Gtm_kConfigPtr->GtmModuleConfigPtr->GtmGclkNum);
    GTM_SFR_INIT_USER_MODE_WRITE32(GTM_CMU_GCLK_DEN.U ,\
                        Gtm_kConfigPtr->GtmModuleConfigPtr->GtmGclkDen);
    /* Reenable the Clocks which were enabled earlier. A valid clock enable will
       be read as 11b. For enabling, 10b has to be written, hence the
       conversion is performed as mentioned below */
    for(Count = GTM_ZERO_U; Count < GTM_CMU_TOTAL_CLOCKS; Count++)
    {
      if(( RegTemp & \
                  (GTM_CMU_REG_CLK_ENABLE << (Count * GTM_CMU_BITS_PER_CLK)) ) )
      {
        RegTemp1 |= (GTM_ENABLE_CLK << (Count * GTM_CMU_BITS_PER_CLK));
      }
    }
  }

  if(ClockSettingPtr != NULL_PTR)      /* Configure CMU clocks if configured  */
  {
    /* If clock is enabled, then it needs to be disabled for this activity */
    if(EnableClock == GTM_ZERO_U)
    {
      /* Disable all clock enables  */
      GTM_SFR_INIT_USER_MODE_WRITE32(GTM_CMU_CLK_EN.U,GTM_DISABLE_ALL_CLOCKS);
    }

    /* Invoke the functions to configure Configurable and External Clocks */
    Gtm_lCmuConfClkConfigure();
    Gtm_lCmuExtClkConfigure();


    /* Enable clocks */
    GTM_SFR_INIT_USER_MODE_WRITE32(GTM_CMU_CLK_EN.U ,(unsigned_int)\
                                (RegTemp1 |(ClockSettingPtr->GtmClockEnable)));
    /* Indicate that Clock has been enabled and need not be enabled again */
    EnableClock = GTM_ZERO_U;
  }
  if(EnableClock == GTM_ONE_U)
  {
    GTM_SFR_INIT_USER_MODE_WRITE32(GTM_CMU_CLK_EN.U ,(uint32)RegTemp1);
  }
}



/*******************************************************************************
** Syntax : void Gtm_lSaveTgcStatus(Ifx_GTM_TOM_TGC_TYPE *TomTgcRegPtr)       **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  TomTgcRegPtr - Pointer to TGC Configuration              **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to Save TGC information                         **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lSaveTgcStatus(Ifx_GTM_TOM_TGC_TYPE *TomTgcRegPtr)
{
  uint8 MinorCnt;

  for(MinorCnt = GTM_ZERO_U; MinorCnt < GTM_SIXTEEN_U; MinorCnt++)
  {
    if((GTM_SFR_INIT_USER_MODE_READ32(TomTgcRegPtr->ENDIS_STAT.U) &  \
    (GTM_TOM_CHAN_ENABLE << (MinorCnt * GTM_TOM_BITS_PER_CHAN))) > GTM_ZERO_UL)
    {
      GTM_SFR_INIT_USER_MODE_MODIFY32((TomTgcRegPtr->ENDIS_CTRL.U),\
         GTM_GET_LAST_16BITS,((uint32)\
         (uint32)GTM_TWO_U << (MinorCnt * GTM_TOM_BITS_PER_CHAN)))

    }
    if((GTM_SFR_INIT_USER_MODE_READ32(TomTgcRegPtr->OUTEN_STAT.U) &
       (GTM_TOM_CHAN_ENABLE<<(MinorCnt * GTM_TOM_BITS_PER_CHAN))) > GTM_ZERO_UL)
    {
      GTM_SFR_INIT_USER_MODE_MODIFY32(TomTgcRegPtr->OUTEN_CTRL.U ,
           GTM_GET_LAST_16BITS,((uint32)\
           ((uint32)GTM_TWO_U << (MinorCnt * GTM_TOM_BITS_PER_CHAN))))
    }
  }
}


/*******************************************************************************
** Syntax : void Gtm_lTomHostTrigger(void)                                    **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to provide TOM Host Trigger                     **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lTomHostTrigger(void)
{
  Ifx_GTM_TOM_TGC_TYPE *TomTgcRegPtr;            /* Pointer to TOM TGC Reg    */
  uint8 MajorCnt;           /* Variable to maintain Loop Count                */
  uint8 MinorCnt;           /* Variable to maintain Loop Count                */


  /* Check the Fupd status and invoke a Host Trigger to set the clocks        */
  /* MajorCnt - Count to maintain TOM Module Count            */
  /* MinorCnt - Count to maintain TGC Count                   */
  for(MajorCnt = GTM_ZERO_U; MajorCnt < GTM_NO_OF_TOM_MODULES; MajorCnt++)
  {
    for(MinorCnt = GTM_ZERO_U; MinorCnt < GTM_NO_OF_TGC_PER_MODULE;MinorCnt++)
    {
      TomTgcRegPtr = &(((*(Ifx_GTM_TOMx*)(volatile void *)(MODULE_GTM.TOM)).
                                TOM_TGC[MajorCnt].TGC[MinorCnt]));

      if((GTM_SFR_INIT_USER_MODE_READ32(TomTgcRegPtr->FUPD_CTRL.U) & \
               GTM_TOM_TGC_GET_FUPD) > GTM_ZERO_UL)
      {
        /* Invoke an Host Trigger */
        GTM_SFR_INIT_USER_MODE_MODIFY32(TomTgcRegPtr->GLB_CTRL.U,\
            GTM_GLB_CTRL_CLR_MASK,GTM_BIT_SET)
      }
    }
  }
}


/*******************************************************************************
** Syntax : void Gtm_lTomTgcConfigure(void)                                   **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to initialize TGC's of TOM                      **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lTomTgcConfigure(void)
{
  const Gtm_TomTgcConfigType *TomConfigPtr;      /* Pointer to TOM Config     */
  Ifx_GTM_TOM_TGC_TYPE* TomTgcRegPtr;            /* Pointer to TOM TGC Reg    */
  uint8 TomCnt;             /* Variable to TOM Channel Initialization Count   */
  uint8 MajorCnt;           /* Variable to maintain Loop Count                */
  uint8 MinorCnt;           /* Variable to maintain Loop Count                */
  uint8 ModuleNo;           /* Variable to hold Module Number                 */
  uint8 TgcNumber;          /* Variable to hold TGC Number                    */
  volatile uint32 MajorCountLoopLimit = (((GTM_NO_OF_TOM_MODULES * \
                       GTM_NO_OF_TGC_PER_MODULE)/ GTM_BITS_IN_U32) + GTM_ONE_U);
  /* Count to maintain track of the index in TOM TGC Config      */
  TomCnt = GTM_ZERO_U;

  /* MajorCnt -Count to maintain an index to the GtmTomTgcUsage Array         */
  /* MinorCnt -Count to maintain an index to the channels in
     GtmTomTgcUsage Array*/
  for(MajorCnt = GTM_ZERO_U; MajorCnt < MajorCountLoopLimit; MajorCnt++)
  {
    for(MinorCnt = GTM_ZERO_U; MinorCnt < GTM_BITS_IN_U8 ; MinorCnt++)
    {
      /* Check if the TGC is configured */
      if((Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTomTgcUsage[MajorCnt] &
         (GTM_BIT_SET << MinorCnt)) > GTM_ZERO_UL)
      {
        /* Extract Module Number and Channel Number from the Loop Count       */
        ModuleNo = ((MajorCnt * GTM_BITS_IN_U32) + MinorCnt)/
                     GTM_NO_OF_TGC_PER_MODULE;
        TgcNumber = (MinorCnt) % GTM_NO_OF_TGC_PER_MODULE;

        /* MISRA Rule Violation 17.4
           Pointer arithmetic other than array indexing used
           This cannot be avoided because of Post Build structure */

        /* Get the pointer to the TGC configuration */
        /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic used due to 
     PBConfigStructure and is within allowed range.*/
        TomConfigPtr =                                                         \
              &(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTomTgcConfigPtr[TomCnt]);

        TomCnt++;

        /* Get the Pointer to the TGC registers */
        TomTgcRegPtr = &((*(Ifx_GTM_TOMx*)(volatile void *)
                           (MODULE_GTM.TOM)).TOM_TGC[ModuleNo].TGC[TgcNumber]);


        GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->ACT_TB.U),\
                           TomConfigPtr->GtmTomActTb);
        GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->INT_TRIG.U),\
                           (uint32)(TomConfigPtr->GtmTomIntTrig));

        /* Check if the TGC is configured ie. if the channels for TGC is
           configured for Complex usage */
        if(TomConfigPtr->GtmTomTgcConfigGrpPtr != NULL_PTR)
        {
          GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->FUPD_CTRL.U),\
                           TomConfigPtr->GtmTomTgcConfigGrpPtr->GtmTomFupd);
          GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->OUTEN_CTRL.U),\
            (uint32)(TomConfigPtr->GtmTomTgcConfigGrpPtr->GtmTomOutenCtrl));
          GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->OUTEN_STAT.U),\
                        (TomConfigPtr->GtmTomTgcConfigGrpPtr->GtmTomOutenStat));
          GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->GLB_CTRL.U),\
          (((uint32)TomConfigPtr->GtmTomTgcConfigGrpPtr->GtmTomUpdateEn)\
                                                    << GTM_SIXTEEN_U));
          GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->ENDIS_CTRL.U),\
            (uint32)(TomConfigPtr->GtmTomTgcConfigGrpPtr->GtmTomEndisCtrl));
          GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->ENDIS_STAT.U),\
            (uint32)(TomConfigPtr->GtmTomTgcConfigGrpPtr->GtmTomEndisStat));

        }
      }
    }
  }
}

/*******************************************************************************
** Syntax : void Gtm_lPortConfig(void)                                        **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to initialize Port Interfaces to GTM            **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lPortConfig(void)
{
  const Gtm_PortConfigType *PortConfigPtr;       /* Pointer to Port Config    */
  uint8 Count;
  volatile uint32 GtmToPortToutSelRegCount = GTM_NO_OF_TIM_MODULES;
  /* Port configuration for TIM Channels */
  PortConfigPtr = Gtm_kConfigPtr->GtmPortConfigPtr;
  if(Gtm_kConfigPtr->GtmPortConfigPtr != NULL_PTR)
  {
    for(Count = GTM_ZERO_U ; Count < GtmToPortToutSelRegCount ; Count++)
    {
      GTM_SFR_INIT_WRITE32((MODULE_GTM.INOUTSEL.TIM[Count].INSEL.U),\
                                   PortConfigPtr->TimInSel[Count]);
    }
    /* Port configuration for TOM Channels */
    for(Count = GTM_ZERO_U ; Count < GTM_NO_OF_TOUTSEL_REGISTERS ; Count++)
    {
      GTM_SFR_INIT_WRITE32((MODULE_GTM.INOUTSEL.T.OUTSEL[Count].U),
                                    PortConfigPtr->ToutSel[Count]);
    }
  }
}

/*******************************************************************************
** Syntax : void Gtm_lTbuConfig(void)                                         **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to initialize TBU                               **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lTbuConfig(void)
{
  const Gtm_TbuConfigType *TbuConfigPtr;         /* Pointer to Tbu Config     */
  volatile Gtm_TbuType *GtmTbu;                  /* TBU Register type         */
  uint8 Count;
  uint32 RegVal;
  /* Check if Tbu need to be Initialized */
  if(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTbuConfigPtr != NULL_PTR)
  {
    /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
    GtmTbu = (volatile Gtm_TbuType*)(void*)&(MODULE_GTM.TBU);

    for(Count =GTM_ZERO_U; Count < GTM_NO_OF_TBU_CHANNELS; Count++)
    {
      /* MISRA Rule Violation 17.4
         Pointer arithmetic other than array indexing used
         This cannot be avoided because of Post Build structure */

     /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic used due to 
     PBConfigStructure and is within allowed range.*/
     TbuConfigPtr=&(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTbuConfigPtr[Count]);
      /* If the Tbu channel need to initialized*/
      if(Gtm_lGetUnused8(TbuConfigPtr->TbuChannelCtrl) == (boolean)FALSE)
      {
        /* Disable the channel if in case enabled prior */
        GTM_SFR_INIT_USER_MODE_WRITE32(GTM_TBU_CHEN.U , \
                                (GTM_BIT_SET << (GTM_BITS_PER_TBU * Count)));
        GTM_SFR_INIT_USER_MODE_WRITE32((GtmTbu->GtmTbuCh[Count].CH_CTRL.U),\
                     (uint32)((uint32)TbuConfigPtr->TbuChannelCtrl &     \
                     GTM_TBU_GET_LOWER_NIBBLE));
        GTM_SFR_INIT_USER_MODE_WRITE32((GtmTbu->GtmTbuCh[Count].CH_BASE.U),\
                      TbuConfigPtr->TbuBaseValue);
        RegVal = (uint32)(Gtm_lGetBit8(TbuConfigPtr->TbuChannelCtrl,\
                          GTM_TBU_CONFIGURED));
        RegVal = (uint32)(RegVal << ((GTM_BITS_PER_TBU * Count) + GTM_ONE_U));
        GTM_SFR_INIT_USER_MODE_WRITE32((GTM_TBU_CHEN.U),RegVal);
      }
    }
  }
}
/*******************************************************************************
** Syntax : void Gtm_lTtcanConnectionsConfig(void)                            **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to initialize TTCAN Connections                 **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lTtcanConnectionsConfig(void)
{
  GTM_SFR_INIT_USER_MODE_WRITE32((MODULE_GTM.INOUTSEL.CAN.OUTSEL.U),  \
             (uint32)(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTtcanTriggers[0]));
}

/*******************************************************************************
** Syntax : Std_ReturnType Gtm_lEnableGtm(void)                               **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     E_OK if GTM clock is enabled successfully                **
**                   E_NOT_OK if GTM clock is not enabled successfully        **
**                                                                            **
** Description :     Function to Enable GTM                                   **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Gtm_lEnableGtm(void)
{
  Std_ReturnType GtmEnableStatus= E_NOT_OK;
  uint32 GtmClockEnableStatus;

  GTM_SFR_INIT_RESETENDINIT();

  /* Enable the GTM Module */
  GTM_SFR_INIT_MODIFY32(MODULE_GTM.CLC.U,GTM_CLC_DISR_CLR_MASK, GTM_ZERO_U)

  GTM_SFR_INIT_SETENDINIT();

  #ifdef IFX_GTM_DEBUG
    GtmClockEnableStatus = (((GTM_SFR_INIT_USER_MODE_READ32(\
                               MODULE_GTM.CLC.U)>>GTM_ONE_U)&GTM_ONE_U)\
                               | TestGtm_DebugMask02);
  #else
    GtmClockEnableStatus = (((uint32)(GTM_SFR_INIT_USER_MODE_READ32(\
                                MODULE_GTM.CLC.U))>>GTM_ONE_U)&GTM_ONE_U);
  #endif

  /* Check if GTM clock is enabled successfully*/
  if(GtmClockEnableStatus == GTM_ZERO_U)
  {
    volatile uint32 ReadBack;

    #if (GTM_SFR_RESET_ENABLE == STD_ON)

    /* Reset the GTM kernel */
    Gtm_lResetKernelInit ();
    #endif

   GTM_SFR_INIT_RESETENDINIT();
    /* Enable GTM Sleep */
    if(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmModuleSleepEnable==              \
                                                               GTM_SLEEP_ENABLE)
    {
      /* Enable Sleep Mode if configured */
      GTM_SFR_INIT_MODIFY32(MODULE_GTM.CLC.U,GTM_CLC_EDIS_CLR_MASK, GTM_ZERO_U)
    }
    else
    {
      GTM_SFR_INIT_MODIFY32(MODULE_GTM.CLC.U,GTM_CLC_CLR_MASK, \
                                                    GTM_CLC_EDIS_SET_MASK)
      /* Disable Sleep Mode if configured */
    }

    GTM_SFR_INIT_SETENDINIT();

    /* Read back the CLC register */
    ReadBack = GTM_SFR_INIT_USER_MODE_READ32(MODULE_GTM.CLC.U);

    /* ReadBack is required to ensure the CLC register modifications */
    UNUSED_PARAMETER(ReadBack)

    /*Set the GTM Status to Enabled */
    GtmEnableStatus = E_OK;

  }

  return (GtmEnableStatus);

}

#if (GTM_SFR_RESET_ENABLE == STD_ON)

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Gtm_lResetKernelInit(void)                  **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to Reset GTM  kernel                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lResetKernelInit(void)
{
  uint32 KernelResetTimeOutCount = GTM_KERNEL_RESET_DELAY;
  volatile uint32 ResetReadBack;
  uint32 ResetStatus;

  /* Reset the kernel */
  GTM_SFR_INIT_RESETENDINIT();

  GTM_SFR_INIT_MODIFY32(GTM_KRST0.U,GTM_KRST0_CLR_MASK,GTM_ONE_U)
  ResetReadBack = GTM_SFR_INIT_USER_MODE_READ32(GTM_KRST0.U);
  GTM_SFR_INIT_WRITE32(GTM_KRST1.U,GTM_ONE_U);
  ResetReadBack = GTM_SFR_INIT_USER_MODE_READ32(GTM_KRST1.U);

  GTM_SFR_INIT_SETENDINIT();


  /* Wait for Reset acknowledgement */
  do
  {
    #ifdef IFX_GTM_DEBUG
    ResetStatus = (((((uint32)GTM_SFR_INIT_USER_MODE_READ32(GTM_KRST0.U))>>\
                   GTM_ONE_U)&GTM_ONE_U)& TestGtm_DebugMask01);
    #else
    ResetStatus = ((((uint32)GTM_SFR_INIT_USER_MODE_READ32(GTM_KRST0.U))>>\
                   GTM_ONE_U)&GTM_ONE_U);
    #endif
    KernelResetTimeOutCount--;
  }while((ResetStatus == GTM_ZERO_U) && \
                                       (KernelResetTimeOutCount > GTM_ZERO_U));

  GTM_SFR_INIT_RESETENDINIT();
  /* Clear the status bit */
  GTM_SFR_INIT_WRITE32(GTM_KRSTCLR.U ,GTM_ONE_U);
  GTM_SFR_INIT_SETENDINIT();

  /* Read back kernel reset clear register */
  ResetReadBack = GTM_SFR_INIT_USER_MODE_READ32(GTM_KRSTCLR.U);
  UNUSED_PARAMETER(ResetReadBack)
}

#endif


#if (GTM_DEINIT_API_ENABLE == STD_ON)

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE void Gtm_lResetKernelDeInit(void)                **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to Reset GTM  kernel                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lResetKernelDeInit(void)
{
  uint32 KernelResetTimeOutCount = GTM_KERNEL_RESET_DELAY;
  volatile uint32 ResetReadBack;
  uint32 ResetStatus;

  /* Reset the kernel */
  GTM_SFR_DEINIT_RESETENDINIT();

  GTM_SFR_DEINIT_MODIFY32(GTM_KRST0.U,GTM_KRST0_CLR_MASK,GTM_ONE_U)
  ResetReadBack = GTM_SFR_DEINIT_USER_MODE_READ32(GTM_KRST0.U);
  GTM_SFR_DEINIT_WRITE32(GTM_KRST1.U,GTM_ONE_U);
  ResetReadBack = GTM_SFR_DEINIT_USER_MODE_READ32(GTM_KRST1.U);

  GTM_SFR_DEINIT_SETENDINIT();


  /* Wait for Reset acknowledgement */
  do
  {
    #ifdef IFX_GTM_DEBUG
    ResetStatus = (((((uint32)GTM_SFR_DEINIT_USER_MODE_READ32(GTM_KRST0.U))>>\
                   GTM_ONE_U)&GTM_ONE_U)& TestGtm_DebugMask01);
    #else
    ResetStatus = ((((uint32)GTM_SFR_DEINIT_USER_MODE_READ32(GTM_KRST0.U))>>\
                    GTM_ONE_U)&GTM_ONE_U);
    #endif
    KernelResetTimeOutCount--;
  }while((ResetStatus == GTM_ZERO_U) && \
                                       (KernelResetTimeOutCount > GTM_ZERO_U));

  GTM_SFR_DEINIT_RESETENDINIT();
  /* Clear the status bit */

  GTM_SFR_DEINIT_WRITE32(GTM_KRSTCLR.U,GTM_ONE_U);
  GTM_SFR_DEINIT_SETENDINIT();

  /* Read back kernel reset clear register */

  ResetReadBack = GTM_SFR_DEINIT_USER_MODE_READ32(GTM_KRSTCLR.U);
  UNUSED_PARAMETER(ResetReadBack)
}

#endif

/*******************************************************************************
** Syntax : void Gtm_lTomConfigure(void)                                      **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non-reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     Function to Initialize TOM Module                        **
*******************************************************************************/
IFX_LOCAL_INLINE void Gtm_lTomConfigure(void)
{
  Ifx_GTM_TOM_TGC_TYPE *TomTgcRegPtr;               /* Pointer to TOM TGC Reg */
  /* The following 3 variables backs up the FUPD, OUTEN and ENDIS before
     initialization of TOM channels start. The array size should be maximum
     of TOM. */
  uint32 FupdBackupValue[GTM_NO_OF_TOM_MODULES][GTM_NO_OF_TGC_PER_MODULE];
  uint32 OutenCtrlBackupValue[GTM_NO_OF_TOM_MODULES][GTM_NO_OF_TGC_PER_MODULE];
  uint32 EndisCtrlBackupValue[GTM_NO_OF_TOM_MODULES][GTM_NO_OF_TGC_PER_MODULE];
  uint8 Count;              /* Variable to maintain Loop Count                */
  uint8 LoopCount;          /* Variable to maintain Loop Count                */
  uint8 MinorCnt;           /* Variable to maintain Loop Count                */
  uint8 ModuleIndex;        /* Variable to hold the Module that uses the Timer*/



  /* This is required to retain the old values of EndisCtrl, OutenCtrl and Fupd,
     hence a Fupd given for setting the new clock value will not affect the
     previously existing channels */
  for(Count = GTM_ZERO_U; Count < GTM_NO_OF_TOM_MODULES; Count++)
  {
    for(LoopCount = GTM_ZERO_U;
        LoopCount < GTM_NO_OF_TGC_PER_MODULE; LoopCount++)
    {
      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      TomTgcRegPtr = &((*(Ifx_GTM_TOMx*)(void*)
                     (MODULE_GTM.TOM)).TOM_TGC[Count].TGC[LoopCount]);

      FupdBackupValue[Count][LoopCount] =                        \
               GTM_SFR_INIT_USER_MODE_READ32((TomTgcRegPtr->FUPD_CTRL.U));
      OutenCtrlBackupValue[Count][LoopCount] =                   \
               GTM_SFR_INIT_USER_MODE_READ32((TomTgcRegPtr->OUTEN_CTRL.U));
      EndisCtrlBackupValue[Count][LoopCount] =                   \
               GTM_SFR_INIT_USER_MODE_READ32((TomTgcRegPtr->ENDIS_CTRL.U));
      /* Make EndisCtrl and OutenCtrl to its Stat values */
      /* Clear the FUPD contents so on a Host Trigger, none of the old
         parameters are updated */
      GTM_SFR_INIT_USER_MODE_WRITE32((TomTgcRegPtr->FUPD_CTRL.U),\
                                                 GTM_TOM_TGC_CLEAR_FUPD);

      Gtm_lSaveTgcStatus(TomTgcRegPtr);

      for(MinorCnt = GTM_ZERO_U;
          MinorCnt < GTM_TOM_CHANNELS_PER_TGC; MinorCnt++)
      {

        ModuleIndex = Gtm_lGetModuleIndex(                                     \
        (uint32)(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTomModuleUsage[Count]),
                    (uint8)(MinorCnt + (LoopCount * GTM_TOM_CHANNELS_PER_TGC)));

        if(ModuleIndex > GTM_ZERO_U)
        {

          /* Clear the information in FUPD/OutEn/EndisCtrl if the corresponding
             channel is configured for GPT or PWM */
          FupdBackupValue[Count][LoopCount] &=
                   ~(GTM_TOM_CHAN_ENABLE << (GTM_TOM_BITS_PER_CHAN * MinorCnt));
          OutenCtrlBackupValue[Count][LoopCount]&=
                   ~(GTM_TOM_CHAN_ENABLE << (GTM_TOM_BITS_PER_CHAN * MinorCnt));
          EndisCtrlBackupValue[Count][LoopCount]&=
                   ~(GTM_TOM_CHAN_ENABLE << (GTM_TOM_BITS_PER_CHAN * MinorCnt));
        }
      }
    }
  }

#if ((GTM_PWM_MODULE_USED == STD_ON) || (GTM_GPT_MODULE_USED == STD_ON))
  /* Clock Setting for GPT and PWM Modules */
  Gtm_lTomClockSetting();
  Gtm_lTomHostTrigger();
  /* Clock Settings End */
#endif
  /*  #if ((GTM_PWM_MODULE_USED == STD_ON) || (GTM_GPT_MODULE_USED == STD_ON))*/


  Gtm_lTomComplexConfig();

  Gtm_lTomTgcConfigure();

  /* Get the old values of Fupd/OutenCtrl/OutenStat values back again */
  for(Count = GTM_ZERO_U; Count < GTM_NO_OF_TOM_MODULES; Count++)
  {
    for(LoopCount = GTM_ZERO_U;
        LoopCount < GTM_NO_OF_TGC_PER_MODULE; LoopCount++)
    {
      /* Get the pointer to the TOM Registers */
      TomTgcRegPtr = &((*(Ifx_GTM_TOMx*)                                       \
           (volatile void *)(MODULE_GTM.TOM)).TOM_TGC[Count].TGC[LoopCount]);
      for(MinorCnt = GTM_ZERO_U; MinorCnt < GTM_SIXTEEN_U; MinorCnt++)
      {
        if((EndisCtrlBackupValue[Count][LoopCount]&                            \
                   (GTM_TOM_CHAN_ENABLE<<(MinorCnt * GTM_TOM_BITS_PER_CHAN)))> \
                   GTM_ZERO_UL)
        {
          GTM_SFR_INIT_USER_MODE_MODIFY32((TomTgcRegPtr->ENDIS_CTRL.U),\
                (GTM_ENDIS_CLR_MASK),\
                ((uint32)((uint32)GTM_TOM_CHAN_DISABLE <<\
                ((MinorCnt * GTM_TOM_BITS_PER_CHAN)+GTM_ONE_U))))

        }
        if((OutenCtrlBackupValue[Count][LoopCount]&
               (uint32)((uint32)GTM_TOM_CHAN_ENABLE<<(MinorCnt * \
               GTM_TOM_BITS_PER_CHAN)))> GTM_ZERO_UL)
        {
          GTM_SFR_INIT_USER_MODE_MODIFY32((TomTgcRegPtr->OUTEN_CTRL.U),\
                GTM_OUTEN_CLR_MASK,((uint32)((uint32)GTM_TOM_CHAN_DISABLE <<\
                ((MinorCnt * GTM_TOM_BITS_PER_CHAN)+GTM_ONE_U))))
        }
        if((FupdBackupValue[Count][LoopCount] &
                (GTM_TOM_CHAN_ENABLE<<(MinorCnt * GTM_TOM_BITS_PER_CHAN))) >   \
                GTM_ZERO_UL)
        {
          GTM_SFR_INIT_USER_MODE_MODIFY32((TomTgcRegPtr->FUPD_CTRL.U),\
                (GTM_FUPD_CLR_MASK),((uint32)((uint32)GTM_TOM_CHAN_DISABLE <<\
                ((MinorCnt * GTM_TOM_BITS_PER_CHAN)+GTM_ONE_U))))
        }
      }
    }
  }
}

/*******************************************************************************
** Traceability : [cover parentID=DS_NAS_GTM_PR1092_4,
DS_NAS_HE1_GTM_PR1094_4,DS_NAS_HE2_GTM_PR1094_4]
** Syntax : void Gtm_ModifyOutputCtrl (uint8 TimerModule, uint8 ModuleNo,     **
**          uint8 ChannelNo, boolean ToEnable)                                **
** [/cover]                                                                   **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  TimerModule 0 TOM                                    **
**                   ModuleNo TOM Module Number                             **
**                   ChannelNo TOM Channel Number                           **
**                   ToEnable TRUE Enable TOM Output                     **
**                              FALSE Disable TOM Output                   **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     None                                                     **
**                                                                            **
** Description :     This service enables/disables the TOM Channel output.    **
*******************************************************************************/
void Gtm_ModifyOutputCtrl (uint8 TimerModule, uint8 ModuleNo,
                                    uint8 ChannelNo, boolean ToEnable)
{
  uint32 ActionToPerform;

  ActionToPerform = ((ToEnable == (boolean)TRUE)?\
                                           GTM_TOM_ENABLE:GTM_TOM_CHAN_DISABLE);

  if (TimerModule == GTM_TOM)
  {
    if(ChannelNo < GTM_TOM_TGC0_CHOSEN)
    {
      GTM_SFR_RUNTIME_WRITE32( \
      ((*(Ifx_GTM_TOMx*)(volatile void *)(MODULE_GTM.TOM)).TOM_TGC[ModuleNo]. \
             TGC[GTM_TGC0].OUTEN_STAT.U), (unsigned_int)(ActionToPerform << \
                                         (GTM_TOM_BITS_PER_CHAN * ChannelNo)));
    }
    else if(ChannelNo < GTM_TOM_TGC1_CHOSEN)
    {
      GTM_SFR_RUNTIME_WRITE32( \
      ((*(Ifx_GTM_TOMx*)(volatile void *)(MODULE_GTM.TOM)).TOM_TGC[ModuleNo]. \
                                                TGC[GTM_TGC1].OUTEN_STAT.U) , \
          (unsigned_int)(ActionToPerform <<  \
          (GTM_TOM_BITS_PER_CHAN * (ChannelNo - GTM_TOM_CHANNELS_PER_TGC))));
    }
    else
    {
      /* Do Nothing */
    }
  }
  else
  {
    /* Do Nothing */
  }
}

/*check if MCU InitCheck is enabled*/
#if (GTM_MCUINITCHECK_API == STD_ON)

/*******************************************************************************
** Traceability : [cover parentID=DS_MCAL_GTM_0001,DS_MCAL_GTM_0006,
DS_MCAL_GTM_0011,DS_MCAL_GTM_0130,DS_MCAL_GTM_0010,
DS_MCAL_GTM_0043]
 **
**                                                                            **
** Syntax          : void Gtm_InitCheck (const Gtm_ConfigType* ConfigPtr)     **
** [/cover]                                                                   **
**                                                                            **
** Service ID      :    None                                                  **
**                                                                            **
** Sync/Async      :    Synchronous                                           **
**                                                                            **
** Reentrancy      :    Non Reentrant                                         **
**                                                                            **
** Parameters (in) :    ConfigPtr - Pointer to GTM Driver configuration set   **
**                                                                            **
** Parameters (out):    None                                                  **
**                                                                            **
** Return value    :    E_OK - if initialization comparison is success        **
**                      E_NOT_OK - if initialization comparison fails         **
**                                                                            **
** Description     : This routine verifies the initialization the GTM driver. **
**                                                                            **
*******************************************************************************/
Std_ReturnType Gtm_InitCheck (const Gtm_ConfigType* ConfigPtr)
{
  uint32 TempCompVar = 0x0U;
  uint32 CompareVal = GTM_32_XOR_COMPARE_VAL;
  uint32 TempClockEnable;
  uint32 Count;
  const Gtm_GeneralConfigType *GeneralConfigPtr; /* Pointer to General Config */
  const Gtm_ClockSettingType *ClockSettingPtr;
  Std_ReturnType ErrorFlag;

  ErrorFlag = E_OK;
  TempClockEnable = (uint32)((uint32)(((uint32)GTM_SFR_INIT_USER_MODE_READ32(\
               MODULE_GTM.CLC.U))>> GTM_ONE_U)&GTM_ONE_U);
  /* Check if config pointer is NULL or GTM clock is not enabled properly
     by Gtm_Init*/
  if ((NULL_PTR == ConfigPtr) || (TempClockEnable == GTM_ONE_U))
  {
    ErrorFlag = E_NOT_OK;
  }
  else
  {
  ClockSettingPtr = ConfigPtr->GtmClockSettingPtr;
  GeneralConfigPtr = ConfigPtr->GtmModuleConfigPtr->GtmGeneralConfigPtr;

  TempClockEnable = ClockSettingPtr->GtmClockEnable;

  /* check the consistency of CLC register*/
  TempCompVar = (((uint32)GTM_SFR_INIT_USER_MODE_READ32(MODULE_GTM.CLC.U))>>\
                     GTM_CLC_EDIS_OFF_POS);
  TempCompVar = TempCompVar & GTM_ONE_U;
  if(ConfigPtr->GtmModuleConfigPtr->GtmModuleSleepEnable == GTM_SLEEP_ENABLE)
  {
    TempCompVar = TempCompVar ^ ~(uint32)(GTM_ZERO_U);
  }
  else
  {
    TempCompVar = TempCompVar ^ ~(uint32)(GTM_ONE_U);
  }
  CompareVal = ( TempCompVar == GTM_32_XOR_COMPARE_VAL) ?
                                          CompareVal : (CompareVal & (0U));

  /* check the consistency of GTM CMU registers*/
  /* CMU CLK EN register*/
  TempCompVar = TempClockEnable ^ GTM_GET_EN_REG_BIT;
  TempCompVar = TempCompVar ^ ~(uint32)(GTM_SFR_INIT_USER_MODE_READ32(\
                                   GTM_CMU_CLK_EN.U));
  CompareVal = CompareVal ^ (~((uint32)TempCompVar));

  /* CMU CLK NUM and DEN register*/
  TempCompVar = ConfigPtr->GtmModuleConfigPtr->GtmGclkNum;
  TempCompVar = TempCompVar ^ ~(((uint32)(GTM_SFR_INIT_USER_MODE_READ32(\
                               GTM_CMU_GCLK_NUM.U))));
  CompareVal = CompareVal ^ (~((uint32)TempCompVar));
  TempCompVar = ConfigPtr->GtmModuleConfigPtr->GtmGclkDen;
  TempCompVar = TempCompVar ^ ~(((uint32)(GTM_SFR_INIT_USER_MODE_READ32(\
                               GTM_CMU_GCLK_DEN.U))));
  CompareVal = CompareVal ^ ~(((uint32)TempCompVar));


  /* CMU CLK CTRL register configuration*/
  for(Count = GTM_ZERO_U; Count < GTM_CMU_NO_OF_CLOCKS; Count++)
  {
    if((TempClockEnable ) &                              \
            (uint32)(GTM_CMU_CLK_ENABLE << (Count * GTM_CMU_BITS_PER_CLK)))
    {
      TempCompVar = ClockSettingPtr->GtmCmuClkCnt[Count];
      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      TempCompVar = TempCompVar ^ ~((uint32)(GTM_SFR_INIT_USER_MODE_READ32(\
         (((Gtm_CmuType*)(void*)&(MODULE_GTM.CMU))->ClkCtrl[Count].CTRL.U))));
      CompareVal = CompareVal ^ ~((uint32)(TempCompVar));
    }
  }

  /* CMU EXT : NUM and DEN  register configuration*/
  for(Count = GTM_ZERO_U; Count < GTM_CMU_NO_OF_EXT_CLOCKS; Count++)
  {
   if(ClockSettingPtr->GtmClockEnable & \
      (uint32)(GTM_CMU_CLK_ENABLE <<((Count * GTM_CMU_BITS_PER_CLK) + \
      GTM_CMU_START_FROM_EXTCLK)))
   {
     TempCompVar = ClockSettingPtr->GtmEclk[Count].CmuEclkNum;
     /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
     TempCompVar = TempCompVar ^ ~((uint32)(GTM_SFR_INIT_USER_MODE_READ32(\
           (((Gtm_CmuType*)(void*)&(MODULE_GTM.CMU))->CmuEclk[Count].\
              CmuEclkNum.U))));
     CompareVal = CompareVal ^ ~((uint32)(TempCompVar));

     TempCompVar = ClockSettingPtr->GtmEclk[Count].CmuEclkDen;
     /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
     TempCompVar = TempCompVar ^ ~((uint32)(GTM_SFR_INIT_USER_MODE_READ32(\
       (((Gtm_CmuType*)(void *)&(MODULE_GTM.CMU))->CmuEclk[Count].\
          CmuEclkDen.U))));
     CompareVal = CompareVal ^ ~((uint32)(TempCompVar));
   }
  }

  /* CMU FXCLK register configuration*/
  TempCompVar = (ClockSettingPtr->GtmFxdClkControl) & GTM_GET_FIXED_CLK_VAL;
  /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
  TempCompVar = TempCompVar ^ ~((uint32)GTM_SFR_INIT_USER_MODE_READ32(\
               (((Gtm_CmuType*)(void*)&(MODULE_GTM.CMU))->GtmFxdClkControl)));
  CompareVal = CompareVal ^ ~(TempCompVar);

  /* check the consistency of GTM CTRL register*/
  TempCompVar = GTM_SFR_INIT_USER_MODE_READ32(GTM_CTRL.U) ;
  TempCompVar = TempCompVar ^ ~((uint32)(GeneralConfigPtr->GtmCtrlValue));
  CompareVal = CompareVal ^ ~((uint32)(TempCompVar));
  /* check the consistency of GTM IRQ EN register */
  TempCompVar = GTM_SFR_INIT_USER_MODE_READ32(GTM_IRQ_EN.U) ;
  TempCompVar = TempCompVar ^ ~((uint32)
                         (GeneralConfigPtr->GtmIrqEnable & GTM_GET_IRQ_VAL_8));
  CompareVal = CompareVal ^ ~((uint32)(TempCompVar));
    /* check the consistency of GTM IRQ MODE register */
  TempCompVar = GTM_SFR_INIT_USER_MODE_READ32(GTM_IRQ_MODE.U) ;
  TempCompVar = TempCompVar ^ ~((uint32)(((GeneralConfigPtr->GtmIrqEnable) & \
                                   GTM_GET_MODE_VAL_8) >> GTM_SHIFT_TO_LSB_8));
  CompareVal = CompareVal ^ ~((uint32)(TempCompVar));
  /* check GTM SRN Enable register*/
  TempCompVar = (uint32)(GTM_SFR_INIT_USER_MODE_READ32(SRC_GTMAEIIRQ.U) & \
                           GTM_BIT_SRE_MASK) ;
  TempCompVar = TempCompVar ^ ~((uint32)(GTM_BIT_SET << GTM_BIT_SRE));
  CompareVal = CompareVal ^ ~((uint32)(TempCompVar));
  }
  if (!(CompareVal == GTM_32_XOR_COMPARE_VAL))
  {
    ErrorFlag = E_NOT_OK;
  }
  return ErrorFlag;
}

#endif
/* #if (GTM_MCUINITCHECK_API == STD_ON)
*/

#ifdef GTM_TIM_MODULE_USED

/*******************************************************************************
** Syntax : IFX_LOCAL_INLINE uint32 Gtm_lGetTimIrqStatus(uint8 ModuleNo,      **
**                                                       uint8 ChannelNumber) **
**                                                                            **
** Service ID:       none                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       reentrant                                                **
**                                                                            **
** Parameters (in):  ModuleNo - TIM Module Number                             **
**                   ChannelNumber - Channel number (0..7)                    **
**                                                                            **
** Parameters (out): None                                                     **
**                                                                            **
** Return value:     IrqStatus - Interrupt status of the channels             **
**                                                                            **
** Description :     This function fetches the interrupt status of the channel**
**                   number                                                   **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Gtm_lGetTimIrqStatus(uint8 ModuleNo,uint8 ChannelNumber)
{
  volatile uint32 RegVal;
  uint32 IrqStatus;
  uint8 RegPos;

  /* Ascertain the position of Interrupt status for the corresponding module
     and channel*/
  RegPos = ((ModuleNo % GTM_TIM_MODULES_IN_ICM_REG) * \
                                   GTM_CHANNELS_PER_TIM_MODULE) + ChannelNumber;

  /* Identify the ICM register corresponding to the Module number */
  /* Identify the ICM register corresponding to the Module number */
  /* MISRA Rule Violation 11.4 and 17.4 Pointer to Pointer conversion*/
  /* Pointer to pointer conversion is performed to simplify the code. Since
     each of the ICM register has a different name, to make this available in a
     single functions, this conversion is performed. This pointer arithmetic
     is desired */


  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmetic is used to efficiently
   access the SFRs of multiple GTM ICM registers.*/
  RegVal = *(((volatile uint32*)(volatile void *)GTM_TIM_ICM_BASE_ADDRESS) + \
                                         (ModuleNo/GTM_TIM_MODULES_IN_ICM_REG));


  /* Retrieve the IRQ status of the channel number and the next channel */
  IrqStatus = (RegVal & (GTM_GET_TIM_INT_STATUS << RegPos)) >> (RegPos);
  return(IrqStatus);
}

#endif
/*  for #if (defined(GTM_TIM_MODULE_USED) && (GTM_MCUSAFETY_ENABLE == STD_ON))
*/

#define GTM_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"

