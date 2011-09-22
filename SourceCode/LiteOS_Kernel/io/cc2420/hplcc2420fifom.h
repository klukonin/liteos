/**
   @file hplcc2420fifom.h
   @brief hpl cc2420 implementation. 
   
* @author Jason Hill
* @author David Gay
* @author Philip Levis
*/

#ifndef hplcc2420fifomH
#define hplcc2420fifomH
#include "../../types/types.h"
#include "../radio/amcommon.h"

/** \addtogroup radiocc2420 */
/** @{ */
inline result_t hplcc2420fifom_HPLCC2420FIFO_TXFIFODone(uint8_t arg_0xa40cd20,
                                                        uint8_t *
                                                        arg_0xa40ce80);
inline void hplcc2420fifom_signalTXdone(void);
inline result_t hplcc2420fifom_HPLCC2420FIFO_writeTXFIFO(uint8_t len, uint8_t *
                                                         msg);
inline result_t hplcc2420fifom_HPLCC2420FIFO_RXFIFODone(uint8_t arg_0xa40c690,
                                                        uint8_t *
                                                        arg_0xa40c7f0);
inline result_t hplcc2420fifom_HPLCC2420FIFO_readRXFIFO(uint8_t len,
                                                        uint8_t * msg);
/** @} */
#endif
