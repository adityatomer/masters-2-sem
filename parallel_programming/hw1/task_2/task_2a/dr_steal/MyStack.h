#ifndef MYSTACK_H
#define MYSTACK_H

#include<stdio.h>
#include <iostream>
#include <stack>
#include <mutex>
#include "basicheader.h"
using namespace std;

class MyStack{
	std::stack<SyncType*>stk;
	std::mutex m;

public:

	bool empty(){
		std::unique_lock<std::mutex>lock(m);
		return stk.empty();
	}

	int size(){
		std::unique_lock<std::mutex>lock(m);
		return stk.size();
	}

	SyncType* top(){
		std::unique_lock<std::mutex>lock(m);
		return stk.top();
	}

	void push(SyncType *st){
		std::unique_lock<std::mutex>lock(m);
		stk.push(st);
	}

	SyncType* pop(){
		std::unique_lock<std::mutex>lock(m);
		SyncType *st=stk.top();
		stk.pop();
		return st;
	}

	SyncType* decrementStackSyncObject(SyncType *st){
		std::unique_lock<std::mutex>lock(m);	
		if(st==NULL){
			return NULL;
		}
		(st->value)--;
		if(st->value==0){
			// cout<<"STACK Top address: "<<&stk.top()<<" top value: "<<stk.top()<<" pointer st address: "<<st<<" st.value: "<<st->value<<" ";
			stk.pop();
		}
		// cout<<"next top is: "<<stk.top()<<endl;
		return st;
	}

	

	~MyStack();
};

#endif



