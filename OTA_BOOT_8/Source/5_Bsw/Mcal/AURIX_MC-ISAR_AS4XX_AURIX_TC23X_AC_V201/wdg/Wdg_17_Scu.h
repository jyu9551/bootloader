 /*****************************************************************************
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
**  $FILENAME   : Wdg_17_Scu.h $                                             **
**                                                                           **
**  $CC VERSION : \main\67 $                                                 **
**                                                                           **
**  $DATE       : 2016-05-11 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains functionalities of WDG driver           **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/

#ifndef WDG_17_SCU_H
#define WDG_17_SCU_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Traceability : [cover parentID=SAS_NAS_WDG_PR746,SAS_NAS_ALL_PR746,
DS_NAS_WDG_PR1651_PR235_PR236_PR628,DS_NAS_WDG_PR641_PR642_PR644_PR622,
DS_NAS_WDG_PR1650_PR1654,SAS_NAS_ALL_PR748,SAS_NAS_ALL_PR128,
DS_NAS_HE2_WDG_PR3054,DS_NAS_EP_WDG_PR3054][/cover]                           */

/* Inclusion of WdgIf_Types.h is required only for AS402 spec */
#include "Std_Types.h"
/* Inclusion of WdgIf_Types.h is required only for AS402 spec */
#include "WdgIf_Types.h"
#include "Mcal.h"
/* Wdg Module Configuration header file */
#include "Wdg_17_Scu_Cfg.h"

#include "SchM_17_McalCfg.h"
#include SCHM_WDG_17_SCU_HEADER

/* Traceability : [cover parentID=DS_AS_WDG066][/cover]            */
#if ( WDG_DEV_ERROR_DETECT == STD_ON)
  /* Import of DET functionality */
  #include "Det.h"
#endif
  /* End Of WDG_DEV_ERROR_DETECT */
  
  
#if ((WDG_USER_MODE_INIT_API_ENABLE == STD_ON) || \
     (WDG_USER_MODE_RUNTIME_API_ENABLE == STD_ON) || \
     (WDG_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON))
           #include "Wdg_Protect.h"
#endif

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/* Traceability : [cover parentID=SAS_AS_WDG156]*/
#define WDG_17_SCU_VENDOR_ID                ((uint16)17)
/* WDG MODULE INSTANCE ID */
#define WDG_17_SCU_MODULE_ID                ((uint8)102U)
/* [/cover] */
/* Wdg Development error detection enabled WDG010,WDG063*/
/* Traceability : [cover parentID=DS_AS40X_WDG010, DS_AS_WDG063]*/

#if ( WDG_DEV_ERROR_DETECT  == STD_ON)

/* API service used in wrong context (e.g. driver not initialized) */
#define WDG_E_DRIVER_STATE ((uint8)0x10)

/* API service called with wrong/inconsistent parameter(s) */
#define WDG_E_PARAM_MODE   ((uint8)0x11)

/*API Service called with wrong parameter.Config is a null pointer */
#define WDG_E_PARAM_CONFIG  ((uint8)0x12)

/*The passed timeout value is higher than the maximum timeout value*/
#define WDG_E_PARAM_TIMEOUT ((uint8)0x13)

/*API is called with wrong pointer value*/
#define WDG_E_PARAM_POINTER ((uint8)0x14)

/*API is called with wrong password value*/
#define WDG_E_PARAM_PASSWD ((uint8)0x15)

/*
   API Service ID's WDG010.
*/
/* API Service ID for Wdg_17_Scu_Init() */
#define WDG_SID_INIT             ((uint8)0x00)

/* API Service ID for Wdg_17_Scu_SetMode() */
#define WDG_SID_SETMODE          ((uint8)0x01)

/* API Service ID for Wdg_ GetVersionInfo() */
#define WDG_SID_GETVERSIONINFO   ((uint8)0x04)


/* API Service ID for Wdg_17_Scu_Cbk_GtmNotificationSRV() */
#define WDG_SID_GTMNOTIF         ((uint8)0x05)

