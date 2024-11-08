#include "Rte_BswDcm.h"
#include "Definition.h"
#include "UdsNrcList.h"
#include "Utility.h"

#define FRAME_TYPE_SINGLE 0u
#define TIME_DIAG_P2_CAN_SERVER		TIME_50MS
#define TIME_DIAG_P2_MUL_CAN_SERVER	TIME_5S
#define REF_VALUE_FILLER_BYTE		0xAAu

#define SID_DiagSessionControl				0x10u
#define SID_EcuReset						0x11u
#define SID_SecurityAccess					0x27u
#define SID_CommunicationControl			0x28u
#define SID_RoutineControl					0x31u
#define SID_WriteDataByIdentifier			0x2Eu
#define SID_ReadDataByIdentifier			0x22u

#define SFID_DiagnosticSessionControl_DefaultSession						0x01u
#define SFID_DiagnosticSessionControl_ProgrammingSession		            0x02u
#define SFID_DiagnosticSessionControl_ExtendedSession						0x03u
#define SFID_EcuReset_HardReset 											0x01u
#define SFID_SecurityAccess_RequestSeed										0x01u
#define SFID_SecurityAccess_SendKey											0x02u
#define SFID_CommunicationControl_enableRxAndTx								0x00u
#define SFID_CommunicationControl_disableRxAndTx							0x03u
#define SFID_RoutineControl_Start						0x01u
#define DID_VariantCode 0xF101u
#define RID_AliveCountIncrease					0xF000u
#define RID_AliveCountDecrease					0xF001u

#define CommunicationType_normalCommunicationMessages		0x01u

/* DIAGNOSTIC_SESSION */
//enum
//{
//	DIAG_SESSION_DEFAULT,
//	DIAG_SESSION_EXTENDED
//};

typedef struct {
	ST_DiagStatus stDiagStatus;
    uint8 isServiceRequest;
    uint8 isPhysical;
    uint8 diagDataBufferRx[MAX_CAN_MSG_DLC];
    uint8 diagDataBufferTx[MAX_CAN_MSG_DLC];
    uint8 diagDataLengthRx;
    uint8 diagDataLengthTx;
}ST_BswDcmData;

static ST_BswDcmData stBswDcmData;

static uint8 diagnosticSessionControl(void);
static uint8 ecuReset(void);
static uint8 securityAccess(void);
static uint8 communicationControl(void);
static uint8 routineControl(void);
static uint8 writeDataByIdentifier(void);
static uint8 readDataByIdentifier(void);

void REcbBswDcm_initialize(void)
{
	(void) 0;
}

void REtBswDcm_bswDcmMain(void)
{
	uint8 serviceId;
	uint8 errorResult = FALSE;
	if(stBswDcmData.isServiceRequest == TRUE)
	{
		stBswDcmData.isServiceRequest = FALSE;
		serviceId = stBswDcmData.diagDataBufferRx[0];
		if(serviceId == SID_DiagSessionControl)
		{
			errorResult = diagnosticSessionControl();
		}
		else if(serviceId == SID_EcuReset)
		{
			errorResult = ecuReset();
		}
		else if(serviceId == SID_SecurityAccess)
		{
			errorResult = securityAccess();
		}
		else if(serviceId == SID_CommunicationControl)
		{
			errorResult = communicationControl();
		}
		else if(serviceId == SID_RoutineControl)
		{
			errorResult = routineControl();
		}
		else if(serviceId == SID_WriteDataByIdentifier)
		{
			errorResult = writeDataByIdentifier();
		}
		else if(serviceId == SID_ReadDataByIdentifier)
		{
			errorResult = readDataByIdentifier();
		}
		else
		{
			errorResult = NRC_ServiceNotSupported;
		}

		if(errorResult != FALSE)
		{
			stBswDcmData.diagDataLengthTx = 3u;
			stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
			stBswDcmData.diagDataBufferTx[1] = 0x7Fu;
			stBswDcmData.diagDataBufferTx[2] = stBswDcmData.diagDataBufferRx[0];
			stBswDcmData.diagDataBufferTx[3] = errorResult;
		}

		for(uint8 i=stBswDcmData.diagDataLengthTx+1u; i<TX_MSG_LEN_PHY_RES; i++)
		{
			stBswDcmData.diagDataBufferTx[i] = REF_VALUE_FILLER_BYTE;
		}
		Rte_Call_BswDcm_rEcuAbsCan_canSend(TX_MSG_IDX_PHY_RES, stBswDcmData.diagDataBufferTx);
	}
}

