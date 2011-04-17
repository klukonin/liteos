#include "../hardware/avrhardware.h"
#include "../hardware/micaz/micazhardware.h"
#include "../types/types.h"
#include "../kernel/threadkernel.h"
#include "../kernel/threadtools.h"
#include "../kernel/scheduling.h"
#include "../io/radio/amradio.h"
#include "../storage/filesys/inode.h"
#include "../storage/filesys/fsapi.h"
#include "../bootloader/bootloader.h"
#include "../sensors/adcdriver.h"
#include "realmain.h"
#include "../config/nodeconfig.h"
#include "../storage/bytestorage/bytestorage.h"
#include "../io/serial/stdserial.h"
#include "../storage/filesys/vectorflash.h"
#include "../storage/filesys/vectornode.h"
#include "../types/string.h"
#include "../timer/generictimer.h"
#include "../timer/globaltiming.h"
#include "../io/avrserial/serialprint.h"
#include "../kernel/threadmodel.h"
#include <stdlib.h>
#include "../io/radio/packethandler.h"
#include "../storage/filesys/storageconstants.h"

#include "../shell/commandhandle.h"
#ifdef RADIO_CC2420
#include "../io/cc2420/cc2420controlm.h"
#endif 

#include "../sensors/leds.h"
#include "../storage/flash/atmelflash.h"

#include "../libraries/commonapp.h"

#define MICAZCONFIGMESSAGERECEIVED 10
#define MICAZ_INIT_READY           11
static uint16_t nodeid;




//This function receives init command in MicaZ installation task, and sets up the node status for reboot, so that the fiel system will
//be reformatted 

void initCommandReceived(uint8_t * receivebuffer)
{
    uint8_t i;

    genericwriteBytes(NETWORKNAMEOFFSET, 16, &receivebuffer[0]);
    genericwriteBytes(NODEFILENAMEOFFSET, 15, &receivebuffer[16]);
    node_setradiochannel(receivebuffer[31]);
    node_writenodeid(receivebuffer[32]);
    node_setinitstatus(MICAZCONFIGMESSAGERECEIVED);
    for (i = 0; i < 32; i++)
    {
        printfstr('\0');
    }
    avr_resetNode();
}

//The platform specific entry point. This is the place where the program gets executed. 
//Every time the system gets compiled, there should be at most one such entry point. 


