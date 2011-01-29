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


uint8_t channelhopbuffer[300]; 

int channelhop()
{ 

 uint8_t channel;
 
 for (channel = 11;channel <25;channel++)
 { lib_radio_set_channel(channel); 
  
 lib_radio_send_string("Hello, world!\n"); 
 lib_yellow_toggle();
 lib_sleep_thread(1000);

 }
 
 channel = 15; 
 lib_radio_set_channel(channel); 
 return 0; 

}

