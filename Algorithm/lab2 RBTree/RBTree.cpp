#include<stdio.h>
#include<stdlib.h>
#define RED   0
#define BLACK 1
#define NIL   10000
#define GUARD -10000

typedef int Type;

//结点 
typedef struct RBNode{
	int            low,high;
	int            max;
    int            color;
    Type           key;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
}RBNode,*RBTree;


//根 
typedef struct {
    RBNode *node;
}RBRoot;

//比较a,b,c大小 
int max_3(int a,int b,int c){
	int max;
	max=(a>b)?a:b;
	max=(max>c)?max:c;
	return max;
}

//判断是否overlap 
int overlapJudge(int low1,int high1,int low2,int high2)
{
	if((low1<=high2)&&(low2<high1)) return 1;
	else return 0;
}


//左旋 
void leftRotate(RBRoot *root, RBNode *x)
{
    RBNode *y=x->right;
    x->right=y->left;
    if(y->left!=NULL)
        y->left->parent=x;
    y->parent=x->parent;
    if(x->parent==NULL) root->node=y;
    else if(x->parent->left==x)  x->parent->left=y;
    else x->parent->right=y;
    x->parent=y;
    y->left=x;
    
    //区间树扩充(先x后y) 
	int x_left_max,x_right_max,y_left_max,y_right_max;
	if(x->left) x_left_max=x->left->max; else x_left_max=GUARD;
	if(x->right) x_right_max=x->right->max; else x_right_max=GUARD;
	x->max=max_3(x->high,x_left_max,x_right_max);

	if(y->left) y_left_max=y->left->max; else y_left_max=GUARD;
	if(y->right) y_right_max=y->right->max; else y_right_max=GUARD;
	y->max=max_3(y->high,y_left_max,y_right_max);
         
}

//右旋 
void rightRotate(RBRoot *root, RBNode *y)
{
    RBNode *x=y->left;
    y->left=x->right;
    if(x->right!=NULL)
        x->right->parent=y;
    x->parent=y->parent;
    if(y->parent==NULL) root->node=x;
    else if(y->parent->right==y)  y->parent->right=x;
    else y->parent->left=x;
    y->parent=x;
    x->right=y;    
    
    //区间树扩充(先y后x) 
	int x_left_max,x_right_max,y_left_max,y_right_max;
	if(y->left) y_left_max=y->left->max; else y_left_max=GUARD;
	if(y->right) y_right_max=y->right->max; else y_right_max=GUARD;
	y->max=max_3(y->high,y_left_max,y_right_max);
	
	if(x->left) x_left_max=x->left->max; else x_left_max=GUARD;
	if(x->right) x_right_max=x->right->max; else x_right_max=GUARD;
	x->max=max_3(x->high,x_left_max,x_right_max);
	
}



//插入调整
void insertFixUp(RBRoot *root,RBNode *node)
{
	RBNode *parent,*gparent;
	//父结点存在且为红色 
	while((parent=node->parent)&&(node->parent->color==RED)){
		gparent=parent->parent;
		//case1~3 p[z]为左孩子 
		if(parent==gparent->left){
			//case1:z的叔叔为RED 
			if(gparent->right&&(gparent->right->color==RED))
			{
				parent->color=BLACK;
				gparent->right->color=BLACK;
				gparent->color=RED;
				node=gparent;
				continue;
			}			 
			//case2:z的叔叔为BLACK，且z为右孩子 
		    if((!gparent->right||(gparent->right->color==BLACK))&&(node==node->parent->right)){
				node=node->parent;
				leftRotate(root,node);
			}
			//case 3:z的叔叔y是黑色，且z为左孩子 
		    if((!gparent->right||(gparent->right->color==BLACK))&&(node==node->parent->left)){
				node->parent->color=BLACK;
				node->parent->parent->color=RED;
				rightRotate(root,node->parent->parent);
			}
		} 
		//case4~6：p[z]为右孩子 
		else{
			//case 4:z的叔叔为RED 
			if(gparent->left&&gparent->left->color==RED){
				parent->color=BLACK;
				gparent->left->color=BLACK;
				gparent->color=RED;
				node=gparent;
				continue;
			}	
			//case 5:z的叔叔为BLACK，且z为左孩子 
			if((!gparent->left||(gparent->left->color==BLACK))&&(node==node->parent->left)){
				node=node->parent;
				rightRotate(root,node);
			}
			//case 6:z的叔叔y是黑色，且z为右孩子 
			if((!gparent->left||(gparent->left->color==BLACK))&&(node==node->parent->right)){
				node->parent->color=BLACK;
				node->parent->parent->color=RED;
				leftRotate(root,node->parent->parent);
			}
		}
	}//while
	root->node->color=BLACK;
} 



