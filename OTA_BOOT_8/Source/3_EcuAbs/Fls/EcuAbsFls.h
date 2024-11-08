#ifndef EcuAbsFls_H
#define EcuAbsFls_H

extern void EA_initFls(void);
extern void EA_erasePflashBlock(uint8 pflashArea, P_uint8 errorResult);
extern void EA_writePflash(uint32 flashAddress, uint32 dataSize, P_uint8 data, P_uint8 errorResult);

#endif
