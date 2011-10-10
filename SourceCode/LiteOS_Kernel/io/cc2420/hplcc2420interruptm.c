/**
   @file hplcc2420interruptm.c
   @brief hpl cc2420 interrupt implementation. 
   
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
#include "../../timer/generictimer.h"

#include "../../kernel/threadkernel.h" //for isthread

#include "../../hardware/avrhardware.h"
#include "../../hardware/micaz/micazhardware.h"

#include "../../config/nodeconfig.h"
#include "../../types/types.h"
#include "../radio/amcommon.h"
#include "../radio/amradio.h"
#include "../radio/packethandler.h"

#include "../../types/byteorder.h"
#include "../../kernel/scheduling.h"

extern volatile uint16_t *old_stack_ptr;

extern volatile uint16_t *stackinterrupt_ptr;

uint8_t hplcc2420interruptm_FIFOWaitForState;
uint8_t hplcc2420interruptm_FIFOLastState;
uint8_t hplcc2420interruptm_CCAWaitForState;
uint8_t hplcc2420interruptm_CCALastState;

//This function is the start position of the receving pacekts
//void  __vector_7(void) __attribute__((signal, used,   externally_visible)); 
//void __attribute((signal, used, externally_visible)) __vector_7(void)

   #ifdef ENERGY_INSTRUMENTATION
   extern volatile uint32_t interruptcost; 
   volatile uint32_t alcounter;
   volatile uint16_t ahcounter; 
   volatile uint32_t alcounter2; 
   volatile uint16_t ahcounter2;
   #endif  

SIGNAL(INT6_vect)
{
    result_t val = SUCCESS;
    uint8_t isthreadtrue;
    _atomic_t _atomic;
    
   #ifdef ENERGY_INSTRUMENTATION
   __asm__ __volatile__ ("sei" ::);
   	
   {    
     _atomic = _atomic_start();
	   alcounter = getCurrentResolution();
	   ahcounter = getCurrentCounterHigh();
  	 _atomic_end(_atomic); 
   }

   #endif
   
   
   
    isthreadtrue = 0;
    _atomic = _atomic_start();
    if (is_thread())
    {
        isthreadtrue = 1;
        SWAP_STACK_PTR(stackinterrupt_ptr, old_stack_ptr);
    }
    _atomic_end(_atomic);
    val = hplcc2420interruptm_FIFOP_fired();
    if (val == FAIL)
    {
        *(volatile unsigned char *)(unsigned int)&*(volatile unsigned char *)
            (0x39 + 0x20) &= ~(1 << 6);
        *(volatile unsigned char *)(unsigned int)&*(volatile unsigned char *)
            (0x38 + 0x20) |= 1 << 6;
    }
    _atomic = _atomic_start();
    if (isthreadtrue == 1)
    {
        isthreadtrue = 0;
        SWAP_STACK_PTR(old_stack_ptr, stackinterrupt_ptr);
        //thread_yield();   
    }
    _atomic_end(_atomic);
   
   //    timing code currently commented out 
   
   #ifdef ENERGY_INSTRUMENTATION
       {
        
       _atomic = _atomic_start();
       alcounter2 = getCurrentResolution();
       ahcounter2 = getCurrentCounterHigh();
       if (ahcounter2 == ahcounter)
       interruptcost +=alcounter2 - alcounter; 
       else
       interruptcost += ((uint32_t)(ahcounter2-ahcounter))*50000*50000 + alcounter2 - alcounter; 
       _atomic_end(_atomic); 
       __asm__ __volatile__ ("cli" ::);
       }

  #endif       
     
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFOP_startWait(bool low_to_high)
{
    {
        _atomic_t _atomic = _atomic_start();

        {
            CC2420_FIFOP_INT_MODE(low_to_high);
            *(volatile unsigned char *)(unsigned int)&*(volatile unsigned
                                                        char *)(0x39 + 0x20) |=
                1 << 6;
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFOP_disable(void)
{
    *(volatile unsigned char *)(unsigned int)&*(volatile unsigned char *)(0x39
                                                                          +
                                                                          0x20)
        &= ~(1 << 6);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFOP_fired(void)
{
    unsigned char result;

    result = cc2420radiom_FIFOP_fired();
    return result;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFOTimer_start(char arg_0xa315138,
                                                    uint32_t arg_0xa315290)
{
    unsigned char result;

    result = GenericTimerStart(10, arg_0xa315138, arg_0xa315290);
    return result;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFO_default_fired(void)
{
    return FAIL;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFO_fired(void)
{
    unsigned char result;

    result = hplcc2420interruptm_FIFO_default_fired();
    return result;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFOTimer_fired(void)
{
    uint8_t FIFOState;
    result_t val = SUCCESS;

    FIFOState = LITE_READ_CC_FIFO_PIN();
    if (hplcc2420interruptm_FIFOLastState !=
        hplcc2420interruptm_FIFOWaitForState && FIFOState ==
        hplcc2420interruptm_FIFOWaitForState)
    {
        val = hplcc2420interruptm_FIFO_fired();
        if (val == FAIL)
        {
            return SUCCESS;
        }
    }
    hplcc2420interruptm_FIFOLastState = FIFOState;
    return hplcc2420interruptm_FIFOTimer_start(TIMER_ONE_SHOT, 1);
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_CCA_fired(void)
{
    unsigned char result;

    result = cc2420controlm_CCA_fired();
    return result;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_CCATimer_fired(void)
{
    uint8_t CCAState;
    result_t val = SUCCESS;

    CCAState = LITE_READ_CC_CCA_PIN();
    if (hplcc2420interruptm_CCALastState != hplcc2420interruptm_CCAWaitForState
        && CCAState == hplcc2420interruptm_CCAWaitForState)
    {
        val = hplcc2420interruptm_CCA_fired();
        if (val == FAIL)
        {
            return SUCCESS;
        }
    }
    hplcc2420interruptm_CCALastState = CCAState;
    return hplcc2420interruptm_CCATimer_start(TIMER_ONE_SHOT, 1);
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_CCA_startWait(bool low_to_high)
{
    {
        _atomic_t _atomic = _atomic_start();

        hplcc2420interruptm_CCAWaitForState = low_to_high;
        _atomic_end(_atomic);
    }
    hplcc2420interruptm_CCALastState = LITE_READ_CC_CCA_PIN();
    return hplcc2420interruptm_CCATimer_start(TIMER_ONE_SHOT, 1);
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_CCATimer_start(char arg_0xa315138,
                                                   uint32_t arg_0xa315290)
{
    unsigned char result;

    result = GenericTimerStart(11, arg_0xa315138, arg_0xa315290);
    return result;
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_enableEvents(void)
{
    HPLTimer1M_CaptureT1_enableEvents();
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_clearOverflow(void)
{
    HPLTimer1M_CaptureT1_clearOverflow();
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_setEdge(uint8_t arg_0xa4c5e80)
{
    HPLTimer1M_CaptureT1_setEdge(arg_0xa4c5e80);
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_disableEvents(void)
{
    HPLTimer1M_CaptureT1_disableEvents();
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_SFD_enableCapture(bool low_to_high)
{
    {
        _atomic_t _atomic = _atomic_start();

        {
            hplcc2420interruptm_SFDCapture_disableEvents();
            hplcc2420interruptm_SFDCapture_setEdge(low_to_high);
            hplcc2420interruptm_SFDCapture_clearOverflow();
            hplcc2420interruptm_SFDCapture_enableEvents();
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_SFD_disable(void)
{
    hplcc2420interruptm_SFDCapture_disableEvents();
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_SFD_captured(uint16_t arg_0xa41e788)
{
    unsigned char result;

    result = cc2420radiom_SFD_captured(arg_0xa41e788);
    return result;
}

//-------------------------------------------------------------------------
inline bool hplcc2420interruptm_SFDCapture_isOverflowPending(void)
{
    unsigned char result;

    result = HPLTimer1M_CaptureT1_isOverflowPending();
    return result;
}

//-------------------------------------------------------------------------
void hplcc2420interruptm_SFDCapture_captured(uint16_t time)
{
    result_t val = SUCCESS;

    val = hplcc2420interruptm_SFD_captured(time);
    if (val == FAIL)
    {
        hplcc2420interruptm_SFDCapture_disableEvents();
    }
    else
    {
        if (hplcc2420interruptm_SFDCapture_isOverflowPending())
        {
            hplcc2420interruptm_SFDCapture_clearOverflow();
        }
    }
}
