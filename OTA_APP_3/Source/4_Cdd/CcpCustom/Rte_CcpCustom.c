#include "Rte_CcpCustom.h"
#include "Definition.h"
#include "Utility.h"

typedef struct {
	uint8_t address0;
	uint8_t address1;
	uint8_t address2;
	uint8_t address3;
	uint8_t data3;
	uint8_t data2;
	uint8_t data1;
	uint8_t data0;
}ST_CppData;

#define MEMORY_ADDRESS_START 0x70000000u
#define MEMORY_ADDRESS_END 0x7002DFFFu
#define CCP_CMD_READ 0x1u
#define CCP_CMD_WRITE 0x2u
#define CCP_CMD_MONITORING 0x3u
#define MAX_CCP_MONITORING_CNT 64u
#define CCP_MON_MSG_COUNT 8u

static ST_CppData stCppMonitoring[MAX_CCP_MONITORING_CNT];
static uint8_t ccpRes[TX_MSG_LEN_CCP_RES];
static uint8_t ccpMon[CCP_MON_MSG_COUNT][TX_MSG_LEN_CCP_MON_MAX];
static uint8_t ccpMonitoringRequestCount = 0u;
static uint8_t ccpMonitoringTxIndex = 0u;
static uint8_t isBusyCcpResMsgBuf;
static uint8 isNeedSendCcpRes;
static uint8_t isBusyCcpMonMsgBuf[CCP_MON_MSG_COUNT];
static uint8_t dlcOfCcpMonTx = 8u;
static uint16 cycleTimeOfCcpMon = 10u;
static void processCcpMonitoring(void);
static void setCppMonitoringData(void);
static void sendCcpRes(void);
static void sendCcpMon(uint8_t i);
static void responseCcpReadWrite(uint32_t address);
static void writeCCPData(uint32_t address, uint8_t size, const uint8_t * data);
static void cbCanTxConfirmationRes(void);
static void cbCanTxConfirmationMon0(void);
static void cbCanTxConfirmationMon1(void);
static void cbCanTxConfirmationMon2(void);
static void cbCanTxConfirmationMon3(void);
static void cbCanTxConfirmationMon4(void);
static void cbCanTxConfirmationMon5(void);
static void cbCanTxConfirmationMon6(void);
static void cbCanTxConfirmationMon7(void);

void REcbCcpCustom_initialize(void)
{
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_RES, cbCanTxConfirmationRes);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON0, cbCanTxConfirmationMon0);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON1, cbCanTxConfirmationMon1);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON2, cbCanTxConfirmationMon2);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON3, cbCanTxConfirmationMon3);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON4, cbCanTxConfirmationMon4);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON5, cbCanTxConfirmationMon5);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON6, cbCanTxConfirmationMon6);
	UT_registerCanTxConfirmation(TX_MSG_IDX_CCP_MON7, cbCanTxConfirmationMon7);
}

void REtCcpCustom_ccpCustomTask1ms(void)
{
	if(isNeedSendCcpRes == TRUE)
	{
		sendCcpRes();
	}

	if((UT_getTimeEcuAlive1ms() % cycleTimeOfCcpMon) == 0u)
	{
		setCppMonitoringData();
	}
	processCcpMonitoring();
}

