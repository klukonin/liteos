#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

#define N 4
#define M 4
#define EPS (1e-6)

double pvalue;
double prr;
double rssi;
double lqi;

double theta[N];
double y[2*N+2];
double mem[N];
double P[N][N];
double F[N];
double L[N];
double tmp1[N][N];
double tmp2[N];

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
		double F0[4];
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
double predict() {
	int i, j, k;
	// update P[][].
	double mid1[N];
	for (i = 0; i < N; ++i) {
		double x = 0;
		for (j = 0; j < N; ++j) {
			x += F[j]*P[j][i];
		}
		mid1[i] = x;
	}
	double mx = 0;
	for (i = 0; i < N; ++i)
		mx += mid1[i]*F[i];
	double mid2[N];
	for (i = 0; i < N; ++i) {
		double x = 0;
		for (j = 0; j < N; ++j) {
			x += P[i][j]*F[j];
		}
		mid2[i] = x;
	}
	double mid3[N][N];
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			mid3[i][j] = mid2[i]*F[j];
	double mid4[N][N];
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			double x = 0;
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
		double x = 0;
		for (j = 0; j < N; ++j)
			x += P[i][j]*F[j];
		mid1[i] = x;
	}
	for (i = 0; i < N; ++i) {
		double x = 0;
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
	double mt = 0;
	for (i = 0; i < N; ++i)
		mt += theta[i]*F[i];
		
	return mt + mx;
}

void shift() {
	int i;
	for (i = 0; i < 9; ++i)
		y[i] = y[i+1];
}

int main()
{
	FILE *inData = fopen("input2.txt", "r");
	FILE *outData = fopen("ar2lqi.out", "w");
	FILE *outCost = fopen("ar2lqi-cost.out", "w");
	//freopen("input2.txt", "r", stdin);
	//freopen("ar2lqi.out", "w", stdout);
	int cnt = 0;
	while (fscanf(inData, "%lf%lf%lf", &rssi, &lqi, &prr) != EOF)
	{
		if (cnt < 9) {
			y[cnt++] = lqi/800;
			fprintf(outData, "-1\n");
			continue;
		} // buffer 9 values.
		else if (cnt == 9) {
			y[cnt++] = lqi/800;
			calc_para();
			shift();
			fprintf(outData, "-1\n");
			continue;
		} // the last buffer value and calculate the parameters.
				
		clock_t start = clock();
		// do the predict.
		y[9] = lqi/800;
		pvalue = predict();
		shift();
		clock_t end = clock();
		fprintf(outCost, "%lf\n", (end-start));
		fprintf(outData, "%lf\n", pvalue*800);
	}
	fclose(inData);
	fclose(outData);
	fclose(outCost);
	return 0;
}

