// pl0 compiler source code



#pragma warning(disable:4996)





#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>



#include "pl011.h"

#include "11set.c"



//////////////////////////////////////////////////////////////////////

// print error message.

void error(int n)

{

	int i;



	printf("      ");

	for (i = 1; i <= cc - 1; i++)

		printf(" ");

	printf("^\n");

	printf("Error %3d: %s\n", n, err_msg[n]);

	err++;

} // error



//////////////////////////////////////////////////////////////////////

void getch(void)

{

	if (cc == ll)

	{

		if (feof(infile))    //next 没有了                                              //infile  是文件

		{

			printf("\nPROGRAM INCOMPLETE\n");//文件未完成

			exit(1);

		}

		ll = cc = 0;

		printf("%5d  ", cx);

		while ( (!feof(infile)) // added & modified by alex 01-02-09 继续读文件

			    && ((ch = getc(infile)) != '\n'))//并且读到的字符不为'\n'

		{

			printf("%c", ch);

			line[++ll] = ch;

		} // while

		//遇到\n跳出循环，

		printf("\n");

		line[++ll] = ' ';

	}

	ch = line[++cc];//（若是从循环出来的，就是新读入的字符串中的第一个，若cc！=ll，即ll>cc，则会是新读取(cc中没有但是ll中有的)的第一个。）

} // getch

//则getch函数就是：若是ll中有cc中没有的，就ch为读取没有的第一个；否则就是读取一个串直到遇到\n，ch为读取的第一个

//////////////////////////////////////////////////////////////////////

// gets a symbol from input stream.

void getsym(void)

{

	int i, k;

	char a[MAXIDLEN + 1];



	while (ch == ' '||ch == '\t')

		getch();//要是遇到的是空格或者制表符，就调用getch函数，直到有字符出现



	if (isalpha(ch))//读到英文字母

	{ // symbol is a reserved word or an identifier.

		k = 0;

		do

		{

			if (k < MAXIDLEN)

				a[k++] = ch;

			getch();

		}

		while (isalpha(ch) || isdigit(ch));//字母数字组成

		a[k] = 0;

		strcpy(id, a);

		word[0] = id;

		i = NRW;

		while (strcmp(id, word[i--]));

		if (++i)

			sym = wsym[i]; // symbol is a reserved word

		else                //要是i=0时才匹配，则i此时为-1，也就是id不是关键字

			sym = SYM_IDENTIFIER;   // symbol is an identifier

	}

	else if (isdigit(ch))    //第一个是数字

	{ // symbol is a number.

		k = num = 0;

		sym = SYM_NUMBER;         //则它肯定是一个数字

		do

		{

			num = num * 10 + ch - '0';//将他们转换成十进制

			k++;

			getch();

		}

		while (isdigit(ch));

		if (k > MAXNUMLEN)

			error(25);     // The number is too great.

	}

	else if (ch == ':')   ///！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！1

	{

		getch();

		if (ch == '=')

		{

			sym = SYM_BECOMES; // :=

			getch();

		}

		else

		{

			sym = SYM_NULL;       // illegal?

		}

	}

	else if (ch == '>')

	{

		getch();

		if (ch == '=')

		{

			sym = SYM_GEQ;     // >=

			getch();

		}

		else

		{

			sym = SYM_GTR;     // >

		}

	}

	else if (ch == '<')

	{

		getch();

		if (ch == '=')

		{

			sym = SYM_LEQ;     // <=

			getch();

		}

		else if (ch == '>')

		{

			sym = SYM_NEQ;     // <>     ？？？？？？？？？？？？？？？

			getch();

		}

		else

		{

			sym = SYM_LES;     // <

		}

	}

	else

	{ // other tokens,加减乘除等运算符

		i = NSYM;

		csym[0] = ch;

		while (csym[i--] != ch);

		if (++i)                

		{

			sym = ssym[i];

			getch();

		}

		else            //也不是运算符

		{

			printf("Fatal Error: Unknown character.\n");

			exit(1);

		}

	}

} // getsym



//////////////////////////////////////////////////////////////////////

// generates (assembles) an instruction.组合产生一条指令

void gen(int x, int y, int z)