void REoiCcpCustom_pCcpCustomCmd_processCcpRequest(P_uint8 ccpReq)
{
	static uint8_t  ccu8CntLoginCCP = 0u;
	uint32_t addressReq;
	uint8_t commend = (ccpReq[0] >> 6u);
	uint8_t size;
	uint8_t sequence;

	if(ccu8CntLoginCCP == 0u)
	{
		if((0x11u == ccpReq[0]) && (0x11u == ccpReq[1]) && (0x11u == ccpReq[2]) && (0x11u == ccpReq[3]) && (0x11u == ccpReq[4]) && (0x11u == ccpReq[5]) && (0x11u == ccpReq[6]) && (0x11u == ccpReq[7]))
		{
			ccu8CntLoginCCP = 1u;
		}
		else
		{
			ccu8CntLoginCCP = 0u;
		}
	}
	else if(ccu8CntLoginCCP == 1u)
	{
		if((0xFFu == ccpReq[0]) && (0xFFu == ccpReq[1]) && (0xFFu == ccpReq[2]) && (0xFFu == ccpReq[3]) && (0xFFu == ccpReq[4]) && (0xFFu == ccpReq[5]) && (0xFFu == ccpReq[6]) && (0xFFu == ccpReq[7]))
		{
			ccu8CntLoginCCP = 2u;
		}
		else
		{
			ccu8CntLoginCCP = 0u;
		}
	}
	else if(ccu8CntLoginCCP == 2u)
	{
		if((0xEEu == ccpReq[0]) && (0xEEu == ccpReq[1]) && (0xEEu == ccpReq[2]) && (0xEEu == ccpReq[3]) && (0xEEu == ccpReq[4]) && (0xEEu == ccpReq[5]) && (0xEEu == ccpReq[6]) && (0xEEu == ccpReq[7]))
		{
			ccu8CntLoginCCP = 3u;
		}
		else
		{
			ccu8CntLoginCCP = 0u;
		}
	}
	else	/* ccu8CntLoginCCP == 3u */
	{
		if(commend == CCP_CMD_READ)
		{
			addressReq = (0x70000000u) | ((uint32_t)ccpReq[1]<<16u) | ((uint32_t)ccpReq[2]<<8u) | ccpReq[3];
			responseCcpReadWrite(addressReq);
		}
		if(commend == CCP_CMD_WRITE)
		{
			size = ccpReq[0] & 0x3Fu;
			addressReq = (0x70000000u) | ((uint32_t)ccpReq[1]<<16u) | ((uint32_t)ccpReq[2]<<8u) | ccpReq[3];
			writeCCPData(addressReq, size, (const uint8_t*)&(ccpReq[4]));
			responseCcpReadWrite(addressReq);
		}
		if(commend == CCP_CMD_MONITORING)
		{
			if( (ccpReq[0] == 0xFFu) && (ccpReq[1] == 0xFEu) && (ccpReq[2] == 0xFDu) && (ccpReq[3] == 0xFCu) && (ccpReq[4] == 0xFBu) && (ccpReq[5] == 0xFAu) && (ccpReq[6] == 0x00u))
			{
				ccpMonitoringRequestCount = ccpReq[7];
				if(ccpMonitoringRequestCount == 0u) /* when SPM stop, go to default */
				{
					dlcOfCcpMonTx = 8u;
				}
			}
			else
			{
				sequence = ccpReq[0] & 0x3Fu;
				stCppMonitoring[sequence].address0 = 0x70u;
				stCppMonitoring[sequence].address1 = ccpReq[1];
				stCppMonitoring[sequence].address2 = ccpReq[2];
				stCppMonitoring[sequence].address3 = ccpReq[3];

				commend = (ccpReq[4] >> 6u);
				if(commend == CCP_CMD_MONITORING)
				{
					sequence = ccpReq[4] & 0x3Fu;
					stCppMonitoring[sequence].address0 = 0x70u;
					stCppMonitoring[sequence].address1 = ccpReq[5];
					stCppMonitoring[sequence].address2 = ccpReq[6];
					stCppMonitoring[sequence].address3 = ccpReq[7];
				}
				(void) memcpy(ccpRes, ccpReq, TX_MSG_LEN_CCP_RES);
				isNeedSendCcpRes = TRUE;
			}
		}
		else
		{
			(void) 0;
		}
	}
}



