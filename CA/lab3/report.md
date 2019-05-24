## lab3  cache设计和实现



### 一.实验要求

- **阶段一**: 将直接映射策略的cache修改为N路组相连的cache

  -宏定义设置组相连度	-写回 & 带写分配	-FIFO和LRU两种替换策略

  -使用python脚本生成一个新的testbench，并对自己的cache进行验证

  -修改组相连度等参数进行多次验证

- **阶段二**: 使用阶段一编写的N路组相联cache, 正确运行测试程序

- **阶段三**: 对不同cache策略和参数进行性能和资源的测试评估	

### 二.实验环境和工具

- vivado自带的波形仿真工具
- notapad++ 

### 三.实验内容

#### 阶段一 编写cache并验证正确性

****

 **1.cache设计说明**

原本给定 `cache.sv` 文件中已实现一个简单cache (直接映射策略,写回带写分配)。现需将直接映射修改为N路组相联，并增加替换策略，设计方案如下

**N路组相联的实现**

- cache结构数组添加一个维度，该维度的大小为 WAY_CNT, 表示一个set里有多少行

  ```verilog
  reg [31:0] cache_mem [SET_SIZE][WAY_CNT][LINE_SIZE];
  reg [TAG_ADDR_LEN-1:0] cache_tags   [SET_SIZE][WAY_CNT];
  reg valid  [SET_SIZE][WAY_CNT];
  reg dirty  [SET_SIZE][WAY_CNT];
  ```

- 新增变量

  ```verilog
  reg     [ WAY_CNT-1 : 0 ]   way_addr;   // 路地址
  reg     [ WAY_CNT-1 : 0 ]   out_way;    // 扇出路
  ```

- 实现并行命中判断：为了判断是否命中，直接相连cache每次只需要判断一个valid，一个dirty，一个TAG是否命中，但组相连cache则需要在组内并行的判断每路line是否命中

  ```verilog
  always @ (*) begin  // 判断输入的address 是否在 cache 中命中  
      cache_hit = 1'b0;
      for(integer i = 0; i < WAY_CNT; i++) begin
          if( valid[set_addr][i] && cache_tags[set_addr][i] == tag_addr ) begin
              cache_hit = 1'b1;
              way_addr  = i;
          end
      end
  end
  ```

**FIFO和LRU两种替换策略的实现**

- 当cache需要换出时，直接相连cache没有选择，因为每个组中只有1个line，只能换出换入这唯一的line。但组相连cache需要决策换出哪个line。为了实现FIFO策略和LRU策略，还需要加入一些辅助的wire和reg变量。

  ```verilog
  enum {FIFO, LRU} swap_out_strategy;
  integer time_cnt;//计时器，每个时钟周期加一
  reg     [      15 : 0 ] LRU_record[SET_SIZE][WAY_CNT];
  reg     [ WAY_CNT : 0 ] FIFO_record[SET_SIZE][WAY_CNT];
  ```

- FIFO策略的实现

  -选择机制:

  `FIFO_record`为每个cache行维护一个value值, 每一行的value值为一个[0,`WAY_CNT`]间的正整数，0表示该行为空。在决定换出哪一行时，如果当该set中有空行，则选择该空行进行换出。如果该set没有空行，选择value值等于WAY_CNT的。

  -value值维护方法: 

  每当有新行存入的时候，该新行的value置为1，其余非空行的value值加1。

- LRU策略的实现

  -选择机制:

  `LRU_record` 为每个cache行维护一个value值, 在决定换出哪一行时，选择value值最小的

  -value值维护方法: 

  在任何读写请求命中cache的一行时，置该行的value值为当前计时器`time_cnt`的值。在任何读写请求未命中cache时，需要换入新行，置该新行的value值为当前计时器`time_cnt`的值

- FIFO与LRU的切换

  手动修改，在cache状态机的rst部分，以下语句必须注释掉一句

  ```verilog
  swap_out_strategy <= LRU
  //swap_out_strategy <= FIFO
  ```

**2.使用testbench验证**

每当读取的数据符合预期，validation_count 这个 变量就+1，直到完成所有读出数据的验证后，validation_count 变成-1，即 0xffffffff。说明 cache 读写验证通过。

cmd进入`D:\vivado\lab3\3_CacheLab\CacheSrcCode` 目录

输入命令`python .\generate_cache_tb.py 32 > cache_tb.sv `命令获取测试文件。以下测试中使用读写规模N=32。

- FIFO
  `组相联度=3`

  <div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/1.png" style="zoom:50%"></div>

  `组相联度=5`

  <div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/2.png" style="zoom:45%"></div>

