#include<stdio.h>
#include<iostream>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<queue> 
#include<vector>
#include<string>
#include<stack>

using namespace std;
#define MAXTOKENLEN 50
#define MAXTOKENS   100

struct token
{
    char type;
    char string[MAXTOKENLEN];
};

struct token S_stack[MAXTOKENS]; //保存第一个identifier前的所有token

struct token latest;//刚读入的token

typedef enum type_tag
{
	IDENTIFIER,TYPE  
}TYPE_TAG;

/*栈操作*/
int S_top=-1;
#define	S_pop	S_stack[S_top--]
#define	S_push(S_s)	S_stack[++S_top]=S_s 

char StrLine[1024];
int  StrLine_P;

/*反括号栈*/
char OutputStack[1024];
int O_top=-1;
#define	O_pop	OutputStack[O_top--]
#define	O_push(O_s) OutputStack[++O_top]=O_s 

/*值类型队列*/
queue <struct token> Value_Queue;
 

/*类型大小*/
int TypeSize;

/*类型检查*/
int err; 

/*token总数*/
int TokenNum;

/*函数参数处理*/
char IdentifierName[MAXTOKENLEN];
int ip=0;
int k=0;
int para_num=0;
int pretop;
int preO_top;
struct token prestack[MAXTOKENS];


///---【函数声明】-------------------------------
TYPE_TAG ClassifyString();        //F1:字符串分类
void GetToken();                  //F2:取标记
void Read_To_First_Identifier();  		//F3:读至第一个标识符
void Read_To_First_Parameter();			//F4:读至第一个标识符(参数用)
void Skip_First_Parameter();            //F5:跳过参数名(打印用) 


void Deal_With_Function_Args();			//F6:处理函数
void Deal_With_Arrays();				//F7:处理数组
void Deal_With_Possible_Parameters(); 	//F7:处理参数 
void Deal_With_Pointer();				//F8:处理指针
void Deal_With_Function();				//F9:处理函数打印 


void Deal_With_Declarator();      //F10:处理声明器

void Print_Value_Type_Size();	 //F11:输出ValueType及type size

void  errprint();				//F12:错误输出








//F1:字符串分类
TYPE_TAG ClassifyString()
{
	char *s = latest.string;
	if(!strcmp(s, "void")) return TYPE;
    if(!strcmp(s, "int")) return TYPE;
	return IDENTIFIER;
}


//F2:取标记
void GetToken() 
{  
	
	char *p = latest.string;
   
    
    while(StrLine[StrLine_P]==' ')
    {
    	StrLine_P++;
	}
	
    *p=StrLine[StrLine_P];
    
    if(isalnum(*p))//是字母数字组合
	{          
        while(isalnum(*++p = StrLine[++StrLine_P]));  
        *p = '\0';
        latest.type = ClassifyString();
      	TokenNum++;
        return ;
    }      
	
	if(*p == '*'){        //遇到指针 
        strcpy(latest.string, "pointer(");
        latest.type = '*';
        TokenNum++;
        StrLine_P++;
        return ;
    }

	latest.string[1] = '\0';
    latest.type = *p;
	StrLine_P++;
	TokenNum++;
    return ;
}


//F3:读至第一个标识符
void Read_To_First_Identifier()
{
	para_num=0;
    GetToken();
    while(latest.type != IDENTIFIER)
    {
        S_push(latest);
        GetToken();
    }
    k=StrLine_P;
	strcpy(IdentifierName,latest.string);
	Deal_With_Possible_Parameters();
	printf("%s is type of: ",IdentifierName);
	StrLine_P=k;
    Deal_With_Function(); 
    StrLine_P=k;
    GetToken();
}



//F4:读至第一个标识符(参数用)
void Read_To_First_Parameter()
{
    GetToken();
    while(latest.type != IDENTIFIER)
    {
        S_push(latest);
        GetToken();
    }
	printf("parameter %s is type of: ",latest.string);
    GetToken();
} 


