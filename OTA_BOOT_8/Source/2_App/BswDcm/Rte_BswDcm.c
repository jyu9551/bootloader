#include "Rte_BswDcm.h"
#include "Definition.h"
#include "UdsNrcList.h"
#include "Utility.h"
#include "KISA_SHA256.h"

#define DIAG_BUFFER_SIZE 4096u

#define FRAME_TYPE_SINGLE 0u
#define FRAME_TYPE_FIRST        1u
#define FRAME_TYPE_CONSECUTIVE  2u
#define FRAME_TYPE_FLOW_CONTROL 3u

#define REF_VALUE_FLOW_STATUS_CTS   0u
#define REF_VALUE_BLOCK_SIZE        0u
#define REF_VALUE_ST_MIN            0u

#define TIME_DIAG_P2_CAN_SERVER		TIME_50MS
#define TIME_DIAG_P2_MUL_CAN_SERVER	TIME_5S
#define TIME_DIAG_S3_SERVER         TIME_5S
#define REF_VALUE_FILLER_BYTE		0xAAu
#define MAX_NUMBER_OF_BLOCK_LENGTH  (512u+2u)

#define SID_DiagSessionControl				0x10u
#define SID_EcuReset						0x11u
#define SID_SecurityAccess					0x27u
#define SID_CommunicationControl			0x28u
#define SID_RoutineControl					0x31u
#define SID_WriteDataByIdentifier			0x2Eu
#define SID_ReadDataByIdentifier			0x22u
#define SID_RequestDownload                 0x34u
#define SID_TransferData                    0x36u
#define SID_RequestTransferExit             0x37u

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
#define RID_CheckProgrammingDependencies        0x0001u
#define RID_EraseMemory                         0x0002u
#define RCOR_EraseMemory_App                    0xF001u

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
    uint8 diagDataBufferRx[DIAG_BUFFER_SIZE];
    uint8 diagDataBufferTx[MAX_CAN_MSG_DLC];
    uint16 diagDataLengthRx;
    uint8 diagDataLengthTx;
    uint32 timeS3ServerStart;
	uint16 firstFrameDataLength;
    uint32 downloadMemoryAddress;
    uint32 downloadMemorySize;
    uint32 downloadDataLength;
    uint8 downloadBlockSequence;
    SHA256_INFO info;
}ST_BswDcmData;

static ST_BswDcmData stBswDcmData;

static uint8 diagnosticSessionControl(void);
static uint8 ecuReset(void);
static uint8 securityAccess(void);
static uint8 communicationControl(void);
static uint8 routineControl(void);
static uint8 writeDataByIdentifier(void);
static uint8 readDataByIdentifier(void);
static uint8 requestDownload(void);
static uint8 transferData(void);
static uint8 requestTransferExit(void);

