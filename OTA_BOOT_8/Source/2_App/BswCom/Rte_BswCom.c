#include "Rte_BswCom.h"
#include "Definition.h"
#include "Utility.h"

typedef struct {
    uint8 canMsgBuffer[TOTAL_CAN_MSG_COUNT][MAX_CAN_MSG_DLC];
} ST_BswComData;
static ST_BswComData stBswComData;

typedef struct
{
	uint8 message[RX_MSG_LEN_SAS_01];
	uint8 sas01TestValue01;			/* Sas01_TestValue01, StartBit:0, Length:8, unsigned, Intel */
	uint16 sas01TestValue02;		/* Sas01_TestValue02, StartBit:22, Length:12, unsigned, Intel */
	uint16 sas01TestValue03;		/* Sas01_TestValue03, StartBit:62, Length:12, unsigned, Motorola */
}ST_Sas01;
static ST_Sas01 stSas01;

typedef struct
{
	uint8 message[TX_MSG_LEN_SDC_01];
	uint8 aliveCount;				/* aliveCount, StartBit:0, Length:8, unsigned, Intel */
	uint16 echoSas01TestValue02;	/* Echo_Sas01_TestValue02, StartBit:22, Length:12, unsigned, Intel */
	uint16 echoSas01TestValue03;	/* Echo_Sas01_TestValue03, StartBit:62, Length:12, unsigned, Motorola */
}ST_Sdc01;
static ST_Sdc01 stSdc01;

static void processingDataRx(void);
static void processingDataTx(void);
static void sendCanTx(void);

void REcbBswCom_initialize(void)
{
	Rte_Call_BswCom_rEcuAbsCan_setPduInfo(RX_MSG_IDX_PHY_REQ, RX_MSG_LEN_PHY_REQ, RX_MSG_ID_PHY_REQ, stBswComData.canMsgBuffer[RX_MSG_IDX_PHY_REQ]);
	Rte_Call_BswCom_rEcuAbsCan_setPduInfo(RX_MSG_IDX_FUN_REQ, RX_MSG_LEN_FUN_REQ, RX_MSG_ID_FUN_REQ, stBswComData.canMsgBuffer[RX_MSG_IDX_FUN_REQ]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(RX_MSG_IDX_CCP_REQ, RX_MSG_LEN_CCP_REQ, RX_MSG_ID_CCP_REQ, stBswComData.canMsgBuffer[RX_MSG_IDX_CCP_REQ]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(RX_MSG_IDX_SAS_01, RX_MSG_LEN_SAS_01, RX_MSG_ID_SAS_01, stBswComData.canMsgBuffer[RX_MSG_IDX_SAS_01]);

    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_SDC_01, TX_MSG_LEN_SDC_01, TX_MSG_ID_SDC_01, stBswComData.canMsgBuffer[TX_MSG_IDX_SDC_01]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_PHY_RES, TX_MSG_LEN_PHY_RES, TX_MSG_ID_PHY_RES, stBswComData.canMsgBuffer[TX_MSG_IDX_PHY_RES]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_RES, TX_MSG_LEN_CCP_RES, TX_MSG_ID_CCP_RES, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_RES]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON0, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON0, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON0]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON1, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON1, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON1]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON2, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON2, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON2]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON3, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON3, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON3]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON4, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON4, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON4]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON5, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON5, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON5]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON6, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON6, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON6]);
    Rte_Call_BswCom_rEcuAbsCan_setPduInfo(TX_MSG_IDX_CCP_MON7, TX_MSG_LEN_CCP_MON_DEFAULT, TX_MSG_ID_CCP_MON7, stBswComData.canMsgBuffer[TX_MSG_IDX_CCP_MON7]);
}

void REtBswCom_bswComMain(void)
{
	processingDataRx();
	processingDataTx();
	sendCanTx();
	if(stSas01.sas01TestValue01 == 0xFF)
	{
	    while(1);
	}
}

void REcbBswCom_canRxIndication(void)
{
	uint8 handle;
	uint8 length;
	uint32 id;
	uint8 data[MAX_CAN_MSG_DLC];

	UT_getCanRxIndicationData(&handle, &length, &id, data);

	if(handle == RX_MSG_IDX_SAS_01)
	{
		(void)memcpy(stSas01.message, data, length);
	}
    else if(handle == RX_MSG_IDX_FUN_REQ)
    {
        Rte_Call_BswCom_rDcmCmd_processDiagRequest(FALSE, data, length);
    }
    else if(handle == RX_MSG_IDX_PHY_REQ)
    {
        Rte_Call_BswCom_rDcmCmd_processDiagRequest(TRUE, data, length);
    }
    else if(handle == RX_MSG_IDX_CCP_REQ)
    {
        Rte_Call_BswCom_rCcpCustomCmd_processCcpRequest(data);
    }
    else
    {
        (void) 0;
    }
}

static void processingDataRx(void)
{
	UT_readSignalData(stSas01.message, 0u, 8u, TRUE, DataType_uint8, &stSas01.sas01TestValue01);
	UT_readSignalData(stSas01.message, 22u, 12u, TRUE, DataType_uint16, &stSas01.sas01TestValue02);
	UT_readSignalData(stSas01.message, 62u, 12u, FALSE, DataType_uint16, &stSas01.sas01TestValue03);
}

static void processingDataTx(void)
{
	stSdc01.echoSas01TestValue02 = stSas01.sas01TestValue02;
	stSdc01.echoSas01TestValue03 = stSas01.sas01TestValue03;
	(void)memset(stSdc01.message, 0, TX_MSG_LEN_SDC_01);
	UT_writeSignalData(stSdc01.message, 0u, 8u, TRUE, DataType_uint8, &stSdc01.aliveCount);
	UT_writeSignalData(stSdc01.message, 22u, 12u, TRUE, DataType_uint16, &stSdc01.echoSas01TestValue02);
	UT_writeSignalData(stSdc01.message, 62u, 12u, FALSE, DataType_uint16, &stSdc01.echoSas01TestValue03);
}

static void sendCanTx(void)
{
    static uint32 countCanTx = 0u;
    ST_DiagStatus stDiagStatus;
    Rte_Call_BswCom_rDcmCmd_getDiagStatus(&stDiagStatus);
    if(stDiagStatus.isCommunicationDisable == FALSE)
    {
        countCanTx++;
        if((countCanTx % TIME_DECI_100MS) == 0)
        {
            stSdc01.message[1] = 0x0B;
            Rte_Call_BswCom_rEcuAbsCan_canSend(TX_MSG_IDX_SDC_01, stSdc01.message);
            if(stDiagStatus.isAliveCountDecrease == FALSE)
            {
                stSdc01.aliveCount++;
            }
            else
            {
                stSdc01.aliveCount--;
            }
        }
    }
}
