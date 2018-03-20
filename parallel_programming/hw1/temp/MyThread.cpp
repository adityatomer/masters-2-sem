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
			if(diff>5){
				return true;
			}
			return false;
		}

		void compute(){
			time_t st=time(0);
			while(exitCondition(st)){
				//steal
				if(myDeque.size()==0){
					Work work=stealWorkFromRandomDeque();
					this.deque.push_back(work);
				}else{
					//void ParRecMM(LL **x, LL **y, int n, int threadId){
					Work work=myDeque.pop_back();
					// void (*fun_ptr)(LL**,LL**,int) = &fun;
					// MyDeque d;
					// // (*fun_ptr)(a,b,c);
					// d.push_back((*fun_ptr));
					// FP f=d.pop_back();
					// (*f)(a,b,c);

					*work.fp(work.z,work.x,work.y,work.size,work.threadId);
					cout<<"printing from MyThread"<<endl;
					printMat(work.z,work.size);
					return;
				}
			}
		}
}