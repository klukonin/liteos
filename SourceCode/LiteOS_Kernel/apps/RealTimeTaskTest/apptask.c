#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libfile.h"
#include <util/delay.h>

uint8_t threadAbuffer[200];
uint8_t threadBbuffer[200];
uint8_t threadCbuffer[200];

void compTaskA(){
    _delay_ms(100);
    return;
}

void threadA()
{
    while (1)
	{
        lib_sleep_thread(1000);
        //lib_yellow_toggle();
        lib_post_real_time_user_task(compTaskA, 1000);
    }
  
  return; 
}





void compTaskB(){
    _delay_ms(200);
    return;
}

void threadB()
{
    while (1)
	{
        lib_sleep_thread(2000);
        //lib_green_toggle();
        lib_post_real_time_user_task(compTaskB, 2000);
    }
  
  return; 
}




void compTaskC(){
    _delay_ms(1000);
    return;
}

void threadC()
{
    while (1)
	{
        lib_sleep_thread(5000);
        //lib_red_toggle();
        lib_post_real_time_user_task(compTaskC, 5000);
    }
  
  return; 
}



