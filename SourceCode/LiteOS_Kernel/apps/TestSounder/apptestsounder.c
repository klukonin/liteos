#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libsounder.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libadc.h"

int sounderbuffer[200];

void sounder()
{

 
 
 
   while (1)
   	{
	   
	   lib_yellow_toggle(); 
	   lib_sounder_on();     
	   lib_sleep_thread(2000);
	   lib_sounder_off();
     lib_sleep_thread(2000);
    }
   
	return; 
}

