#include "syscall.h"
#include "../sensors/leds.h"
#include "../kernel/threadkernel.h"
#include "threadsyscall.h"
#include "../kernel/threadtools.h"
#include "../io/radio/socket.h"
#include "filesocket.h"
#include "../storage/filesys/fsapi.h"
#include "../storage/filesys/stdfsa.h"
#include "../io/radio/packethandler.h"
#include "../storage/eeprom/ioeeprom.h"
#include "../io/cc2420/cc2420controlm.h"
#include "../kernel/threaddata.h"
#include "../utilities/math.h"
#include "../kernel/scheduling.h"
#include "../storage/bytestorage/bytestorage.h"
#include "../config/nodeconfig.h"
#include "../timer/generictimer.h"
#include "../timer/globaltiming.h"
#include "../sensors/sounder.h"
#include "../bootloader/bootloader.h"
#include "../io/serial/stdserial.h"
#include "threadsyscall.h"
#include "filesocket.h"
#include "adcsocket.h"
#include "../debugging/eventlogger.h"
#include "eepromsocket.h"
#include "radiocontrol.h"




/**\defgroup syscall System calls*/
 


//Boundary EA00: node
//Node operations including reading and setting node ID and name, random number generator 
//-----------------------------------------------------------------------------

//-------------------------------------------------------------------------
void getNodeIdTask()
{
    uint16_t nodeid;

    nodeid = node_readnodeid();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (nodeid));
}

//-------------------------------------------------------------------------

void getNodeIdTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETNODEID, currentindex);
    getNodeIdTask();
}

//-------------------------------------------------------------------------
void getNodeID() __attribute__ ((section(".systemcall.1")))
    __attribute__ ((naked));
void getNodeID()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETNODEID
    getNodeIdTask_Logger();
#endif
#else
    getNodeIdTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void setNodeIdTask()
{
    uint16_t nodeid;
    asm volatile ("mov %A0, r20" "\n\t" "mov %B0, r21" "\n\t":"=r" (nodeid):);

    node_writenodeid(nodeid);
}

//-------------------------------------------------------------------------
void setNodeIdTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETNODEID, currentindex);
    setNodeIdTask();
}

//-------------------------------------------------------------------------
void setNodeID() __attribute__ ((section(".systemcall.1")))
    __attribute__ ((naked));
void setNodeID()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SETNODEID
    setNodeIdTask_Logger();
#endif
#else
    setNodeIdTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
void getRandomTask()
{
    uint16_t num;

    num = getRandomNumber();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (num));
}

//-------------------------------------------------------------------------

void getRandomTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETRANDOMNUMBER, currentindex);
    getRandomTask();
}

//-------------------------------------------------------------------------

/**\ingroup syscall 
Get a random number from the kernel.
*/
void getRandomNumberSyscall() __attribute__ ((section(".systemcall.1")))
    __attribute__ ((naked));
void getRandomNumberSyscall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETRANDOMNUMBER
    getRandomTask_Logger();
#endif
#else
    getRandomTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Boundary EA80: thread
//Thread operations
//-----------------------------------------------------------------------------



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Get the address of the currentthread pointer. Its address, not content. 

//-------------------------------------------------------------------------
void getCurrentThreadAddress_avr()
{
    thread **returnthreadaddr;

    returnthreadaddr = getCurrentThreadAddress();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0"
                  "\n\t"::"r" (returnthreadaddr));
}

//-------------------------------------------------------------------------
void getCurrentThreadAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTTHREADADDRESS, currentindex);
    getCurrentThreadAddress_avr();
}



void getCurrentThreadAddressSyscall() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void getCurrentThreadAddressSyscall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETCURRENTTHREADADDRESS
    getCurrentThreadAddress_Logger();
#endif
#else
    getCurrentThreadAddress_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------
void getThreadIndexAddress_avr()
{
    int index;

    index = getThreadIndexAddress();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (index));
}

