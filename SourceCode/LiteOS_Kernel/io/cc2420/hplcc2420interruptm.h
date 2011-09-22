/**
   @file hplcc2420interruptm.h
   @brief hpl cc2420 interrupt implementation. 
   
* @author Jason Hill
* @author David Gay
* @author Philip Levis
*/

#ifndef hplcc2420interruptmH
#define hplcc2420interruptmH
#include "../../types/types.h"
/** \addtogroup radiocc2420 */
/** @{ */
inline result_t hplcc2420interruptm_CCA_startWait(bool low_to_high);
inline void hplcc2420interruptm_SFDCapture_enableEvents(void);
inline void hplcc2420interruptm_SFDCapture_clearOverflow(void);
inline void hplcc2420interruptm_SFDCapture_setEdge(uint8_t arg_0xa4c5e80);
inline void hplcc2420interruptm_SFDCapture_disableEvents(void);
inline result_t hplcc2420interruptm_SFD_enableCapture(bool low_to_high);
inline result_t hplcc2420interruptm_FIFOP_startWait(bool low_to_high);
inline result_t hplcc2420interruptm_CCA_fired(void);
inline result_t hplcc2420interruptm_CCATimer_fired(void);
inline result_t hplcc2420interruptm_FIFOTimer_start(char arg_0xa315138,
                                                    uint32_t arg_0xa315290);
inline result_t hplcc2420interruptm_FIFO_default_fired(void);
inline result_t hplcc2420interruptm_FIFO_fired(void);
inline result_t hplcc2420interruptm_FIFOTimer_fired(void);
inline result_t hplcc2420interruptm_FIFOP_disable(void);
inline result_t hplcc2420interruptm_FIFOP_fired(void);
inline result_t hplcc2420interruptm_SFD_disable(void);
inline result_t hplcc2420interruptm_SFD_captured(uint16_t arg_0xa41e788);
inline bool hplcc2420interruptm_SFDCapture_isOverflowPending(void);
inline void hplcc2420interruptm_SFDCapture_captured(uint16_t time);
inline result_t hplcc2420interruptm_CCATimer_start(char arg_0xa315138,
                                                   uint32_t arg_0xa315290);
/**@} */
#endif
