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
**  $FILENAME   : Gtm_Platform.c $                                           **
**                                                                           **
**  $CC VERSION : \main\dev_tc23x\16 $                                       **
**                                                                           **
**  $DATE       : 2016-03-28 $                                               **
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

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Own header file, this includes own configuration file also */
#include "Gtm.h"
#include "Gtm_Local.h"
/* Include Irq definitions for IRQ */
#include "IfxSrc_reg.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define GTM_GET_MODE_VAL_16 (0xC000UL)
/* Bits to shift to get the mode information to LSB for a 16 bit variable */
#define GTM_SHIFT_TO_LSB_16 (14U)
/* Get the IRQ Enable Information from a 16 bit variable */
#define GTM_GET_IRQ_VAL_16 (0x1FFFUL)

#define GTM_GET_MODE_VAL_8_U (0xC0U)

#define GTM_ADC_CONNECT_GET_LOWER_NIBBLE (0x0FUL)
#define GTM_ADC_CONNECT_GET_UPPER_NIBBLE (0xF0UL)
#define GTM_BITS_PER_ADC_CONNECTION (4U)
#define GTM_BITS_IN_U32 (32U)
#define GTM_CLEAR_INTERRUPT (3UL)
#define GTM_TOM_ATOM_CLK_POS (12U)
#define GTM_CLEAR_STICKY_BITS (0x52000000U)
#define GTM_TOM_NO_OF_SRC_REG (8U)
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/*Memory Map of the GTM Code*/
#define GTM_START_SEC_CODE
#include "MemMap.h"

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
**                      Global Function Definitions                           **
*******************************************************************************/
/*******************************************************************************
** Syntax : void Gtm_lTimConfigure(void)                                      **
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
** Description :     Function to initialize TIM module                        **
*******************************************************************************/
void Gtm_lTimConfigure(void)
{
  const Gtm_TimConfigType *TimConfigPtr;         /* Pointer to TIM Config     */
  Ifx_GTM_TIM_CH_TYPE *TimChannelRegPtr;       /* Pointer to TIM Channel    */
  Ifx_GTM_TIM_IN_SRC_RESET_TYPE *TimInScrRegPtr;
                                              /* Pointer to TIM IN_SRC reg    */

  uint8 TimCnt;             /* Variable to TIM Channel Initialization Count   */
  uint8 MinorCnt;           /* Variable to maintain Loop Count                */
  uint8 ChannelNo;          /* Variable to hold Channel Number                */
  uint8 TimMode;            /* Variable to hold the TIM Mode                  */

  /* Count to maintain track of the index in TIM Channel Config */
  TimCnt = GTM_ZERO_U;
  /* MajorCnt -Count to maintain an index to the GtmTimUsage Array            */
  /* MinorCnt -Count to maintain an index to the channels in GtmTimUsage Array*/
  for(MinorCnt = GTM_ZERO_U; MinorCnt < GTM_BITS_IN_U8; MinorCnt++)
  {
    /* MISRA Rule Violation 17.4 Pointer arithmetic other than array indexing 
                 used. This cannot be avoided because of Post Build structure */
      /* Derive Module Number and Channel Number from Loop Count information  */
      ChannelNo = MinorCnt % GTM_CHANNELS_PER_TIM_MODULE;

    /* Check if the channel is configured */
    if((Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTimUsage[0U] &
       (GTM_BIT_SET << MinorCnt)) > GTM_ZERO_UL)
    {
      /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic used due to 
     PBConfigStructure and is within allowed range.*/
      TimConfigPtr =&(Gtm_kConfigPtr->GtmModuleConfigPtr->\
                                                     GtmTimConfigPtr[TimCnt]);

      TimCnt++;

      /* Get the Pointer to the Channel Register Structure */
      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      TimChannelRegPtr = &(((*(Ifx_GTM_TIMx*)(void *)(MODULE_GTM.TIM)).
                                             CH_TIM[0U].CH[ChannelNo]));
      /*IFX_MISRA_RULE_11_05_STATUS= volatile in terms of pointer access. 
         Permitted for special function registers.*/
      TimInScrRegPtr = &(((*(Ifx_GTM_TIMx*)(void *)(MODULE_GTM.TIM)).
                                          IN_SRC_RST[0U].IN_SRC_RESET));

      /* Configure all Channel paramters if the Channel usage is Complex    */
      if(TimConfigPtr->TimUsage == GTM_DRIVER_COMPLEX)
      {
        /* Mask the TIM Enable status before writing the control register */
        GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_CTRL.U), \
                               (TimConfigPtr->TimCtrlValue & ~(GTM_BIT_SET)));

        /* Configure Filter related parameters only if the Filter is
           configured */
        if(TimConfigPtr->GtmTimFltPtr!= NULL_PTR)
        {
          GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_FLT_RE.U), \
                            TimConfigPtr->GtmTimFltPtr->TimRisingEdgeFilter);
          GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_FLT_FE.U), \
                           TimConfigPtr->GtmTimFltPtr->TimFallingEdgeFilter);
        }
        GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_TDUV.U), \
                              TimConfigPtr->TimTduValue);
        GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_IRQ_MODE.U), \
               (((uint32)TimConfigPtr->TimIrqEn & GTM_GET_MODE_VAL_8_U) >>
                                                        GTM_SHIFT_TO_LSB_8));

        /* Clear all Pending Interrupts */
        GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_IRQ_NOTIFY.U),\
                   GTM_CLEAR_TIM_INTERRUPT);
        GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_IRQ_EN.U), \
                       ((uint32)TimConfigPtr->TimIrqEn & GTM_GET_IRQ_VAL_8));
        /*Setting TIM EIRQ values*/
        GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_EIRQ_EN.U)  , \
              ((uint32)TimConfigPtr->TimErrIrqEn & GTM_TIM_GET_EIRQ_VAL_32));

        /*Setting TIM IN_SRC values*/
        GTM_SFR_INIT_USER_MODE_WRITE32((TimInScrRegPtr->IN_SRC.U), \
               ((uint32)TimConfigPtr->TimInSrcSel & GTM_TIM_GET_INSRC_VAL_32));

        TimMode = \
        (uint8)((TimConfigPtr->TimCtrlValue & GTM_TIM_GET_MODE) >> GTM_ONE_U);

        /* CNTS register is writable only in TIPM mode */
        if(TimMode == GTM_TIM_MODE_TIPM)
        {
          GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_CNTS.U), \
                                TimConfigPtr->TimCntsValue);
        }

        /* Update the TIM Enable bit if enabled */
        GTM_SFR_INIT_USER_MODE_MODIFY32((TimChannelRegPtr->CH_CTRL.U),\
               GTM_TIM_CH_CTRL_CLR_MASK,\
               (TimConfigPtr->TimCtrlValue & GTM_TIM_ENABLE_MASK))

       /* Update the TIM External Control register if EXT_CAP_EN is enabled */
        GTM_SFR_INIT_USER_MODE_MODIFY32((TimChannelRegPtr->CH_ECTRL.U),\
                 GTM_SEVEN_U,\
                 (TimConfigPtr->TimExtCapSrc & GTM_TIM_EXT_CAP_ENABLE_MASK))


        /* Enable SRN */
        GTM_SFR_INIT_MODIFY32((MODULE_SRC.GTM.GTM[0].TIM[0U][ChannelNo].U),\
            GTM_SRC_CLEAR_MASK,(unsigned_int)(GTM_BIT_SET << GTM_BIT_SRE))


      }
      /* If the channel is configured for ICU, then enable the SRN          */
      else
      {
        GTM_SFR_INIT_USER_MODE_WRITE32((TimChannelRegPtr->CH_IRQ_NOTIFY.U), \
                                 GTM_CLEAR_TIM_INTERRUPT);

        /* Enable SRN */
        GTM_SFR_INIT_MODIFY32((MODULE_SRC.GTM.GTM[0].TIM[0U][ChannelNo].U),\
            GTM_SRC_CLEAR_MASK,(unsigned_int)(GTM_BIT_SET << GTM_BIT_SRE))
      }
    }
  }

}
/*******************************************************************************
** Syntax : void Gtm_lAdcConnectionsConfig(void)                              **
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
** Description :     Function to initialize ADC Connections                   **
*******************************************************************************/
 void Gtm_lAdcConnectionsConfig(void)
{
  uint32 RegTemp1;          /* Temporary variable to store the register value */
  uint32 RegTemp2;          /* Temporary variable to store the register value */
  uint8 Count;

  /* Check if Adc Connections is configured */
  if(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmAdcConnectionsPtr != NULL_PTR)
  {
    RegTemp1 = GTM_ZERO_UL;
    RegTemp2 = GTM_ZERO_UL;

    /* MISRA Rule Violation 17.4
       Pointer arithmetic other than array indexing used
       This cannot be avoided because of Post Build structure */

    for(Count=GTM_ZERO_U; Count < GTM_ADC_CONN_PER_REGISTER; Count++)
    {
      /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic used due to 
     PBConfigStructure and is within allowed range.*/
      RegTemp1 |=                                                              \
       ((uint32)Gtm_kConfigPtr->GtmModuleConfigPtr->GtmAdcConnectionsPtr[Count]\
                                   & GTM_ADC_CONNECT_GET_LOWER_NIBBLE) <<      \
                                    (GTM_BITS_PER_ADC_CONNECTION * Count);
      /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic used due to 
     PBConfigStructure and is within allowed range.*/
      RegTemp2 |=                                                              \
      (((uint32)Gtm_kConfigPtr->GtmModuleConfigPtr->GtmAdcConnectionsPtr[Count]\
            & GTM_ADC_CONNECT_GET_UPPER_NIBBLE) >> GTM_BITS_PER_ADC_CONNECTION)\
            << (GTM_BITS_PER_ADC_CONNECTION * Count);

    }
    GTM_SFR_INIT_USER_MODE_WRITE32((GTM_ADCTRIG0OUT0.U),(uint32)RegTemp1);
    GTM_SFR_INIT_USER_MODE_WRITE32((GTM_ADCTRIG1OUT0.U),(uint32)RegTemp2);
  }
}

