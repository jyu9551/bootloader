#ifndef UTILITY_H
#define UTILITY_H

#include "Platform_Types.h"
#include "Definition.h"



extern void UT_updateTimeEcuAlive1ms(void);
extern uint32_t UT_getTimeEcuAlive1ms(void);
extern void UT_updateTimeEcuAlive10ms(void);
extern uint32_t UT_getTimeEcuAlive10ms(void);
extern void UT_registerCanTxConfirmation(uint8 pduIndex, FP_void_void func);
extern void UT_canTxConfirmation(uint8 pduIndex);
extern void UT_canRxIndication(uint8 handle, uint8 length, uint32 id, CP_uint8 pu8Data);
extern void UT_getCanRxIndicationData(P_uint8 handle, P_uint8 length, P_uint32 id, P_uint8 pu8Data);

#define DataType_uint8	0u
#define DataType_uint16	1u
#define DataType_uint32	2u
#define DataType_sint8	3u
#define DataType_sint16	4u
#define DataType_sint32	5u

extern void UT_readSignalData(CP_uint8 buffer, uint16_t startBit, uint16_t length, uint8_t isByteOrderIntel, uint8_t dataType, void* pSignal);
extern void UT_writeSignalData(uint8_t* buffer, uint16_t startBit, uint16_t length, uint8_t isByteOrderIntel, uint8_t dataType, const void* pSignal);

#endif
