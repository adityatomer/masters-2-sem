#include<stdio.h>
#include<iostream>
#include<math.h>
#include<time.h>
#include<array>
#include <stdlib.h>
#include <ctime>
#define r 15
#define mod 10
using namespace std;

int getRandomNumber(long seed){
	int iSecret = rand() % mod + 1;

	return iSecret;
}

long long** getMatrixOfSizeR(long seed, int n){
	long long **x=0;
	x=new long long*[n];
	for(int i=0;i<n;++i){
		x[i]=new long long[n];
		for(int j=0;j<n;++j){
			x[i][j]=getRandomNumber(seed);
		}
	}
	return x;
}

long long** initilaizeResultMatrix(){
	long long **x=0;
	x=new long long*[n];
	for(int i=0;i<n;++i){
		x[i]=new long long[n];
		for(int j=0;j<n;++j){
			x[i][j]=0;
		}
	}
	return x;
}

long long** getMul_IJK(long long **z, long long **x, long long **y,long long n){
	z=initilaizeResultMatrix();
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			for(int k=0;k<n;++k){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	return z;
}

long long** getMul_IKJ(long long **z, long long **x, long long **y,long long n){
	z=initilaizeResultMatrix();
	for(int i=0;i<n;++i){
		for(int k=0;k<n;++k){
			for(int j=0;j<n;++j){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	return z;
}

long long** getMul_JIK(long long **z, long long **x, long long **y,long long n){
	z=initilaizeResultMatrix();
	for(int j=0;j<n;++j){
		for(int i=0;i<n;++i){
			for(int k=0;k<n;++k){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	return z;
}

long long** getMul_JKI(long long **z, long long **x, long long **y,long long n){
	z=initilaizeResultMatrix();
	for(int j=0;j<n;++j){
		for(int k=0;k<n;++k){
			for(int i=0;i<n;++i){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	return z;
}

long long** getMul_KIJ(long long **z, long long **x, long long **y,long long n){
	z=initilaizeResultMatrix();
	for(int k=0;k<n;++k){
		for(int i=0;i<n;++i){
			for(int j=0;j<n;++j){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	return z;
}

long long** getMul_KJI(long long **z, long long **x, long long **y,long long n){
	z=initilaizeResultMatrix();
	for(int k=0;k<n;++k){
		for(int j=0;j<n;++j){
			for(int i=0;i<n;++i){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	return z;
}

void printMat(int **mat,int n){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			cout<<mat[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"----------\n";
}

void exitIfTimeTakenLessThan_5Min(long timeTakenLong, long exitTimeLong){
	if(timeTaken > exitTime){
		cout<<"Exiting!!! Time taken more than 5 Minutes.";
		exit(0);
	}
}

void part_1_a(){
	int maxDimension=10;
	for(int p=1;p<=maxDimension;++p){
		cout<<"*****************************************\n";
		cout<<"Dimension: "<<p<<endl;
		int n=pow(2,p);
		long long **x;
		long long **y;
		long long **z;
		long seed=time(NULL);
		x=getMatrixOfSizeR(seed,n);
		y=getMatrixOfSizeR(seed,n);
		
		// printMat(x,n);
		// printMat(y,n);
		// printMat(z,n);
		time_t st = time(0);
		z=getMul_IJK(x,y,z,n);
		long timeTaken=(time(0)-st);
		cout<<"time taken IJK: "<<timeTaken<<endl;
		exitIfTimeTakenLessThan_5Min(timeTaken,exitTimeLong);

		st = time(0);
		z=getMul_IKJ(x,y,z,n);
		timeTaken=(time(0)-st);
		cout<<"time taken IKJ: "<<timeTaken<<endl;
		exitIfTimeTakenLessThan_5Min(timeTaken,exitTimeLong);

		st = time(0);
		z=getMul_JIK(x,y,z,n);
		timeTaken=(time(0)-st);
		cout<<"time taken JIK: "<<timeTaken<<endl;
		exitIfTimeTakenLessThan_5Min(timeTaken,exitTimeLong);

		st = time(0);
		z=getMul_JKI(x,y,z,n);
		timeTaken=(time(0)-st);
		cout<<"time taken JKI: "<<timeTaken<<endl;
		exitIfTimeTakenLessThan_5Min(timeTaken,exitTimeLong);

		st = time(0);
		z=getMul_KIJ(x,y,z,n);
		timeTaken=(time(0)-st);
		cout<<"time taken KIJ: "<<timeTaken<<endl;
		exitIfTimeTakenLessThan_5Min(timeTaken,exitTimeLong);

		st = time(0);
		z=getMul_KJI(x,y,z,n);
		timeTaken=(time(0)-st);
		cout<<"time taken KJI: "<<timeTaken<<endl;
		exitIfTimeTakenLessThan_5Min(timeTaken,exitTimeLong);
	}
}

int main(){
	part_1_a();

	return 0;
}
