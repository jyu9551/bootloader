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
**  $FILENAME   : Mcu_Crc.c $                                                **
**                                                                            **
**  $CC VERSION : \main\dev_tc23x\5 $                                        **
**                                                                            **
**  $DATE       : 2015-08-13 $                                               **
**                                                                            **
**  AUTHOR      : DL-AUTOSAR-Engineering                                      **
**                                                                            **
**  VENDOR      : Infineon Technologies                                       **
**                                                                            **
**  DESCRIPTION : This file contains basic initialization of CRC(FCE) module. **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: No                                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include "Mcu.h"
#include "Mcu_Local.h"

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

#if (MCU_CRC_HW_USED == STD_ON)  

/*Memory Map of the MCU Code*/
#define MCU_START_SEC_CODE
#include "MemMap.h"


/*******************************************************************************
** Syntax : Std_ReturnType Mcu_lCrcInit (void)                                **
**                                                                            **
** Service ID:    None                                                        **
**                                                                            **
** Sync/Async:    Synchronous                                                 **
**                                                                            **
** Reentrancy:    Non-reentrant                                               **
**                                                                            **
** Parameters (in):   None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value:      E_OK                                                    **
**                                                                            **
** Description :  This service shall initialize CRC HW if present.            **
**                                                                            **
*******************************************************************************/
Std_ReturnType Mcu_lCrcInit(void)
{
  return(E_OK);
}/*End of Mcu_lCrcInit()*/

#if (MCU_DEINIT_API == STD_ON)
/*******************************************************************************
** Syntax : void Mcu_lCrcDeInit (void)                                        **
**                                                                            **
** Service ID:    None                                                        **
**                                                                            **
** Sync/Async:    Synchronous                                                 **
**                                                                            **
** Reentrancy:    Non-reentrant                                               **
**                                                                            **
** Parameters (in):   None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value:      None                                                    **
**                                                                            **
** Description :  This service shall de-initialize FCE and
                  remove the clock connection                                 **
**                                                                            **
*******************************************************************************/
void Mcu_lCrcDeInit(void)
{
}/*End of Mcu_lCrcDeInit()*/
#endif

#if (MCU_SAFETY_ENABLE == STD_ON)
/*******************************************************************************
** Syntax : Std_ReturnType Mcu_lCrcInitCheck (void)                           **
**                                                                            **
** Service ID:    None                                                        **
**                                                                            **
** Sync/Async:    Synchronous                                                 **
**                                                                            **
** Reentrancy:    Non-reentrant                                               **
**                                                                            **
** Parameters (in):   None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :    E_OK - if initialization comparison is success        **
**                      E_NOT_OK - if initialization comparison fails         **
**                                                                            **
** Description :  This service shall verify the CRC specific                  **
**                initialization done by MCU.                                 **
**                                                                            **
*******************************************************************************/
Std_ReturnType Mcu_lCrcInitCheck(void)
{ 
  return E_OK;
}/*End of Mcu_lCrcInitCheck()*/

#endif /*End Of MCU_SAFETY_ENABLE*/

#define MCU_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives
 is allowed only for MemMap.h*/
#include "MemMap.h"

#endif /*#if (MCU_CRC_HW_USED == STD_ON) */
