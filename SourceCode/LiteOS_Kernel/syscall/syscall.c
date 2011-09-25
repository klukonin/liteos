/** @file syscall.c
	@brief syscall definitions.
	@author Qing Charles Cao (cao@utk.edu)
*/

#include "syscall.h"
#include "../sensors/leds.h"
#include "../kernel/threadkernel.h"
#include "socketthread.h"
#include "../kernel/threadtools.h"
#include "../syscall/socketradiodata.h"
#include "socketfile.h"
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
#include "socketthread.h"
#include "socketfile.h"
#include "socketadc.h"
#include "socketeeprom.h"
#include "../utilities/eventlogger.h"
#include "socketradiocontrol.h"
#include "../kernel/scheduling.h"




 


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

#ifdef TRACE_ENABLE
void getNodeIdTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETNODEID, currentindex);
    getNodeIdTask();
}

#endif
//-------------------------------------------------------------------------
void getNodeID() __attribute__ ((section(".systemcall.1")))
    __attribute__ ((naked));
void getNodeID()
{
#ifdef TRACE_ENABLE
    getNodeIdTask_Logger();
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
#ifdef TRACE_ENABLE
void setNodeIdTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETNODEID, currentindex);
    setNodeIdTask();
}
#endif

//-------------------------------------------------------------------------
void setNodeID() __attribute__ ((section(".systemcall.1")))
    __attribute__ ((naked));
void setNodeID()
{
#ifdef TRACE_ENABLE
    setNodeIdTask_Logger();
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
#ifdef TRACE_ENABLE
void getRandomTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETRANDOMNUMBER, currentindex);
    getRandomTask();
}
#endif

//-------------------------------------------------------------------------

/**\ingroup syscall 
Get a random number from the kernel.
*/
void getRandomNumberSyscall() __attribute__ ((section(".systemcall.1")))
    __attribute__ ((naked));
void getRandomNumberSyscall()
{
#ifdef TRACE_ENABLE
    getRandomTask_Logger();
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
#ifdef TRACE_ENABLE
void getCurrentThreadAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTTHREADADDRESS, currentindex);
    getCurrentThreadAddress_avr();
}
#endif


void getCurrentThreadAddressSyscall() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void getCurrentThreadAddressSyscall()
{
#ifdef TRACE_ENABLE
    getCurrentThreadAddress_Logger();
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
#ifdef TRACE_ENABLE
void getThreadIndexAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTTHREADINDEX, currentindex);
    getThreadIndexAddress_avr();
}
#endif 

/**\ingroup syscall
Get the index number of the current thread in the thread table. 
This function is going to put the address into two registers, R24 and R25.
*/
void getCurrentThreadIndex() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void getCurrentThreadIndex()
{
#ifdef TRACE_ENABLE
    getThreadIndexAddress_Logger();
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
#ifdef TRACE_ENABLE
void getThreadTableStart_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();
    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETTHREADCONTROLBLOCK, currentindex);
    getThreadTableStart_avr();
}
#endif 

/**\ingroup syscall 
*/
void getThreadTableStartSyscall() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void getThreadTableStartSyscall()
{
#ifdef TRACE_ENABLE
    getThreadTableStart_Logger();
#else
    getThreadTableStart_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef TRACE_ENABLE
void thread_yield_logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YIELDFUNCTION, currentindex);
    thread_yield();
}
#endif

/**\ingroup syscall 
Yield current thread.
*/
void yieldfunction() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void yieldfunction()
{
#ifdef TRACE_ENABLE
    thread_yield_logger();
#else
    thread_yield();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}


//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void postNewThreadTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_POSTTHREADTASK, currentindex);
    postNewThreadTask();
}
#endif 

/**\ingroup syscall 
Trigger the thread scheduling task. 
*/
void postThreadTask() __attribute__ ((section(".systemcall.2")))
    __attribute__ ((naked));
void postThreadTask()
{
#ifdef TRACE_ENABLE
    postNewThreadTask_Logger();
#else
    postNewThreadTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



/*\ingroup syscall 
Trigger the thread scheduling task. 

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

  #ifdef TRACE_ENABLE
  #ifdef TRACE_ENABLE_SYSCALL_BREAKPOINTYIELD
   break_point_function_Logger();
  #endif
  #else
   break_point_function();   
  #endif
   asm volatile( "nop":: );
   asm volatile( "ret":: );
}


*/
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
#ifdef TRACE_ENABLE
void postNewTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_POSTTASKSYSCALL, currentindex);
    postNewTask_avr();
}
#endif 

