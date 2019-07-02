#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <iomanip>

using namespace std;
#define ROWSIZE 5
#define COLUMNSIZE 5
 
const unsigned int M = 250;


int dir[4][2] = { {-1, 0},//up
                   {1,  0},//down
                   {0,  -1},//left
                   {0,  1}//right
				};


typedef struct STATUS{
    int data[ROWSIZE][COLUMNSIZE];
    int zeroRow,zeroColumn;     
}STATUS;


char dirCode[] = {"UDLR"};
char rDirCode[] = {"DURL"};
char path[M]; // 最优解
STATUS start, target; // 起始和终止状态
int maxDepth = 0; // 深度边界
 
 

int H2(STATUS status1, STATUS status2){
	int h = 0;  
	 for (int i = 0; i < ROWSIZE; ++i)
        for (int j = 0; j < COLUMNSIZE; ++j) {
            if (status1.data[i][j] != status2.data[i][j])
                ++h;
        }
	return h;   
}




//IDAstar算法开始
bool dfs(STATUS cur,int depth,int h,char preDir){
	
	//IDA*估值函数剪枝
	//当前局面的估价函数值+当前的搜索深度 > 预定义的最大搜索深度时剪枝
	if( (depth+h>maxDepth)||depth>100) return false;
 
	 if(memcmp(&cur, &target, sizeof(STATUS)) == 0 )  
    {
        path[depth] = '\0';  
        return true;  
    }  
 
	STATUS next;
	for(int i=0;i<4;++i){
		if(dirCode[i]==preDir) continue;//不能回到上一状态
		next=cur;
		int row,column;
		row = cur.zeroRow + dir[i][0];  
        column = cur.zeroColumn + dir[i][1];  
		//处理4个缺口
		if(row==2 && column==-1)	
			{row=2; column=COLUMNSIZE-1;}
		if(row==2 && column == COLUMNSIZE )	
			{row=2; column=0;}
		if(row==-1 && column==2)	
			{row=ROWSIZE-1; column==2;}
		if(row==ROWSIZE && column==2)	
			{row=0; column==2;}
		next.zeroRow=row;
		next.zeroColumn=column;
		//状态不合法则回退
        if( !( next.zeroRow >= 0 && next.zeroRow < ROWSIZE && next.zeroColumn >= 0 && next.zeroColumn < COLUMNSIZE ) )  
            continue;  
 
		swap(next.data[cur.zeroRow][cur.zeroColumn], next.data[next.zeroRow][next.zeroColumn]); //置换变成新的状态
		int nexth=H2(next,target);//重新计算h值
		path[depth] = dirCode[i];  
        if(dfs(next, depth + 1, nexth, rDirCode[i]))  
            return true;  
	}
	return false;
}


int IDAstar(){
	int h = H2(start,target);  
	cout<<"h:"<<h<<endl;
    maxDepth = h;  
    while (!dfs(start,0, h, '\0')) 
        maxDepth++;  
    return maxDepth;  
}


//初始状态赋值
void input_start(){
	char line[50]={0};
	string trans;
	ifstream ifs("input.txt");
	//打开文件失败
	if (ifs.fail())  
    {  
       cout<<"fail to open file"<<endl;
	   exit(0);
    }  
    	
	for (int i = 0; i < ROWSIZE; i++) {
		ifs.getline(line,sizeof(line));
		stringstream word(line);
		for(int j=0;j<COLUMNSIZE;j++){
			word>>trans;
			start.data[i][j]=atoi(trans.c_str());
			if(start.data[i][j]==0){start.zeroRow=i;start.zeroColumn=j;} 
		}
    }
    
    ifs.close();
}

//目标状态赋值
void input_target(){
	char line[50]={0};
	string trans;
	ifstream ifs("target.txt");
	//打开文件失败
	if (ifs.fail())  
    {  
       cout<<"fail to open file"<<endl;
	   exit(0);
    }  
    	
	for (int i = 0; i < ROWSIZE; i++) {
		ifs.getline(line,sizeof(line));
		stringstream word(line);
		for(int j=0;j<COLUMNSIZE;j++){
			word>>trans;
			target.data[i][j]=atoi(trans.c_str());
			if(target.data[i][j]==0){target.zeroRow=i; target.zeroColumn=j;} 
		}
    }
    
    ifs.close();
}

int main(){
	input_start();
	input_target();
	
	//DEBUG:打印初始状态start
	for(int i=0;i<ROWSIZE;i++){
		for(int j=0;j<COLUMNSIZE;j++){
			cout<<start.data[i][j]<<" ";
		} 
		cout<<endl;
	} 
	 clock_t begin,end;
    double cost;   
	cout<<"开始执行"<<endl;
		begin=clock();
		IDAstar(); 
		end=clock();
		cost=(double)(end-begin)/CLOCKS_PER_SEC;
    	printf("%lfs\n",cost);
        cout<<path<<endl;
		cout<<strlen(path)<<endl;  

	return 0;
}