- LRU

  `组相联度=3`

  <div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/3.png" style="zoom:53%"></div>

  `组相联度=5`

  <div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/4.png" style="zoom:53%"></div>



#### 阶段二 正确运行快排&矩阵乘法

****

**1.测试快速排序**

- cmd命令行进入`D:\vivado\lab3\3_CacheLab\ASM-Benchmark/generate_inst`目录

- 使用cmd命令`python asm2verilogrom.py QuickSort.S InstructionRAM.sv ` 获取保存了指令流的指令存储器文件 InstructionRAM.sv ,使用其中的内容替换 Vivado 工程中的 InstructionRam.sv

- cmd命令行进入`D:\vivado\lab3\3_CacheLab\ASM-Benchmark/generate_data`目录

- 使用cmd命令`python .\generate_mem_for_quicksort.py 256 > mem.sv  ` 示生成256个被打乱的数，保存在数据存储器文件mem.sv中，使用其中的内容替换vivado 工程中的 mem.sv 文件

- 在 vivado 工程中开始仿真。波形运行一段时间后，会发现 mem.sv 中原本乱序的数组变有 序了（从小到大排列），说明快速排序运行成功

  <div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/5.png" style="zoom:53%"></div>

**2.测试矩阵乘法**

- cmd命令行进入`D:\vivado\lab3\3_CacheLab\ASM-Benchmark/generate_data`目录

- 使用cmd命令`python .\generate_mem_for_matmul.py 16 > mem.sv  ` 生成两个初始的方阵（源矩阵）放在数据 RAM 内，这两个 RAM 的大小为 16*16，同时 为矩阵乘法的结果（目的矩阵）准备一块内存空间。计算结果在注释中。

- 使用cmd命令`python asm2verilogrom.py MatMul.S InstructionRAM.sv   ` 得到一个保存了指令流的指令存储器文件 InstructionRam.sv。使 用其中的内容替换 Vivado 工程中的 InstructionRam.sv。然后进行仿真即可。

- 在 vivado 工程中开始仿真。仿真后查看波形图中，mem.sv 中的 ram_cell 变量，是否与注释中相同。如果相同说明运行正确。 

  <div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/6.png" style="zoom:53%"></div>

#### 阶段三 评估cache性能和占用资源

****

对不同cache策略和参数进行性s能和资源的测试评估，测试说明如下:

- 变量参数设定

​	问题类型(快排/矩阵乘法)，替换策略(FIFO/LRU)，组相联度(WAY_CNT=1,2,3,4,5,6,7)，

​	组数(SET_ADDR_LEN=2,3,4,5)，line 大小(LINE_ADDR_LEN=2,3,4,5)

- 不变量&因变量设定

  固定测试数据规模(256个数的快排 / 16*16的矩阵乘法)，

  固定主存大小(LINE_ADDR_LEN+SET_ADDR_LEN+TAG_ADDR_LEN=13 )

- 评测标准

  -资源占用: LUT(Estimation和Utilization参数，简称为E1和U1)，FF(Estimation和Utilization参数，简称为E2和U2)

  -性能: 使用运行 benchmark 时的时钟周期性进行评估，转化为使用命中率R度量 ，R=hit/(hit+miss)

**1.快速排序**

- 如下表格给出25组测试结果，截图附在实验报告最后

- 使用控制变量法确定一个较优的 cache 参数和策略, 先确定line大小，再确定组数，最后确定组相联度

- **对于LRU策略 (组别1~13)**

  综合性能和消耗，选取一个折中的方案：组别1~4中组别2最优,故确定line大小为3。组别5~8中组别6最优,故确定组数为3。组别9~13中组别12最优,故确定组相联度为6。

  故对于LRU策略，一个较优的cache参数设定为(line大小=3，组数=3，相联度=6), 此时命中率为98.08%，资源占用为LUT(E1)=2467, FF(E2)=5464

- **对于FIFO策略 (组别14~25)**

  -综合性能和消耗，选取一个折中的方案：组别14~17中组别15最优,故确定line大小为3。组别18~21中组别20最优,故确定组数为4。组别22~25中组别24最优,故确定组相联度为3。

  -故对于FIFO策略，一个较优的cache参数设定为(line大小=3，组数=4，相联度=3), 此时命中率为99.25%，资源占用为LUT(E1)=1859, FF(E2)=5169

