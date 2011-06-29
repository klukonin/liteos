/** @file socketeeprom.c
	@brief The functional implementation for socket eeprom.

	@author Qing Cao (cao@utk.edu)
*/


#include "socketeeprom.h"
#include "../storage/bytestorage/bytestorage.h"


static genericByteStorageTaskNode storageTask;

void *getGenericStorageNodeAddress()
{
    return (void *)&storageTask;
}

//-------------------------------------------------------------------------
void genericReadTask()
{
    genericreadBytes(storageTask.addr, storageTask.nBytes, (void *)
                     storageTask.buffer);
}

//-------------------------------------------------------------------------
void genericWriteTask()
{
    genericwriteBytes(storageTask.addr, storageTask.nBytes, (void *)
                      storageTask.buffer);
}