//F5:跳过参数名(打印用) 
void Skip_First_Parameter()
{
	GetToken();
    while(latest.type != IDENTIFIER)
    {
        S_push(latest);
        GetToken();
    }
    GetToken();
}




//F6:处理函数
void Deal_With_Function_Args()
{	
	while(latest.type != ')')
	{
		GetToken();
    }
    GetToken();
    printf("=>");	
}


//F7:处理数组
void Deal_With_Arrays()
{
    //读取"[size]"后将其打印并继续读取
	while(latest.type == '['){
//		struct token *value=new struct token;
		struct token value;
		value.type='p';
        printf("array");
        strcpy(value.string,"array\0");     
        Value_Queue.push(value);
        printf("(");
        strcpy(value.string,"(\0");     
        Value_Queue.push(value);
        
        O_push(')');
       
        GetToken();
        if(isdigit(latest.string[0])){
            printf("%d",atoi(latest.string));
            Value_Queue.push(latest);
			printf(",");
			strcpy(value.string,",\0");     
			Value_Queue.push(value);
//            TypeSize=TypeSize*atoi(latest.string);
            GetToken();
        }
        else
        {
        	printf("%s,",latest.string);
			Value_Queue.push(latest);
			strcpy(value.string,",\0");     
			Value_Queue.push(value);
			err=1;                       //err1:array'length should be a digits
			GetToken();
		}
        GetToken();
        if(latest.type == '(')
          err = 4;                      //err4:array fo function,a[]() is not allowed
    }
}




//F8:处理指针
void Deal_With_Pointer()
{   
	while(S_stack[S_top].type == '*')
	{
        printf("%s", S_stack[S_top].string);
		 O_push(')');
//        cout<<"【指针push】";
        Value_Queue.push(S_stack[S_top]);
        S_top--;
    }
}	

//F8:处理参数 
void Deal_With_Possible_Parameters(){
	for(ip=0;ip<S_top;ip++){
		prestack[ip]=S_stack[ip];
	}
	pretop=S_top;
	preO_top=O_top;
	O_top=-1;
	GetToken();
	if(latest.type=='('){
		struct token value;
		value.type='p';
    	strcpy(value.string,"function\0");     
    	Value_Queue.push(value);
		do
		{
			para_num++;
			S_top=-1;
			Read_To_First_Parameter();
			Deal_With_Declarator();
			printf("\n");
//			printf("%s\n",latest.string);
		}
		while(latest.type==','||latest.type!=')');
	}
	
	S_top=pretop;
	O_top=preO_top;
	for(ip=0;ip<S_top;ip++){
		S_stack[ip]=prestack[ip];
	}
}

//F9:处理函数打印 
void Deal_With_Function(){
	if(!para_num)
		return;
	else{
		printf("function(");
		O_push(')');
		for(ip=0;ip<S_top;ip++){
		prestack[ip]=S_stack[ip];
		}
		pretop=S_top;
		preO_top=O_top;
		O_top=-1;
	
		GetToken();
		for(ip=0;ip<para_num;ip++){
			S_top=-1;
			Skip_First_Parameter();
			Deal_With_Declarator();
			if(ip<para_num-1 )printf(" X ");
		}
			
		S_top=pretop;
		O_top=preO_top;
		for(ip=0;ip<S_top;ip++){
			S_stack[ip]=prestack[ip];
		}
	}
}




//F10:处理声明器
void Deal_With_Declarator()
{
	if(latest.type=='[')
		Deal_With_Arrays();
	
    else if (latest.type == '(')
    	Deal_With_Function_Args();
    
	Deal_With_Pointer();
	
	while(S_top >= 0)
	{
        
        if(S_stack[S_top].type == '('){  //判断是否是左括号
            S_pop;
            GetToken();
            Deal_With_Declarator();
        }
		else
		{
			
            printf("%s",S_stack[S_top].string);
            Value_Queue.push(S_stack[S_top]);
            S_top--;

    	}
        
        while(O_top>=0)
		{
			printf("%c",O_pop);
			struct token value;
			value.type='p';
        	strcpy(value.string,")\0");     
        	Value_Queue.push(value);	
		}
        
	}
	if(StrLine[StrLine_P]=='[')
	{
		err=2;          //函数返回值为数组。 
	}
	else if(StrLine[StrLine_P]=='(')
	{
		err=3;          //函数返回值为函数。 
	}
}


