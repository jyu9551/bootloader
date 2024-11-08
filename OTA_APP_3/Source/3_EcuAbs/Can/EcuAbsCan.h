#ifndef EcuAbsCan_H
#define EcuAbsCan_H

#include "Rte_Type.h"

void EA_initCan(void);
void EA_mainCan(void);
void EA_setPduInfo(uint8 handle, uint8 length, uint32 id, P_uint8 buffer);
void EA_canSend(uint8 handle, CP_uint8 buffer);

#endif
