#include<iostream>
#include<stdio.h>
using namespace std;
typedef long long LL;
typedef void(* FP)(LL**,LL**,int);

struct Work{
	FP sp;
	LL** x;
	LL** y;
	int size;
};

struct SyncType{
	int type;
	int value;
	LL** parent_X;
	LL** parent_Y;
};

int getRandomNumber(int mod){
	srand(time(NULL));
	int iSecret = rand() % mod + 1;
	return iSecret;
}

// int main(){
// 	return 0;
// }