#if (GTM_DEINIT_API_ENABLE == STD_ON)
/*******************************************************************************
** Traceability : [cover parentID=                    ]                       **
** Syntax :  void Gtm_lResetGtmSRCReg (void)                                  **
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
** Description :  This service shall de-initialize GTM registers that are not **
** de-initaliazed by GTM kernel reset                                         **
**                                                                            **
*******************************************************************************/
void Gtm_lResetGtmSRCReg(void)
{
  uint8 ModuleNo;
  uint8 ChannelNo;
  /* De-init AEI SRC register*/
  /* Clear Sticky bits of SRC register */
  GTM_SFR_DEINIT_MODIFY32((SRC_GTMAEIIRQ.U),GTM_SRC_CLEAR_MASK,\
    GTM_CLEAR_STICKY_BITS)

  /* Disable SRE  */
  GTM_SFR_DEINIT_MODIFY32((SRC_GTMAEIIRQ.U),GTM_SRC_SRE_CLEAR_MASK,\
         GTM_ZERO_U)
  /* De-init TIM SRC registers */

  for(ChannelNo=GTM_ZERO_U; ChannelNo < GTM_CHANNELS_PER_TIM_MODULE;\
                                                                ChannelNo++)
  {
    /* Clear Sticky bits of SRC register */
    GTM_SFR_DEINIT_MODIFY32((MODULE_SRC.GTM.GTM[0U].TIM[0U][ChannelNo].U),\
       GTM_SRC_CLEAR_MASK,GTM_CLEAR_STICKY_BITS)
    /* Disable SRE  */

    GTM_SFR_DEINIT_MODIFY32((MODULE_SRC.GTM.GTM[0U].TIM[0U][ChannelNo].U),\
         GTM_SRC_SRE_CLEAR_MASK,GTM_ZERO_U)

  }

  /* De-init TOM SRC registers */
  for(ModuleNo=GTM_ZERO_U; ModuleNo < GTM_NO_OF_TOM_MODULES; ModuleNo++)
  {
    for(ChannelNo=GTM_ZERO_U; ChannelNo < GTM_TOM_NO_OF_SRC_REG;\
                                                                 ChannelNo++)
    {
      /* Clear Sticky bits of SRC register */
     GTM_SFR_DEINIT_MODIFY32(\
         (MODULE_SRC.GTM.GTM[0U].TOM[ModuleNo][ChannelNo].U),\
         GTM_SRC_CLEAR_MASK,\
         GTM_CLEAR_STICKY_BITS)
      /* Disable SRE  */
     GTM_SFR_DEINIT_MODIFY32(\
         (MODULE_SRC.GTM.GTM[0U].TOM[ModuleNo][ChannelNo].U),\
         GTM_SRC_SRE_CLEAR_MASK,
         GTM_ZERO_U)
    }
  }
}
/* End of Gtm_lResetGtmSRCReg() */
#endif

