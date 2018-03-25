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
			// cout<<"stealing from random queue: "<<randomNumber<<" ";
			Work w;
			if(this->allThreads[randomNumber]->myDeque.size()!=0){
				Work w=this->allThreads[randomNumber]->myDeque.pop_front();	
			}
			
			return w;
		}

		bool exitCondition(int index,int totalThreads){
			if(index>totalThreads){
				return true;
			}
			return false;
		}

		void compute(){
			time_t st=time(0);
			int index=0;
			std::vector<int>randomNumbers=getRandomNumbers(1000,10000);
			int j=0;
			while(true){
				// cout<<"myDeque.size(): "<<myDeque.size()<<endl;
				//steal
				if(myDeque.size()==0){
					if(exitCondition(index++,allThreads.size())){
						break;
					}
				}else{
					// cout<<"IN COMPUTE \n";
					int randomWorkId=randomNumbers[j++];
					Work work=myDeque.pop_back();
					// cout<<"Dequing from queue. Q.size(): "<<myDeque.size()<<" workId: "<<work.id<<" matrix size: "<<work.n<<"\n";
					//void ParRecMM(LL** z, LL **x, LL **y,int z_row,int z_col, 
					// int x_row,int x_col,int y_row,int y_col, int n, int threadId){
					(*(work.fp))(work.z, work.x, work.y, work.z_row, work.z_col,
						work.x_row, work.x_col, work.y_row, work.y_col, work.n, 
							work.threadId, work.syncTypePtr,randomWorkId);
				}
			}
		}
};