//-------------------------------------------------------------------------
void getThreadIndexAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTTHREADINDEX, currentindex);
    getThreadIndexAddress_avr();
}

/**\ingroup syscall
Get the index number of the current thread in the thread table. 
This function is going to put the address into two registers, R24 and R25.
*/
void getCurrentThreadIndex() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void getCurrentThreadIndex()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETCURRENTTHREADINDEX
    getThreadIndexAddress_Logger();
#endif
#else
    getThreadIndexAddress_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
void getThreadTableStart_avr()
{
    void *addr;

    addr = getThreadTableStart();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (addr));
}


 
//-------------------------------------------------------------------------
void getThreadTableStart_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();
    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETTHREADTABLESTART, currentindex);
    getThreadTableStart_avr();
}

/**\ingroup syscall 
*/
void getThreadTableStartSyscall() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void getThreadTableStartSyscall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETTHREADTABLESTART
    getThreadTableStart_Logger();
#endif
#else
    getThreadTableStart_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void thread_yield_logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YIELDFUNCTION, currentindex);
    thread_yield();
}

/**\ingroup syscall 
Yield current thread.
*/
void yieldfunction() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void yieldfunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_YIELDFUNCTION
    thread_yield_logger();
#endif
#else
    thread_yield();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}


//-------------------------------------------------------------------------
void postNewThreadTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_POSTTHREADTASK, currentindex);
    postNewThreadTask();
}

/**\ingroup syscall 
Trigger the thread scheduling task. 
*/
void postThreadTask() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void postThreadTask()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_POSTTHREADTASK
    postNewThreadTask_Logger();
#endif
#else
    postNewThreadTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}






 

/**\ingroup syscall 
Trigger the thread scheduling task. 
*/
void postThreadTaskNoLog() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void postThreadTaskNoLog()
{

    postNewThreadTask();

    asm volatile ("nop"::);
    asm volatile ("ret"::);
}
 




void break_point_function_Logger()
{
  uint8_t currentindex; 
 _atomic_t _atomic = _atomic_start();
 currentindex = getThreadIndexAddress();
 _atomic_end(_atomic); 
 addTrace(TRACE_SYSCALL_BREAKPOINTYIELD, currentindex);
  break_point_function(); 
}



void breakfunction() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void breakfunction(){

  #ifdef TRACE_ENABLE_SYSCALLEVENT
  #ifdef TRACE_ENABLE_SYSCALL_BREAKPOINTYIELD
   break_point_function_Logger();
  #endif
  #else
   break_point_function();   
  #endif
   asm volatile( "nop":: );
   asm volatile( "ret":: );
}


//-----------------------------------------------------------------------------
//Boundary EB00: task
//Task operations
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------
void postNewTask_avr()
{
    void (*fp) (void);
    uint16_t priority;
    asm volatile ("mov %A0, r20" "\n\t" "mov %B0, r21" "\n\t":"=r" (fp):);
    asm volatile ("mov %A0, r22" "\n\t" "mov %B0, r23"
                  "\n\t":"=r" (priority):);
    postTask(fp, priority);
}

//-------------------------------------------------------------------------
void postNewTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_POSTTASKSYSCALL, currentindex);
    postNewTask_avr();
}

/**\ingroup syscall 
Posttask here for backward compatibility
Bug to be fixed here. 
*/
void postTaskSysCall() __attribute__ ((section(".systemcall.3")))
    __attribute__ ((naked));
void postTaskSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_POSTTASKSYSCALL
    postNewTask_Logger();
#endif
#else
    postNewTask_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}




//-------------------------------------------------------------------------
void getStackPtrTask()
{
    void **returnthreadaddr;

    returnthreadaddr = getKernelStackAddress();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0"
                  "\n\t"::"r" (returnthreadaddr));
}

void getStackPtrTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_KERNELSTACK, currentindex);
    getStackPtrTask();
}


