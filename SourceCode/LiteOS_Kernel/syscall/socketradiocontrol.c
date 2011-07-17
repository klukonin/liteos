/** @file socketradiocontrol.c
	@brief The functional implementation for socket radio control. 

	@author Qing Cao (cao@utk.edu)
*/


#include "socketradiocontrol.h"
#include "../syscall/socketradiodata.h"
#include "../kernel/threadkernel.h"
#include "../io/radio/amradio.h"


 
radio_receiving_buffer radio_buf;

//-------------------------------------------------------------------------
void setRadioFrequencyTask()
{
    volatile uint16_t freq;
    asm volatile ("mov  %A0, r20" "\n\t" "mov  %B0, r21" "\n\t":"=r" (freq):);

    cc2420controlm_CC2420Control_TuneManual(freq);
    return;
}

//-------------------------------------------------------------------------
void setRadioChannelTask()
{
    volatile uint16_t channel;
    asm volatile ("mov  %A0, r20" "\n\t" "mov  %B0, r21" "\n\t":"=r" (channel):);

    AMStandard_TuneChannel(channel);
    return;
}

//-------------------------------------------------------------------------
void setRadioPowerTask()
{
    volatile uint16_t powerlevel;
    asm volatile ("mov  %A0, r20" "\n\t" "mov  %B0, r21"
                  "\n\t":"=r" (powerlevel):);
    AMStandard_TunePower(powerlevel);
    return;
}

void SocketRadioSend()
{
   	//debugrf230
    //Leds_redToggle(); 

    postTask(send_task, 9);
    //After it is woken up, return

	sleepThread(20);
    restoreRadioState();
    return;
}


//void getRadioInfo()  __attribute__((naked));
void *getRadioSendingBufferAddr()
{
    return (void *)&radiosocketdata;
    //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
    // asm volatile("ret"::); 
}

 
 
 //void getRadioInfo()  __attribute__((naked));
void *getRadioReceivingBufferAddr()
{
    return (void *)&radio_buf;
    //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
    // asm volatile("ret"::); 
}
 
 
 
//system call interface for registering an event 
void registerReceiverHandle_syscall()
{
    registerEvent(radio_buf.port, radio_buf.maxLength, radio_buf.dataReady,
                  radio_buf.data, radio_buf.packetinfo,
                  radio_buf.handlefunc);
}
