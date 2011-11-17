#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libadc.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libserial.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libsystem.h"




/*
 Limitations:
 It is known that the current implementation uses random number + nodeid to track duplicates. If a node reboots, it may reuse the random numbers and 
 the nodeid, therefore generating packets that may look like duplicates. This is a known limitation due to node reboot. 
*/


//Register port 20 for data packets. No neighbor exchanges


/*Each packet format:
     nodeid  2 bytes
     randomsig  2 bytes
     data content 
*/

uint8_t floodingprotocolbuffer[400];

uint16_t nodeid; 

uint16_t packetsreceived, packetsdelivered; 


//Pointer to the current thread data structure. Useful for waking up the current thread. 
lib_thread **thiscurrentthread;
lib_thread *thisthread; 

typedef struct
{
  uint16_t randomsig; 
  uint16_t nodeid; 
  uint8_t valid; 
}
historypacket; 

#define HISTORYBUFFERSIZE 20

//The current index
uint8_t indexOfPackets;

//Neighbor table 
static historypacket historypackettable[ HISTORYBUFFERSIZE ];


//This function initlizes the neighhood table 

void initTable() {
   int i;
   for ( i = 0; i < HISTORYBUFFERSIZE; i ++ ) {
      historypackettable[ i ].valid = 0;
   } 
   indexOfPackets = 0;
}


uint8_t historycheck(uint16_t nodeid, uint16_t randomid)
{
   uint8_t i; 
   for (i=0; i < HISTORYBUFFERSIZE; i++) {
   	 if (historypackettable[i].valid == 0)
	 	continue;
	 if ((historypackettable[i].randomsig == randomid ) && (historypackettable[i].nodeid == nodeid))
	 	return 1; 
   	}
   return 0; 
}


void inserthistorytable(uint16_t nodeid, uint16_t randomid)
{

   historypackettable[indexOfPackets].valid = 1; 
   historypackettable[indexOfPackets].nodeid = nodeid; 
   historypackettable[indexOfPackets].randomsig = randomid; 
   indexOfPackets = (indexOfPackets + 1)%HISTORYBUFFERSIZE;
   return;  
}



//-------------------------------------------------------------------------
static uint8_t incomingMsg[ 32 ];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[ 4 ];


//Handles incoming data msg. This usually wakeup the current thread 
void wakeupMeOnDataMsg() {

   thisthread->state = STATE_ACTIVE;
   syscall_post_thread_task();    
}


//Registers the data packet for receving incoming packets. This enables receiving both remote and local data packets

void registerDataPacket() {

   _atomic_t currentatomic;
   radiohandletype *radiohandleaddr;
   
   void (*getaddrfp)(void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT; 

   //Finds the radio handle, so that when this code is ported to be externally used, it will be obtained via a system call 
   radiohandleaddr =  lib_get_current_radio_receive_handle_addr();

   incomingDataLength = 0;
   
   //set up the radiohandleaddr data structures
   radiohandleaddr->port = 20;
   radiohandleaddr->maxLength = 32;
   radiohandleaddr->dataReady = &incomingDataLength;
   radiohandleaddr->data = incomingMsg;
   radiohandleaddr->packetinfo = incomingPacketInfo;
   radiohandleaddr->handlefunc = wakeupMeOnDataMsg;

   //close the interrupt     
   currentatomic = _atomic_start();
   
   //call the radio handle set to store the data structure into the handle vectors 
   getaddrfp();     
   //syscall_registerEvent();
   
   //open the interrupt 
   _atomic_end( currentatomic );
   return ;
}




//This is the entry point. It broadcasts neighbor exchanges perodically, or handles incoming packets (non-update neighbor)

void floodingprotocol() {

   uint8_t portnum;
   uint16_t nodeid; 

   uint16_t randomsig;
   uint16_t incomingnodeid; 

   uint8_t temp;
   uint8_t jitter; 
   uint16_t comingnodeid; 
  

   initTable();
   nodeid = lib_get_node_id();
   
   //register both the neighborhood's update and the message incoming 

   thiscurrentthread = lib_get_current_thread(); 
   thisthread = *thiscurrentthread; 
      
   registerDataPacket();

   packetsreceived = packetsdelivered = 0; 


   while ( 1 ) {

         lib_hibernate_thread();
		 incomingnodeid = 0; 
         //handles the data message by restructing it and broadcast it  
         //If the packet is new, send it both locally and remotely

		 temp  = incomingMsg[ 0 ];
		 incomingnodeid += ((uint16_t)temp); 
		 temp = incomingMsg[ 1 ];
		 incomingnodeid += ((uint16_t)temp) * 256;

         randomsig = 0; 
		 temp = incomingMsg[ 2 ];
		 randomsig += ((uint16_t)temp);

   	     temp = incomingMsg[ 3 ];
		 randomsig += ((uint16_t)temp) * 256;

		 portnum = incomingMsg[4]; 

		 if ((nodeid == incomingnodeid)&&(historycheck(incomingnodeid, randomsig) == 0))
		 	{
    		 	inserthistorytable(incomingnodeid, randomsig); 
				incomingMsg[5] = nodeid%256; 
				incomingMsg[6] = nodeid/256; 
				
				jitter = lib_random()%20;
				lib_sleep_thread(jitter); 
				lib_radio_send_msg(20, 0xffff, incomingDataLength, incomingMsg);
				packetsreceived ++; 
		    }
		 

		 else if (historycheck(incomingnodeid, randomsig) == 0)
		 	{
             
				comingnodeid = incomingMsg[5];
				if ((comingnodeid != nodeid -1 )&&( comingnodeid!=nodeid+1))
					{
					}
				else
					{
                lib_radio_send_msg(portnum, 0, incomingDataLength, incomingMsg); 
				
				inserthistorytable(incomingnodeid, randomsig); 

				incomingMsg[5] = nodeid%256;
				
				jitter = lib_random()%20;
				lib_sleep_thread(jitter); 
				lib_radio_send_msg(20, 0xffff, incomingDataLength, incomingMsg);
				packetsdelivered++; 
					}
			  
					}
		 else
		 	{
            
		 	}
   	}
   return; 
}


			 
        