//Get the address of the kernel stack 
void getStackPtr() __attribute__ ((section(".systemcall.4")))
    __attribute__ ((naked));
void getStackPtr()
{
	#ifdef TRACE_ENABLE_SYSCALL
  #ifdef TRACE_ENABLE_SYSCALL_KERNELSTACK
    getStackPtrTask_Logger();
  #endif
  #else 
    getStackPtrTask();
  #endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}




//-----------------------------------------------------------------------------
//Boundary EC00 Radio operations 
//-----------------------------------------------------------------------------



//because this system call directly links into the radio module, therefore, we do not need to consider the problem of directly using registers. 
void setRadioFrequencyTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETRADIOFREQUENCY, currentindex);
#ifdef RADIO_CC2420
    setRadioFrequencyTask();
#endif
}

/**\ingroup syscall 
Set the radio frequency, stored in the registers. 
*/
//inline result_t cc2420controlm_CC2420Control_TuneManual(uint16_t DesiredFreq);
void setRadioFrequency() __attribute__ ((section(".systemcall.5"))) __attribute__
    ((naked));
void setRadioFrequency()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SETRADIOFREQUENCY
    setRadioFrequencyTask_Logger();
#endif
#else
#ifdef RADIO_CC2420
    setRadioFrequencyTask();
#endif
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void setRadioChannelTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETRADIOCHANNEL, currentindex);
#ifdef RADIO_CC2420
    setRadioChannelTask();
#endif
}

/**\ingroup syscall 
Set the channel, stored in the registers. 
*/
//inline result_t cc2420controlm_CC2420Control_TuneChannel(uint8_t channel); 
void setRadioChannel() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void setRadioChannel()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SETRADIOCHANNEL
    setRadioChannelTask_Logger();
#endif
#else
#ifdef RADIO_CC2420
    setRadioChannelTask();
#endif
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void setRadioPowerTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETRADIOPOWER, currentindex);
#ifdef RADIO_CC2420
    setRadioPowerTask();
#endif
}

/**\ingroup syscall 
Set the radio power, stored in the registers. 
*/
//inline result_t cc2420controlm_CC2420Control_TunePower(uint8_t powerlevel);
void setRadioPower() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void setRadioPower()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SETRADIOPOWER
    setRadioPowerTask_Logger();
#endif
#else
#ifdef RADIO_CC2420
    setRadioPowerTask();
#endif
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
void getRadioSendingBuffer_avr()
{
    void *addr;

    addr = getRadioSendingBufferAddr();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (addr));
}

//-------------------------------------------------------------------------
void getRadioSendingBuffer_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTRADIOINFOADDRESS, currentindex);
    getRadioSendingBuffer_avr();
}

/**\ingroup syscall 
Get the Radio info address for populate it to send the radio packet info to the kernel. 
*/
void getRadioSendingBuffer() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void getRadioSendingBuffer()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETCURRENTRADIOINFOADDRESS
    getRadioSendingBuffer_Logger();
#endif
#else
    getRadioSendingBuffer_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
void SocketRadioSend_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETRADIOSENDFUNCTION, currentindex);
    SocketRadioSend();
}

/**\ingroup syscall 
Call the radio send function indirectly. 
*/
void invokeSocketRadioSend() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void invokeSocketRadioSend()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETRADIOSENDFUNCTION
    SocketRadioSend_Logger();
#endif
#else
    SocketRadioSend();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//following are receiving

//-------------------------------------------------------------------------
void getReceiverBuffer_avr()
{
    void *addr;

    addr = getRadioReceivingBufferAddr();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (addr));
}

//-------------------------------------------------------------------------
void getReceiverBuffer_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTRADIORECEIVERHANDLEADDRESS, currentindex);
    getReceiverBuffer_avr();
}

/**\ingroup syscall
Get the radio handle for registering a receiving handle. 
*/
 

