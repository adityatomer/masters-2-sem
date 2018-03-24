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
void func2(LL** z, LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n, int threadId, SyncType *syncType,int workId);

void ParRecMM(LL** z, LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n, int threadId, SyncType *syncType, int workId){
	cout<<"ParRecMM "<<" threadId: "<<threadId<<" workId: "<<workId<<" z_row: "<<z_row<<" z_col: "<<z_col<<" x_row: "<<x_row<<" x_col: "<<x_col<<" y_row: "<<y_row<<" y_col: "<<y_col<<" size: "<<n<<" syncType: "<<syncType;
	if(syncType!=NULL){
		cout<<" syncType: "<<syncType->type<<" syncValue: "<<syncType->value<<endl;
		if(syncType->value<=0){
			cout<<"syncType->n: "<<syncType->n<<" syncType->z_row: "<<syncType->z_row<<" syncType->z_col: "<<syncType->z_col<<" syncType->x_row: "<<syncType->x_row<<" syncType->x_col: "<<syncType->x_col<<" syncType->y_row: "<<syncType->y_row<<" syncType->y_col: "<<syncType->y_col<<endl<<endl<<endl;
		}
	}else{
		cout<<endl;
	}
	
	if(n==1){
		cout<<"BASE CASE\n";
		getMul_IKJ(z,x,y,z_row, z_col, x_row, x_col, y_row, y_col,n,threadId);
		bool isSyncTypeNull=true;
		allThreads[threadId]->myStack.decrementStackSyncObject(syncType);
		if(syncType!=NULL && syncType->type==1 && syncType->value==0){
			SyncType *s2=getSYNC_2(syncType);
			allThreads[threadId]->myStack.push(s2);
			cout<<"SyncType S1 is 0, creating S2 =====>"<<s2<<" "<<s2->type<<" "<<s2->value<<" \n";//<<syncType->z_row<<" "<<syncType->z_col<<" "<<syncType->x_row<<" "<<syncType->x_col<<" "<<syncType->y_row<<" "<<syncType->y_col<<" "<<syncType->n<<endl;
			func2(z,x,y,syncType->z_row,syncType->z_col, syncType->x_row, syncType->x_col, syncType->y_row, syncType->y_col,syncType->n,threadId,s2,workId);
		}
		return;
	}

	SyncType *s1=NULL;
	if(syncType==NULL || syncType->type==2){
		s1=getSYNC_1(n,z_row,z_col,x_row,x_col,y_row,y_col);
		allThreads[threadId]->myStack.push(s1);	
	}else{
		s1=syncType;
	}
	if(s1->value==0){
		return;
	}
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col, x_row, x_col, y_row, y_col, n/2,threadId,s1));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col+n/2, x_row, x_col, y_row, y_col+n/2, n/2,threadId,s1));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col, x_row+n/2, x_col, y_row, y_col, n/2,threadId,s1));
	ParRecMM(z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col, y_row, y_col+n/2, n/2, threadId,NULL,workId);
	s1=allThreads[threadId]->myStack.decrementStackSyncObject(s1);
}


void func2(LL** z, LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n, int threadId, SyncType *syncType, int workId){
	cout<<"Func2 "<<" threadId: "<<threadId<<" z_row: "<<z_row<<" z_col: "<<z_col<<" x_row: "<<x_row<<" x_col: "<<x_col<<" y_row: "<<y_row<<" y_col: "<<y_col<<" size: "<<n<<" syncType: "<<syncType;
	if(syncType!=NULL){
		cout<<" syncType: "<<syncType->type<<" syncValue: "<<syncType->value<<endl;
	}else{
		cout<<endl;
	}
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col, x_row, x_col+n/2, y_row+n/2, y_col, n/2,threadId,syncType));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col+n/2, x_row, x_col+n/2, y_row+n/2, y_col+n/2, n/2,threadId,syncType));
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col, x_row+n/2, x_col+n/2, y_row+n/2, y_col, n/2,threadId,syncType));
	ParRecMM(z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col+n/2, y_row+n/2, y_col+n/2, n/2, threadId,NULL,workId);
	allThreads[threadId]->myStack.decrementStackSyncObject(syncType);
	// if(syncType->value==0){
	// 	return;
	// }
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
	int n=4;
	int num_thread=1;
	int mainThreadId=0;
	time_t seedTime = time(0);
	LL **x = getMatrixOfSizeR(n, 10);
	LL **y = getMatrixOfSizeR(n, 10);
	LL **z = getMatrixOfSizeR(n,10,false);
	createParallelThreads(num_thread);
	allThreads[mainThreadId]->myDeque.push_back(getWorkObject(&ParRecMM,z,x,y,0,0,0,0,0,0,n,mainThreadId,NULL));
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