/**
   @file hpltimer1.h
   @brief hpl timer implementation. 
   
*  @author Su Ping
* @author (converted to nesC by Sam Madden)
* @author David Gay
* @author Intel Research Berkeley Lab
* @author Phil Levis
*/

#ifndef HPLTIMERH
#define HPLTIMERH
#include "../../types/types.h"
#include "../radio/amcommon.h"
/** \addtogroup timer */

/** @{ */
enum __nesc_unnamed4266
{
    TCLK_CPU_OFF = 0, TCLK_CPU_DIV1 = 1, TCLK_CPU_DIV8 = 2, TCLK_CPU_DIV64 = 3,
    TCLK_CPU_DIV256 = 4, TCLK_CPU_DIV1024 = 5
};
enum __nesc_unnamed4267
{
    TIMER1_DEFAULT_SCALE = TCLK_CPU_DIV64, TIMER1_DEFAULT_INTERVAL = 255
};



inline result_t HPLTimer1M_StdControl_init(void);
inline result_t HPLTimer1M_StdControl_start(void);
inline result_t HPLTimer1M_StdControl_stop(void);
inline result_t HPLTimer1M_Timer1_setRate(uint16_t interval, char scale);
inline void HPLTimer1M_CaptureT1_enableEvents(void);
inline void HPLTimer1M_CaptureT1_clearOverflow(void);
inline void HPLTimer1M_CaptureT1_disableEvents(void);
inline result_t HPLTimer1M_Timer1_default_fire(void);
inline result_t HPLTimer1M_Timer1_fire(void);
inline uint16_t HPLTimer1M_CaptureT1_getEvent(void);
inline void HPLTimer1M_CaptureT1_captured(uint16_t arg_0xa4d7ac0);
inline bool HPLTimer1M_CaptureT1_isOverflowPending(void);
inline void HPLTimer1M_CaptureT1_setEdge(uint8_t LowToHigh);

/**@} */
#endif
