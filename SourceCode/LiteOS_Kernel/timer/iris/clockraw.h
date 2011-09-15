#ifndef CLOCKH
#define CLOCKH


#include "../../hardware/avrhardware.h"
#include "../../types/types.h"

/*\defgroup clock Clock operations 
   This module defines the data structures and operations to control the hardware timers to implement clocks 
 */
/*@{ */
uint8_t HPLClock_set_flag;
uint8_t HPLClock_mscale;
uint8_t HPLClock_nextScale;
uint8_t HPLClock_minterval;

/** This function reads the counter of the clock */
inline uint8_t HPLClock_Clock_readCounter(void);
inline uint8_t HPLClock_Clock_getInterval(void);
inline result_t HPLClock_Clock_fire(void);
inline void HPLClock_Clock_setInterval(uint8_t value);
inline result_t HPLClock_Clock_setRate(char interval, char scale);
void HPLClock_Timer3_Start();
void HPLClock_Timer3_Stop();
uint16_t HPLClock_readTimeCounterHigh();
uint32_t HPLClock_readTimeCounterLow();

/*@} */
#endif