{

	if (cx > CXMAX)

	{

		printf("Fatal Error: Program too long.\n");

		exit(1);

	}

	code[cx].f = x;

	code[cx].l = y;

	code[cx++].a = z;

} // gen



//////////////////////////////////////////////////////////////////////

// tests if error occurs and skips all symbols that do not belongs to s1 or s2.

void test(symset s1, symset s2, int n)

{

	symset s;



	if (! inset(sym, s1))            //不在其中s1中

	{

		error(n);

		s = uniteset(s1, s2);      //合并s1和s2

		while(! inset(sym, s))

			getsym();             //有非法的，就舍弃掉，直到有合法的输入为止

		destroyset(s);

	}

} // test



//////////////////////////////////////////////////////////////////////

int dx;  // data allocation index



// enter object(constant, variable or procedre) into table.

void enter(int kind)

{

	mask* mk;



	tx++;

	strcpy(table[tx].name, id);

	table[tx].kind = kind;

	switch (kind)

	{

	case ID_CONSTANT:

		if (num > MAXADDRESS)

		{

			error(25); // The number is too great.

			num = 0;

		}

		table[tx].value = num;

		break;

	case ID_VARIABLE:

		mk = (mask*) &table[tx];

		mk->level = level;

		mk->address = dx++;

		break;

	case ID_PROCEDURE:

		mk = (mask*) &table[tx];

		mk->level = level;

		break;

	} // switch

} // enter



//////////////////////////////////////////////////////////////////////

// locates identifier in symbol table.

int position(char* id)

{

	int i;

	strcpy(table[0].name, id);

	i = tx + 1;

	while (strcmp(table[--i].name, id) != 0);

	return i;

} // position



//////////////////////////////////////////////////////////////////////

//常数声明

void constdeclaration()                      

{

	if (sym == SYM_IDENTIFIER)

	{

		getsym();

		if (sym == SYM_EQU || sym == SYM_BECOMES)        //=或者:=的时候

		{

			if (sym == SYM_BECOMES)

				error(1); // Found ':=' when expecting '='.

			getsym();

			if (sym == SYM_NUMBER)

			{

				enter(ID_CONSTANT);

				getsym();

			}

			else

			{

				error(2); // There must be a number to follow '='.

			}

		}

		else

		{

			error(3); // There must be an '=' to follow the identifier.

		}

	} else	error(4);

	 // There must be an identifier to follow 'const', 'var', or 'procedure'.

} // constdeclaration



//////////////////////////////////////////////////////////////////////

//变量声明

void vardeclaration(void)

{

	if (sym == SYM_IDENTIFIER)

	{

		enter(ID_VARIABLE);

		getsym();

	}

	else

	{

		error(4); // There must be an identifier to follow 'const', 'var', or 'procedure'.

	}

} // vardeclaration



//////////////////////////////////////////////////////////////////////

//输出从from到to指令

void listcode(int from, int to)

