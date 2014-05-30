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
#include "math.h"

uint16_t thisnodeid;
uint32_t counter;
uint16_t active;
uint32_t packet_id;

uint8_t protocolbuffer[300];

lib_thread **thiscurrentthread;
lib_thread *currentthread;

uint8_t sendingMsg[32];
uint8_t incomingMsg[32];
uint8_t incomingDataLength;
uint8_t incomingPacketInfo[4];

float theta[4];
float err[6];
float y[6];
float fai[6][4];
float eps = 1e-3;
float tmp0[4][4];
float tmp1[4][4];
float tmp2[4];
float tmp3[4];

float pvalue;
float rssi;
float lqi;
float prr;


void intermediate0(int Num) {
	tmp0[0][0] = (2*pow(fai[0][0],2)+2*pow(fai[1][0],2)+2*pow(fai[2][0],2)+2*pow(fai[3][0],2)+2*pow(fai[4][0],2)+2*pow(fai[5][0],2))/Num+0.001;
	tmp0[0][1] = (2*fai[0][0]*fai[0][1]+2*fai[1][0]*fai[1][1]+2*fai[2][0]*fai[2][1]+2*fai[3][0]*fai[3][1]+2*fai[4][0]*fai[4][1]+2*fai[5][0]*fai[5][1])/Num;
	tmp0[0][2] = (2*fai[0][0]*fai[0][2]+2*fai[1][0]*fai[1][2]+2*fai[2][0]*fai[2][2]+2*fai[3][0]*fai[3][2]+2*fai[4][0]*fai[4][2]+2*fai[5][0]*fai[5][2])/Num;
	tmp0[0][3] = (2*fai[0][0]*fai[0][3]+2*fai[1][0]*fai[1][3]+2*fai[2][0]*fai[2][3]+2*fai[3][0]*fai[3][3]+2*fai[4][0]*fai[4][3]+2*fai[5][0]*fai[5][3])/Num;
	tmp0[1][0] = (2*fai[0][0]*fai[0][1]+2*fai[1][0]*fai[1][1]+2*fai[2][0]*fai[2][1]+2*fai[3][0]*fai[3][1]+2*fai[4][0]*fai[4][1]+2*fai[5][0]*fai[5][1])/Num;
	tmp0[1][1] = (2*pow(fai[0][1], 2)+2*pow(fai[1][1],2)+2*pow(fai[2][1],2)+2*pow(fai[3][1],2)+2*pow(fai[4][1],2)+2*pow(fai[5][1],2))/Num+0.001;
	tmp0[1][2] = (2*fai[0][1]*fai[0][2]+2*fai[1][1]*fai[1][2]+2*fai[2][1]*fai[2][2]+2*fai[3][1]*fai[3][2]+2*fai[4][1]*fai[4][2]+2*fai[5][1]*fai[5][2])/Num;
	tmp0[1][3] = (2*fai[0][1]*fai[0][3]+2*fai[1][1]*fai[1][3]+2*fai[2][1]*fai[2][3]+2*fai[3][1]*fai[3][3]+2*fai[4][1]*fai[4][3]+2*fai[5][1]*fai[5][3])/Num;
	tmp0[2][0] = (2*fai[0][0]*fai[0][2]+2*fai[1][0]*fai[1][2]+2*fai[2][0]*fai[2][2]+2*fai[3][0]*fai[3][2]+2*fai[4][0]*fai[4][2]+2*fai[5][0]*fai[5][2])/Num;
	tmp0[2][1] = (2*fai[0][1]*fai[0][2]+2*fai[1][1]*fai[1][2]+2*fai[2][1]*fai[2][2]+2*fai[3][1]*fai[3][2]+2*fai[4][1]*fai[4][2]+2*fai[5][1]*fai[5][2])/Num;
	tmp0[2][2] = (2*pow(fai[0][2],2)+2*pow(fai[1][2],2)+2*pow(fai[2][2],2)+2*pow(fai[3][2],2)+2*pow(fai[4][2],2)+2*pow(fai[5][2],2))/Num+0.001;
	tmp0[2][3] = (2*fai[0][2]*fai[0][3]+2*fai[1][2]*fai[1][3]+2*fai[2][2]*fai[2][3]+2*fai[3][2]*fai[3][3]+2*fai[4][2]*fai[4][3]+2*fai[5][2]*fai[5][3])/Num;
	tmp0[3][0] = (2*fai[0][0]*fai[0][3]+2*fai[1][0]*fai[1][3]+2*fai[2][0]*fai[2][3]+2*fai[3][0]*fai[3][3]+2*fai[4][0]*fai[4][3]+2*fai[5][0]*fai[5][3])/Num;
	tmp0[3][1] = (2*fai[0][1]*fai[0][3]+2*fai[1][1]*fai[1][3]+2*fai[2][1]*fai[2][3]+2*fai[3][1]*fai[3][3]+2*fai[4][1]*fai[4][3]+2*fai[5][1]*fai[5][3])/Num;
	tmp0[3][2] = (2*fai[0][2]*fai[0][3]+2*fai[1][2]*fai[1][3]+2*fai[2][2]*fai[2][3]+2*fai[3][2]*fai[3][3]+2*fai[4][2]*fai[4][3]+2*fai[5][2]*fai[5][3])/Num;
	tmp0[3][3] = (2*pow(fai[0][3],2)+2*pow(fai[1][3],2)+2*pow(fai[2][3],2)+2*pow(fai[3][3],2)+2*pow(fai[4][3],2)+2*pow(fai[5][3],2))/Num+0.001;
}

