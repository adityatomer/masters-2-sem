#ifndef HEADERS_H
#define HEADERS_H

#include<iostream>
#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<array>
#include <ctime>
#include<vector>
#define RANDOM_NUMBER_RANDOM_NUMBER_MOD 10
typedef long long LL;
typedef void(* FP)(LL**,LL**,LL**,int,int,int,int,int,int,int,int);
using namespace std;

struct Work{
	// MyStack *stackPtr;
	FP fp;
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

struct SyncType{
	int type;
	int value;
};

int getRandomNumber(int mod){
	srand(time(NULL));
	int iSecret = rand() % mod + 1;
	return iSecret;
}

LL** getMatrixOfSizeR(int n, int mod, bool isRandom=true){
	LL **x=0;
	x=new LL*[n];
	for(int i=0;i<n;++i){
		x[i]=new LL[n];
		for(int j=0;j<n;++j){
			if(isRandom){
				x[i][j]=getRandomNumber(mod);	
			}else{
				x[i][j]=0;
			}
			
		}
	}
	return x;
}

Work getWorkObject(FP fp,LL** z,LL** x,LL** y, int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n, int threadId){
	Work w;//=new Work;
	w.fp=fp;
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