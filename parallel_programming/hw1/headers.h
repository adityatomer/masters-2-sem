#ifndef HEADERS_H
#define HEADERS_H

#include<iostream>
#include<stdio.h>
using namespace std;
typedef long long LL;
typedef void(* FP)(LL**,LL**,LL**,int,int);

struct Work{
	FP fp;
	int threadId;
	LL** x;
	LL** y;
	LL** z;
	int size;
	int quadrant;
};

struct SyncType{
	// LL** x11_y11= cilk_spawn ParRecMM(x11, y11, n/2);
	// LL** x11_y12= cilk_spawn ParRecMM(x11, y12, n/2);
 	// LL** x21_y11= cilk_spawn ParRecMM(x21, y11, n/2);
	// LL** x21_y12= ParRecMM(x21, y12, n/2);
	// sum(Z,x11_y11,x12_y21,0,0,n);
	// sum(Z,x11_y12,x12_y22,0,n/2,n);
	// sum(Z,x21_y11,x22_y21,n/2,0,n);
	// sum(Z,x21_y12,x22_y22,n/2,n/2,n);
	int type;
	int value;
	LL** z11;//x11_y11_x12_y21;//x11_y11_x12_y21
	LL** z12;//x11_y11_x12_y21;//x11_y11_x12_y21
	LL** z21;//x11_y11_x12_y21;//x11_y11_x12_y21
	LL** z22;//x11_y11_x12_y21;//x11_y11_x12_y21
	LL** parent_X;
	LL** parent_Y;
};

int getRandomNumber(int mod){
	srand(time(NULL));
	int iSecret = rand() % mod + 1;
	return iSecret;
}

Work getWorkObject(FP fp,LL** z,LL** x,LL** y, int size, int threadId){
	Work w;//=new Work;
	w.fp=fp;
	w.x=x;
	w.y=y;
	w.z=z;
	w.size=size;
	w.threadId=threadId;
	return w;
}

void printMat(LL **mat,int n){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			cout<<mat[i][j]<<" ";
		}
		cout<<endl;
	}
}
#endif