void intermediate1() {
	tmp1[0][0] = (tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[3][1]*tmp0[1][3]*tmp0[2][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[0][1] = -(tmp0[0][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][1]*tmp0[2][3]*tmp0[3][2]-tmp0[2][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][1]*tmp0[0][3]*tmp0[3][2]+tmp0[3][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][1]*tmp0[0][3]*tmp0[2][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[0][2] = (tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[3][1]*tmp0[0][3]*tmp0[1][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[0][3] = -(tmp0[0][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][1]*tmp0[0][3]*tmp0[1][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[1][0] = -(tmp0[1][0]*tmp0[2][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][3]*tmp0[3][2]+tmp0[3][0]*tmp0[1][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][3]*tmp0[2][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[1][1] = (tmp0[0][0]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[2][3]*tmp0[3][2]-tmp0[2][0]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[0][3]*tmp0[3][2]+tmp0[3][0]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[0][3]*tmp0[2][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[1][2] = -(tmp0[0][0]*tmp0[1][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][3]*tmp0[3][2]-tmp0[1][0]*tmp0[0][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][3]*tmp0[3][2]+tmp0[3][0]*tmp0[0][2]*tmp0[1][3]-tmp0[3][0]*tmp0[0][3]*tmp0[1][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[1][3] = (tmp0[0][0]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[0][3]*tmp0[1][2])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[2][0] = (tmp0[1][0]*tmp0[2][1]*tmp0[3][3]-tmp0[1][0]*tmp0[2][3]*tmp0[3][1]-tmp0[2][0]*tmp0[1][1]*tmp0[3][3]+tmp0[2][0]*tmp0[1][3]*tmp0[3][1]+tmp0[3][0]*tmp0[1][1]*tmp0[2][3]-tmp0[3][0]*tmp0[1][3]*tmp0[2][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[2][1] = -(tmp0[0][0]*tmp0[2][1]*tmp0[3][3]-tmp0[0][0]*tmp0[2][3]*tmp0[3][1]-tmp0[2][0]*tmp0[0][1]*tmp0[3][3]+tmp0[2][0]*tmp0[0][3]*tmp0[3][1]+tmp0[3][0]*tmp0[0][1]*tmp0[2][3]-tmp0[3][0]*tmp0[0][3]*tmp0[2][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[2][2] = (tmp0[0][0]*tmp0[1][1]*tmp0[3][3]-tmp0[0][0]*tmp0[1][3]*tmp0[3][1]-tmp0[1][0]*tmp0[0][1]*tmp0[3][3]+tmp0[1][0]*tmp0[0][3]*tmp0[3][1]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]-tmp0[3][0]*tmp0[0][3]*tmp0[1][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[2][3] = -(tmp0[0][0]*tmp0[1][1]*tmp0[2][3]-tmp0[0][0]*tmp0[1][3]*tmp0[2][1]-tmp0[1][0]*tmp0[0][1]*tmp0[2][3]+tmp0[1][0]*tmp0[0][3]*tmp0[2][1]+tmp0[2][0]*tmp0[0][1]*tmp0[1][3]-tmp0[2][0]*tmp0[0][3]*tmp0[1][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[3][0] = -(tmp0[1][0]*tmp0[2][1]*tmp0[3][2]-tmp0[1][0]*tmp0[2][2]*tmp0[3][1]-tmp0[2][0]*tmp0[1][1]*tmp0[3][2]+tmp0[2][0]*tmp0[1][2]*tmp0[3][1]+tmp0[3][0]*tmp0[1][1]*tmp0[2][2]-tmp0[3][0]*tmp0[1][2]*tmp0[2][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[3][1] = (tmp0[0][0]*tmp0[2][1]*tmp0[3][2]-tmp0[0][0]*tmp0[2][2]*tmp0[3][1]-tmp0[2][0]*tmp0[0][1]*tmp0[3][2]+tmp0[2][0]*tmp0[0][2]*tmp0[3][1]+tmp0[3][0]*tmp0[0][1]*tmp0[2][2]-tmp0[3][0]*tmp0[0][2]*tmp0[2][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[3][2] = -(tmp0[0][0]*tmp0[1][1]*tmp0[3][2]-tmp0[0][0]*tmp0[1][2]*tmp0[3][1]-tmp0[1][0]*tmp0[0][1]*tmp0[3][2]+tmp0[1][0]*tmp0[0][2]*tmp0[3][1]+tmp0[3][0]*tmp0[0][1]*tmp0[1][2]-tmp0[3][0]*tmp0[0][2]*tmp0[1][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);
	tmp1[3][3] = (tmp0[0][0]*tmp0[1][1]*tmp0[2][2]-tmp0[0][0]*tmp0[1][2]*tmp0[2][1]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]+tmp0[1][0]*tmp0[0][2]*tmp0[2][1]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]-tmp0[2][0]*tmp0[0][2]*tmp0[1][1])/(tmp0[0][0]*tmp0[1][1]*tmp0[2][2]*tmp0[3][3]-tmp0[0][0]*tmp0[1][1]*tmp0[2][3]*tmp0[3][2]-tmp0[0][0]*tmp0[2][1]*tmp0[1][2]*tmp0[3][3]+tmp0[0][0]*tmp0[2][1]*tmp0[1][3]*tmp0[3][2]+tmp0[0][0]*tmp0[3][1]*tmp0[1][2]*tmp0[2][3]-tmp0[0][0]*tmp0[3][1]*tmp0[1][3]*tmp0[2][2]-tmp0[1][0]*tmp0[0][1]*tmp0[2][2]*tmp0[3][3]+tmp0[1][0]*tmp0[0][1]*tmp0[2][3]*tmp0[3][2]+tmp0[1][0]*tmp0[2][1]*tmp0[0][2]*tmp0[3][3]-tmp0[1][0]*tmp0[2][1]*tmp0[0][3]*tmp0[3][2]-tmp0[1][0]*tmp0[3][1]*tmp0[0][2]*tmp0[2][3]+tmp0[1][0]*tmp0[3][1]*tmp0[0][3]*tmp0[2][2]+tmp0[2][0]*tmp0[0][1]*tmp0[1][2]*tmp0[3][3]-tmp0[2][0]*tmp0[0][1]*tmp0[1][3]*tmp0[3][2]-tmp0[2][0]*tmp0[1][1]*tmp0[0][2]*tmp0[3][3]+tmp0[2][0]*tmp0[1][1]*tmp0[0][3]*tmp0[3][2]+tmp0[2][0]*tmp0[3][1]*tmp0[0][2]*tmp0[1][3]-tmp0[2][0]*tmp0[3][1]*tmp0[0][3]*tmp0[1][2]-tmp0[3][0]*tmp0[0][1]*tmp0[1][2]*tmp0[2][3]+tmp0[3][0]*tmp0[0][1]*tmp0[1][3]*tmp0[2][2]+tmp0[3][0]*tmp0[1][1]*tmp0[0][2]*tmp0[2][3]-tmp0[3][0]*tmp0[1][1]*tmp0[0][3]*tmp0[2][2]-tmp0[3][0]*tmp0[2][1]*tmp0[0][2]*tmp0[1][3]+tmp0[3][0]*tmp0[2][1]*tmp0[0][3]*tmp0[1][2]);

}

void intermediate2(int Num) {
	tmp2[0] = (2*fai[0][0]*err[0]+2*fai[1][0]*err[1]+2*fai[2][0]*err[2]+2*fai[3][0]*err[3]+2*fai[4][0]*err[4]+2*fai[5][0]*err[5])/Num;
	tmp2[1] = (2*fai[0][1]*err[0]+2*fai[1][1]*err[1]+2*fai[2][1]*err[2]+2*fai[3][1]*err[3]+2*fai[4][1]*err[4]+2*fai[5][1]*err[5])/Num;
	tmp2[2] = (2*fai[0][2]*err[0]+2*fai[1][2]*err[1]+2*fai[2][2]*err[2]+2*fai[3][2]*err[3]+2*fai[4][2]*err[4]+2*fai[5][2]*err[5])/Num;
	tmp2[3] = (2*fai[0][3]*err[0]+2*fai[1][3]*err[1]+2*fai[2][3]*err[2]+2*fai[3][3]*err[3]+2*fai[4][3]*err[4]+2*fai[5][3]*err[5])/Num;
}

void intermediate3() {
	tmp3[0] = tmp1[0][0]*tmp2[0]+tmp1[0][1]*tmp2[1]+tmp1[0][2]*tmp2[2]+tmp1[0][3]*tmp2[3];
	tmp3[1] = tmp1[1][0]*tmp2[0]+tmp1[1][1]*tmp2[1]+tmp1[1][2]*tmp2[2]+tmp1[1][3]*tmp2[3];
	tmp3[2] = tmp1[2][0]*tmp2[0]+tmp1[2][1]*tmp2[1]+tmp1[2][2]*tmp2[2]+tmp1[2][3]*tmp2[3];
	tmp3[3] = tmp1[3][0]*tmp2[0]+tmp1[3][1]*tmp2[1]+tmp1[3][2]*tmp2[2]+tmp1[3][3]*tmp2[3];
	// update theta[4] is the final step. I integrate into this function here.
	int i = 0;
	for(i = 0; i < sizeof(theta); i++)
	theta[i] -= tmp3[i];
}

void errorN() {
	//int l = sizeof(y);
	float tx = theta[0];
	float ta = theta[1];
	float tb = theta[3];
	float tc = theta[2];
	float y_pred[6] = {tc*tx, 0, 0, 0, 0, 0};
	int i, j;
	err[0] = y[0] - y_pred[0];
	for (i = 1; i < 6; i++) {
		float sum = 0;
		for (j = 0; j < i; j++) {
			float m = ta - tb*tc;
			float n = i - j - 1;
			float p = pow(m,n);
			sum += tc*p*tb*y[j];
		}
		y_pred[i] = sum + tc*pow((ta-tb*tc), i)*tx;
		err[i] = y[i] - y_pred[i];
	}
}

void faiN() {
	//int l = sizeof(y);
	float tx = theta[0];
	float ta = theta[1];
	float tb = theta[3];
	float tc = theta[2];
	int i, j;
	for (i = 0; i < 6; i++)
	for (j = 0; j < 4; j++)
	fai[i][j] = 0;
	fai[0][0] = tc;
	fai[0][1] = 0;
	fai[0][2] = tx;
	fai[0][3] = 0;
	for (i = 1; i < 6; i++) {
		fai[i][0] = tc*pow((ta-tb*tc), i);
		float sum2 = 0;
		for (j = 0; j < i; j++)
		sum2 += tc*(i-j-1)*pow(ta-tb*tc, i-2-j)*tb*y[j];
		fai[i][1] = tc*i*pow(ta-tb*tc, i-1)*tx + sum2;
		float sum3 = 0;
		for (j = 0; j < i; j++)
		sum3 += pow(ta-tb*tc, i-1-j)*tb*y[j] + tc*(i-j-1)*(-tb)*pow(ta-tb*tc, i-j-2)*tb*y[j];
		fai[i][2] = pow(ta-tb*tc,i)*tx + tc*i*(-tb)*pow(ta-tb*tc, i-1)*tx + sum3;
		float sum4 = 0;
		for (j = 0; j < i; j++)
		sum4 += tc*(i-j-1)*(-tc)*pow(ta-tb*tc, i-j-2)*tb*y[j] + tc*pow(ta-tb*tc, i-j-1)*y[j];
		fai[i][3] = tc*i*(-tc)*pow(ta-tb*tc, i-1)*tx + sum4;
	}
}

float sqrmean(float *a) {
	//int n = sizeof(a);
	float sum = 0;
	int i;
	for (i = 0; i < 6; i++)
	sum += a[i]*a[i];
	return sum/6;
}

void predictLQI(float *ans) {
	int num = 6;
	errorN();
	float J = sqrmean(err);
	while (J > 0.5) {
		faiN();

		// Negative fai[6][4]
		int j, k;
		for (j = 0; j < 6; j++)
		for (k = 0; k < 4; k++)
		fai[j][k] = -1*fai[j][k];

		errorN();

		// TODO: Update theta[4].
		// intermediate 0:
		intermediate0(num);
		// intermediate 1:
		intermediate1();
		// intermediate 2:
		intermediate2(num);
		// intermediate 3:
		intermediate3();
		errorN();
		J = sqrmean(err);
		//++l;
		//if (l > 10) break;
	}
	float sum = 0;
	int i;
	for (i = 0; i < num; i++)
	sum += pow(theta[1]-theta[3]*theta[2], num-i-1)*theta[3]*y[i];

	// shift y after current calc, leave the last room for next value comes.
	for (i = 0; i + 1 < num; i++)
	y[i] = y[i+1];
	(*ans) = theta[2]*(pow(theta[1]-theta[3]*theta[2], num)*theta[0] + sum);
}


void wakeupMeOnDataMsg()
{
	lib_green_toggle();
	active = 1;
	return;
}

//-------------------------------------------------------------------------
void registerIncomingDataMsg()
{
	_atomic_t currentatomic;
	radiohandletype *radiohandleaddr;
	void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

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
	//call the radio handle set to store the data structure into the handle vectors
	getaddrfp();
	//open the interrupt
	_atomic_end(currentatomic);
	return;
}


// keep sending the data.
void Send()
{
	packet_id = 0;
	while (1)
	{
		// copy pkt id as the first 4 digits.
		uint32_t pkt = packet_id;
		sendingMsg[0] = pkt % 256;
		pkt /= 256;
		sendingMsg[1] = pkt % 256;
		pkt /= 256;
		sendingMsg[2] = pkt % 256;
		pkt /= 256;
		sendingMsg[3] = pkt;
		
		// fill the rest of buffer.
		int index = 4;
		for (index = 4; index < 32; index++)
		{
			sendingMsg[index] = index;
		}
		
		// actual sending.
		lib_get_radio_lock();
		lib_radio_send_msg(10, 2, 32, sendingMsg);
		lib_release_radio_lock();
		lib_yellow_toggle();
		
		// increase pkt id.
		packet_id += 1;
		
		// sleep 500ms.
		lib_sleep_thread(500);
	}
}

void Receive()
{
	registerIncomingDataMsg();
	
	y[0] = y[1] = y[2] = y[3] = y[4] = y[5] = 0;
	theta[0] = 0.3;
	theta[1] = 0.4;
	theta[2] = 10;
	theta[3] = 0.005;
	
	uint8_t cnt = 0;
	while (1)
	{
		lib_sleep_thread(100);
		if (!active) continue;
		active = 0;
		
		// parse the pkt id.
		uint32_t pkt = incomingMsg[3];
		pkt = pkt*256 + incomingMsg[2];
		pkt = pkt*256 + incomingMsg[1];
		pkt = pkt*256 + incomingMsg[0];
		
		// get RSSI, LQI or PRR.
		rssi = 1.0 * incomingPacketInfo[0];
		
		// predict.
		if (cnt >= 5) {
			y[5] = rssi/100;
			predictLQI(&pvalue);

			// output results.
			pvalue *= 100;
			lib_printf_uinteger32(pkt);
			lib_printf_uinteger32((uint32_t)pvalue);
			lib_printf_uinteger32((uint32_t)rssi);
			lib_printf_ln();
			continue;
		}
		y[cnt++] = prr/100;
	}
}

void protocol()
{
	thisnodeid = lib_get_node_id();
	thiscurrentthread = lib_get_current_thread();
	currentthread = *thiscurrentthread;
	
	if (thisnodeid == 1)
	{
		Send();
	}
	else
	{
		Receive();
	}
}
