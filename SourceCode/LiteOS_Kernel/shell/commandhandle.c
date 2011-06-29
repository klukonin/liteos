/** @file commandhandle.c
	@brief The detailed implementation of the command handle. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#include "commandhandle.h"


//File system module 
#include "../storage/filesys/stdfsa.h"
#include "../storage/filesys/fsconfig.h"
#include "../storage/filesys/inode.h"
#include "../storage/filesys/fs_structure.h"

//Thread kernel module
#include "../kernel/threadkernel.h"
#include "../kernel/threadtools.h"
#include "../kernel/scheduling.h"

//timing
#include "../timer/generictimer.h"

//Types
#include "../types/types.h"
#include "../types/string.h"
#include "../storage/bytestorage/bytestorage.h"

//Radio
#include "../io/radio/packethandler.h"
#include "../syscall/socketradiodata.h"
#include "../io/radio/amradio.h"
#include "../io/radio/amcommon.h"

//config 
#include "../config/nodeconfig.h"

//utilities
#include "../utilities/math.h"

//boot 
#include "../bootloader/bootloader.h"

//hardware
#include "../hardware/avrhardware.h"
#include "../hardware/micaz/micazhardware.h"


#include <stdlib.h>
 

extern volatile uint16_t *old_stack_ptr;
extern void (*timercallback[8]) ();

uint8_t IncomingLength;
uint8_t IncomingMsg[64];
uint8_t PacketInfo[2];
uint16_t chunksequencenum, expectNextNum;
uint8_t createthreadname[12];
uint8_t shellbuffer[256];

volatile thread *thisthread;

  
MYFILE *openedfile;
MYFILE *fp;
unsigned char reply[32];

//for reply 231
uint8_t blockid;
char filename[13];
uint16_t nodeid;

typedef struct {
	 uint16_t addr;
	 uint8_t breakpointinstructions[6];
	 uint8_t active; 
} br_type;

br_type br_state[NUM_BREAKPOINTS];

void init_breakpoint_table()
{
   int i;
   for (i=0; i< NUM_BREAKPOINTS; i++)
     br_state[i].active = 0; 	
	
}


uint8_t get_breakpoint_index()
{
    int i; 	
	  for (i=0; i< NUM_BREAKPOINTS; i++)
     {
     	  if ( br_state[i].active == 0)
     	  	{
     	  		 br_state[i].active = 1; 
     	  		 return i; 
     	    }
	   } 
	   return 255;  
}

 


 

//-------------------------------------------------------------------------
//Note that this function should consider splitting a message into multiple pieces, if the requested memory chunk is too large 
void reply_debugging_print(uint8_t * receivebuffer)
{
    //This is the reply for the print command 
    uint16_t addr;
    uint16_t size;
    uint8_t chunkindex;
    uint8_t numOfChunks;

    addr = ((uint16_t) receivebuffer[3]) * 256 + (receivebuffer[4]);
    size = (uint16_t) receivebuffer[5] * 256 + receivebuffer[6];
    numOfChunks = size / 24 + 1;
    for (chunkindex = 0; chunkindex < numOfChunks; chunkindex++)
    {
        reply[0] = 24 + 6;
        reply[1] = 91;
        reply[2] = nodeid;
        reply[3] = chunkindex;
        reply[4] = size / 256;
        reply[5] = size % 256;
        //Note that this expose some piece of memory that is NOT part of the requested data 
        mystrncpy((char *)&reply[6], (char *)addr, 24);
        addr = addr + 24;
        StandardSocketSend(0xefef, 0xffff, 32, reply);
    }
    //printStringN(reply,32); 
}

//this is the reply for the set command 
void reply_debugging_set(uint8_t * receivebuffer)
{
    uint16_t addr;
    uint8_t size;
    uint8_t i;

    addr = (uint16_t) receivebuffer[3] * 256 + receivebuffer[4];
    size = receivebuffer[5];
    for (i = 0; i < size; i++)
    {
        *((unsigned char *)(addr + i)) = receivebuffer[6 + i];
    }
    reply[0] = 3;
    reply[1] = 96;
    reply[2] = nodeid;
    //mystrncpy(&reply[3], (unsigned char *)addr, size); 
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //printStringN(reply,32); 
}

//-------------------------------------------------------------------------
void reply_debugging_insert_avr_breakpoint(uint8_t * receivebuffer)
{
    uint16_t pagecount;
    uint8_t offset;
    uint8_t instructions[6];
    uint32_t temp; 
    uint8_t index; 
    
    pagecount = (uint16_t) receivebuffer[3] * 256 + receivebuffer[4];
    offset = receivebuffer[5];
    
    temp = (uint32_t)pagecount * 256 + (uint32_t)offset; 
    temp = temp/2; 
    
    index = get_breakpoint_index();
    br_state[index].addr = temp;
    
    boot_insertBreakPoint(pagecount, offset, instructions);
    reply[0] = 12;
    reply[1] = 92;
    reply[2] = nodeid;
    reply[3] = receivebuffer[3];
    reply[4] = receivebuffer[4];
    reply[5] = receivebuffer[5];
    for (offset = 0; offset < 6; offset++)
    {
        reply[6 + offset] = instructions[offset];
         br_state[index].breakpointinstructions[offset] = instructions[offset]; 
    }
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------

//this should copy the thread information from a file 
//to be written as 93, and use the name of the thread to continue its operations. 
//the shell should support operating on the files to directly find out the instructions 
//Using python or Perl should be OK as well 
//Support for breakpoints should be finished tomorrow
//And the thread dump and thread_state_restore should be the next task. 
//and this task should check the address to see which location is the address to be need. 
//use uint32_t because uint16_t is too small 
#ifdef PLATFORM_AVR
void reply_debugging_remove_avr_breakpoint(uint8_t * receivebuffer)
{
    uint32_t addrbreakpoint;
    uint16_t pagecount;
    uint8_t offset;
    uint8_t i;
    uint8_t breakpointfound = 0;

    addrbreakpoint = ((uint16_t) receivebuffer[3] * 256) + receivebuffer[4];
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state == STATE_BREAK)
        {
            uint8_t *tempptr = (uint8_t *) thread_table[i].sp + 47;
            uint16_t addr =
                (((uint16_t) (*tempptr)) * 256 + *(tempptr + 1) - 3);
            if (addr == addrbreakpoint)
            {
                thread_table[i].state = STATE_ACTIVE;
                *tempptr = addrbreakpoint / 256;
                *(tempptr + 1) = addrbreakpoint % 256;
                break;
            }
        }
    }
    if (i < LITE_MAX_THREADS)
    {
        pagecount = addrbreakpoint / 128;
        offset = addrbreakpoint * 2 % 256;
        boot_removeBreakPoint(pagecount, offset, &receivebuffer[5]);
        breakpointfound = 1;
    }
    reply[0] = 5;
    reply[1] = 93;
    reply[2] = nodeid;
    reply[3] = breakpointfound;
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------

#endif
//-------------------------------------------------------------------------
static void thread_state_restore(uint8_t index, uint8_t * filename)
{
    MYFILE *fp;
    uint8_t threadsize;
    uint16_t threadramsize;
    uint8_t *internal_ram_start;
    threadsize = sizeof(struct thread);
    threadramsize =
        (uint16_t) ((uint8_t *) thread_table[index].ramend -
                    (uint8_t *) thread_table[index].ramstart + 1);
    internal_ram_start = (uint8_t *) thread_table[index].ramstart;
    fp = fsopen((char *)filename, "r");
    fread2(fp, &thread_table[index], threadsize);
    fseek2(fp, threadsize, 1);
    fread2(fp, internal_ram_start, threadramsize);
    fclose2(fp);
}

//-------------------------------------------------------------------------
static void thread_state_snapshot(uint8_t index, uint8_t * filename)
{
    MYFILE *fp;
    uint8_t threadsize;
    uint16_t threadramsize;
    uint8_t *internal_ram_start;
    threadsize = sizeof(struct thread);
    threadramsize =
        (uint16_t) ((uint8_t *) thread_table[index].ramend -
                    (uint8_t *) thread_table[index].ramstart + 1);
    internal_ram_start = (uint8_t *) thread_table[index].ramstart;
    fp = fsopen((char *)filename, "w");
    fwrite2(fp, &thread_table[index], threadsize);
    fseek2(fp, threadsize, 1);
    fwrite2(fp, internal_ram_start, threadramsize);
    fclose2(fp);
}

//this should copy the thread information into a file 
//The packet should contain the information regarding the user 
//THe packet should contain the index of the thread and the file name that will be written as the new file 
//the file name part requires careful debugging of the file system 
void reply_thread_state_snapshot_tofile(uint8_t * receivebuffer)
{
    uint8_t i;
    uint8_t testtrue = 0;

    receivebuffer[receivebuffer[0]] = '\0';
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state != STATE_NULL)
        {
            if (superstring
                ((char *)thread_table[i].threadName,
                 (char *)&receivebuffer[5]) == 0)
            {
                testtrue = i;
                break;
            }
        }
    }
    if (testtrue != 0)
    {
        receivebuffer[receivebuffer[3] + 5] = '/';
        thread_state_snapshot(testtrue,
                              (uint8_t *) & receivebuffer[receivebuffer[3] +
                                                          5]);
    }
    reply[0] = 4;
    reply[1] = 94;
    reply[2] = nodeid;
    reply[3] = testtrue;
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//this should copy the thread information from the file to the thread
//the packet should ocntain the file name as well as the index of the thread to be restored
//note that it will check the file size to ensure that is sufficient information for the thread to be restored
void reply_thread_state_restore_fromfile(uint8_t * receivebuffer)
{
    uint8_t i;
    uint8_t testtrue = 0;

    receivebuffer[receivebuffer[0]] = '\0';
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state != STATE_NULL)
        {
            if (superstring
                ((char *)thread_table[i].threadName,
                 (char *)&receivebuffer[5]) == 0)
            {
                testtrue = i;
                break;
            }
        }
    }
    i = receivebuffer[3];
    receivebuffer[5 + i] = '/';
    if (testtrue != 0)
    {
        thread_state_restore(testtrue, (uint8_t *) & receivebuffer[5 + i]);
    }
    reply[0] = 4;
    reply[1] = 95;
    reply[2] = nodeid;
    reply[3] = testtrue;
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------
void reply_ls_networkname()
{
    //   uint16_t temp2; 
    uint8_t temp = mystrlen(networkid);
    uint16_t randomsleep;

    reply[0] = temp + 3;
    reply[1] = 101;
    //   foobar(); 
    reply[2] = nodeid;
    mystrncpy((char *)&reply[3], networkid, temp);
    randomsleep = getRandomNumber() % 400;
    sleepThread(randomsleep);
    // for (temp2=0;temp2<9000;temp2++)
    // {
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    // sleepThread(20);
    // }
    //   printStringN(reply,32); 
    //printString("hello,reply2!\n\0");
}

//-------------------------------------------------------------------------
void reply_ls_nodename(uint8_t * receivebuffer)
{
    uint8_t temp = receivebuffer[0] - 3;
    uint16_t randomsleep;

    if (mystrncmp((char *)receivebuffer, 3, (char *)networkid, 0, temp) == 0)
    {
        temp = mystrlen(filenameid);
        reply[0] = temp + 3;
        reply[1] = 102;
        reply[2] = nodeid;
        mystrncpy((char *)&reply[3], filenameid, temp);
        randomsleep = getRandomNumber() % 400;
        sleepThread(randomsleep);
        StandardSocketSend(0xefef, 0xffff, 32, reply);
        //printStringN(reply,32); 
    }
}

//Get the block information of sub directory and files using simplfied version 
void reply_ls_short(uint8_t block)
{
    //Actually this is not used because to ensure that the system is simple, we only use the long version 
    return;
}

//Get the block information of sub directory and files using long version 
void reply_ls_long(uint8_t block)
{
    uint8_t blockindex;
    uint8_t childblock;
    uint8_t seq = 0;

    if (openedfile != NULL)
    {
        //fclose2( openedfile );
        openedfile = NULL;
    }
    for (blockindex = 0; blockindex < 10; blockindex++)
    {
        childblock = fsread8uint(block, DIR_ADDRSUBOFFSET + blockindex);
        if (childblock == 0)
        {
            continue;
        }
        reply[0] = 32;
        reply[1] = 104;
        reply[2] = nodeid;
        reply[3] = childblock;
        reply[4] = seq;
        seq++;
        finfonode((char *)&reply[5], (int)childblock);
	    sleepThread(20);
	
        StandardSocketSend(0xefef, 0xffff, 32, reply);
        //     printStringN(reply,32); 
    }
}

//-------------------------------------------------------------------------
void reply_mkdir(uint8_t * receivebuffer)
{
    uint8_t blockid;
    char dirname[13];
    uint8_t newblockid;

    blockid = receivebuffer[3];
    mystrncpy(dirname, (char *)&receivebuffer[4], receivebuffer[0] - 4);
    dirname[receivebuffer[0] - 4] = '\0';
    newblockid = createDir(dirname, (int)blockid);
    reply[0] = 4;
    reply[1] = 141;
    reply[2] = nodeid;
    reply[3] = newblockid;
	 sleepThread(20);
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    // printStringN(reply,32);      
}

//-------------------------------------------------------------------------
void reply_cp_fromPCtoNode_type1(uint8_t * receivebuffer)
{
    uint8_t blockid;
    char filename[13];
    int fid;
    uint8_t newblockid;
      
    releaseAllFid();
    blockid = receivebuffer[3];
    mystrncpy(filename, (char *)&receivebuffer[4], receivebuffer[0] - 4);
    filename[receivebuffer[0] - 4] = '\0';
    newblockid = createFileFromDirectory(filename, (int)blockid);
    chunksequencenum = 0;
    expectNextNum = 1;
    reply[0] = 4;
    reply[1] = 121;
    reply[2] = nodeid;
    reply[3] = newblockid;
    fid = getFreeFid();
    openFile(newblockid, fid, 2);
    openedfile = &fidtable[fid];
	 sleepThread(20);
    // printStringN(reply,32);      
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------
void reply_cp_fromPCtoNode_type2(uint8_t * receivebuffer)
{
    uint16_t SyncNum;

    if (receivebuffer[3] == 0xff)
    {
        //now this is a sync
        SyncNum = ((uint16_t) receivebuffer[4]) * 256 + receivebuffer[5];
        if (SyncNum > expectNextNum - 1)
        {
            reply[0] = 7;
            reply[1] = 122;
            reply[2] = nodeid;
            reply[3] = 15;
            reply[4] = 15;
            reply[5] = (expectNextNum - 1) / 256;
            reply[6] = (expectNextNum - 1) % 256;
            StandardSocketSend(0xefef, 0xffff, 32, reply);
        }
        else
        {
            reply[0] = 7;
            reply[1] = 122;
            reply[2] = nodeid;
            reply[3] = 15;
            reply[4] = 15;
            reply[5] = 0;
            reply[6] = 0;
            StandardSocketSend(0xefef, 0xffff, 32, reply);
        }
        return;
    }
    chunksequencenum = ((uint16_t) receivebuffer[4]) * 256 + receivebuffer[5];
    if (chunksequencenum != expectNextNum)
    {
        return;
    }
    fwrite2(openedfile, &receivebuffer[7], receivebuffer[6]);
    fseek2(openedfile, receivebuffer[6], 1);
    expectNextNum++;
    // printStringN(reply,32);      
    //StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------
void reply_cp_fromPCtoNode_type3()
{
    fclose2(openedfile);
    // printStringN(reply,32);      
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------
void reply_touch(uint8_t * receivebuffer)
{
    uint8_t blockid;
    char filename[13];
    uint8_t newblockid;

    blockid = receivebuffer[3];
    mystrncpy(filename, (char *)&receivebuffer[4], receivebuffer[0] - 4);
    filename[receivebuffer[0] - 4] = '\0';
    newblockid = createFileFromDirectory(filename, (int)blockid);
    reply[0] = 4;
    reply[1] = 151;
    reply[2] = nodeid;
    reply[3] = newblockid;
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //printStringN(reply,32);       
}

//-------------------------------------------------------------------------
void reply_fromNodeToPC_type1(uint8_t * receivebuffer)
{
    uint8_t blockid;
    char filename[13];
    int fid;
    uint16_t filesize;
    uint16_t round;
    uint16_t remain;
    uint16_t i;
    uint8_t newblockid;
    uint8_t temp1, temp2;

    blockid = receivebuffer[3];
    mystrncpy(filename, (char *)&receivebuffer[4], receivebuffer[0] - 4);
    filename[receivebuffer[0] - 4] = '\0';
    newblockid = existBlockAddr(filename, (int)blockid);
    reply[1] = 124;
    reply[2] = nodeid;
    fid = getFreeFid();
    openFile(newblockid, fid, 1);
    fp = &fidtable[fid];
    filesize = fp->size;
    temp1 = filesize / 256;
    temp2 = filesize % 256;
    reply[3] = temp1;
    reply[4] = temp2;
    //disabletracingfunction();
    //send out the size 
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    
    //    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //sleepThread( 100 );
    reply[3] = newblockid;
    round = filesize / 20;
    remain = filesize % 20;
    for (i = 0; i < round; i++)
    {
        fread2(fp, &reply[6], 20);
        fseek2(fp, 20, 1);
        reply[0] = 26;
        temp1 = i / 256;
        temp2 = i % 256;
        reply[4] = temp1;
        reply[5] = temp2;
        //printStringN(reply,32);                   
        // sleepThread( 100 );
        StandardSocketSend(0xefef, 0xffff, 32, reply);
    }
    if (remain > 0)
    {
        fread2(fp, &reply[6], remain);
        reply[0] = (uint8_t) remain + 6;
        reply[4] = round / 256;
        reply[5] = round % 256;
        round++;
        //printStringN(reply,32);
        //sleepThread( 25 );
        StandardSocketSend(0xefef, 0xffff, 32, reply);
    }
    temp1 = round / 256;
    temp2 = round % 256;
    //terminate signal  is here!
    reply[0] = 7;
    reply[3] = 0xef;
    reply[4] = 0xef;
    reply[5] = temp1;
    reply[6] = temp2;
    //ephread( 100 );
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //sleepThread( 100 );
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //sleepThread( 100 );
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //sleepThread( 100 );
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //do not close now!
    //       fclose2(fp); 
    
    //enabletracingfunction();
}

//-------------------------------------------------------------------------
void reply_rm(uint8_t * receivebuffer)
{
    uint8_t blockid;
    char filename[13];
    uint8_t newblockid;

    blockid = receivebuffer[3];
    mystrncpy(filename, (char *)&receivebuffer[4], receivebuffer[0] - 4);
    filename[receivebuffer[0] - 4] = '\0';
    newblockid = existBlockAddr(filename, (int)blockid);
    if (newblockid > 0)
    {
        deleteNode(newblockid);
    }
    reply[0] = 4;
    reply[1] = 161;
    reply[2] = nodeid;
    reply[3] = newblockid;
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    //printStringN(reply,32);       
}

//get patch request
void reply_fromNodeToPC_type2(uint8_t * receivebuffer)
{
    uint16_t pos;
    //disabletracingfunction();

    pos = ((uint16_t) receivebuffer[4]) * 256 + receivebuffer[5];
    reply[0] = 26;
    reply[1] = 125;
    reply[2] = nodeid;
    reply[3] = pos / 256;
    reply[4] = pos % 256;
    fseek2(fp, (int32_t) pos, 0);
    fread2(fp, &reply[6], 20);
    StandardSocketSend(0xefef, 0xffff, 32, reply);
    
   // enabletracingfunction();
}

//-------------------------------------------------------------------------
//possible bug here 
void reply126(uint8_t * receivebuffer)
{
    fclose2(fp);
}

//-------------------------------------------------------------------------
void reply_ps(uint8_t * receivebuffer)
{
    uint8_t i, len;

    //reply[0] = 15;
    reply[1] = 171;
    reply[2] = nodeid;
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state != STATE_NULL)
        {
            len = mystrlen((char *)thread_table[i].threadName);
            reply[0] = len + 4;
            reply[3] = thread_table[i].state;
            //if this is a break thread, then fetch the real number
            if (reply[3] == STATE_BREAK)
            {
                uint8_t *tempptr = (uint8_t *) thread_table[i].sp + 47;
                uint16_t addr =
                    (((uint16_t) (*tempptr)) * 256 + *(tempptr + 1) - 3);
                reply[4] = addr / 256;
                reply[5] = addr % 256;
                reply[0] += 2;
                mystrncpy((char *)&reply[6],
                          (char *)thread_table[i].threadName, len);
            }
            else
            {
                mystrncpy((char *)&reply[4],
                          (char *)thread_table[i].threadName, len);
            }
            StandardSocketSend(0xefef, 0xffff, 32, reply);
        }
    }
}

//-------------------------------------------------------------------------
void reply_killthread(uint8_t * receivebuffer)
{
    uint8_t i;
    uint8_t testtrue = 0;
    uint8_t *start, *end;
    uint8_t index;

    index = 0; 
    start = NULL;
    end = NULL;
    receivebuffer[receivebuffer[0]] = '\0';
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state != STATE_NULL)
        {
            if (superstring
                ((char *)thread_table[i].threadName,
                 (char *)&receivebuffer[3]) == 0)
            {
                start = (uint8_t *) thread_table[i].ramstart;
                end = (uint8_t *) thread_table[i].ramend;
                index = i;
                thread_table[i].state = STATE_NULL;
                testtrue = 1;
            }
        }
    }
    if (testtrue == 1)
    {
        deleteThreadRegistrationInReceiverHandles(start, end);
        if (thread_table[index].thread_clear_function != NULL)
        {
            (*thread_table[index].thread_clear_function) ();
            thread_table[index].thread_clear_function = NULL;
        }
        cbi(MCUCR, SE);
        if (timercallback[index] != NULL)
        {
            timercallback[index] = NULL;
            GenericTimerStop(index + 12);
        }
        /*for ( i = 0; i < RECEIVE_HANDLE_NUM; i ++ )
           { if (( receivehandles[ i ].handlevalid == 1 ) && ( receivehandles[ i ].dataReady <= end ) && ( receivehandles[ i ].dataReady >= start )) {
           receivehandles[ i ].handlevalid = 0;
           receivehandles[ i ].port = 0;     
           }
           }
         */
    }
    reply[0] = 4;
    reply[1] = 232;
    reply[2] = nodeid;
    reply[3] = testtrue;
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------
void reply_du(uint8_t * receivebuffer)
{
    int e1, e2, e3;

    e1 = fcheckFSAllocation();
    e2 = fcheckStorageAllocation();
    e3 = 0;
    if (e2 == 256)
    {
        e2 = 128;
        e3 = 128;
    }
    reply[0] = 6;
    reply[1] = 211;
    reply[2] = nodeid;
    reply[3] = e1;
    reply[4] = e2;
    reply[5] = e3;
    //printStringN(reply,32);       
    StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------
void reply_search(uint8_t * receivebuffer)
{
    char filename[13];
    uint8_t searchresults[20];
    uint8_t numofsearch;
    uint8_t i;

    mystrncpy(filename, (char *)&receivebuffer[3], receivebuffer[0] - 3);
    filename[receivebuffer[0] - 3] = '\0';
    fsearch(searchresults, &numofsearch, filename);
    for (i = 0; i < numofsearch; i++)
    {
        fdirnode((char *)&reply[3], 29, searchresults[i]);
        reply[0] = mystrlen((char *)&reply[3]) + 3;
        reply[1] = 221;
        reply[2] = nodeid;
        //printStringN(reply,32);       
        StandardSocketSend(0xefef, 0xffff, 32, reply);
    }
}

//-------------------------------------------------------------------------
void reply_formatSystem(uint8_t * receivebuffer)
{
    formatSystem();
    buildRootNode();
    
}

//-------------------------------------------------------------------------
void reply_nodeRestart(uint8_t * receivebuffer)
{
    avr_resetNode();
}

//-------------------------------------------------------------------------
void WakeupMe()
{
    thisthread->state = STATE_ACTIVE;
    postNewThreadTask();
}

//-------------------------------------------------------------------------
void InitShell()
{
    IncomingLength = 0;
    nodeid = CURRENT_NODE_ID;
    registerEvent(0xfefe, 64, &IncomingLength, IncomingMsg, PacketInfo,
                  WakeupMe);
    init_breakpoint_table();
}

//This is the stub function of the shell thread that is used to handle incoming packets 
void ShellThread()
{
    while (1)
    {
        if (IncomingLength > 0)
        {
            commandHandle(IncomingMsg, IncomingLength);
            IncomingLength = 0;
        }
        else
        {
            
            current_thread->state = STATE_SLEEP;
            thisthread = current_thread;
            thread_yield();
        }
    }
}

//-------------------------------------------------------------------------
void commandHandle(uint8_t * receivebuffer, uint8_t total)
{
    uint8_t commandtype;

    if (!((receivebuffer[2] == 0) || (receivebuffer[2] == nodeid)))
    {
        return;
    }
    commandtype = receivebuffer[1];
    switch (commandtype)
    {
        //reply_devicehandling handles the device driver issues 
    
    case 91:
        reply_debugging_print(receivebuffer);
        break;
#ifdef PLATFORM_AVR
    case 92:
        reply_debugging_insert_avr_breakpoint(receivebuffer);
        break;
    case 93:
        reply_debugging_remove_avr_breakpoint(receivebuffer);
        break;
#endif
    case 94:
        reply_thread_state_snapshot_tofile(receivebuffer);
        break;
    case 95:
        reply_thread_state_restore_fromfile(receivebuffer);
        break;
    case 96:
        reply_debugging_set(receivebuffer);
        break;
    case 101:
        reply_ls_networkname();
        break;
    case 102:
        reply_ls_nodename(receivebuffer);
        break;
    case 103:
        reply_ls_long(receivebuffer[3]);
        break;
    case 104:
        reply_ls_long(receivebuffer[3]);
        break;
    case 121:
        reply_cp_fromPCtoNode_type1(receivebuffer);
        break;
    case 122:
        reply_cp_fromPCtoNode_type2(receivebuffer);
        break;
    case 123:
        reply_cp_fromPCtoNode_type3();
        break;
    case 124:
        reply_fromNodeToPC_type1(receivebuffer);
        break;
    case 125:
        reply_fromNodeToPC_type2(receivebuffer);
        break;
    case 141:
        reply_mkdir(receivebuffer);
        break;
    case 151:
        reply_touch(receivebuffer);
        break;
    case 161:
        reply_rm(receivebuffer);
        break;
    case 171:
        reply_ps(receivebuffer);
        break;
    
    case 211:
        reply_du(receivebuffer);
        break;
    case 221:
        reply_search(receivebuffer);
        break;
    
    case 233:
        reply_formatSystem(receivebuffer);
        break;
    case 242:
        reply_nodeRestart(receivebuffer);
        break;
    }
}
