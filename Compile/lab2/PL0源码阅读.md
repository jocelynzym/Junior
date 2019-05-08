# PL/0编译器源码阅读

[TOC]

## 1.源码结构

源码在四个文件中set.h/set.c/pl0.h/pl0.c，其中pl0.c为核心代码，其余三个做一些辅助性工作

#### set.h/set.c

​	一些链表操作的声明，函数具体实现在set.c中

```c
typedef struct snode
{
	int elem;
	struct snode* next;
} snode, *symset;
symset phi, declbegsys, statbegsys, facbegsys, relset;

symset createset(int data, .../* SYM_NULL */);
/*创建链表s，参数可变*/
void destroyset(symset s);
/*删除链表s*/
symset uniteset(symset s1, symset s2);
/*把两个有序链表合并成一个有序链表，第一个结点不存放数据*/
int inset(int elem, symset s);
/*查找链表s中是否有元素elem*/

```

#### pl0.h

pl0.c的头文件，包括一些全局变量和宏

```c
//宏
#define NRW        12     // number of reserved words
#define TXMAX      500    // length of identifier table
#define MAXNUMLEN  14     // maximum number of digits in numbers
#define NSYM       11     // maximum number of symbols in array ssym and csym
#define MAXIDLEN   10     // length of identifiers
#define MAXADDRESS 32767  // maximum address
#define MAXLEVEL   32     // maximum depth of nesting block
#define CXMAX      500    // size of code array
#define MAXSYM     30     // maximum number of symbols  
#define STACKSIZE  1000   // maximum storage
```

```c
//全局变量
char ch;         // last character read
int  sym;        // last symbol read
char id[MAXIDLEN + 1]; // last identifier read
int  num;        // last number read
int  cc;         // character count
int  ll;         // line length
int  kk;
int  err;        //total error
int  cx;         // index of current instruction to be generated.
int  level = 0;  
int  tx = 0;

char line[80];
instruction code[CXMAX];
```

#### pl0.c

编译器的核心部分，包括词法分析，语法分析，代码生成，错误检测

在函数分析中详细分析pl0.c文件

## 2.函数分析

#### (1)statement()

statement() 函数是该编译器的核心，它在 block() 中被调用，用来具体实现代码的翻译，我们许多的扩展都是在这里修改。

statement() 每调用一次只能执行一条语句的翻译，但这一条概念不是指一行，而是指一个结构，比如 begin…end 、for 、while 等。虽然这些结构有好多代码，但每个结构只需一个 statement() 便可以解决。所以也就不奇怪为什么 block() 中只调用了一次 statement() 了。因为每个块都是由 begin….end 结构组成的，在 statement() 看来，这是一条语句。至于 begin….end 中使用了很多结构， 比如 for、if、赋值等语句每一个都需要调用 statement()。

#### (2)block()

block() 用来把一程序块翻译为中间语言代码，块的范围是指从声明到 begin….end 结束。也就是说 main()只需调用一次 block() 即可完成所有代码的翻译。那么 block() 函数的意思在哪里呢？ 比如你声明一个 procedure, procedure 中的格式和 main 是一样的，也是先声明然后是 begin 语句，所以在 procedure 中也只需调用一次 block() 函数。

#### (3) void error(int n)

打印出错信息，参数为错误编号，根据编号打印错误信息

```c
void error(int n) //参数n代表错误编号
{
	int i;
	printf("      ");
	for (i = 1; i <= cc - 1; i++)
		printf(" ");
	printf("^\n");
	printf("Error %3d: %s\n", n, err_msg[n]);
	err++;       //记录错误总数
} // error
```

#### (4) void test(symset s1, symset s2, int n)

测试是否出错, 当sym不在链表s1中时报错，s2是一个备用补救集合，当sym不在s1中时，合并s1,s2为s，继续取symbol，直到symbol属于集合s为止

```c
void test(symset s1, symset s2, int n)
{
	symset s;
	if (! inset(sym, s1))
	{
		error(n);
		s = uniteset(s1, s2);
		while(! inset(sym, s))
			getsym();
		destroyset(s);
	}
} // test
```

#### (5)getch() 与 getsym()

