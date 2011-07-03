/** @file realmain.c
       @brief The system entry point.

       This file is the entry point of the entire LiteOS kernel. All functions are invocations of other system modules. 

        @author Qing Charles Cao (cao@utk.edu)
      
*/


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
#include "./realmain.h"
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

/** @brief The starting point of the control loop 

       @return  SUCCESS (0)
*/

int main()
{
	 
    //micaz specific initilizations, hardware init
    LITE_SET_PIN_DIRECTIONS();
    
    //sensors init
    adcdriver_init_adc();

    //kernel ints
    initScheduling();
	
    thread_init();
    
    //inits printing 
    initUSART();

	//Leds initing
	Leds_Leds(); 
    
     Leds_redToggle();
     Leds_greenToggle();
     Leds_yellowToggle();
     mystrncpy(networkid, "testbed\0", 8);
     mystrncpy(filenameid, "node00\0", 7);
	 
     CURRENT_NODE_ID = 8; 

     nodeid = CURRENT_NODE_ID;
	 
	 filenameid[4] = (char)(nodeid/10 + 0x30); 
	 filenameid[5] = (char)(nodeid%10 + 0x30); 

  	 atmel_flash_init();

	 sounder_init();      

	 #ifdef FORMATFILESYSTEM
     formatSystem();
     buildRootNode();
     #endif
	 
	 
     genericwriteBytes(NETWORKNAMEOFFSET, 16, networkid);
     genericwriteBytes(NODEFILENAMEOFFSET, 16, filenameid);
     node_writenodeid(nodeid);
	 
     	 
     Leds_redToggle();
     Leds_greenToggle();
     Leds_yellowToggle();
     srand(CURRENT_NODE_ID);
  

    //timer and radio 
    GenericTimerInit();
	
    //for global timing purpose use
    GenericTimingStart(); 
     
    AMStandard_Control_init();
    AMStandard_Control_start();
    
    initRadioHandle();
    
    InitShell();

	    
    #ifdef RADIO_CC2420
    cc2420controlm_CC2420Control_TuneChannel(21); 
    cc2420controlm_CC2420Control_TunePower(31);
    #endif
    
    #ifdef TESTPRINTING
    uint32_t counter;    
    printfuinteger32(234234223);
    printfstrln();
    printfinteger32(-5555555);
    printfstrln();
    printfinteger32(777777);
    printfstrln();
    printfstr("Showing the status!!! \n"); 
	printfstrln();
	counter = getCurrentResolution();
    printfintegeru32(counter); 
    printfstrln();
	#endif    

   create_thread(ShellThread, (uint16_t *) shellbuffer,
                  STACK_TOP(shellbuffer), 0, 15, "sysshell", 0, 0);
  
   create_thread(blink, (uint16_t *) blinkbuffer,
                 STACK_TOP(blinkbuffer), 0, 15, "test", 0, 0);
   //sleeping configureation 
   #ifdef ENERGYSAVINGMODE
   sbi(MCUCR, SM0);
   sbi(MCUCR, SM1);
   cbi(MCUCR, SM2); 
   sbi(MCUCR, SE);
   #endif
   
     _avr_enable_interrupt();

    while (1)
    {
        runNextTask();
    }
    return 0;
}
