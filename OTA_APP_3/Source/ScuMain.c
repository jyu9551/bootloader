#include "Rte_Type.h"
#include "Rte_EcuAbs.h"
#include "Rte_BswCom.h"
#include "Rte_BswDcm.h"
#include "Rte_CcpCustom.h"
#include "Utility.h"

static void Task_10ms(void);

void GptNotification_1ms(void)
{
    UT_updateTimeEcuAlive1ms();

    REtCcpCustom_ccpCustomTask1ms();
    if((UT_getTimeEcuAlive1ms() % 10u) == 0u)
    {
        UT_updateTimeEcuAlive10ms();
    }
}

static void Task_10ms(void)
{
    REtEcuAbs_ecuAbsMain();
    REtBswCom_bswComMain();
    REtBswDcm_bswDcmMain();
}

void ScuMain(void)
{
    static uint32_t previousTimeEcuAlive10ms = 0u;
    REcbEcuAbs_initialize();
    REcbBswCom_initialize();
    REcbBswDcm_initialize();
    REcbCcpCustom_initialize();
    while(TRUE)
    {
        if(previousTimeEcuAlive10ms != UT_getTimeEcuAlive10ms())
        {
            previousTimeEcuAlive10ms = UT_getTimeEcuAlive10ms();
            Task_10ms();
        }
    }
}