void REoiBswDcm_pDcmCmd_processDiagRequest(uint8 isPhysical, P_uint8 diagReq, uint8 length)
{
	(void) length;
	uint8_t frameType = diagReq[0] >> 4u;
	if (frameType == FRAME_TYPE_SINGLE)
	{
		memset(stBswDcmData.diagDataBufferRx, 0, MAX_CAN_MSG_DLC);
		stBswDcmData.diagDataLengthRx = (diagReq[0] & 0x0Fu);
		stBswDcmData.isPhysical = isPhysical;
		(void)memcpy(stBswDcmData.diagDataBufferRx, &diagReq[1], stBswDcmData.diagDataLengthRx);
		stBswDcmData.isServiceRequest = TRUE;
	}
}

void REoiBswDcm_pDcmCmd_getDiagStatus(P_stDiagStatus pstDiagStatus)
{
	(void)memcpy(pstDiagStatus, &stBswDcmData.stDiagStatus, sizeof(ST_DiagStatus));
}

static uint8 diagnosticSessionControl(void)
{
	uint8 errorResult = FALSE;
	uint8 subFunctionId = stBswDcmData.diagDataBufferRx[1] & 0x7Fu;
	if(stBswDcmData.diagDataLengthRx != 2u)
	{
		errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
	}
	else if(subFunctionId == SFID_DiagnosticSessionControl_DefaultSession)
	{
		stBswDcmData.stDiagStatus.currentSession = DIAG_SESSION_DEFAULT;
	}
	else if(subFunctionId == SFID_DiagnosticSessionControl_ProgrammingSession)
	{
		stBswDcmData.stDiagStatus.currentSession = DIAG_SESSION_PROGRAMMING;
	}
	else if(subFunctionId == SFID_DiagnosticSessionControl_ExtendedSession)
	{
		stBswDcmData.stDiagStatus.currentSession = DIAG_SESSION_EXTENDED;
	}
	else
	{
		errorResult = NRC_SubFunctionNotSupported;
	}
	if(errorResult == FALSE)
	{
		stBswDcmData.diagDataLengthTx = 6u;
		stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
		stBswDcmData.diagDataBufferTx[1] = SID_DiagSessionControl + 0x40u;
		stBswDcmData.diagDataBufferTx[2] = subFunctionId;
		stBswDcmData.diagDataBufferTx[3] = (TIME_DIAG_P2_CAN_SERVER & 0xFF00u) >> 8u;		/* High Byte : TIME_DIAG_P2_CAN_SERVER */
		stBswDcmData.diagDataBufferTx[4] = (TIME_DIAG_P2_CAN_SERVER & 0xFFu);				/* Low Byte : TIME_DIAG_P2_CAN_SERVER */
		stBswDcmData.diagDataBufferTx[5] = (uint8)(((TIME_DIAG_P2_MUL_CAN_SERVER/10u) & 0xFF00u) >> 8u);	/* High Byte : P2*CAN_SERVER */
		stBswDcmData.diagDataBufferTx[6] = ((TIME_DIAG_P2_MUL_CAN_SERVER/10u) & 0xFFu);			/* Low Byte : P2*CAN_SERVER */
	}
	return errorResult;
}

static uint8 ecuReset(void)
{
	uint8 subFunctionId = stBswDcmData.diagDataBufferRx[1] & 0x7Fu;
	uint8 errorResult = FALSE;

	if (stBswDcmData.diagDataLengthRx != 2u)
	{
		errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
	}
	else if(subFunctionId == SFID_EcuReset_HardReset)
	{
		stBswDcmData.stDiagStatus.isEcuReset = TRUE;
	}
	else
	{
		errorResult = NRC_SubFunctionNotSupported;
	}
	if(errorResult == FALSE)
	{
		stBswDcmData.diagDataLengthTx = 2u;
		stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
		stBswDcmData.diagDataBufferTx[1] = SID_EcuReset + 0x40u;
		stBswDcmData.diagDataBufferTx[2] = subFunctionId;
	}
	return errorResult;
}

