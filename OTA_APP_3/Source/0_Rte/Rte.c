#include "Rte_Type.h"
#include "Rte_EcuAbs.h"
#include "Rte_BswDcm.h"
#include "Rte_CcpCustom.h"

void Rte_Call_BswCom_rEcuAbsCan_setPduInfo(uint8 handle, uint8 length, uint32 id, P_uint8 buffer)
{
	REoiEcuAbs_pEcuAbsCan_setPduInfo(handle, length, id, buffer);
}

void Rte_Call_BswCom_rEcuAbsCan_canSend(uint8 handle, P_uint8 buffer)
{
	REoiEcuAbs_pEcuAbsCan_canSend(handle, buffer);
}

void Rte_Call_BswCom_rDcmCmd_processDiagRequest(uint8 isPhysical, P_uint8 diagReq, uint8 length)
{
	REoiBswDcm_pDcmCmd_processDiagRequest(isPhysical, diagReq, length);
}

void Rte_Call_BswCom_rDcmCmd_getDiagStatus(P_stDiagStatus pstDiagStatus)
{
    REoiBswDcm_pDcmCmd_getDiagStatus(pstDiagStatus);
}

void Rte_Call_BswCom_rCcpCustomCmd_processCcpRequest(P_uint8 ccpReq)
{
    REoiCcpCustom_pCcpCustomCmd_processCcpRequest(ccpReq);
}

void Rte_Call_BswDcm_rEcuAbsCan_canSend(uint8 handle, P_uint8 buffer)
{
    REoiEcuAbs_pEcuAbsCan_canSend(handle, buffer);
}

void Rte_Call_BswDcm_rEcuAbsNvm_readNvmData(uint16 block, P_uint8 data)
{
    REoiEcuAbs_pEcuAbsNvm_readNvmData(block, data);
}

void Rte_Call_BswDcm_rEcuAbsNvm_writeNvmData(uint16 block, P_uint8 data)
{
    REoiEcuAbs_pEcuAbsNvm_writeNvmData(block, data);
}

void Rte_Call_EcuAbs_rDcmCmd_getDiagStatus(P_stDiagStatus pstDiagStatus)
{
    REoiBswDcm_pDcmCmd_getDiagStatus(pstDiagStatus);
}

void Rte_Call_CcpCustom_rEcuAbsCan_canSend(uint8 handle, P_uint8 buffer)
{
    REoiEcuAbs_pEcuAbsCan_canSend(handle, buffer);
}
