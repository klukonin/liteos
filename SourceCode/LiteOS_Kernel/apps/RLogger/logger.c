#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libeeprom.h"
#include "../../libraries/libadc.h"
#include "../../types/types.h"


 

uint8_t loggerbuffer[400];

//Pointer to the current thread data structure. Useful for waking up the current thread. 
lib_thread **thiscurrentthread;
lib_thread *currentthread;
uint16_t packetslocal;
uint16_t packetsremote;
uint16_t packetsdeliver;

LIB_MYFILE *fp;



//Find out whether there is packet pending or not
static uint8_t iswakenup;


//-------------------------------------------------------------------------
static uint8_t incomingMsg[32];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[4];

//Handles incoming data msg. This usually wakesup the current thread 
void wakeupMeOnDataMsg()
{
    //(*thiscurrentthread)->state = STATE_ACTIVE;
    currentthread->state = STATE_ACTIVE;
    iswakenup = 1;
    syscall_post_thread_task();
}

//Registers the data packet for receving incoming packets. This enables receving both remote and local data packets
void registerDataPacket()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

    //Finds the radio handle, so that when this code is ported to be externally used, it will be obtained via a system call 
    radiohandleaddr = lib_get_current_radio_receive_handle_addr();
    incomingDataLength = 0;
    //set up the radiohandleaddr data structures
    radiohandleaddr->port = 12;
    radiohandleaddr->maxLength = 32;
    radiohandleaddr->dataReady = &incomingDataLength;
    radiohandleaddr->data = incomingMsg;
    radiohandleaddr->packetinfo = incomingPacketInfo;
    radiohandleaddr->handlefunc = wakeupMeOnDataMsg;
    //close the interrupt     
    currentatomic = _atomic_start();
    iswakenup = 0;
    //call the radio handle set to store the data structure into the handle vectors 
    getaddrfp();
    //syscall_registerEvent();
    //open the interrupt 
    _atomic_end(currentatomic);
    return;
}

//This is the entry point. It broadcasts neighbor exchanges perodically, or handles incoming packets (non-update neighbor)
void logger()
{
    
     
    //register both the negibhorhood update and the message incoming 
    thiscurrentthread = lib_get_current_thread();
    currentthread = *thiscurrentthread;
    registerDataPacket();
     
    fp = lib_mfopen("/data", "w");

    while (1)
    {
        if (iswakenup == 1)
        {
            lib_mfwrite(fp, (char *)incomingMsg, 4);
            lib_mfseek(fp,4, 1);
            lib_mfwrite(fp, (char *)incomingPacketInfo, 2);
			      lib_mfseek(fp,2, 1);
            iswakenup = 0; 
			lib_green_toggle();
        }
        else
        {
            lib_hibernate_thread();
        }
        //wakes up and check the new data message?
        
        
     }   
        
     
}