{

	int i;

	

	printf("\n");

	for (i = from; i < to; i++)

	{

		printf("%5d %s\t%d\t%d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);

	}

	printf("\n");

} // listcode



//////////////////////////////////////////////////////////////////////

//加减括号

void factor(symset fsys)

{

	void expression(symset fsys);

	int i;

	symset set;

	

	test(facbegsys, fsys, 24); // The symbol can not be as the beginning of an expression.这是啥鬼东西



	if (inset(sym, facbegsys))

	{

		if (sym == SYM_IDENTIFIER)

		{//检查当前标识符是什么东西：变量，常量还是程序名

			if ((i = position(id)) == 0)

			{

				error(11); // Undeclared identifier.

			}

			else

			{

				switch (table[i].kind)

				{

					mask* mk;

				case ID_CONSTANT:

					gen(LIT, 0, table[i].value);

					break;

				case ID_VARIABLE:

					mk = (mask*) &table[i];

					gen(LOD, level - mk->level, mk->address);

					break;

				case ID_PROCEDURE:

					error(21); // Procedure identifier can not be in an expression.

					break;

				} // switch

			}

			getsym();

		}

		else if (sym == SYM_NUMBER)

		{

			if (num > MAXADDRESS)

			{

				error(25); // The number is too great.

				num = 0;

			}

			gen(LIT, 0, num);

			getsym();

		}

		else if (sym == SYM_LPAREN)              //左括号

		{

			getsym();

			set = uniteset(createset(SYM_RPAREN, SYM_NULL), fsys);

			expression(set);

			destroyset(set);

			if (sym == SYM_RPAREN)

			{

				getsym();

			}

			else

			{

				error(22); // Missing ')'.

			}

		}

		else if(sym == SYM_MINUS) // UMINUS,  Expr -> '-' Expr

		{  

			 getsym();

			 factor(fsys);

			 gen(OPR, 0, OPR_NEG);

		}

		test(fsys, createset(SYM_LPAREN, SYM_NULL), 23);

	} // if

} // factor



//////////////////////////////////////////////////////////////////////

//乘除

void term(symset fsys)

{

	int mulop;

	symset set;

	

	set = uniteset(fsys, createset(SYM_TIMES, SYM_SLASH, SYM_NULL));

	factor(set);

	while (sym == SYM_TIMES || sym == SYM_SLASH)//多个乘除操作

	{

		mulop = sym;

		getsym();

		factor(set);

		if (mulop == SYM_TIMES)

		{

			gen(OPR, 0, OPR_MUL);

		}

		else

		{

			gen(OPR, 0, OPR_DIV);

		}

	} // while

	destroyset(set);

} // term



//////////////////////////////////////////////////////////////////////

//表达式

void expression(symset fsys)

{

	int addop;

	symset set;



	set = uniteset(fsys, createset(SYM_PLUS, SYM_MINUS, SYM_NULL));

	

	term(set);

	while (sym == SYM_PLUS || sym == SYM_MINUS)

	{

		addop = sym;

		getsym();

		term(set);

		if (addop == SYM_PLUS)

		{

			gen(OPR, 0, OPR_ADD);

		}

		else

		{

			gen(OPR, 0, OPR_MIN);

		}

	} // while



	destroyset(set);

} // expression



//////////////////////////////////////////////////////////////////////

//条件语句

void condition(symset fsys)

{

	int relop;

	symset set;



	if (sym == SYM_ODD)

	{

		getsym();

		expression(fsys);

		gen(OPR, 0, 6);

	}

	else

	{

		set = uniteset(relset, fsys);

		expression(set);

		destroyset(set);

		if (! inset(sym, relset))

		{

			error(20);  //Relative operators expected.

		}

		else

		{

			relop = sym;

			getsym();

			expression(fsys);

			switch (relop)

			{

			case SYM_EQU:

				gen(OPR, 0, OPR_EQU);

				break;

			case SYM_NEQ:

				gen(OPR, 0, OPR_NEQ);

				break;

			case SYM_LES:

				gen(OPR, 0, OPR_LES);

				break;

			case SYM_GEQ:

				gen(OPR, 0, OPR_GEQ);

				break;

			case SYM_GTR:

				gen(OPR, 0, OPR_GTR);

				break;

			case SYM_LEQ:

				gen(OPR, 0, OPR_LEQ);

				break;

			} // switch

		} // else

	} // else

} // condition



//////////////////////////////////////////////////////////////////////

void fuzhi(symset fsys,int i)
{
	mask* mk;
        int j;
        int asym;

	if(sym!=SYM_IDENTIFIER)
		expression(fsys);
	else
	{
                symset set;
                mask* mk0;
                set = uniteset(fsys,createset(SYM_PLUS,SYM_MINUS,SYM_TIMES, SYM_SLASH,SYM_BECOMES,SYM_NULL));
		if((j=position(id)) == 0)
		{
			error(11);
		}
		/*else
		{
			switch (table[j].kind)
				{
					//mask* mk0;
				case ID_CONSTANT:
					gen(LIT, 0, table[j].value);
					break;
				case ID_VARIABLE:
					mk0 = (mask*) &table[j];
					gen(LOD, level - mk0->level, mk0->address);
					break;
				case ID_PROCEDURE:
					error(21); // Procedure identifier can not be in an expression.
					break;
				} // switch
		}*/
                asym=sym;
                getsym();
		test(set, createset(SYM_LPAREN,SYM_NULL), 23);
         
         if(sym == SYM_BECOMES)
         {
                   // gen(STO, level - mk0->level, mk0->address);
		    if (table[j].kind != ID_VARIABLE)
		    {
			error(12); // Illegal assignment.
			j = 0;
		     }
		
		getsym();
		fuzhi(fsys,j);
		
         }
         //后面是一个表达式的时候
         else
         {
                
         	int mulop;
         	symset set0;
                if(j!=0)
                 {
                   switch (table[j].kind)
				{
					//mask* mk0;
				case ID_CONSTANT:
					gen(LIT, 0, table[j].value);
					break;
				case ID_VARIABLE:
					mk0 = (mask*) &table[j];
					gen(LOD, level - mk0->level, mk0->address);
					break;
				case ID_PROCEDURE:
					error(21); // Procedure identifier can not be in an expression.
					break;
				} // switch
                  }
         	set0 = uniteset(fsys,createset(SYM_PLUS,SYM_MINUS,SYM_TIMES, SYM_SLASH, SYM_NULL));
         	while(sym == SYM_TIMES || sym == SYM_SLASH)
         	{
                  mulop = sym;
		          getsym();
		          factor(set0);
		          if (mulop == SYM_TIMES)
		        {
			      gen(OPR, 0, OPR_MUL);
		        }
		          else
		        {
			      gen(OPR, 0, OPR_DIV);
		        }
         	}
         	destroyset(set0);



         	int addop;
         	symset set1;
         	set1 = uniteset(fsys,createset(SYM_PLUS,SYM_MINUS,SYM_NULL));
         	while(sym == SYM_PLUS || sym == SYM_MINUS)
         	{
         		addop = sym;
         		getsym();
         		term(set1);
         		if(addop== SYM_PLUS)
         		{
         			gen(OPR, 0 , OPR_ADD);
         		}
         		else
         		{
         			gen(OPR, 0 , OPR_MIN);
         		}
         	}
         	destroyset(set1);
         }
	}
    mk = (mask*) &table[i];
		if (i)
		{
			gen(STO, level - mk->level, mk->address);
                        gen(LOD, level - mk->level, mk->address);
		}
}

///////////////////////////////////////////////////////////////////////

//声明

void statement(symset fsys)

{

	int i, cx1, cx2;

	symset set1, set;



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

		if (sym == SYM_BECOMES)                            //赋值语句！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！1

		{

			getsym();

		}

		else

		{

			error(13); // ':=' expected.

		}

		fuzhi(fsys,i);

		//*expression(fsys);

		mk = (mask*) &table[i];

		if (i)

		{

			gen(STO, level - mk->level, mk->address);

		}

	}

	else if (sym == SYM_CALL)               //程序调用

	{ // procedure call

		getsym();

		if (sym != SYM_IDENTIFIER)

		{

			error(14); // There must be an identifier to follow the 'call'.

		}

		else

		{

			if (! (i = position(id)))

			{

				error(11); // Undeclared identifier.

			}

			else if (table[i].kind == ID_PROCEDURE)

			{

				mask* mk;

				mk = (mask*) &table[i];

				gen(CAL, level - mk->level, mk->address);

			}

			else

			{

				error(15); // A constant or variable can not be called. 

			}

			getsym();

		}

	} 

	else if (sym == SYM_IF)

	{ // if statement

		getsym();

		set1 = createset(SYM_THEN, SYM_DO, SYM_NULL);

		set = uniteset(set1, fsys);

		condition(set);

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

		statement(fsys);

		code[cx1].a = cx;	

	}

	else if (sym == SYM_BEGIN)

	{ // block

		getsym();

		set1 = createset(SYM_SEMICOLON, SYM_END, SYM_NULL);

		set = uniteset(set1, fsys);

		statement(set);

		while (sym == SYM_SEMICOLON || inset(sym, statbegsys))

		{

			if (sym == SYM_SEMICOLON)

			{

				getsym();

			}

			else

			{

				error(10);

			}

			statement(set);

		} // while

		destroyset(set1);

		destroyset(set);

		if (sym == SYM_END)

		{

			getsym();

		}

		else

		{

			error(17); // ';' or 'end' expected.

		}

	}

	else if (sym == SYM_WHILE)

	{ // while statement

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

	}

	test(fsys, phi, 19);

} // statement

			

