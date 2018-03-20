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
	std::vector<MyThread>allThreads;
	public:
		MyDeque myDeque;
		MyStack myStack;

		MyThread(int id){
			this.id=id;
		}

		void setAllThreads(std::vector<MyThread> allThreads){
			this.allThreads=allThreads;
		}

		Work stealWorkFromRandomDeque(){
			int randomNumber=getRandomNumber(allThreads.size());
			Work w=allThreads[allThreads].deque.pop_front();
			return w;
		}

		bool exitCondition(time_t st){
			int diff=time(0)-st;
			if(diff>300){
				return true;
			}
			return false;
		}

		void compute(){
			while(true){
				//steal
				if(myDeque.size()==0){
					Work work=stealWorkFromRandomDeque();
					this.deque.push_back(work);
				}else{
					Work work=myDeque.pop_back();
					
				}
			}
		}
}