void REcbBswDcm_initialize(void)
{
    stBswDcmData.stDiagStatus.currentSession = DIAG_SESSION_PROGRAMMING;
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
        else if(serviceId == SID_RequestDownload)
        {
            errorResult = requestDownload();
        }
        else if(serviceId == SID_TransferData)
        {
            errorResult = transferData();
        }
        else if(serviceId == SID_RequestTransferExit)
        {
            errorResult = requestTransferExit();
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
	if(UT_getTimeEcuAlive1ms() > (stBswDcmData.timeS3ServerStart + TIME_DIAG_S3_SERVER))
	{
	    stBswDcmData.stDiagStatus.currentSession = DIAG_SESSION_DEFAULT;
	}
}

void REoiBswDcm_pDcmCmd_processDiagRequest(uint8 isPhysical, P_uint8 diagReq, uint8 length)
{
    uint8_t frameType;
    stBswDcmData.timeS3ServerStart = UT_getTimeEcuAlive1ms();

    frameType = diagReq[0] >> 4u;
    if (frameType == FRAME_TYPE_SINGLE)
    {
        stBswDcmData.isPhysical = isPhysical;
        stBswDcmData.diagDataLengthRx = (diagReq[0] & 0x0Fu);
        (void)memcpy(stBswDcmData.diagDataBufferRx, &diagReq[1], stBswDcmData.diagDataLengthRx);
        stBswDcmData.isServiceRequest = TRUE;
    }
    else if (frameType == FRAME_TYPE_FIRST)
    {
        stBswDcmData.firstFrameDataLength = (((uint16)diagReq[0] & 0x000Fu) << 8u) | diagReq[1];
        stBswDcmData.diagDataLengthRx = length - 2u;
        (void)memcpy(stBswDcmData.diagDataBufferRx, &diagReq[2], stBswDcmData.diagDataLengthRx);
        // Send FlowControl
        (void) memset(stBswDcmData.diagDataBufferTx, REF_VALUE_FILLER_BYTE, TX_MSG_LEN_PHY_RES);
        stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_FLOW_CONTROL << 4u) | REF_VALUE_FLOW_STATUS_CTS;
        stBswDcmData.diagDataBufferTx[1] = REF_VALUE_BLOCK_SIZE;
        stBswDcmData.diagDataBufferTx[2] = REF_VALUE_ST_MIN;
        Rte_Call_BswDcm_rEcuAbsCan_canSend(TX_MSG_IDX_PHY_RES, stBswDcmData.diagDataBufferTx);
    }
    else if (frameType == FRAME_TYPE_CONSECUTIVE)
    {
        uint16 remainDataLength = stBswDcmData.firstFrameDataLength - stBswDcmData.diagDataLengthRx;
        if(remainDataLength > (length-1u))
        {
            (void)memcpy(&stBswDcmData.diagDataBufferRx[stBswDcmData.diagDataLengthRx], &diagReq[1], length-1u);
            stBswDcmData.diagDataLengthRx += (length-1u);
        }
        else
        {
            (void)memcpy(&stBswDcmData.diagDataBufferRx[stBswDcmData.diagDataLengthRx], &diagReq[1], remainDataLength);
            stBswDcmData.diagDataLengthRx += remainDataLength;
            stBswDcmData.isServiceRequest = TRUE;
        }
    }
    else
    {
        (void) 0;
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
    uint16 seed = 0x1234;
    uint16 key;
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
            if(key == 0x5678u)
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

char key[32];
unsigned char hash[32];
unsigned char *p_signature;
static uint8 routineControl(void)
{
	uint8 errorResult = FALSE;
	uint8 subFunctionId = stBswDcmData.diagDataBufferRx[1];
	uint16_t routineIdentifier = ((uint16_t)stBswDcmData.diagDataBufferRx[2] << 8) | (uint16_t)stBswDcmData.diagDataBufferRx[3];
    uint16 routineControlOptionRecord;

	if(stBswDcmData.diagDataLengthRx < 4u)
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
	else if(routineIdentifier == RID_EraseMemory)
	{
	    routineControlOptionRecord = ((uint16_t)stBswDcmData.diagDataBufferRx[4] << 8) | (uint16_t)stBswDcmData.diagDataBufferRx[5];
	    if(routineControlOptionRecord == RCOR_EraseMemory_App)
	    {
	        Rte_Call_BswDcm_rEcuAbsFls_erasePflashBlock(PFLASH_AREA_APPICATION_SECONDARY, &errorResult);
	    }
	    else
	    {
	        errorResult = NRC_RequestOutRange;
	    }

	    // SHA256 미리하기
        (void) memset(key, 0x11, sizeof(key));
        SHA256_Init(&stBswDcmData.info);
        SHA256_Process(&stBswDcmData.info, key, 32);
	}
	else if(routineIdentifier == RID_CheckProgrammingDependencies)  //////////////////Check Dependencies////////////////////
	{
	    // SHA-256
//	    SHA256_Process(&stBswDcmData.info, (unsigned char *)MEMORY_ADDRESS_BACKUP, MEMORY_SIZE_512K - MEMORY_DATA_VALID_SIGNATURE_LENGTH - MEMORY_DATA_VALID_PATTERN_LENGTH);
	    SHA256_Close(&stBswDcmData.info, hash);

	    int cmp_flag = 0;
	    p_signature = (unsigned char*)0x801FFFC0;
	    for (int i=0; i<32; i++){
	        if(p_signature[i] != hash[i]){
	            cmp_flag = 1;
	            break;
	        }
	    }

	    if (cmp_flag == 1){    // 다르다
	        errorResult = NRC_GeneralReject;
	    }
	    else {  // 같다
            // copy : BACKUP -> APP
            Rte_Call_BswDcm_rEcuAbsFls_erasePflashBlock(PFLASH_AREA_APPICATION_PRIMARY, &errorResult);
            Rte_Call_BswDcm_rEcuAbsFls_writePflash(MEMORY_ADDRESS_APPLICATION, MEMORY_SIZE_512K, (P_uint8) MEMORY_ADDRESS_BACKUP, &errorResult);

            errorResult = FALSE;
	    }
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

static uint8 requestDownload(void)
{
    uint8 errorResult;
    uint8 dataFormatIdentifier = stBswDcmData.diagDataBufferRx[1];
    uint8 compressionMethod = (dataFormatIdentifier & 0xF0u) >> 4u;
    uint8 encryptingMethod = dataFormatIdentifier & 0x0Fu;
    uint8 addressAndLengthFormatIdentifier = stBswDcmData.diagDataBufferRx[2];
    uint32 lengthOfDataSize = (addressAndLengthFormatIdentifier & 0xF0u) >> 4u;
    uint32 lengthOfAddress = addressAndLengthFormatIdentifier & 0x0Fu;
    uint32 memoryAddress;
    uint32 memorySize;

    if(stBswDcmData.diagDataLengthRx != 11u)
    {
        errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
    }
    else if(stBswDcmData.stDiagStatus.isSecurityUnlock == FALSE)
    {
        errorResult = NRC_SecurityAccessDenied;
    }
    else if((compressionMethod != 0x0u) || (encryptingMethod != 0x0u))
    {
        errorResult = NRC_RequestOutRange;
    }
    else if((lengthOfDataSize != 0x4u) || (lengthOfAddress != 0x4u))
    {
        errorResult = NRC_RequestOutRange;
    }
    else
    {
        stBswDcmData.downloadDataLength = 0u;
        memoryAddress = ((uint32)stBswDcmData.diagDataBufferRx[3] << 24u) | ((uint32)stBswDcmData.diagDataBufferRx[4] << 16u) |
                ((uint32)stBswDcmData.diagDataBufferRx[5] << 8u) | ((uint32)stBswDcmData.diagDataBufferRx[6] << 0u);
        memorySize = ((uint32)stBswDcmData.diagDataBufferRx[7] << 24u) | ((uint32)stBswDcmData.diagDataBufferRx[8] << 16u) |
                ((uint32)stBswDcmData.diagDataBufferRx[9] << 8u) | ((uint32)stBswDcmData.diagDataBufferRx[10] << 0u);
        if((memoryAddress == MEMORY_ADDRESS_APPLICATION) && (memorySize == MEMORY_SIZE_APPLICATION))
        {
            stBswDcmData.downloadMemoryAddress = memoryAddress;
            stBswDcmData.downloadMemorySize = memorySize;
            stBswDcmData.downloadDataLength = 0u;
            stBswDcmData.downloadBlockSequence = 1u;
            errorResult = FALSE;
        }
        else
        {
            stBswDcmData.downloadMemoryAddress = 0u;
            stBswDcmData.downloadMemorySize = 0u;
            stBswDcmData.downloadDataLength = 0u;
            errorResult = NRC_RequestOutRange;
        }

        if(errorResult == FALSE)
        {
            stBswDcmData.diagDataLengthTx = 4u;
            stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
            stBswDcmData.diagDataBufferTx[1] = SID_RequestDownload + 0x40u;
            stBswDcmData.diagDataBufferTx[2] = 0x20u;   /* maxNumberofBlockLength is 2Byte */
            stBswDcmData.diagDataBufferTx[3] = (MAX_NUMBER_OF_BLOCK_LENGTH & 0xFF00) >> 8u ;
            stBswDcmData.diagDataBufferTx[4] = (MAX_NUMBER_OF_BLOCK_LENGTH & 0x00FF);
        }
    }
    return errorResult;
}

static uint8 transferData(void)
{
    uint8 errorResult;
    uint8 blockSequence = stBswDcmData.diagDataBufferRx[1];
    uint16 dataLength = stBswDcmData.diagDataLengthRx-2u;
    if(stBswDcmData.diagDataLengthRx < 3u)
    {
        errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
    }
    else if(blockSequence != stBswDcmData.downloadBlockSequence)
    {
        errorResult = NRC_WrongBlockSequeceNumber;
    }
    else if((stBswDcmData.downloadDataLength + dataLength) > stBswDcmData.downloadMemorySize)
    {
        errorResult = NRC_TransferDataSuspended;
    }
    else
    {

        if (MEMORY_ADDRESS_BACKUP + stBswDcmData.downloadDataLength < 0x801FFFC0 - dataLength){
            for (int i=2; i<dataLength+2; i++) {
                stBswDcmData.diagDataBufferRx[i] -= 0x05;
            }
        }
        else {
            for (int i=2; i<dataLength + 2 - 32; i++) {
                stBswDcmData.diagDataBufferRx[i] -= 0x05;
            }
        }

        //Rte_Call_BswDcm_rEcuAbsFls_writePflash(stBswDcmData.downloadMemoryAddress + stBswDcmData.downloadDataLength, dataLength, &stBswDcmData.diagDataBufferRx[2], &errorResult);
        Rte_Call_BswDcm_rEcuAbsFls_writePflash(MEMORY_ADDRESS_BACKUP + stBswDcmData.downloadDataLength, dataLength, &stBswDcmData.diagDataBufferRx[2], &errorResult);

        if (MEMORY_ADDRESS_BACKUP + stBswDcmData.downloadDataLength < 0x801FFFC0 - dataLength){
            SHA256_Process(&stBswDcmData.info, &stBswDcmData.diagDataBufferRx[2], dataLength);
        }
        else {
            SHA256_Process(&stBswDcmData.info, &stBswDcmData.diagDataBufferRx[2], dataLength-MEMORY_DATA_VALID_SIGNATURE_LENGTH);
        }

        if(errorResult == FALSE)
        {
            stBswDcmData.downloadDataLength += dataLength;
            stBswDcmData.downloadBlockSequence++;
            stBswDcmData.diagDataLengthTx = 2u;
            stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
            stBswDcmData.diagDataBufferTx[1] = SID_TransferData + 0x40u;
            stBswDcmData.diagDataBufferTx[2] = blockSequence;


            if(stBswDcmData.downloadDataLength != stBswDcmData.downloadMemorySize)
            {
               // SHA256_Process(&stBswDcmData.info, &stBswDcmData.diagDataBufferRx[2], dataLength);
            }
            else
            {
              //  SHA256_Process(&stBswDcmData.info, &stBswDcmData.diagDataBufferRx[2], dataLength-MEMORY_DATA_VALID_SIGNATURE_LENGTH);
            }
        }
        else
        {
            errorResult = NRC_GeneralProgrammingFailure;
        }
    }
    return errorResult;
}

static uint8 requestTransferExit(void)
{
    uint8 errorResult;
    if (stBswDcmData.diagDataLengthRx != 1u)
    {
        errorResult = NRC_IncorrectMessageLengthOrInvalidFormat;
    }
    else if(stBswDcmData.downloadDataLength != stBswDcmData.downloadMemorySize)
    {
        errorResult = NRC_GeneralProgrammingFailure;
    }
    else
    {
        //Rte_Call_BswDcm_rEcuAbsFls_writePflash(MEMORY_PATTERN_VALID_APP_ADDR, MEMORY_DATA_VALID_PATTERN_LENGTH, (P_uint8) MEMORY_DATA_VALID_PATTERN_VALUE, &errorResult);
        Rte_Call_BswDcm_rEcuAbsFls_writePflash(MEMORY_PATTERN_VALID_BACKUP_ADDR, MEMORY_DATA_VALID_PATTERN_LENGTH, (P_uint8) MEMORY_DATA_VALID_PATTERN_VALUE, &errorResult);

        if(errorResult == FALSE)
        {
            stBswDcmData.diagDataLengthTx = 1u;
            stBswDcmData.diagDataBufferTx[0] = (FRAME_TYPE_SINGLE << 4u) | stBswDcmData.diagDataLengthTx;
            stBswDcmData.diagDataBufferTx[1] = SID_RequestTransferExit + 0x40u;
        }
        else
        {
            errorResult = NRC_GeneralProgrammingFailure;
        }
    }
    return errorResult;
}
