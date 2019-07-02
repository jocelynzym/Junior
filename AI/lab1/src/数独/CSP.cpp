#include <iostream>
#include <ctime>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdlib.h>
using namespace std;

//一个2维数组
//代表2个规则：用于存储每行、每列数字占用情况。
//当值为0表示未占用，1表示已占用
//值域
int rule[2][10][11];
//一个二维数组，用于存储数独的数字分布状态
int sudoku[10][11];
//一个二维数组，代表数独在初始状态中空白的区域
int flag[10][11];
//1个2维数组，表示约束程度
//rule_level[0][i]表示第i行未填格子数
//rule_level[1][j]表示第j列未填格子数
int rule_level[2][10];

//1个2维数组，表示残留值域大小(第i行第j列格子合法取值数)
int remain_count[10][11];


int N,K;

int total_blank=0;
int cur_blank=0;
int recommend_i,recommend_j;

bool get=false;


int findNextValue(int rowIndex, int colIndex, int start) {
    for (int i = start; i < 9; i++) {
        if (rule[0][rowIndex][i]) continue;
        if (rule[1][colIndex][i]) continue;
        return i;
    }
    return -1;
}

bool inSudoku(int rowIndex, int colIndex, int value){
	for(int i=0;i<N;i++){
		if(value==sudoku[rowIndex][i]) return true;
		if(value==sudoku[i][colIndex]) return true;
	}
	return false;
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

void show_rule(){
	cout<<"行占用: "<<endl; 
	for(int j=0;j<K;j++)
			 cout<<j+1<<" ";
	cout<<endl;
	for(int i=0;i<N;i++){
		for(int j=0;j<K;j++)
			 cout<<rule[0][i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
	cout<<"列占用: "<<endl; 
	for(int j=0;j<K;j++)
			 cout<<j+1<<" ";
	cout<<endl;
	for(int i=0;i<N;i++){
		for(int j=0;j<K;j++)
			 cout<<rule[1][i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}


void H1(){
	//if(cur_blank==0) return;
	int min=K+1;
	int constraint=0;
	
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)
			if(sudoku[i][j]==0) {
				recommend_i=i;recommend_j=j;
			
				min = remain_count[i][j];
				constraint=rule_level[0][i]+rule_level[1][j]-2;
				i=N;
				break;
				}
	}
		
		
	
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(sudoku[i][j]==0){
				if(remain_count[i][j]<min){
				min = remain_count[i][j];
				recommend_i=i;
				recommend_j=j;
				constraint=rule_level[0][i]+rule_level[1][j]-2;
				}
				else if(remain_count[i][j]==min){
					if(constraint<rule_level[0][i]+rule_level[1][j]-2){
						recommend_i=i;
						recommend_j=j;
						constraint=rule_level[0][i]+rule_level[1][j]-2;
					}
				}
				
			} 
		}
	}
}//end H1


void H3(){
	
}



void update(int rowIndex, int colIndex, int value){
//update rule,sudoku,rule_level,remain_count
	if(	sudoku[rowIndex][colIndex]==value) return;
	int state=0;
	if(value>0){
		state=1;
		//update rule
    	rule[0][rowIndex][value-1] = state;
    	rule[1][colIndex][value-1] = state;
		//sudoku
		sudoku[rowIndex][colIndex]=value;
		//rule_level
		rule_level[0][rowIndex]--;
		rule_level[1][colIndex]--;
		//remain_count
		//remain_count[rowIndex][colIndex]=0;
		for(int k=0;k<N;k++){
			if(!inSudoku(rowIndex,k,value)&&k!=colIndex)
				remain_count[rowIndex][k]--;
			if(!inSudoku(k,colIndex,value) && k!=rowIndex)
				remain_count[k][colIndex]--;
		}
		//remain_count[rowIndex][colIndex]=0;
	}
	else if(value==0){
		//update rule
    	rule[0][rowIndex][sudoku[rowIndex][colIndex]-1] = 0;
    	rule[1][colIndex][sudoku[rowIndex][colIndex]-1] = 0;
		//sudoku
		sudoku[rowIndex][colIndex]=0;
		//rule_level
		rule_level[0][rowIndex]++;
		rule_level[1][colIndex]++;
		//remain_count
		for(int k=0;k<N;k++){
			if(k!=colIndex)
				remain_count[rowIndex][k]++;
			if(k!=rowIndex)
				remain_count[k][colIndex]++;
		}
	}
	
	
}


bool forward_checking(int rowIndex, int colIndex, int value){
	update(rowIndex,colIndex,value);
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(remain_count[i][j]==0 && sudoku[i][j]==0) 
				return false;
		}
	}
	return true;
}


bool solve(int rowIndex, int colIndex) {
	if(get) return true;
	H1();
	H3();
	if(cur_blank==0) {return true;}
	//对于所有可能取值
	for(int value=1;value<=K;value++){
		if(rule[0][recommend_i][value-1]==1 || rule[1][recommend_j][value-1]==1 )
			continue; 
		if(forward_checking(recommend_i,recommend_j,value)){
			cur_blank--;
			if(cur_blank==0) {get=true; show(); break; return true;}
			//show(); 
			solve(recommend_i, recommend_j);
			cur_blank++;
			update(recommend_i, recommend_j, 0);
		}
			if(get) break;
		
			update(recommend_i, recommend_j, 0);		
	}
	update(rowIndex, colIndex, 0);
	if(get) return true;
    return false;
}




void init(){
//init flag,rule,sudoku,rule_level,remain_count
	for(int i=0;i<10;i++){
		rule_level[0][i]=0;
		rule_level[1][i]=0;
		for(int j=0;j<11;j++){
			flag[i][j]=0;
			sudoku[i][j]=0;
			rule[0][i][j]=0;
			rule[1][i][j]=0;
			remain_count[i][j]=0;
		}		
	}
		
}





void readfile(){
//get the input matrix
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
	for(int i=0;i<N;i++){//init rule_level
		rule_level[0][i]=N-1;
		rule_level[1][i]=N-1;
		for(int j = 0; j < N; j++){	
			remain_count[i][j]=K;
		}
	}
	for (int i = 0; i < N; ++i) {
		ifs.getline(line,sizeof(line));
		cout<<line<<endl;//DEBUG
		stringstream word(line);
		for(int j = 0; j < N; ++j){
			word>>trans;
			sudoku[i][j]=atoi(trans.c_str());
			if(sudoku[i][j]!=0){//not zero
				flag[i][j]=1;
				rule[0][i][sudoku[i][j]-1]=1;
				rule[1][j][sudoku[i][j]-1]=1;
				//update rule_level
				rule_level[0][i]--;
				rule_level[1][j]--;
				//update remain_count
				if(!inSudoku(i,j,sudoku[i][j]))
				for(int k=0;k<N;k++){	
				remain_count[i][k]--;
				remain_count[k][j]--;
				}
				remain_count[i][j]=0;
			}
			else{//zero
				total_blank++;
			}
		}
    }
    ifs.close();
	cur_blank=total_blank;
	
}
 

int main() {
    long time1, time2;
    init();
	readfile();
    time1 = clock();
    if(!solve(0, 0)) cout<<"no result"<<endl;;//从(0,0)位置开始填数
    time2 = clock();
    cout << "求解过程共用" << time2 - time1 << "毫秒" << endl;
    return 0;
}






