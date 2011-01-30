#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libadc.h"

uint8_t hellobuffer[250]; 
char *string = "hello, world"; 

int hello()
{
         
	while (1)
	{
   

  lib_yellow_toggle(); 
  lib_radio_send_string(string); 
  lib_sleep_thread(1000);
	}
	return 0; 
}


