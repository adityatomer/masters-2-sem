#include "headers.h"
#include "MyThread.h"
using namespace std;

std::vector<MyThread*>allThreads;

void getMul_IKJ(LL **z,LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, LL n, int threadId){
	for(int i=0;i<n;++i){
		for(int k=0;k<n;++k){
			for(int j=0;j<n;++j){
				z[z_row+i][z_col+j]+=x[x_row+i][x_col+k]*y[y_row+k][y_col+j];
			}
		}
	}
}

// void getMul(LL **z,LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, LL n){
// 	for(int i=0;i<n;++i){
// 		for(int j=0;j<n;++j){
// 			for(int k=0;k<n;++k){
// 				z[z_row+i][z_col+j]+=x[i][k]*y[k][j];
// 			}
// 		}
// 	}
// }

// LL** getMul_KIJ(LL **x, LL **y,LL n){
// 	LL **z;
// 	for(int k=0;k<n;++k){
// 		for(int i=0;i<n;++i){
// 			for(int j=0;j<n;++j){
// 				z[i][j]+=x[i][k]*y[k][j];
// 			}
// 		}
// 	}
// 	printMat(z,n);
// 	return z;
// }

void ParRecMM(LL** z, LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n, int threadId){
	if(n==1){
		getMul_IKJ(z,x,y,z_row, z_col, x_row, x_col, y_row, y_col,n,threadId);
		return;
	}

	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col, x_row, x_col, y_row, y_col, n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col+n/2, x_row, x_col, y_row, y_col+n/2, n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col, x_row+n/2, x_col, y_row, y_col, n/2,threadId));
	ParRecMM(z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col, y_row, y_col+n/2, n/2, threadId);

	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col, x_row, x_col+n/2, y_row+n/2, y_col, n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col+n/2, x_row, x_col+n/2, y_row+n/2, y_col+n/2, n/2,threadId));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col, x_row+n/2, x_col+n/2, y_row+n/2, y_col, n/2,threadId));
	ParRecMM(z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col+n/2, y_row+n/2, y_col+n/2, n/2, threadId);
	
}

//last value of i will be served as current thread id
void createParallelThreads(int num_thread){
	for(int i=0;i<num_thread;++i){
		allThreads.push_back(new MyThread(i));
	}

	for(int i=0;i<num_thread;++i){
		allThreads[i]->setAllThreads(allThreads);
	}

	for(int i=0;i<num_thread;++i){
		allThreads[i]->setAllThreads(allThreads);
	}
}

int main(){
	int n=8;
	int num_thread=1;
	int mainThreadId=0;
	time_t seedTime = time(0);
	LL **x = getMatrixOfSizeR(n, 10);
	LL **y = getMatrixOfSizeR(n, 10);
	LL **z = getMatrixOfSizeR(n,10,false);
	createParallelThreads(num_thread);
	allThreads[mainThreadId]->myDeque.push_back(getWorkObject(&ParRecMM,z,x,y,0,0,0,0,0,0,n,mainThreadId));
	for(int i=0;i<allThreads.size();++i){
		allThreads[i]->compute();
	}
	printMat(x,n);
	cout<<"--------------"<<endl;
	printMat(y,n);
	cout<<"---result----"<<endl;

	printMat(z,n);
	return 0;
}