/**\ingroup syscall 
Posttask here for backward compatibility
Bug to be fixed here. 
*/
void postTaskSysCall() __attribute__ ((section(".systemcall.3")))
    __attribute__ ((naked));
void postTaskSysCall()
{
#ifdef TRACE_ENABLE
    postNewTask_Logger();
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

#ifdef TRACE_ENABLE
void getStackPtrTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_KERNELSTACK, currentindex);
    getStackPtrTask();
}
#endif 


//Get the address of the kernel stack 
void getStackPtr() __attribute__ ((section(".systemcall.4")))
    __attribute__ ((naked));
void getStackPtr()
{
  #ifdef TRACE_ENABLE
      getStackPtrTask_Logger();
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
#ifdef TRACE_ENABLE
void setRadioFrequencyTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETRADIOFREQUENCY, currentindex);
    setRadioFrequencyTask();
}
#endif 

/**\ingroup syscall 
Set the radio frequency, stored in the registers. 
*/
//inline result_t cc2420controlm_CC2420Control_TuneManual(uint16_t DesiredFreq);
void setRadioFrequency() __attribute__ ((section(".systemcall.5"))) __attribute__
    ((naked));
void setRadioFrequency()
{
#ifdef TRACE_ENABLE
    setRadioFrequencyTask_Logger();
#else
    setRadioFrequencyTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void setRadioChannelTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETRADIOCHANNEL, currentindex);
    setRadioChannelTask();
}
#endif
/**\ingroup syscall 
Set the channel, stored in the registers. 
*/
//inline result_t cc2420controlm_CC2420Control_TuneChannel(uint8_t channel); 
void setRadioChannel() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void setRadioChannel()
{
#ifdef TRACE_ENABLE
    setRadioChannelTask_Logger();
#else
    setRadioChannelTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void setRadioPowerTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETRADIOPOWER, currentindex);
    setRadioPowerTask();
 
}
#endif 

/**\ingroup syscall 
Set the radio power, stored in the registers. 
*/
//inline result_t cc2420controlm_CC2420Control_TunePower(uint8_t powerlevel);
void setRadioPower() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void setRadioPower()
{
#ifdef TRACE_ENABLE
    setRadioPowerTask_Logger();
#else
    setRadioPowerTask();
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
#ifdef TRACE_ENABLE
void getRadioSendingBuffer_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETRADIOSENDBUFFER, currentindex);
    getRadioSendingBuffer_avr();
}
#endif 

/**\ingroup syscall 
Get the Radio info address for populate it to send the radio packet info to the kernel. 
*/
void getRadioSendingBuffer() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void getRadioSendingBuffer()
{
#ifdef TRACE_ENABLE
    getRadioSendingBuffer_Logger();
	//getRadioSendingBuffer_avr();
#else
    getRadioSendingBuffer_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void SocketRadioSend_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_INVOKERADIOSENDFUNCTION, currentindex);
    SocketRadioSend();
}
#endif 

/**\ingroup syscall 
Call the radio send function indirectly. 
*/
void invokeSocketRadioSend() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void invokeSocketRadioSend()
{
#ifdef TRACE_ENABLE
    SocketRadioSend_Logger();
	//SocketRadioSend();
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
#ifdef TRACE_ENABLE
void getReceiverBuffer_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETRADIORECEIVEBUFFER, currentindex);
    getReceiverBuffer_avr();
}
#endif 

/**\ingroup syscall
Get the radio handle for registering a receiving handle. 
*/
 

void getRadioReceivingBuffer() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void getRadioReceivingBuffer()

{
#ifdef TRACE_ENABLE
    getReceiverBuffer_Logger();
#else
    getReceiverBuffer_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void syscall_registerEvent_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_REGISTERRECEIVERHANDLE, currentindex);
    registerReceiverHandle_syscall();
}
#endif 

/**\ingroup syscall 
Register a receiving handle for incoming packet. 
*/

void setCurrentRadioHandle() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void setCurrentRadioHandle()
{
#ifdef TRACE_ENABLE
    syscall_registerEvent_Logger();
#else 
    registerReceiverHandle_syscall();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



void getStandardAMRadioLock()
{
    
    uint16_t lock; 

    lock = AMStandard_getLock();
    asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (lock));
}
	

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void getCurrentRadioLock_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETRADIOLOCK, currentindex);
    getStandardAMRadioLock();
}
#endif 

/**\ingroup syscall
Get the current radio lock. 
*/
 

void getCurrentRadioLock() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void getCurrentRadioLock()

