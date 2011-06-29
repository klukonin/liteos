/** @file micazhardware.c
       @brief The functional implementation details of the micaz hardware definitions shared by MicaZ and IRIS. 

       @author Qing Charles Cao (cao@utk.edu)
*/


#include "micazhardware.h"

void inline CC2420_FIFOP_INT_MODE(bool LowToHigh)
{
    sbi(EICRB, ISC61);          // edge mode
    if (LowToHigh)
    {
        sbi(EICRB, ISC60);
    }                           //trigger on rising level
    else
    {
        cbi(EICRB, ISC60);
    }                           //trigger on falling level
}

 




//-------------------------------------------------------------------------
LITE_ASSIGN_PIN(CC_RSTN, A, 6); // chipcon reset
LITE_ASSIGN_PIN(CC_VREN, A, 5); // chipcon power enable
//LITE_ASSIGN_PIN(CC_FIFOP1, D, 7);  // fifo interrupt
LITE_ASSIGN_PIN(CC_FIFOP, E, 6);        // fifo interrupt
LITE_ASSIGN_PIN(CC_FIFOP1, E, 6);       // fifo interrupt
LITE_ASSIGN_PIN(CC_CCA, D, 6);  // 
LITE_ASSIGN_PIN(CC_SFD, D, 4);  // chipcon packet arrival
LITE_ASSIGN_PIN(CC_CS, B, 0);   // chipcon enable
LITE_ASSIGN_PIN(CC_FIFO, B, 7); // chipcon fifo
LITE_ASSIGN_PIN(RADIO_CCA, D, 6);       // 

 
void setCC2420Pins(){
 //CC2420 pins  
    LITE_MAKE_MISO_INPUT();
    LITE_MAKE_MOSI_OUTPUT();
    LITE_MAKE_SPI_SCK_OUTPUT();
    LITE_MAKE_CC_RSTN_OUTPUT();
    LITE_MAKE_CC_VREN_OUTPUT();
    LITE_MAKE_CC_CS_INPUT();
    LITE_MAKE_CC_FIFOP1_INPUT();
    LITE_MAKE_CC_CCA_INPUT();
    LITE_MAKE_CC_SFD_INPUT();
    LITE_MAKE_CC_FIFO_INPUT();
    LITE_MAKE_RADIO_CCA_INPUT();
   
  }