| 组别   | 替换策略 | 组相联度W | 组数S | line大小 | 资源占用LUT(E1,U1) | 资源占用FF(E2,U2) | 命中率R(%) |
| ------ | -------- | --------- | ----- | -------- | ------------------ | ----------------- | ---------- |
| 1      | LRU      | 3         | 3     | 2        | (1939,0.64)        | ( 3058, 0.50)     | 93.75      |
| **2**  | LRU      | 3         | 3     | 3        | (2491,0.82)        | (5462 ,0.90 )     | 98.08      |
| 3      | LRU      | 3         | 3     | 4        | (4404,1.45)        | (10312 , 1.70)    | 99.56      |
| 4      | LRU      | 3         | 3     | 5        | (8866,2.92)        | (20031 , 3.30)    | 99.83      |
| 5      | LRU      | 3         | 2     | 3        | (1939,0.64)        | (3058 ,0.50 )     | 96.51      |
| **6**  | LRU      | 3         | 3     | 3        | ( 2491,0.82 )      | (5462 ,0.90 )     | 98.08      |
| 7      | LRU      | 3         | 4     | 3        | (4811 ,1.58 )      | ( 9934,1.64 )     | 99.10      |
| 8      | LRU      | 3         | 5     | 3        | (8324 ,2.74 )      | ( 18833,3.1 )     | 99.25      |
| 9      | LRU      | 3         | 3     | 3        | ( 1861,0.61 )      | (5169 ,0.85 )     | 96.97      |
| 10     | LRU      | 4         | 3     | 3        | (2523 ,0.83 )      | ( 5461, 0.90)     | 98.08      |
| 11     | LRU      | 5         | 3     | 3        | (2479 ,0.82 )      | (5461 ,0.90 )     | 98.08      |
| **12** | LRU      | 6         | 3     | 3        | ( 2467, 0.81)      | (5464 ,0.90 )     | 98.08      |
| 13     | LRU      | 7         | 3     | 3        | (2521 ,0.83)       | ( 5465, 0.90)     | 98.08      |
| 14     | FIFO     | 3         | 3     | 2        | (714 ,0.24 )       | (1668 , 0.27)     | 95.17      |
| **15** | FIFO     | 3         | 3     | 3        | (1351 ,0.44 )      | ( 3065,0.50 )     | 98.54      |
| 16     | FIFO     | 3         | 3     | 4        | (2328 ,0.77 )      | ( 5870,0.97 )     | 99.64      |
| 17     | FIFO     | 3         | 3     | 5        | ( 4194, 1.38)      | ( 11492, 1.89)    | 99.83      |
| 18     | FIFO     | 3         | 2     | 3        | (1141 ,0.38)       | ( 2009,0.33 )     | 97.39      |
| 19     | FIFO     | 3         | 3     | 3        | (1351 ,0.44 )      | (3065 ,0.50 )     | 98.54      |
| **20** | FIFO     | 3         | 4     | 3        | (1859 ,0.61 )      | (5169 ,0.85 )     | 99.25      |
| 21     | FIFO     | 3         | 5     | 3        | (3459 ,1.14 )      | (9361 ,1.54 )     | 99.25      |
| 22     | FIFO     | 1         | 4     | 3        | (1861 ,0.61 )      | (5169 ,0.85 )     | 96.97      |
| 23     | FIFO     | 2         | 4     | 3        | (1860,0.61 )       | ( 5169,0.85 )     | 99.06      |
| **24** | FIFO     | 3         | 4     | 3        | (1859 ,0.61 )      | (5169 ,0.85 )     | 99.25      |
| 25     | FIFO     | 4         | 4     | 3        | (1783 ,0.59 )      | (5169 ,0.85 )     | 99.25      |

**2.矩阵乘法**

- 如下表格给出25组测试结果，截图附在实验报告最后

- 使用控制变量法确定一个较优的 cache 参数和策略, 先确定line大小，再确定组数，最后确定组相联度

- **对于LRU策略 (组别1~13)**

  综合性能和消耗，选取一个折中的方案：组别1~4中组别3最优,故确定line大小为4。组别5~7中组别6最优,故确定组数为3。组别8~13中组别9最优,故确定组相联度为3。

  故对于LRU策略，一个较优的cache参数设定为(line大小=4，组数=3，相联度=3), 此时命中率为86.16%，资源占用为LUT(E1)=4404, FF(E2)=10312

- **对于FIFO策略 (组别14~25)**

  综合性能和消耗，选取一个折中的方案：组别14~17中组别16最优,故确定line大小为4。组别18~20中组别19最优,故确定组数为3。组别21~25中组别24最优,故确定组相联度为5。

  对于FIFO策略，一个较优的cache参数设定为(line大小=4，组数=3，相联度=5), 此时命中率为99.27%，资源占用为LUT(E1)=2065, FF(E2)=5870

