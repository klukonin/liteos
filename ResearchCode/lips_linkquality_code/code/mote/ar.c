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
uint32_t counter;
uint16_t active;
uint32_t packet_id;

uint8_t protocolbuffer[300];

lib_thread **thiscurrentthread;
lib_thread *currentthread;

static uint8_t sendingMsg[32];
static uint8_t incomingMsg[32];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[4];

#define N 4
#define M 4
#define EPS (1e-6)

float pvalue;
float prr;
float rssi;
float lqi;

float theta[N];
float y[2*N+2];
float mem[N];
float P[N][N];
float F[N];
float L[N];
float tmp1[N][N];
float tmp2[N];

void inv() {
	P[0][0] = (tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[3][1]*tmp1[1][3]*tmp1[2][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[0][1] = -(tmp1[0][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][1]*tmp1[2][3]*tmp1[3][2]-tmp1[2][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][1]*tmp1[0][3]*tmp1[3][2]+tmp1[3][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[0][2] = (tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[3][1]*tmp1[0][3]*tmp1[1][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[0][3] = -(tmp1[0][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[1][0] = -(tmp1[1][0]*tmp1[2][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][3]*tmp1[3][2]+tmp1[3][0]*tmp1[1][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][3]*tmp1[2][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[1][1] = (tmp1[0][0]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[2][3]*tmp1[3][2]-tmp1[2][0]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[0][3]*tmp1[3][2]+tmp1[3][0]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[0][3]*tmp1[2][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[1][2] = -(tmp1[0][0]*tmp1[1][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][3]*tmp1[3][2]-tmp1[1][0]*tmp1[0][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][3]*tmp1[3][2]+tmp1[3][0]*tmp1[0][2]*tmp1[1][3]-tmp1[3][0]*tmp1[0][3]*tmp1[1][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[1][3] = (tmp1[0][0]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[0][3]*tmp1[1][2]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[2][0] = (tmp1[1][0]*tmp1[2][1]*tmp1[3][3]-tmp1[1][0]*tmp1[2][3]*tmp1[3][1]-tmp1[2][0]*tmp1[1][1]*tmp1[3][3]+tmp1[2][0]*tmp1[1][3]*tmp1[3][1]+tmp1[3][0]*tmp1[1][1]*tmp1[2][3]-tmp1[3][0]*tmp1[1][3]*tmp1[2][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[2][1] = -(tmp1[0][0]*tmp1[2][1]*tmp1[3][3]-tmp1[0][0]*tmp1[2][3]*tmp1[3][1]-tmp1[2][0]*tmp1[0][1]*tmp1[3][3]+tmp1[2][0]*tmp1[0][3]*tmp1[3][1]+tmp1[3][0]*tmp1[0][1]*tmp1[2][3]-tmp1[3][0]*tmp1[0][3]*tmp1[2][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[2][2] = (tmp1[0][0]*tmp1[1][1]*tmp1[3][3]-tmp1[0][0]*tmp1[1][3]*tmp1[3][1]-tmp1[1][0]*tmp1[0][1]*tmp1[3][3]+tmp1[1][0]*tmp1[0][3]*tmp1[3][1]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]-tmp1[3][0]*tmp1[0][3]*tmp1[1][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[2][3] = -(tmp1[0][0]*tmp1[1][1]*tmp1[2][3]-tmp1[0][0]*tmp1[1][3]*tmp1[2][1]-tmp1[1][0]*tmp1[0][1]*tmp1[2][3]+tmp1[1][0]*tmp1[0][3]*tmp1[2][1]+tmp1[2][0]*tmp1[0][1]*tmp1[1][3]-tmp1[2][0]*tmp1[0][3]*tmp1[1][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[3][0] = -(tmp1[1][0]*tmp1[2][1]*tmp1[3][2]-tmp1[1][0]*tmp1[2][2]*tmp1[3][1]-tmp1[2][0]*tmp1[1][1]*tmp1[3][2]+tmp1[2][0]*tmp1[1][2]*tmp1[3][1]+tmp1[3][0]*tmp1[1][1]*tmp1[2][2]-tmp1[3][0]*tmp1[1][2]*tmp1[2][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[3][1] = (tmp1[0][0]*tmp1[2][1]*tmp1[3][2]-tmp1[0][0]*tmp1[2][2]*tmp1[3][1]-tmp1[2][0]*tmp1[0][1]*tmp1[3][2]+tmp1[2][0]*tmp1[0][2]*tmp1[3][1]+tmp1[3][0]*tmp1[0][1]*tmp1[2][2]-tmp1[3][0]*tmp1[0][2]*tmp1[2][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[3][2] = -(tmp1[0][0]*tmp1[1][1]*tmp1[3][2]-tmp1[0][0]*tmp1[1][2]*tmp1[3][1]-tmp1[1][0]*tmp1[0][1]*tmp1[3][2]+tmp1[1][0]*tmp1[0][2]*tmp1[3][1]+tmp1[3][0]*tmp1[0][1]*tmp1[1][2]-tmp1[3][0]*tmp1[0][2]*tmp1[1][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
	P[3][3] = (tmp1[0][0]*tmp1[1][1]*tmp1[2][2]-tmp1[0][0]*tmp1[1][2]*tmp1[2][1]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]+tmp1[1][0]*tmp1[0][2]*tmp1[2][1]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]-tmp1[2][0]*tmp1[0][2]*tmp1[1][1]+EPS)/(tmp1[0][0]*tmp1[1][1]*tmp1[2][2]*tmp1[3][3]-tmp1[0][0]*tmp1[1][1]*tmp1[2][3]*tmp1[3][2]-tmp1[0][0]*tmp1[2][1]*tmp1[1][2]*tmp1[3][3]+tmp1[0][0]*tmp1[2][1]*tmp1[1][3]*tmp1[3][2]+tmp1[0][0]*tmp1[3][1]*tmp1[1][2]*tmp1[2][3]-tmp1[0][0]*tmp1[3][1]*tmp1[1][3]*tmp1[2][2]-tmp1[1][0]*tmp1[0][1]*tmp1[2][2]*tmp1[3][3]+tmp1[1][0]*tmp1[0][1]*tmp1[2][3]*tmp1[3][2]+tmp1[1][0]*tmp1[2][1]*tmp1[0][2]*tmp1[3][3]-tmp1[1][0]*tmp1[2][1]*tmp1[0][3]*tmp1[3][2]-tmp1[1][0]*tmp1[3][1]*tmp1[0][2]*tmp1[2][3]+tmp1[1][0]*tmp1[3][1]*tmp1[0][3]*tmp1[2][2]+tmp1[2][0]*tmp1[0][1]*tmp1[1][2]*tmp1[3][3]-tmp1[2][0]*tmp1[0][1]*tmp1[1][3]*tmp1[3][2]-tmp1[2][0]*tmp1[1][1]*tmp1[0][2]*tmp1[3][3]+tmp1[2][0]*tmp1[1][1]*tmp1[0][3]*tmp1[3][2]+tmp1[2][0]*tmp1[3][1]*tmp1[0][2]*tmp1[1][3]-tmp1[2][0]*tmp1[3][1]*tmp1[0][3]*tmp1[1][2]-tmp1[3][0]*tmp1[0][1]*tmp1[1][2]*tmp1[2][3]+tmp1[3][0]*tmp1[0][1]*tmp1[1][3]*tmp1[2][2]+tmp1[3][0]*tmp1[1][1]*tmp1[0][2]*tmp1[2][3]-tmp1[3][0]*tmp1[1][1]*tmp1[0][3]*tmp1[2][2]-tmp1[3][0]*tmp1[2][1]*tmp1[0][2]*tmp1[1][3]+tmp1[3][0]*tmp1[2][1]*tmp1[0][3]*tmp1[1][2]+EPS);
}


// calculate tmp1arameters after the first 10 buffer stores.
void calc_para() {
	int i, j, k;
	for (i = 0; i < N; ++i)
	mem[i] = y[10-N+i];
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			tmp1[i][j] = 0;
			tmp2[j] = 0;
		}
	}
	for (i = 0; i < N; ++i) {
		float F0[4];
		F0[0] = -y[10-N+i-1];
		F0[1] = -y[10-N+i-2];
		F0[2] = -y[10-N+i-3];
		F0[3] = -y[10-N+i-4];
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				tmp1[j][k] += F0[j]*F0[k];
			}
			tmp2[j] += F0[j]*mem[i];
		}
	}
	inv();  // P = inv(tmp1);
	for (i = 0; i < N; ++i) {
		theta[i] = 0;
		for (j = 0; j < N; ++j) {
			theta[i] += P[i][j]*tmp2[j];
		}
	}
	for (i = 0; i < N; ++i)
	F[i] = -y[10-i-1];
}

// predict action starts from the 11th value.
void predictLQI(float *pvalue) {
	int i, j, k;
	// update P[][].
	float mid1[N];
	for (i = 0; i < N; ++i) {
		float x = 0;
		for (j = 0; j < N; ++j) {
			x += F[j]*P[j][i];
		}
		mid1[i] = x;
	}
	float mx = 0;
	for (i = 0; i < N; ++i)
	mx += mid1[i]*F[i];
	float mid2[N];
	for (i = 0; i < N; ++i) {
		float x = 0;
		for (j = 0; j < N; ++j) {
			x += P[i][j]*F[j];
		}
		mid2[i] = x;
	}
	float mid3[N][N];
	for (i = 0; i < N; ++i)
	for (j = 0; j < N; ++j)
	mid3[i][j] = mid2[i]*F[j];
	float mid4[N][N];
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			float x = 0;
			for (k = 0; k < N; ++k)
			x += mid3[i][j]*P[j][k];
			mid4[i][j] = x;
		}
	}
	for (i = 0; i < N; ++i)
	for (j = 0; j < N; ++j)
	P[i][j] = P[i][j] - mid4[i][j]/(1.0+mx);

	// update L[].
	for (i = 0; i < N; ++i) {
		float x = 0;
		for (j = 0; j < N; ++j)
		x += P[i][j]*F[j];
		mid1[i] = x;
	}
	for (i = 0; i < N; ++i) {
		float x = 0;
		for (j = 0; j < N; ++j)
		x += F[j]*P[j][i];
		mid2[i] = x;
	}
	mx = 0;
	for (i = 0; i < N; ++i)
	mx += mid2[i]*F[i];
	for (i = 0; i < N; ++i)
	L[i] = mid1[i]/(1.0+mx);

	// update theta[].
	mx = 0;
	for (i = 0; i < N; ++i)
	mx += theta[i]*F[i];
	mx = y[9] - mx; // current y - mx.
	for (i = 0; i < N; ++i)
	theta[i] += L[i]*mx;

	// copy F[] and update F[].
	for (i = 0; i < N; ++i) {
		mid1[i] = F[i];
		F[i] = -y[10-i-1];
	}

	// predict here.
	mx = 0;
	for (i = 0; i < N; ++i)
	mx += theta[i]*mid1[i];
	mx = y[9] - mx;
	float mt = 0;
	for (i = 0; i < N; ++i)
	mt += theta[i]*F[i];

	*pvalue = mt + mx;
}

void shift() {
	int i;
	for (i = 0; i < 9; ++i)
	y[i] = y[i+1];
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
