#ifndef BASICHEADER_H
#define BASICHEADER_H

#include <iostream>
#include <stdio.h>
#include <mutex>
using namespace std;
typedef long long LL;

std::mutex m;
struct SyncType{
	int type;
	int value;
	int n;
	int z_row;
	int z_col;
	int x_row;
	int x_col;
	int y_row;
	int y_col;
	SyncType* syncType;
};

int readSyncValue(SyncType *s){
		std::unique_lock<std::mutex>lock(m);	
		return s->value;
}

typedef void(* FP)(LL**,LL**,LL**,int,int,int,int,int,int,int,int,SyncType*);

struct Work{
	FP fp;
	SyncType *syncTypePtr;
	LL** x;
	LL** y;
	LL** z;
	int n;
	int z_row;
	int z_col;
	int x_row;
	int x_col;
	int y_row;
	int y_col;
	int threadId;
};

#endif