#include "EcuAbsGpt.h"
#include "Gpt_Cfg.h"
#include "Gpt.h"

#define Gpt_Channel_TASK_1ms 0u

void EA_initGpt(void)
{
	Gpt_EnableNotification((Gpt_ChannelType) Gpt_Channel_TASK_1ms);
    Gpt_StartTimer(Gpt_Channel_TASK_1ms, 5000u );   /* Channel 0 : gpt is configured to 1ms, 80MHz/16/5000tick = 1000hz = 1ms */
}

void EA_mainGpt(void)
{
    (void) 0;
}
