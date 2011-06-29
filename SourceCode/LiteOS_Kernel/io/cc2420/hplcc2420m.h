/**
   @file hplcc2420m.h
   @brief hpl cc2420  implementation. 
   
* @author Jason Hill
* @author David Gay
* @author Philip Levis
*/

#ifndef HPLCC2420MH
#define HPLCC2420MH
#include "../../types/types.h"
/** \addtogroup radio */
/** @{ */
inline result_t HPLCC2420M_StdControl_init(void);
inline result_t HPLCC2420M_HPLCC2420RAM_writeDone(uint16_t arg_0xa45b460,
                                                  uint8_t arg_0xa45b5a8,
                                                  uint8_t * arg_0xa45b708);
inline void HPLCC2420M_signalRAMWr(void);
inline result_t HPLCC2420M_HPLCC2420RAM_write(uint16_t addr, uint8_t length,
                                              uint8_t * buffer);
inline result_t HPLCC2420M_HPLCC2420_write(uint8_t addr, uint16_t data);
inline uint8_t HPLCC2420M_HPLCC2420_cmd(uint8_t addr);
inline uint16_t HPLCC2420M_HPLCC2420_read(uint8_t addr);

/** @} */
#endif
