#include<stdio.h>
#include<iostream>
#include<math.h>
#include<time.h>
#define r 2

using namespace std;

int getRandomNumber(long seed){

}

int main(){
	int n=pow(2,r);
	int x[n][n];
	int y[n][n];
	int z[n][n];
	long seed=time(NULL);
	cout<<getRandomNumber(seed);
	return 0;
}
