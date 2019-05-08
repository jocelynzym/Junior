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
	
	#pragma omp parallel for reduction(+:sum) private(x) 
	
		for(i=0;i<=n;i++){
			x=i*h;
			sum+=4.0/(1.0+x*x);
		}
 
	pi = sum * h;
	printf("%lf\n",pi);
	
	return 0;
}



