#ifndef Rte_Type_H
#define Rte_Type_H

#include "Std_Types.h"
#include <string.h>

typedef uint8* P_uint8;
typedef uint32* P_uint32;
typedef const uint8* CP_uint8;
typedef void (*FP_void_void)(void);

typedef struct ST_AdcValue {
    uint16 adcBattery;
    uint16 adcIgnition;
    uint16 adcHwVersion;
} ST_AdcValue;
typedef ST_AdcValue* P_stAdcValue;

typedef struct {
	uint8 currentSession;
	uint8 isSecurityUnlock;
	uint8 isEcuReset;
	uint8 isCommunicationDisable;
	uint8 isAliveCountDecrease;
}ST_DiagStatus;
typedef ST_DiagStatus* P_stDiagStatus;

#endif
