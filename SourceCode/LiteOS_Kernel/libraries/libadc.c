/** @file libadc.c
       @brief The functional implementation for the sensor API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#include "libadc.h"
#include "liteoscommon.h"
#include "libthread.h"
#include "libsystem.h"
#include "../types/types.h"

int lib_get_light()
{
   lib_thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_LIGHT;
   current_thread = lib_get_current_thread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  
  
}


int lib_get_temp()
{
   lib_thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_TEMP;
   current_thread = lib_get_current_thread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  

}


int lib_get_magx()
{
   lib_thread** current_thread; 
   
   {_atomic_t test = _atomic_start();
   
   void (*fp)() = (void (*)(void))ADC_READ_MAGX;
   current_thread = lib_get_current_thread();    
   fp(); 
   
   _atomic_end(test);
  }
   return (*current_thread)->data.adcstate.adcreading;  

}


int lib_get_magy()
{
   lib_thread** current_thread; 
  {_atomic_t test = _atomic_start();
   void (*fp)() = (void (*)(void))ADC_READ_MAGY;
   current_thread = lib_get_current_thread();    
   fp(); 
   
      
   _atomic_end(test);
  }
   return (*current_thread)->data.adcstate.adcreading;  
}


int lib_get_accx()
{
   lib_thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_ACCX;
   current_thread = lib_get_current_thread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  
}


int lib_get_accy()
{
   lib_thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_ACCY;
   current_thread = lib_get_current_thread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  
}
