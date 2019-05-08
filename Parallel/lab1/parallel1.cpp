#include <stdio.h>
#include <omp.h>

static long n = 1000000;  
double h; 

#define NUM_THREADS 2

int main(){
	int i;
	h=1.0/double(n);
	double x,pi,sum[NUM_THREADS];
	
	omp_set_num_threads(NUM_THREADS);//设置2线程
	
	#pragma omp parallel private(i) //并行域开始,每个线程都会执行该代码
	{
		double x;
		int id;
		id = omp_get_thread_num();
		for(i=id,sum[id]=0.0;i<n;i=i+NUM_THREADS){
			x=i*h;
			sum[id]+=4.0/(1.0+x*x);
		}
	}
	
	for(i=0,pi=0.0;i<NUM_THREADS;i++) 
		pi+=sum[i]*h;
	
	
	printf("%lf\n",pi);
	return 0;
}

