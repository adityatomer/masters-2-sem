#include <iostream>
#include <deque>
#include<time.h>
#include<stdio.h>
#include "MyDeque.h"
#include "MyStack.h"
#include "headers.h"
using namespace std;

class MyThread{
	int id;
	std::vector<MyThread*>allThreads;
	public:
		MyDeque myDeque;
		MyStack myStack;

		MyThread(int id){
			this->id=id;
		}

		void setAllThreads(std::vector<MyThread*> allThreads){
			this->allThreads=allThreads;
		}

		Work stealWorkFromRandomDeque(){
			int randomNumber=getRandomNumber(allThreads.size());
			Work w=this->allThreads[randomNumber]->myDeque.pop_front();
			return w;
		}

		bool exitCondition(int index){
			if(index>1){
				return true;
			}
			return false;
		}

		void compute(){
			time_t st=time(0);
			int index=0;
			while(true){
				// cout<<"myDeque.size(): "<<myDeque.size()<<endl;
				//steal
				if(myDeque.size()==0){
					if(exitCondition(index++)){
						break;
					}else{
						// Work work=stealWorkFromRandomDeque();
						// (*(work.fp))(work.z, work.x, work.y, work.z_row, work.z_col,
						// work.x_row, work.x_col, work.y_row, work.y_col, work.n, 
						// 	work.threadId);
					}
				}else{
					// cout<<"IN COMPUTE \n";
					Work work=myDeque.pop_back();
					//void ParRecMM(LL** z, LL **x, LL **y,int z_row,int z_col, 
					// int x_row,int x_col,int y_row,int y_col, int n, int threadId){
					(*(work.fp))(work.z, work.x, work.y, work.z_row, work.z_col,
						work.x_row, work.x_col, work.y_row, work.y_col, work.n, 
							work.threadId);
				}
			}
		}
};

