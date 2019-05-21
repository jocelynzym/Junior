## lab3  cache设计和实现



### 一.实验目标

- 权衡cache size增大带来的命中率提升收益和存储资源电路面积的开销
- 权衡选择合适的组相连度（相连度增大cache size也会增大，但是冲突miss会减低)
- 体会使用复杂电路实现复杂替换策略带来的收益和简单替换策略的优势（有时候简单策略比复杂策略效果不差很多甚至可能更好）
- 理解写回法的优劣

### 二.实验环境和工具

- vivado自带的波形仿真工具
- notapad++ 

### 三.实验内容

#### 阶段一

****

**1.实验要求**

- 将直接映射策略的cache修改为N路组相连的cache

  -宏定义设置组相连度

  -写回 & 带写分配

  -FIFO和LRU两种替换策略

- 进行cache读写测试 

  -使用python脚本生成一个新的testbench，并对自己的cache进行验证

  -要求FIFO和LRU策略都要验证

  -修改组相连度等参数进行多次验证

**2.已提供**

- `cache.sv` 一个简单的cache。直接映射,写回带写分配
- `generate_cache_tb.py` 一个python脚本。用于生成不同规模的testbench文件。这些testbench文件对cache进行多次随机读写，最后从cache中读取数据并验证cache的正确性。
- `cache_tb.sv` 一个已经生成的testbench

**3.代码分析**

- `cache.sv`

  ```verilog
  
  ```

- `generate_cache_tb.py`

  ```python
  
  ```

- `cache_tb.v`


**4.修改部分**





#### 阶段二

****

1.目标

- 使用阶段一编写的N路组相联cache，正确运行测试程序	





#### 阶段三

****

1.目标

- 对不同cache策略和参数进行性能和资源的测试评估
- 编写实验报告



### 五.实验总结

------

- 



#### 六.BUG & DEBUG

------







------

【参考资料】

- [cache浅析](https://www.cnblogs.com/mikewolf2002/p/6197224.html)
- 