/* API Service ID for Wdg_17_Scu_SetTriggerCondition() */
#define WDG_SID_TRIGGER_CONDITION   ((uint8)0x03)

/* API Service ID for Wdg_17_Scu_InitAltResetPassword() */
#define WDG_SID_INIT_ALTRESETPASSWORD ((uint8)0x08)

#endif /* End of #if (WDG_DEV_ERROR_DETECT == STD_ON) */

/* [/cover] */
#define WDG_INIT_ACCESS                (10U)
#define WDG_RUNTIME_ACCESS             (11U)


#if (WDG_USER_MODE_INIT_API_ENABLE == STD_ON)

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define  WDG_SFR_INIT_GETCOREID()    (WDG_INIT_GETCOREID())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/    
#define WDG_SFR_INIT_RESETENDINIT()           (WDG_INIT_RESETENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_INIT_SETENDINIT()             (WDG_INIT_SETENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced 
 initialiser*/
#define WDG_SFR_INIT_WRITE32(reg,value)       WDG_INIT_WRITE32((reg), (value))

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced 
 initialiser*/
#define WDG_SFR_INIT_MODIFY32(reg,clearmask,setmask)     \
                                  WDG_INIT_MODIFY32((reg),(clearmask),(setmask))


#if (WDG_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define WDG_SFR_INIT_USER_MODE_WRITE32(reg, value)     \
                               WDG_INIT_USER_MODE_OS_API_WRITE32((reg), (value))

   /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define WDG_SFR_INIT_USER_MODE_READ32(reg)             \
                                          WDG_INIT_USER_MODE_OS_API_READ32(reg)

#else

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define WDG_SFR_INIT_USER_MODE_WRITE32(reg, value)        (reg) = (value)
  
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define WDG_SFR_INIT_USER_MODE_READ32(reg)        (reg)
#endif /* WDG_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON  */

