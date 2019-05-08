#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

#define N 9

int A[N][N]{ 31,-13,0,0,0,-10,0,0,0,
		-13,35,-9,0,-11,0,0,0,0,
		0,-9,31,-10,0,0,0,0,0,
		0,0,-10,79,-30,0,0,0,-9,
		0,0,0,-30,57,-7,0,-5,0,
		0,0,0,0,-7,47,-30,0,0,
		0,0,0,0,0,-30,41,0,0,
		0,0,0,0,-5,0,0,27,-2,
		0,0,0,-9,0,0,0,-2,29
		};
int b[N]={-15,27,-23,0,-20,12,-7,7,10};
	
	
double x_k[N]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
double x_k1[N]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

double control(){
	double max=0.0,temp=0.0;
	for(int i=0;i<N;i++){
		temp=x_k1[i]-x_k[i];
		if(temp<0) temp=-temp;
		if(temp>max)
			max=temp;
	}
	//cout<<"max:"<<max<<endl;
	return max;
}



void Jacobi(){
	int count=0;
	while(1){
		count++;
		for(int i=0;i<N;i++){
			double sum=0.0;
			for(int j=0;j<N;j++){
				if(i!=j){
					sum+=(double)A[i][j]*x_k[j];
				}
			}
			x_k1[i]=-(sum-(double)b[i])/(double)A[i][i];	
		}
		
		if(control()<=(double)1e-5) break;
		
		for(int i=0;i<N;i++){
			x_k[i]=x_k1[i];
		}
		
	}
	
	cout<<"迭代次数:"<<count<<endl;
	cout<<"数值解:"<<endl;
	for(int i=0;i<N;i++){
			cout<<x_k1[i]<< " ";
		}
	
}


void Guass(){
	int count=0;
	double x_k_temp[N];
	while(1){
		count++;
		
		for(int i=0;i<N;i++){
				x_k_temp[i]=x_k[i];
			}
		
		for(int i=0;i<N;i++){
			double sum=0.0;
			for(int j=0;j<N;j++){
				if(i!=j){
					sum+=(double)A[i][j]*x_k_temp[j];
				}
			}
			x_k1[i]=-(sum-(double)b[i])/(double)A[i][i];
			x_k_temp[i]=x_k1[i];
		}
		
		if(control()<=(double)1e-5) break;
		
		for(int i=0;i<N;i++){
			x_k[i]=x_k1[i];
		}
		
	}
	
	cout<<"迭代次数:"<<count<<endl;
	cout<<"数值解:"<<endl;
	for(int i=0;i<N;i++){
			cout<<x_k1[i]<< " ";
		}
	
}


int main(){

	
	//Jacobi();
	Guass();
	
	return 0;
}


