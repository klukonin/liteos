#include "socketfile.h"
#include "../kernel/threadkernel.h"
#include "socketthread.h"
#include "../kernel/threadtools.h"
#include "../kernel/threaddata.h"

#include "../storage/filesys/fsapi.h"

#include "../kernel/scheduling.h"


char filepathaddr[20];
char filemodeaddr[5];

extern volatile thread *current_thread;
  
volatile thread *openthread;

uint8_t threadindex;
MYFILE *filehandle;
uint8_t *databuffer;
uint16_t nBytes;
int offset, position;



//-------------------------------------------------------------------------
//Get the file path address
void *getFilePathAddress()
{
    return (void *)filepathaddr;
}

//-------------------------------------------------------------------------
//Get the file mode address
void *getFileModeAddress()
{
    return (void *)filemodeaddr;
}

//-------------------------------------------------------------------------
void openfile_task()
{
    MYFILE *temp = fopen2(filepathaddr, filemodeaddr);

    openthread->filedata.filestate.fileptr = (uint8_t *) temp;
    
	 barrier_unblock(7, 1);

     return;

}

//-------------------------------------------------------------------------
void openFileTask()
{
    openthread = current_thread;
    postTask(openfile_task, 5);
    return;
}

//-------------------------------------------------------------------------
void closefile_task()
{
    fclose2(filehandle);
	barrier_unblock(7, 2);
}

//-------------------------------------------------------------------------
void closeFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.filestate.fileptr;
    postTask(closefile_task, 5);
}

//-------------------------------------------------------------------------
void readfile_task()
{
    fread2(filehandle, databuffer, nBytes);
	barrier_unblock(7, 3);
}

//-------------------------------------------------------------------------
void readFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.filestate.fileptr;
    databuffer = current_thread->filedata.filestate.bufferptr;
    nBytes = current_thread->filedata.filestate.bytes;
    postTask(readfile_task, 5);
}

//-------------------------------------------------------------------------
void writefile_task()
{
    fwrite2(filehandle, databuffer, nBytes);
	barrier_unblock(7, 4);
     
}

//-------------------------------------------------------------------------
void writeFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.filestate.fileptr;
    databuffer = current_thread->filedata.filestate.bufferptr;
    nBytes = current_thread->filedata.filestate.bytes;
    postTask(writefile_task, 5);
}

//-------------------------------------------------------------------------
void seekFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.fileseekstate.fileptr;
    offset = current_thread->filedata.fileseekstate.offset;
    position = current_thread->filedata.fileseekstate.position;
    fseek2(filehandle, (int32_t) offset, position);
    return;
}
