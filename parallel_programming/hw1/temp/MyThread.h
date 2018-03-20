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

		bool exitCondition(time_t st){
			int diff=time(0)-st;
			if(diff>2){
				return false;
			}
			return true;
		}

		void compute(){
			time_t st=time(0);
			int ind=0;
			while(ind++ <100){
				cout<<"myDeque.size(): "<<myDeque.size()<<endl;
				//steal
				if(myDeque.size()==0){
					// Work work=stealWorkFromRandomDeque();
					// this->myDeque.push_back(work);
				}else{
					cout<<"IN COMPUTE \n";
					Work work=myDeque.pop_back();
					// void (*fun_ptr)(LL**,LL**,int) = &fun;
					// MyDeque d;
					// // (*fun_ptr)(a,b,c);
					// d.push_back((*fun_ptr));
					// FP f=d.pop_back();
					// (*f)(a,b,c);

					// FP fp;
					// int threadId;
					// LL** x;
					// LL** y;
					// LL** z;
					// int size;
					// int quadrant;

					//void ParRecMM(LL**z, LL **x, LL **y, int n, int threadId){
					
					(*(work.fp))(work.z,work.x,work.y,work.size,work.threadId);
					
					printMat(work.z,work.size);
					return;
				}
			}
		}
};