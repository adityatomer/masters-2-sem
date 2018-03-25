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
	MyDeque* centralizedDeque;
	public:
		MyStack myStack;

		MyThread(int id){
			this->id=id;
		}

		void setCentralizedDeque(MyDeque* centralizedDeque){
			this->centralizedDeque=centralizedDeque;
		}

		bool exitCondition(int index,int totalThreads){
			if(index>10){
				return true;
			}
			return false;
		}

		void compute(int totalThreads){
			time_t st=time(0);
			int index=0;
			std::vector<int>randomNumbers=getRandomNumbers(1000,10000);
			int j=0;
			while(true){
				if(centralizedDeque->size()==0){
					if(exitCondition(index++,totalThreads)){
						break;
					}
				}else{
					// cout<<"IN COMPUTE \n";
					int randomWorkId=randomNumbers[j++];
					Work work=centralizedDeque->pop_back();
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

