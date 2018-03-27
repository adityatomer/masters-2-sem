#ifndef MyDeque_H
#define MyDeque_H
#include<stdio.h>
#include <iostream>
#include <deque>
#include <mutex>
#include "basicheader.h"
using namespace std;
// typedef long long LL;
// typedef void(* FP)(LL**,LL**,int);

class MyDeque{
	//LL** ParRecMM(LL **x, LL **y, int n)
	private:
	std::deque<Work*> q;
	std::mutex m;

	public:
		MyDeque(){

		}

		int size(){
			std::unique_lock<std::mutex> lock(m);
			return q.size();
		}
		void push_front(Work *item){
			std::unique_lock<std::mutex> lock(m);
			q.push_front(item);
		}

		void push_back(Work *item){
			std::unique_lock<std::mutex> lock(m);
			q.push_back(item);
		}

		Work* pop_front(){
			std::unique_lock<std::mutex> lock(m);
			if(q.size()==0){
				return NULL;
			}
			Work *front=q.front();
			q.pop_front();
			return front;
		}

		Work* pop_back(){
			std::unique_lock<std::mutex> lock(m);
			if(q.size()==0){
				return NULL;
			}
			Work *back=q.back();
			q.pop_back();
			return back;
		}

		// ~MyDeque();

};

#endif
// void fun(LL **a,LL **b,int c)
// {
//     printf("Value of C is %d\n", c);
// }

// int main(){
	// LL **a;
	// LL **b;
	// int c=10;

	// void (*fun_ptr)(LL**,LL**,int) = &fun;
	// MyDeque d;
	// // (*fun_ptr)(a,b,c);
	// d.push_back((*fun_ptr));
	// FP f=d.pop_back();
	// (*f)(a,b,c);

// 	return 0;

// }