//F11:输出ValueType及type size
void Print_Value_Type_Size()
{
        int flag_value;  //0表示array,1表示pointer,2表示int,3表示为其他
        
        vector<struct token> T;
        while(!Value_Queue.empty())
        {
        	T.push_back(Value_Queue.front());
        	Value_Queue.pop();
		}
		
        
        struct token Qmem=T.front();
        if(!strcmp(Qmem.string,"array")) flag_value=0;
        else if(!strcmp(Qmem.string,"pointer(")) flag_value=1;
        else if(!strcmp(Qmem.string,"int")) flag_value=2;
        else flag_value=3;
        printf("\n");
        
        //输出ValueType 
        switch(flag_value)
        {
        	case 0:{
        		cout<<"ValueType is:";
        		vector<struct token>::iterator it;
				for (it = T.begin()+4; it != T.end()-1; it++)
    				cout << it->string;
				break;
			}	
        	case 1:{
        		cout<<"ValueType is:";
        		vector<struct token>::iterator it;
				for (it = T.begin()+1; it != T.end()-1; it++)
    				cout << it->string;
				break;
			}
			case 2:{
				break;
			}
        	default: break;
		}
		cout<<endl;
		//输出type size 
		switch(flag_value)
        {
        	case 0:{
        		cout<<"type size: ";
        		vector<struct token>::iterator it;
				for (it = T.begin()+2; it != T.end()-1; it++){
					char *p=it->string;
					if(isalnum(*p)&&(!isalpha(*p))) TypeSize=TypeSize*atoi(it->string);
					if(!strcmp(p,"pointer(")) break; 				
				}
    			cout << TypeSize;
				break;
			}	
        	case 1:{
        	    TypeSize=1;
        	    printf("type size: %d\n",TypeSize);
				break;
			}
			case 2:{
				TypeSize=1;
				printf("type size: %d\n",TypeSize);
				break;
			}
        	default: break;
		}	
}

//F12:错误输出
void  errprint()
{
	switch(err)
	{
		case 1:printf("The length of Array must be digits\n");break;
		case 2:printf("The return of Functions can't be Array!\n");break;
		case 3:printf("The return of Functions can't be Function!\n");break;
	    case 4:printf("Array of Function is not allowed!\n");break;
	}
}

int main()
{
	
	char filename[]="test2.txt";
	FILE *fp;

	
	if((fp = fopen(filename,"r")) == NULL){
		printf("cannot open file");
		return -1; 
	}
	
	/*处理每行声明*/
	while (!feof(fp))
	{
		TokenNum=0;
		StrLine[1024]={0};
        StrLine_P=0;
        TypeSize=1; 
        err =0;
		fgets(StrLine,1024,fp); 
		printf("声明式:%s",StrLine);
		Read_To_First_Identifier();
    	Deal_With_Declarator();
    	
    	printf("\nTypeChecking...");
		if(err==0)
    	{		
		  printf("OK!");
		  Print_Value_Type_Size();  //输出ValueType及type size 
          
		}
		else
		{
			printf("\nError %d:",err);
			errprint();
			printf("error!\n");
			}
    	while(latest.type==',')
    	{
    		printf("\n");
    		S_top =0;
    		err = 0;
    		Read_To_First_Identifier();
    		Deal_With_Declarator();		
      		printf("\nTypeChecking...");
		if(err==0)
    	{		
		  printf("OK!");
		  Print_Value_Type_Size();  //输出ValueType及type size 
          
		}
		else
		{
			printf("ERROR %d:",err);
			errprint();
			printf("error!\n");
			}
		}
    
		printf("\n\n");
	}	
	return 0; 
}








