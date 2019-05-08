## lab2  扩充PL/0编译程序



### 实验要求

****

对PL0编译器进行功能扩展，增加如下三个功能

> 1.实现 else子句。(张抑淼)
>
> 2.实现 break 语句与continue 语句。(张抑淼，胡天竺)
>
> 3.实现形如 “i := j := k := 100; ”的连续赋值语句。 (张莎)



### 环境配置

****

**编译环境安装**

```
//安装编译环境
sudo apt-get install build-essential 
```

**将所有程序拷贝到同一个目录下,gcc编译**

​	PL/0语言编译器源程序包括如下C程序文件，PL0.h，PL0.c ，set.h，set.c 

```c
编译：gcc -o plcc pl0.c //得到PL0编译器
运行：./plcc            //运行PL0编译器
```

**将测试用例转换为unix下文本格式**

```c
//安装dos2unix
sudo apt-get install dos2unix 
//格式转换
dos2unix example1.txt
//检查是否转换成功
cat -v example1.txt
```





### 实验内容

****

### Part1 实现else子句 

#### 1.原方案：原PL0程序if-then语句的实现分析

> 原有pl0语言只支持 if-then，即是只支持if语句，先来分析原PL0编译器是如何实现if跳转的

```c
void statement(symset fsys)
{
    //...
else if (sym == SYM_IF)
/*准备按照if语句处理*/
	{ 
		getsym();
		set1 = createset(SYM_THEN, SYM_DO, SYM_NULL);
		set = uniteset(set1, fsys);
    	/*检查下一个符号是不是then或do*/
		condition(set);
    	/*处理if语句后面的内容，一个逻辑表达式的处理，执行完这条语句，栈顶会存放计算的结果值，后面的jpc指令根据栈顶值决定要不要跳转*/
		destroyset(set1);
		destroyset(set);
		if (sym == SYM_THEN)
		{ /*后跟符号为then*/
			getsym();
		}
		else
		{/*缺少then*/
			error(16); // 'then' expected.
		}
		cx1 = cx;
    	/*保存当前指令地址*/
		gen(JPC, 0, 0);
    	/* 生成条件跳转指令，跳转地址未知，暂时写0 */
		statement(fsys);
    	/* 处理then后的语句 */
		code[cx1].a = cx;	
        /* cx为then后语句执行完的位置，它正是前面未定的跳转地址 */
	}
	//...
}
```

#### 2.修改方案：实现if-then-else语句

**实现思路**

>原 if-then语句：当if为真，整个程序顺序执行。当if为假，跳过then中语句。
>
>现if-then-else语句：当if为真，跳过else部分。当if为假，跳过then中语句。
>
>修改内容：在if的then语句执行完毕后，增加一个直接跳转jmp，因为如果执行了then，则一定不会执行else
>
>特别注意：递归调用**void statement(symset fsys)**处理then后的语句时，需要把SYM_ELSE加入statement的参数集fsys中

```c
void statement(symset fsys)
{
    //...
else if (sym == SYM_IF)
/*准备按照if语句处理*/
	{ 
		getsym();
		set1 = createset(SYM_THEN, SYM_DO,SYM_NULL);
		set = uniteset(set1, fsys);
    	/*检查下一个符号是不是then或do或else*/
		condition(set);
    	/*处理if语句后面的内容，一个逻辑表达式的处理，执行完这条语句，栈顶会存放计算的结果值，后面的jpc指令根据栈顶值决定要不要跳转*/
		destroyset(set1);
		destroyset(set);
		if (sym == SYM_THEN)
		{ 
			getsym();
		}
		else
		{
			error(16); // 'then' expected.
		}
		cx1 = cx;
		gen(JPC, 0, 0);
    	set1 = createset(SYM_ELSE,SYM_NULL); //*重要
		set = uniteset(set1, fsys);          //*重要
		statement(set);
    	destroyset(set1);			//*重要
		destroyset(set);			//*重要
    	/* 处理then后的语句 */
    	if(sym == SYM_ELSE)
		{
			getsym();
			cx2 = cx;
			gen(JMP,0,0);
			code[cx1].a = cx;
			statement(fsys);
			code[cx2].a=cx;
		}
		else
		{
			code[cx1].a = cx;
		}
	}
	//...
}
```

