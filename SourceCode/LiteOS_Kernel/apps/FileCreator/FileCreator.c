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

 LIB_MYFILE *fileptr;
 LIB_MYFILE  *configptr; 
 uint16_t samples; 
 uint16_t reading; 
 uint16_t sleeptime; 
 uint16_t type; 

uint8_t filecreatorbuffer[400];

int filecreator()
{ 


 fileptr = lib_mfopen("/file1", "w");
 samples = 50; 
 sleeptime = 100; 

 while (1)
 {
  if (samples >0)
   samples--; 
   else
   break; 
    
   lib_sleep_thread(sleeptime);

   lib_yellow_toggle(); 

   {{

    char _tempbuffer[32];
    char *_temp1 = "Hello ";
    char *_temp3 = "World\n";
	  uint8_t lengthstring;
    
    _tempbuffer[0] = '\0';
    lib_string_append(_tempbuffer, _temp1);
    lib_string_append(_tempbuffer, _temp3);

    lengthstring = lib_string_length(_tempbuffer); 

    lib_mfwrite(fileptr, _tempbuffer, lengthstring);
  	lib_mfseek(fileptr, lengthstring, 1);

    }}
   
}


lib_mfclose(fileptr);

return 0; 

}

