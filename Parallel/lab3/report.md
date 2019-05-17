## lab3  CUDA实现向量加法和矩阵乘法



### 3.1 实验题目

****

**1.向量加法**

向量加法。定义A,B两个一维数组，编写GPU程序将A和B对应项相加，将结果保存在数组C中。分别测试数组规模为10W、20W、100W、200W、1000W、2000W时其与CPU加法的运行时间之比。

**2.矩阵乘法**

矩阵乘法。定义A，B 两个二维数组。使用GPU 实现矩阵乘法。并对比串行程序，给出加速比。



### 3.2 实验环境说明

****

实验室提供GPU集群，可用本地PC远程连接，方法如下

**1.虚拟终端**

- 使用虚拟终端putty.exe

**2.登录**

- IP和端口:  202.38.69.241:39099
- 账号和密码: 账号为test001-test020共20个，密码统一为a123456，可自行选择，并在选择的账号下建立自己的子文件夹

**3.编写作业脚本**testt

- 登录账号: test001
- 自己的子文件夹: joce
- 需运行的文件: matrix_cuda.cu

```c
#PBS -N cudatestjob//cudatestjob是为你此次要运行的程序起的任务名
  #PBS -o /ghome/test001/joce/$PBS_JOBID.out//标准输出文件
  #PBS -e /ghome/test001/joce/$PBS_JOBID.err//错误输出文件
  #PBS -l nodes=G142:gpus=1//规定使用的节点数
  #PBS -r y //指明作业是否可运行，y 为可运行，n 为不可运行
  #PBS -q batch//
  cd $PBS_O_WORKDIR //原来直接在节点上运行时所在的目录
  echo Time is `date`
  echo Directory is $PWD
  echo This job runs on following nodes:
  echo -n "Node:"
  cat $PBS_NODEFILE
  echo -n "Gpus:"
  cat $PBS_GPUFILE
  echo "CUDA_VISIBLE_DEVICES:"$CUDA_VISIBLE_DEVICES
  startdocker -c "nvcc -o /ghome/test001/joce/matrix_cuda /ghome/test001/joce/matrix_cuda.cu"  bit:5000/deepo
  startdocker -c "/ghome/test001/joce/matrix_cuda"  bit:5000/deepo
```

**4.上传CUDA代码文件**

- 可以选择使用WinSCP工具(Window)或SCP命令(Linux)等可远程上传文件的手段将自己的代码文件和脚本上传至 “/ghome/< testname>/<用户名>/ “的目录下

- 也可以选择在joce文件夹下新建文件，需要用到的命令如下ls

  -新建文件: `touch matrix.cu `

  -打开vim: `vim matrix.cu`

  -粘贴系统剪切板内容: `shift+insert`

  -切换到命令模式: `Esc`

  -保存并退出vim: `:wq`

**5.提交脚本及验证**

- 在Putty的console中cd到自己的子文件夹，并输入: `qsub <脚本名>.pbs`以提交脚本

- 使用cat命令查看输出文件核对正确性



### 3.3 CUDA编程模型说明

****

**1.简介**

CUDA（Compute Unified Device Architecture）是显卡厂商NVIDIA推出的运算平台。 CUDA是一种由NVIDIA推出的通用并行计算架构，该架构使GPU能够解决复杂的计算问题。 它包含了CUDA指令集架构（ISA）以及GPU内部的并行计算引擎。 开发人员现在可以使用C语言来为CUDA架构编写程序，C语言是应用最广泛的一种高级编程语言。所编写出的程序于是就可以在支持CUDA的处理器上以超高性能运行。

**2.GPU架构**

1 * GPU = n * 流式处理器SM

1 * 流式处理器SM = m * 可同步执行的处理器SP + 共享存储 + cache +多线程指令发射单元

**3.存储器层次结构**

内存分为3个层次:

- 流式多处理器片上内存

  SP的寄存器/局部内存 + 共享存储 + cache

- GPU设备内存

  所有处理器均可访问GPU设备内存

- 主机内存

  是一般意义的内存，GPU设备不能直接访问主机内存，需要通过拷贝API传递数据

- 下图展示GPU设备内存与各流式多处理器片上内存结构

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab3/pic/1.png" style="zoom:50%">

**4.线程组织结构**

线程分为3个层次:

- 线程

  最基本的执行单元，每个线程执行相同代码

- 线程块(block)

  若干线程的分组，Block内一个块至多512个线程，线程块可以是一维、二维或者三维的

- 线程网络(grid)

  是若干线程块的网格，Grid是一维和二维的

线程ID的计算:

- 线程块内用局部ID标记threadID
- 线程块用线程块ID标记blockID
- 全局ID = blockID*blockDim + threadID

**5.CUDA程序结构**

CUDA程序的结构大体是：`{主机串行->GPU并行}+ -> 主机串行`，这样的串并交叉结构。主机串行过渡到GPU并行时需要将数据从主机内存上拷贝到GPU设备内存上，GPU执行完毕时也需要把数据拷贝回来。

**6.CUDA内核函数**

