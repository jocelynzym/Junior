# lab2 红黑树&区间树(C实现)

> part 1 红黑树的插入删除操作
>
> part 2 区间树重叠区间查找 
>
> Part3 测试

## part 1 红黑树的插入删除操作

1.红黑树特性

- 满足二叉查找树特征：对于任意结点x，x.key>=x.left.key, x.key<=x.rightkey
- 根节点是黑色
- NIL结点为黑色
- 若一个结点为红，则它的子结点为黑
- 任意结点x的所有子孙路径，黑结点数目相同

2.红黑树基本定义

```c
#define RED   0
#define BLACK 1

typedef int Type;

typedef struct{
    unsigned char color;
    Type key;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
}RBNode,*RBTree;

typedef struct {
    RBNode *node;
}RBRoot;

```

3.对结点x左旋`leftRotate`

```c++
算法步骤：
(1)找到x的孩子:   y=x->right
(2)y的孩子过继:   x->right=y->left;  
                if(y->left!=NULL)  y->left->parent=x;   
(3)y的新父子关系： y->parent=x->parent; 
                 x->parent->child=y;
              【注意：若x.parent=NULL,则x为根，故此时将y设置为根】
(4)x和y的父子关系：y->left=x; x->parent=y;
```

```c++
void leftRotate(RBRoot *root, RBNode *x)
{
    RBNode *y=x->right;
    x->right=y->left;
    if(y->left!=NULL)
        y->left->parent=x;
    y->parent=x->parent;
    if(x->parent==NULL) root=y;
    else if(x->parent->left==x)  x->parent->left=y;
    else x->parent->right=y;
    x->parent=y;
    y->left=x;
         
}
```

3.对结点y右旋`rightRotate`

​      【将左旋中，x<->y，left<->right，即可】

```c++
void rightRotate(RBRoot *root, RBNode *y)
{
    RBNode *x=y->left;
    y->left=x->right;
    if(x->right!=NULL)
        x->right->parent=y;
    x->parent=y->parent;
    if(y->parent==NULL) root=x;
    else if(y->parent->right==y)  y->parent->right=x;
    else y->parent->left=x;
    y->parent=x;
    x->right=y;    
}
```

4.插入操作`insert`&&`insertFixUp`

```c++
(1)将z结点按BST树规则插入红黑树中，z是叶子结点(insert)
(2)将z涂红
(3)调整使其满足红黑树的性质(insertFixUp)
```

```c++
//插入算法insert
void insert(RBRoot *root,RBNode *node)
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
    
    if(y!=NULL)
       if(node->key<y->key) y->left=node;
       else y->right=node;
    else root->node=node;
    
    node->color=RED;
    insertFixUp(root,node);
}
```

```c
调整算法insertFixUp步骤：
(1)判断是否需要调整：若z为根，将其涂黑，结束；
                  若z不为根，且p[z]为黑，无需调整，结束；
                  若z不为根，且p[z]为红，则需调整
                  【此时隐含条件：p[z]不为根，p[p[z]]存在且为黑】。
(2)调整方法：
【case1~3 p[z]为左孩子】
case 1: z的叔叔为RED；
         step1:z的爸爸和叔叔变黑，z的祖父变红；
         step2:z上溯到祖父处，z=p[p[z]]
         step3:继续循环，最多上溯到根。若上溯到根，此时将根涂黑，终止
case 2: z的叔叔为BLACK且z为右孩子；
		 step1:z=p[z]
		 step2:左旋z，等价变换为case 3
case 3: z的叔叔为BLACK且z为左孩子；
		 step1:z的爸爸变黑，祖父变红
		 step2:右旋z的祖父
		 step3:调整终止
【case4~6 p[z]为右孩子】
case 4: z的叔叔为RED；
         step1:z的爸爸和叔叔变黑，z的祖父变红；
         step2:z上溯到祖父处，z=p[p[z]]
         step3:继续循环，最多上溯到根。若上溯到根，此时将根涂黑，终止
case 5: z的叔叔为BLACK且z为左孩子；
		 step1:z=p[z]
		 step2:右旋z，等价变换为case 6
case 6: z的叔叔为BLACK且z为右孩子；
		 step1:z的爸爸变黑，祖父变红
		 step2:左旋z的祖父
		 step3:调整终止
```