读入函数。 
　　**getch()** 是以字符为单位读入的，遇到空格跳过。每次执行完后读入一个字符到变量 **ch**中。**getsym()** 是以符号位单位读入的，里面有调用 **getch()** 来完成这一操作，每次执行后读入一个符号到变量 **sym** 中。比如在编译

#### (6) expression()

处理表达式的值，包括简单表达式，大于等于，小于等于...等表达式。 

#### (7)factor() 和 term()

这两个函数同样用于计算表达式，计算的表达式也更为简单。 嵌套关系为： **simpleexpression()** 调用 **term()**，**term()** 调用 **factor()**。**factor()** 用于计算因子，而 **term()** 用于计算项。 

`++a * 2 + 5`  

++a 由 factor() 计算，* 2 由 term() 计算，+ 5 由 simpleexpression() 计算。因为加减是项与项之间的运算，而乘除是项内的运算，类似于 ++a 则是因子的计算。整个函数嵌套严密，分工明确，我们计算时只用调用 simpleexpression() 就可以了。如果要扩展 ++ 这样的功能，则要去修改 factor() 。

#### (9)position()

**position()** 函数顾名思义就是寻找地址，也是众多函数中最简单的一个了。它用来返回声明的一个变量在 **nametab[]** 中的下表。如果返回值为 0 表示未找到，说明该变量未声明。

####(10)assignment()

**assignment()** 用来完成赋值操作，既可以是整型，也可以是字符型。用法类似于 **expression()**，比如赋值语句`a :=1 `中：只需先 **getsym()** 读到 ‘**a**‘, 然后执行 **assignment()** 即可完成赋值。跟 **expression()** 一样也会多读一个 **sym**。

#### 





## 3.指令集

一条指令由指令码f，层差l和操作数a构成。格式为f--l--a

指令码f及对应的功能如下：



#### (1)LIT：将常数置于栈顶

将操作数a加载到栈顶，并使栈顶加1；

####(2)OPR：算逻运算指令

 根据操作数a进行相关运算，运算结果保存在栈顶，操作数a及相应的功能如下： 
​	OPR_NEG：	栈顶的数取反；

​	OPR_ADD：     次栈顶与栈顶的数相加，并使栈顶减1；

​	OPR_MIN：      次栈顶与栈顶的数相减，并使栈顶减1；

​	OPR_MUL：     次栈顶与栈顶的数相乘，并使栈顶减1；

​	OPR_DIV：       次栈顶与栈顶的数相除，并使栈顶减1；

​	OPR_MOD：    次栈顶与栈顶的数取余，并使栈顶减1；

​	OPR_SHL：       次栈顶左移保存在栈顶的位数，并使栈顶减1；

​	OPR_SHR：      次栈顶右移保存在栈顶的位数，并使栈顶减1；

​	OPR_BIT_NOT：    栈顶按位非运算；

​	OPR_BIT_AND：    次栈顶与栈顶按位与运算，并使栈顶减1；

​	OPR_BIT_XOR：     次栈顶与栈顶按位异或运算，并使栈顶减1；

​	OPR_BIT_OR：              次栈顶与栈顶按位或运算，并使栈顶减1；

​	OPR_ODD：     （弃用）栈顶为奇数将栈顶置1；

​	OPR_EQU：      （弃用）次栈顶等于栈顶将栈顶置1，并使栈顶减1；

#### (3)LOD：将变量置于栈顶

根据层差l与偏移地址a寻址变量，将变量的值加载到栈顶，并使栈顶加1；

#### (4)STO：将栈顶的值赋与某变量

根据层差l与偏移地址a寻址变量，将栈顶的值保存到变量对应的地址处，栈顶不变；

#### (5)CAL：用于过程调用的指令

调用过程，建立函数栈帧，根据层差l建立函数的访问链、调用链，保存函数返回地址，并使pc指向函数的入口绝对地址a；

#### (6)INT：在数据栈中分配存贮空间

将栈顶增加（或减少）a表示的大小；

#### (7)JMP：无条件跳转

无条件跳转，将pc指向相对当前指令偏移为a的指令地址；

#### (8)JPC：有条件跳转

如果栈顶为0则跳转，将pc指向相对当前指令偏移为a的指令地址，并使栈顶减1；











