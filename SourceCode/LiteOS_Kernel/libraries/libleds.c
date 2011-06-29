/** @file libleds.c
       @brief The functional implementation  for the Leds API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#include "libleds.h"
#include "liteoscommon.h"
#include "../types/types.h"
void lib_green_toggle()
{
 void (*greenfp)() = (void (*)(void))GREEN_LED_TOGGLE_FUNCTION; 
 greenfp();                              



}

void lib_red_toggle()
{
                          
  void (*redfp)() = (void (*)(void))RED_LED_TOGGLE_FUNCTION; 
  redfp();    
}


void lib_yellow_toggle()
{
                          
  void (*yellowfp)() = (void (*)(void))YELLOW_TOGGLE_FUNCTION; 
  yellowfp();    
}



void lib_red_on()
{
                          
  void (*redfp)() = (void (*)(void))RED_ON_FUNCTION; 
  redfp();    
}



void lib_red_off()
{
                          
  void (*redfp)() = (void (*)(void))RED_OFF_FUNCTION; 
  redfp();    
}



void lib_green_on()
{
                          
  void (*greenfp)() = (void (*)(void))GREEN_ON_FUNCTION; 
  greenfp();    
}


void lib_green_off()
{
                          
  void (*greenfp)() = (void (*)(void))GREEN_OFF_FUNCTION; 
  greenfp();    
}

void lib_yellow_on()
{
                          
  void (*yellowfp)() = (void (*)(void))YELLOW_ON_FUNCTION; 
  yellowfp();    
}

void lib_yellow_off()
{
                          
  void (*yellowfp)() = (void (*)(void))YELLOW_OFF_FUNCTION; 
  yellowfp();    
}
