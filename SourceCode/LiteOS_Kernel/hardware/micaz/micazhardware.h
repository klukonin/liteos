/** @file micazhardware.h
       @brief The functional prototypes of the micaz hardware definitions shared by MicaZ and IRIS. 

       @author Qing Charles Cao (cao@utk.edu)
*/




#ifndef LITE_HARDWARE_H
#define LITE_HARDWARE_H

#include "../avrhardware.h"

#include "../../types/types.h"


/** @addtogroup avrhardware */

/** @{ */
 
// ChipCon control assignments
#define LITE_CC_FIFOP_INT SIG_INTERRUPT6

// CC2420 Interrupt definition
#define CC2420_FIFOP_INT_ENABLE()  sbi(EIMSK , INT6)
#define CC2420_FIFOP_INT_DISABLE() cbi(EIMSK , INT6)
#define CC2420_FIFOP_INT_CLEAR() sbi(EIFR, INTF6)

void inline CC2420_FIFOP_INT_MODE(bool LowToHigh);

LITE_ASSIGN_PIN_H(CC_RSTN, A, 6);       // chipcon reset
LITE_ASSIGN_PIN_H(CC_VREN, A, 5);       // chipcon power enable
//LITE_ASSIGN_PIN_H(CC_FIFOP1, D, 7);  // fifo interrupt
LITE_ASSIGN_PIN_H(CC_FIFOP, E, 6);      // fifo interrupt
LITE_ASSIGN_PIN_H(CC_FIFOP1, E, 6);     // fifo interrupt
LITE_ASSIGN_PIN_H(CC_CCA, D, 6);        // 
LITE_ASSIGN_PIN_H(CC_SFD, D, 4);        // chipcon packet arrival
LITE_ASSIGN_PIN_H(CC_CS, B, 0); // chipcon enable
LITE_ASSIGN_PIN_H(CC_FIFO, B, 7);       // chipcon fifo
LITE_ASSIGN_PIN_H(RADIO_CCA, D, 6);     // 
void setCC2420Pins();

/** @} */
#endif //LITE_HARDWARE_H
