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
 
typedef struct node		//��
{
	int adjvertex;
	infotype info;
	struct node* next;
}edgenode;

typedef struct vnode	//����
{
    vertextype vertex;
    edgenode *firstedge;
}vertexnode;

typedef struct			//ͼ
{
    vertexnode adjlist[maxnum];
    int vertexnum;
    int edgenum;
}ALgraph;


typedef struct			//����
{
	vertexnode data[maxnum];
	int front,rear;
}queue,*Pqueue;


//F1:ͼ�Ĵ�������
void creat(ALgraph *g)
{
	int i,j,k;
	edgenode *p,*q;
	
    printf(">>>��ʼ��������\n");
	for(i=0;i<g->vertexnum;i++)
	{
		g->adjlist[i].vertex=i;
		g->adjlist[i].firstedge=NULL;
	}

	printf(">>>��ʼ������\n");
	srand((unsigned)time(NULL));
	for(k=0;k<g->edgenum;k++)
	{
		i=rand()%(g->vertexnum);
		j=rand()%(g->vertexnum);
//		printf("��%d�����ɶ���%d������%d\n",k+1,i,j); 
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



//F2�����л�����������
	//F2-1����ʼ�� 
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
	
	//F2-2: �ж϶����Ƿ�Ϊ�� 
	int empty(Pqueue q)
	{
		if(q&&q->front==q->rear)
		return 1;
		else return 0;
	} 
	
	//F2-3: �����
	int in(Pqueue q,ALgraph *g,int v)
	{
		if((q->rear+1)%maxnum==q->front)
		{
			printf("����");
			return -1;
		}
		else
		{
			q->rear++;
			q->data[q->rear]=g->adjlist[v];
			return 1;
		}
	}
	
	//F2-4: ������
	int out(Pqueue q,vertexnode *x)
	{
		if(empty(q))
		{
			printf("�ӿ�\n");
			return -1;
		}
		else
		{
			q->front++;
			*x=q->data[q->front];
			return 1;
		}
	}
	 
//F3��visit()
void visit(ALgraph *g,int v)
{
	printf("[%d] ",g->adjlist[v].vertex);
}

 

//F4��BFS���� 
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


//debug:��ӡ���ɵ�ͼ
void print(ALgraph *g)
{
	int Enum=g->edgenum;
	int Vnum=g->vertexnum;
	printf("Enum=%d\n",g->edgenum);
	printf("Vnum=%d\n",g->vertexnum);
	for(int i=0;i<Vnum;i++)
	{
		printf("����<%d>ָ��:",i);
		edgenode *p=g->adjlist[i].firstedge;
		while(p)
		{
			printf("����[%d] ",p->adjvertex);
			p=p->next;
		}
		
	}
}

int main()
{
	ALgraph *g;
	ALgraph GRAPH;
	g=&GRAPH;
	
//	//��ģΪ10
//	printf("��ģΪ10\n"); 
//	g->vertexnum=5;
//    g->edgenum=10;
//    
//    //��ģΪ100 
//    printf("��ģΪ100\n");
//	g->vertexnum=70;
//    g->edgenum=100;
//    
//    //��ģΪ1000
//	printf("��ģΪ1000\n"); 
//	g->vertexnum=30;
//    g->edgenum=1000;
//    
//    //��ģΪ10000
//	printf("��ģΪ10000\n"); 
//	g->vertexnum=600;
//    g->edgenum=10000;
//    
    //��ģΪ100000
	printf("��ģΪ100000\n"); 
	g->vertexnum=80000;
    g->edgenum=100000;
    
    
	clock_t t1,t2;
	t1=clock();
	creat(g);
	t2=clock();
	cout<<">>>��ͼʱ��:"<<t2-t1<<"ms"<<endl;
    t1=clock();
	bfs(g,0);
	t2=clock();
	cout<<">>>BFSʱʱ��:"<<t2-t1<<"ms"<<endl;
	return 0;
}





