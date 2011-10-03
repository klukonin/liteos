/** @file commandprocessor.c
	@brief The detailed implementation of command handling in serial port. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#include "commandprocessor.h"
#include "../io/radio/amradio.h"
#include "../timer/generictimer.h"

#ifdef BASE_MODE

Radio_Msg broadcastmsg; 

volatile uint8_t currentlength; 

uint8_t dataToSend[128]; 

uint8_t *pack; 

uint8_t i; 

uint8_t currentindex; 

#ifdef VER_DEBUG
Radio_MsgPtr temp;
#endif 
   

void broadcastCommand(uint8_t *receivebuffer, uint8_t length)
{

    
	 uint8_t reallength = receivebuffer[0]; 
	
	     
	 if (receivebuffer[1] == 2)
	  {
	   AMStandard_TuneChannel(receivebuffer[2]); 
	   return;   	  
	  }

     { _atomic_t _atomic = _atomic_start();
    
     {
	  pack = (uint8_t *)broadcastmsg.data;

      for (i=0;i<reallength; i++)
	    pack[i] = receivebuffer[i]; 
     }
     _atomic_end(_atomic); }  

    broadcastmsg.port = 0xfefe; 
    AMStandard_SendMsg_send(0xfefe, 0xffff, reallength, &broadcastmsg); 
	
	Broadcast2SerialAlternative(&broadcastmsg);
	
	//postTask(restoreRadioState, 5); 
    GenericTimerStart(14, TIMER_ONE_SHOT, 20); 
    return; 
}


void senddata(uint8_t length)
{
    printStringN(dataToSend, length); 

}



Radio_MsgPtr Broadcast2SerialAlternative(Radio_MsgPtr msg)
{
   
{ _atomic_t _atomic = _atomic_start();
    


   pack = (uint8_t*)(msg); 
   temp = msg; 
   
   currentlength = pack[0]; 

    
   
   currentindex = 0;

   dataToSend[currentindex++] = 0x7e;
   dataToSend[currentindex++] = 0x42;
  
     

   for (i=6;i<4+6;i++)
   {
   
   
     uint8_t temp = pack[i]; 
     if (temp == 0x7e)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5e; }
     else  if (temp == 0x7d)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5d; }
	  else 
	   dataToSend[currentindex++] = temp; 

   

   }

   dataToSend[currentindex++] = currentlength;


   for (i=4+6;i<currentlength+4+6;i++)
   {
   
   
     uint8_t temp = pack[i]; 
     if (temp == 0x7e)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5e; }
     else  if (temp == 0x7d)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5d; }
	  else 
	   dataToSend[currentindex++] = temp; 

   

   }
  

  
  dataToSend[currentindex++] = 0;
  dataToSend[currentindex++] = 0;
  dataToSend[currentindex++] = 0x7e; 
   _atomic_end(_atomic); }  
  

  senddata(currentindex); 
  
  return msg;      

}



#endif 