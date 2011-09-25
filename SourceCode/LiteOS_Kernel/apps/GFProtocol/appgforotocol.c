#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libeeprom.h"
#include "../../libraries/libadc.h"
 
#include "../../types/types.h"



uint16_t thisnodeid;

uint8_t protocolbuffer[200];

//Pointer to the current thread data structure. Useful for waking up the current thread. 
lib_thread **thiscurrentthread;
lib_thread *currentthread;
uint16_t packetslocal;
uint16_t packetsremote;
uint16_t packetsdeliver;
uint16_t packetsdrop; 

//The neighbor table
typedef struct
{
    uint16_t nodeid;
    uint8_t local_x;
    uint8_t local_y;
    uint8_t valid;
} nbr_table_entry;

//The maximum size of the neighbor table 
#define NBR_SIZE 7
//The current number of neighbors
uint8_t numOfNeighbors;

//Neighbor table 
static nbr_table_entry nbtTable[NBR_SIZE];

//Find out whether there is packet pending or not
static uint8_t iswakenup;

//Every five seconds, send out neighbor update message (beacons)
#define NBR_UPDATE_CYCLE 5000


//This function initlizes the neighhood table 
void initTable()
{
    int i;

    for (i = 0; i < NBR_SIZE; i++)
    {
        nbtTable[i].valid = 0;
    }
    numOfNeighbors = 0;
    iswakenup = 0;
}

//-------------------------------------------------------------------------
static uint8_t incomingMsg[32];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[4];

//Handles incoming data msg. This usually wakesup the current thread 
void wakeupMeOnDataMsg()
{
    //(*thiscurrentthread)->state = STATE_ACTIVE;
    currentthread->state = STATE_ACTIVE;
    iswakenup = 1;
    syscall_post_thread_task();
}

//Registers the data packet for receving incoming packets. This enables receving both remote and local data packets
void registerDataPacket()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

    //Finds the radio handle, so that when this code is ported to be externally used, it will be obtained via a system call 
    radiohandleaddr = lib_get_current_radio_receive_handle_addr();
    incomingDataLength = 0;
    //set up the radiohandleaddr data structures
    radiohandleaddr->port = 10;
    radiohandleaddr->maxLength = 32;
    radiohandleaddr->dataReady = &incomingDataLength;
    radiohandleaddr->data = incomingMsg;
    radiohandleaddr->packetinfo = incomingPacketInfo;
    radiohandleaddr->handlefunc = wakeupMeOnDataMsg;
    //close the interrupt     
    currentatomic = _atomic_start();
    iswakenup = 0;
    //call the radio handle set to store the data structure into the handle vectors 
    getaddrfp();
    //syscall_registerEvent();
    //open the interrupt 
    _atomic_end(currentatomic);
    return;
}

//-------------------------------------------------------------------------
static uint8_t incomingNbrMsg[16];
static uint8_t incomingNbrLength;
static uint8_t incomingNbrPacketInfo[4];

//handles the incoming message from a neighbor. This usually scanns the neighborhood table and sets up stuff 
void wakeupMeOnNbrMsg()
{
    int i;
    uint8_t neighbornodeid;

    for (i = 0; i < numOfNeighbors; i++)
    {
        if ((nbtTable[i].valid == 1) && (nbtTable[i].nodeid ==
                                         incomingNbrMsg[0]))
        {
            return;
        }
    }
    neighbornodeid = incomingNbrMsg[0];
    if ((neighbornodeid != thisnodeid - 1) && (neighbornodeid != thisnodeid + 1))
    {
        return;
    }
    nbtTable[numOfNeighbors].nodeid = incomingNbrMsg[0];
    nbtTable[numOfNeighbors].local_x = incomingNbrMsg[1];
    nbtTable[numOfNeighbors].local_y = incomingNbrMsg[2];
    nbtTable[numOfNeighbors].valid = 1;
    numOfNeighbors++;
    return;
}

