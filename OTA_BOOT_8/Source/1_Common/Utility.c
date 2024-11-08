#include "Utility.h"

#define BIT_LEN_OF_1BYTE 8u

typedef struct
{
	uint8 handle;
	uint8 length;
	uint32 id;
	uint8 data[MAX_CAN_MSG_DLC];
} ST_CanRxIndecation;

extern void REcbBswCom_canRxIndication(void);

static ST_CanRxIndecation stCanRxIndecation;
static uint32_t timeEcuAlive1ms = 10u;
static uint32_t timeEcuAlive10ms = 1u;
static FP_void_void functionCallbackTxConfirmation[TOTAL_CAN_MSG_COUNT];

void UT_updateTimeEcuAlive1ms(void)
{
	timeEcuAlive1ms++;
}

uint32_t UT_getTimeEcuAlive1ms(void)
{
	return timeEcuAlive1ms;
}

void UT_updateTimeEcuAlive10ms(void)
{
	timeEcuAlive10ms++;
}

uint32_t UT_getTimeEcuAlive10ms(void)
{
	return timeEcuAlive10ms;
}

void UT_registerCanTxConfirmation(uint8 pduIndex, FP_void_void func)
{
	functionCallbackTxConfirmation[pduIndex] = func;
}

void UT_canTxConfirmation(uint8 pduIndex)
{
	if (functionCallbackTxConfirmation[pduIndex] != (void *)NULL)
	{
		functionCallbackTxConfirmation[pduIndex]();
	}
}

void UT_getCanRxIndicationData(P_uint8 handle, P_uint8 length, P_uint32 id, P_uint8 pu8Data)
{
	*handle = stCanRxIndecation.handle;
	*length = stCanRxIndecation.length;
	*id = stCanRxIndecation.id;
	(void)memcpy(pu8Data, stCanRxIndecation.data, stCanRxIndecation.length);
}

void UT_canRxIndication(uint8 handle, uint8 length, uint32 id, CP_uint8 pu8Data)
{
	if(length <= MAX_CAN_MSG_DLC)
	{
		stCanRxIndecation.handle = handle;
		stCanRxIndecation.length = length;
		stCanRxIndecation.id = id;
		(void)memcpy(stCanRxIndecation.data, pu8Data, length);
		REcbBswCom_canRxIndication();
	}
}

void UT_readSignalData(CP_uint8 buffer, uint16_t startBit, uint16_t length, uint8_t isByteOrderIntel, uint8_t dataType, void *pSignal)
{
	uint16 bytePosition = startBit / BIT_LEN_OF_1BYTE;
	uint16 startBitPositionAtByte = startBit % BIT_LEN_OF_1BYTE;
	uint16 byteExtend = ((startBitPositionAtByte + length - 1u) / BIT_LEN_OF_1BYTE) + 1u;

	uint64_t signalValue = (uint64_t)((uint64_t)buffer[bytePosition] >> startBitPositionAtByte);

	if(byteExtend > 1u)
	{
		bytePosition = (isByteOrderIntel == TRUE) ? (bytePosition + 1u) : (bytePosition - 1u);
		for (uint8 i = 1u; i < byteExtend; i++)
		{
			signalValue |= (uint64_t)(buffer[bytePosition]) << ((i * BIT_LEN_OF_1BYTE) - startBitPositionAtByte);
			bytePosition = (isByteOrderIntel == TRUE) ? (bytePosition + 1u) : (bytePosition - 1u);
		}
	}

	switch (dataType)
	{
	case DataType_uint8:
		*((uint8_t *)pSignal) = (uint8_t)(signalValue << (8u - length)) >> (8u - length);
		break;
	case DataType_uint16:
		*((uint16_t *)pSignal) = (uint16_t)(signalValue << (16u - length)) >> (16u - length);
		break;
	case DataType_uint32:
		*((uint32_t *)pSignal) = (uint32_t)(signalValue << (32u - length)) >> (32u - length);
		break;
	case DataType_sint8:
		*((int8_t*)pSignal) = (int8_t)(signalValue << (8u - length)) >> (8u - length);
		break;
	case DataType_sint16:
		*((int16_t*)pSignal) = (int16_t)(signalValue << (16u - length)) >> (16u - length);
		break;
	case DataType_sint32:
		 *((int32_t*)pSignal) = (int32_t)(signalValue << (32u - length)) >> (32u - length);
		break;
	default:
		(void)0;
		break;
	}
}

