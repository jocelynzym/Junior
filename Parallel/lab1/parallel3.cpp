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
	
	#pragma omp parallel private(x,i,sum) //����x,sumΪÿ���߳�˽��
	{
		int id;
		id = omp_get_thread_num();
		//printf("thread id:%d\n",id);
		for(i=id,sum=0.0;i<=n;i=i+NUM_THREADS){
			x=i*h;
			sum+=4.0/(1.0+x*x);
		}
	#pragma omp critical //ָ���������ͬһʱ��ֻ����һ���߳̽��в���
		pi += sum*h;
		//printf("sum%d:%lf\n",id,sum*h);
	}
	
	
	printf("%lf\n",pi);
	
	return 0;
}