//-------------------------------------------------------------------------
void registerNeighborhoodPacket()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

    radiohandleaddr = lib_get_current_radio_receive_handle_addr();
    incomingNbrLength = 0;
    //set up the radiohandleaddr data structures
    radiohandleaddr->port = 11;
    radiohandleaddr->maxLength = 16;
    radiohandleaddr->dataReady = &incomingNbrLength;
    radiohandleaddr->data = incomingNbrMsg;
    radiohandleaddr->packetinfo = incomingNbrPacketInfo;
    radiohandleaddr->handlefunc = wakeupMeOnNbrMsg;
    //close the interrupt     
    currentatomic = _atomic_start();
    //call the radio handle set to store the data structure into the handle vectors 
    getaddrfp();
    //open the interrupt 
    _atomic_end(currentatomic);
    return;
}

//-------------------------------------------------------------------------
static uint8_t nbrUpdateMsg[16];
void setNeighborMsg()
{
    nbrUpdateMsg[0] = thisnodeid;
    nbrUpdateMsg[1] = thisnodeid % 256;
    nbrUpdateMsg[2] = thisnodeid / 256;
    return;
}

//This is the entry point. It broadcasts neighbor exchanges perodically, or handles incoming packets (non-update neighbor)
void protocol()
{
    uint8_t destx, desty, from, to;
    uint8_t i;
    uint16_t maxdist;
    uint8_t index;
    uint8_t portnum;

    packetsremote = packetslocal = packetsdeliver = packetsdrop = 0; 
    index = 100;
    initTable();
    thisnodeid = lib_get_node_id();
    //register both the negibhorhood update and the message incoming 
    thiscurrentthread = lib_get_current_thread();
    currentthread = *thiscurrentthread;
    registerDataPacket();
    registerNeighborhoodPacket();
    while (1)
    {
        if (iswakenup == 1)
        {
            //If event occurs then skip the current sleep 
        }
        else
        {
            lib_sleep_thread(NBR_UPDATE_CYCLE);
        }
        //wakes up and check the new data message?
        if (iswakenup == 1)
        {
            //handles the data message by restructing it and broadcast it based on the neighborhood table information 
            //sends out the new modified message to the next hop and waits for its complete
            //This must be a data packet, whether remote or local!!!
            iswakenup = 0;
            destx = incomingMsg[0];
            desty = incomingMsg[1];
            from = incomingMsg[2];
            to = incomingMsg[3];
            portnum = incomingMsg[4];
            if (to == thisnodeid)
            {
                //StandardSocketSend( portnum, 0, incomingDataLength, incomingMsg );
                lib_radio_send_msg(portnum, 0, incomingDataLength, incomingMsg);
                packetsdeliver++;
            }
            else
            {
                index = 100;
                maxdist = 32000;
				{
					int tempx, tempy; 
					tempx= thisnodeid%256;
					tempy = thisnodeid/256; 
					maxdist =
                            (tempx -
                             destx) * (tempx - destx) +
                            (tempy -
                             desty) * (tempy - desty);
					
				}
                for (i = 0; i < numOfNeighbors; i++)
                {
                    if (nbtTable[i].valid == 1)
                    {
                        uint16_t tempdist;

                        tempdist =
                            (nbtTable[i].local_x -
                             destx) * (nbtTable[i].local_x - destx) +
                            (nbtTable[i].local_y -
                             desty) * (nbtTable[i].local_y - desty);
                        if (tempdist < maxdist)
                        {
                            index = i;
                            maxdist = tempdist;
                        }
                    }
                }
                if (index != 100)
                {
					lib_get_radio_lock();
					lib_radio_set_channel(21); 
                    lib_radio_send_msg(10, nbtTable[index].nodeid, 				
                    incomingDataLength, incomingMsg);
					lib_release_radio_lock();
                    lib_green_toggle();
                    incomingDataLength = 0;
                    if (from == thisnodeid)
                    {
                        packetslocal++;
                    }
                    else
                    {
                        packetsremote++;
                    }
                }
				
				else 
				{
					packetsdrop++; 
					
				}
            }
        }
        else
        {
            //sends out hte neighborhood update message in case the previous action has used up the time slot anyway. The next
            //sleep will be another NBR_UPDATE_CYCLE 
            setNeighborMsg();
            lib_yellow_toggle();
			lib_get_radio_lock();
			lib_radio_set_channel(21); 
            lib_radio_send_msg(11, 0xffff, 16, nbrUpdateMsg);
			lib_release_radio_lock(); 
			
        }
    }
}
