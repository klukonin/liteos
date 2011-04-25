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

static uint16_t nodeid;


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
     node_setradiochannel(22);
     Leds_redToggle();
     Leds_greenToggle();
     Leds_yellowToggle();
   
  

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
    
 		
// 	  uint32_t counter;    
//
    printfuinteger32(234234223);
    printfstrln();
    printfinteger32(-5555555);
    printfstrln();
    printfinteger32(777777);
    printfstrln();
    printfstr("Showing the status!!! \n"); 
	printfstrln();
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
   // create_thread(tracemain, (uint16_t *) tracebuffer,
     //            STACK_TOP(tracebuffer), 0, 15, "trace", 0, 0);
    
	//  create_thread(sounder, (uint16_t *) sounderbuffer,
      //            STACK_TOP(sounderbuffer), 0, 15, "sounder", 0, 0);
      
    // create_thread(countermain, (uint16_t *)countertorfm, STACK_TOP(countertorfm), 0, 15, "counter", 0, 0);
      
    create_thread(printserialmain, (uint16_t *)printserialbuffer,
                STACK_TOP(printserialbuffer), 0, 15, "print", 0, 0);

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
