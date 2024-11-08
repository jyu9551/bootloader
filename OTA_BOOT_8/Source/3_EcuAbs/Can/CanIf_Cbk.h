#ifndef CANIF_CBK_H
#define CANIF_CBK_H

#include "ComStack_Types.h"
#include "CanIf.h"

extern void CanIf_ControllerBusOff(uint8 ControllerId);
extern void CanIf_RxIndication(Can_HwHandleType Hrh, Can_IdType CanId, uint8 CanDlc, const uint8 *CanSduPtr);
extern void CanIf_TxConfirmation(PduIdType CanTxPduId);
extern void CanIf_ControllerModeIndication(uint8 ControllerId, CanIf_ControllerModeType ControllerMode);

#endif
