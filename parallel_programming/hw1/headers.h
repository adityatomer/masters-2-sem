#ifndef HEADERS_H
#define HEADERS_H

#include <string> 
#include <stdlib.h>
#include <ctime>
#include<iostream>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<array>
#include<vector>

typedef long long LL;
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
};

typedef void(* FP)(LL**,LL**,LL**,int,int,int,int,int,int,int,int,SyncType*,int);
using namespace std;

struct Work{
	int id;
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

int getRandomNumber(int mod){
	srand(time(NULL));
	int iSecret = rand() % mod + 1;
	return iSecret;
}

std::vector<int> getRandomNumbers(int total,int mod){
        std::vector<int>randNums;
        srand(time(NULL));
        for(int i=0;i<total;++i){
        	randNums.push_back(rand() % mod + 1);
        }
    return randNums;
}

SyncType* getSYNC_1(int n,int z_row,int z_col,int x_row,int x_col,int y_row,int y_col){
	SyncType *st=new SyncType();
	st->type=1;
	st->value=4;
	st->n=n;
	st->z_row=z_row;
	st->z_col=z_col;
	st->x_row=x_row;
	st->x_col=x_col;
	st->y_row=y_row;
	st->y_col=y_col;
	return st;
}

SyncType* getSYNC_2(SyncType *syncType){
	SyncType *st=new SyncType();
	st->type=2;
	st->value=4;
	st->n=syncType->n;
	st->z_row=syncType->z_row;
	st->z_col=syncType->z_col;
	st->x_row=syncType->x_row;
	st->x_col=syncType->x_col;
	st->y_row=syncType->y_row;
	st->y_col=syncType->y_col;
	return st;
}

LL** getMatrixOfSizeR(int n, int mod, bool isRandom=true){
	LL **x=0;
	x=new LL*[n];
	for(int i=0;i<n;++i){
		x[i]=new LL[n];
		for(int j=0;j<n;++j){
			if(isRandom){
				x[i][j]=j+1;
			}else{
				x[i][j]=0;
			}
			
		}
	}
	return x;
}

Work getWorkObject(FP fp,LL** z,LL** x,LL** y, int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n, int threadId,SyncType *syncTypePtr){
	Work w;//=new Work;
	// w.id=wId();
	w.fp=fp;
	w.syncTypePtr=syncTypePtr;//
	w.z=z;
	w.x=x;
	w.y=y;
	w.z_row=z_row;
	w.z_col=z_col;
	w.x_row=x_row;
	w.x_col=x_col;
	w.y_row=y_row;
	w.y_col=y_col;
	w.n=n;
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