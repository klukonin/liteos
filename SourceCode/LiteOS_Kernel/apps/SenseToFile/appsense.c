/** @file appblink.c
	@brief The functional implementation of blink. 

	@author Qing Cao (cao@utk.edu)
*/


#include <util/delay.h>

#define F_CPU 8000000UL //8MHz
#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../libraries/libfile.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libfile.h"
 
uint8_t sensebuffer[200]; 

#define MAX 50

uint16_t counter = 0; 

LIB_MYFILE *fileptr; 

uint16_t lightreadingbuffer[2][MAX];

uint8_t indexofbuffer; 
uint8_t indexofreading; 


void writeToFileTask()
{
    _atomic_t _atomic = _atomic_start();
	uint8_t temp;
	     temp = 1-indexofbuffer; 
		 lib_mfwrite(fileptr, &lightreadingbuffer[temp][0], MAX*2);	 
		 lib_mfseek(fileptr, MAX*2, 1);
	_atomic_end(_atomic); 
}

void closeTheFileTask()
{
        lib_mfclose(fileptr);	
}


void sense()
{
	indexofbuffer = 0; 
	indexofreading = 0; 
	int i; 
	
	fileptr = lib_mfopen("/logtrace", "w");
	for (i=0; i< 200; i++)
	{
    lib_sleep_thread(100);  
   // lib_red_toggle();
   // lib_radio_send_string("hello,world\n");
    lib_yellow_toggle();
    counter = lib_get_light();
    lightreadingbuffer[indexofbuffer][indexofreading] = counter; 
	indexofreading ++;
	
	if (indexofreading == MAX)
	{   indexofbuffer = 1 - indexofbuffer; 
	    indexofreading = 0; 
        writeToFileTask();
    }
	}    
	closeTheFileTask();
	while (1)
	{
	lib_sleep_thread(100); 
	}
  return; 
}