void UT_writeSignalData(uint8_t *buffer, uint16_t startBit, uint16_t length, uint8_t isByteOrderIntel, uint8_t dataType, const void *pSignal)
{
	uint8_t tempForClearBit;
	uint16_t bytePosition = startBit / BIT_LEN_OF_1BYTE;
	uint16_t startBitPositionAtByte = startBit % BIT_LEN_OF_1BYTE;
	uint8_t i;
	uint16_t byteExtend = ((startBitPositionAtByte + length - 1u) / BIT_LEN_OF_1BYTE) + 1u;
	uint8_t byteData;
	uint64_t signalValue = 0u;
	uint16_t deleteLeftBit;
	switch (dataType)
	{
	case DataType_uint8:
		signalValue = *((uint8_t*)pSignal);
		break;
	case DataType_uint16:
		signalValue = *((uint16_t*)pSignal);
		break;
	case DataType_uint32:
		signalValue = *((uint32_t*)pSignal);
		break;
	case DataType_sint8:
		signalValue = *((int8_t*)pSignal);
		break;
	case DataType_sint16:
		signalValue = *((int16_t*)pSignal);
		break;
	case DataType_sint32:
		signalValue = *((int32_t*)pSignal);
		break;
	default:
		(void) 0;
		break;
	}

	byteData = (uint8_t)(signalValue << startBitPositionAtByte);
	if ((length + startBitPositionAtByte - 1u) < BIT_LEN_OF_1BYTE)
	{
		deleteLeftBit = BIT_LEN_OF_1BYTE - (length + startBitPositionAtByte);
		tempForClearBit = (uint8_t)(0xFFu << startBitPositionAtByte);
		tempForClearBit = (uint8_t)(((tempForClearBit << deleteLeftBit) >> deleteLeftBit) & ((deleteLeftBit >= 8u) ? 0xFFu : (0xFFu >> deleteLeftBit)));
		tempForClearBit = ~tempForClearBit;
		buffer[bytePosition] = buffer[bytePosition] & tempForClearBit;
		buffer[bytePosition] |= (uint8_t)(((byteData << deleteLeftBit) >> deleteLeftBit) & ((deleteLeftBit >= 8u) ? 0xFFu : (0xFFu >> deleteLeftBit)));
	}
	else
	{
		tempForClearBit = (uint8_t)(0xFFu << startBitPositionAtByte);
		tempForClearBit = ~tempForClearBit;
		buffer[bytePosition] = buffer[bytePosition] & tempForClearBit;
		buffer[bytePosition] |= byteData;
		signalValue = signalValue >> (BIT_LEN_OF_1BYTE - startBitPositionAtByte);
		bytePosition = (isByteOrderIntel == TRUE) ? (bytePosition + 1u) : (bytePosition - 1u);
		for (i = 1u; i < (byteExtend - 1u); i++)
		{
			buffer[bytePosition] = (uint8_t)signalValue;
			signalValue = signalValue >> 8u;
			bytePosition = (isByteOrderIntel == TRUE) ? (bytePosition + 1u) : (bytePosition - 1u);
		}
		deleteLeftBit = BIT_LEN_OF_1BYTE - ((length + startBitPositionAtByte) % BIT_LEN_OF_1BYTE);
		tempForClearBit = 0xFFu;
		tempForClearBit = (uint8_t)(((tempForClearBit << deleteLeftBit) >> deleteLeftBit) & ((deleteLeftBit >= 8u) ? 0xFFu : (0xFFu >> deleteLeftBit)));
		tempForClearBit = ~tempForClearBit;
		buffer[bytePosition] = buffer[bytePosition] & tempForClearBit;
		buffer[bytePosition] |= (uint8_t)(((signalValue << deleteLeftBit) >> deleteLeftBit) & ((deleteLeftBit >= 8u) ? 0xFFu : (0xFFu >> deleteLeftBit)));
	}
}