{
#ifdef TRACE_ENABLE
    getCurrentRadioLock_Logger();
#else
    getStandardAMRadioLock();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



void setStandardAMRadioLock()
{
    
    AMStandard_setLock();
}
	

 
//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void setCurrentRadioLock_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SETRADIOLOCK, currentindex);
    setStandardAMRadioLock();
}
#endif 

/**\ingroup syscall
Set the current radio lock. 
*/
 

void setCurrentRadioLock() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void setCurrentRadioLock()

{
#ifdef TRACE_ENABLE
    setCurrentRadioLock_Logger();
#else
    setStandardAMRadioLock();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}


void releaseStandardAMRadioLock()
{
    
    AMStandard_releaseLock();
}


 

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void releaseCurrentRadioLock_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_RELEASERADIOLOCK, currentindex);
    releaseStandardAMRadioLock();
}
#endif 

/**\ingroup syscall
Set the current radio lock. 
*/
 

void releaseCurrentRadioLock() __attribute__ ((section(".systemcall.5")))
    __attribute__ ((naked));
void releaseCurrentRadioLock()

{
#ifdef TRACE_ENABLE
    releaseCurrentRadioLock_Logger();
#else
    releaseStandardAMRadioLock();
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
#ifdef TRACE_ENABLE
void Leds_redToggle_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_REDTOGGLEFUNCTION, currentindex);
    Leds_redToggle();
}

#endif 

/**\ingroup syscall 
Toggle the red LED. 
*/
void redtogglefunction() __attribute__ ((section(".systemcall.6"))) __attribute__
    ((naked));
void redtogglefunction()
{
#ifdef TRACE_ENABLE
    Leds_redToggle_Logger();
#else
    Leds_redToggle();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_yellowToggle_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YELLOWTOGGLEFUNCTION, currentindex);
    Leds_yellowToggle();
}
#endif 

/**\ingroup syscall 
Toggle the yellow LED. 
*/
void yellowtogglefunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void yellowtogglefunction()
{
#ifdef TRACE_ENABLE
    Leds_yellowToggle_Logger();
#else
    Leds_yellowToggle();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_greenToggle_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GREENTOGGLEFUNCTION, currentindex);
    Leds_greenToggle();
}
#endif 

/**\ingroup syscall 
Toggle the green LED.
*/
void greentogglefunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void greentogglefunction()
{
#ifdef TRACE_ENABLE
    Leds_greenToggle_Logger();
#else
    Leds_greenToggle();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_redOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_REDONFUNCTION, currentindex);
    Leds_redOn();
}
#endif 

/**\ingroup syscall 
Turn the red LED on. 
*/
void redonfunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void redonfunction()
{
#ifdef TRACE_ENABLE
    Leds_redOn_Logger();
#else
    Leds_redOn();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_redOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_REDOFFFUNCTION, currentindex);
    Leds_redOff();
}
#endif 

/**\ingroup syscall 
Turn the red LED off. 
*/
void redofffunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void redofffunction()
{
#ifdef TRACE_ENABLE
    Leds_redOff_Logger();
#else
    Leds_redOff();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_yellowOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YELLOWONFUNCTION, currentindex);
    Leds_yellowOn();
}
#endif 

/**\ingroup syscall 
Turn the yellow LED on.
*/
void yellowonfunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void yellowonfunction()
{
#ifdef TRACE_ENABLE
    Leds_yellowOn_Logger();
#else
    Leds_yellowOn();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_yellowOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_YELLOWOFFFUNCTION, currentindex);
    Leds_yellowOff();
}
#endif 

/**\ingroup syscall 
Turn the yellow LED off. 
*/
void yellowofffunction() __attribute__ ((section(".systemcall.6"))) __attribute__
    ((naked));
void yellowofffunction()
{
#ifdef TRACE_ENABLE
    Leds_yellowOff_Logger();
#else
    Leds_yellowOff();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_greenOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GREENONFUNCTION, currentindex);
    Leds_greenOn();
}
#endif 

/**\ingroup syscall 
Turn the green LED on. 
*/
void greenonfunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void greenonfunction()
{
#ifdef TRACE_ENABLE
    Leds_greenOn_Logger();
#else
    Leds_greenOn();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void Leds_greenOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GREENOFFFUNCTION, currentindex);
    Leds_greenOff();
}
#endif 

/**\ingroup syscall 
Turn the green LED off. 
*/
void greenofffunction() __attribute__ ((section(".systemcall.6")))
    __attribute__ ((naked));
void greenofffunction()
{
#ifdef TRACE_ENABLE
    Leds_greenOff_Logger();
#else
    Leds_greenOff();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void ADCLight_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCLIGHTSYSCALL, currentindex);
    ADCLight();
}
#endif 