#else

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define  WDG_SFR_INIT_GETCOREID()     (Mcal_GetCoreId())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_INIT_RESETENDINIT()   (Mcal_ResetENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced 
 initialiser*/
#define WDG_SFR_INIT_WRITE32(reg, value)  ((reg) = (value))

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
 /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced 
 initialiser*/
#define WDG_SFR_INIT_MODIFY32(reg, clearmask,setmask) \
    { uint32 (val) ; (val) = (reg) ; (val) &= (clearmask); (val) |= (setmask); \
                                                               (reg) = (val);}

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/         
#define WDG_SFR_INIT_SETENDINIT()  (Mcal_SetENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_INIT_USER_MODE_WRITE32(reg, value)     ((reg) = (value))

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_INIT_USER_MODE_READ32(reg) (reg)

#endif /* WDG_USER_MODE_INIT_API_ENABLE == STD_ON  */

#if (WDG_USER_MODE_RUNTIME_API_ENABLE == STD_ON)
/*WDG UserMode is enabled*/

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_GETCOREID()    (WDG_RUNTIME_GETCOREID())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_RESETENDINIT()         (WDG_RUNTIME_RESETENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_SETENDINIT()           (WDG_RUNTIME_SETENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_WRITE32(reg,value)   WDG_RUNTIME_WRITE32((reg), (value))

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced 
 initialiser*/
#define WDG_SFR_RUNTIME_MODIFY32(reg,clearmask,setmask)   \
                               WDG_RUNTIME_MODIFY32((reg),(clearmask),(setmask))

#if (WDG_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
    
    /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    #define WDG_SFR_RUNTIME_USER_MODE_WRITE32(reg, value)  \
                           WDG_RUNTIME_USER_MODE_OS_API_WRITE32((reg), (value))
                           
    /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    #define WDG_SFR_RUNTIME_USER_MODE_READ32(reg)        \
                                      WDG_RUNTIME_USER_MODE_OS_API_READ32(reg)

#else

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define WDG_SFR_RUNTIME_USER_MODE_WRITE32(reg, value)     ((reg) = (value))

  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define WDG_SFR_RUNTIME_USER_MODE_READ32(reg)    (reg)

#endif /* WDG_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON  */

#else

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_GETCOREID()          (Mcal_GetCoreId())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_RESETENDINIT()           (Mcal_ResetENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_WRITE32(reg, value)      ((reg) = (value))

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced 
 initialiser*/
#define WDG_SFR_RUNTIME_MODIFY32(reg, clearmask,setmask) \
      { uint32 (val) ; (val) = (reg) ; (val) &= (clearmask); \
                                        (val) |= (setmask); (reg) = (val);}

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/              
#define WDG_SFR_RUNTIME_SETENDINIT()   (Mcal_SetENDINIT())

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/        
#define WDG_SFR_RUNTIME_USER_MODE_WRITE32(reg, value)    ((reg) = (value))

/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define WDG_SFR_RUNTIME_USER_MODE_READ32(reg)      (reg)
   
#endif /* WDG_USER_MODE_RUNTIME_API_ENABLE = STD_ON */


/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
/* The timeout value given in timer ticks.*/
#if ( WDG_MAXQM_TIMERS  != 0U)

typedef uint32 Wdg_17_Scu_GtmValueType;
#endif

/** Traceability : [cover parentID=DS_NAS_WDG_PR643_1] */
#if ((WDG_DEV_ERROR_DETECT == STD_ON) || (WDG_DEBUG_SUPPORT == STD_ON))
typedef enum
{
    WDG_UNINIT = 0,
    WDG_IDLE,
    WDG_BUSY
} Wdg_17_Scu_StatusType;
#endif/*((WDG_DEV_ERROR_DETECT == STD_ON) || (WDG_DEBUG_SUPPORT == STD_ON))*/
/* [/cover] */
/*
Collection of Gtm related settings.
*/
/** Traceability : [cover parentID=DS_AS40X_WDG105] */
#if (WDG_MAXQM_TIMERS != 0U)
typedef struct
{
  
  /* Fast callback period in ticks */
  Wdg_17_Scu_GtmValueType   FastServiceGtmCbkTicks; 
  /* Slow callback period in ticks */
  Wdg_17_Scu_GtmValueType   SlowServiceGtmCbkTicks;
  /* Fast callback period in time ms */
  uint16   FastServiceGtmCbkTime;
  /* Slow callback period in time ms */
  uint16   SlowServiceGtmCbkTime;
  
  /*Gtm timer module shortname that services H/W Wdg */
  uint8  GtmModuleForService;
  /*Gtm timer module ID that services H/W Wdg */
  uint8  GtmModuleNumberForService;
  /*Gtm timer channel ID that services H/W Wdg */
  uint8  GtmChannelNumberForService;
  uint8   Reserved;
}Wdg_17_Scu_GtmSettingsType;
#endif
/* [/cover] */
/*
  Wdg_ConfgType: This is the type of Wdg external data structure containing
  the overall initialization data for the Wdg Driver
*/
typedef struct Wdg_17_Scu_ConfigType
{
  #if (WDG_MAXQM_TIMERS != 0U)
  /* Gtm related settings */
  Wdg_17_Scu_GtmSettingsType GtmConf[WDG_MAXQM_TIMERS];
  
  /*FastModeReloadValue: Reload value for fast mode */
  uint16 FastModeReloadValue[WDG_MAXQM_TIMERS];

  /*SlowModeReloadValue: Reload value for slow mode */
  uint16 SlowModeReloadValue[WDG_MAXQM_TIMERS];
  #endif
  #if (WDG_MAXSAFE_TIMERS != 0U)
  /*SlowModeReloadValue: Reload value for slow mode */
  uint16 ReloadValue[WDG_MAXSAFE_TIMERS];
  
  /*Initial password for WDG with safety  */
  uint16 InitialPassword[WDG_MAXSAFE_TIMERS];
  #endif
   
  /* DefaultMode : Default Configuration Mode for Operation after
              Wdg_17_Scu_Init */
  WdgIf_ModeType DefaultMode[WDG_MAX_TIMERS];
  
  #if (WDG_MAXSAFE_TIMERS != 0U)
  #if (WDG_ENABLE_TIME_CHECK == STD_ON)
  /*Timer tolerance value for WDG with safety */
  uint8 TimerTolerance[WDG_MAXSAFE_TIMERS];
  #endif
  #endif
  /* indicates WDGx in safety/non safety  */
  uint8 WdgSafetyProperty[WDG_MAX_TIMERS];
  
  /* WDGx in safety */
  #if (WDG_MAXSAFE_TIMERS != 0U)
  uint8 WdgSafeIndex[WDG_MAX_TIMERS];
  #endif
   
  /* WDGx in QM */
  #if (WDG_MAXQM_TIMERS != 0U)
  uint8 WdgQmIndex[WDG_MAX_TIMERS];
  #endif 
  
} Wdg_17_Scu_ConfigType;



/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
#define WDG_17_SCU_START_SEC_POSTBUILDCFG
#include "MemMap.h"
/* Declaration of Wdg Post Build Configuration */
extern const Wdg_17_Scu_ConfigType Wdg_ConfigRoot[WDG_17_CONFIG_COUNT];
#define WDG_17_SCU_STOP_SEC_POSTBUILDCFG
#include "MemMap.h"
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
#if (WDG_PB_FIXEDADDR == STD_ON)
#define WDG_17_SCU_START_SEC_CONST_32BIT
#include "MemMap.h"
/* To store the Wdg driver configuration pointer (Fixed address) */
 extern const Wdg_17_Scu_ConfigType * const Wdg_kConfigPtr;
#define WDG_17_SCU_STOP_SEC_CONST_32BIT
#include "MemMap.h"
#endif /*(WDG_PB_FIXEDADDR == STD_ON)*/ 

#if (WDG_PB_FIXEDADDR == STD_OFF)

#define WDG_17_SCU_START_SEC_VAR_32BIT
#include "MemMap.h"

/* To store the Wdg Driver configuration pointer */
/*IFX_MISRA_RULE_08_08_STATUS=Wdg_kConfigPtr is declared extern only 
  here and is defined in Wdg_17_Scu.c. This violation is reported incorrectly 
  by PC-lint tool */
extern const Wdg_17_Scu_ConfigType *Wdg_kConfigPtr;


#define WDG_17_SCU_STOP_SEC_VAR_32BIT
#include "MemMap.h" 
#endif /*(WDG_PB_FIXEDADDR == STD_OFF)*/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
#define WDG_17_SCU_START_SEC_CODE
#include "MemMap.h"




#if (WDG_MAXSAFE_TIMERS != 0U)

/*******************************************************************************
** Syntax           : _IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitEnter(void)    **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to enter critical section during initialization**
**                                                                            **
*******************************************************************************/
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitEnter(void);


/*******************************************************************************
** Syntax           : _IFXEXTERN_ IFX_INLINE void Wdg_lSchMTriggerEnter(void) **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to enter critical section during WDGx          **
**                                                triggering                  **
**                                                                            **
*******************************************************************************/
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMTriggerEnter(void);



/*******************************************************************************
** Syntax           : _IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeEnter(void) **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to enter critical section during WDGx          **
**                                                Set Off MOde                **
**                                                                            **
*******************************************************************************/
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeEnter(void);


/*******************************************************************************
** Syntax           : _IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitExit(void)     **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to exit critical section during WDGx           **
**                                                init                        **
**                                                                            **
*******************************************************************************/
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitExit(void);



/*******************************************************************************
** Syntax           :  _IFXEXTERN_ IFX_INLINE void Wdg_lSchMTriggerExit(void) **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to exit critical section during WDGx           **
**                                                triggering                  **
**                                                                            **
*******************************************************************************/
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMTriggerExit(void);


/*******************************************************************************
** Syntax           : _IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeExit(void)  **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to exit critical section in SetOffMode         **
**                                                                            **
*******************************************************************************/
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeExit(void);


#endif

/*******************************************************************************
** Syntax           : extern void Wdg_17_Scu_Init( const Wdg_17_Scu_ConfigType**
**                                                         *ConfigPtr )       **
**                                                                            **
** Service ID       : 0                                                       **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters(in)   : ConfigPtr (Pointer to configuration set)                **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Driver Module Initialization function.This routine      **
**                    initializes the watchdog driver and watchdog hardware   **
**                    i.e it sets the default watchdog mode and timeout       **
**                    period as provided in the configuration set. This       **
**                    configuration set shall be chosen from a limited number **
**                    of statically configured sets.                          **
*******************************************************************************/
extern void Wdg_17_Scu_Init( const Wdg_17_Scu_ConfigType *ConfigPtr ); 


#if ( WDG_MAXQM_TIMERS  != 0U)
/*******************************************************************************
** Syntax           : extern Std_ReturnType Wdg_17_Scu_SetMode                  
**                                             ( WdgIf_ModeType Mode )        **
**                                                                            **
** Service ID       : 1                                                       **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : WdgIf_ModeType Mode                                     **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Std_ReturnType (E_OK/E_NOT_OK)                          **
**                                                                            **
** Description      : By choosing one of a limited number of statically       **
**                    configured settings (e.g. toggle or window watchdog,    **
**                    different timeout periods) the watchdog driver and      **
**                    hardware can be switched between the following three    **
**                    different watchdog Modes Off/Slow/Fast:                 **
**                                                                            **
*******************************************************************************/
extern Std_ReturnType Wdg_17_Scu_SetMode( WdgIf_ModeType Mode );



/*******************************************************************************
** Syntax           : extern void Wdg_17_Scu_SetTriggerCondition(uint16 timeout)
**                                                                            **
** Service ID       : 3                                                       **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : uint16 timeout                                          **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : The function Wdg_17_Scu_SetTriggerCondition shall sets  **
**                    Timeout value (milliseconds) for setting the trigger    **
**                    counter - WDG144                                        **
**                                                                            **
*******************************************************************************/
extern void Wdg_17_Scu_SetTriggerCondition(uint16 timeout);


#endif

#if (WDG_MAXSAFE_TIMERS != 0U)
/*******************************************************************************
** Syntax           : void Wdg_17_Scu_InitAltResetPassword(                   **
**             const Wdg_17_Scu_ConfigType *ConfigPtr, uint16 ResetPassword ) **
**                                                                            **
** Service ID       : 0x08                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters(in)   : ConfigPtr (Pointer to configuration set)                **
**                    ResetPassword (New Initial password to be set )         **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Driver Module Initialization function.This routine      **
**                    initializes the watchdog driver and watchdog hardware   **
**                    i.e it sets the default watchdog mode and timeout       **
**                    period as provided in the configuration set. This       **
**                    configuration set shall be chosen from a limited number **
**                    of statically configured sets. In addition user can     **
**                    provide a start password to be updated.                 **
*******************************************************************************/
extern void Wdg_17_Scu_InitAltResetPassword(
                  const Wdg_17_Scu_ConfigType* ConfigPtr, uint16 ResetPassword);

/*******************************************************************************
** Traceability     :                                                         **
** Syntax           : extern void Wdg_17_Scu_SafeTrigger(uint16 SeqPassword,  **
                                             uint16 TimerCheckValue           **
                                             )                                **
**                                                                            **
**                                                                            **
** Service ID       : 0x06                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Triggers the watchdog hardware.                         **
**                    It has to be called cyclically by some upper layer      **
**                    function (usually the watchdog manager) in order to     **
**                    prevent the watchdog hardware from expiring.            **
**                                                                            **
*******************************************************************************/
extern void Wdg_17_Scu_SafeTrigger(uint16 SeqPassword, uint16 TimerCheckValue );

/*******************************************************************************
** Syntax           :extern Std_ReturnType Wdg_17_Scu_SetOffMode              **
**                                (uint16 SeqPassword1,                       **
**                                 uint16 SeqPassword2)                       **
**                                                                            **
** Service ID       : 7                                                       **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : WdgIf_ModeType Mode                                     **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Std_ReturnType (E_OK/E_NOT_OK)                          **
**                                                                            **
** Description      : By choosing one of a limited number of statically       **
**                    configured settings (e.g. toggle or window watchdog,    **
**                    different timeout periods) the watchdog driver and      **
**                    hardware can be switched between the following three    **
**                    different watchdog Modes Off/Slow/Fast:                 **
**                                                                            **
*******************************************************************************/
extern void Wdg_17_Scu_SetOffMode( uint16 SeqPassword1, uint16 SeqPassword2 );
               
#endif

/*******************************************************************************
** Traceability     : [cover parentID=DS_AS_WDG067, DS_AS_WDG109,DS_AS_WDG174
                      DS_AS_WDG099, DS_AS_WDG068, DS_AS40X_WDG105,
                      DS_NAS_WDG_PR730]                                       **
**                                                                            **
** Syntax           : void Wdg_17_Scu_GetVersionInfo                          **
**                   ( Std_VersionInfoType *VersionInfoPtr)                   **
**                    [/cover]                                                **
**                                                                            **
** Service ID       : 4                                                       **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : VersionInfoPtr (Pointer to store the version            **
**                   information of this module)                              **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This service returns the version information of module. **
**                    The version information includes:                       **
**                    - Module Id                                             **
**                    - Vendor Id                                             **
**                    - Instance Id (Applicable only for WDG SWS 2.2.0)       **
**                    - Vendor specific version numbers (BSW00407)            **
**                                                                            **
*******************************************************************************/
#if ((WDG_VERSION_INFO_API == STD_ON) && (WDG_DEV_ERROR_DETECT == STD_ON))
/*IFX_MISRA_RULE_19_04_STATUS=GetVersionInfo cannot be expand to a 
  braced initialiser */
/*IFX_MISRA_RULE_19_07_STATUS=GetVersionInfo is declared as a function 
  like macro as per AUTOSAR*/
#define Wdg_17_Scu_GetVersionInfo(VersionInfoPtr)                             \
{                                                                              \
  /* Check if the passed parameter is a NULL_PTR.  If so, escalate... */       \
  if((VersionInfoPtr) == NULL_PTR)                                             \
  {                                                                            \
    Det_ReportError(                                                           \
                     (uint16)WDG_17_SCU_MODULE_ID,                             \
                     WDG_17_SCU_INSTANCE_ID,                                   \
                     WDG_SID_GETVERSIONINFO,                                   \
                     WDG_E_PARAM_POINTER                                       \
                   );/* End of report to  DET */                               \
  }                                                                            \
  else                                                                         \
  {                                                                            \
    /* (pointer VersionInfoPtr is not checked for NULL pointer) */             \
    /* Get WDG Vendor ID */                                                    \
    ((Std_VersionInfoType*)(VersionInfoPtr))->vendorID = WDG_17_SCU_VENDOR_ID; \
    /* Get WDG Module ID */                                                    \
    ((Std_VersionInfoType*)(VersionInfoPtr))->moduleID = WDG_17_SCU_MODULE_ID; \
    /* Get WDG module Software major version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_major_version =               \
                                       (uint8)WDG_17_SCU_SW_MAJOR_VERSION;     \
    /* Get WDG module Software minor version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_minor_version =               \
                                       (uint8)WDG_17_SCU_SW_MINOR_VERSION;     \
    /* Get WDG module Software patch version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_patch_version =               \
                                       (uint8)WDG_17_SCU_SW_PATCH_VERSION;     \
  }                                                                            \
}/* end of Wdg_17_Scu_GetVersionInfo() */
#elif ((WDG_VERSION_INFO_API == STD_ON) && (WDG_DEV_ERROR_DETECT == STD_OFF))
/*IFX_MISRA_RULE_19_07_STATUS=GetVersionInfo is declared as a function
 like macro as per AUTOSAR.*/
/*IFX_MISRA_RULE_19_04_STATUS=GetVersionInfo cannot be expand to a braced 
 initialiser*/ 
#define Wdg_17_Scu_GetVersionInfo(VersionInfoPtr)                              \
{                                                                              \
  if((VersionInfoPtr) != NULL_PTR)                                             \
  {                                                                            \
    /* Get WDG Vendor ID */                                                    \
    ((Std_VersionInfoType*)(VersionInfoPtr))->vendorID = WDG_17_SCU_VENDOR_ID; \
    /* Get WDG Module ID */                                                    \
    ((Std_VersionInfoType*)(VersionInfoPtr))->moduleID = WDG_17_SCU_MODULE_ID; \
    /* Get WDG module Software major version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_major_version =               \
                                         (uint8)WDG_17_SCU_SW_MAJOR_VERSION;   \
    /* Get WDG module Software minor version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_minor_version =               \
                                         (uint8)WDG_17_SCU_SW_MINOR_VERSION;   \
    /* Get WDG module Software patch version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_patch_version =               \
                                         (uint8)WDG_17_SCU_SW_PATCH_VERSION;   \
  }                                                                            \
}/* end of Wdg_17_Scu_GetVersionInfo() */
#else
/*IFX_MISRA_RULE_19_04_STATUS=GetVersionInfo cannot be 
  expand to a braced initialiser*/
/*IFX_MISRA_RULE_19_07_STATUS=GetVersionInfo is declared as a function 
  like macro as per AUTOSAR*/
#define Wdg_17_Scu_GetVersionInfo(VersionInfoPtr)                              \
ERROR_Wdg_17_Scu_GetVersionInfo_NOT_SELECTED
#endif



/******************************************************************************/

/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
#if (WDG_MAXSAFE_TIMERS != 0U)
/*******************************************************************************
** Syntax           :_IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitEnter(void)     **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to enter critical section during initialization**
**                                                                            **
*******************************************************************************/
/*Function to enter critical section during initialization */
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitEnter(void)
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions 
  defined in header files*/
{
  SchM_Enter_Wdg_17_Scu_SafetyInit();
}
/*******************************************************************************
** Syntax           :  IFX_INLINE void Wdg_lSchMTriggerEnter(void)            **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to enter critical section during WDGx          **
**                                                triggering                  **
**                                                                            **
*******************************************************************************/
/*Function to enter critical section during WDGx triggering */
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMTriggerEnter(void)
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions 
  defined in header files*/
{
  SchM_Enter_Wdg_17_Scu_SafetyTrigger();
}

/*******************************************************************************
** Syntax           :_IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeEnter(void)  **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to enter critical section during WDGx          **
**                                                Set Off MOde                **
**                                                                            **
*******************************************************************************/
/*Function to enter critical section in SetOffMode */
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeEnter(void)
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions 
  defined in header files*/
{
  SchM_Enter_Wdg_17_Scu_SafetyOffMode();
}
/*******************************************************************************
** Syntax           :_IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitExit(void)      **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to exit critical section during WDGx           **
**                                                init                        **
**                                                                            **
*******************************************************************************/
/*Function to exit critical section during initialization */
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMInitExit(void)
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions 
  defined in header files*/
{
  SchM_Exit_Wdg_17_Scu_SafetyInit();
}

/*******************************************************************************
** Syntax           : _IFXEXTERN_ IFX_INLINE void Wdg_lSchMTriggerExit(void)  **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to exit critical section during WDGx           **
**                                                triggering                  **
**                                                                            **
*******************************************************************************/
/*Function to exit critical section during WDGx triggering */
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMTriggerExit(void)
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions 
  defined in header files*/
{
  SchM_Exit_Wdg_17_Scu_SafetyTrigger();
}

/*******************************************************************************
** Syntax           : _IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeExit(void)  **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-reentrant                                           **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to exit critical section in SetOffMode         **
**                                                                            **
*******************************************************************************/
/*Function to exit critical section in SetOffMode */
_IFXEXTERN_ IFX_INLINE void Wdg_lSchMOffModeExit(void)
/*IFX_MISRA_RULE_08_05_STATUS=Allowed for inline functions 
  defined in header files*/
{
  SchM_Exit_Wdg_17_Scu_SafetyOffMode();
}

#endif
















#define WDG_17_SCU_STOP_SEC_CODE
#include "MemMap.h"

#endif /* WDG_H */
