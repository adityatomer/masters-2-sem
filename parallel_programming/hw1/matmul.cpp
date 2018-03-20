#include<stdio.h>
#include<iostream>
#include<math.h>
#include<time.h>
#include<array>
#include <ctime>
#include <stdlib.h>
#include<cilk/cilk.h> 
using namespace std;
#define mod 10


int getRandomNumber(long seed){
	int iSecret = rand() % mod + 1;
	return iSecret;
}

LL** getMatrixOfSizeR(long seed, int n,bool isRandom=true){
	LL **x=0;
	x=new LL*[n];
	for(int i=0;i<n;++i){
		x[i]=new LL[n];
		for(int j=0;j<n;++j){
			if(isRandom){
				x[i][j]=getRandomNumber(seed);	
			}else{
				x[i][j]=0;
			}
			
		}
	}
	return x;
}

void printMat(LL **mat,int n){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			cout<<mat[i][j]<<" ";
		}
		cout<<endl;
	}
}

LL** getMul_IKJ(LL **x, LL **y,LL n){
	LL **z=new LL*[n];
	for(int i=0;i<n;++i){
		z[i]=new LL[n];
		
		for(int k=0;k<n;++k){
			for(int j=0;j<n;++j){
				z[i][j]=x[i][k]*y[k][j];
			}
		}
	}
	return z;
}

LL** getMul_KIJ(LL **x, LL **y,LL n){
	LL **z;
	for(int k=0;k<n;++k){
		for(int i=0;i<n;++i){
			for(int j=0;j<n;++j){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	printMat(z,n);
	return z;
}


LL** getNewMatOfSmallerSize(LL **orignalMat, int x,int y, int n){
	LL **z=0;
	z=new LL*[n];
	int ind_x=0;
	int ind_y=0;

	for(int i=x;i<x+n;++i){
		z[ind_x]=new LL[n];
		ind_y=0;
		for(int j=y;j<y+n;++j){
			z[ind_x][ind_y++]=orignalMat[i][j];
		}
		ind_x++;
	}
	return z;
}

void sum(LL **z, LL **x, LL **y, int row, int col,int n){
	int rowInd=0;
	int colInd=0;
	for(int i=row;i<row+n/2;++i){
		colInd=0;
		for(int j=col;j<col+n/2;++j){
			z[i][j]=x[rowInd][colInd]+y[rowInd][colInd];
			colInd++;
		}
		rowInd++;
	}
}

LL** ParRecMM(LL **x, LL **y, int n){
	if(n==1){
		return getMul_IKJ(x,y,n);
	}
	LL **x11=getNewMatOfSmallerSize(x,0,0,n/2);
	LL **x12=getNewMatOfSmallerSize(x,0,n/2,n/2);
	LL **x21=getNewMatOfSmallerSize(x,n/2,0,n/2);
	LL **x22=getNewMatOfSmallerSize(x,n/2,n/2,n/2);
	LL **y11=getNewMatOfSmallerSize(y,0,0,n/2);
	LL **y12=getNewMatOfSmallerSize(y,0,n/2,n/2);
	LL **y21=getNewMatOfSmallerSize(y,n/2,0,n/2);
	LL **y22=getNewMatOfSmallerSize(y,n/2,n/2,n/2);

	//cout<<"-------------------"<<endl;
	//printMat(Z,n);	
	//cout<<"-------------------"<<endl;
	LL** x11_y11= cilk_spawn ParRecMM(x11, y11, n/2);
	LL** x11_y12= cilk_spawn ParRecMM(x11, y12, n/2);
    LL** x21_y11= cilk_spawn ParRecMM(x21, y11, n/2);
	LL** x21_y12= ParRecMM(x21, y12, n/2);
	cilk_sync;
	LL** x12_y21= cilk_spawn ParRecMM(x12, y21, n/2);
	LL** x12_y22= cilk_spawn ParRecMM(x12, y22, n/2);
	LL** x22_y21= cilk_spawn ParRecMM(x22, y21, n/2);
	LL** x22_y22= ParRecMM(x22, y22, n/2);
	
	cilk_sync;	

	LL** Z=getMatrixOfSizeR(0,n,false);

	sum(Z,x11_y11,x12_y21,0,0,n);
	sum(Z,x11_y12,x12_y22,0,n/2,n);
	sum(Z,x21_y11,x22_y21,n/2,0,n);
	sum(Z,x21_y12,x22_y22,n/2,n/2,n);

	return Z;
}

std::std::vector<MyThread> createParallelThreads(int num_thread){
	std::vector<MyThread>allThreads;
	for(int i=0;i<num_thread;++i){
		MyThread mythread=new MyThread(i);
		allThreads.push(mythread);
	}
	for(int i=0;i<num_thread;++i){
		allThreads[i].setAllThreads(allThreads);
	}

	for(int i=0;i<num_thread - 1;++i){
		cilk_spawn allThreads[i].setAllThreads(allThreads);
	}

	return allThreads;
}

int main(){
	int n=4;
	time_t seedTime = time(0);
	LL **x = getMatrixOfSizeR(seedTime,n);
	seedTime = time(0);
	LL **y = getMatrixOfSizeR(seedTime,n);
	LL **z=ParRecMM(x,y,n);
	printMat(x,n);
	cout<<"--------------"<<endl;
	printMat(y,n);
	cout<<"---result----"<<endl;
	printMat(z,n);
	return 0;

}