#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <cuda.h>

#define RANDOM(x) (rand() % x)

#define MAX 100000

#define BLOCKSIZE 16


//主机调用，设备执行，返回为空
__global__ void add(const int *a, const int *b, int *c, int n) {
    int i = (blockIdx.x * gridDim.x + blockIdx.y) * blockDim.x * blockDim.y + threadIdx.x * blockDim.x + threadIdx.y;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}


int main(int argc, char **argv) {
    int n = 512;
    int i;
    timeval start, finish;

    if (argc == 2) {
        n = atoi(argv[1]);
    }
	
	//主机内存分配
    int *host_a = (int *)malloc(sizeof(int) * n);
    int *host_b = (int *)malloc(sizeof(int) * n);
    int *host_c = (int *)malloc(sizeof(int) * n);
    int *host_d = (int *)malloc(sizeof(int) * n);
	
	//数据初始匿
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        host_a[i] = RANDOM(MAX);
        host_b[i] = RANDOM(MAX);
    }
	
	//设备内存分配
    cudaError_t error = cudaSuccess;
    int *device_a, *device_b, *device_c;
    error = cudaMalloc((void **)&device_a, sizeof(int) * n);
    error = cudaMalloc((void **)&device_b, sizeof(int) * n);
    error = cudaMalloc((void **)&device_c, sizeof(int) * n);
	
    if (error != cudaSuccess) {
        printf("Fail to cudaMalloc on GPU");
        return 1;
    }

//GPU parallel start
    gettimeofday(&start, 0);
	//主机向设备拷贝数捿
    cudaMemcpy(device_a, host_a, sizeof(int) * n, cudaMemcpyHostToDevice);
    cudaMemcpy(device_b, host_b, sizeof(int) * n, cudaMemcpyHostToDevice);
	
    int gridsize = (int)ceil(sqrt(ceil(n / (BLOCKSIZE * BLOCKSIZE))));

    dim3 dimBlock(BLOCKSIZE, BLOCKSIZE, 1);//设置参数
    dim3 dimGrid(gridsize, gridsize, 1);

    add<<<dimGrid, dimBlock>>>(device_a, device_b, device_c, n);//启动kernel
    cudaThreadSynchronize();

    cudaMemcpy(host_c, device_c, sizeof(int) * n, cudaMemcpyDeviceToHost);//结果拷贝到host

    gettimeofday(&finish, 0);

    double t = 1000000 * (finish.tv_sec - start.tv_sec) + finish.tv_usec - start.tv_usec;
    printf("GPU: %lf ms\n", t / 1000);
//GPU parallel finish


//CPU serial start
    gettimeofday(&start, 0);

    for (i = 0; i < n; i++) {
        host_d[i] = host_a[i] + host_b[i];
    }

    gettimeofday(&finish, 0);

    t = 1000000 * (finish.tv_sec - start.tv_sec) + finish.tv_usec - start.tv_usec;
    printf("CPU: %lf ms\n", t / 1000);
//CPU serial start

//check
    int errorNum = 0;
    for (int i = 0; i < n; i++) {
        if (host_c[i] != host_d[i]) {
            errorNum ++;
            printf("Error occurs at index: %d: a + b = %d + %d = %d, but c = %d, c2 = %d\n", i, host_a[i], host_b[i], host_a[i] + host_b[i], host_c[i], host_d[i]);
        }
    }
    if (errorNum == 0) {
        printf("Successfully run on GPU and CPU!\n");
    } else {
        printf("%d error(s) occurs!\n", errorNum);
    }

    free(host_a);
    free(host_b);
    free(host_c);
    free(host_d);

    cudaFree(device_a);
    cudaFree(device_b);
    cudaFree(device_c);

    return 0;
}
