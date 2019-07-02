#include <iostream>
#include <ctime>
#include <math.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdlib.h>
using namespace std;


//一个二维数组，用于存储数独的数字分布状态
int sudoku[10][11];
//一个二维数组，代表数独在初始状态中空白的区域
int flag[10][11];

int score[10];
int sum_score;

int N,K;

double T=100,T_min=0.1;
double R=0.9;


int F1(int rowIndex, int colIndex1, int colIndex2){
//计算交换后的总得分total_score，并更新score值
	int score1=0,score2=0;
	sum_score=sum_score-score[colIndex1]-score[colIndex2];
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(i==j) continue;
			else{
				if(sudoku[i][colIndex1]==sudoku[j][colIndex1]) score1++;
				if(sudoku[i][colIndex2]==sudoku[j][colIndex2]) score2++;
			} 
		}
	}
	score[colIndex1]=score1;
	score[colIndex2]=score2;
	sum_score=sum_score+score1+score2;
	return sum_score;
}


int F2(){
	
}


int swap(int rowIndex,int colIndex1, int colIndex2){
	int temp;
	temp = sudoku[rowIndex][colIndex1];
	sudoku[rowIndex][colIndex1]=sudoku[rowIndex][colIndex2];
	sudoku[rowIndex][colIndex2]=temp;
}




void show() {
	cout<<"result:"<<endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout<<sudoku[i][j]<<" ";
        }
        cout << endl;
    }
    cout<<endl;
}

void show_score() {
	cout<<"score:"<<endl;
	
    for (int i = 0; i < N; i++) {
            cout<<score[i]<<" ";
    }
    cout<<endl;
}




bool annealing(){
	int get_row,get_col1,get_col2;
	int f0,f1,df;
	int count=0;
	while(sum_score!=0){
		count++;//DEBUG
		//cout<<count<<endl;
		//if(count==100000){cout<<count<<endl; show(); show_score(); }
		//if(count==1000000) break;
		srand((int)time(0));
		get_row = rand() % (N);
		get_col1= rand() % (N);
		while(flag[get_row][get_col1]) get_col1= rand() % (N);
		get_col2= rand() % (N);
		while(flag[get_row][get_col2]) get_col2= rand() % (N);
		f0=sum_score;
		swap(get_row,get_col1,get_col2);
		f1=F1(get_row,get_col1,get_col2);
		df=f1-f0;
		if(df>0){
			double con=exp(-df/T);
			double p=(rand()%100000)*0.00001;
			if(con<=p){
				swap(get_row,get_col2,get_col1);
				F1(get_row,get_col1,get_col2);
			} 
		}
		T=R*T;
	}
	return true;
}



bool inRow(int rowIndex,int value){
	for(int i=0;i<N;i++)
		if(sudoku[rowIndex][i]==value) return true;
	return false;
}


void init1(){
//init score,sum_score,sudoku;
	//init sudoku,fill row
	srand((int)time(0));
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(sudoku[i][j]==0){
				while(1){
					int a = rand() % (K) + 1;
					if(!inRow(i,a)) {sudoku[i][j]=a; break;}
				}
			}
		}
	}
	//init score[10]
	for(int col=0;col<N;col++){
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				if(i==j) continue;
				else{
					if(sudoku[i][col]==sudoku[j][col]) score[col]++;
				} 
			}
		}		
	}
	//init sum_score
	for(int i=0;i<N;i++) sum_score+=score[i];
	show();
	show_score();
	
}


void readfile(){
//get the input matrix, init flag
	char line[50]={0};
	string trans;
	ifstream ifs("input.txt");
	if (ifs.fail())
	{cout<<"fail to open file"<<endl; exit(0);}  
	//set N,K
	ifs.getline(line,sizeof(line));
	cout<<line<<endl;//DEBUG
	stringstream word(line);
	word>>trans;
	N=atoi(trans.c_str());
	word>>trans;
	K=atoi(trans.c_str());
	//set matirx
	for (int i = 0; i < N; ++i) {
		ifs.getline(line,sizeof(line));
		cout<<line<<endl;//DEBUG
		stringstream word(line);
		for(int j = 0; j < N; ++j){
			word>>trans;
			sudoku[i][j]=atoi(trans.c_str());
			if(sudoku[i][j]!=0){//not zero
				flag[i][j]=1;
			}
		}
    }
    ifs.close();
}
 




int main() {
    long time1, time2;
    bool state;
	readfile();
	init1();
    time1 = clock();
    state=annealing();
    time2 = clock();
    if(state) {show(); show_score();}
	else cout<<"no result"<<endl;
    cout << "求解过程共用" << time2 - time1 << "毫秒" << endl;
    return 0;
}






