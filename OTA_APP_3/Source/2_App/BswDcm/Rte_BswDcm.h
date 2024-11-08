#ifndef Rte_BswDcm_H
#define Rte_BswDcm_H

#include "Rte_Type.h"

extern void Rte_Call_BswDcm_rEcuAbsCan_canSend(uint8 handle, P_uint8 buffer);
extern void Rte_Call_BswDcm_rEcuAbsNvm_readNvmData(uint16 block, P_uint8 data);
extern void Rte_Call_BswDcm_rEcuAbsNvm_writeNvmData(uint16 block, P_uint8 data);

void REcbBswDcm_initialize(void);
void REtBswDcm_bswDcmMain(void);
void REoiBswDcm_pDcmCmd_processDiagRequest(uint8 isPhysical, P_uint8 diagReq, uint8 length);
void REoiBswDcm_pDcmCmd_getDiagStatus(P_stDiagStatus pstDiagStatus);


#endif
