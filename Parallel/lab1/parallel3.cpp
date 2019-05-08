#include <stdio.h>
#include <omp.h>

static long n = 1000000;  
double h; 

#define NUM_THREADS 2


int main(){
	int i;
	h=1.0/double(n);
	double x=0.0,pi=0.0,sum=0.0;
	omp_set_num_threads(NUM_THREADS);
	
	#pragma omp parallel private(x,i,sum) //变量x,sum为每个线程私有
	{
		int id;
		id = omp_get_thread_num();
		//printf("thread id:%d\n",id);
		for(i=id,sum=0.0;i<=n;i=i+NUM_THREADS){
			x=i*h;
			sum+=4.0/(1.0+x*x);
		}
	#pragma omp critical //指定代码段在同一时刻只能由一个线程进行操作
		pi += sum*h;
		//printf("sum%d:%lf\n",id,sum*h);
	}
	
	
	printf("%lf\n",pi);
	
	return 0;
}


