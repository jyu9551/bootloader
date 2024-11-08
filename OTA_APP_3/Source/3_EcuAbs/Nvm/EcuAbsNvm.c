#include "EcuAbsNvm.h"
#include "Fee.h"
#include "Fls_17_Pmu.h"
#include "Utility.h"

extern void NvM_JobEndNotification(void);
extern void NvM_JobErrorNotification(void);

static uint16 ignCycle = 0;
void EA_initNvm(void)
{
    Fee_Init();
    EA_readNvmData(DATA_ID_IgnCycle, (P_uint8)&ignCycle);
    ignCycle++;
    EA_writeNvmData(DATA_ID_IgnCycle, (P_uint8)&ignCycle);
}

void EA_mainNvm(void)
{
	(void) 0;
}

void NvM_JobEndNotification(void)
{
	(void) 0;
}

void NvM_JobErrorNotification(void)
{
	(void) 0;
}

void EA_readNvmData(uint16 block, P_uint8 data)
{
	uint16 length;
	if(block == DATA_ID_IgnCycle)
	{
		length = DATA_LEN_IgnCycle;
	}
	else if(block == DATA_ID_VariantCode)
	{
		length = DATA_LEN_VariantCode;
	}
	else
	{
		length = 0u;
	}

	if(length != 0u)
	{
		(void) Fee_Read(block, 0u, data, length);
		while(Fee_GetStatus() != MEMIF_IDLE)
		{
			Fee_MainFunction();
			Fls_17_Pmu_MainFunction();
		}
	}
}

void EA_writeNvmData(uint16 block, P_uint8 data)
{
	(void) Fee_Write(block, data);
	while(Fee_GetStatus() != MEMIF_IDLE)
	{
		Fee_MainFunction();
		Fls_17_Pmu_MainFunction();
	}
}
