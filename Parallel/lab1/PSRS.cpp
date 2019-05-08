#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<omp.h>

using namespace std;

#define num_threads 3

int *L,*R;

//Merge函数合并两个子数组形成单一的已排好序的字数组
//并代替当前的子数组A[p..r] 
void Merge(int *a, int p, int q, int r)
{
    int i,j,k;
    int n1 = q - p + 1;
    int n2 = r - q;
    L = (int*)malloc((n1+1)*sizeof(int));
    R = (int*)malloc((n2+1)*sizeof(int));
    for(i=0; i<n1; i++)
        L[i] = a[p+i];
    L[i] = 65536;
    for(j=0; j<n2; j++)
        R[j] = a[q+j+1];
    R[j] = 65536;
    i=0,j=0;
    for(k=p; k<=r; k++){
        if(L[i]<=R[j]){
            a[k] = L[i];
            i++;
        }
        else{
            a[k] = R[j];
            j++;
        }
    }
} 
//归并排序
void MergeSort(int *a, int p, int r)
{
    if(p<r){
        int q = (p+r)/2;
        MergeSort(a,p,q);
        MergeSort(a,q+1,r);
        Merge(a,p,q,r);
    }
} 

void PSRS(int *array, int n)
{
    int id;
    int i=0;
    int count[num_threads][num_threads] = { 0 };    //表示线程id的第m类有count[id][m]个元素 
    int base = n / num_threads;     
    int p[num_threads*num_threads]; 
    int pivot[num_threads-1];       
    int pivot_array[num_threads][num_threads][50]={0};  

    omp_set_num_threads(num_threads);
#pragma omp parallel shared(base,array,n,i,p,pivot,count) private(id)
    {
        id = omp_get_thread_num();
        //段内串行排序 
        MergeSort(array,id*base,(id+1)*base-1);






//正则采样 
#pragma omp critical
        for(int k=0; k<num_threads; k++)
            p[i++] = array[(id)*base+(k)*base/(num_threads)];

#pragma omp barrier

//p个样本进行排序+选出主元 
#pragma omp master
        {
            MergeSort(p,0,i-1);
            for(int m=0; m<num_threads-1; m++)
                pivot[m] = p[(m+1)*num_threads];
        }
#pragma omp barrier


        //主元划分
        for(int k=0; k<base; k++)
        {
            for(int m=0; m<num_threads; m++)
            {
                if(array[id*base+k] <= pivot[m])
                {
                    pivot_array[id][m][count[id][m]++] = array[id*base+k];
                    break;
                }
                else if(m == num_threads-1) 
                {
                    pivot_array[id][m][count[id][m]++] = array[id*base+k];
                }
            }
        }
    }

//全局交换+段内归并排序 
#pragma omp parallel shared(pivot_array,count)
    {
        int id=omp_get_thread_num();
        for(int k=0; k<num_threads; k++) 
        {
            if(k!=id)
            {
                memcpy(pivot_array[id][id]+count[id][id],pivot_array[k][id],sizeof(int)*count[k][id]);
                count[id][id] += count[k][id];
            }
        }
        MergeSort(pivot_array[id][id],0,count[id][id]-1);
    }
    
   
    printf("result:\n");
    for(int k=0; k<num_threads; k++)
    {
        for(int m=0; m<count[k][k]; m++)
        {
            printf("%d ",pivot_array[k][k][m]);
        }
        printf("\n");
    }
}

int main()
{
    int array[27] = { 15,46,48,93,39,6,72,91,14,  36,69,40,89,61,97,12,21,54,  53,97,84,58,32,27,33,72,20 };
    PSRS(array, 27);
    return 0;
}
