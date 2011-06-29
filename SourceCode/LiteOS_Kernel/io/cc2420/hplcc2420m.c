/**
   @file hplcc2420m.c
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
bool HPLCC2420M_bSpiAvail;
uint8_t *HPLCC2420M_rambuf;
uint8_t HPLCC2420M_ramlen;
uint16_t HPLCC2420M_ramaddr;

//set up basic settings
inline result_t HPLCC2420M_StdControl_init(void)
{
    HPLCC2420M_bSpiAvail = TRUE;
    LITE_MAKE_MISO_INPUT();
    LITE_MAKE_MOSI_OUTPUT();
    LITE_MAKE_SPI_SCK_OUTPUT();
    LITE_MAKE_CC_RSTN_OUTPUT();
    LITE_MAKE_CC_VREN_OUTPUT();
    LITE_MAKE_CC_CS_OUTPUT();
    LITE_MAKE_CC_FIFOP1_INPUT();
    LITE_MAKE_CC_CCA_INPUT();
    LITE_MAKE_CC_SFD_INPUT();
    LITE_MAKE_CC_FIFO_INPUT();
    {
        _atomic_t _atomic = _atomic_start();

        {
            LITE_MAKE_SPI_SCK_OUTPUT();
            LITE_MAKE_MISO_INPUT();
            LITE_MAKE_MOSI_OUTPUT();
            sbi(SPSR, SPI2X);   // Double speed spi clock
            sbi(SPCR, MSTR);    // Set master mode
            cbi(SPCR, CPOL);    // Set proper polarity...
            cbi(SPCR, CPHA);    // ...and phase
            cbi(SPCR, SPR1);    // set clock, fosc/2 (~3.6 Mhz)
            cbi(SPCR, SPR0);
            //    sbi(SPCR, SPIE);             // enable spi port interrupt
            sbi(SPCR, SPE);     // enable spie port
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t HPLCC2420M_HPLCC2420RAM_writeDone(uint16_t arg_0xa45b460,
                                                  uint8_t arg_0xa45b5a8,
                                                  uint8_t * arg_0xa45b708)
{
    unsigned char result;

    result = cc2420controlm_HPLChipconRAM_writeDone(arg_0xa45b460,
                                                    arg_0xa45b5a8,
                                                    arg_0xa45b708);
    return result;
}

//-------------------------------------------------------------------------
inline void HPLCC2420M_signalRAMWr(void)
{
    HPLCC2420M_HPLCC2420RAM_writeDone(HPLCC2420M_ramaddr, HPLCC2420M_ramlen,
                                      HPLCC2420M_rambuf);
}

//-------------------------------------------------------------------------
inline result_t HPLCC2420M_HPLCC2420RAM_write(uint16_t addr, uint8_t length,
                                              uint8_t * buffer)
{
    uint8_t i = 0;
    uint8_t status;

    if (!HPLCC2420M_bSpiAvail)
    {
        return FALSE;
    }
    {
        _atomic_t _atomic = _atomic_start();

        {
            HPLCC2420M_bSpiAvail = FALSE;
            HPLCC2420M_ramaddr = addr;
            HPLCC2420M_ramlen = length;
            HPLCC2420M_rambuf = buffer;
            LITE_CLR_CC_CS_PIN();
            outp(((HPLCC2420M_ramaddr & 0x7F) | 0x80), SPDR);
            //ls address      and set RAM/Reg flagbit
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            status = inp(SPDR);
            outp(((HPLCC2420M_ramaddr >> 1) & 0xC0), SPDR);     //ms address
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            status = inp(SPDR);
            for (i = 0; i < HPLCC2420M_ramlen; i++)
            {
                //buffer write
                outp(HPLCC2420M_rambuf[i], SPDR);
                //        call USARTControl.tx(rambuf[i]);
                while (!(inp(SPSR) & 0x80))
                {
                }
                ;               //wait for spi xfr to complete
            }
        }
        _atomic_end(_atomic);
    }
    HPLCC2420M_bSpiAvail = TRUE;
    return postTask(HPLCC2420M_signalRAMWr, 5);
}

//-------------------------------------------------------------------------
inline result_t HPLCC2420M_HPLCC2420_write(uint8_t addr, uint16_t data)
{
    uint8_t status;

    {
        _atomic_t _atomic = _atomic_start();

        {
            HPLCC2420M_bSpiAvail = FALSE;
            LITE_CLR_CC_CS_PIN();
            outp(addr, SPDR);
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            status = inp(SPDR);
            if (addr > CC2420_SAES)
            {
                outp(data >> 8, SPDR);
                while (!(inp(SPSR) & 0x80))
                {
                }
                ;               //wait for spi xfr to complete
                outp(data & 0xff, SPDR);
                while (!(inp(SPSR) & 0x80))
                {
                }
                ;               //wait for spi xfr to complete
            }
            HPLCC2420M_bSpiAvail = TRUE;
        }
        _atomic_end(_atomic);
    }
    LITE_SET_CC_CS_PIN();       //disable chip select
    return status;
}

//-------------------------------------------------------------------------
inline uint8_t HPLCC2420M_HPLCC2420_cmd(uint8_t addr)
{
    uint8_t status;

    {
        _atomic_t _atomic = _atomic_start();

        {
            LITE_CLR_CC_CS_PIN();
            outp(addr, SPDR);
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            status = inp(SPDR);
        }
        _atomic_end(_atomic);
    }
    LITE_SET_CC_CS_PIN();
    return status;
}

//-------------------------------------------------------------------------
inline uint16_t HPLCC2420M_HPLCC2420_read(uint8_t addr)
{
    uint16_t data = 0;
    uint8_t status;

    {
        _atomic_t _atomic = _atomic_start();

        {
            HPLCC2420M_bSpiAvail = FALSE;
            LITE_CLR_CC_CS_PIN();       //enable chip select
            outp(addr | 0x40, SPDR);
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
            data = inp(SPDR);
            outp(0, SPDR);
            while (!(inp(SPSR) & 0x80))
            {
            }
            ;                   //wait for spi xfr to complete
            data = (data << 8) | inp(SPDR);
            LITE_SET_CC_CS_PIN();       //disable chip select
            HPLCC2420M_bSpiAvail = TRUE;
        }
        _atomic_end(_atomic);
    }
    return data;
}