static uint8 securityAccess(void)
{
	uint8 subFunctionId = stBswDcmData.diagDataBufferRx[1];
	static uint16 genKey;
	uint16 seed, key;
	uint8 errorResult = FALSE;

	stBswDcmData.stDiagStatus.isSecurityUnlock = FALSE;
	if(stBswDcmData.stDiagStatus.currentSession == DIAG_SESSION_DEFAULT)
	{
		errorResult = NRC_ServiceNotSupportedInActiveSession;
	}
	else if(subFunctionId == SFID_SecurityAccess_RequestSeed)
	{
		if(stBswDcmData.diagDataLengthRx != 2u)
		{
			errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
		}
		else
		{
		    if(UT_getTimeEcuAlive1ms() % 3 == 1)
		    {
		        seed = 0x1234;
		        genKey = 0x4567;
		    }
		    else if(UT_getTimeEcuAlive1ms() % 3 == 2)
		    {
		        seed = 0x2345;
		        genKey = 0x5678;
		    }
		    else
		    {
		        seed = 0x3456;
		        genKey = 0x6789;
		    }
			stBswDcmData.diagDataLengthTx = 4u;
			stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
			stBswDcmData.diagDataBufferTx[1] = SID_SecurityAccess + 0x40u;
			stBswDcmData.diagDataBufferTx[2] = subFunctionId;
			stBswDcmData.diagDataBufferTx[3] = (uint8)((seed & 0xFF00u) >> 8u);
			stBswDcmData.diagDataBufferTx[4] = (uint8)(seed & 0xFFu);
		}
	}
	else if(subFunctionId == SFID_SecurityAccess_SendKey)
	{
		if(stBswDcmData.diagDataLengthRx != 4u)
		{
			errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
		}
		else
		{
			key = ((uint16)stBswDcmData.diagDataBufferRx[2] << 8u) | stBswDcmData.diagDataBufferRx[3];
			if(key == genKey)
			{
				stBswDcmData.diagDataLengthTx = 2u;
				stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
				stBswDcmData.diagDataBufferTx[1] = SID_SecurityAccess + 0x40u;
				stBswDcmData.diagDataBufferTx[2] = subFunctionId;
				stBswDcmData.stDiagStatus.isSecurityUnlock = TRUE;
			}
			else
			{
				errorResult = NRC_InvalidKey;
			}
		}
	}
	else
	{
		errorResult = NRC_SubFunctionNotSupported;
	}
	return errorResult;
}

static uint8 communicationControl(void)
{
	uint8 subFunctionId = stBswDcmData.diagDataBufferRx[1] & 0x7Fu;
	uint8 errorResult = FALSE;
	uint8 communicationType = stBswDcmData.diagDataBufferRx[2];

	if(stBswDcmData.diagDataLengthRx != 3u)
	{
		errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
	}
	else if(stBswDcmData.stDiagStatus.currentSession != DIAG_SESSION_EXTENDED)
	{
		errorResult = NRC_SubFunctionNotSupportedInActiveSession;
	}
	else if(communicationType != CommunicationType_normalCommunicationMessages)
	{
		errorResult = NRC_RequestOutRange;
	}
	else if(subFunctionId == SFID_CommunicationControl_enableRxAndTx)
	{
		stBswDcmData.stDiagStatus.isCommunicationDisable = FALSE;
		stBswDcmData.diagDataLengthTx = 2u;
		stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
		stBswDcmData.diagDataBufferTx[1] = SID_CommunicationControl + 0x40u;
		stBswDcmData.diagDataBufferTx[2] = subFunctionId;
	}
	else if(subFunctionId == SFID_CommunicationControl_disableRxAndTx)
	{
		stBswDcmData.stDiagStatus.isCommunicationDisable = TRUE;
		stBswDcmData.diagDataLengthTx = 2u;
		stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
		stBswDcmData.diagDataBufferTx[1] = SID_CommunicationControl + 0x40u;
		stBswDcmData.diagDataBufferTx[2] = subFunctionId;
	}
	else
	{
		errorResult = NRC_SubFunctionNotSupported;
	}
	return errorResult;
}

