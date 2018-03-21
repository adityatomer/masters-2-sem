#include<stdio.h>
#include<iostream>
#include<math.h>
#include<time.h>
#include<array>
#include <ctime>
#include <stdlib.h>
#include "headers.h"
using namespace std;
#define mod 10


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
				x[i][j]=j;
			}else{
				x[i][j]=0;
			}
			
		}
	}
	return x;
}


void getMul_IKJ(LL **z,LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, LL n){
	for(int i=0;i<n;++i){
		for(int k=0;k<n;++k){
			for(int j=0;j<n;++j){
				z[z_row+i][z_col+j]+=x[x_row+i][x_col+k]*y[y_row+k][y_col+j];
			}
		}
	}
}

void getMul(LL **z,LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, LL n){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			for(int k=0;k<n;++k){
				z[z_row+i][z_col+j]+=x[i][k]*y[k][j];
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


// LL** getNewMatOfSmallerSize(LL **orignalMat, int x,int y, int n){
// 	LL **z=0;
// 	z=new LL*[n];
// 	int ind_x=0;
// 	int ind_y=0;

// 	for(int i=x;i<x+n;++i){
// 		z[ind_x]=new LL[n];
// 		ind_y=0;
// 		for(int j=y;j<y+n;++j){
// 			z[ind_x][ind_y++]=orignalMat[i][j];
// 		}
// 		ind_x++;
// 	}
// 	return z;
// }

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

void ParRecMM(LL** z, LL **x, LL **y,int z_row,int z_col, int x_row,int x_col,int y_row,int y_col, int n){
	if(n==1){
		return getMul_IKJ(z,x,y,z_row, z_col, x_row, x_col, y_row, y_col,n);
	}
 	// par: (X_11, U_11, V_11), (X_12, U_11, V_12), (X_21, U_21, V_11), (X_22, U_21, V_12)
	ParRecMM(z, x, y, z_row, z_col, x_row, x_col, y_row, y_col, n/2);
	ParRecMM(z, x, y, z_row, z_col+n/2, x_row, x_col, y_row, y_col+n/2, n/2);
    ParRecMM(z, x, y, z_row+n/2, z_col, x_row+n/2, x_col, y_row, y_col, n/2);
	ParRecMM(z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col, y_row, y_col+n/2, n/2);
	
	// par: (X_11, U_12, V_21), (X_12, U_12, V_22), (X_21, U_22, V_21), (X_22, U_22, V_22)
	ParRecMM(z, x, y, z_row, z_col, x_row, x_col+n/2, y_row+n/2, y_col, n/2);
	ParRecMM(z, x, y, z_row, z_col+n/2, x_row, x_col+n/2, y_row+n/2, y_col+n/2, n/2);
	ParRecMM(z, x, y, z_row+n/2, z_col, x_row+n/2, x_col+n/2, y_row+n/2, y_col, n/2);
	ParRecMM(z, x, y, z_row+n/2, z_col+n/2, x_row+n/2, x_col+n/2, y_row+n/2, y_col+n/2, n/2);
	
}

int main(){
	int n=4;
	time_t seedTime = time(0);
	LL **x = getMatrixOfSizeR(seedTime,n);
	seedTime = time(0);
	LL **y = getMatrixOfSizeR(seedTime,n);
	LL **z = getMatrixOfSizeR(seedTime,n,false);
	ParRecMM(z,x,y,0,0,0,0,0,0,n);
	printMat(x,n);
	cout<<"--------------"<<endl;
	printMat(y,n);
	cout<<"---result----"<<endl;
	printMat(z,n);
	return 0;

}