//////////////////////////////////////////////////////////////////////

void block(symset fsys)

{

	int cx0; // initial code index

	mask* mk;

	int block_dx;

	int savedTx;

	symset set1, set;



	dx = 3;

	block_dx = dx;

	mk = (mask*) &table[tx];                                      //?????????

	mk->address = cx;

	gen(JMP, 0, 0);                               //到第零条指令

	if (level > MAXLEVEL)

	{

		error(32); // There are too many levels.

	}

	do

	{

		if (sym == SYM_CONST)

		{ // constant declarations

			getsym();

			do

			{

				constdeclaration();

				while (sym == SYM_COMMA)

				{

					getsym();

					constdeclaration();

				}

				if (sym == SYM_SEMICOLON)

				{

					getsym();

				}

				else

				{

					error(5); // Missing ',' or ';'.

				}

			}

			while (sym == SYM_IDENTIFIER);

		} // if



		if (sym == SYM_VAR)

		{ // variable declarations

			getsym();

			do

			{

				vardeclaration();

				while (sym == SYM_COMMA)

				{

					getsym();

					vardeclaration();

				}

				if (sym == SYM_SEMICOLON)

				{

					getsym();

				}

				else

				{

					error(5); // Missing ',' or ';'.

				}

			}

			while (sym == SYM_IDENTIFIER);

		} // if

		block_dx = dx; //save dx before handling procedure call!

		while (sym == SYM_PROCEDURE)

		{ // procedure declarations

			getsym();

			if (sym == SYM_IDENTIFIER)

			{

				enter(ID_PROCEDURE);

				getsym();

			}

			else

			{

				error(4); // There must be an identifier to follow 'const', 'var', or 'procedure'.

			}





			if (sym == SYM_SEMICOLON)

			{

				getsym();

			}

			else

			{

				error(5); // Missing ',' or ';'.

			}



			level++;

			savedTx = tx;

			set1 = createset(SYM_SEMICOLON, SYM_NULL);

			set = uniteset(set1, fsys);

			block(set);

			destroyset(set1);

			destroyset(set);

			tx = savedTx;

			level--;



			if (sym == SYM_SEMICOLON)

			{

				getsym();

				set1 = createset(SYM_IDENTIFIER, SYM_PROCEDURE, SYM_NULL);

				set = uniteset(statbegsys, set1);

				test(set, fsys, 6);

				destroyset(set1);

				destroyset(set);

			}

			else

			{

				error(5); // Missing ',' or ';'.

			}

		} // while

		dx = block_dx; //restore dx after handling procedure call!

		set1 = createset(SYM_IDENTIFIER, SYM_NULL);

		set = uniteset(statbegsys, set1);

		test(set, declbegsys, 7);

		destroyset(set1);

		destroyset(set);

	}

	while (inset(sym, declbegsys));



	code[mk->address].a = cx;

	mk->address = cx;

	cx0 = cx;

	gen(INT, 0, block_dx);

	set1 = createset(SYM_SEMICOLON, SYM_END, SYM_NULL);

	set = uniteset(set1, fsys);

	statement(set);

	destroyset(set1);

	destroyset(set);

	gen(OPR, 0, OPR_RET); // return

	test(fsys, phi, 8); // test for error: Follow the statement is an incorrect symbol.

	listcode(cx0, cx);

} // block



