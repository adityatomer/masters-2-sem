#include<stdio.h>
#include<iostream>
#include<math.h>
#include<time.h>
#include<array>
#include <ctime>
#include <stdlib.h>
using namespace std;
#define mod 10
typedef long long LL;

int getRandomNumber(long seed){
	int iSecret = rand() % mod + 1;
	return iSecret;
}

LL** getMatrixOfSizeR(long seed, int n,bool isRandom=true){
	LL **x=0;
	x=new LL*[n];
	for(int i=0;i<n;++i){
		x[i]=new LL[n];
		for(int j=0;j<n;++j){
			if(isRandom){
				x[i][j]=1;//getRandomNumber(seed);	
			}else{
				x[i][j]=0;
			}
			
		}
	}
	return x;
}

void printMat(LL **mat,int n){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			cout<<mat[i][j]<<" ";
		}
		cout<<endl;
	}
}

void getMul_IKJ(LL **z,LL **x, LL **y,int mat_A_row_st, int mat_A_col_st, int mat_B_row_st, int mat_B_col_st,LL n){
	cout<<"getMul_IKJ mat_A_row_st: "<<mat_A_row_st<<" mat_A_col_st: "<<mat_A_col_st<<" mat_B_col_st: "<<mat_B_row_st<<" mat_B_col_st:" <<mat_B_col_st<<" n: "<<n<<endl;
	printMat(z,n);
	cout<<"--------\n";
	printMat(x,n);
	cout<<"--------\n";
	printMat(y,n);
	
	for(int i=0;i<n;++i){
		for(int k=0;k<n;++k){
			for(int j=0;j<n;++j){

				z[mat_A_row_st+i][mat_B_col_st+j]+=x[i][k]*y[k][j];
				cout<<"i: "<<mat_A_row_st+i<<" "<<"j: "<<mat_B_col_st+j<<" value: "<<z[mat_A_row_st+i][mat_B_col_st+j]<<endl;
			}
			
		}
	}
}

LL** getMul_KIJ(LL **x, LL **y,LL n){
	LL **z;
	for(int k=0;k<n;++k){
		for(int i=0;i<n;++i){
			for(int j=0;j<n;++j){
				z[i][j]+=x[i][k]*y[k][j];
			}
		}
	}
	printMat(z,n);
	return z;
}


LL** getNewMatOfSmallerSize(LL **orignalMat, int x,int y, int n){
	LL **z=0;
	z=new LL*[n];
	int ind_x=0;
	int ind_y=0;

	for(int i=x;i<x+n;++i){
		z[ind_x]=new LL[n];
		ind_y=0;
		for(int j=y;j<y+n;++j){
			z[ind_x][ind_y++]=orignalMat[i][j];
		}
		ind_x++;
	}
	return z;
}

void sum(LL **z, LL **x, LL **y, int row, int col,int n){
	int rowInd=0;
	int colInd=0;
	for(int i=row;i<row+n/2;++i){
		colInd=0;
		for(int j=col;j<col+n/2;++j){
			z[i][j]=x[rowInd][colInd]+y[rowInd][colInd];
			colInd++;
		}
		rowInd++;
	}
}

void ParRecMM(LL **Z, LL **x, LL **y, int mat_A_row, int mat_A_col, int mat_B_row, int mat_B_col, int n){
	if(n==2){
		// getMul_IKJ(Z,x,y,mat_A_row,mat_A_col,mat_B_row,mat_B_col,n);
		return;
	}
	LL **x11=getNewMatOfSmallerSize(x,0,0,n/2);
	LL **x12=getNewMatOfSmallerSize(x,0,n/2,n/2);
	LL **x21=getNewMatOfSmallerSize(x,n/2,0,n/2);
	LL **x22=getNewMatOfSmallerSize(x,n/2,n/2,n/2);
	LL **y11=getNewMatOfSmallerSize(y,0,0,n/2);
	LL **y12=getNewMatOfSmallerSize(y,0,n/2,n/2); cilk::current_worker_id()
	LL **y21=getNewMatOfSmallerSize(y,n/2,0,n/2);
	LL **y22=getNewMatOfSmallerSize(y,n/2,n/2,n/2);
	// LL** temp = getMatrixOfSizeR(0,n,false);

	ParRecMM(Z, x11, y11, 0,0,0,0,n/2);
	// ParRecMM(Z, x11, y12, 0,0,0,n/2,n/2);
 //    ParRecMM(Z, x21, y11, n/2,0,0,0,n/2);
	// ParRecMM(Z, x21, y12, n/2,0,0,n/2,n/2);
	// ParRecMM(Z, x12, y21, 0,n/2,n/2,0,n/2);
	// ParRecMM(Z, x12, y22, 0,n/2,n/2,n/2,n/2);
	// ParRecMM(Z, x22, y21, n/2,n/2,n/2,0,n/2);
	// ParRecMM(Z, x22, y22, n/2,n/2,n/2,n/2,n/2);
	// printMat(x,n);
	// printMat(y,n);
	
	getMul_IKJ(Z, x, y, mat_A_row, mat_A_col, mat_B_row, mat_B_col, n);

}


int main(){
	int n=8;
	time_t seedTime = 0;
	LL **x = getMatrixOfSizeR(seedTime,n);
	seedTime = 0;
	LL **y = getMatrixOfSizeR(seedTime,n);
	LL** Z=getMatrixOfSizeR(0,n,false);

	ParRecMM(Z,x,y,0,0,0,0,n);
	printMat(x,n);
	cout<<"----------"<<endl;
	printMat(y,n);
	cout<<"----------"<<endl;
	printMat(Z,n);
	return 0;

}

