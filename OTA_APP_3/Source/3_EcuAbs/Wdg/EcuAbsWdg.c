#include "EcuAbsWdg.h"
#include "WdgIf_Types.h"
#include "Wdg_17_Scu.h"

#define WDIF_FAST_MODE200MS		200
#define WDIF_SLOW_MODE1500MS	1500

static void setWdgTrigger(WdgIf_ModeType mode, uint16 timeout);

void EA_initWdg(void)
{
	setWdgTrigger(WDGIF_SLOW_MODE, WDIF_SLOW_MODE1500MS);
}

void EA_mainWdg(void)
{
	setWdgTrigger(WDGIF_FAST_MODE, WDIF_FAST_MODE200MS);
}

static void setWdgTrigger(WdgIf_ModeType mode, uint16 timeout)
{
	(void)Wdg_17_Scu_SetMode(WDGIF_OFF_MODE);
	(void)Wdg_17_Scu_SetMode(mode);
	Wdg_17_Scu_SetTriggerCondition(timeout);
}
