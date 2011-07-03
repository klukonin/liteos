/**
   @file hplcc2420fifom.c
   @brief hpl cc2420 implementation. 
   
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
bool hplcc2420fifom_bSpiAvail;
uint8_t *hplcc2420fifom_txbuf;
uint8_t *hplcc2420fifom_rxbuf;
uint8_t hplcc2420fifom_txlength;
uint8_t hplcc2420fifom_rxlength;
bool hplcc2420fifom_rxbufBusy;
bool hplcc2420fifom_txbufBusy;
inline result_t hplcc2420fifom_HPLCC2420FIFO_TXFIFODone(uint8_t arg_0xa40cd20,
                                                        uint8_t *
                                                        arg_0xa40ce80)
{
    unsigned char result;

    result = cc2420radiom_HPLChipconFIFO_TXFIFODone(arg_0xa40cd20,
                                                    arg_0xa40ce80);
    return result;
}

//-------------------------------------------------------------------------
inline void hplcc2420fifom_signalTXdone(void)
{
    uint8_t _txlen;
    uint8_t *_txbuf;

    {
        _atomic_t _atomic = _atomic_start();

        {
            _txlen = hplcc2420fifom_txlength;
            _txbuf = hplcc2420fifom_txbuf;
            hplcc2420fifom_txbufBusy = FALSE;
        }
        _atomic_end(_atomic);
    }
    hplcc2420fifom_HPLCC2420FIFO_TXFIFODone(_txlen, _txbuf);
}

//-------------------------------------------------------------------------
inline result_t hplcc2420fifom_HPLCC2420FIFO_writeTXFIFO(uint8_t len, uint8_t *
                                                         msg)
{
    uint8_t i = 0;
    uint8_t status;
    bool returnFail =  FALSE;

    {
        _atomic_t _atomic = _atomic_start();

        {
            if (hplcc2420fifom_txbufBusy)
            {
                returnFail = TRUE;
            }
            else
            {
                hplcc2420fifom_txbufBusy = TRUE;
            }
        }
        _atomic_end(_atomic);
    }
    if (returnFail)
    {
        return FAIL;
    }
    {
        _atomic_t _atomic = _atomic_start();

        {
            hplcc2420fifom_bSpiAvail = FALSE;
            hplcc2420fifom_txlength = len;
            hplcc2420fifom_txbuf = msg;
            LITE_CLR_CC_CS_PIN();
            outp(CC2420_TXFIFO, SPDR);
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            status = inp(SPDR);
            for (i = 0; i < hplcc2420fifom_txlength; i++)
            {
                outp(*hplcc2420fifom_txbuf, SPDR);
                hplcc2420fifom_txbuf++;
                while (!(inp(SPSR) & 0x80))
                {
                }
                ;               //wait for spi xfr to complete
            }
            hplcc2420fifom_bSpiAvail = TRUE;
        }
        _atomic_end(_atomic);
    }
    LITE_SET_CC_CS_PIN();
    if (postTask(hplcc2420fifom_signalTXdone, 5) == FAIL)
    {
        {
            _atomic_t _atomic = _atomic_start();

            hplcc2420fifom_txbufBusy = FALSE;
            _atomic_end(_atomic);
        }
        return FAIL;
    }
    return status;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420fifom_HPLCC2420FIFO_RXFIFODone(uint8_t arg_0xa40c690,
                                                        uint8_t *
                                                        arg_0xa40c7f0)
{
    unsigned char result;

    result = cc2420radiom_HPLChipconFIFO_RXFIFODone(arg_0xa40c690,
                                                    arg_0xa40c7f0);
    return result;
}

//-------------------------------------------------------------------------
inline void hplcc2420fifom_signalRXdone(void)
{
    uint8_t _rxlen;
    uint8_t *_rxbuf;

    {
        _atomic_t _atomic = _atomic_start();

        {
            _rxlen = hplcc2420fifom_rxlength;
            _rxbuf = hplcc2420fifom_rxbuf;
            hplcc2420fifom_rxbufBusy = FALSE;
        }
        _atomic_end(_atomic);
    }
    hplcc2420fifom_HPLCC2420FIFO_RXFIFODone(_rxlen, _rxbuf);
}

//-------------------------------------------------------------------------
inline
    //Seems that the transmitted len is the actual length over the air 
    //The first number in the frame is the remaining part 
    result_t hplcc2420fifom_HPLCC2420FIFO_readRXFIFO(uint8_t len,
                                                     uint8_t * msg)
{
    uint8_t status;
    uint8_t i;
    bool returnFail = FALSE;

    {
        _atomic_t _atomic = _atomic_start();

        {
            if (hplcc2420fifom_rxbufBusy)
            {
                returnFail = TRUE;
            }
            else
            {
                hplcc2420fifom_rxbufBusy = TRUE;
            }
        }
        _atomic_end(_atomic);
    }
    if (returnFail)
    {
        return FAIL;
    }
    {
        _atomic_t _atomic = _atomic_start();

        {
            hplcc2420fifom_bSpiAvail = FALSE;
            hplcc2420fifom_rxbuf = msg;
            LITE_CLR_CC_CS_PIN();
            outp(CC2420_RXFIFO | 0x40, SPDR);   //output Rxfifo address
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            status = inp(SPDR);
            outp(0, SPDR);
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            hplcc2420fifom_rxlength = inp(SPDR);
            //My revise
            //The first byte, however, only contains a part of the whole, which needs to add 1 to mean that it is the total to be read 
            //and this also defaults to len, which is the MSG_DATA_SIZE
            if (hplcc2420fifom_rxlength > 0)
            {
                hplcc2420fifom_rxbuf[0] = hplcc2420fifom_rxlength;
                hplcc2420fifom_rxlength++;
                if (hplcc2420fifom_rxlength > len)
                {
                    hplcc2420fifom_rxlength = len;
                }
                for (i = 1; i < hplcc2420fifom_rxlength; i++)
                {
                    outp(0, SPDR);
                    while (!(inp(SPSR) & 0x80))
                    {
                    }
                    ;           //wait for spi xfr to complete
                    hplcc2420fifom_rxbuf[i] = inp(SPDR);
                }
            }
            hplcc2420fifom_bSpiAvail = TRUE;
        }
        _atomic_end(_atomic);
    }
    LITE_SET_CC_CS_PIN();
    if (postTask(hplcc2420fifom_signalRXdone, 5) == FAIL)
    {
        {
            _atomic_t _atomic = _atomic_start();

            hplcc2420fifom_rxbufBusy = FALSE;
            _atomic_end(_atomic);
        }
        return FAIL;
    }
    return SUCCESS;
}
