#include "Rte_EcuAbs.h"
#include "EcuAbsGpt.h"
#include "EcuAbsAdc.h"
#include "EcuAbsCan.h"
#include "EcuAbsDio.h"
#include "EcuAbsSpi.h"
#include "EcuAbsWdg.h"
#include "EcuAbsNvm.h"
#include "EcuAbsMcu.h"
//#include "Mcu.h"

void REcbEcuAbs_initialize(void)
{
	EA_initMcu();
	EA_initWdg();
	EA_initDio();
	EA_initAdc();
	EA_initGpt();
	EA_initCan();
	EA_initSpi();
	EA_initNvm();
}

void REtEcuAbs_ecuAbsMain(void)
{
//    ST_DiagStatus stDiagStatus;
    EA_mainMcu();
    EA_mainAdc();
    EA_mainDio();
    EA_mainSpi();
    EA_mainWdg();
    EA_mainNvm();
    EA_mainGpt();
    EA_mainCan();
//    Rte_Call_EcuAbs_rDcmCmd_getDiagStatus(&stDiagStatus);
//    if(stDiagStatus.isEcuReset == TRUE)
//    {
//        Mcu_PerformReset();
//    }
}

void REoiEcuAbs_pEcuAbsCan_setPduInfo(uint8 handle, uint8 length, uint32 id, P_uint8 buffer)
{
    EA_setPduInfo(handle, length, id, buffer);
}

void REoiEcuAbs_pEcuAbsCan_canSend(uint8 handle, P_uint8 buffer)
{
    EA_canSend(handle, buffer);
}

void REoiEcuAbs_pEcuAbsNvm_readNvmData(uint16 block, P_uint8 data)
{
	EA_readNvmData(block, data);
}

void REoiEcuAbs_pEcuAbsNvm_writeNvmData(uint16 block, P_uint8 data)
{
	EA_writeNvmData(block, data);
}
