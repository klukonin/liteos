/** @file cc2420controlm.c
       @brief The definitions of the control module for cc2420. 

	This file implements the cc2420 control module. 
	@author Joe Polastre
*     @author Alan Broad
	@author Qing Charles Cao 
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
 



 
uint8_t cc2420controlm_state;
uint16_t cc2420controlm_gCurrentParameters[14];
inline result_t cc2420controlm_SplitControl_init(void)
{
    uint8_t _state = FALSE;

    {
        _atomic_t _atomic = _atomic_start();

        {
            if (cc2420controlm_state == cc2420controlm_IDLE_STATE)
            {
                cc2420controlm_state = cc2420controlm_INIT_STATE;
                _state = TRUE;
            }
        }
        //Qing Revision
        //IDLE state
        cc2420controlm_state = cc2420controlm_IDLE_STATE;
       
    
        _atomic_end(_atomic);
    }
    if (!_state)
    {
        return FAIL;
    }
    cc2420controlm_HPLChipconControl_init();
    //Basically, reset everything page 64
    cc2420controlm_gCurrentParameters[CP_MAIN] = 0xf800;
    //Basically, check page 65. Very easy 
    cc2420controlm_gCurrentParameters[CP_MDMCTRL0] =
        ((((0 << 11) | (2 << 8)) | (3 << 6)) | (1 << 5)) | (2 << 0);
    //Page 66
    cc2420controlm_gCurrentParameters[CP_MDMCTRL1] = 20 << 6;
    //reset values
    cc2420controlm_gCurrentParameters[CP_RSSI] = 0xE080;
    //reset
    cc2420controlm_gCurrentParameters[CP_SYNCWORD] = 0xA70F;
    //the last one, according to page 52 of the data sheet and page 67, default to output 0dbm 
    cc2420controlm_gCurrentParameters[CP_TXCTRL] =
        ((((1 << 14) | (1 << 13)) | (3 << 6)) | (1 << 5)) | (CC2420_DEF_RFPOWER <<
                                                             0);
    cc2420controlm_gCurrentParameters[CP_RXCTRL0] =
        (((((1 << 12) | (2 << 8)) | (3 << 6)) | (2 << 4)) | (1 << 2)) | (1 <<
                                                                         0);
    cc2420controlm_gCurrentParameters[CP_RXCTRL1] =
        (((((1 << 11) | (1 << 9)) | (1 << 6)) | (1 << 4)) | (1 << 2)) | (2 <<
                                                                         0);
    //PAGE 51 of the manual 
    cc2420controlm_gCurrentParameters[CP_FSCTRL] =
        (1 << 14) | ((357 + 5 * (CC2420_DEF_CHANNEL - 11)) << 0);
    cc2420controlm_gCurrentParameters[CP_SECCTRL0] =
        (((1 << 8) | (1 << 7)) | (1 << 6)) | (1 << 2);
    cc2420controlm_gCurrentParameters[CP_SECCTRL1] = 0;
    cc2420controlm_gCurrentParameters[CP_BATTMON] = 0;
    //fifop and cca polarity are inversed
    cc2420controlm_gCurrentParameters[CP_IOCFG0] = (127 << 0) | (1 << 9);
    cc2420controlm_gCurrentParameters[CP_IOCFG1] = 0;
    {
        _atomic_t _atomic = _atomic_start();

        cc2420controlm_state = cc2420controlm_INIT_STATE_DONE;
        _atomic_end(_atomic);
    }
    postTask(cc2420controlm_taskInitDone, 5);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CCA_startWait(bool arg_0xa422588)
{
    unsigned char result;

    result = hplcc2420interruptm_CCA_startWait(arg_0xa422588);
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CC2420Control_VREFOn(void)
{
    LITE_SET_CC_VREN_PIN();
    LITE_uwait(600);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CC2420Control_VREFOff(void)
{
    LITE_CLR_CC_VREN_PIN();
    LITE_uwait(600);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_SplitControl_initDone(void)
{
    unsigned char result;

    result = cc2420radiom_CC2420SplitControl_initDone();
    return result;
}

//-------------------------------------------------------------------------
inline void cc2420controlm_taskInitDone(void)
{
    cc2420controlm_SplitControl_initDone();
}

//-------------------------------------------------------------------------
inline uint8_t cc2420controlm_HPLChipcon_cmd(uint8_t arg_0xa403928)
{
    unsigned char result;

    result = HPLCC2420M_HPLCC2420_cmd(arg_0xa403928);
    return result;
}

//-------------------------------------------------------------------------
inline uint8_t cc2420controlm_HPLChipcon_write(uint8_t arg_0xa403d80, uint16_t
                                               arg_0xa403ed0)
{
    unsigned char result;

    result = HPLCC2420M_HPLCC2420_write(arg_0xa403d80, arg_0xa403ed0);
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CC2420Control_OscillatorOn(void)
{
    uint16_t i;
    uint8_t status;

    i = 0;
    cc2420controlm_HPLChipcon_write(0x1D, 24);
    cc2420controlm_CCA_startWait(TRUE);
    status = cc2420controlm_HPLChipcon_cmd(0x01);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_HPLChipconControl_start(void)
{
    unsigned char result;

    result = HPLTimer1M_StdControl_start();
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_SplitControl_start(void)
{
    result_t status;
    uint8_t _state = FALSE;

    {
        _atomic_t _atomic = _atomic_start();

        {
            if (cc2420controlm_state == cc2420controlm_INIT_STATE_DONE)
            {
                cc2420controlm_state = cc2420controlm_START_STATE;
                _state = TRUE;
            }
        }
        _atomic_end(_atomic);
    }
    if (!_state)
    {
        return FAIL;
    }
    cc2420controlm_HPLChipconControl_start();
    cc2420controlm_CC2420Control_VREFOn();
    LITE_CLR_CC_RSTN_PIN();
    wait_cycle();
    LITE_SET_CC_RSTN_PIN();
    wait_cycle();
    status = cc2420controlm_CC2420Control_OscillatorOn();
    return status;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CC2420Control_RxMode(void)
{
    cc2420controlm_HPLChipcon_cmd(0x03);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_SplitControl_startDone(void)
{
    unsigned char result;

    result = cc2420radiom_CC2420SplitControl_startDone();
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CC2420Control_TuneManual(uint16_t DesiredFreq)
{
    int fsctrl;
    uint8_t status;

    fsctrl = DesiredFreq - 2048;
    cc2420controlm_gCurrentParameters[CP_FSCTRL] =
        (cc2420controlm_gCurrentParameters[CP_FSCTRL] & 0xfc00) | (fsctrl <<
                                                                   0);
    status =
        cc2420controlm_HPLChipcon_write(0x18,
                                        cc2420controlm_gCurrentParameters
                                        [CP_FSCTRL]);
    //IF THE oscillator is running, turn on the rx mode 
    // STATUS bit 06 means that the oscillator is running or not 
    if (status & (1 << 6))
    {
        cc2420controlm_HPLChipcon_cmd(0x03);
    }
    return SUCCESS;
}

//the channel must be 11 to 26
inline result_t cc2420controlm_CC2420Control_TuneChannel(uint8_t channel)
{
    uint16_t freq;

    freq = 2405 + 5 * (channel - 11);
    cc2420controlm_CC2420Control_TuneManual(freq);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CC2420Control_TunePower(uint8_t powerlevel)
{
    uint8_t status;

    //  cc2420controlm_gCurrentParameters[CP_FSCTRL] = (cc2420controlm_gCurrentParameters[CP_FSCTRL] & 0xfc00) | (fsctrl << 0);
    cc2420controlm_gCurrentParameters[CP_TXCTRL] =
        (cc2420controlm_gCurrentParameters[CP_TXCTRL] & 0xffe0) | (powerlevel &
                                                                   0x1f);
    status =
        cc2420controlm_HPLChipcon_write(0x15,
                                        cc2420controlm_gCurrentParameters
                                        [CP_FSCTRL]);
    //IF THE oscillator is running, turn on the rx mode 
    // STATUS bit 06 means that the oscillator is running or not 
    if (status & (1 << 6))
    {
        cc2420controlm_HPLChipcon_cmd(0x03);
    }
    return SUCCESS;
}



//-------------------------------------------------------------------------
inline result_t cc2420controlm_HPLChipconRAM_writeDone(uint16_t addr, uint8_t
                                                       length,
                                                       uint8_t * buffer)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_HPLChipconRAM_write(uint16_t arg_0xa45ad38,
                                                   uint8_t arg_0xa45ae80,
                                                   uint8_t * arg_0xa45afe0)
{
    unsigned char result;

    result = HPLCC2420M_HPLCC2420RAM_write(arg_0xa45ad38, arg_0xa45ae80,
                                           arg_0xa45afe0);
    return result;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CC2420Control_setShortAddress(uint16_t addr)
{
    addr = toLSB16(addr);
    return cc2420controlm_HPLChipconRAM_write(0x16A, 2, (uint8_t *) & addr);
}

//-------------------------------------------------------------------------
inline uint16_t cc2420controlm_HPLChipcon_read(uint8_t arg_0xa4103b0)
{
    unsigned int result;

    result = HPLCC2420M_HPLCC2420_read(arg_0xa4103b0);
    return result;
}

//-------------------------------------------------------------------------
inline bool cc2420controlm_SetRegs(void)
{
    uint16_t data;

    cc2420controlm_HPLChipcon_write(0x10,
                                    cc2420controlm_gCurrentParameters
                                    [CP_MAIN]);
    cc2420controlm_HPLChipcon_write(0x11,
                                    cc2420controlm_gCurrentParameters
                                    [CP_MDMCTRL0]);
    data = cc2420controlm_HPLChipcon_read(0x11);
    if (data != cc2420controlm_gCurrentParameters[CP_MDMCTRL0])
    {
        return FALSE;
    }
    cc2420controlm_HPLChipcon_write(0x12,
                                    cc2420controlm_gCurrentParameters
                                    [CP_MDMCTRL1]);
    cc2420controlm_HPLChipcon_write(0x13,
                                    cc2420controlm_gCurrentParameters
                                    [CP_RSSI]);
    cc2420controlm_HPLChipcon_write(0x14,
                                    cc2420controlm_gCurrentParameters
                                    [CP_SYNCWORD]);
    cc2420controlm_HPLChipcon_write(0x15,
                                    cc2420controlm_gCurrentParameters
                                    [CP_TXCTRL]);
    cc2420controlm_HPLChipcon_write(0x16,
                                    cc2420controlm_gCurrentParameters
                                    [CP_RXCTRL0]);
    cc2420controlm_HPLChipcon_write(0x17,
                                    cc2420controlm_gCurrentParameters
                                    [CP_RXCTRL1]);
    cc2420controlm_HPLChipcon_write(0x18,
                                    cc2420controlm_gCurrentParameters
                                    [CP_FSCTRL]);
    cc2420controlm_HPLChipcon_write(0x19,
                                    cc2420controlm_gCurrentParameters
                                    [CP_SECCTRL0]);
    cc2420controlm_HPLChipcon_write(0x1A,
                                    cc2420controlm_gCurrentParameters
                                    [CP_SECCTRL1]);
    cc2420controlm_HPLChipcon_write(0x1C,
                                    cc2420controlm_gCurrentParameters
                                    [CP_IOCFG0]);
    cc2420controlm_HPLChipcon_write(0x1D,
                                    cc2420controlm_gCurrentParameters
                                    [CP_IOCFG1]);
    cc2420controlm_HPLChipcon_cmd(0x09);
    cc2420controlm_HPLChipcon_cmd(0x08);
    return TRUE;
}

//-------------------------------------------------------------------------
inline void cc2420controlm_PostOscillatorOn(void)
{
    //This fucntion sets up all the registers of the radio module 
    cc2420controlm_SetRegs();
    //This function sets up the short address of the node, and therefore, if the mac frame includes a short address, it should be matched
    cc2420controlm_CC2420Control_setShortAddress(CURRENT_NODE_ID);
    //tHIS TURNS THE CHANELL
    cc2420controlm_CC2420Control_TuneManual(((cc2420controlm_gCurrentParameters
                                              [CP_FSCTRL] << 0) & 0x1FF) +
                                            2048);
    {
        _atomic_t _atomic = _atomic_start();

        cc2420controlm_state = cc2420controlm_START_STATE_DONE;
        _atomic_end(_atomic);
    }
    cc2420controlm_SplitControl_startDone();
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CCA_fired(void)
{
    cc2420controlm_HPLChipcon_write(0x1D, 0);
    postTask(cc2420controlm_PostOscillatorOn, 5);
    return FAIL;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_HPLChipconControl_init(void)
{
    unsigned char result;

    result = HPLCC2420M_StdControl_init();
    result = rcombine(result, HPLTimer1M_StdControl_init());
    return result;
}
