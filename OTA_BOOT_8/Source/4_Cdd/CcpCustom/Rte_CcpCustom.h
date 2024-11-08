#ifndef Rte_CcpCustom_H
#define Rte_CcpCustom_H

#include "Rte_Type.h"

extern void Rte_Call_CcpCustom_rEcuAbsCan_canSend(uint8 handle, P_uint8 buffer);

void REcbCcpCustom_initialize(void);
void REtCcpCustom_ccpCustomTask1ms(void);
void REoiCcpCustom_pCcpCustomCmd_processCcpRequest(P_uint8 ccpReq);

#endif
