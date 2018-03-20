#include <iostream>
#include <stack>
#include "headers.h"
#include<stdio.h>
using namespace std;

class MyStack{
	std::stack<SyncType>stk;
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

	SyncType top(){
		std::unique_lock<std::mutex>lock(m);
		return stk.top();
	}

	void push(SyncType st){
		std::unique_lock<std::mutex>lock(m);
		stk.push(st);
	}

	SyncType pop(){
		std::unique_lock<std::mutex>lock(m);
		SyncType st=stk.top();
		stk.pop();
		return st;
	}

	~MyStack();
};

// int main(){
// 	MyStack s;
// 	SyncType st;
// 	st.type=1;
// 	st.value=4;
// 	s.push(st);


// 	if(s.empty()){
// 		cout<<"stack is empty";
// 		return 0;
// 	}else{
// 		cout<<"stack is not empty";
// 	}

// 	SyncType st1=s.pop();
// 	cout<<"type: "<<st1.type<<" value: "<<st1.value;
// 	return 0;
// }





