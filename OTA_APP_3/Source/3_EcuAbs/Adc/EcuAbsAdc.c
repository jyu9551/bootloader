#include "EcuAbsAdc.h"
#include "Adc.h"
#include "Rte_EcuAbs.h"
#include "Dio_Cfg.h"
#include "Dio.h"
#include "Adc_Cfg.h"

#define NUM_OF_AdcHwUnit0_AdcGroup0 1u
#define NUM_OF_AdcHwUnit1_AdcGroup0 2u

static uint16 adcValueHwVersionResistance = 0u;
static uint16 adcValueIgnitionVoltage = 0u;
static uint16 adcValueBatteryVoltage = 0u;

static Adc_ValueGroupType adcBuffeAdcHwUnit0AdcGroup0[NUM_OF_AdcHwUnit0_AdcGroup0];
static Adc_ValueGroupType adcBuffeAdcHwUnit1AdcGroup0[NUM_OF_AdcHwUnit1_AdcGroup0];

void EA_initAdc(void)
{
    Adc_SetupResultBuffer(AdcConf_AdcGroup_AdcHwUnit0_AdcGroup0, adcBuffeAdcHwUnit0AdcGroup0);
    Adc_EnableGroupNotification(AdcConf_AdcGroup_AdcHwUnit0_AdcGroup0);
    Adc_SetupResultBuffer(AdcConf_AdcGroup_AdcHwUnit1_AdcGroup0, adcBuffeAdcHwUnit1AdcGroup0);
    Adc_EnableGroupNotification(AdcConf_AdcGroup_AdcHwUnit1_AdcGroup0);
}

void EA_mainAdc(void)
{
    Adc_StartGroupConversion(AdcConf_AdcGroup_AdcHwUnit0_AdcGroup0);
    Adc_StartGroupConversion(AdcConf_AdcGroup_AdcHwUnit1_AdcGroup0);
}

void AdcNotification_AdcHwUnit0_AdcGroup0(void)
{
    adcValueHwVersionResistance = adcBuffeAdcHwUnit0AdcGroup0[AdcConf_AdcChannel_HwVersionResistance];
}

void AdcNotification_AdcHwUnit1_AdcGroup0(void)
{
    adcValueIgnitionVoltage = adcBuffeAdcHwUnit1AdcGroup0[AdcConf_AdcChannel_IgnitionVoltage];
    adcValueBatteryVoltage = adcBuffeAdcHwUnit1AdcGroup0[AdcConf_AdcChannel_BatteryVoltage];
}

void EA_getAdcValue(P_stAdcValue pstAdcValue)
{
    pstAdcValue->adcBattery = adcValueBatteryVoltage;
    pstAdcValue->adcIgnition = adcValueIgnitionVoltage;
    pstAdcValue->adcHwVersion = adcValueHwVersionResistance;
}
