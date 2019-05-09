#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include "mpi.h"

int i,j,k;

int cmp(const void * a, const void * b) {
  if (*(int*)a < *(int*)b) return -1;
  if (*(int*)a > *(int*)b) return 1;
  else return 0;
}



void phase1(int *array, int N, int startIndex, int subArraySize, int *pivots, int p) {
  // 局部排序
  qsort(array + startIndex, subArraySize, sizeof(array[0]), cmp);

  // 正则采样
  for (i = 0; i < p; i++) {
    pivots[i] = array[startIndex + (i * (N / (p * p)))];    
  }
  return;
}


//采样排序+选择主元+主元划分
void phase2(int *array, int startIndex, int subArraySize, int *pivots, int *partitionSizes, int p, int myId) {
  int *collectedPivots = (int *) malloc(p * p * sizeof(pivots[0]));
  int *phase2Pivots = (int *) malloc((p - 1) * sizeof(pivots[0]));        
  int index = 0;


  MPI_Gather(pivots, p, MPI_INT, collectedPivots, p, MPI_INT, 0, MPI_COMM_WORLD);       
  if (myId == 0) {
	//采样排序: collectedPivots[p*p]存放全体主元，对其快排序
    qsort(collectedPivots, p * p, sizeof(pivots[0]), cmp);          
    //选择主元: phase2Pivots[p-1]存放选出的主元  
    for (i = 0; i < (p -1); i++) {
      phase2Pivots[i] = collectedPivots[(((i+1) * p) + (p / 2)) - 1];
    }
  }
  MPI_Bcast(phase2Pivots, p - 1, MPI_INT, 0, MPI_COMM_WORLD);//root进程向各进程发送选出的主元
  
  //主元划分:partitionSizes[p-1]记录各线程的划分位置
  for ( i = 0; i < subArraySize; i++) {
    if (array[startIndex + i] > phase2Pivots[index]) {
      index += 1;
    }
    if (index == p) {
      partitionSizes[p - 1] = subArraySize - i + 1;
      break;
    }
    partitionSizes[index]++ ;
  }
  free(collectedPivots);
  free(phase2Pivots);
  return;
}


//全局交换
void phase3(int *array, int startIndex, int *partitionSizes, int **newPartitions, int *newPartitionSizes, int p) {
  int totalSize = 0;
  int *sendDisp = (int *) malloc(p * sizeof(int));
  int *recvDisp = (int *) malloc(p * sizeof(int));

  MPI_Alltoall(partitionSizes, 1, MPI_INT, newPartitionSizes, 1, MPI_INT, MPI_COMM_WORLD);
  
  for ( i = 0; i < p; i++) {//计算每一类划分的元素数量
    totalSize += newPartitionSizes[i];
  }
  *newPartitions = (int *) malloc(totalSize * sizeof(int));

  sendDisp[0] = 0; //计算位移
  recvDisp[0] = 0;      
  for ( i = 1; i < p; i++) {
    sendDisp[i] = partitionSizes[i - 1] + sendDisp[i - 1];
    recvDisp[i] = newPartitionSizes[i - 1] + recvDisp[i - 1];
  }

  MPI_Alltoallv(&(array[startIndex]), partitionSizes, sendDisp, MPI_INT, *newPartitions, newPartitionSizes, recvDisp, MPI_INT, MPI_COMM_WORLD);

  free(sendDisp);
  free(recvDisp);
  return;
}


