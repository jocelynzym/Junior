## lab4 分支预测



### 一.实验设计

- **阶段一**: 实现btb

  -使用直接映射策略，每个PC地址对应一个固定块

  -在取指阶段利用PC寻址btb，如果命中，则说明这是一条跳转指令，利用从BTB中获取到的地址去取icache

  -在EX阶段判断预测是否正确，并相应地修改状态机状态

- **阶段二**: 实现bht

  -对于BHT在BTB的基础上实现一个状态转移图，实现2bits的转移

### 二.实验环境和工具

- vivado自带的波形仿真工具
- notapad++ 

### 三.实验结果

#### 1.真值表

| BTB  | BHT  | REAL | NPC_PRED | flush | NPC_REAL      | BTB update |
| ---- | ---- | ---- | -------- | ----- | ------------- | ---------- |
| Y    | Y    | Y    | BUF      | N     | BUF           | N          |
| Y    | Y    | N    | BUF      | Y     | PC_EX+4       | Y          |
| Y    | N    | Y    | PC_IF+4  | Y     | Branch_Targrt | Y          |
| Y    | N    | N    | PC_IF+4  | N     | PC_IF+4       | N          |
| N    | Y    | Y    | PC_IF+4  | Y     | Branch_Targrt | Y          |
| N    | Y    | N    | PC_IF+4  | N     | PC_IF+4       | N          |
| N    | N    | Y    | PC_IF+4  | Y     | Branch_Targrt | Y          |
| N    | N    | N    | PC_IF+4  | N     | PC_IF+4       | N          |

具体设计时引用了一个Op_Branch信号, 判断EX段是否为Branch指令，所以在具体实现btb/bth时可能与上表稍有出入

#### 2.测试文件btb.S执行结果

- 寄存器内容

  对于3种方法(无预测/btb/bht)，在正确执行测试文件后，寄存器内容相同

  ![](G:\github\Junior\CA\lab4\pic\1.png)

- 分支预测失败情况

  (1)无预测

  ![](G:\github\Junior\CA\lab4\pic\2.png)

  (2)btb

  ![](G:\github\Junior\CA\lab4\pic\3.png)

  (3)bht

  ![](G:\github\Junior\CA\lab4\pic\4.png)

#### 3.测试文件bht.S执行结果

- **寄存器内容**

  对于3种方法(无预测/btb/bht)，在正确执行测试文件后，寄存器内容相同

  ![](G:\github\Junior\CA\lab4\pic\5.png)

- **分支预测失败情况**

  (1)无预测

  ![](G:\github\Junior\CA\lab4\pic\6.png)

  (2)btb

  ![](G:\github\Junior\CA\lab4\pic\7.png)

  (3)bht

  ![](G:\github\Junior\CA\lab4\pic\8.png)



### 四.实验报告

****

**1.分支收益和分支代价**

- 预测错误代价:  2 clocks
- 预测正确收益: 无flush和stall

**2.统计未使用分支预测和使用分支预测的总周期数**

|        | 测试文件btb.S | 测试文件bht.S |
| ------ | ------------- | ------------- |
| 无预测 | 510           | 536           |
| btb.v  | 314           | 382           |
| bht.v  | 316           | 368           |

**3.统计分支指令数目、动态分支预测正确次数和错误次数**

btb.S总分支为101, 而bht.S的总分支数为110, 下表为正确/错误次数

|        | 测试文件btb.S(true/false) | 测试文件bht.S(true/false) |
| ------ | ------------------------- | ------------------------- |
| 无预测 | 1/100                     | 11/99                     |
| btb.v  | 99/2                      | 79/22                     |
| bht.v  | 98/3                      | 95/15                     |

**4.对比不同策略，分析以上几点关系**

- 静态预测假设每次都not take，预测失败概率很大
- 动态预测在循环结构中比静态预测占优，当循环次数越大占优越明显
- 在二重循环中,bht更有优势,因为bht在存在循环某次not taken但是下一次taken情形下，预测成功的概率更大

**5.计算整体CPI和加速比**

btb.S总指令数为307, bht.S总指令数为335

|        | btb.S(CPI/加速比) | bht.S(CPI/加速比) |
| ------ | ----------------- | ----------------- |
| 无预测 | 1.66/1.00         | 1.60/1.00         |
| btb.v  | 1.02/1.63         | 1.14/1.40         |
| bht.v  | 1.03/1.61         | 1.10/1.45         |







