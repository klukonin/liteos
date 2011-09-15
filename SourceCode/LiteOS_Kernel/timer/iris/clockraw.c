#include "clockraw.h"
#include "../timerraw.h"
 
#include "../../kernel/threadkernel.h"
#include "../../hardware/avrhardware.h"


extern volatile uint16_t *old_stack_ptr;
extern volatile uint16_t *stackinterrupt_ptr;


//Added to support 16-bit timing 
volatile uint16_t highcounter;
volatile uint16_t lowcounter;
void HPLClock_Timer3_Start()
{
    //Prescaler 0
    //TCCR1B |= (1 << 0); // Timer1 (Clock)
    //Set up the tccr to be driven by the clock with cycle wise driven 
    sbi(TCCR3B, CS30);
    //Configure Timer1 to be CTC mode 
    sbi(TCCR3B, WGM32);
    //Enable interrupt 
    sbi(TIMSK3, OCIE3A);
    //Once the 50000 compared, then itnerrupt 
    //So one interrupt every 50000 cpu cycles 
    OCR3A = 50000;
    //Counters 
    lowcounter = highcounter = 0;
    //Zero timer
    TCNT3 = 0;
}

//-------------------------------------------------------------------------
void HPLClock_Timer3_Stop()
{
    //stop the timer 
    cbi(TCCR3B, CS30);
}

//-------------------------------------------------------------------------
uint16_t HPLClock_readTimeCounterHigh()
{
    return highcounter;
}

//-------------------------------------------------------------------------
inline uint32_t HPLClock_readTimeCounterLow()
{
    uint16_t temp;
    uint16_t m;
    uint32_t retval;
    uint16_t tempcounter;
    unsigned char sreg;

    sreg = SREG;
    //Disable interrupt
    asm volatile ("cli");

    temp = TCNT3;
    //check against the problem when the read of TCNT is not atomic 
    m = temp & 0xff;
    if ((m == 0xfe) || (m == 0xff))
    {
        temp = temp - 0x100;
    }
    //Solve the problem that the counter might should have increased 
    if (temp <= 1)
    {
        tempcounter = lowcounter + 1;
    }
    else
    {
        tempcounter = lowcounter;
    }
    //get the return value 
    retval = (((uint32_t) tempcounter) * 50000) + (uint32_t) temp;
    //restore sreg
    SREG = sreg;
    //return 
    return retval;
}



inline uint8_t HPLClock_Clock_readCounter(void)
{
    return TCNT2;
    //return * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x32 + 0x20);
}

//-------------------------------------------------------------------------
inline uint8_t HPLClock_Clock_getInterval(void)
{
    //IRIS
    return OCR2A;
    //return * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20);
}

//-------------------------------------------------------------------------
inline result_t HPLClock_Clock_fire(void)
{
    unsigned char result;

    result = TimerM_Clock_fire();
    return result;
}

//-------------------------------------------------------------------------
inline void HPLClock_Clock_setInterval(uint8_t value)
{
    //iris 
    OCR2A = value;
    //* (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20) = value;
}

//-------------------------------------------------------------------------
inline result_t HPLClock_Clock_setRate(char interval, char scale)
{
    scale &= 0x7;
    scale |= 0x8;
    //shoudl be 1011 here in scale 
    {
        _atomic_t _atomic = _atomic_start();

        {
            //IRIS
            cbi(TIMSK2, TOIE2);
            cbi(TIMSK2, OCIE2A);        //Disable TC0 interrupt
            sbi(ASSR, AS2);     //set Timer/Counter0 to be asynchronous
            //from the CPU clock with a second external
            //clock(32,768kHz)driving it.
            outp((2 << WGM20), TCCR2A);
            outp(scale, TCCR2B);        //prescale the timer to be clock/128 to make it
            outp(0, TCNT2);
            outp(interval, OCR2A);
            sbi(TIMSK2, OCIE2A);
            /* * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) &= ~(1 << 0);
             * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) &= ~(1 << 1);
             * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x30 + 0x20) |= 1 << 3;
             * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x33 + 0x20) = scale;
             * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x32 + 0x20) = 0;
             * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20) = interval;
             * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) |= 1 << 1;
             */
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
_INTERRUPT(SIG_OUTPUT_COMPARE2A)
{
    _atomic_t _atomic;
    uint8_t isthreadtrue;

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
            if (HPLClock_set_flag)
            {
                HPLClock_mscale = HPLClock_nextScale;
                HPLClock_nextScale |= 0x8;
                outp(HPLClock_nextScale, TCCR2B);
                outp(HPLClock_minterval, OCR2A);
                HPLClock_set_flag = 0;
            }
        }
        _atomic_end(_atomic);
    }
    HPLClock_Clock_fire();
    _atomic = _atomic_start();
    if (isthreadtrue == 1)
    {
        isthreadtrue = 0;
        SWAP_STACK_PTR(old_stack_ptr, stackinterrupt_ptr);
        //thread_yield();
    }
    _atomic_end(_atomic);
}

//_INTERRUPT(SIG_OUTPUT_COMPARE0)
//void __vector_15() __attribute__ ((interrupt, spontaneous, C))
//void __attribute((interrupt))   __vector_15(void)


ISR(TIMER3_COMPA_vect)
{
    _atomic_t _atomic;

    _atomic = _atomic_start_avr();
    lowcounter++;
    if (lowcounter == 50000)
    {
        highcounter++;
        lowcounter = 0;
    }
    _atomic_end_avr(_atomic);
}
