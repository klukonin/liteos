#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <time.h>
//#include <windows.h>
using namespace std;

static double theta[4];
static double err[6];
static double y[6];
static double fai[6][4];
static double eps = 1e-3;
static double tmp0[4][4];
static double tmp1[4][4];
static double tmp2[4];
static double tmp3[4];

static double pvalue;
static double rssi;
static double lqi;
static double prr;



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
	double tx = theta[0];
	double ta = theta[1];
	double tb = theta[3];
	double tc = theta[2];
	double y_pred[6] = {tc*tx, 0, 0, 0, 0, 0};
	int i, j;
	err[0] = y[0] - y_pred[0];
	for (i = 1; i < 6; i++) {
		double sum = 0;
		for (j = 0; j < i; j++) {
			double m = ta - tb*tc;
			double n = i - j - 1;
			double p = pow(m,n);
			sum += tc*p*tb*y[j];
		}
		y_pred[i] = sum + tc*pow((ta-tb*tc), i)*tx;
		err[i] = y[i] - y_pred[i];
	}	
}

void faiN() {
	//int l = sizeof(y);
	double tx = theta[0];
	double ta = theta[1];
	double tb = theta[3];
	double tc = theta[2];
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
		double sum2 = 0;
		for (j = 0; j < i; j++)
			sum2 += tc*(i-j-1)*pow(ta-tb*tc, i-2-j)*tb*y[j];
		fai[i][1] = tc*i*pow(ta-tb*tc, i-1)*tx + sum2;
		double sum3 = 0;
		for (j = 0; j < i; j++) 
			sum3 += pow(ta-tb*tc, i-1-j)*tb*y[j] + tc*(i-j-1)*(-tb)*pow(ta-tb*tc, i-j-2)*tb*y[j];
		fai[i][2] = pow(ta-tb*tc,i)*tx + tc*i*(-tb)*pow(ta-tb*tc, i-1)*tx + sum3;
		double sum4 = 0;
		for (j = 0; j < i; j++)
			sum4 += tc*(i-j-1)*(-tc)*pow(ta-tb*tc, i-j-2)*tb*y[j] + tc*pow(ta-tb*tc, i-j-1)*y[j];
		fai[i][3] = tc*i*(-tc)*pow(ta-tb*tc, i-1)*tx + sum4;
	}
}

double sqrmean(double *a) {
	//int n = sizeof(a);
	double sum = 0;
	int i;
	for (i = 0; i < 6; i++)
		sum += a[i]*a[i];
	return sum/6;
}

void predictLQI(double *ans) {
	int num = 6;
	int l = 1;
	errorN();
	double J = sqrmean(err);
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
	double sum = 0;
	int i;
	for (i = 0; i < num; i++)
		sum += pow(theta[1]-theta[3]*theta[2], num-i-1)*theta[3]*y[i];
	
	// shift y after current calc, leave the last room for next value comes.
	for (i = 0; i + 1 < num; i++)
		y[i] = y[i+1];
	(*ans) = theta[2]*(pow(theta[1]-theta[3]*theta[2], num)*theta[0] + sum);
}

int main()
{

	y[0] = y[1] = y[2] = y[3] = y[4] = y[5] = 0;
	theta[0] = 0.3;
	theta[1] = 0.4;
	theta[2] = 10;
	theta[3] = 0.005;
	//freopen("input2.txt", "r", stdin);
	//freopen("sp2-lqi.out", "w", stdout);
	FILE *inData = fopen("../../distance-data/data-1m.in", "r");
	FILE *outData = fopen("../../distance-data/data-1m-rssi.out", "w");
	//FILE *outCost = fopen("../../distance-data/data-1m.cost", "w");
	
	int cnt = 0;
	while (fscanf(inData, "%lf%lf%lf", &rssi, &lqi, &prr) != EOF)
	{
		if (cnt >= 5) {
			y[5] = rssi/200;
			//int start = GetTickCount();
			predictLQI(&pvalue);
			//int end = GetTickCount();
			//fprintf(outCost, "%d\n", end-start);
			fprintf(outData, "%lf\n", pvalue*200);
			continue;
		}
		y[cnt++] = rssi/200;
		fprintf(outData, "-1\n");
	}
	return 0;
}

