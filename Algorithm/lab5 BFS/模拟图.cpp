#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<windows.h>
#define TRUE  1
#define FALSE 0
#define maxnum 100000
using namespace std;
int visited[maxnum]={0};

typedef int vertextype;
typedef int infotype;
 
typedef struct node		//边
{
	int adjvertex;
	infotype info;
	struct node* next;
}edgenode;

typedef struct vnode	//顶点
{
    vertextype vertex;
    edgenode *firstedge;
}vertexnode;

typedef struct			//图
{
    vertexnode adjlist[maxnum];
    int vertexnum;
    int edgenum;
}ALgraph;


typedef struct			//队列
{
	vertexnode data[maxnum];
	int front,rear;
}queue,*Pqueue;


//F1:图的创建函数
void creat(ALgraph *g)
{
	int i,j,k;
	edgenode *p,*q;
	
    printf(">>>开始创建顶点\n");
	for(i=0;i<g->vertexnum;i++)
	{
		g->adjlist[i].vertex=i;
		g->adjlist[i].firstedge=NULL;
	}

	printf(">>>开始创建边\n");
	srand((unsigned)time(NULL));
	for(k=0;k<g->edgenum;k++)
	{
		i=rand()%(g->vertexnum);
		j=rand()%(g->vertexnum);
//		printf("第%d条边由顶点%d到顶点%d\n",k+1,i,j); 
		p=(edgenode *)malloc(sizeof(edgenode));
		p->adjvertex=j;
		p->next=NULL;
		if(g->adjlist[i].firstedge==NULL)
		g->adjlist[i].firstedge=p;
		else
		{
		    q=g->adjlist[i].firstedge;
			g->adjlist[i].firstedge=p;	
			p->next=q;
		}
	}
}



//F2：队列基本操作函数
	//F2-1：初始化 
	Pqueue init()
	{
		Pqueue q;
		q=(Pqueue)malloc(sizeof(queue));
		if(q)
		{
			q->front=0;
			q->rear=0;
		}
		return q;
	}
	
	//F2-2: 判断队列是否为空 
	int empty(Pqueue q)
	{
		if(q&&q->front==q->rear)
		return 1;
		else return 0;
	} 
	
	//F2-3: 入队列
	int in(Pqueue q,ALgraph *g,int v)
	{
		if((q->rear+1)%maxnum==q->front)
		{
			printf("队满");
			return -1;
		}
		else
		{
			q->rear++;
			q->data[q->rear]=g->adjlist[v];
			return 1;
		}
	}
	
	//F2-4: 出队列
	int out(Pqueue q,vertexnode *x)
	{
		if(empty(q))
		{
			printf("队空\n");
			return -1;
		}
		else
		{
			q->front++;
			*x=q->data[q->front];
			return 1;
		}
	}
	 
//F3：visit()
void visit(ALgraph *g,int v)
{
	printf("[%d] ",g->adjlist[v].vertex);
}

 

//F4：BFS函数 
void bfs(ALgraph *g,int v)	
{
	edgenode *p;
	int w;
	vertexnode u;
	Pqueue q;
	q=init();
	visited[v]=TRUE;
	in(q,g,v);
	int v1;
	for(v1=0;v1<g->vertexnum;v1++)
	visited[v1]=false;
	int i=1;
	while(!empty(q))
	{
		out(q,&u);
		for(p=u.firstedge;p;p=p->next)
		{
			w=p->adjvertex;
			if(!visited[w])
			{
//				visit(g,w);
				visited[w]=true;
				in(q,g,w);
			}
		}	
		
	}
}


//debug:打印生成的图
void print(ALgraph *g)
{
	int Enum=g->edgenum;
	int Vnum=g->vertexnum;
	printf("Enum=%d\n",g->edgenum);
	printf("Vnum=%d\n",g->vertexnum);
	for(int i=0;i<Vnum;i++)
	{
		printf("顶点<%d>指向:",i);
		edgenode *p=g->adjlist[i].firstedge;
		while(p)
		{
			printf("顶点[%d] ",p->adjvertex);
			p=p->next;
		}
		
	}
}

int main()
{
	ALgraph *g;
	ALgraph GRAPH;
	g=&GRAPH;
	
//	//规模为10
//	printf("规模为10\n"); 
//	g->vertexnum=5;
//    g->edgenum=10;
//    
//    //规模为100 
//    printf("规模为100\n");
//	g->vertexnum=70;
//    g->edgenum=100;
//    
//    //规模为1000
//	printf("规模为1000\n"); 
//	g->vertexnum=30;
//    g->edgenum=1000;
//    
//    //规模为10000
//	printf("规模为10000\n"); 
//	g->vertexnum=600;
//    g->edgenum=10000;
//    
    //规模为100000
	printf("规模为100000\n"); 
	g->vertexnum=80000;
    g->edgenum=100000;
    
    
	clock_t t1,t2;
	t1=clock();
	creat(g);
	t2=clock();
	cout<<">>>建图时间:"<<t2-t1<<"ms"<<endl;
    t1=clock();
	bfs(g,0);
	t2=clock();
	cout<<">>>BFS时时间:"<<t2-t1<<"ms"<<endl;
	return 0;
}





