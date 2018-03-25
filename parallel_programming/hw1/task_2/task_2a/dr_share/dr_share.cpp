#include "headers.h"
#include "MyThread.h"
#include<cilk/cilk.h>
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
	// cout<<"ParRecMM "<<" threadId: "<<threadId<<" z_row: "<<z_row<<" z_col: "<<z_col<<" x_row: "<<x_row<<" x_col: "<<x_col<<" y_row: "<<y_row<<" y_col: "<<y_col<<" size: "<<n<<" syncType: "<<syncType;
	// if(syncType!=NULL){
	// 	cout<<" syncType: "<<syncType->type<<" syncValue: "<<syncType->value<<endl;
	// 	// if(syncType->value<=0){
	// 	// 	cout<<"syncType->n: "<<syncType->n<<" syncType->z_row: "<<syncType->z_row<<" syncType->z_col: "<<syncType->z_col<<" syncType->x_row: "<<syncType->x_row<<" syncType->x_col: "<<syncType->x_col<<" syncType->y_row: "<<syncType->y_row<<" syncType->y_col: "<<syncType->y_col<<endl<<endl<<endl;
	// 	// }
	// }else{
	// 	cout<<endl;
	// }
	
	if(n==1){
		// cout<<"BASE CASE\n";
		getMul_IKJ(z,x,y,z_row, z_col, x_row, x_col, y_row, y_col,n,threadId);
 		allThreads[threadId]->myStack.decrementStackSyncObject(syncType);

		// if(syncType!=NULL && syncType->type==1 && syncType->value==0){
			// SyncType *s2=getSYNC_2(syncType);
			// allThreads[threadId]->myStack.push(s2);
			// cout<<"SyncType S1 is 0, creating S2 =====>"<<s2<<" "<<s2->type<<" "<<s2->value<<" \n";//<<syncType->z_row<<" "<<syncType->z_col<<" "<<syncType->x_row<<" "<<syncType->x_col<<" "<<syncType->y_row<<" "<<syncType->y_col<<" "<<syncType->n<<endl;
			// func2(z,x,y,syncType->z_row,syncType->z_col, syncType->x_row, syncType->x_col, syncType->y_row, syncType->y_col,syncType->n,threadId,s2,workId);
			if(syncType->value==0){
				// cout<<"\nFunc2 SyncType 2 reached 0 for "<<" threadId: "<<threadId<<" z_row: "<<z_row<<" z_col: "<<z_col<<" x_row: "<<x_row<<" x_col: "<<x_col<<" y_row: "<<y_row<<" y_col: "<<y_col<<" size: "<<n<<" syncType: "<<syncType;
				// if(syncType!=NULL){
				// 	cout<<" syncType: "<<syncType->type<<" syncValue: "<<syncType->value<<endl;
				// }else{
				// 	cout<<endl;
				// }
				SyncType *p=syncType;

				while(p!=NULL){
					if(p->type==1){
						// cout<<" p->type: "<<p->type;
						if(p->value==0){
							// cout<<" p->value is zero"<<endl;
							SyncType *s2=getSYNC_2(p);
							allThreads[threadId]->myStack.push(s2);
							// cout<<"SyncType S1 is 0, creating S2 =====>"<<s2<<" "<<s2->type<<" "<<s2->value<<" \n";//<<syncType->z_row<<" "<<syncType->z_col<<" "<<syncType->x_row<<" "<<syncType->x_col<<" "<<syncType->y_row<<" "<<syncType->y_col<<" "<<syncType->n<<endl;
							func2(z,x,y,s2->z_row,s2->z_col, s2->x_row, s2->x_col, s2->y_row, s2->y_col,s2->n,threadId,s2,workId);
							// p=p->syncType;
							// allThreads[threadId]->myStack.decrementStackSyncObject(p);	
							break;
						}else{
							// cout<<" p->value is non zero, breaking"<<endl;
							break;
						}
					}else{
						if(p->value==0){
							// cout<<"PARENT BEFORE p->value: "<<p<<" "<<p->value<<endl;
							p=p->syncType;
							// cout<<"PARENT 1. BEFORE p->value: "<<p<<" "<<p->value<<endl;
							allThreads[threadId]->myStack.decrementStackSyncObject(p);	
							// cout<<"========== p: "<<p<<" "<<p->value<<endl;
							// cout<<"PARENT AFTER p->value: "<<p->value<<endl;
						}else{
							// cout<<" p->value is non zero, breaking"<<endl;
							break;
						}						
						
					} 
				}
			}


		return;
	}

	SyncType *s1=getSYNC_1(n,z_row,z_col,x_row,x_col,y_row,y_col,syncType);
	allThreads[threadId]->myStack.push(s1);	
	
	// int threadId;
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col, x_row, x_col, y_row, y_col, n/2,threadId,s1));
	
	threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col+n/2, x_row, x_col, y_row, y_col+n/2, n/2,threadId,s1));
	
	threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col, x_row+n/2, x_col, y_row, y_col, n/2,threadId,s1));
	
	threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col, y_row, y_col+n/2, n/2, threadId,s1));
}


void func2(LL** z, LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n, int threadId, SyncType *syncType, int workId){
	// cout<<"Func2    "<<" threadId: "<<threadId<<" z_row: "<<z_row<<" z_col: "<<z_col<<" x_row: "<<x_row<<" x_col: "<<x_col<<" y_row: "<<y_row<<" y_col: "<<y_col<<" size: "<<n<<" syncType: "<<syncType;
	// if(syncType!=NULL){
	// 	cout<<" syncType: "<<syncType->type<<" syncValue: "<<syncType->value<<endl;
	// }else{
	// 	cout<<endl;
	// }
	// int threadId;
	threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col, x_row, x_col+n/2, y_row+n/2, y_col, n/2,threadId,syncType));
	threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row, z_col+n/2, x_row, x_col+n/2, y_row+n/2, y_col+n/2, n/2,threadId,syncType));
	threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col, x_row+n/2, x_col+n/2, y_row+n/2, y_col, n/2,threadId,syncType));
	threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col+n/2, y_row+n/2, y_col+n/2, n/2, threadId,syncType));
	// allThreads[threadId]->myStack.decrementStackSyncObject(syncType);
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
	int n=256;
	int num_thread=4;
	time_t seedTime = time(0);
	LL **x = getMatrixOfSizeR(n, 10);
	LL **y = getMatrixOfSizeR(n, 10);
	LL **z = getMatrixOfSizeR(n,10,false);
	time_t st=time(NULL);
	createParallelThreads(num_thread);
	int threadId=getRandomNumber(allThreads.size());
	allThreads[threadId]->myDeque.push_back(getWorkObject(&ParRecMM,z,x,y,0,0,0,0,0,0,n,threadId,NULL));
	for(int i=0;i<allThreads.size();++i){
		cilk_spawn allThreads[i]->compute();
	}
	cilk_sync;
	time_t totalTime=time(NULL)-st;
	printMat(x,n);
	cout<<"--------------"<<endl;
	printMat(y,n);
	cout<<"---result----"<<endl;
	printMat(z,n);
	cout<<"\n\nTotal Time taken DR_SHARE (Excluding Matrix printing time): "<<totalTime<<endl<<endl;
	return 0;
}
