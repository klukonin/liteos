#ifndef EEPROMSOCKETH
#define EEPROMSOCKETH

#include "../types/types.h"
/** \ingroup bytestorage 
The generic byte storage task structure 
*/
typedef struct
{
    uint16_t addr;
    uint16_t nBytes;
    uint8_t *buffer;
} genericByteStorageTaskNode;

void *getGenericStorageNodeAddress();

void genericReadTask();

//-------------------------------------------------------------------------
void genericWriteTask();



#endif 