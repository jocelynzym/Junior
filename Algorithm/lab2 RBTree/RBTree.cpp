#include<stdio.h>
#include<stdlib.h>
#define RED   0
#define BLACK 1
#define NIL   10000
#define GUARD -10000

typedef int Type;

//��� 
typedef struct RBNode{
	int            low,high;
	int            max;
    int            color;
    Type           key;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
}RBNode,*RBTree;


//�� 
typedef struct {
    RBNode *node;
}RBRoot;

//�Ƚ�a,b,c��С 
int max_3(int a,int b,int c){
	int max;
	max=(a>b)?a:b;
	max=(max>c)?max:c;
	return max;
}

//�ж��Ƿ�overlap 
int overlapJudge(int low1,int high1,int low2,int high2)
{
	if((low1<=high2)&&(low2<high1)) return 1;
	else return 0;
}


//���� 
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
    
    //����������(��x��y) 
	int x_left_max,x_right_max,y_left_max,y_right_max;
	if(x->left) x_left_max=x->left->max; else x_left_max=GUARD;
	if(x->right) x_right_max=x->right->max; else x_right_max=GUARD;
	x->max=max_3(x->high,x_left_max,x_right_max);

	if(y->left) y_left_max=y->left->max; else y_left_max=GUARD;
	if(y->right) y_right_max=y->right->max; else y_right_max=GUARD;
	y->max=max_3(y->high,y_left_max,y_right_max);
         
}

//���� 
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
    
    //����������(��y��x) 
	int x_left_max,x_right_max,y_left_max,y_right_max;
	if(y->left) y_left_max=y->left->max; else y_left_max=GUARD;
	if(y->right) y_right_max=y->right->max; else y_right_max=GUARD;
	y->max=max_3(y->high,y_left_max,y_right_max);
	
	if(x->left) x_left_max=x->left->max; else x_left_max=GUARD;
	if(x->right) x_right_max=x->right->max; else x_right_max=GUARD;
	x->max=max_3(x->high,x_left_max,x_right_max);
	
}



//�������
void insertFixUp(RBRoot *root,RBNode *node)
{
	RBNode *parent,*gparent;
	//����������Ϊ��ɫ 
	while((parent=node->parent)&&(node->parent->color==RED)){
		gparent=parent->parent;
		//case1~3 p[z]Ϊ���� 
		if(parent==gparent->left){
			//case1:z������ΪRED 
			if(gparent->right&&(gparent->right->color==RED))
			{
				parent->color=BLACK;
				gparent->right->color=BLACK;
				gparent->color=RED;
				node=gparent;
				continue;
			}			 
			//case2:z������ΪBLACK����zΪ�Һ��� 
		    if((!gparent->right||(gparent->right->color==BLACK))&&(node==node->parent->right)){
				node=node->parent;
				leftRotate(root,node);
			}
			//case 3:z������y�Ǻ�ɫ����zΪ���� 
		    if((!gparent->right||(gparent->right->color==BLACK))&&(node==node->parent->left)){
				node->parent->color=BLACK;
				node->parent->parent->color=RED;
				rightRotate(root,node->parent->parent);
			}
		} 
		//case4~6��p[z]Ϊ�Һ��� 
		else{
			//case 4:z������ΪRED 
			if(gparent->left&&gparent->left->color==RED){
				parent->color=BLACK;
				gparent->left->color=BLACK;
				gparent->color=RED;
				node=gparent;
				continue;
			}	
			//case 5:z������ΪBLACK����zΪ���� 
			if((!gparent->left||(gparent->left->color==BLACK))&&(node==node->parent->left)){
				node=node->parent;
				rightRotate(root,node);
			}
			//case 6:z������y�Ǻ�ɫ����zΪ�Һ��� 
			if((!gparent->left||(gparent->left->color==BLACK))&&(node==node->parent->right)){
				node->parent->color=BLACK;
				node->parent->parent->color=RED;
				leftRotate(root,node->parent->parent);
			}
		}
	}//while
	root->node->color=BLACK;
} 



