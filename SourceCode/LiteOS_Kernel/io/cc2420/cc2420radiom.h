/**
   @file cc2420radiom.h
   @brief cc2420 implementation 
   
* @author Jason Hill
* @author David Gay
* @author Philip Levis
*/

#ifndef CC2420RADIOMH
#define CC2420RADIOMH
#include "../radio/amcommon.h"
#include "../../types/types.h"
/** \addtogroup radio */
/** @{ */
inline result_t cc2420radiom_SplitControl_default_initDone(void);
inline result_t cc2420radiom_CC2420SplitControl_initDone(void);
inline result_t cc2420radiom_SplitControl_init(void);
inline result_t cc2420radiom_CC2420SplitControl_start(void);
inline result_t cc2420radiom_SplitControl_start(void);
inline void cc2420radiom_startRadio(void);
inline result_t cc2420radiom_StdControl_start(void);
inline result_t cc2420radiom_SFD_enableCapture(bool arg_0xa41e260);
inline uint8_t cc2420radiom_HPLChipcon_cmd(uint8_t arg_0xa403928);
inline void cc2420radiom_sendPacket(void);
inline void cc2420radiom_tryToSend(void);
inline result_t cc2420radiom_HPLChipconFIFO_TXFIFODone(uint8_t length,
                                                       uint8_t * data);
inline result_t cc2420radiom_HPLChipconFIFO_writeTXFIFO(uint8_t arg_0xa40c010,
                                                        uint8_t *
                                                        arg_0xa40c170);
inline void cc2420radiom_startSend(void);
inline result_t cc2420radiom_Send_send(Radio_MsgPtr pMsg);
inline result_t cc2420radiom_SplitControl_default_startDone(void);
inline result_t cc2420radiom_FIFOP_startWait(bool arg_0xa422588);
inline result_t cc2420radiom_CC2420Control_RxMode(void);
inline result_t cc2420radiom_CC2420SplitControl_startDone(void);
inline uint16_t cc2420radiom_HPLChipcon_read(uint8_t arg_0xa4103b0);
inline result_t cc2420radiom_FIFOP_disable(void);
inline void cc2420radiom_delayedRXFIFOtask(void);
inline result_t cc2420radiom_FIFOP_fired(void);
inline Radio_MsgPtr cc2420radiom_Receive_receive(Radio_MsgPtr arg_0xa31b650);
inline void cc2420radiom_PacketRcvd(void);
inline result_t cc2420radiom_HPLChipconFIFO_RXFIFODone(uint8_t length,
                                                       uint8_t * data);
inline result_t cc2420radiom_HPLChipconFIFO_readRXFIFO(uint8_t arg_0xa4118a0,
                                                       uint8_t *
                                                       arg_0xa411a00);
inline result_t cc2420radiom_Send_sendDone(Radio_MsgPtr arg_0xa3c3710,
                                           result_t arg_0xa3c3860);
inline void cc2420radiom_PacketSent(void);
inline result_t cc2420radiom_SFD_disable(void);
inline result_t cc2420radiom_SFD_captured(uint16_t time);
inline void cc2420radiom_sendFailed(void);
inline void cc2420radiom_flushRXFIFO(void);
inline void cc2420radiom_delayedRXFIFO(void);
inline result_t cc2420radiom_SplitControl_initDone(void);
inline void restorecc2420state();

/**@} */
#endif