//////////////////////////////////////////////////////////////////////

//????

int base(int stack[], int currentLevel, int levelDiff)

{

	int b = currentLevel;

	

	while (levelDiff--)

		b = stack[b];

	return b;

} // base



//////////////////////////////////////////////////////////////////////

// interprets and executes codes.

void interpret()

{

	int pc;        // program counter

	int stack[STACKSIZE];

	int top;       // top of stack

	int b;         // program, base, and top-stack register

	instruction i; // instruction register



	printf("Begin executing PL/0 program.\n");



	pc = 0;

	b = 1;

	top = 3;

	stack[1] = stack[2] = stack[3] = 0;

	do

	{

		i = code[pc++];

		switch (i.f)

		{

		case LIT:

			stack[++top] = i.a;

			break;

		case OPR:

			switch (i.a) // operator

			{

			case OPR_RET:

				top = b - 1;

				pc = stack[top + 3];

				b = stack[top + 2];

				break;

			case OPR_NEG:

				stack[top] = -stack[top];

				break;

			case OPR_ADD:

				top--;

				stack[top] += stack[top + 1];

				break;

			case OPR_MIN:

				top--;

				stack[top] -= stack[top + 1];

				break;

			case OPR_MUL:

				top--;

				stack[top] *= stack[top + 1];

				break;

			case OPR_DIV:

				top--;

				if (stack[top + 1] == 0)

				{

					fprintf(stderr, "Runtime Error: Divided by zero.\n");

					fprintf(stderr, "Program terminated.\n");

					continue;

				}

				stack[top] /= stack[top + 1];

				break;

			case OPR_ODD:

				stack[top] %= 2;

				break;

			case OPR_EQU:

				top--;

				stack[top] = stack[top] == stack[top + 1];

				break;

			case OPR_NEQ:

				top--;

				stack[top] = stack[top] != stack[top + 1];

				break;

			case OPR_LES:

				top--;

				stack[top] = stack[top] < stack[top + 1];

				break;

			case OPR_GEQ:

				top--;

				stack[top] = stack[top] >= stack[top + 1];

				break;

			case OPR_GTR:

				top--;

				stack[top] = stack[top] > stack[top + 1];

				break;

			case OPR_LEQ:

				top--;

				stack[top] = stack[top] <= stack[top + 1];

				break;

			} // switch

			break;

		case LOD:

			stack[++top] = stack[base(stack, b, i.l) + i.a];

			break;

		case STO:

			stack[base(stack, b, i.l) + i.a] = stack[top];

			printf("%d\n", stack[top]);

			top--;

			break;

		case CAL:

			stack[top + 1] = base(stack, b, i.l);

			// generate new block mark

			stack[top + 2] = b;

			stack[top + 3] = pc;

			b = top + 1;

			pc = i.a;

			break;

		case INT:

			top += i.a;

			break;

		case JMP:

			pc = i.a;

			break;

		case JPC:

			if (stack[top] == 0)

				pc = i.a;

			top--;

			break;

		} // switch

	}

	while (pc);



	printf("End executing PL/0 program.\n");

} // interpret



