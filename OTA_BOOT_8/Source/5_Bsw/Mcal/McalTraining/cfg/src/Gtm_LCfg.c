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
**  FILENAME  : Gtm_LCfg.c                                                   **
**                                                                           **
**  $CC VERSION : \main\dev_tc23x\4 $                                        **
**                                                                           **
**  DATE, TIME: 2024-08-13, 09:38:46                                         **
**                                                                           **
**  GENERATOR : Build b141014-0350                                           **
**                                                                           **
**  AUTHOR    : DL-AUTOSAR-Engineering                                       **
**                                                                           **
**  VENDOR    : Infineon Technologies                                        **
**                                                                           **
**  DESCRIPTION  : GTM configuration generated out of ECU configuration      **
**                 file (Mcu.bmd/.xdm)                                       **
**                                                                           **
**  TRACEABILITY: 
    [cover parentID=SAS_AS4XX_GTM_PR679_1,SAS_AS4XX_GTM_PR679_2,
    SAS_AS4XX_GTM_PR679_3]
[/cover]                                                                     **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/*******************************************************************************
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
**  DATE, TIME: 2024-08-13, 09:38:46                                         **
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

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/


/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/


/*******************************************************************************
**                      Global Funtion Declarations                           **
*******************************************************************************/
extern void Wdg_17_Scu_Cbk_GtmNotifySRV
  (uint8 ModuleType, uint8 ModuleNo, uint8 ChannelNo,uint16 IrqNotifVal);
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

#define GTM_START_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#ifdef GTM_MOD_CH_CONFIGURED
#if (GTM_MOD_CH_CONFIGURED == (STD_ON))

const Gtm_NotificationConfigType Gtm_kNotifConfig0 =
{
#if (GTM_NO_OF_TIM_CH_CONF_NOTIF > 0U)
{
    /*TIM Notification pointers*/
},
#endif 
  
#if (GTM_NO_OF_TOM_CH_CONF_NOTIF > 0U)
{    /*TOM Notification pointers*/

        &Wdg_17_Scu_Cbk_GtmNotifySRV,
  },
#endif
};
#endif 
/*#ifdef GTM_MOD_CH_CONFIGURED*/
#endif 
/*#if (GTM_MOD_CH_CONFIGURED == (STD_ON))*/

#define GTM_STOP_SEC_CONST_UNSPECIFIED
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


