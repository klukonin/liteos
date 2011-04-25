#ifndef AMRADIOH
#define AMRADIOH
#include "amcommon.h"


extern bool AMStandard_state;
inline bool AMStandard_Control_init(void);
inline result_t AMStandard_socketradiocontrol_start(void);
inline result_t AMStandard_TimerControl_start(void);
inline bool AMStandard_Control_start(void);
inline result_t AMStandard_RadioSend_send(Radio_MsgPtr arg_0xa3c31f8);
inline void AMStandard_sendTask(void);
result_t AMStandard_SendMsg_send(uint16_t port, uint16_t addr, uint8_t length,
                                 Radio_MsgPtr data);
inline result_t AMStandard_SendMsg_default_sendDone(uint8_t id, Radio_MsgPtr
                                                    msg, result_t success);
inline result_t AMStandard_SendMsg_sendDone(uint16_t arg_0xa3b8f90,
                                            Radio_MsgPtr arg_0xa31a0a0,
                                            result_t arg_0xa31a1f0);
inline result_t AMStandard_default_sendDone(void);
inline Radio_MsgPtr AMStandard_ReceiveMsg_default_receive(uint8_t id,
                                                          Radio_MsgPtr msg);
inline Radio_MsgPtr AMStandard_ReceiveMsg_receive(uint16_t port, Radio_MsgPtr
                                                  msg);
inline Radio_MsgPtr received(Radio_MsgPtr packet);
inline Radio_MsgPtr AMStandard_RadioReceive_receive(Radio_MsgPtr packet);
inline result_t AMStandard_RadioSend_sendDone(Radio_MsgPtr msg, result_t
                                              success);
inline result_t AMStandard_reportSendDone(Radio_MsgPtr msg, result_t success);
inline void restoreRadioState();
#endif