主机调用设备代码的唯一接口就是Kernel函数，使用限定符:`__global__`

调用内核函数需要在内核函数名后添加<<<>>>指定内核函数配置，如<<<DimGrid, DimBlock>>>指定线程网络和线程块维度。若当前硬件无法满足用户配置，则内核函数不会被执行，直接返回错误。

**7.CUDA限定符**

- 函数限定符

  | 函数限定符   | 在何处执行 | 从何处调用 | 特性                 |
  | ------------ | ---------- | ---------- | -------------------- |
  | __ device __ | 设备       | 设备       | 函数地址无法获取     |
  | __ global __ | 设备       | 主机       | 返回类型必须为空     |
  | __ host __   | 主机       | 主机       | 等于不使用任何限定符 |

- 变量限定符

  | 变量限定符     | 位于何处   | 可以访问的线程       | 主机访问         |
  | -------------- | ---------- | -------------------- | ---------------- |
  | __ device __   | 全局存储器 | 线程网格内的所有线程 | 通过运行时库访问 |
  | __ constant __ | 固定存储器 | 线程网格内的所有线程 | 通过运行时库访问 |
  | __ shared __   | 共享存储器 | 线程块内的所有线程   | 不可从主机访问   |

**8.同步**

- CPU启动kernel函数是异步的，它并不会阻塞等到GPU执行完kernel函数才执行后面的CPU部分，因此如果后续程序立即需要用到上一个kernel函数的结果我们需要显式设置同步障来阻塞CPU程序。
- 一个线程块内需要同步共享存储器的共享变量`（__ shared __）`时，需要在使用前显式调用`__syncthreads()`同步块内所有线程。
- 同一个Grid中不同Block之间无法设置同步。

**9.CUDA运行时API**

详见[官网文档](http://docs.nvidia.com/cuda/cuda-runtime-api/index.html)，以下为部分内存管理的函数

| 名称       | 简介                         |
| ---------- | ---------------------------- |
| cudaMemcpy | 用于在主机和设备之间拷贝数据 |
| cudaMalloc | 在设备上分配动态内存         |
| cudaFree   | 释放回收在设备上分配动态内存 |

```c
__host__ cudaError_t cudaMemcpy( 
    void* dst, 
    const void* src, 
    size_t count, 
    cudaMemcpyKind kind //枚举类型cudaMemcpyHostToDevice表示把主机数据拷贝到内存以及逆向cudaMemcpyDeviceToHost。
    )

__host__ __device__ cudaError_t cudaMalloc( 
    //两个限定符表示可以在主机或设备上调用
    void** devPtr, 
    size_t size 
    )

__host__ __device__ cudaError_t cudaFree( 
    //两个限定符表示可以在主机或设备上调用
    void* devPtr 
    )
```



### 3.4 算法说明

****

**1.CUDA向量加法**

- 实现长度为n的两个向量的加法，n由命令行运行参数确定

- 程序流程

  开始---------------------------------------------------------------

  **step1: 主机内存分配+数据初始化** 

  host_a: 源操作数1

  host_b: 源操作数2

  host_c: GPU并行计算的结果

  host_d: CPU串行计算的结果

  **step2: 设备内存分配**

  device_a: 存储host_a

  device_b: 存储host_b

  device_c: 存储计算结果

  **step3: 主机向设备拷贝数据**

  **step4: 并行执行**

  主机调用向量加法Kernel函数，执行并行向量加法，打印计算时间

  **step5: 主机串行执行**

  主机执行串行向量加法，打印计算时间

  **step6: 检查并行计算结果与串行计算结果是否一致** 

  结束----------------------------------------------------------------

**2.CUDA矩阵乘法**

- 实现维度为n*n的两个矩阵的乘法，n由命令行运行参数确定。
- 所有的二维矩阵都用一维数组表示，索引时计算对应下标。程序流程和向量加法几乎一模一样，不再重复说明



### 3.5 测试结果

****

**1.向量加法**

| 数据规模 | 并行时间(ms) | 串行时间(ms) | 加速比 |
| -------- | ------------ | ------------ | ------ |
| 10w      | 0.806        | 0.734        | 0.91   |
| 20w      | 1.691        | 1.609        | 0.95   |
| 100w     | 4.755        | 7.301        | 1.54   |
| 200w     | 9.156        | 14.427       | 1.58   |
| 1000w    | 51.483       | 73.855       | 1.43   |
| 2000w    | 94.890       | 144.828      | 1.53   |

**2.矩阵乘法**

| 数据规模 | 并行时间(ms) | 串行时间(ms) | 加速比 |
| -------- | ------------ | ------------ | ------ |
| 10       | 0.126        | 0.007        | 0.06   |
| 100      | 0.408        | 7.351        | 18.02  |
| 500      | 21.207       | 956.95       | 45.12  |
| 1000     | 155.342      | 5732.826     | 36.90  |





****

【ref】

- [GPU集群资料](http://mccipc.ustc.edu.cn/mediawiki/index.php/Gpu-cluster-manual)
- [thread索引的计算](https://blog.csdn.net/hujingshuang/article/details/53097222)