static void processCcpMonitoring(void)
{
	uint8_t txMsgCount;
	uint8_t loopCount;
	uint8_t i;
	uint8 ccpMonIndexId;
	uint8 ccpMonIndexData;
	uint8 ccpMonCountTx;

	if( (ccpMonitoringTxIndex < ccpMonitoringRequestCount) &&\
			(isBusyCcpMonMsgBuf[0] == FALSE) &&\
			(isBusyCcpMonMsgBuf[1] == FALSE) &&\
			(isBusyCcpMonMsgBuf[2] == FALSE) &&\
			(isBusyCcpMonMsgBuf[3] == FALSE) &&\
			(isBusyCcpMonMsgBuf[4] == FALSE) &&\
			(isBusyCcpMonMsgBuf[5] == FALSE) &&\
			(isBusyCcpMonMsgBuf[6] == FALSE) &&\
			(isBusyCcpMonMsgBuf[7] == FALSE) )
	{
		if(dlcOfCcpMonTx == 8u)
		{
			txMsgCount = ((ccpMonitoringRequestCount - ccpMonitoringTxIndex) > 14u) ? 14u : (ccpMonitoringRequestCount - ccpMonitoringTxIndex);
			ccpMon[0][0] = stCppMonitoring[ccpMonitoringTxIndex].address0;
			ccpMon[0][1] = stCppMonitoring[ccpMonitoringTxIndex].address1;
			ccpMon[0][2] = stCppMonitoring[ccpMonitoringTxIndex].address2;
			ccpMon[0][3] = stCppMonitoring[ccpMonitoringTxIndex].address3;
			ccpMon[0][4] = txMsgCount;
			ccpMon[0][5] = 0xFFu;
			ccpMon[0][6] = 0xFFu;
			ccpMon[0][7] = 0xFFu;
			isBusyCcpMonMsgBuf[0] = TRUE;
			sendCcpMon(0u);

			loopCount = txMsgCount/2u;
			for(i=0u; i<loopCount; i++)
			{
				ccpMon[i+1u][0] = stCppMonitoring[ccpMonitoringTxIndex].data0;
				ccpMon[i+1u][1] = stCppMonitoring[ccpMonitoringTxIndex].data1;
				ccpMon[i+1u][2] = stCppMonitoring[ccpMonitoringTxIndex].data2;
				ccpMon[i+1u][3] = stCppMonitoring[ccpMonitoringTxIndex].data3;
				ccpMonitoringTxIndex++;
				ccpMon[i+1u][4] = stCppMonitoring[ccpMonitoringTxIndex].data0;
				ccpMon[i+1u][5] = stCppMonitoring[ccpMonitoringTxIndex].data1;
				ccpMon[i+1u][6] = stCppMonitoring[ccpMonitoringTxIndex].data2;
				ccpMon[i+1u][7] = stCppMonitoring[ccpMonitoringTxIndex].data3;
				ccpMonitoringTxIndex++;
				isBusyCcpMonMsgBuf[i+1u] = TRUE;
				sendCcpMon(i+1u);
			}

			if((txMsgCount % 2u) != 0u)
			{
				ccpMon[i+1u][0] = stCppMonitoring[ccpMonitoringTxIndex].data0;
				ccpMon[i+1u][1] = stCppMonitoring[ccpMonitoringTxIndex].data1;
				ccpMon[i+1u][2] = stCppMonitoring[ccpMonitoringTxIndex].data2;
				ccpMon[i+1u][3] = stCppMonitoring[ccpMonitoringTxIndex].data3;
				ccpMonitoringTxIndex++;
				ccpMon[i+1u][4] = 0xFFu;
				ccpMon[i+1u][5] = 0xFFu;
				ccpMon[i+1u][6] = 0xFFu;
				ccpMon[i+1u][7] = 0xFFu;
				isBusyCcpMonMsgBuf[i+1u] = TRUE;
				sendCcpMon(i+1u);
			}
		}
		else if((dlcOfCcpMonTx == 32u) || (dlcOfCcpMonTx == 64u))
		{
			ccpMonIndexId = 0u;
			ccpMonIndexData = 0u;
			(void)memset(ccpMon[ccpMonIndexId], 0xFF, dlcOfCcpMonTx);
			for(ccpMonitoringTxIndex=0u; ccpMonitoringTxIndex<ccpMonitoringRequestCount; ccpMonitoringTxIndex++)
			{
				ccpMon[ccpMonIndexId][ccpMonIndexData + 0u] = stCppMonitoring[ccpMonitoringTxIndex].data0;
				ccpMon[ccpMonIndexId][ccpMonIndexData + 1u] = stCppMonitoring[ccpMonitoringTxIndex].data1;
				ccpMon[ccpMonIndexId][ccpMonIndexData + 2u] = stCppMonitoring[ccpMonitoringTxIndex].data2;
				ccpMon[ccpMonIndexId][ccpMonIndexData + 3u] = stCppMonitoring[ccpMonitoringTxIndex].data3;
				ccpMonIndexData = ccpMonIndexData + 4u;
				if((ccpMonIndexData % dlcOfCcpMonTx) == 0u)
				{
					ccpMonIndexData = 0u;
					ccpMonIndexId++;
					(void)memset(ccpMon[ccpMonIndexId], 0xFF, dlcOfCcpMonTx);
				}
			}
			ccpMonCountTx = (ccpMonitoringRequestCount*4u) / dlcOfCcpMonTx;
			if(((ccpMonitoringRequestCount*4u) % dlcOfCcpMonTx) != 0u)
			{
				ccpMonCountTx++;
			}
			for(i=0u; i<ccpMonCountTx; i++)
			{
				isBusyCcpMonMsgBuf[i] = TRUE;
				sendCcpMon(i);
			}
		}
		else
		{
			(void) 0;
		}

	}
}