//BST树插入
void RBinsert(RBRoot *root,RBNode *node)
{
    RBNode *y=NULL;
    RBNode *x=root->node;
    
    while(x!=NULL)
    {
        y = x;
        if (node->key < x->key)  x = x->left;
        else x = x->right;
    }
    
    node->parent=y;
    
    if(y!=NULL){
    	if(node->key<y->key) y->left=node;
        else y->right=node;
	}
	     
    else root->node=node;
    node->color=RED;
    
    
    //区间树操作intervalInsert添加部分 
    //begin： 
    int x_left_max,x_right_max;
    x=node;
	while(x){
		if(x->left) x_left_max=x->left->max; else x_left_max=GUARD;
		if(x->right) x_right_max=x->right->max; else x_right_max=GUARD;
		x->max=max_3(x->high,x_left_max,x_right_max);
		x=x->parent;
	}
	//end 
	
    insertFixUp(root,node);
    
    
 




  	  
//删除调整 
void deleteFixUp(RBRoot *root,RBNode *x)
{   
     RBNode *w; //x的兄弟 
    //x非根且为黑色 
	while((x!=root->node)&&(x->color==BLACK||!x)){
		//case1~4: x为左子
		if(x->parent->left==x){
			w=x->parent->right;
			//case1:x的兄弟w为RED
			if(w->color==RED)
			{
				w->color=BLACK;
				x->parent->color=RED;
				leftRotate(root,x->parent);
				w=x->parent->right;
			}
			//case 2:x的兄弟为w为BLACK，且w的两个孩子都为BLACK
			if((!w->left||w->left->color==BLACK)&&(!w->right||w->right->color==BLACK))
			{
				w->color=RED;
				x=x->parent;
			}
			//case3,case4
			else{
				//case3:x的兄弟w为BLACK，且w左子为RED，右子为BLACK(转化为case4)
				//case3转化为case4 
				if(!w->right||w->right->color==BLACK)
				{
					w->left->color=BLACK;
					w->color= RED;
					rightRotate(root,w);
					w=x->parent->right;
				}
				//case 4:x的兄弟w为BLACK，且w右子为RED
					w->color=x->parent->color;
					x->parent->color=BLACK;
					w->right->color=BLACK;
					leftRotate(root,x->parent);
					x=root->node;
			}//case3,case4		
		}//case1~4 
		
		//case5~8:x为右子
		else{
			w=x->parent->left;
			//case 5:x的兄弟w为RED
			if(w->color==RED)
			{
				w->color=BLACK;
				x->parent->color=RED;
				rightRotate(root,x->parent);
				w=x->parent->left;
			}
			//case 6:x的兄弟为w为BLACK，且w的两个孩子都为BLACK
			if((!w->left||w->left->color==BLACK)&&(!w->right||w->right->color==BLACK))
			{
				w->color=RED;
				x=x->parent;
			}
			//case7,case8
			else{
				//case7:x的兄弟w为BLACK，且w右子为RED，左子为BLACK
				//case7转化为case8
				if(!w->left||w->left->color==BLACK)
				{
					w->right->color=BLACK;
					w->color= RED;
					leftRotate(root,w);
					w=x->parent->left;
				}
				//case 8:x的兄弟w为BLACK，且w左子为RED
					w->color=x->parent->color;
					x->parent->color=BLACK;
					w->left->color=BLACK;
					rightRotate(root,x->parent);
					x=root->node;
			}//case7,case8	
		}//case5~8
		
	}//while
	
	x->color=BLACK;
}


//清除NIL结点 
void RBSweepNil(RBNode *cur){
	if(!cur) return;
	RBSweepNil(cur->left);
	if(cur->key==NIL){
		if(cur->parent->left==cur) cur->parent->left=NULL;
		else  cur->parent->right=NULL;
	}
	RBSweepNil(cur->right);
}


//BST删除 
void RBdelete(RBRoot *root,RBNode *node)
  {
  	RBNode *rep; //要删除的结点 
  	RBNode *x; //rep的待连接子结点 
  	 
  	//step1: 判定要删除的结点rep 
  		//case1 & case2: 左右孩子至少一个为空,rep=node
	if(node->left==NULL||node->right==NULL) rep=node;	
  		//case3:左右孩子都不为空,rep=node的中序后继 
  	else{
  		rep=node->right;
  		while(rep->left!=NULL) rep=rep->left;
	  } 
	    
	//step2: 为了连接rep的孩子x到rep的父亲,先确定x 
	if(rep->left) x=rep->left;
	else x=rep->right;	
		

  	//step3:连接rep的孩子x到rep的父亲
  		//如果x为空，将其改造成一个NIL结点
	if(!x){
		x=new RBNode[1];
		x->color=BLACK;
		x->key=NIL;
		x->left=NULL;
		x->right=NULL;
		x->parent=rep->parent; 
	} 
  		//设置x的新父亲 
    if(x) x->parent=rep->parent;
	    //设置p[rep]的新孩子
	if(rep->parent==NULL) root->node=x;
	else if(rep->parent->left==rep)  rep->parent->left=x;
	else rep->parent->right=x;
	

	//step4:删除结点
	    //case3：删除前要将rep内容copy到node
	if(rep!=node){
		node->low=rep->low;
		node->high=rep->high;
		node->max=rep->max;
		node->color=rep->color;
		node->key=rep->key;
	}
		//case1,case2:直接删除即可 
	
	
	//区间树操作intervalDelete添加部分 
	//自x向上调整max值 
    //begin： 
    int temp_left_max,temp_right_max;
    RBNode *temp;
    temp=x;
	while(temp){
		if(temp->left) temp_left_max=temp->left->max; else temp_left_max=GUARD;
		if(temp->right) temp_right_max=temp->right->max; else temp_right_max=GUARD;
		temp->max=max_3(temp->high,temp_left_max,temp_right_max);
		temp=temp->parent;
	}
	//end 
	
	
	if(rep->color==BLACK) deleteFixUp(root,x);
	
	free(rep);
	
	//清除NIL结点
	 RBSweepNil(root->node);
}


//区间树查找 
RBNode *intervalSearch(RBRoot *root,int low,int high)
{
	RBNode *x=root->node;
	while(x&&(!overlapJudge(low,high,x->low,x->high))){
		if(x->left&&x->left->max>=low) x=x->left;
		else x=x->right;
	}
	return x;
}


RBRoot *testCreate()
{
	Type a_low[10]={0,5,6,8,15,16,17,19,25,26};
	Type a_high[10]={3,8,10,9,23,21,19,20,30,26};

	RBNode *iter;
	RBRoot *root=new RBRoot[1];	
	root->node=NULL;
	
	for(int i=0;i<10;i++){
		RBNode *node=new RBNode[1];
		node->low=a_low[i];
		node->high=a_high[i];
		node->max=node->high;
		node->color=RED;
		node->key=node->low;
		node->left=NULL;
		node->parent=NULL;
		node->right=NULL;

		RBinsert(root,node);	
	}
	
	return root;
} 

//打印结点 
void showRBNode(RBNode *iter)
{
	if(!iter){printf("要查看的结点不存在"); return ;}

	printf("结点：\n");
	if(iter->color==0) printf(" p.color=RED\n");
	else printf(" p.color=BLACK\n");
	
    printf("p.区间=[%d,%d]\n",iter->low,iter->high); 
    printf("p.max=%d\n",iter->max);
    printf("p.key=%d\n",iter->key); 
    
    if(iter->left) printf("p.left.key=%d\n",iter->left->key); 
    else printf("p.left=NULL\n");
    
    if(iter->right) printf("p.right.key=%d\n",iter->right->key); 
    else printf("p.right=NULL\n");
    
    if(iter->parent) printf("p.parent.key=%d\n",iter->parent->key); 
    else printf("p.parent=NULL\n");
    
    printf("-----------------------------------------------\n");
}

//前序遍历 
void PreOrderTravel(RBNode *T)
{
	if(!T) return;
	showRBNode(T);
    PreOrderTravel(T->left);
    PreOrderTravel(T->right);
}




//中序遍历 
void InOrderTravel(RBNode *T)
{
	if(!T) return;
    InOrderTravel(T->left);
    showRBNode(T);
    InOrderTravel(T->right);
}

//打印红黑树 
void showRBTree(RBNode *T)
{
	printf("中序遍历：\n"); 
	InOrderTravel(T);
	printf("前序遍历：\n"); 
	PreOrderTravel(T);
}




int main(){
	RBRoot *root;
	//测试RBinsert 
	//说明：给指定序列构造一棵红黑树，并打印 
	printf("插入测试：\n"); 
	root=testCreate();
	showRBTree(root->node);
	
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
    printf("-----------------------------------------------\n");
  
	//测试intervalSearch
	//说明： 给定区间[22,31]，返回一个与其重叠区间，打印该结点的信息
	printf("intervalSearch test：\n"); 
	RBNode *IntervalTest;
	IntervalTest=intervalSearch(root,22,31);
	showRBNode(IntervalTest);
	
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
    printf("-----------------------------------------------\n");
  
	
	//测试delete
	//说明：删除结点[5,8]，打印红黑树
	 printf("RBdelete test：\n"); 
	 RBdelete(root,root->node->left->right);
	 showRBTree(root->node);
     showRBNode(root->node->right->right->right); 
    
    
	printf("finished!\n");
	return 0;
}