//////////////////////////////////////////////////////////////////////

int main ()

{

	FILE* hbin;

	char s[80];

	int i;

	symset set, set1, set2;



	printf("Please input source file name: "); // get file name to be compiled

	scanf("%s", s);

	if ((infile = fopen(s, "r")) == NULL)

	{

		printf("File %s can't be opened.\n", s);

		exit(1);

	}



	phi = createset(SYM_NULL);

	relset = createset(SYM_EQU, SYM_NEQ, SYM_LES, SYM_LEQ, SYM_GTR, SYM_GEQ, SYM_NULL);

	

	// create begin symbol sets

	declbegsys = createset(SYM_CONST, SYM_VAR, SYM_PROCEDURE, SYM_NULL);

	statbegsys = createset(SYM_BEGIN, SYM_CALL, SYM_IF, SYM_WHILE, SYM_NULL);

	facbegsys = createset(SYM_IDENTIFIER, SYM_NUMBER, SYM_LPAREN, SYM_MINUS, SYM_NULL);



	err = cc = cx = ll = 0; // initialize global variables

	ch = ' ';

	kk = MAXIDLEN;



	getsym();



	set1 = createset(SYM_PERIOD, SYM_NULL);

	set2 = uniteset(declbegsys, statbegsys);

	set = uniteset(set1, set2);

	block(set);                      //set就是const、var、procedure、null、begin、call、if、while、.

	destroyset(set1);

	destroyset(set2);

	destroyset(set);

	destroyset(phi);

	destroyset(relset);

	destroyset(declbegsys);

	destroyset(statbegsys);

	destroyset(facbegsys);



	if (sym != SYM_PERIOD)

		error(9); // '.' expected.

	if (err == 0)

	{

		hbin = fopen("hbin.txt", "w");

		for (i = 0; i < cx; i++)

			fwrite(&code[i], sizeof(instruction), 1, hbin);

		fclose(hbin);

	}

	if (err == 0)

		interpret();

	else

		printf("There are %d error(s) in PL/0 program.\n", err);

	listcode(0, cx);

	return 0;

} // main



//////////////////////////////////////////////////////////////////////

// eof pl0.c

