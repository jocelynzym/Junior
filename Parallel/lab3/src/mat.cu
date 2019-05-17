#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <cuda.h>

#define RANDOM(x) (rand() % x)

#define MAX 100000

#define BLOCKSIZE 16

__global__ void multiply(const int *a, const int *b, int *c, int n) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    int col = blockIdx.y * blockDim.y + threadIdx.y;

    int k;
    int sum = 0;

    if (row < n && col < n) {
        for (k = 0; k < n; k++) {
            sum += a[row * n + k] * b[k * n + col];
        }

        c[row * n + col] = sum;
    }
}

int main(int argc, char **argv) {
    int n = 512;
    int i, j, k;
    timeval start, finish;

    if (argc == 2) {
        n = atoi(argv[1]);
    }

    int *host_a = (int *)malloc(sizeof(int) * n * n);
    int *host_b = (int *)malloc(sizeof(int) * n * n);
    int *host_c = (int *)malloc(sizeof(int) * n * n);
    int *host_c2 = (int *)malloc(sizeof(int) * n * n);

    srand(time(NULL));

    for (i = 0; i < n * n; i++) {
        host_a[i] = RANDOM(MAX);
        host_b[i] = RANDOM(MAX);
    }

    cudaError_t error = cudaSuccess;

    int *device_a, *device_b, *device_c;
    error = cudaMalloc((void **)&device_a, sizeof(int) * n * n);
    error = cudaMalloc((void **)&device_b, sizeof(int) * n * n);
    error = cudaMalloc((void **)&device_c, sizeof(int) * n * n);

    if (error != cudaSuccess) {
        printf("Fail to cudaMalloc on GPU");
        return 1;
    }

//GPU parallel start
    gettimeofday(&start, 0);

    cudaMemcpy(device_a, host_a, sizeof(int) * n * n, cudaMemcpyHostToDevice);
    cudaMemcpy(device_b, host_b, sizeof(int) * n * n, cudaMemcpyHostToDevice);

    double num = ceil(pow((double)n,2) / pow((double)BLOCKSIZE, 2));
    int gridsize = (int)ceil(sqrt(num));

    dim3 dimBlock(BLOCKSIZE, BLOCKSIZE, 1);
    dim3 dimGrid(gridsize, gridsize, 1);

    multiply<<<dimGrid, dimBlock>>>(device_a, device_b, device_c, n);
    cudaThreadSynchronize();

    cudaMemcpy(host_c, device_c, sizeof(int) * n * n, cudaMemcpyDeviceToHost);

    gettimeofday(&finish, 0);

    double t = 1000000 * (finish.tv_sec - start.tv_sec) + finish.tv_usec - start.tv_usec;
    printf("GPU: %lf ms\n", t / 1000);
//GPU parallel finish


//CPU serial start
    gettimeofday(&start, 0);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            host_c2[i * n + j] = 0;
            for (k = 0; k < n; k++) {
                host_c2[i * n + j] += host_a[i * n + k] * host_b[k * n + j];
            }
        }
    }

    gettimeofday(&finish, 0);

    t = 1000000 * (finish.tv_sec - start.tv_sec) + finish.tv_usec - start.tv_usec;
    printf("CPU: %lf ms\n", t / 1000);
//CPU serial start

//check
    int errorNum = 0;
    for (int i = 0; i < n * n; i++) {
        if (host_c[i] != host_c2[i]) {
            errorNum ++;
            printf("Error occurs at index: %d: c = %d, c2 = %d\n", i, host_c[i], host_c2[i]);
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
    free(host_c2);

    cudaFree(device_a);
    cudaFree(device_b);
    cudaFree(device_c);

    return 0;
}