| 组别   | 替换策略 | 组相联度W | 组数S | line大小 | 资源占用LUT(E1,U1) | 资源占用FF(E2,U2) | 命中率R(%) |
| ------ | -------- | --------- | ----- | -------- | ------------------ | ----------------- | ---------- |
| 1      | LRU      | 3         | 3     | 2        | (1939,0.64)        | (3058,0.50)       | 44.11      |
| 2      | LRU      | 3         | 3     | 3        | (2491,0.82)        | (5462,0.82)       | 44.99      |
| **3**  | LRU      | 3         | 3     | 4        | (4404,1.45)        | (10312,1.70)      | 86.16      |
| 4      | LRU      | 3         | 3     | 5        | (8866,2.92)        | (20031,3.30)      | 87.90      |
| 5      | LRU      | 3         | 2     | 4        | (3741,1.23)        | (6032,0.99)       | 46.99      |
| **6**  | LRU      | 3         | 3     | 4        | (4404,1.45)        | (10312,1.70)      | 86.16      |
| 7      | LRU      | 3         | 4     | 4        | (8237,2.71)        | (18867,3.11)      | 93.57      |
| 8      | LRU      | 2         | 3     | 4        | (4562,1.50)        | (10320,1.70)      | 86.08      |
| **9**  | LRU      | 3         | 3     | 4        | (4404,1.45)        | (10312,1.70)      | 86.16      |
| 10     | LRU      | 4         | 3     | 4        | (4468,1.47)        | (10307,1.70)      | 86.16      |
| 11     | LRU      | 5         | 3     | 4        | (4818,1.59)        | (10327,1.70)      | 86.16      |
| 12     | LRU      | 6         | 3     | 4        | (4299,1.42)        | (10315,1.70)      | 86.16      |
| 13     | LRU      | 7         | 3     | 4        | (6880,2.27)        | (16719,2.75)      | 86.16      |
| 14     | FIFO     | 3         | 3     | 2        | (714,0.24)         | (1668,0.27)       | 44.32      |
| 15     | FIFO     | 3         | 3     | 3        | (1351,0.44)        | (3065,0.50)       | 45.08      |
| **16** | FIFO     | 3         | 3     | 4        | (2328,0.77)        | (5870,0.97)       | 88.47      |
| 17     | FIFO     | 3         | 3     | 5        | (4194,1.38)        | (11492,1.89)      | 99.72      |
| 18     | FIFO     | 3         | 2     | 4        | (2210,0.73)        | (3794,0.62)       | 43.51      |
| **19** | FIFO     | 3         | 3     | 4        | (2328,0.77)        | (5870,0.97)       | 88.47      |
| 20     | FIFO     | 3         | 4     | 4        | (3489,1.15)        | (10014,1.65)      | 99.43      |
| 21     | FIFO     | 2         | 3     | 4        | (2329,0.77)        | (5870,0.97)       | 86.03      |
| 22     | FIFO     | 3         | 3     | 4        | (2328,0.77)        | (5870,0.97)       | 88.47      |
| 23     | FIFO     | 4         | 3     | 4        | (2339,0.77)        | (5870,0.97)       | 99.24      |
| **24** | FIFO     | 5         | 3     | 4        | (2065,0.68)        | (5870,0.97)       | 99.27      |
| 25     | FIFO     | 6         | 3     | 4        | (2074,0.68)        | (5870,0.97)       | 99.43      |

### 五.实验总结

****

- 因为全面评测需评测组数过多,每组统计耗时5~10分钟不等。特别是当cache占用资源较多时，综合用时较长。故以上评测采用固定变量法, 仅作50组评测，得到局部最优解
- cache size增大会带来命中率提升，但是会加倍消耗存储资源电路面积的开销。随着cache size的增大，命中率的增长会逐渐慢下来
- 组相联度的增大，cache size也会增大，会增加资源消耗，但不是加倍，同时命中率也会增大
- 一般来说, 在相同配置情况下,LRU的性能比FIFO好
- cache的具体参数配置, 还依赖于程序具有不同的资源需求
- 写回法可以保证主存和cache的一致性，但会增大时间开销， 因为每次写cache都要写主存





------

【附录】资源报告截图, 因图片过多, 仅列出2组波形和评测报告作为示例

**1.快速排序组别1**

- 资源报告

<div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/7.png" style="zoom:53%"></div>

- 缺失率统计

 <div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/8.png" style="zoom:50%"></div>

**2.矩阵乘法组别1**

- 资源报告

<div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/9.png" style="zoom:53%"></div>

- 缺失率统计

<div align=center><img src="https://github.com/jocelynzym/Junior/blob/master/CA/lab3/pic/10.png" style="zoom:60%"></div>