```c
//调整算法insertFixUp
void insertFixUp(RBRoot *root,RBNode *node)
{
	RBNode *parent,*gparent;
	//父结点存在且为红色 
	while((parent=node->parent)&&(node->parent->color==RED)){
		gparent=parent->parent;
		//case1~3 p[z]为左孩子 
		if(parent==gparent->left){
			//case1:z的叔叔为RED 
			if(gparent->right->color ==RED){
				parent->color=BLACK;
				gparent->right->color=BLACK;
				gparent->color=RED;
				node=gparent;
				continue;
			}			 
			//case2:z的叔叔为BLACK，且z为右孩子 
		    if((gparent->right->color==BLACK)&&(node==node->parent->right)){
				node=node->parent;
				leftRotate(root,node);
			}
			//case 3:z的叔叔y是黑色，且z为左孩子 
		    if((gparent->right->color==BLACK)&&(node==node->parent->left)){
				node->parent->color=BLACK;
				node->parent->parent->color=RED;
				rightRotate(root,node->parent->parent);
				break;
			}
		} 
		//case4~6：p[z]为右孩子 
		else{
			//case 4:z的叔叔为RED 
			if(gparent->left->color==RED){
				parent->color=BLACK;
				gparent->left->color=BLACK;
				gparent->color=RED;
				node=gparent;
				continue;
			}	
			//case 5:z的叔叔为BLACK，且z为左孩子 
			if((gparent->left->color==BLACK)&&(node==node->parent->left)){
				node=node->parent;
				rightRotate(root,node);
			}
			//case 6:z的叔叔y是黑色，且z为右孩子 
			if((gparent->left->color==BLACK)&&(node==node->parent->right)){
				node->parent->color=BLACK;
				node->parent->parent->color=RED;
				leftRotate(root,node->parent->parent);
				break;
			}
		}
	}//while
	root->node->color=BLACK;
} 
```

5.删除操作`RBdelete` && `deleteFixUp`

```c
删除操作说明: 
(1)按BST树规则删除结点RBdelete
(2)按红黑树性质调节删除结点后的树deleteFixUp
```

```c
delete算法步骤
(1)对待删除结点z分类，rep为真正删除的结点，x为rep的唯一孩子
	无孩子：rep=z,x=NULL
	仅一子：rep=z,x=rep.child
	两子：rep=z的中序后继，x=rep.child
(2)将x与rep.parent相连
(3)若z有两子，则将rep的所有信息复制到z结点(父子指针信息除外)，删除rep。若z无孩子或仅一子，直接删除rep
```

```c
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
	
	if(rep->color==BLACK) deleteFixUp(root,x);
	
	free(rep);
	
	//清除NIL结点
	 RBSweepNil(root->node);
}

```

```c
deleteFixUp算法步骤
x为删去结点的唯一孩子或为nil
(1)若x是根，直接移去多余一层黑色(树黑高减1)，终止； 
(2)若x原为红，将y的黑色涂到x上，终止
(3)若x非根节点，且为黑色，则x为双黑。通过变色、旋转 使多余黑色向上传播，直到某个红色节点或传到根，此时分8种情况调整
【case1~4：x为P[X]左子，case5~8：x为p[x]右子】
case 1:x的兄弟w为RED (转化为case2，3，4处理)
		step1:w变黑，p[x]变红
		step2:p[x]左旋，w指向x的新兄弟
case 2:x的兄弟为w为BLACK，且w的两个孩子都为BLACK
		step1:w变红，w的黑色涂到p[x]上
		step2:x上溯到p[x] (此时新x颜色可能为BLACK或dBLACK)
        step3:对新x继续迭代
case 3:x的兄弟w为BLACK，且w左子为RED，右子为BLACK(转化为case4)
		step1:w左子变黑，w变红
		step2:w右旋，w指向x的新兄弟
case 4:x的兄弟w为BLACK，且w右子为RED
		step1:p[x]的颜色c涂到w上
		step2:p[x]涂黑，w的右子变黑 P[x]左旋
        step3:终结处理，令x为root
case 5~ case 8 与 case 1~ case 4对称，将所有left换成right即可
```

```c
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
```

## part 2 区间树重叠区间查找

区间树在红黑树基础上扩展而来，在PART1的基础上，扩展的方法及代码如下

