#include<stdio.h>
#include<iostream>
#include<math.h>
#include<time.h>
#include<array>
#include<vector>
#include <ctime>
#include <stdlib.h>
// #include<cilk/cilk.h> 
#include "MyThread.h"
using namespace std;
#define mod 10
typedef long long LL;

std::vector<MyThread*>allThreads;
// int getRandomNumber(long seed){
// 	int iSecret = rand() % mod + 1;
// 	return iSecret;
// }

LL** getMatrixOfSizeR(int n,bool isRandom=true){
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

LL** getMul_IKJ(LL **z,LL **x, LL **y,LL n){
	// LL **z=new LL*[n];
	for(int i=0;i<n;++i){
		// z[i]=new LL[n];
		
		for(int k=0;k<n;++k){
			for(int j=0;j<n;++j){
				z[i][j]=x[i][k]*y[k][j];
			}
		}
	}
	// cout<<"start getMul_IKJ";
	printMat(z,n);
	// cout<<"end getMul_IKJ";
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

void copySubMatrixToBigMatrix(LL** z, LL** z_sub, int n_sub, int quad){
	cout<<"copySubMatrixToBigMatrix n_sub: "<<n_sub<<" quad: "<<quad<<endl;
	printMat(z,n_sub*2);
	cout<<"end copySubMatrixToBigMatrix n_sub: "<<n_sub<<" quad: "<<quad<<endl;
	int z_row_index=0;
	int z_col_index=0;
	if(quad==2){
		z_col_index=n_sub;
	}
	if(quad==3){
		z_row_index=n_sub;
	}if(quad==4){
		z_row_index=n_sub;
		z_col_index=n_sub;
	}
	
	for(int i=0;i<n_sub;++i){
		for(int j=0;j<n_sub;++j){
			cout<<"i: "<<i<<" j: "<<j<<endl;
			z[z_row_index][z_col_index++]+=z_sub[i][j];
		}
		z_col_index=z_col_index-n_sub;
		z_row_index++;
	}
}

void ParRecMM(LL**z, LL **x, LL **y, int n, int threadId){
	cout<<"threadId: "<<threadId<<" n: "<<n<<endl;
	if(n==1){
		getMul_IKJ(z,x,y,n);
		return;
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
	
	LL** z11=getMatrixOfSizeR(n/2,false);
	LL** z12=getMatrixOfSizeR(n/2,false);
	LL** z21=getMatrixOfSizeR(n/2,false);
	LL** z22=getMatrixOfSizeR(n/2,false);
	
	printMat(z22,n/2);
	
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z11,x11,y11,n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z12,x11,y12,n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z21,x21, y11,n/2,threadId));
	ParRecMM(z22,x21, y12, n/2,threadId);


	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z11,x12, y21, n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z12,x12, y22, n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z21,x22, y21, n/2,threadId));
	ParRecMM(z22,x22, y22, n/2,threadId);


	// printMat(z11,n/2);

	// copySubMatrixToBigMatrix(z, z11,n/2,1);
	cout<<"BEFORE\n";
	printMat(z,n);
	printMat(z12,n/2);
	
	copySubMatrixToBigMatrix(z, z12,n/2,2);
	printMat(z,n);
	cout<<"AFTER\n";
	cout<<endl;
	// copySubMatrixToBigMatrix(z, z21,n/2,3);
	// copySubMatrixToBigMatrix(z, z22,n/2,4);

	// LL** x11_y11= ParRecMM(x11, y11, n/2); 
	// LL** x11_y12= cilk_spawn ParRecMM();
 //    LL** x21_y11= cilk_spawn ParRecMM();
	// LL** x21_y12= ParRecMM();
	// cilk_sync;
	// LL** x12_y21= cilk_spawn ParRecMM();
	// LL** x12_y22= cilk_spawn ParRecMM();
	// LL** x22_y21= cilk_spawn ParRecMM();
	// LL** x22_y22= ParRecMM();
	
	// cilk_sync;	

	// LL** Z=getMatrixOfSizeR(0,n,false);

	// sum(Z,x11_y11,x12_y21,0,0,n);
	// sum(Z,x11_y12,x12_y22,0,n/2,n);
	// sum(Z,x21_y11,x22_y21,n/2,0,n);
	// sum(Z,x21_y12,x22_y22,n/2,n/2,n);

	// return Z;
}

//last value of i will be served as current thread id
int createParallelThreads(int num_thread){
	int i=0;
	for(i=0;i<num_thread;++i){
		allThreads.push_back(new MyThread(i));
	}

	for(int i=0;i<num_thread;++i){
		allThreads[i]->setAllThreads(allThreads);
	}

	for(int i=0;i<num_thread;++i){
		allThreads[i]->setAllThreads(allThreads);
	}

	

	return i-1;
}

int main(){
	int n=4;
	int num_thread=1;
	int currentThreadId=createParallelThreads(num_thread);
	cout<<"currentThreadId ==============> "<<currentThreadId;
	LL **x = getMatrixOfSizeR(n);
	
	cout<<"Matrix X\n";
	printMat(x,n);
	
	
	LL **y = getMatrixOfSizeR(n);
	cout<<"Matrix y\n";
	printMat(y,n);

	LL **z = getMatrixOfSizeR(n);

	Work w=getWorkObject(&ParRecMM,z,x,y,n,currentThreadId);
	allThreads[currentThreadId]->myDeque.push_back(w);
	// ParRecMM(z,x,y,n,currentThreadId);
	// allThreads[currentThreadId]->compute();
	for(int i=0;i<allThreads.size();++i){
		allThreads[i]->compute();
	}

	

	// printMat(x,n);
	// cout<<"--------------"<<endl;
	// printMat(y,n);
	// cout<<"---result----"<<endl;
	// // printMat(z,n);
	return 0;

}