int main()
{
	 
    //micaz specific initilizations
    //hardware ini
    LITE_SET_PIN_DIRECTIONS();
    
    //sensors
    adcdriver_init_adc();

    //kernels 
    initScheduling();
    thread_init();
    
    //printing
    initUSART();
    
    
 // printfstr("System booted!\n");

    //for the following, read from else section, which defaults to the 0xff as the MicaZ is first initied by reprogramming
    if (node_readinitstatus() == MICAZCONFIGMESSAGERECEIVED)
    {
        //current_node_id is expected to be platform independent variable 
        CURRENT_NODE_ID = node_readnodeid();
        srand(CURRENT_NODE_ID);
        genericreadBytes(NETWORKNAMEOFFSET, 16, networkid);
        genericreadBytes(NODEFILENAMEOFFSET, 16, filenameid);
        nodeid = CURRENT_NODE_ID;
        networkid[16] = '\0';
        filenameid[16] = '\0';
        Leds_redToggle();
        Leds_greenToggle();
        Leds_yellowToggle();
        formatSystem();
        buildRootNode();
        Leds_redToggle();
        Leds_greenToggle();
        Leds_yellowToggle();
        // node_setinitstatus(MICAZ_INIT_READY); 
        node_setinitstatus(MICAZCONFIGMESSAGERECEIVED);
    }
    else if (node_readinitstatus() == MICAZ_INIT_READY)
    {
        CURRENT_NODE_ID = node_readnodeid();
        srand(CURRENT_NODE_ID);
        genericreadBytes(NETWORKNAMEOFFSET, 16, networkid);
        genericreadBytes(NODEFILENAMEOFFSET, 16, filenameid);
        nodeid = CURRENT_NODE_ID;
        networkid[16] = '\0';
        filenameid[16] = '\0';
        Leds_redToggle();
        Leds_greenToggle();
        Leds_yellowToggle();
        //formatSystem();
        //buildRootNode();
        //buildDeviceDirectory();
        readVectorFlashFromExternalStorage();
        readVectorNodeFromExternalStorage();
        Leds_redToggle();
        Leds_greenToggle();
        Leds_yellowToggle();
        //node_setinitstatus(MICAZ_INIT_READY);  
    }
    //entry point. Every time the system is booted, start from here. 
    else
    {
        /* The following is for debugging the kernel , where no reprogramming overboard is used 
         */
        
		if (JTAG_TOGGLE == 1)
		{
        Leds_redToggle();
        Leds_greenToggle();
        Leds_yellowToggle();
        mystrncpy(networkid, "sn01\0", 5);
        mystrncpy(filenameid, "nodeR\0", 6);


        CURRENT_NODE_ID = 2;
        nodeid = CURRENT_NODE_ID;
        srand(nodeid);
        formatSystem();
        buildRootNode();
        genericwriteBytes(NETWORKNAMEOFFSET, 16, networkid);
        genericwriteBytes(NODEFILENAMEOFFSET, 16, filenameid);
        node_writenodeid(nodeid);
        node_setinitstatus(MICAZCONFIGMESSAGERECEIVED);
        node_setradiochannel(22);
        Leds_redToggle();
        Leds_greenToggle();
        Leds_yellowToggle();
       }
        
    }

   
  

    //timer and radio 
    GenericTimerInit();
    //for global timing purpose use
    GenericTimingStart(); 
     
    AMStandard_Control_init();
    AMStandard_Control_start();
    
    initRadioHandle();
    
    InitShell();
    //initTrace();
    

 
 
    
    
    
    {
        uint8_t currentchannel;
         
         
        currentchannel = node_getradiochannel();
#ifdef RADIO_CC2420
				
        cc2420controlm_CC2420Control_TuneChannel(currentchannel);
       
#endif
#ifdef RADIO_CC2420
        cc2420controlm_CC2420Control_TunePower(31);
        
#endif
    }
    
// 		{
// 	  uint32_t counter;    
//
//    printfintegeru32(234234223);
//    printfstrln();
//    printfinteger32(-5555555);
//    printfstrln();
//    printfinteger32(777777);
//    printfstrln();
//    printfstr("Showing the status!!! \n"); 

//    counter = getCurrentResolution();
//    printfintegeru32(counter); 
//    printfstrln();
// 		}
//    

    //threadsequencelogging
    //eeprom_chunk_init();

   create_thread(ShellThread, (uint16_t *) shellbuffer,
                  STACK_TOP(shellbuffer), 0, 15, "sysshell", 0, 0);



    //trace program must be thread 1 to enable the tracing functionality. 
    //create_thread(tracemain, (uint16_t *) tracebuffer,
      //           STACK_TOP(tracebuffer), 0, 15, "trace", 0, 0);
    
	//  create_thread(sounder, (uint16_t *) sounderbuffer,
      //            STACK_TOP(sounderbuffer), 0, 15, "sounder", 0, 0);
      
    // create_thread(countermain, (uint16_t *)countertorfm, STACK_TOP(countertorfm), 0, 15, "counter", 0, 0);
      
    //create_thread(hello, (uint16_t *)hellobuffer,
      //            STACK_TOP(hellobuffer), 0, 15, "hello", 0, 0);

    //create_thread(count, (uint16_t *)countbuffer,
      //            STACK_TOP(countbuffer), 0, 15, "count", 0, 0);
    // create_thread(logger, (uint16_t *)loggerbuffer,     
      //          STACK_TOP(loggerbuffer), 0, 15, "logger", 0, 0);
                  
    //create_thread(gfapp, (uint16_t *) gfbuffer,
      //            STACK_TOP(gfbuffer), 0, 15, "gfapp", 0, 0);
                  
    //create_thread(protocol, (uint16_t *) protocolbuffer,
      //            STACK_TOP(protocolbuffer), 0, 15, "proto", 0, 0);
                                 
  


    //sleeping configureation 
    // sbi(MCUCR, SM0);
    // sbi(MCUCR, SM1);
    // cbi(MCUCR, SM2); 
    // sbi(MCUCR, SE);
   
    //GenericTimerStart(15, TIMER_REPEAT, 100); 


  #ifdef ENERGY_SHARE_SCHEDULING

      ecb_init();     
      GenericTimerStart(12, TIMER_REPEAT, 1000); 
     
  #endif 

    _avr_enable_interrupt();

    while (1)
    {
        runNextTask();
    }
    return 0;
}