void getRadioReceivingBuffer() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void getRadioReceivingBuffer()

{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETCURRENTRADIORECEIVERHANDLEADDRESS
    getReceiverBuffer_Logger();
#endif
#else
    getReceiverBuffer_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void syscall_registerEvent_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETCURRENTRADIOHANDLE, currentindex);
    registerReceiverHandle_syscall();
}

/**\ingroup syscall 
Register a receiving handle for incoming packet. 
*/
void setCurrentRadioHandle() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void setCurrentRadioHandle()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SETCURRENTRADIOHANDLE
    syscall_registerEvent_Logger();
#endif
#else 
    registerReceiverHandle_syscall();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-----------------------------------------------------------------------------
//Boundary EC80 Device: LED Operations
//-----------------------------------------------------------------------------



//-------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Leds_redToggle_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_REDTOGGLEFUNCTION, currentindex);
    Leds_redToggle();
}

/**\ingroup syscall 
Toggle the red LED. 
*/
void redtogglefunction() __attribute__ ((section(".systemcall.6"))) __attribute__
    ((naked));
void redtogglefunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_REDTOGGLEFUNCTION
    Leds_redToggle_Logger();
#endif
#else
    Leds_redToggle();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void Leds_yellowToggle_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YELLOWTOGGLEFUNCTION, currentindex);
    Leds_yellowToggle();
}

/**\ingroup syscall 
Toggle the yellow LED. 
*/
void yellowtogglefunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void yellowtogglefunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_YELLOWTOGGLEFUNCTION
    Leds_yellowToggle_Logger();
#endif
#else
    Leds_yellowToggle();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
void Leds_greenToggle_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GREENTOGGLEFUNCTION, currentindex);
    Leds_greenToggle();
}

/**\ingroup syscall 
Toggle the green LED.
*/
void greentogglefunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void greentogglefunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GREENTOGGLEFUNCTION
    Leds_greenToggle_Logger();
#endif
#else
    Leds_greenToggle();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
void Leds_redOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_REDONFUNCTION, currentindex);
    Leds_redOn();
}

/**\ingroup syscall 
Turn the red LED on. 
*/
void redonfunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void redonfunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_REDONFUNCTION
    Leds_redOn_Logger();
#endif
#else
    Leds_redOn();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void Leds_redOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_REDOFFFUNCTION, currentindex);
    Leds_redOff();
}

/**\ingroup syscall 
Turn the red LED off. 
*/
void redofffunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void redofffunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_REDOFFFUNCTION
    Leds_redOff_Logger();
#endif
#else
    Leds_redOff();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void Leds_yellowOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YELLOWONFUNCTION, currentindex);
    Leds_yellowOn();
}

/**\ingroup syscall 
Turn the yellow LED on.
*/
void yellowonfunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void yellowonfunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_YELLOWONFUNCTION
    Leds_yellowOn_Logger();
#endif
#else
    Leds_yellowOn();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void Leds_yellowOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YELLOWOFFFUNCTION, currentindex);
    Leds_yellowOff();
}

/**\ingroup syscall 
Turn the yellow LED off. 
*/
void yellowofffunction() __attribute__ ((section(".systemcall.6"))) __attribute__
    ((naked));
void yellowofffunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_YELLOWOFFFUNCTION
    Leds_yellowOff_Logger();
#endif
#else
    Leds_yellowOff();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void Leds_greenOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GREENONFUNCTION, currentindex);
    Leds_greenOn();
}

/**\ingroup syscall 
Turn the green LED on. 
*/
void greenonfunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void greenonfunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GREENONFUNCTION
    Leds_greenOn_Logger();
#endif
#else
    Leds_greenOn();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void Leds_greenOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GREENOFFFUNCTION, currentindex);
    Leds_greenOff();
}

/**\ingroup syscall 
Turn the green LED off. 
*/
void greenofffunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void greenofffunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GREENOFFFUNCTION
    Leds_greenOff_Logger();
