/**
   @file hpltimer1.c
   @brief hpl timer implementation. 
   
*  @author Su Ping
* @author (converted to nesC by Sam Madden)
* @author David Gay
* @author Intel Research Berkeley Lab
* @author Phil Levis
*/


#include <avr/interrupt.h>
#include "cc2420const.h"
#include "cc2420controlm.h"
#include "cc2420radiom.h"
#include "hplcc2420fifom.h"
#include "hplcc2420interruptm.h"
#include "hplcc2420m.h"
#include "hpltimer1.h"
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
extern volatile uint16_t *stackinterrupt_ptr;
extern volatile uint16_t *old_stack_ptr;

//THis file is the organized HPLTIMER1 
//The fire is not connected becuase no one is using it
//only the capture interface is used and has a reverse link 
//To do  is to add the headers and ge tthe function calls outside 
/*The timing measurement code now commented out 
   used for global timing */

   #ifdef ENERGY_INSTRUMENTATION
   volatile uint32_t lcounter, lcounter2;
   volatile uint16_t hcounter, hcounter2; 
   extern volatile uint32_t interruptcost; 
   #endif
uint8_t HPLTimer1M_set_flag;
uint8_t HPLTimer1M_mscale;
uint8_t HPLTimer1M_nextScale;
uint16_t HPLTimer1M_minterval;
inline result_t HPLTimer1M_StdControl_init(void)
{
    {
        _atomic_t _atomic = _atomic_start();

        {
            HPLTimer1M_mscale = TCLK_CPU_DIV256;
            HPLTimer1M_minterval = TIMER1_DEFAULT_INTERVAL;
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t HPLTimer1M_StdControl_start(void)
{
    uint16_t mi;
    uint8_t ms;

    {
        _atomic_t _atomic = _atomic_start();

        {
            mi = HPLTimer1M_minterval;
            ms = HPLTimer1M_mscale;
        }
        _atomic_end(_atomic);
    }
    HPLTimer1M_Timer1_setRate(mi, ms);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t HPLTimer1M_StdControl_stop(void)
{
    uint16_t mi;

    {
        _atomic_t _atomic = _atomic_start();

        {
            mi = HPLTimer1M_minterval;
        }
        _atomic_end(_atomic);
    }
    HPLTimer1M_Timer1_setRate(mi, 0);   //default scale=0=OFF
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t HPLTimer1M_Timer1_setRate(uint16_t interval, char scale)
{
    scale &= 0x7;
    scale |= 0x8;
    {
        _atomic_t _atomic = _atomic_start();

        {
            outp(0, TCCR1A);    //stop the timer's clock
            cbi(TIMSK, OCIE1A); //disable output compare
            cbi(TIMSK, TOIE1);  //disable Overflow interrupts
            cbi(TIMSK, TICIE1); //clear input capture
            outw(TCNT1L, 0);    //clear the 16bit count 
            outw(OCR1AL, interval);     //set the compare value
            sbi(TIFR, OCF1A);   //clear Timer1A OCF flag by writing 1
            //              sbi(TIMSK, OCIE1A);       //enable OCIE1A interrupt
            outp(scale, TCCR1B);        //starts the timer with sc
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline void HPLTimer1M_CaptureT1_enableEvents(void)
{
    cbi(TCCR1B, WGM13);
    cbi(TCCR1B, WGM12);
    sbi(TIMSK, TICIE1);
}

//-------------------------------------------------------------------------
inline void HPLTimer1M_CaptureT1_clearOverflow(void)
{
    sbi(TIFR, TOV1);
    return;
}

//-------------------------------------------------------------------------
inline void HPLTimer1M_CaptureT1_disableEvents(void)
{
    cbi(TIMSK, TICIE1);         //disable
    sbi(TIFR, ICF1);            //clear any pending interrupt}
}

//-------------------------------------------------------------------------
inline result_t HPLTimer1M_Timer1_default_fire(void)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t HPLTimer1M_Timer1_fire(void)
{
    unsigned char result;

    result = HPLTimer1M_Timer1_default_fire();
    return result;
}

//-------------------------------------------------------------------------
inline uint16_t HPLTimer1M_CaptureT1_getEvent(void)
{
    uint16_t i;

    {
        _atomic_t _atomic = _atomic_start();

        i = inw(ICR1L);
        _atomic_end(_atomic);
    }
    return i;
}

//-------------------------------------------------------------------------
inline void HPLTimer1M_CaptureT1_captured(uint16_t arg_0xa4d7ac0)
{
    hplcc2420interruptm_SFDCapture_captured(arg_0xa4d7ac0);
}

//void   __vector_11(void) __attribute__((signal, used,   externally_visible)); 
//void  __attribute((signal, used, externally_visible))   __vector_11(void) 
SIGNAL(TIMER1_CAPT_vect)
{
    uint8_t isthreadtrue;
    _atomic_t _atomic;

    #ifdef ENERGY_INSTRUMENTATION
       __asm__ __volatile__ ("sei" ::);
       {
       _atomic = _atomic_start();
       lcounter = getCurrentResolution();
       hcounter = getCurrentCounterHigh();
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
    HPLTimer1M_CaptureT1_captured(HPLTimer1M_CaptureT1_getEvent());
    _atomic = _atomic_start();
    if (isthreadtrue == 1)
    {
        isthreadtrue = 0;
        SWAP_STACK_PTR(old_stack_ptr, stackinterrupt_ptr);
        // thread_yield();   
    }
    _atomic_end(_atomic);
    
    
    #ifdef ENERGY_INSTRUMENTATION      
       {
       _atomic = _atomic_start();
       lcounter2 = getCurrentResolution();
       hcounter2 = getCurrentCounterHigh();
       if (hcounter2 == hcounter)
       interruptcost += lcounter2 - lcounter; 
       else
       interruptcost += ((uint32_t)(hcounter2-hcounter))*50000*50000 + lcounter2 - lcounter; 
       _atomic_end(_atomic); 
       }
       __asm__ __volatile__ ("cli" ::);     
    #endif
}

//-------------------------------------------------------------------------
inline bool HPLTimer1M_CaptureT1_isOverflowPending(void)
{
    return (inp(TIFR) & TOV1);
}

//-------------------------------------------------------------------------
inline void HPLTimer1M_CaptureT1_setEdge(uint8_t LowToHigh)
{
    if (LowToHigh)
    {
        sbi(TCCR1B, ICES1);
    }
    //rising edge
    else
    {
        cbi(TCCR1B, ICES1);
    }
    //falling edge
    //Set InputCapture pin PortD pin4 as INPUT
    //  LITE_MAKE_CC_SFD_INPUT();
    sbi(TIFR, ICF1);            //clear any pending interrupt
    return;
}

//void  __vector_12(void)  __attribute__((signal, used,   externally_visible)); 
//void __attribute((interrupt, used, externally_visible))  __vector_12(void)
ISR(TIMER1_COMPA_vect)
{
    _atomic_t _atomic;
    uint8_t isthreadtrue;

    /*   
       __asm__ __volatile__ ("sei" ::);
       SHOWME(int12 start\n);
     */
    isthreadtrue = 0;
    _atomic = _atomic_start();
    if (is_thread())
    {
        isthreadtrue = 1;
        SWAP_STACK_PTR(stackinterrupt_ptr, old_stack_ptr);
    }
    _atomic_end(_atomic);
    {
        _atomic_t _atomic = _atomic_start();

        {
            if (HPLTimer1M_set_flag)
            {
                HPLTimer1M_mscale = HPLTimer1M_nextScale;
                HPLTimer1M_nextScale |= 0x8;
                outp(HPLTimer1M_nextScale, TCCR1B);     //update the clock scale
                outw(OCR1AL, HPLTimer1M_minterval);     //update the compare value
                HPLTimer1M_set_flag = 0;
            }
        }
        _atomic_end(_atomic);
    }
    HPLTimer1M_Timer1_fire();
    _atomic = _atomic_start();
    if (isthreadtrue == 1)
    {
        isthreadtrue = 0;
        SWAP_STACK_PTR(old_stack_ptr, stackinterrupt_ptr);
        //  thread_yield();   
    }
    _atomic_end(_atomic);
    /*              
       SHOWME(int12 end\n);
       __asm__ __volatile__ ("cli" ::);     
     */
}
