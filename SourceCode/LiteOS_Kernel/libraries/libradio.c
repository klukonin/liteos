/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, Hossein Ahmadi 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "libradio.h"
#include "../types/types.h"
#include "liteoscommon.h"


static uint8_t buffer[MAX_MSG_LENGTH];

uint8_t radioReceiveDataReady;

uint8_t radioReceivePacketInfo[4];

lib_thread* mythread;

void lib_radio_send_msg_syscall()
{
 void (*radiosendfp)() = (void (*)(void))SOCKET_RADIO_SEND_FUNCTION;
 radiosendfp();
}

//This function sends out a string

void lib_radio_send_string(uint8_t *msg)
{
    uint8_t temp = (uint8_t)lib_string_length((char *)msg);
 
	return lib_radio_send_msg(10, 0xffff, temp, msg);
 
}

void lib_radio_send_integer_u16(uint16_t value)
{
  uint8_t temp1,temp2;
  temp1 = (uint8_t) (value >> 8);
  temp2 = (uint8_t) (value & 0x00ff);

  buffer[0] = temp1;
  buffer[1] = temp2;
  buffer[2] = buffer[3] = 0xee;

  return lib_radio_send_msg(12, 0xffff, 16, buffer);
}

radiohandletype *lib_get_current_radio_receive_handle_addr()
{
   radiohandletype *currentradioinfo;

   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_RADIO_RECEIVER_BUFFER;

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currentradioinfo)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currentradioinfo;
}


void lib_radio_set_freq(uint16_t freq)
{

   void (*getaddrfp)(void) = (void (*)(void))SET_RADIO_FREQ_FUNCTION;

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);

   asm volatile(" mov r20, %A0" "\n\t"
	              "mov r21, %B0" "\n\t"
				 :
				 :"r" (freq)
                );
   getaddrfp();
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return;
}

void lib_radio_set_channel(uint8_t channel)
{

  uint16_t maskchannel = channel;

  void (*getaddrfp)(void) = (void (*)(void))SET_RADIO_CHANNEL_FUNCTION;

  asm volatile("push r20" "\n\t"
               "push r21" "\n\t"
               ::);

  asm volatile(" mov r20, %A0" "\n\t"
	              "mov r21, %B0" "\n\t"
				 :
				 :"r" (maskchannel)
                );
   getaddrfp();
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return;
}

void lib_radio_set_power(uint8_t power)
{

  uint16_t maskchannel = power;

  void (*getaddrfp)(void) = (void (*)(void))SET_RADIO_POWER_FUNCTION;

  asm volatile("push r20" "\n\t"
               "push r21" "\n\t"
               ::);

  asm volatile(" mov r20, %A0" "\n\t"
	              "mov r21, %B0" "\n\t"
				 :
				 :"r" (maskchannel)
                );
   getaddrfp();
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return;
}



radioinfotype *lib_get_current_radio_send_addr()
{
   radioinfotype *currentradioinfo;
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_RADIO_SENDER_BUFFER;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currentradioinfo)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currentradioinfo;
}


 
 




void lib_radio_send_msg(uint16_t port, uint16_t address, uint8_t length, uint8_t *msg)
{


   lib_thread** current_thread;
   radioinfotype *radioinfoaddr;



   current_thread = lib_get_current_thread();
   radioinfoaddr = lib_get_current_radio_send_addr();
 
     
  // (*current_thread)->energycontrolblock.energycost += (uint16_t)RADIO_SEND * (uint16_t)length;           
      		 
            
      

   radioinfoaddr-> socket_port = port;
   radioinfoaddr->socket_addr = address;
   radioinfoaddr->socket_msg_len  = length;
   radioinfoaddr->socket_msg  = msg;
   lib_radio_send_msg_syscall();

   lib_sleep_thread(20);

 

   return;
}

//wakeup the current thread once an incoming packet arrives

void lib_wakeup_mythread()
{
  mythread->state = STATE_ACTIVE;
  syscall_post_thread_task();
}

int lib_radio_receive(uint16_t port, uint8_t maxlength, uint8_t *msg)
{
   return lib_radio_receive_timed(port, maxlength, msg, 0);
}

int lib_radio_receive_timed(uint16_t port, uint8_t maxlength, uint8_t *msg, uint16_t time)
{
   _atomic_t currentatomic;
   
   void (*radio_register_function_pointer)(void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;
   lib_thread** current_thread = lib_get_current_thread();
   radiohandletype *radiohandleaddr = lib_get_current_radio_receive_handle_addr();

   //set up the radiohandleaddr data structures

   radiohandleaddr->port = port;
   radiohandleaddr->maxLength = MAX_MSG_LENGTH;
   radiohandleaddr->dataReady = &radioReceiveDataReady;
   radiohandleaddr->data = buffer;
   radiohandleaddr->packetinfo = radioReceivePacketInfo;
   radiohandleaddr->handlefunc = lib_wakeup_mythread;

	//close the interrupt
	currentatomic = _atomic_start();
	
	//call the radio handle set to store the data structure into the handle vectors
	radio_register_function_pointer();

	//set up the current thread into sleep mode
   (*current_thread)->state = STATE_SLEEP;

	//set up mythread so that later can wake up this thread
	mythread = *current_thread;

	//open the interrupt
	_atomic_end(currentatomic);

	if (time == 0)
		lib_yield();
	else
		lib_sleep_thread(time);
	
	uint8_t i;
	for (i = 0; i < maxlength && i < MAX_MSG_LENGTH; i++)
		msg[i] = buffer[i];
		
   //(*current_thread)->energycontrolblock.energycost += (uint16_t)RADIO_RECEIVE * (uint16_t)radioReceiveDataReady;           
	
  
	return radioReceiveDataReady;
}
