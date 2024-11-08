#include "CanIf_Cbk.h"
#include "IfxCan_reg.h"
#include "Can_17_MCanP.h"
#include "Utility.h"

void CanIf_RxIndication(Can_HwHandleType Hrh, Can_IdType CanId, uint8 CanDlc, const uint8 *CanSduPtr)
{
    UT_canRxIndication(Hrh, CanDlc, CanId, CanSduPtr);
}

void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
    UT_canTxConfirmation(CanTxPduId);
}

void CanIf_ControllerBusOff(uint8 ControllerId)
{
	(void) 0;
}

void CanIf_ControllerModeIndication( uint8 ControllerId, CanIf_ControllerModeType ControllerMode )
{
	(void)0;
}