//BST������
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
    
    
    //����������intervalInsert��Ӳ��� 
    //begin�� 
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
    
    
 




  	  
//ɾ������ 
void deleteFixUp(RBRoot *root,RBNode *x)
{   
     RBNode *w; //x���ֵ� 
    //x�Ǹ���Ϊ��ɫ 
	while((x!=root->node)&&(x->color==BLACK||!x)){
		//case1~4: xΪ����
		if(x->parent->left==x){
			w=x->parent->right;
			//case1:x���ֵ�wΪRED
			if(w->color==RED)
			{
				w->color=BLACK;
				x->parent->color=RED;
				leftRotate(root,x->parent);
				w=x->parent->right;
			}
			//case 2:x���ֵ�ΪwΪBLACK����w���������Ӷ�ΪBLACK
			if((!w->left||w->left->color==BLACK)&&(!w->right||w->right->color==BLACK))
			{
				w->color=RED;
				x=x->parent;
			}
			//case3,case4
			else{
				//case3:x���ֵ�wΪBLACK����w����ΪRED������ΪBLACK(ת��Ϊcase4)
				//case3ת��Ϊcase4 
				if(!w->right||w->right->color==BLACK)
				{
					w->left->color=BLACK;
					w->color= RED;
					rightRotate(root,w);
					w=x->parent->right;
				}
				//case 4:x���ֵ�wΪBLACK����w����ΪRED
					w->color=x->parent->color;
					x->parent->color=BLACK;
					w->right->color=BLACK;
					leftRotate(root,x->parent);
					x=root->node;
			}//case3,case4		
		}//case1~4 
		
		//case5~8:xΪ����
		else{
			w=x->parent->left;
			//case 5:x���ֵ�wΪRED
			if(w->color==RED)
			{
				w->color=BLACK;
				x->parent->color=RED;
				rightRotate(root,x->parent);
				w=x->parent->left;
			}
			//case 6:x���ֵ�ΪwΪBLACK����w���������Ӷ�ΪBLACK
			if((!w->left||w->left->color==BLACK)&&(!w->right||w->right->color==BLACK))
			{
				w->color=RED;
				x=x->parent;
			}
			//case7,case8
			else{
				//case7:x���ֵ�wΪBLACK����w����ΪRED������ΪBLACK
				//case7ת��Ϊcase8
				if(!w->left||w->left->color==BLACK)
				{
					w->right->color=BLACK;
					w->color= RED;
					leftRotate(root,w);
					w=x->parent->left;
				}
				//case 8:x���ֵ�wΪBLACK����w����ΪRED
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


//���NIL��� 
void RBSweepNil(RBNode *cur){
	if(!cur) return;
	RBSweepNil(cur->left);
	if(cur->key==NIL){
		if(cur->parent->left==cur) cur->parent->left=NULL;
		else  cur->parent->right=NULL;
	}
	RBSweepNil(cur->right);
}


//BSTɾ�� 
void RBdelete(RBRoot *root,RBNode *node)
  {
  	RBNode *rep; //Ҫɾ���Ľ�� 
  	RBNode *x; //rep�Ĵ������ӽ�� 
  	 
  	//step1: �ж�Ҫɾ���Ľ��rep 
  		//case1 & case2: ���Һ�������һ��Ϊ��,rep=node
	if(node->left==NULL||node->right==NULL) rep=node;	
  		//case3:���Һ��Ӷ���Ϊ��,rep=node�������� 
  	else{
  		rep=node->right;
  		while(rep->left!=NULL) rep=rep->left;
	  } 
	    
	//step2: Ϊ������rep�ĺ���x��rep�ĸ���,��ȷ��x 
	if(rep->left) x=rep->left;
	else x=rep->right;	
		

  	//step3:����rep�ĺ���x��rep�ĸ���
  		//���xΪ�գ���������һ��NIL���
	if(!x){
		x=new RBNode[1];
		x->color=BLACK;
		x->key=NIL;
		x->left=NULL;
		x->right=NULL;
		x->parent=rep->parent; 
	} 
  		//����x���¸��� 
    if(x) x->parent=rep->parent;
	    //����p[rep]���º���
	if(rep->parent==NULL) root->node=x;
	else if(rep->parent->left==rep)  rep->parent->left=x;
	else rep->parent->right=x;
	

	//step4:ɾ�����
	    //case3��ɾ��ǰҪ��rep����copy��node
	if(rep!=node){
		node->low=rep->low;
		node->high=rep->high;
		node->max=rep->max;
		node->color=rep->color;
		node->key=rep->key;
	}
		//case1,case2:ֱ��ɾ������ 
	
	
	//����������intervalDelete��Ӳ��� 
	//��x���ϵ���maxֵ 
    //begin�� 
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
	
	//���NIL���
	 RBSweepNil(root->node);
}


//���������� 
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

//��ӡ��� 
void showRBNode(RBNode *iter)
{
	if(!iter){printf("Ҫ�鿴�Ľ�㲻����"); return ;}

	printf("��㣺\n");
	if(iter->color==0) printf(" p.color=RED\n");
	else printf(" p.color=BLACK\n");
	
    printf("p.����=[%d,%d]\n",iter->low,iter->high); 
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

//ǰ����� 
void PreOrderTravel(RBNode *T)
{
	if(!T) return;
	showRBNode(T);
    PreOrderTravel(T->left);
    PreOrderTravel(T->right);
}




//������� 
void InOrderTravel(RBNode *T)
{
	if(!T) return;
    InOrderTravel(T->left);
    showRBNode(T);
    InOrderTravel(T->right);
}

//��ӡ����� 
void showRBTree(RBNode *T)
{
	printf("���������\n"); 
	InOrderTravel(T);
	printf("ǰ�������\n"); 
	PreOrderTravel(T);
}




int main(){
	RBRoot *root;
	//����RBinsert 
	//˵������ָ�����й���һ�ú����������ӡ 
	printf("������ԣ�\n"); 
	root=testCreate();
	showRBTree(root->node);
	
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
    printf("-----------------------------------------------\n");
  
	//����intervalSearch
	//˵���� ��������[22,31]������һ�������ص����䣬��ӡ�ý�����Ϣ
	printf("intervalSearch test��\n"); 
	RBNode *IntervalTest;
	IntervalTest=intervalSearch(root,22,31);
	showRBNode(IntervalTest);
	
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
	printf("-----------------------------------------------\n");
    printf("-----------------------------------------------\n");
  
	
	//����delete
	//˵����ɾ�����[5,8]����ӡ�����
	 printf("RBdelete test��\n"); 
	 RBdelete(root,root->node->left->right);
	 showRBTree(root->node);
     showRBNode(root->node->right->right->right); 
    
    
	printf("finished!\n");
	return 0;
}







