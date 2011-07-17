/**
   @file cc2420radiom.c
   @brief cc2420 implementation 
   
* @author Jason Hill
* @author David Gay
* @author Philip Levis
*/


#include "cc2420const.h"
#include "cc2420controlm.h"
#include "cc2420radiom.h"
#include "hplcc2420fifom.h"
#include "hplcc2420interruptm.h"
#include "hplcc2420m.h"
#include "hpltimer1.h"

#include "../../hardware/avrhardware.h"
#include "../../hardware/micaz/micazhardware.h"

#include "../../config/nodeconfig.h"
#include "../../types/types.h"
#include "../radio/amcommon.h"
#include "../radio/amradio.h"
#include "../radio/packethandler.h"

#include "../../types/byteorder.h"
#include "../../kernel/scheduling.h"
 
uint8_t cc2420radiom_countRetry;
uint8_t cc2420radiom_stateRadio;
uint8_t cc2420radiom_currentDSN;
bool cc2420radiom_bAckEnable;
bool cc2420radiom_bPacketReceiving;
uint8_t cc2420radiom_txlength;
Radio_MsgPtr cc2420radiom_txbufptr;
Radio_MsgPtr cc2420radiom_rxbufptr;
Radio_Msg cc2420radiom_RxBuf;
 
enum cc2420radiom___nesc_unnamed4269
{
    cc2420radiom_DISABLED_STATE = 0, cc2420radiom_DISABLED_STATE_STARTTASK,
    cc2420radiom_IDLE_STATE, cc2420radiom_TX_STATE, cc2420radiom_TX_WAIT,
    cc2420radiom_PRE_TX_STATE, cc2420radiom_POST_TX_STATE,
    cc2420radiom_POST_TX_ACK_STATE, cc2420radiom_RX_STATE,
    cc2420radiom_POWER_DOWN_STATE, cc2420radiom_WARMUP_STATE,
    cc2420radiom_TIMER_INITIAL = 0, cc2420radiom_TIMER_BACKOFF,
    cc2420radiom_TIMER_ACK
};