static uint8 routineControl(void)
{
	uint8 errorResult = FALSE;
	uint8 subFunctionId = stBswDcmData.diagDataBufferRx[1];
	uint16_t routineIdentifier = ((uint16_t)stBswDcmData.diagDataBufferRx[2] << 8) | (uint16_t)stBswDcmData.diagDataBufferRx[3];

	if(stBswDcmData.diagDataLengthRx != 4u)
	{
		errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
	}
	else if(subFunctionId != SFID_RoutineControl_Start)
	{
		errorResult = NRC_SubFunctionNotSupported;
	}
	else if (routineIdentifier == RID_AliveCountIncrease)
	{
		stBswDcmData.stDiagStatus.isAliveCountDecrease = FALSE;
	}
	else if (routineIdentifier == RID_AliveCountDecrease)
	{
		stBswDcmData.stDiagStatus.isAliveCountDecrease = TRUE;
	}
	else
	{
		errorResult = NRC_RequestOutRange;
	}

	if(errorResult == FALSE)
	{
		stBswDcmData.diagDataLengthTx = 4u;
		stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
		stBswDcmData.diagDataBufferTx[1] = SID_RoutineControl + 0x40u;
		stBswDcmData.diagDataBufferTx[2] = subFunctionId;
		stBswDcmData.diagDataBufferTx[3] = (uint8)((routineIdentifier & 0xFF00) >> 8u);
		stBswDcmData.diagDataBufferTx[4] = (uint8)(routineIdentifier & 0xFFu);
	}
	return errorResult;
}

static uint8 writeDataByIdentifier(void)
{
	uint8 variantCode[DATA_LEN_VariantCode] = {0u,};
	uint8 errorResult = FALSE;
	uint16 dataIdentifier = ((uint16_t)stBswDcmData.diagDataBufferRx[1] << 8) | (uint16_t)stBswDcmData.diagDataBufferRx[2];

	if(stBswDcmData.stDiagStatus.currentSession != DIAG_SESSION_EXTENDED)
	{
	    errorResult = NRC_ServiceNotSupportedInActiveSession;
	}
	else if(stBswDcmData.stDiagStatus.isSecurityUnlock == FALSE)
    {
        errorResult = NRC_SecurityAccessDenied;
    }
	else if(dataIdentifier == DID_VariantCode)
	{
		if(stBswDcmData.diagDataLengthRx != 7u)
		{
			errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
		}
		else
		{
			variantCode[0] = stBswDcmData.diagDataBufferRx[3];
			variantCode[1] = stBswDcmData.diagDataBufferRx[4];
			variantCode[2] = stBswDcmData.diagDataBufferRx[5];
			variantCode[3] = stBswDcmData.diagDataBufferRx[6];
			Rte_Call_BswDcm_rEcuAbsNvm_writeNvmData(DATA_ID_VariantCode, variantCode);
			stBswDcmData.diagDataLengthTx = 3u;
			stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
			stBswDcmData.diagDataBufferTx[1] = SID_WriteDataByIdentifier + 0x40u;
			stBswDcmData.diagDataBufferTx[2] = (uint8)((dataIdentifier & 0xFF00) >> 8u);
			stBswDcmData.diagDataBufferTx[3] = (uint8)(dataIdentifier & 0xFFu);
		}
	}
	else
	{
		errorResult = NRC_RequestOutRange;
	}
	return errorResult;
}

static uint8 readDataByIdentifier(void)
{
	uint8 variantCode[DATA_LEN_VariantCode] = {0u,};
	uint8 errorResult = FALSE;
	uint16 dataIdentifier = ((uint16_t)stBswDcmData.diagDataBufferRx[1] << 8) | (uint16_t)stBswDcmData.diagDataBufferRx[2];

	if(stBswDcmData.diagDataLengthRx != 3u)
	{
		errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
	}
	else if(dataIdentifier == DID_VariantCode)
	{
		Rte_Call_BswDcm_rEcuAbsNvm_readNvmData(DATA_ID_VariantCode, variantCode);
		stBswDcmData.diagDataLengthTx = 7u;
		stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
		stBswDcmData.diagDataBufferTx[1] = SID_ReadDataByIdentifier + 0x40u;
		stBswDcmData.diagDataBufferTx[2] = (uint8)((dataIdentifier & 0xFF00) >> 8u);
		stBswDcmData.diagDataBufferTx[3] = (uint8)(dataIdentifier & 0xFFu);
		stBswDcmData.diagDataBufferTx[4] = variantCode[0];
		stBswDcmData.diagDataBufferTx[5] = variantCode[1];
		stBswDcmData.diagDataBufferTx[6] = variantCode[2];
		stBswDcmData.diagDataBufferTx[7] = variantCode[3];
	}
	else
	{
		errorResult = NRC_RequestOutRange;
	}
	return errorResult;
}
