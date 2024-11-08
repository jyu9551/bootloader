#ifndef EcuAbsNvm_H
#define EcuAbsNvm_H

#include "Rte_Type.h"

void EA_initNvm(void);
void EA_mainNvm(void);
void EA_readNvmData(uint16 block, P_uint8 data);
void EA_writeNvmData(uint16 block, P_uint8 data);

#endif
