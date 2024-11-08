#include "EcuAbsCan.h"
#include "Can_GeneralTypes.h"
#include "Can_17_MCanP.h"
#include "Definition.h"

Can_PduType PduInfo[TOTAL_CAN_MSG_COUNT];

void EA_initCan(void)
{
	(void)Can_17_MCanP_SetControllerMode(CAN_HWCONTROLLER0, CAN_T_START);
	Can_17_MCanP_EnableControllerInterrupts(CAN_HWCONTROLLER0);
}

void EA_mainCan(void)
{
    (void) 0;
}

void EA_setPduInfo(uint8 handle, uint8 length, uint32 id, P_uint8 buffer)
{
	PduInfo[handle].swPduHandle = handle;
	PduInfo[handle].length = length;
	PduInfo[handle].id = id;
	PduInfo[handle].sdu = buffer;
}

void EA_canSend(uint8 handle, CP_uint8 buffer)
{
	(void) memcpy(PduInfo[handle].sdu, buffer, PduInfo[handle].length);
	(void) Can_17_MCanP_Write(handle, &PduInfo[handle]);
}