#### 3.其他实现细节(ELSE的词法分析)

> 除了对`void statement(symset fsys)`中if语句结构的修改，还需要增加else关键字，相关实现细节如下

**修改关键字个数**

```c
#define NRW        12     // 关键字个数11->12
```

**在枚举集合中添加else标识**

```c
enum symtype
{
	//...
	SYM_ELSE            //增添SYM_ELSE
};
```

**修改保留字个数**

```c
#define NSYM       11  //保留字个数10->11 
```

**设置保留字名字**

因为编译器中查找关键字使用折半查找，限制条件是查找数据必须有序，这里按照首字母字母表顺序

```c
//在"do"和"end"之间增加"else"
char* word[NRW + 1] =
{
	"", /* place holder */
	"begin", "call", "const", "do", "else","end","if",
	"odd", "procedure", "then", "var", "while"
};
```

**设置保留字符号**

```c
//在SYM_DO和SYM_END之间增加SYM_ELSE
int wsym[NRW + 1] =
{
	SYM_NULL, SYM_BEGIN, SYM_CALL, SYM_CONST, SYM_DO, SYM_ELSE,SYM_END,
	SYM_IF, SYM_ODD, SYM_PROCEDURE, SYM_THEN, SYM_VAR, SYM_WHILE
};
```

#### 4.debug过程

**打印报错信息**

```c
"Incorrect symbol"
```

**报错信息来源**

```c
test(fsys, phi, 19);
```

**结论**

​	原始PL0的FIRST(S)集与FOLLOW(S)

