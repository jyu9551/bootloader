#include "EcuAbsMcu.h"
#include "Rte_EcuAbs.h"
#include "IfxScu_reg.h"
#include "Mcu.h"
#include "Definition.h"

void EA_initMcu(void)
{
   (void) 0;
}

void EA_mainMcu(void)
{
    ST_DiagStatus stDiagStatus;
    Rte_Call_EcuAbs_rDcmCmd_getDiagStatus(&stDiagStatus);
    if(stDiagStatus.isEcuReset == TRUE)
    {
        Mcu_PerformReset();
    }
    else if(stDiagStatus.currentSession == DIAG_SESSION_PROGRAMMING)
    {
    	Mcal_ResetSafetyENDINIT_Timed(MCU_SAFETY_ENDINT_TIMEOUT);
    	SCU_RSTCON2.B.USRINFO = RESET_REASON_REPROGRAMMING_REQUEST;
    	Mcal_SetSafetyENDINIT_Timed();
    	Mcu_PerformReset();
    }
    else
    {
    	(void) 0;
    }
}