#endif
#else
    Leds_greenOff();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------
void ADCLight_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCLIGHTSYSCALL, currentindex);
    ADCLight();
}

/**\ingroup syscall 
Get the reading from light sensor and store the result in the thread table. 
*/
void ADCLightSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCLightSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_ADCLIGHTSYSCALL
    ADCLight_Logger();
#endif
#else
    ADCLight();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void ADCTemp_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCTEMPSYSCALL, currentindex);
    ADCTemp();
}

/**\ingroup syscall 
Get the reading from temperature and store the result in the thread table. 
*/
void ADCTempSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCTempSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_ADCTEMPSYSCALL
    ADCTemp_Logger();
#endif
#else
    ADCTemp();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void ADCMagX_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCMAGXSYSCALL, currentindex);
    ADCMagX();
}

/**\ingroup syscall 
Get the reading from the X axis for the magnetic sensor and store the result in the thread table.  
*/
void ADCMagXSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCMagXSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_ADCMAGXSYSCALL
    ADCMagX_Logger();
#endif
#else
    ADCMagX();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void ADCMagY_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCMAGYSYSCALL, currentindex);
    ADCMagY();
}

/**\ingroup syscall 
Get the reading from the Y axis for the magnetic sensor and store the result in the thread table. 
*/
void ADCMagYSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCMagYSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_ADCMAGYSYSCALL
    ADCMagY_Logger();
#endif
#else
    ADCMagY();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void ADCAccX_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCACCXSYSCALL, currentindex);
    ADCAccX();
}

/**\ingroup syscall 
Get the reading from the X axis for the accelerator and store the result in the thread table. 
*/
void ADCAccXSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCAccXSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_ADCACCXSYSCALL
    ADCAccX_Logger();
#endif
#else
    ADCAccX();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void ADCAccY_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCACCYSYSCALL, currentindex);
    ADCAccY();
}

/**\ingroup syscall 
Get the reading from the Y axis for the accelerator and store the result in the thread table. 
*/
void ADCAccYSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCAccYSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_ADCACCYSYSCALL
    ADCAccY_Logger();
#endif
#else
    ADCAccY();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}


/* added by Qi Mi (qm8e@virginia.edu) */
void sounderOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SOUNDERONFUNCTION, currentindex);
    sounderOn();
}

/**\ingroup syscall 
Turn the sounder on. 
*/
void sounderonfunction() __attribute__ ((section(".systemcall.7"))) __attribute__
    ((naked));
void sounderonfunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SOUNDERONFUNCTION
    sounderOn_Logger();         //defined in avr\syscall.c
#endif
#else
    //sounder_init();
    sounderOn();          //defined in micaz\sounder.c
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void sounderOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SOUNDEROFFFUNCTION, currentindex);
    sounderOff();
}

/**\ingroup syscall 
Turn the sounder off. 
*/
void sounderofffunction() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void sounderofffunction()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SOUNDEROFFFUNCTION
    sounderOff_Logger();        //defined in avr\syscall.c
#endif
#else
    //sounder_init();
    sounderOff();         //defined in micaz\sounder.c
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}




//-----------------------------------------------------------------------------
//Boundary ED80 Device: EEPROM Operations
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------

  

//-------------------------------------------------------------------------
void getEEPROMHandleInfo()
{
    void *addr;

    addr = getGenericStorageNodeAddress();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (addr));
}

//-------------------------------------------------------------------------
void getEEPROMHandleInfo_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTEEPROMHANDLEADDRESS, currentindex);
    getEEPROMHandleInfo();
}

/**\ingroup syscall 
Get the EEPROM handle address for read/write. 
*/
void getCurrentEEPROMHandleAddress() __attribute__ ((section(".systemcall.8")))
    __attribute__ ((naked));
void getCurrentEEPROMHandleAddress()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETCURRENTEEPROMHANDLEADDRESS
    getEEPROMHandleInfo_Logger();