![img](https://img-blog.csdn.net/20170316133103290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmVjdHN1bHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

​	then的follow集中要加入else，不然会出现"Incorrect symbol"错误

#### 5.测试

**else测试文件**

```c
var i,j;
begin
  i := -1;
  if i=1 then 
  begin 
	j := i + 7; 
	j := 0;
  end
  else j := 10;
end.
```

**else测试结果**

![2.1 else测试结果]()



------

### Part2 实现break/continue【by zym/htz】

#### 1.原方案：原PL0程序while语句的实现分析

> 原有pl0语言只支持 while循环，为了在循环体中添加break语句，先来分析原PL0编译器是如何实现while循环的

```c
void statement(symset fsys)
{
    //...
	else if (sym == SYM_WHILE)
	{ /*按照while语句处理*/
		cx1 = cx;
		getsym();
		set1 = createset(SYM_DO, SYM_NULL);
		set = uniteset(set1, fsys);
		condition(set);
		destroyset(set1);
		destroyset(set);
        /*条件判断*/
		cx2 = cx;
		gen(JPC, 0, 0);
        /*待回填的条件跳转*/
		if (sym == SYM_DO)
		{
			getsym();
		}
		else
		{
			error(18); // 'do' expected.
		}
		statement(fsys);
        /*处理do后语句*/
		gen(JMP, 0, cx1);
        /*跳转到cx1处*/
		code[cx2].a = cx;
        /*回填跳转地址*/
	}
	//...
}
```

#### 2.一个不完全修改方案：实现单层或特定结构多层的break/continue语句

这是一个初步的想法和思路，在后面会对其进行改进。即利用信号量的思路，利用一个全局变量指示程序中是否出现break。但不完备，因为仅能实现单层的break功能(程序仅含一个break语句)，以及具有特定循环结构的多层break功能(break语句不会出现在while语句前)。对于break语句出现在while语句前的循环和递归调用中的break语句，不能普适地适应。

**实现思路**

> 实现break语句：当前symbol为break时，产生一个待回填的无条件跳转，跳转到while语句的循环出口处(即回填地址)，待回填指令的地址由一个全局变量jx记录
>
> 实现continue语句：与break语句相比，仅需改变跳转地址。break语句跳转到while语句的循环出口，continue语句跳转到while循环的首部。其他操作与break语句相同。
>
> 特别注意：需要定义一个全局变量flag，用来记录是否有需要处理的break/continue

**全局变量jx的定义与初始化**

> 定义全局变量jx,用于记录break跳转指令的位置，初始化为0;
>
> 定义全局变量flag_brk,用于记录当前是否有break关键字需要处理，初始化为0
>
> 定义全局变量bx,用于记录continue跳转指令的位置，初始化为0;
>
> 定义全局变量flag_cnt,用于记录当前是否有continue关键字需要处理，初始化为0

```c
//头文件pl0.c中定义jx,bx,flag_brk,flag_ctn
int jx;  
int flag_brk;
int bx;
int flag_ctn;

//main函数中初始化jx,bx,flag_brk,flag_ctn
err = cc = cx = ll = jx = flag_brk = bx= flag_ctn = 0; 
```

**statement函数中增添条件语句**

```c
else if (sym == SYM_BREAK)
	{ 
    	flag_brk=1;
		jx = cx;
		gen(JMP,0,0); //无条件跳转，用于跳出整个循环，待回填
		getsym();	
	}

else if (sym == SYM_CONTINUE)
	{ 
    	flag_ctn=1;
		bx = cx;
		gen(JMP,0,0); //无条件跳转，用于跳出本次循环，待回填
		getsym();	
	}
```

**statement函数中对while循环的修改**

```c
else if (sym == SYM_WHILE)
	{ 
		cx1 = cx;
		getsym();
		set1 = createset(SYM_DO, SYM_NULL);
		set = uniteset(set1, fsys);
		condition(set);
		destroyset(set1);
		destroyset(set);
		cx2 = cx;
		gen(JPC, 0, 0);
		if (sym == SYM_DO)
		{
			getsym();
		}
		else
		{
			error(18); // 'do' expected.
		}
		statement(fsys);
		gen(JMP, 0, cx1);
		code[cx2].a = cx;
    	if(flag_ctn)
		{	
			code[bx].a=cx1;//回填SYM_CONTINUE的跳转指令,用于跳出本次循环
			flag_ctn=0;
		}	
		if(flag_brk)
		{	
			code[jx].a=cx;//回填SYM_BREAK的跳转指令,用于跳出整个循环
			flag_brk=0;
		}
	}
```

#### 3.一个完全的修改方案：在2的方案中改进break/continue

**实现思路**

> 2中方案的主要缺陷在于，不同层次的break会影响到同一个全局变量flag，从而多个break语句会互相干扰。
>
> 对这个问题的改进是：定义一个全局变量和一个全局数组，分别储存break的层数和break的跳转地址

**全局变量的定义与初始化**

> brk_count表示break的层数，
>
> brk_cx[]存储当前层break后应该跳转到的指令
>
> ctn_count表示break的层数，
>
> ctn_cx[]存储当前层break后应该跳转到的指令

```c
//头文件pl0.c中定义brk_count,brk_cx[4]
int brk_count;  
int brk_cx[4]={0};
int ctn_count;  
int ctn_cx[4]={0};

//main函数中初始化
err = cc = cx = ll = brk_count = ctn_count = 0; 
```

**statement函数中增添条件语句**

```c
else if (sym == SYM_BREAK)
	{ 
    	brk_count++;
		brk_cx[brk_count] = cx;
		gen(JMP,0,0); //无条件跳转，用于跳出整个循环，待回填
		getsym();	
	}

else if (sym == SYM_CONTINUE)
	{ 
    	ctn_count++;
		ctn_cx[ctn_count] = cx;
		gen(JMP,0,0); //无条件跳转，用于跳出整个循环，待回填
		getsym();	
	}
```

**statement函数中对while循环的修改**

```c
else if (sym == SYM_WHILE)
	{ 
		cx1 = cx;
		getsym();
		set1 = createset(SYM_DO, SYM_NULL);
		set = uniteset(set1, fsys);
		condition(set);
		destroyset(set1);
		destroyset(set);
		cx2 = cx;
		gen(JPC, 0, 0);
		if (sym == SYM_DO)
		{
			getsym();
		}
		else
		{
			error(18); // 'do' expected.
		}
		statement(fsys);
        if(ctn_cx[ctn_count]>0)
		{	
			code[ctn_cx[ctn_count]].a=cx1;
			ctn_count--;
		}	
		gen(JMP, 0, cx1);
		code[cx2].a = cx;
    	
    
		if(brk_cx[brk_count]>0)
		{	
			code[brk_cx[brk_count]].a=cx;
			brk_count--;
		}
	}
```

#### 4.其他实现细节(break/continue的词法分析)

> 除了对`void statement(symset fsys)`中if语句结构的修改，还需要增加break/continue关键字，相关实现细节如下

**修改关键字个数**

```c
#define NRW        14     // 关键字个数12->14
```

**在枚举集合中添加else/continue标识**1

```c
enum symtype
{
	//...
	SYM_BREAK,            //增添SYM_BREAK
	SYM_CONTINUE		  //增添SYM_CONTINUE
};
```

**修改保留字个数**

```c
#define NSYM       13  //保留字个数11->13 
```

**设置保留字名字**

因为编译器中查找关键字使用折半查找，限制条件是查找数据必须有序，这里按照首字母字母表顺序

```c
//在"begin"和"call"之间增加"break"
//在"const"和"do"之间增加"continue"
char* word[NRW + 1] =
{
	"", /* place holder */
	"begin", "break","call", "const","continue" "do","else","end","if","odd", "procedure", "then", "var", "while"
};
```

**设置保留字符号**

```c
//在SYM_BEGIN和SYM_CALL之间增加SYM_BREAK
//在SYM_CONST和SYM_DO之间增加SYM_CONTINUE
int wsym[NRW + 1] =
{
	SYM_NULL, SYM_BEGIN, SYM_BREAK,SYM_CALL, SYM_CONST,SYM_CONTINUE,SYM_DO, SYM_ELSE,SYM_END,
	SYM_IF, SYM_ODD, SYM_PROCEDURE, SYM_THEN, SYM_VAR, SYM_WHILE
};
```

#### 5.测试

**break测试文件**

​	目标输出：0 0 0 1 3 200 4 6 200 8 200 18 20

```c

var a,b,c;
begin
	a:=0;
	b:=0;
	c:=0;
	while b<8 do
	begin  
        while b<=5 do
		begin
	    	b:=b+1;
	    	break;
	    	while b<3 do
            begin
            	c:=38;
				break;
				c:=29;
            end;
	    	c:=100;
	 	end; 
        b:=b+2;
	 	c:=200;
    end;
	a:=b+10;
	c:=20;
end.
```

**continue测试文件**

​	目标输出：0 0 0 1 11 2 11 4 2 2 3 99 5

```c
var a,b,c;
begin
   a:=0;
   b:=0;
   c:=0;
   while b<2 do 
   begin
      b:=b+1;
      while a<3 do
      begin
         c:=11;
	     a:=a+2;
         continue;
         c:=1000;
         while a<8 do 
         begin
            a:=a+1;
	    continue;
            c:=1000;
         end;
         c:=12;
      end;
      
      c:=b+1;
      continue;
      c:=1000;
   end;
   c:=99;
   c:=a+1 ;
end.
```





### Part3 实现连续赋值语句

****

#### 1.原方案：原PL0程序赋值语句的实现分析

> 原赋值方案赋值语句：ident  ->  :=  -> 表达式
>
> 读到symbol为SYM_IDENTIFIER即在statement()中,继续读:=，再调用statement()处理赋值符号后的表达式，得到一个值，再赋值给赋值号前的ident

```c
if (sym == SYM_IDENTIFIER)
	{ // variable assignment
		mask* mk;
		if (! (i = position(id)))
		{
			error(11); // Undeclared identifier.
        }
		else if (table[i].kind != ID_VARIABLE)
		{
			error(12); // Illegal assignment.
			i = 0;
		}
		getsym();
		if (sym == SYM_BECOMES)
		{
			getsym();
		}
		else
		{
			error(13); // ':=' expected.
		}

		expression(fsys);
		mk = (mask*) &table[i];
		if (i)
		{
			gen(STO, level - mk->level, mk->address);
		}

	}
```

#### 

