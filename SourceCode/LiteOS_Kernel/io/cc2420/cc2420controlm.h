/** @file cc2420controlm.h
       @brief The declarations of the control module for cc2420. 

	This file declares the cc2420 control module. 
	@author Joe Polastre
*     @author Alan Broad
	@author Qing Charles Cao 
*/

#ifndef CC2420CONTROLMH
#define CC2420CONTROLMH
#include "../../types/types.h"

/** \addtogroup radiocc2420 */
/** @{ */

enum 
{
    cc2420controlm_IDLE_STATE = 0, cc2420controlm_INIT_STATE,
    cc2420controlm_INIT_STATE_DONE, cc2420controlm_START_STATE,
    cc2420controlm_START_STATE_DONE, cc2420controlm_STOP_STATE
};



inline result_t cc2420controlm_SplitControl_init(void);
inline result_t cc2420controlm_CCA_startWait(bool arg_0xa422588);
inline result_t cc2420controlm_CC2420Control_VREFOn(void);
inline result_t cc2420controlm_SplitControl_initDone(void);
inline void cc2420controlm_taskInitDone(void);
inline uint8_t cc2420controlm_HPLChipcon_cmd(uint8_t arg_0xa403928);
inline uint8_t cc2420controlm_HPLChipcon_write(uint8_t arg_0xa403d80, uint16_t
                                               arg_0xa403ed0);
inline result_t cc2420controlm_CC2420Control_OscillatorOn(void);
inline result_t cc2420controlm_HPLChipconControl_start(void);
inline result_t cc2420controlm_SplitControl_start(void);
inline result_t cc2420controlm_CC2420Control_RxMode(void);
inline result_t cc2420controlm_SplitControl_startDone(void);
inline result_t cc2420controlm_CC2420Control_TuneManual(uint16_t DesiredFreq);
inline result_t cc2420controlm_CC2420Control_TuneChannel(uint8_t channel);
inline result_t cc2420controlm_CC2420Control_TunePower(uint8_t powerlevel);
inline result_t cc2420controlm_HPLChipconRAM_write(uint16_t arg_0xa45ad38,
                                                   uint8_t arg_0xa45ae80,
                                                   uint8_t * arg_0xa45afe0);
inline result_t cc2420controlm_CC2420Control_setShortAddress(uint16_t addr);
inline uint16_t cc2420controlm_HPLChipcon_read(uint8_t arg_0xa4103b0);
inline bool cc2420controlm_SetRegs(void);
inline void cc2420controlm_PostOscillatorOn(void);
inline result_t cc2420controlm_CCA_fired(void);
inline result_t cc2420controlm_HPLChipconRAM_writeDone(uint16_t addr, uint8_t
                                                       length,
                                                       uint8_t * buffer);
inline result_t cc2420controlm_HPLChipconControl_init(void);
inline result_t cc2420controlm_CC2420Control_VREFOff(void);


///User system call API. It relies on r20, r21 to get the freq variable. 
void setRadioFrequencyTask();

///User system call API. It relies on r20, r21 to get the freq variable. 
void setRadioChannelTask();

///User system call API. It relies on r20, r21 to get the power level variable. 
void setRadioPowerTask();

/** @} */
#endif
