#ifndef Rte_EcuAbs_H
#define Rte_EcuAbs_H

#include "Rte_Type.h"

extern void Rte_Call_EcuAbs_rDcmCmd_getDiagStatus(P_stDiagStatus pstDiagStatus);

void REcbEcuAbs_initialize(void);
void REtEcuAbs_ecuAbsMain(void);

void REoiEcuAbs_pEcuAbsCan_setPduInfo(uint8 handle, uint8 length, uint32 id, P_uint8 buffer);
void REoiEcuAbs_pEcuAbsCan_canSend(uint8 handle, P_uint8 buffer);
void REoiEcuAbs_pEcuAbsNvm_readNvmData(uint16 block, P_uint8 data);
void REoiEcuAbs_pEcuAbsNvm_writeNvmData(uint16 block, P_uint8 data);


#endif