//归并排序
void phase4(int *partitions, int *partitionSizes, int p, int myId, int *array) {
  /*对partitions数组进行归并排序,即得到最终结果*/
  int *sortedSubList;
  int *recvDisp, *indexes, *partitionEnds, *subListSizes, totalListSize;

  indexes = (int *) malloc(p * sizeof(int));
  partitionEnds = (int *) malloc(p * sizeof(int));
  indexes[0] = 0;
  totalListSize = partitionSizes[0];
  
  for ( i = 1; i < p; i++) {
    totalListSize += partitionSizes[i];
    indexes[i] = indexes[i-1] + partitionSizes[i-1];
    partitionEnds[i-1] = indexes[i];
  }
  partitionEnds[p - 1] = totalListSize;

  sortedSubList = (int *) malloc(totalListSize * sizeof(int));
  subListSizes = (int *) malloc(p * sizeof(int));
  recvDisp = (int *) malloc(p * sizeof(int));

  // 每个线程分别归并排序，得到的有序结果放于sortedSubList[]数组中
  for ( i = 0; i < totalListSize; i++) {
    int lowest = INT_MAX;
    int ind = -1;
    for (j = 0; j < p; j++) {
      if ((indexes[j] < partitionEnds[j]) && (partitions[indexes[j]] < lowest)) {
		lowest = partitions[indexes[j]];
		ind = j;
      }
    }
    sortedSubList[i] = lowest;
    indexes[ind] += 1;
  }

  // 发送各子列表的大小回根进程中
  MPI_Gather(&totalListSize, 1, MPI_INT, subListSizes, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // 计算根进程上的相对于recvbuf的偏移量
  if (myId == 0) {
    recvDisp[0] = 0;
    for ( i = 1; i < p; i++) {
      recvDisp[i] = subListSizes[i - 1] + recvDisp[i - 1];
    }
  }

  //发送各排好序的子列表回根进程中
  MPI_Gatherv(sortedSubList, totalListSize, MPI_INT, array, subListSizes, recvDisp, MPI_INT, 0, MPI_COMM_WORLD);

  free(partitionEnds);
  free(sortedSubList);
  free(indexes);
  free(subListSizes);
  free(recvDisp);
  return;
}

//PSRS排序函数，调用了4个过程函数
void psrs_mpi(int *array, int N)    
{
    int p, myId, *partitionSizes, *newPartitionSizes, nameLength;
    int subArraySize, startIndex, endIndex, *pivots, *newPartitions;
    char processorName[MPI_MAX_PROCESSOR_NAME];


    MPI_Comm_size(MPI_COMM_WORLD,&p);// 进程数 p
    MPI_Comm_rank(MPI_COMM_WORLD,&myId);// 进程编号 myID
    MPI_Get_processor_name(processorName,&nameLength);//进程名 nameLength

    printf("Process %d is on %s\n",myId, processorName);

    pivots = (int *) malloc(p*sizeof(int));//每个进程采集p个样本到pivots[p]中
    partitionSizes = (int *) malloc(p*sizeof(int)); //记录各线程的划分位置
    newPartitionSizes = (int *) malloc(p*sizeof(int)); 
    for ( k = 0; k < p; k++) {
      partitionSizes[k] = 0;
    }

    //1.均匀划分
    startIndex = myId * N / p;
    if (p == (myId + 1)) {
      endIndex = N;
    } 
    else {
      endIndex = (myId + 1) * N / p;
    }
    subArraySize = endIndex - startIndex;
    MPI_Barrier(MPI_COMM_WORLD);
	
    //2.局部排序,3.正则采样
    phase1(array, N, startIndex, subArraySize, pivots, p); //得到每个线程的pivots值，存放采集的主元
    if (p > 1) {
	  //4.采样排序,5.选择主元,6.主元划分
      phase2(array, startIndex, subArraySize, pivots, partitionSizes, p, myId); //得到partitionSizes值,记录各线程的划分位置
	  //7.全局交换
      phase3(array, startIndex, partitionSizes, &newPartitions, newPartitionSizes, p);//得到newPartitions[],存放全局交换后的结果
	  //8.归并排序
      phase4(newPartitions, newPartitionSizes, p, myId, array);
    }

    if (myId == 0) 
     for(k = 0; k < N; k++){
        printf("%d ",array[k]);
     }
     printf("\n");
    if (p > 1) {
      free(newPartitions);
    }
    free(partitionSizes);
    free(newPartitionSizes);
    free(pivots);


  free(array);
  MPI_Finalize();

}

int main(int argc, char *argv[]) {

    int array[27] = { 15,46,48,93,39,6,72,91,14,  36,69,40,89,61,97,12,21,54,  53,97,84,58,32,27,33,72,20 };
    MPI_Init(&argc,&argv);      //MPI初始化
    psrs_mpi(array,27);          //调用PSRS算法进行并行排序

  return 0;
}