static void setCppMonitoringData(void)
{
	uint8_t i;
	uint32_t address;
	for(i=0u; i<ccpMonitoringRequestCount; i++)
	{
		address = (((uint32_t)stCppMonitoring[i].address0 << 24u) | ((uint32_t)stCppMonitoring[i].address1 << 16u)) | (((uint32_t)stCppMonitoring[i].address2 << 8u) | ((uint32_t)stCppMonitoring[i].address3));

		if( (MEMORY_ADDRESS_START <= address) && ( (address+4u) <= MEMORY_ADDRESS_END) )
		{
			stCppMonitoring[i].data0 = *((uint8_t *)address+3u);
			stCppMonitoring[i].data1 = *((uint8_t *)address+2u);
			stCppMonitoring[i].data2 = *((uint8_t *)address+1u);
			stCppMonitoring[i].data3 = *((uint8_t *)address);
		}
	}
	ccpMonitoringTxIndex = 0u;

	(void)memset(isBusyCcpMonMsgBuf, (int8_t)FALSE, (size_t)CCP_MON_MSG_COUNT);
}

static void sendCcpRes(void)
{
	if(isBusyCcpResMsgBuf == FALSE)
	{
		isNeedSendCcpRes = FALSE;
		isBusyCcpResMsgBuf = TRUE;
		Rte_Call_CcpCustom_rEcuAbsCan_canSend(TX_MSG_IDX_CCP_RES, ccpRes);
	}
}

static void sendCcpMon(uint8_t i)
{
	Rte_Call_CcpCustom_rEcuAbsCan_canSend(TX_MSG_IDX_CCP_MON0 + i, ccpMon[i]);
}

static void responseCcpReadWrite(uint32_t address)
{
	ST_CppData ccpResponse;
	ccpResponse.data0 = *((uint8_t *)(address));			/* High byte for Infineon MCU */
	ccpResponse.data1 = *((uint8_t *)(address) + 1u);		/* High byte for Infineon MCU */
	ccpResponse.data2 = *((uint8_t *)(address) + 2u);		/* High byte for Infineon MCU */
	ccpResponse.data3 = *((uint8_t *)(address) + 3u);		/* High byte for Infineon MCU */
	ccpResponse.address0 = 0x70u;
	ccpResponse.address1 = (uint8_t)( (address>>16u) & 0xFFu);
	ccpResponse.address2 = (uint8_t)( (address>>8u) & 0xFFu);
	ccpResponse.address3 = (uint8_t)( (address) & 0xFFu);
	(void) memcpy(ccpRes, &ccpResponse, TX_MSG_LEN_CCP_RES);
	isNeedSendCcpRes = TRUE;
}

static void writeCCPData(uint32_t address, uint8_t size, const uint8_t * data)
{
	if( (MEMORY_ADDRESS_START <= address) && ( (address+size) <= MEMORY_ADDRESS_END) )
	{
		switch(size)
		{
		case 1u:
			*((uint8_t *)(address)) = data[3];		/* High byte for Infineon MCU */
			break;
		case 2u:
			*((uint8_t *)(address)) = data[3];		/* High byte for Infineon MCU */
			*((uint8_t *)(address) + 1) = data[2];	/* High byte for Infineon MCU */
			break;
		case 3u:
			*((uint8_t *)(address)) = data[3];		/* High byte for Infineon MCU */
			*((uint8_t *)(address) + 1) = data[2];	/* High byte for Infineon MCU */
			*((uint8_t *)(address) + 2) = data[1];	/* High byte for Infineon MCU */
			break;
		case 4u:
			*((uint8_t *)(address)) = data[3];		/* High byte for Infineon MCU */
			*((uint8_t *)(address) + 1) = data[2];	/* High byte for Infineon MCU */
			*((uint8_t *)(address) + 2) = data[1];	/* High byte for Infineon MCU */
			*((uint8_t *)(address) + 3) = data[0];	/* High byte for Infineon MCU */
			break;
		default:
			(void) 0;
			break;
		}
	}
}

static void cbCanTxConfirmationRes(void)
{
	isBusyCcpResMsgBuf = FALSE;
}

static void cbCanTxConfirmationMon0(void)
{
	isBusyCcpMonMsgBuf[0] = FALSE;
}

static void cbCanTxConfirmationMon1(void)
{
	isBusyCcpMonMsgBuf[1] = FALSE;
}

static void cbCanTxConfirmationMon2(void)
{
	isBusyCcpMonMsgBuf[2] = FALSE;
}

static void cbCanTxConfirmationMon3(void)
{
	isBusyCcpMonMsgBuf[3] = FALSE;
}

static void cbCanTxConfirmationMon4(void)
{
	isBusyCcpMonMsgBuf[4] = FALSE;
}

static void cbCanTxConfirmationMon5(void)
{
	isBusyCcpMonMsgBuf[5] = FALSE;
}

static void cbCanTxConfirmationMon6(void)
{
	isBusyCcpMonMsgBuf[6] = FALSE;
}

static void cbCanTxConfirmationMon7(void)
{
	isBusyCcpMonMsgBuf[7] = FALSE;
}