/*******************************************************************************
** Syntax : void Gtm_lTomComplexConfig(void)                                  **
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
** Description :     Function to initialize Complex Configuration of TOM      **
*******************************************************************************/
void Gtm_lTomComplexConfig(void)
{
  const Gtm_TomConfigType *TomChannelConfigPtr;  /* Pointer to TOM Channel    */
  Ifx_GTM_TOM_CH_TYPE *TomChannelRegPtr;       /* Ptr to TOM Channel Reg    */
  uint8 TomCnt;             /* Variable to TOM Channel Initialization Count   */
  uint8 MajorCnt;           /* Variable to maintain Loop Count                */
  uint8 MinorCnt;           /* Variable to maintain Loop Count                */
  uint8 ModuleNo;           /* Variable to hold Module Number                 */
  uint8 ChannelNo;          /* Variable to hold Channel Number                */
  volatile uint32 MajorCountLoopLimit = (((GTM_MAX_TOM_CHANNELS - GTM_ONE_U)/\
                                                   GTM_BITS_IN_U32) +GTM_ONE_U);
  /* Count to maintain track of the index in TOM Channel Config  */
  TomCnt = GTM_ZERO_U;

  /* MajorCnt -Count to maintain an index to the GtmTomUsage Array            */
  /* MinorCnt -Count to maintain an index to the channels in GtmTomUsage Array*/
  for(MajorCnt = GTM_ZERO_U; MajorCnt < MajorCountLoopLimit; MajorCnt++)
  {
    for(MinorCnt = GTM_ZERO_U;
       (((MajorCnt * GTM_BITS_IN_U32)+ MinorCnt) < GTM_MAX_TOM_CHANNELS);
       MinorCnt++)
    {
      /* Check if the channel is configured */
      if((Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTomUsage[MajorCnt] &          \
        (GTM_BIT_SET << MinorCnt)) > GTM_ZERO_UL)
      {
        /* MISRA Rule Violation 17.4
           Pointer arithmetic other than array indexing used
           This cannot be avoided because of Post Build structure */

        /* Get the Pointer to the Channel Configuration Register */
        /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic used due to 
     PBConfigStructure and is within allowed range.*/
        TomChannelConfigPtr =
                 &(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTomConfigPtr[TomCnt]);

        TomCnt++;
        /* Initialize the channels if the channel is configured as Complex*/
        if(TomChannelConfigPtr->TomUsage == GTM_DRIVER_COMPLEX)
        {
          /* Extract Module Number and Channel Number from the Loop Count     */
          ModuleNo = ((MajorCnt * GTM_BITS_IN_U32) + MinorCnt)/
                       GTM_CHANNELS_PER_TOM_MODULE;
          ChannelNo = MinorCnt % GTM_CHANNELS_PER_TOM_MODULE;

          /* MISRA Rule Violation 11.4 and 1.2
             Unusual pointer cast (incompatible indirect types)
             cast from pointer to pointer
             Casting of TOM to Gtm_TomChannelRegType is done to change the base
             type of TOM as the type defined in SFR file is not flexible to
             provide an arrayed approach for accessing the TOM channels. */
          /*Get the reference to the TOM channel pointer reg*/
          /*IFX_MISRA_RULE_11_05_STATUS= Volatile in terms of pointer access. 
         Permitted for special function registers.*/
          TomChannelRegPtr = &((((*(Ifx_GTM_TOMx*)(void *)(MODULE_GTM.TOM)).   \
                                            TOM_CH[ModuleNo])).CH[ChannelNo]);

          /* Initialize the Channel parameters */
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->CN0.U),\
                     TomChannelConfigPtr->GtmTomChannelCfgPtr->GtmTomCn0Value);
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->CM0.U),\
                     TomChannelConfigPtr->GtmTomChannelCfgPtr->GtmTomCm0Value);
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->CM1.U),\
                     TomChannelConfigPtr->GtmTomChannelCfgPtr->GtmTomCm1Value);
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->SR0.U),\
                     TomChannelConfigPtr->GtmTomChannelCfgPtr->GtmTomSr0Value);
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->SR1.U),\
                     TomChannelConfigPtr->GtmTomChannelCfgPtr->GtmTomSr1Value);

          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->IRQ_MODE.U),\
                  (uint32)TomChannelConfigPtr->GtmTomIrqMode);
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->CTRL.U), \
                       TomChannelConfigPtr->GtmTomControlWord);

          /* Clear the Pending Interrupts */
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->IRQ_NOTIFY.U),\
                        GTM_CLEAR_INTERRUPT);
          GTM_SFR_INIT_WRITE32((TomChannelRegPtr->IRQ_EN.U),\
               (uint32)TomChannelConfigPtr->GtmTomChannelCfgPtr->GtmTomIrqEn);

          /* Enable SRN */
          GTM_SFR_INIT_MODIFY32(\
             (MODULE_SRC.GTM.GTM[0].TOM[ModuleNo][ChannelNo/2U].U),\
             GTM_SRC_CLEAR_MASK,\
             (unsigned_int)(GTM_BIT_SET << GTM_BIT_SRE))
             
        }
      }
    }
  }
}
/*******************************************************************************
** Syntax : void Gtm_lTomClockSetting(void)                                   **
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
** Description :     Function to initialize TOM Clock information             **
*******************************************************************************/
void Gtm_lTomClockSetting(void)
{
  const Gtm_TomConfigType *TomChannelConfigPtr;  /* Pointer to TOM Channel    */
  Ifx_GTM_TOM_CH_TYPE *TomChannelRegPtr;         /* Ptr to TOM Channel Reg    */
  Ifx_GTM_TOM_TGC_TYPE *TomTgcRegPtr;            /* Pointer to TOM TGC Reg    */
  uint8 TomCnt;             /* Variable to TOM Channel Initialization Count   */
  uint8 MajorCnt;           /* Variable to maintain Loop Count                */
  uint8 MinorCnt;           /* Variable to maintain Loop Count                */
  uint8 ModuleNo;           /* Variable to hold Module Number                 */
  uint8 ChannelNo;          /* Variable to hold Channel Number                */
  uint8 TgcNumber;          /* Variable to hold TGC Number                    */
  uint32 RegVal;
  volatile uint32 MajorCountLoopLimit = (((GTM_MAX_TOM_CHANNELS - GTM_ONE_U)/\
                                                  GTM_BITS_IN_U32) + GTM_ONE_U);


  /* Clock Setting for GPT and PWM Modules */
  /* Count to maintain track of the index in TOM Channel Config   */
  TomCnt = GTM_ZERO_U;
  /* MajorCnt -Count to maintain an index to the GtmTomUsage Array            */
  /* MinorCnt -Count to maintain an index to the channels in GtmTomUsage Array*/
  for(MajorCnt = GTM_ZERO_U; MajorCnt < MajorCountLoopLimit; MajorCnt++)
  {
    for(MinorCnt = GTM_ZERO_U;
       (((MajorCnt * GTM_BITS_IN_U32)+ MinorCnt) < GTM_MAX_TOM_CHANNELS);
       MinorCnt++)
    {
      /* Check if the TOM channel is configured */
      if((Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTomUsage[MajorCnt] &          \
                                       (GTM_BIT_SET << MinorCnt)) > GTM_ZERO_UL)
      {
        /* MISRA Rule Violation 17.4
           Pointer arithmetic other than array indexing used
           This cannot be avoided because of Post Build structure */

        /* Get reference to the Channel configuration */
        /*IFX_MISRA_RULE_17_04_STATUS= Pointer arithmetic used due to 
     PBConfigStructure and is within allowed range.*/
        TomChannelConfigPtr =                                                  \
                 &(Gtm_kConfigPtr->GtmModuleConfigPtr->GtmTomConfigPtr[TomCnt]);

        TomCnt++;
        /* Check if the channel is configured for GPT or PWM */
       if(TomChannelConfigPtr->TomUsage != GTM_DRIVER_COMPLEX)
        {
          /* Extract module number and channel number from the Loop
             Count information */
          ModuleNo = ((MajorCnt * GTM_BITS_IN_U32) + MinorCnt)/
                                                    GTM_CHANNELS_PER_TOM_MODULE;
          ChannelNo = MinorCnt % GTM_CHANNELS_PER_TOM_MODULE;
          TgcNumber =                                                          \
                 (MinorCnt/GTM_TOM_CHANNELS_PER_TGC) % GTM_NO_OF_TGC_PER_MODULE;

          /* MISRA Rule Violation 11.4 and 1.2
             Unusual pointer cast (incompatible indirect types)
             cast from pointer to pointer
             Casting of TOM to Gtm_TomChannelRegType is done to change the base
             type of TOM as the type defined in SFR file is not flexible to
             provide an arrayed approach for accessing the TOM channels. */

          /* Get reference to the Channel Register */
          /*IFX_MISRA_RULE_11_05_STATUS= Volatile in terms of pointer access. 
         Permitted for special function registers.*/
          TomChannelRegPtr = &((((*(Ifx_GTM_TOMx*)(void *)(MODULE_GTM.TOM)).   \
                                             TOM_CH[ModuleNo])).CH[ChannelNo]);


          /*Set the CTRL value*/
          GTM_SFR_INIT_USER_MODE_MODIFY32((TomChannelRegPtr->CTRL.U),\
            GTM_TOM_CH_CTRL_CLK_CLR_MASK,\
          ((uint32)(TomChannelConfigPtr->GtmTomControlWord) & \
             GTM_TOM_CH_CTRL_CLK_MASK))


          /*Set the IRQ_MODE value*/

          GTM_SFR_INIT_USER_MODE_WRITE32((TomChannelRegPtr->IRQ_MODE.U),\
                              TomChannelConfigPtr->GtmTomIrqMode);
          /*Get the TGC reg pointer reference*/
          /*IFX_MISRA_RULE_11_05_STATUS= Volatile in terms of pointer access. 
         Permitted for special function registers.*/
          TomTgcRegPtr = &(((*(Ifx_GTM_TOMx*)(void *)(MODULE_GTM.TOM)).
                                TOM_TGC[ModuleNo].TGC[TgcNumber]));
          /*Set the FUPD_CTRL value*/
          RegVal = (uint32)(GTM_TOM_ENABLE << (GTM_TOM_BITS_PER_CHAN * \
                         (ChannelNo % GTM_TOM_CHANNELS_PER_TGC)));
          GTM_SFR_INIT_USER_MODE_MODIFY32((TomTgcRegPtr->FUPD_CTRL.U),\
                       GTM_WHOLE_REG,RegVal)
          /* Enable SRN */
          GTM_SFR_INIT_MODIFY32((\
               MODULE_SRC.GTM.GTM[0].TOM[ModuleNo][ChannelNo/2U].U),\
               GTM_SRC_CLEAR_MASK,\
               (unsigned_int)(GTM_BIT_SET << GTM_BIT_SRE))
        }
      }
    }
  }
}

#define GTM_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS= File inclusion after pre-processor directives is 
 allowed only for MemMap.h*/
#include "MemMap.h"