1.对红黑树结点的扩展

```c
在结构体中增添属性x->high,x->low,表示区间属性[low,high]
添加max值，x->max表示以x为根的的子树中所有区间端点的最大值
```

```c
//原红黑树结点
typedef struct RBNode{
    int            color;
    Type           key;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
}RBNode,*RBTree;

//现区间树结点
typedef struct RBNode{
    int            low,high;
    int			   max;
    int            color;
    Type           key;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
}RBNode,*RBTree;

```

2.对红黑树旋转的修改

```c
左旋leftRotate(T,x)与右旋rightRotate(T,y)时，仅需修改x与y的max值，修改方法为x.max=max(x.high,x.left.max,x.right.max)，边界情况的处理是若x.left不存在，则x.left.max=-10000
```

```c
//修改后的左旋,在原leftRotate末端添加  
    //区间树扩充(先x后y) 
	int x_left_max,x_right_max,y_left_max,y_right_max;
	if(x->left) x_left_max=x->left->max; else x_left_max=GUARD;
	if(x->right) x_right_max=x->right->max; else x_right_max=GUARD;
	x->max=max_3(x->high,x_left_max,x_right_max);

	if(y->left) y_left_max=y->left->max; else y_left_max=GUARD;
	if(y->right) y_right_max=y->right->max; else y_right_max=GUARD;
	y->max=max_3(y->high,y_left_max,y_right_max);        
}

//修改后的右旋,在原rightRotate末端添加  
    //区间树扩充(先y后x)
	int x_left_max,x_right_max,y_left_max,y_right_max;
	if(y->left) y_left_max=y->left->max; else y_left_max=GUARD;
	if(y->right) y_right_max=y->right->max; else y_right_max=GUARD;
	y->max=max_3(y->high,y_left_max,y_right_max);
	
	if(x->left) x_left_max=x->left->max; else x_left_max=GUARD;
	if(x->right) x_right_max=x->right->max; else x_right_max=GUARD;
	x->max=max_3(x->high,x_left_max,x_right_max);
```

3.对红黑树插入的修改

```c
无需修改insertFixUp,仅需在RBinsert中修改。
插入结点在insertFixUp前，已位于树底，只需在当前插入节点到root这条路径上，自下而上修改max属性值即可，修改规则为x.max=max(x.high,x.left.max,x.right.max)
```

```c
//区间树操作intervalInsert添加部分,添加在fixup前 
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
```

4.对红黑树删除的修改

```c
无需修改deleteFixUp,仅需在RBdelete中修改。
删除结点在deleteFixUp前，已位于树底，只需在当前删除节点到root这条路径上，自下而上修改max属性值即可，修改规则为x.max=max(x.high,x.left.max,x.right.max)
```

```c
//区间树操作intervalDelete添加部分，添加在fixup前  
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
```

5.新操作设计intervalSearch

```c
用于找出树中覆盖区间i的那个结点，没有则返回NULL
```

```c
//判断两个区间是否重叠
int overlapJudge(int low1,int high1,int low2,int high2)
{
	if((low1<=high2)&&(low2<high1)) return 1;
	else return 0;
}

//重叠区间的查找
RBNode *intervalSearch(RBRoot *root,int low,int high)
{
	RBNode *x=root->node;
	while(x&&(!overlapJudge(low,high,x->low,x->high))){
		if(x->left&&x->left->max>=low) x=x->left;
		else x=x->right;
	}
	return x;
}
```

## PART 3 测试

1.测试insert功能

- 输入：10个区间

  ```c
  Type a_low[10]={0,5,6,8,15,16,17,19,25,26};
  Type a_high[10]={3,8,10,9,23,21,19,20,30,26};
  ```

- 输出：打印建立的红黑树，及其每个结点的相关信息

  ![insert_test](https://github.com/jocelynzym/Algorithm/blob/master/pic/pic1.jpg)

2.测试intervalSearch功能

- 输入：查找区间[22,31]

- 输出:   找到结点[15,23],返回结点[15,23]的相关信息


3.测试delete功能

- 输入：删除结点[5,8]

- 输出：打印现存红黑树，及其每个结点相关信息

  ![delete_test](https://github.com/jocelynzym/Algorithm/blob/master/pic/pic2.jpg)

