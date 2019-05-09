### lab 2  RV32I五级流水线CPU

****

#### 一.实验目标

- verilog实现RV32I流水线CPU
- 手写测试用汇编代码，生成.inst和.data文件，观察寄存状态，验证指令的正确性
- 熟悉vivado开发环境、熟悉vivado波形仿真

#### 二.实验环境和工具

- vivado
- notapad++ 

#### 三.实验过程

1.阶段一

- 总计9个待补充模块，根据设计报告补全verilog代码
- 第一阶段即实现全部指令和hazard模块但不作全面测试，只保证其部分正确性
- 熟悉vivado环境：design代码导入和sim代码导入, 仿真时间设置，仿真波形的添加
- 手写测试用汇编文件：编写`阶段一要求指令`的RV32I汇编文件，学习Makefile的用法，生成.data和.inst文件
- DEBUG

2.阶段二

- 手写测试用汇编文件：编写`阶段二要求指令+阶段一要求指令`的RV32I汇编文件，生成.data和.inst文件	
- DEBUG，保证阶段二要求指令的正确性和hazard模块剩余部分的正确性

3.阶段三

- 使用给定测试例，作最后检查


#### 四.实验内容

> 实验内容即verilog代码编写+根据汇编DEBUG，以下给出在lab1设计报告中未提的及代码编写中需注意的细节

**1.计算模块ALU.v**

- ALU接受两个32位操作数，根据AluContrl[3:0]的不同，进行不同的计算操作，将计算结果输出到AluOut[31:0]
- ALU计算功能有: SLL,SRL,SRA,ADD,SUB,XOR,OR,AND,SLT,SLTU,LUI
- SLL & SRL: r1左移(右移)r2的低5位，低位(高位)补0
- SRA:算数右移前将r1转换为有符号数，不然仍可能高位补0
- LUI: 立即数的低位补0在imm模块中实现，直接将Operand2赋值给AluOut即可

**2.立即数扩展模块ImmOperandUnit.v**

- 利用正在被译码的指令的部分编码值，生成不同类型的32bit立即数

- 根据6种不同的指令类型构造32位立即数

- 37条指令分类

  -RTYPE【算逻运算指令】: 

  - SLL, SRL, SRA,  ADD, SUB, XOR, OR, AND, SLT, SLTU
  - 无需使用imm

  -ITYPE【立即数指令】: 

  - LB, LH, LW, LBU, LHU, SLLI, SRLI, SRAI, ADDI, XORI, ORI, ANDI, SLTI, SLTUI 
  - imm[11:0]指令中给出，剩余补0

  -STYPE【存储指令】:

  - SB, SH, SW
  - imm[11:0]指令中给出，剩余补0

  -BTYPE【分支指令】: 

  - BEQ, BNE, BLT, BGE, BLTU, BGEU
  - imm[12:1]指令中给出，剩余补0

  -UTYPE【寄存器立即数】: 

  - LUI, AUIPC
  - imm[31:12]指令中给出，剩余补0

  -JTYPE【跳转指令】: 

  - JAL,  JALR
  - imm[20:1]指令中给出，剩余补0

**3.分支判定模块BranchDecisionMaking.v**

- BranchDecisionMaking接受两个操作数，根据7种BranchTypeE的不同，进行不同的判断，当分支应该taken时，令BranchE=1'b1

**4.load指令结果处理模块DataExt.v**

- DataExt是用来处理非字对齐load的情形，同时根据load的不同模式对Data Mem中load的数进行符号或者无符号拓展，组合逻辑电路

**5.译码模块ControlUnit.v**

- 输入   

  ​    // Op               是指令的操作码部分
  ​    // Fn3              是指令的func3部分
  ​    // Fn7              是指令的func7部分

- Op+Fn3+Fn7可以唯一确定一条指令，根据该指令的数据通路来决定输出

- 各指令Op码,Fn3和Fn7

  | OP      | 指令                                         | FUNCT3                              | FUNCT7                                   |
  | ------- | -------------------------------------------- | ----------------------------------- | ---------------------------------------- |
  | 0000011 | lb,lh,lw,lbu,lhu                             | 000,001,010,100,101                 | ---                                      |
  | 0100011 | sb,sh,sw                                     | 000,001,010                         | ---                                      |
  | 0110011 | add,sub,xor,or,and,sll,srl,sra,slt,sltu      |                                     |                                          |
  | 0010011 | addi,xori,ori,andi,slli,srli,srai,slti,sltiu | 000,100,110,111,001,101,101,010,011 | --,--,--,--,--，0000000，0100000，--，-- |
  | 0110111 | lui                                          | ---                                 | ---                                      |
  | 0010111 | auipc                                        | ---                                 | ---                                      |
  | 1100011 | beq,bne,blt,bge,bltu,bgeu                    | 000,001,100,101,110,111             | ---                                      |
  | 1101111 | jal                                          | ---                                 | ---                                      |
  | 1100111 | jalr                                         | ---                                 | ---                                      |

