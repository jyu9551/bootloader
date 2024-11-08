#include "EcuAbsDio.h"
#include "Definition.h"
#include "Dio.h"
#include "EcuAbsAdc.h"

static void setMcuInhibit(uint8 level);

void EA_initDio(void)
{
	setMcuInhibit(STD_HIGH);
}

void EA_mainDio(void)
{
	static uint16 countOfIgnOff;
	ST_AdcValue stAdcValue;
	EA_getAdcValue(&stAdcValue);
	if(stAdcValue.adcIgnition < REF_VALUE_BAT_4V)
	{
		if(countOfIgnOff < TIME_DECI_100MS)
		{
			countOfIgnOff++;
		}
		else
		{
			setMcuInhibit(STD_LOW);
		}
	}
	else
	{
		countOfIgnOff = 0u;
	}
}

static void setMcuInhibit(uint8 level)
{
	Dio_WriteChannel(DioConf_DioChannel_Inhibit, level);
}
