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

uint8_t sndrecvbuffer[256];
uint16_t pkt_id = 0;
lib_thread **this_cur_thread;
lib_thread *cur_thread;

static uint8_t iswakeup;
static uint8_t incomingMsg[32];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[4];

static LIB_MYFILE *recv_msg_fp;

char sndMsg[20] = "00abcdefghijklmnopq\0";

void Send ()
{
	lib_radio_set_channel(16);
	lib_radio_set_power(3);
	pkt_id = 0;
	while (1)
	{
		char t[2];
		uint16_t x = pkt_id;
		t[0] = x%256;
		x /= 256;
		t[1] = x%256;
		lib_mystrncpy(sndMsg, t, 2);
		//lib_get_radio_lock();
		lib_radio_send_msg(10, 2, 20, (uint8_t *)sndMsg);
		//lib_release_radio_lock(); 
		lib_green_toggle();
		lib_sleep_thread(100);
		++pkt_id;
	}
}

void wakeupTrigger ()
{
	cur_thread->state = STATE_ACTIVE;
	iswakeup = 1;
	syscall_post_thread_task();
}

void Listen()
{
	_atomic_t cur_atomic;
	radiohandletype *rhandleaddr;
	void (*getaddrfp) (void) = (void (*) (void)) REGISTER_RADIO_RECEIVE_EVENT;

	rhandleaddr = lib_get_current_radio_receive_handle_addr();
	rhandleaddr->port = 10;
	rhandleaddr->maxLength = 32;
	rhandleaddr->dataReady = &incomingDataLength;
	rhandleaddr->data = incomingMsg;
	rhandleaddr->packetinfo = incomingPacketInfo;
	rhandleaddr->handlefunc = wakeupTrigger;

	cur_atomic = _atomic_start();
	iswakeup = 0;
	getaddrfp();

	_atomic_end(cur_atomic);
	return;
}

void sndrecv()
{
	// Start the listening thread to receive the packets.
	uint16_t nodeID = lib_get_node_id();
	
	// If it is node#1 (sender), it needs to send packets.
	if (nodeID == 1) 
	{
		Send();
	}
	else
	{
		//lib_yellow_on();
		this_cur_thread = lib_get_current_thread();
		cur_thread = *this_cur_thread;
		Listen();
		recv_msg_fp = lib_mfopen("/val", "w");	

		while (1)
		{
			if (iswakeup == 1)
			{
				
				lib_mfwrite(recv_msg_fp, incomingPacketInfo, 2); //LQI & RSSI
				lib_mfwrite(recv_msg_fp, incomingMsg, 2); //paketID			
				lib_yellow_toggle();
				iswakeup = 0;							
			}
			else
			{
				lib_hibernate_thread();
			}
		}
	}
}