**6.获取下个PC模块NPC_Generator.v**

- 用来生成Next PC值，根据3种不同的跳转信号选择不同的新PC值
- 流水线深的先执行：Jalr_E=Branch_E>JalD

**7.指令存储器模块IDSegReg.v**

- 同步读mem，需要时钟上升沿
- ID段寄存器中同步调用mem，直接将输出传递到ID段组合逻辑

**8.数据存储器模块WBSegReg.v**

- WE为4位独热码，根据输入地址的后两位，选择具体的Byte

**9.冒险与停顿模块HarzardUnit.v**

- HarzardUnit用来处理流水线冲突，通过插入气泡，forward以及冲刷流水段解决数据相关和控制相关，组合逻辑电路

- Forward机制原理

  ​	1.情况1：一级读写转发

  ​		-将sub产生的结果直接转发给and指令

  ​		-条件判断：MEM为写操作，MEM写回寄存器=EX源寄存器

  ​		-转发内容：ALUResult_mem

<img src='E:\同步文件夹\大三下\计算机体系结构\lab\pic\1.jpg' style="zoom 80%">

​		2.情况2：二级读写转发

​			-将sub产生的结果直接转发给or指令

​			-条件判断：WB级为写操作，WB写回寄存器=EX级源寄存器，无一级读写转发

​			-转发内容：RegWriteData_wb 

<img src='E:\同步文件夹\大三下\计算机体系结构\lab\pic\2.jpg' style="zoom 50%">

- Forwarding机制实现

  | 地址         | 操作数来源   |
  | ------------ | ------------ |
  | Forward1E=00 | RegOut1E     |
  | Forward1E=01 | RegWriteData |
  | Forward1E=10 | AluOutM      |
  | Forward2E=00 | RegOut2E     |
  | Forward2E=01 | RegWriteData |
  | Forward2E=10 | AluOutM      |

- Hazard机制原理(stall&flush)

  ​	1.数据冒险：lw+add,且访问同一寄存器

  ​		-在lw与add间插入阻塞，将数据冒险转化为Forward情况2

  ​		-将IF,ID寄存器stall

  ​	2.控制冒险：

  ​		-Branch条件成立||JALR指令,将ID,EX寄存器flush

  ​		-JAL指令,将ID寄存器flush


#### 五.实验总结

****

- 助教已经做了大量工作: 搭建CPU架构，将模块组织成流水线，给出制作测试例的脚本
- 在组成原理MIPS五级流水线CPU的基础上，稍加改动就可以完成RV32I流水线CPU
- 以前没用过vivado，要熟悉一下环境
- 根据波形debug，从一处异常的波形入手追根溯源。可能波形看起来错误很多其实只是一句代码的问题
- bug大多出现在UnitControl.v模块, 该模块指令众多信号众多,错误多也很正常



#### 六.BUG & DEBUG

****

**1.Syntax error near "endcase"**

- endcase的前一条语句未加分号";"

**2.Syntax error near "wire"**

- `wire JAL;`

  本语句的前一条语句未加分号";"

**3.undeclared symbol LOAD,assumed default net type wire**

- `assign LOAD=LB||LH||LW||LBU||LHU;` 

  LOAD未用wire语句定义

**4.Regfile模块无时钟信号输入**

- 意外改动了无需改动的模块

**5.Regfile模块WE3始终为0**

- 逐级排查，从RegwriteW一路排查到RegwriteD

- 发现指令输入正确但RegwriteD输出错误，将错误锁定在UnitControl模块

- 最终错误为RegwriteD的赋值语句写的不全面

**6.2testAll.data跑至374发生错误**

- WBSegReg模块的输入数据对齐问题，除对使能的左移操作，还要相应对输入数据做操作





****

【参考资料】

- [建立工程/源文件/仿真文件](https://blog.csdn.net/yiyang14/article/details/83550413)







