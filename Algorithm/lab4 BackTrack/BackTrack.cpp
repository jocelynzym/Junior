//#define NUM_TASK 10 
//#define NUM_MAC 3 
//#define NUM_TASK 15 
//#define NUM_MAC 5 

#define NUM_TASK 19
#define NUM_MAC 8
#include <iostream>
#include <climits>
using namespace std;
void output(int x[]);
void BackTrack(int task);
int getTime(int time_mac[]);
void output_assign(int best_x[]);
 

int x[NUM_TASK + 1];
int best_x[NUM_TASK+1];//存储最优分配方案
int min_t=INT_MAX;
//int t[NUM_TASK + 1] = { 0,47, 20, 28, 44, 21, 45, 30, 39, 28, 33};
//int t[NUM_TASK + 1] = { 0,98, 84, 50, 23, 32, 99, 22, 76, 72, 61, 81, 39, 76, 54, 37};
int t[NUM_TASK + 1] = { 0,39, 39, 23, 45, 100, 69, 21, 81, 39, 55, 20, 86, 34, 53, 58, 99, 36, 45, 46};

int time_mac[NUM_MAC + 1] = {0};//每个机器运行结束的时间
 
int main() {
	BackTrack(1);
 
	cout << "各个任务执行时间依次为：" << endl;
	for (int i = 1; i <= NUM_TASK; i++) {
		cout << t[i] << " ";
	}
	cout << endl;
	cout << "所需要的最小时间为："<<min_t << endl;
	//output(best_x);
	output_assign(best_x);
	return 0;
}
void BackTrack(int task) {
 
	if (task > NUM_TASK) {
		
		int cur_time = getTime(time_mac);//当前已分配任务的完成时间
		/*输出所有分配情况，以及对应的时间*/
		//output(x);
		//cout << "time=" << cur_time <<endl;
		
		if (cur_time < min_t) { //剪枝
			min_t = cur_time;
			for (int i = 1; i <=NUM_TASK; i++) {
				best_x[i] = x[i];
			}
		}
	}
	else{
		for (int i = 1; i <= NUM_MAC; i++) {
			int flag=0;
		    for(int j=i+1;j<=NUM_MAC; j++){
		    	if (time_mac[i]==time_mac[j]) {
		    		flag=1;
		    		break;
				}
			}
			if(flag==0){
				x[task] = i;
			time_mac[i] += t[task];
			if(time_mac[i]<min_t)
				BackTrack(task+1);
			time_mac[i] -= t[task];
			}
			
		}
	}
}
 
void output(int x[]) {
	for (int i = 1; i <= NUM_TASK; i++) {
		cout << x[i]<< " ";
	}
	cout << endl;
}
 
int getTime(int time_mac[]) {
	int max_time=time_mac[1];
	for (int i = 2; i <= NUM_MAC; i++) {
		if (time_mac[i] > max_time) {
			max_time = time_mac[i];
		}
	}
	return max_time;
}
 
void output_assign(int best_x[]) {
	for (int i = 1; i <= NUM_TASK; i++) {
		cout << "任务" << i << "分配给机器" << best_x[i] << endl;
	}

}
