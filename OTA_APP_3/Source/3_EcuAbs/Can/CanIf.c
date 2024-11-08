/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
/* Inclusion of CanIf module header file  */
#include "CanIf.h"


/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

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
** Syntax : void CanIf_SetWakeupEvent(uint8 CanNetwork,                       **
**                                    CanIf_WakeupSourceType WakeupSource )   **
**                                                                            **
** Service ID: none                                                           **
**                                                                            **
** Sync/Async:  none                                                          **
**                                                                            **
** Reentrancy:  none                                                          **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out):  none                                                    **
**                                                                            **
** Return value:  none                                                        **
**                                                                            **
** Description : This function is called in CanTrcv_MainFunction or           **
**               CanTrcv_CB_WakeupByBus function.                             **
**                                                                            **
*******************************************************************************/
#if(CAN_WAKEUP_CONFIGURED == STD_ON)
void CanIf_SetWakeupEvent(uint8 CanNetwork,
                                   CanIf_WakeupSourceType WakeupSource )
{
	UNUSED_PARAMETER(CanNetwork)
	UNUSED_PARAMETER(WakeupSource)
}


void CanIf_CheckWakeup(CanIf_WakeupSourceType WakeupSource )
{

UNUSED_PARAMETER(WakeupSource)
	
}

void CanIf_TrcvModeIndication(uint8 Transceiver,CanTrcv_TrcvModeType OpMode)
{
	UNUSED_PARAMETER(Transceiver)
	UNUSED_PARAMETER(OpMode)
  // AB print_f("\n Callback function CanIf_TrcvModeIndication is called!\n");
  // AB print_f(" Transceiver = %d \n", Transceiver);
  // AB print_f(" OpMode = %d \n", OpMode);
}

void CanIf_CheckTrcvWakeFlagIndication(uint8 Transceiver)
{
		UNUSED_PARAMETER(Transceiver)
    // AB print_f("\n Callback function CanIf_CheckTrcvWakeFlagIndication is called!\n");
    // AB print_f(" Transceiver = %d \n", Transceiver);
}
#endif
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
