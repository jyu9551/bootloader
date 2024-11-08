#ifndef UdsNrcList_H
#define UdsNrcList_H

#define NRC_GeneralReject 0x10u /* GR */
#define NRC_ServiceNotSupported 0x11u /* SNS */
#define NRC_SubFunctionNotSupported 0x12u /* SFNS */
#define NRC_IncorrectMessageLengthOrInvalidFormat 0x13u /* IML_OR_IF */
#define NRC_ResponseTooLong 0x14u /* RTL */
#define NRC_BusyRepeatRequest 0x21u /* BRR */
#define NRC_ConditionNotCorrect 0x22u /* CNC */
#define NRC_RequestSequenceError 0x24u /* TSE */
#define NRC_RequestOutRange 0x31u /* ROOR */
#define NRC_SecurityAccessDenied 0x33u /* SAD */
#define NRC_InvalidKey 0x35u /* IK */
#define NRC_ExceddedNumberOfAttempts 0x36u /* ENOA */
#define NRC_RequiredTimeDelayNotExpired 0x37u /* RTDNE */
#define NRC_UploadDownloadNotAccepted 0x70u /* UDNA */
#define NRC_TransferDataSuspended 0x71u /* TDS */
#define NRC_GeneralProgrammingFailure 0x72u /* GPF */
#define NRC_WrongBlockSequeceNumber 0x73u /* WBWN */
#define NRC_SubFunctionNotSupportedInActiveSession 0x7Eu /* SFNSIAS */
#define NRC_ServiceNotSupportedInActiveSession 0x7Fu /* SNSIAS */
#define NRC_RequestCorrectlyReceivedResponsePending 0x78u /* RCRRP */
#define NRC_VoltageTooHigh 0x92 /* VTH */
#define NRC_VoltageTooLow 0x93 /* VTL */

#endif