#endif
#else
    getEEPROMHandleInfo();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void readEEPROMTask()
{
    genericReadTask();
}

//-------------------------------------------------------------------------
void readEEPROMTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_READFROMEEPROM, currentindex);
    readEEPROMTask();
}

/**\ingroup syscall 
Read from EEPROM. 
*/
void readFromEEPROM() __attribute__ ((section(".systemcall.8")))
    __attribute__ ((naked));
void readFromEEPROM()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_READFROMEEPROM
    readEEPROMTask_Logger();
#endif
#else
    readEEPROMTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void writeEEPROMTask()
{
    genericWriteTask();
}

//-------------------------------------------------------------------------
void writeEEPROMTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_WRITETOEEPROM, currentindex);
    writeEEPROMTask();
}

/**\ingroup syscall 
Write to EEPROM. 
*/
void writeToEEPROM() __attribute__ ((section(".systemcall.8")))
    __attribute__ ((naked));
void writeToEEPROM()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_WRITETOEEPROM
    writeEEPROMTask_Logger();
#endif
#else
    writeEEPROMTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}






 

//-----------------------------------------------------------------------------
//Boundary EE00 File Operations 
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------
void getFilePathAddress_avr()
{
    void *filepathaddr;

    filepathaddr = getFilePathAddress();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0"
                  "\n\t"::"r" (filepathaddr));
}

//-------------------------------------------------------------------------
void getFilePathAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETFILEPATHADDR, currentindex);
    getFilePathAddress_avr();
}

/**\ingroup syscall 
Get the file path locator, such as /abc/efg, etc. 
*/
void getFilePathAddr() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void getFilePathAddr()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETFILEPATHADDR
    getFilePathAddress_Logger();
#endif
#else
    getFilePathAddress_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------
void getFileModeAddress_avr()
{
    void *addr;

    addr = getFileModeAddress();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (addr));
}

//-------------------------------------------------------------------------
void getFileModeAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETFILEMODEADDR, currentindex);
    getFileModeAddress_avr();
}

/**\ingroup syscall 
Get the file mode for read or write. 
*/
void getFileModeAddr() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void getFileModeAddr()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_GETFILEMODEADDR
    getFileModeAddress_Logger();
#endif
#else
    getFileModeAddress_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------
void openFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_OPENFILESYSCALL, currentindex);
    openFileTask();
}

/**\ingroup syscall 
Open a file. The file handle is stored in the current thread table as well as internally by the kernel. 
*/
void openFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void openFileSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_OPENFILESYSCALL
    openFileTask_Logger();
#endif
#else
    openFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
void closeFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_CLOSEFILESYSCALL, currentindex);
    closeFileTask();
}

/**\ingroup syscall 
Close a file. 
*/
void closeFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void closeFileSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_CLOSEFILESYSCALL
    closeFileTask_Logger();
#endif
#else
    closeFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void readFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_READFILESYSCALL, currentindex);
    readFileTask();
}

/**\ingroup syscall 
Read from a file. 
*/
void readFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void readFileSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_READFILESYSCALL
    readFileTask_Logger();
#endif
#else
    readFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void writeFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_WRITEFILESYSCALL, currentindex);
    writeFileTask();
}

/**\ingroup syscall 
Write to a file. 
*/
void writeFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void writeFileSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_WRITEFILESYSCALL
    writeFileTask_Logger();
#endif
#else
    writeFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
void seekFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SEEKFILESYSCALL, currentindex);
    seekFileTask();
}

/**\ingroup syscall 
Change file opener handle address. 
*/
void seekFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void seekFileSysCall()
{
#ifdef TRACE_ENABLE_SYSCALLEVENT
#ifdef TRACE_ENABLE_SYSCALL_SEEKFILESYSCALL
    seekFileTask_Logger();
#endif
#else
    seekFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}
