/* Traceability : [cover parentID=SAS_NAS_WDG_PR483][/cover]            */
/* Traceability : [cover parentID=SAS_NAS_WDG_PR913][/cover]            */
/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2013)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. Passing on and copying of this document, and communication   **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME  : Wdg_17_Scu_PBCfg.c                                            **
**                                                                            **
**  $CC VERSION : \main\45 $                                                 **
**                                                                            **
**  DATE, TIME: 2024-08-13, 09:38:45                                      **
**                                                                            **
**  GENERATOR : Build b141014-0350                                          **
**                                                                            **
**  AUTHOR    : DL-AUTOSAR-Engineering                                        **
**                                                                            **
**  VENDOR    : Infineon Technologies                                         **
**                                                                            **
**  DESCRIPTION  : WDG configuration generated out of ECU configuration       **
**                 file  (Wdg_17_Scu.bmd and Wdg_17_Scu.xdm)                  **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/




/******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Include module header File */

/* Module header file, which includes module configuration(Wdg_17_Cfg.h) file */
#include "Wdg_17_Scu.h"

/******************************************************************************
**                      Imported Compiler Switch Checks                      **
*******************************************************************************/

/******************************************************************************
**                      Private Macro Definitions                            **
*******************************************************************************/
/*
Configuration Options:
For fast mode, the divider is 256
*/

#define WDG_CLK_DIVIDER_FAST  (256U)

/*
Configuration Options:
For slow mode, the divider 16384
*/

#define WDG_CLK_DIVIDER_SLOW  (16384U)


/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

#define WDG_17_SCU_START_SEC_POSTBUILDCFG
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"



/*
Configuration: Wdg_17_Scu_ConfigType
Overall configuration of the WDT
*/
const Wdg_17_Scu_ConfigType Wdg_ConfigRoot[1]=
{

  {
    /* Traceability : [cover parentID=DS_AS_WDG090, DS_AS_WDG092] [/cover]*/




         
    /* These are the Gtm settings for this configuration instance */     
    {
       {
          /* GTM Configuration for WDG0 */
        
          50530U, /* Fast callback period in ticks. */
          65000U, /* Slow callback period in ticks. */
          10U, /* Fast callback period in ms */
          13U,/*  Slow callback period in ms */
          WDG_GTM_TOM_CHANNEL, /* GTM timer module */
          0U, /* Gtm Module number that services H/W Wdg */
          1U, /* Gtm channel number  that services H/W Wdg */
          (uint8)0U
        
      },


 
    },
 
     
    /* Resolution of watchdog timer in fast mode  : 0.0032 ms
       Minimum timeout period in fast mode        : 0.0032 ms
       Maximum timeout period in fast mode        : 209.7152 ms */

    {

      (uint16)3036U, /* Fast Mode Reload Value for WDG0 */

    },


    /* Resolution of watchdog timer in slow mode  : 0.2048 ms
       Minimum timeout period in slow mode        : 0.2048 ms
       Maximum timeout period in slow mode        : 13421.7728 ms */

    {
      (uint16)58211U, /* Slow Mode Reload Value for WDG0 */

         
    }, 



  
    /* Initial password */

    /* Configuration : WdgDefaultMode:
       Default mode for watchdog driver initialization: WDG083:
       - WDGIF_OFF_MODE
       - WDGIF_SLOW_MODE
       - WDGIF_FAST_MODE
    */
    


    {
      WDGIF_FAST_MODE, /*Default Mode for WDG0 */
  
    },
    
    /* TimerTolerance */
    /*WdgSafetyProperty , 1 = WDGx safe; 0 WDGx QM */
    {
      (0U),

    },
    /* WdgSafeIndex */
    /*  WdgQmIndex*/ 
    {
      (0U),

    }
  }
  

};


#define WDG_17_SCU_STOP_SEC_POSTBUILDCFG
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
**                      Private Function Definitions                          **
*******************************************************************************/


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