inline result_t cc2420radiom_SplitControl_default_initDone(void)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_CC2420SplitControl_initDone(void)
{
    return cc2420radiom_SplitControl_initDone();
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SplitControl_init(void)
{
    {
        _atomic_t _atomic = _atomic_start();

        {
            cc2420radiom_stateRadio = cc2420radiom_DISABLED_STATE;
            cc2420radiom_currentDSN = 0;
            cc2420radiom_bAckEnable = FALSE;
            cc2420radiom_bPacketReceiving = FALSE;
            cc2420radiom_rxbufptr = &cc2420radiom_RxBuf;
            cc2420radiom_rxbufptr->length = 0;
        }
        _atomic_end(_atomic);
    }
 
    return cc2420controlm_SplitControl_init();
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_CC2420SplitControl_start(void)
{
    unsigned char result;

    result = cc2420controlm_SplitControl_start();
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SplitControl_start(void)
{
    uint8_t chkstateRadio;

    {
        _atomic_t _atomic = _atomic_start();

        chkstateRadio = cc2420radiom_stateRadio;
        _atomic_end(_atomic);
    }
    if (chkstateRadio == cc2420radiom_DISABLED_STATE)
    {
        {
            _atomic_t _atomic = _atomic_start();

            {
                cc2420radiom_stateRadio = cc2420radiom_WARMUP_STATE;
                cc2420radiom_countRetry = 0;
                cc2420radiom_rxbufptr->length = 0;
            }
            _atomic_end(_atomic);
        }
        return cc2420radiom_CC2420SplitControl_start();
    }
    return FAIL;
}

//-------------------------------------------------------------------------
inline void cc2420radiom_startRadio(void)
{
    result_t success = FAIL;

    {
        _atomic_t _atomic = _atomic_start();

        {
            if (cc2420radiom_stateRadio ==
                cc2420radiom_DISABLED_STATE_STARTTASK)
            {
                cc2420radiom_stateRadio = cc2420radiom_DISABLED_STATE;
                success = SUCCESS;
            }
        }
        _atomic_end(_atomic);
    }
    if (success == SUCCESS)
    {
        cc2420radiom_SplitControl_start();
    }
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_StdControl_start(void)
{
    result_t success = FAIL;

    {
        _atomic_t _atomic = _atomic_start();

        {
            if (cc2420radiom_stateRadio == cc2420radiom_DISABLED_STATE)
            {
                if (postTask(cc2420radiom_startRadio, 5))
                {
                    success = SUCCESS;
                    cc2420radiom_stateRadio =
                        cc2420radiom_DISABLED_STATE_STARTTASK;
                }
            }
        }
        _atomic_end(_atomic);
    }
    return success;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SplitControl_default_startDone(void)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SplitControl_startDone(void)
{
    unsigned char result;

    result = cc2420radiom_SplitControl_default_startDone();
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SFD_enableCapture(bool arg_0xa41e260)
{
    unsigned char result;

    result = hplcc2420interruptm_SFD_enableCapture(arg_0xa41e260);
    return result;
}

//-------------------------------------------------------------------------
inline uint8_t cc2420radiom_HPLChipcon_cmd(uint8_t arg_0xa403928)
{
    unsigned char result;

    result = HPLCC2420M_HPLCC2420_cmd(arg_0xa403928);
    return result;
}

//-------------------------------------------------------------------------
inline void cc2420radiom_sendPacket(void)
{
    uint8_t status;

    cc2420radiom_HPLChipcon_cmd(0x05);
    status = cc2420radiom_HPLChipcon_cmd(0x00);
    if ((status >> 3) & 0x01)
    {
        cc2420radiom_SFD_enableCapture(TRUE);
    }
    else
    {
        cc2420radiom_sendFailed();
    }
}

//-------------------------------------------------------------------------
inline void cc2420radiom_tryToSend(void)
{
    uint8_t currentstate;

    {
        _atomic_t _atomic = _atomic_start();

        currentstate = cc2420radiom_stateRadio;
        _atomic_end(_atomic);
    }
    if (currentstate == cc2420radiom_PRE_TX_STATE)
    {
        if (!LITE_READ_CC_FIFO_PIN() && !LITE_READ_CC_FIFOP_PIN())
        {
            cc2420radiom_flushRXFIFO();
        }
        if (LITE_READ_RADIO_CCA_PIN())
        {
            {
                _atomic_t _atomic = _atomic_start();

                cc2420radiom_stateRadio = cc2420radiom_TX_STATE;
                _atomic_end(_atomic);
            }
            cc2420radiom_sendPacket();
        }
    }
    else
    {
        cc2420radiom_flushRXFIFO();
    }
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_HPLChipconFIFO_TXFIFODone(uint8_t length,
                                                       uint8_t * data)
{
    cc2420radiom_tryToSend();
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_HPLChipconFIFO_writeTXFIFO(uint8_t arg_0xa40c010,
                                                        uint8_t *
                                                        arg_0xa40c170)
{
    unsigned char result;

    result = hplcc2420fifom_HPLCC2420FIFO_writeTXFIFO(arg_0xa40c010,
                                                      arg_0xa40c170);
    return result;
}

//-------------------------------------------------------------------------
inline void cc2420radiom_startSend(void)
{
    if (!cc2420radiom_HPLChipcon_cmd(0x09))
    {
        cc2420radiom_sendFailed();
        return;
    }
    if (!cc2420radiom_HPLChipconFIFO_writeTXFIFO(cc2420radiom_txlength + 1,
                                                 (uint8_t *)
                                                 cc2420radiom_txbufptr))
    {
        cc2420radiom_sendFailed();
        return;
    }
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_Send_send(Radio_MsgPtr pMsg)
{
    uint8_t currentstate;

    {
        _atomic_t _atomic = _atomic_start();

        currentstate = cc2420radiom_stateRadio;
        _atomic_end(_atomic);
    }

     if (currentstate == cc2420radiom_IDLE_STATE)
    {
        pMsg->fcflo = 0x08;
        if (cc2420radiom_bAckEnable)
        {
            pMsg->fcfhi = 0x21;
        }
        else
        {
            pMsg->fcfhi = 0x01;
        }
        pMsg->destpan = BCAST_ADDRESS;
        //don't understand why it has to be LSB. Must check later. 
        pMsg->addr = toLSB16(pMsg->addr);
        //The length now includes both the 
        pMsg->length = pMsg->length + MSG_HEADER_SIZE + MSG_FOOTER_SIZE;
        pMsg->dsn = ++cc2420radiom_currentDSN;
        pMsg->time = 0;
        cc2420radiom_txlength = pMsg->length - MSG_FOOTER_SIZE;
        cc2420radiom_txbufptr = pMsg;
        cc2420radiom_countRetry = 8;
        {
            _atomic_t _atomic = _atomic_start();

            cc2420radiom_stateRadio = cc2420radiom_PRE_TX_STATE;
            _atomic_end(_atomic);
        }
        if (!postTask(cc2420radiom_startSend, 5))
        {
            cc2420radiom_sendFailed();
        }
        return SUCCESS;
    }
    return FAIL;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_FIFOP_startWait(bool arg_0xa422588)
{
    unsigned char result;

    result = hplcc2420interruptm_FIFOP_startWait(arg_0xa422588);
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_CC2420Control_RxMode(void)
{
    unsigned char result;

    result = cc2420controlm_CC2420Control_RxMode();
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_CC2420SplitControl_startDone(void)
{
    uint8_t chkstateRadio;

    {
        _atomic_t _atomic = _atomic_start();

        chkstateRadio = cc2420radiom_stateRadio;
        _atomic_end(_atomic);
    }
    if (chkstateRadio == cc2420radiom_WARMUP_STATE)
    {
        cc2420radiom_CC2420Control_RxMode();
        cc2420radiom_FIFOP_startWait(FALSE);
        cc2420radiom_SFD_enableCapture(TRUE);
        {
            _atomic_t _atomic = _atomic_start();

            cc2420radiom_stateRadio = cc2420radiom_IDLE_STATE;
            _atomic_end(_atomic);
        }
    }
    cc2420radiom_SplitControl_startDone();
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline uint16_t cc2420radiom_HPLChipcon_read(uint8_t arg_0xa4103b0)
{
    unsigned int result;

    result = HPLCC2420M_HPLCC2420_read(arg_0xa4103b0);
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_FIFOP_disable(void)
{
    unsigned char result;

    result = hplcc2420interruptm_FIFOP_disable();
    return result;
}

//-------------------------------------------------------------------------
inline void cc2420radiom_delayedRXFIFOtask(void)
{
    cc2420radiom_delayedRXFIFO();
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_FIFOP_fired(void)
{
    if (!LITE_READ_CC_FIFO_PIN())
    {
        cc2420radiom_flushRXFIFO();
        return SUCCESS;
    }
    {
        _atomic_t _atomic = _atomic_start();

        {
            if (postTask(cc2420radiom_delayedRXFIFOtask, 5))
            {
                cc2420radiom_FIFOP_disable();
            }
            else
            {
                cc2420radiom_flushRXFIFO();
            }
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline Radio_MsgPtr cc2420radiom_Receive_receive(Radio_MsgPtr arg_0xa31b650)
{
    struct Radio_Msg *result;

    result = AMStandard_RadioReceive_receive(arg_0xa31b650);
    return result;
}

//-------------------------------------------------------------------------
inline void cc2420radiom_PacketRcvd(void)
{
    Radio_MsgPtr pBuf;

    {
        _atomic_t _atomic = _atomic_start();

        {
            pBuf = cc2420radiom_rxbufptr;
        } _atomic_end(_atomic);
    }
    pBuf = cc2420radiom_Receive_receive((Radio_MsgPtr) pBuf);
    {
        _atomic_t _atomic = _atomic_start();

        {
            if (pBuf)
            {
                cc2420radiom_rxbufptr = pBuf;
            }
            cc2420radiom_rxbufptr->length = 0;
            cc2420radiom_bPacketReceiving = FALSE;
        }
        _atomic_end(_atomic);
    }
}

//ALL THE DATA IN LENGTH IS THE CORRECT FRAME 
inline result_t cc2420radiom_HPLChipconFIFO_RXFIFODone(uint8_t length,
                                                       uint8_t * data)
{
    //The data now contains the full packet, starting with a packet length which is the remaining part of the packet, not including itself
    //The length, however, includes everything 
    uint8_t currentstate;

    {
        _atomic_t _atomic = _atomic_start();

        {
            currentstate = cc2420radiom_stateRadio;
        }
        _atomic_end(_atomic);
    }
    if (((!LITE_READ_CC_FIFO_PIN() && !LITE_READ_CC_FIFOP_PIN()) || length
         == 0) || length > MSG_DATA_SIZE)
    {
        cc2420radiom_flushRXFIFO();
        {
            _atomic_t _atomic = _atomic_start();

            cc2420radiom_bPacketReceiving = FALSE;
            _atomic_end(_atomic);
        }
        return SUCCESS;
    }
    cc2420radiom_rxbufptr = (Radio_MsgPtr) data;
    if ((cc2420radiom_rxbufptr->fcfhi & 0x07) != 0x01
        || cc2420radiom_rxbufptr->fcflo != 0x08)
    {
        cc2420radiom_flushRXFIFO();
        {
            _atomic_t _atomic = _atomic_start();

            cc2420radiom_bPacketReceiving = FALSE;
            _atomic_end(_atomic);
        }
        return SUCCESS;
    }
    //Now modifies the length such that it only points to the actual payload defined by the user 
    cc2420radiom_rxbufptr->length = cc2420radiom_rxbufptr->length -
        MSG_HEADER_SIZE - MSG_FOOTER_SIZE;
    //Revised by Qing, from 29 to PAYLOAD_LENGTH 
    if (cc2420radiom_rxbufptr->length > PAYLOAD_LENGTH)
    {
        cc2420radiom_flushRXFIFO();
        {
            _atomic_t _atomic = _atomic_start();

            cc2420radiom_bPacketReceiving = FALSE;
            _atomic_end(_atomic);
        }
        return SUCCESS;
    }
    cc2420radiom_rxbufptr->addr = fromLSB16(cc2420radiom_rxbufptr->addr);
    //CRC and LQI share the information 
    cc2420radiom_rxbufptr->crc = data[length - 1] >> 7;
    cc2420radiom_rxbufptr->strength = data[length - 2];
    cc2420radiom_rxbufptr->lqi = data[length - 1] & 0x7F;
    {
        _atomic_t _atomic = _atomic_start();

        {
            if (!postTask(cc2420radiom_PacketRcvd, 5))
            {
                cc2420radiom_bPacketReceiving = FALSE;
            }
        }
        _atomic_end(_atomic);
    }
    if (!LITE_READ_CC_FIFO_PIN() && !LITE_READ_CC_FIFOP_PIN())
    {
        cc2420radiom_flushRXFIFO();
        return SUCCESS;
    }
    if (!LITE_READ_CC_FIFOP_PIN())
    {
        if (postTask(cc2420radiom_delayedRXFIFOtask, 5))
        {
            return SUCCESS;
        }
    }
    cc2420radiom_flushRXFIFO();
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_HPLChipconFIFO_readRXFIFO(uint8_t arg_0xa4118a0,
                                                       uint8_t * arg_0xa411a00)
{
    unsigned char result;

    result = hplcc2420fifom_HPLCC2420FIFO_readRXFIFO(arg_0xa4118a0,
                                                     arg_0xa411a00);
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_Send_sendDone(Radio_MsgPtr arg_0xa3c3710,
                                           result_t arg_0xa3c3860)
{
    unsigned char result;

    result = AMStandard_RadioSend_sendDone(arg_0xa3c3710, arg_0xa3c3860);
    return result;
}

//-------------------------------------------------------------------------
inline void cc2420radiom_PacketSent(void)
{
    Radio_MsgPtr pBuf;

    {
        _atomic_t _atomic = _atomic_start();

        {
            cc2420radiom_stateRadio = cc2420radiom_IDLE_STATE;
            pBuf = cc2420radiom_txbufptr;
            pBuf->length = pBuf->length - MSG_HEADER_SIZE - MSG_FOOTER_SIZE;
        }
        _atomic_end(_atomic);
    }
    cc2420radiom_Send_sendDone(pBuf, SUCCESS);
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SFD_disable(void)
{
    unsigned char result;

    result = hplcc2420interruptm_SFD_disable();
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SFD_captured(uint16_t time)
{
    switch (cc2420radiom_stateRadio)
    {
    case cc2420radiom_TX_STATE:
        cc2420radiom_SFD_enableCapture(FALSE);
        if (!LITE_READ_CC_SFD_PIN())
        {
            cc2420radiom_SFD_disable();
        }
        else
        {
            cc2420radiom_stateRadio = cc2420radiom_TX_WAIT;
        }
        cc2420radiom_txbufptr->time = time;
        if (cc2420radiom_stateRadio == cc2420radiom_TX_WAIT)
        {
            break;
        }
    case cc2420radiom_TX_WAIT:
        cc2420radiom_stateRadio = cc2420radiom_POST_TX_STATE;
        cc2420radiom_SFD_disable();
        cc2420radiom_SFD_enableCapture(TRUE);
        if (!postTask(cc2420radiom_PacketSent, 5))
        {
            cc2420radiom_sendFailed();
        }
        break;
    default:
        ;
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
void cc2420radiom_sendFailed(void)
{
    {
        _atomic_t _atomic = _atomic_start();

        cc2420radiom_stateRadio = cc2420radiom_IDLE_STATE;
        _atomic_end(_atomic);
    }
    cc2420radiom_txbufptr->length = cc2420radiom_txbufptr->length -
        MSG_HEADER_SIZE - MSG_FOOTER_SIZE;
    cc2420radiom_Send_sendDone(cc2420radiom_txbufptr, FAIL);
}

//-------------------------------------------------------------------------
void cc2420radiom_flushRXFIFO(void)
{
    cc2420radiom_FIFOP_disable();
    cc2420radiom_HPLChipcon_read(0x3F);
    cc2420radiom_HPLChipcon_cmd(0x08);
    cc2420radiom_HPLChipcon_cmd(0x08);
    {
        _atomic_t _atomic = _atomic_start();

        cc2420radiom_bPacketReceiving = FALSE;
        _atomic_end(_atomic);
    }
    cc2420radiom_FIFOP_startWait(FALSE);
}

//-------------------------------------------------------------------------
void cc2420radiom_delayedRXFIFO(void)
{
    //The MSG_DATA_SIZE is defined such that it contains the total length of data plus 2 bytes of the RSSI and LQI 
    uint8_t len = MSG_DATA_SIZE;
    uint8_t _bPacketReceiving;

    if (!LITE_READ_CC_FIFO_PIN() && !LITE_READ_CC_FIFOP_PIN())
    {
        cc2420radiom_flushRXFIFO();
        return;
    }
    {
        _atomic_t _atomic = _atomic_start();

        {
            _bPacketReceiving = cc2420radiom_bPacketReceiving;
            if (_bPacketReceiving)
            {
                if (!postTask(cc2420radiom_delayedRXFIFOtask, 5))
                {
                    cc2420radiom_flushRXFIFO();
                }
            }
            else
            {
                cc2420radiom_bPacketReceiving = TRUE;
            }
        }
        _atomic_end(_atomic);
    }
    //Now read the RX to get the data 
    if (!_bPacketReceiving)
    {
        if (!cc2420radiom_HPLChipconFIFO_readRXFIFO(len, (uint8_t *)
                                                    cc2420radiom_rxbufptr))
        {
            {
                _atomic_t _atomic = _atomic_start();

                cc2420radiom_bPacketReceiving = FALSE;
                _atomic_end(_atomic);
            }
            if (!postTask(cc2420radiom_delayedRXFIFOtask, 5))
            {
                cc2420radiom_flushRXFIFO();
            }
            return;
        }
    }
    cc2420radiom_flushRXFIFO();
}

//-------------------------------------------------------------------------
inline result_t cc2420radiom_SplitControl_initDone(void)
{
    unsigned char result;

    result = cc2420radiom_SplitControl_default_initDone();
    return result;
}

//-------------------------------------------------------------------------
inline void restorecc2420state()
{
    cc2420radiom_stateRadio = 2;
}