/**\ingroup syscall 
Get the reading from light sensor and store the result in the thread table. 
*/
void ADCLightSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCLightSysCall()
{
#ifdef TRACE_ENABLE
    ADCLight_Logger();
#else
    ADCLight();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void ADCTemp_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCTEMPSYSCALL, currentindex);
    ADCTemp();
}
#endif 

/**\ingroup syscall 
Get the reading from temperature and store the result in the thread table. 
*/
void ADCTempSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCTempSysCall()
{
#ifdef TRACE_ENABLE
    ADCTemp_Logger();
#else
    ADCTemp();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void ADCMagX_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCMAGXSYSCALL, currentindex);
    ADCMagX();
}
#endif 

/**\ingroup syscall 
Get the reading from the X axis for the magnetic sensor and store the result in the thread table.  
*/
void ADCMagXSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCMagXSysCall()
{
#ifdef TRACE_ENABLE
    ADCMagX_Logger();
#else
    ADCMagX();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void ADCMagY_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCMAGYSYSCALL, currentindex);
    ADCMagY();
}
#endif 

/**\ingroup syscall 
Get the reading from the Y axis for the magnetic sensor and store the result in the thread table. 
*/
void ADCMagYSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCMagYSysCall()
{
#ifdef TRACE_ENABLE
    ADCMagY_Logger();
#else
    ADCMagY();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void ADCAccX_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCACCXSYSCALL, currentindex);
    ADCAccX();
}
#endif 

/**\ingroup syscall 
Get the reading from the X axis for the accelerator and store the result in the thread table. 
*/
void ADCAccXSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCAccXSysCall()
{
#ifdef TRACE_ENABLE
    ADCAccX_Logger();
#else
    ADCAccX();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------

#ifdef TRACE_ENABLE
void ADCAccY_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_ADCACCYSYSCALL, currentindex);
    ADCAccY();
}
#endif 

/**\ingroup syscall 
Get the reading from the Y axis for the accelerator and store the result in the thread table. 
*/
void ADCAccYSysCall() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void ADCAccYSysCall()
{
#ifdef TRACE_ENABLE
    ADCAccY_Logger();
#else
    ADCAccY();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}


/* added by Qi Mi (qm8e@virginia.edu) */
#ifdef TRACE_ENABLE
void sounderOn_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SOUNDERONFUNCTION, currentindex);
    sounderOn();
}
#endif 

/**\ingroup syscall 
Turn the sounder on. 
*/
void sounderonfunction() __attribute__ ((section(".systemcall.7"))) __attribute__
    ((naked));
void sounderonfunction()
{
#ifdef TRACE_ENABLE
    sounderOn_Logger();         //defined in avr\syscall.c
#else
    //sounder_init();
    sounderOn();          //defined in micaz\sounder.c
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void sounderOff_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SOUNDEROFFFUNCTION, currentindex);
    sounderOff();
}
#endif

/**\ingroup syscall 
Turn the sounder off. 
*/
void sounderofffunction() __attribute__ ((section(".systemcall.7")))
    __attribute__ ((naked));
void sounderofffunction()
{
#ifdef TRACE_ENABLE
    sounderOff_Logger();        //defined in avr\syscall.c
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
#ifdef TRACE_ENABLE
void getEEPROMHandleInfo_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCURRENTEEPROMHANDLEADDRESS, currentindex);
    getEEPROMHandleInfo();
}
#endif 

/**\ingroup syscall 
Get the EEPROM handle address for read/write. 
*/
void getCurrentEEPROMHandleAddress() __attribute__ ((section(".systemcall.8")))
    __attribute__ ((naked));
void getCurrentEEPROMHandleAddress()
{
#ifdef TRACE_ENABLE
    getEEPROMHandleInfo_Logger();
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
#ifdef TRACE_ENABLE
void readEEPROMTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_READFROMEEPROM, currentindex);
    readEEPROMTask();
}
#endif 

/**\ingroup syscall 
Read from EEPROM. 
*/
void readFromEEPROM() __attribute__ ((section(".systemcall.8")))
    __attribute__ ((naked));
void readFromEEPROM()
{
#ifdef TRACE_ENABLE
    readEEPROMTask_Logger();
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
#ifdef TRACE_ENABLE
void writeEEPROMTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_WRITETOEEPROM, currentindex);
    writeEEPROMTask();
}
#endif 

/**\ingroup syscall 
Write to EEPROM. 
*/
void writeToEEPROM() __attribute__ ((section(".systemcall.8")))
    __attribute__ ((naked));
void writeToEEPROM()
{
#ifdef TRACE_ENABLE
    writeEEPROMTask_Logger();
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
#ifdef TRACE_ENABLE
void getFilePathAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETFILEPATHADDR, currentindex);
    getFilePathAddress_avr();
}
#endif 

