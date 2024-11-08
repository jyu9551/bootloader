#ifndef Definition_H
#define Definition_H

#include "Rte_Type.h"

#define DATA_ID_IgnCycle						1u		/* FeeConf_FeeBlockConfiguration_IgnCycle */
#define DATA_ID_VariantCode						2u		/* FeeConf_FeeBlockConfiguration_VariantCode */

#define DATA_LEN_IgnCycle						2u
#define DATA_LEN_VariantCode					4u

/*CanHardwareObject ID (0 ~ 127)*/
#define RX_MSG_IDX_PHY_REQ					0u
#define RX_MSG_IDX_FUN_REQ					1u
#define RX_MSG_IDX_CCP_REQ					2u
#define RX_MSG_IDX_SAS_01					3u
#define RX_MSG_IDX_COUNT					(RX_MSG_IDX_SAS_01 + 1u)

#define TX_MSG_IDX_SDC_01					4u
#define TX_MSG_IDX_PHY_RES					5u
#define TX_MSG_IDX_CCP_RES					6u
#define TX_MSG_IDX_CCP_MON0					7u
#define TX_MSG_IDX_CCP_MON1					8u
#define TX_MSG_IDX_CCP_MON2					9u
#define TX_MSG_IDX_CCP_MON3					10u
#define TX_MSG_IDX_CCP_MON4					11u
#define TX_MSG_IDX_CCP_MON5					12u
#define TX_MSG_IDX_CCP_MON6					13u
#define TX_MSG_IDX_CCP_MON7					14u
#define TX_MSG_IDX_COUNT					((TX_MSG_IDX_CCP_MON7 + 1u) - RX_MSG_IDX_COUNT)
#define TOTAL_CAN_MSG_COUNT					(RX_MSG_IDX_COUNT + TX_MSG_IDX_COUNT)

#define MAX_CAN_MSG_DLC						8u

#define RX_MSG_LEN_PHY_REQ					8u
#define RX_MSG_LEN_FUN_REQ					8u
#define RX_MSG_LEN_CCP_REQ					8u
#define RX_MSG_LEN_SAS_01					8u
#define TX_MSG_LEN_SDC_01	 		  		8u
#define TX_MSG_LEN_PHY_RES	 		  		8u
#define TX_MSG_LEN_CCP_RES					8u
#define TX_MSG_LEN_CCP_MON_DEFAULT			8u
#define TX_MSG_LEN_CCP_MON_MAX				8u

/*************** TX CAN MSG ID ****************/
#define TX_MSG_ID_SDC_01					0x200u
#define TX_MSG_ID_PHY_RES					0x7DBu
#define TX_MSG_ID_CCP_RES					0x771u
#define TX_MSG_ID_CCP_MON0					0x772u
#define TX_MSG_ID_CCP_MON1					0x773u
#define TX_MSG_ID_CCP_MON2					0x774u
#define TX_MSG_ID_CCP_MON3					0x775u
#define TX_MSG_ID_CCP_MON4					0x776u
#define TX_MSG_ID_CCP_MON5					0x777u
#define TX_MSG_ID_CCP_MON6					0x778u
#define TX_MSG_ID_CCP_MON7					0x779u

/*************** RX CAN MSG ID ****************/
#define RX_MSG_ID_PHY_REQ					0x7D3u
#define RX_MSG_ID_FUN_REQ					0x7DFu
#define RX_MSG_ID_CCP_REQ					0x770u
#define RX_MSG_ID_SAS_01					0x100u

#define REF_VALUE_BAT_4V		                819u	/* 4095*4/20 */

#define TIME_50MS								50u
#define TIME_5S									5000u

#define TIME_DECI_100MS							10u		/* 10ms * 1 */
#define TIME_DECI_1S							100u	/* 10ms * 1 */
#define TIME_DECI_5S							500u	/* 10ms * 1 */

/* DIAGNOSTIC_SESSION */
enum
{
	DIAG_SESSION_DEFAULT,
	DIAG_SESSION_PROGRAMMING,
	DIAG_SESSION_EXTENDED
};

/* RESET_REASON */
enum {
	RESET_REASON_NORMAL = 0u,
	RESET_REASON_REPROGRAMMING_REQUEST,
};
#endif
