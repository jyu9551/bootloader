#ifndef Rte_BswCom_H
#define Rte_BswCom_H

#include "Rte_Type.h"

extern void Rte_Call_BswCom_rEcuAbsCan_setPduInfo(uint8 handle, uint8 length, uint32 id, P_uint8 pu8Buffer);
extern void Rte_Call_BswCom_rEcuAbsCan_canSend(uint8 handle, P_uint8 buffer);
extern void Rte_Call_BswCom_rDcmCmd_processDiagRequest(uint8 isPhysical, P_uint8 diagReq, uint8 length);
extern void Rte_Call_BswCom_rDcmCmd_getDiagStatus(P_stDiagStatus pstDiagStatus);
extern void Rte_Call_BswCom_rCcpCustomCmd_processCcpRequest(P_uint8 ccpReq);

void REcbBswCom_initialize(void);
void REtBswCom_bswComMain(void);
void REcbBswCom_canRxIndication(void);

#endif
