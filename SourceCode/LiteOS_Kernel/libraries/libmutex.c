/** @file libmutex.c
       @brief The functional implementation for the mutex API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#include "libsystem.h"
#include "libthread.h"
#include "liteoscommon.h"
#include "../types/types.h"







void lib_file_barrier_block(uint8_t type, uint8_t id){
   
    _atomic_t currentatomic;
	lib_thread **current_thread; 

     
	current_thread = lib_get_current_thread(); 

    currentatomic = _atomic_start();
    
      (*current_thread)->state = STATE_IO;
      (*current_thread)->data.iostate.type = type;
      (*current_thread)->data.iostate.id = id;
      lib_yield();
    
    _atomic_end(currentatomic);
}