/**\ingroup syscall 
Get the file path locator, such as /abc/efg, etc. 
*/
void getFilePathAddr() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void getFilePathAddr()
{
#ifdef TRACE_ENABLE
    getFilePathAddress_Logger();
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
#ifdef TRACE_ENABLE
void getFileModeAddress_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETFILEMODEADDR, currentindex);
    getFileModeAddress_avr();
}
#endif 

/**\ingroup syscall 
Get the file mode for read or write. 
*/
void getFileModeAddr() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void getFileModeAddr()
{
#ifdef TRACE_ENABLE
    getFileModeAddress_Logger();
#else
    getFileModeAddress_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void openFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_OPENFILESYSCALL, currentindex);
    openFileTask();
}
#endif 

/**\ingroup syscall 
Open a file. The file handle is stored in the current thread table as well as internally by the kernel. 
*/
void openFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void openFileSysCall()
{
#ifdef TRACE_ENABLE
    openFileTask_Logger();
#else
    openFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void closeFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_CLOSEFILESYSCALL, currentindex);
    closeFileTask();
}
#endif 

/**\ingroup syscall 
Close a file. 
*/
void closeFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void closeFileSysCall()
{
#ifdef TRACE_ENABLE
    closeFileTask_Logger();
#else
    closeFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void readFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_READFILESYSCALL, currentindex);
    readFileTask();
}
#endif

/**\ingroup syscall 
Read from a file. 
*/
void readFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void readFileSysCall()
{
#ifdef TRACE_ENABLE
    readFileTask_Logger();
#else
    readFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void writeFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_WRITEFILESYSCALL, currentindex);
    writeFileTask();
}
#endif 

/**\ingroup syscall 
Write to a file. 
*/
void writeFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void writeFileSysCall()
{
#ifdef TRACE_ENABLE
    writeFileTask_Logger();
#else
    writeFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void seekFileTask_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();

    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_SEEKFILESYSCALL, currentindex);
    seekFileTask();
}
#endif 

/**\ingroup syscall 
Change file opener handle address. 
*/
void seekFileSysCall() __attribute__ ((section(".systemcall.9")))
    __attribute__ ((naked));
void seekFileSysCall()
{
#ifdef TRACE_ENABLE
    seekFileTask_Logger();
#else
    seekFileTask();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}


//Defintition group 10

//-------------------------------------------------------------------------
void getCPUCounterSyscall_avr()
{
    uint32_t counter;
	 

    counter = getCurrentResolution();
	
    asm volatile ("mov r22, %A0" "\n\t" "mov r23, %B0" "\n\t" "mov r24, %C0" "\n\t" "mov r25, %D0" "\n\t" ::"r" (counter));
}


 
//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void getCPUCounterSyscall_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();
    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCPUCOUNTSYSCALL, currentindex);
    getCPUCounterSyscall_avr();
}
#endif 

/**\ingroup syscall 
*/
void getCPUCounterSyscall() __attribute__ ((section(".systemcall.10")))
    __attribute__ ((naked));
void getCPUCounterSyscall()
{
#ifdef TRACE_ENABLE
    getCPUCounterSyscall_Logger();
#else
    getCPUCounterSyscall_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}



//Defintition group 11

//-------------------------------------------------------------------------

#ifdef PLATFORM_CPU_MEASURE


//-------------------------------------------------------------------------
void getCPUUtilizationSyscall_avr()
{
    uint16_t rate; 
	rate = getUtilization();
	asm volatile ("mov r20, %A0" "\n\t" "mov r21, %B0" "\n\t"::"r" (rate));
}


 
//-------------------------------------------------------------------------
#ifdef TRACE_ENABLE
void getCPUUtilizationSyscall_Logger()
{
    uint8_t currentindex;
    _atomic_t _atomic = _atomic_start();
    currentindex = getThreadIndexAddress();
    _atomic_end(_atomic);
    addTrace(TRACE_SYSCALL_GETCPUUTILIZATIONSYSCALL, currentindex);
    getCPUUtilizationSyscall_avr();
}
#endif 

/**\ingroup syscall 
*/
void getCPUUtilizationSyscall() __attribute__ ((section(".systemcall.11")))
    __attribute__ ((naked));
void getCPUUtilizationSyscall()
{
#ifdef TRACE_ENABLE
    getCPUUtilizationSyscall_Logger();
#else
    getCPUUtilizationSyscall_avr();
#endif
    asm volatile ("nop"::);
    asm volatile ("ret"::);
}

#endif 