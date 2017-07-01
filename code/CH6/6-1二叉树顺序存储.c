 //define CHAR 1 //字符型
 #define CHAR 0 //整型(二者选一) 
 #include<limits.h> /* INT_MAX等 */
 #include<stdio.h> /* EOF(=^Z或F6),NULL */
 #include<stdlib.h> /* atoi() */
 #include<io.h> /* eof() */
 #include<math.h> /* floor(),ceil(),abs() */
 #include<process.h> /* exit() */
 /* 函数结果状态代码 */
 #define TRUE 1
 #define FALSE 0
 #define OK 1
 #define ERROR 0
 #define INFEASIBLE -1
 /* #define OVERFLOW -2 因为在math.h中已定义OVERFLOW的值为3,故去掉此行 */
 typedef int Status; /* Status是函数的类型,其值是函数结果状态代码，如OK等 */
 typedef int Boolean; /* Boolean是布尔类型,其值是TRUE或FALSE */
 
 #if CHAR 
   typedef char TElemType;
   TElemType Nil = ' ';//设字符型以空格符为空
   #define form "%c"
 #else
   typedef int TElemType;
   TElemType Nil = 0;//设整型以0为空
   #define form "%d"
 #endif 
 
 #define MAX_TREE_SIZE 100 //二叉树的最大结点数
 typedef TElemType SqBiTree[MAX_TREE_SIZE];//0号单元存储根节点
 
 typedef struct
 {
 	int level, order;//结点的层，本层序号(按满二叉树计算) 
 }position; 
 
 #define ClearBiTree InitBiTree//在顺序存储结构中，两函数完全一样
 #define DestroyBiTree InitBiTree//在顺序存储结构中，两函数完全一样
 
 typedef int QElemType;//设队列元素类型为整型(序号) 
 typedef struct QNode
 {
 	QElemType data;
 	struct QNode *next;
 }QNode, * QueuePtr;
 
 typedef struct 
 {
 	QueuePtr front, rear;//队头，队尾指针 
 }LinkQueue; 
 
void InitBiTree(SqBiTree T);
void CreateBiTree(SqBiTree T);
Status BiTreeEmpty(SqBiTree T);
int BiTreeDepth(SqBiTree T);
Status Root(SqBiTree T, TElemType *e);
TElemType Value(SqBiTree T, position e);
Status Assign(SqBiTree T, position e, TElemType value);
TElemType Parent(SqBiTree T, TElemType e);
TElemType LeftChild(SqBiTree T, TElemType e);
TElemType RightChild(SqBiTree T, TElemType e);
TElemType LeftSibling(SqBiTree T, TElemType e);
TElemType RightSibling(SqBiTree T, TElemType e);
void Move(SqBiTree q, int j, SqBiTree T, int i);//InsertChild()用到。加
void InsertChild(SqBiTree T, TElemType p, int LR, SqBiTree c);
void InitQueue(LinkQueue *Q);
void DestroyQueue(LinkQueue *Q);
void ClearQueue(LinkQueue *Q);
Status QueueEmpty(LinkQueue Q);
int QueueLength(LinkQueue Q);
Status GetHead_Q(LinkQueue Q, QElemType *e);//避免重名
void EnQueue(LinkQueue *Q, QElemType e);
Status DeQueue(LinkQueue *Q, QElemType *e);
void QueueTraverse(LinkQueue Q, void(*vi)(QElemType));
void print(QElemType i);
Status DeleteChild(SqBiTree T, position p, int LR);
void(*VisitFunc)(TElemType);//函数变量
void PreTraverse(SqBiTree T, int e);
void PreOrderTraverse(SqBiTree T, void(*Visit)(TElemType));
void InTraverse(SqBiTree T, int e);
void InOrderTraverse(SqBiTree T, void(*Visit)(TElemType));
void PostTraverse(SqBiTree T, int e);
void PostOrderTraverse(SqBiTree T, void(*Visit)(TElemType));
void LevelOrderTraverse(SqBiTree T, void(*Visit)(TElemType));
void Print(SqBiTree T);
void visit(TElemType e);

int main()
{
   Status i;
   int j;
   position p;
   TElemType e;
   SqBiTree T,s;
   InitBiTree(T);
   CreateBiTree(T);
   printf("建立二叉树后,树空否？%d(1:是 0:否) 树的深度=%d\n",BiTreeEmpty(T),BiTreeDepth(T));
   i=Root(T,&e);
   if(i)
     printf("二叉树的根为："form"\n",e);
   else
     printf("树空，无根\n");
   printf("层序遍历二叉树:\n");
   LevelOrderTraverse(T,visit);
   printf("中序遍历二叉树:\n");
   InOrderTraverse(T,visit);
   printf("后序遍历二叉树:\n");
   PostOrderTraverse(T,visit);
   printf("请输入待修改结点的层号 本层序号: ");
   scanf("%d%d",&p.level,&p.order);
   e=Value(T,p);
   printf("待修改结点的原值为"form"请输入新值: ",e);
   scanf("%*c"form"%*c",&e);
   Assign(T,p,e);
   printf("先序遍历二叉树:\n");
   PreOrderTraverse(T,visit);
   printf("结点"form"的双亲为"form",左右孩子分别为",e,Parent(T,e));
   printf(form","form",左右兄弟分别为",LeftChild(T,e),RightChild(T,e));
   printf(form","form"\n",LeftSibling(T,e),RightSibling(T,e));
   InitBiTree(s);
   printf("建立右子树为空的树s:\n");
   CreateBiTree(s);
   printf("树s插到树T中,请输入树T中树s的双亲结点 s为左(0)或右(1)子树: ");
   scanf(form"%d",&e,&j);
   InsertChild(T,e,j,s);
   Print(T);
   printf("删除子树,请输入待删除子树根结点的层号 本层序号 左(0)或右(1)子树: ");
   scanf("%d%d%d",&p.level,&p.order,&j);
   DeleteChild(T,p,j);
   Print(T);
   ClearBiTree(T);
   printf("清除二叉树后,树空否？%d(1:是 0:否) 树的深度=%d\n",BiTreeEmpty(T),BiTreeDepth(T));
   i=Root(T,&e);
   if(i)
     printf("二叉树的根为："form"\n",e);
   else
     printf("树空，无根\n");
	return 0;
}
 
 
 
 
 void InitBiTree(SqBiTree T)
 {
 	//构造空二叉树T。因为T是数组名，故不需要&
	int i;
	
	for (i=0; i<MAX_TREE_SIZE; i++)
	{
		T[i] = Nil;//初值为空(Nil在主程中定义) 
	} 
 } 
 
 void CreateBiTree(SqBiTree T)
 {
 	//按层次序输出二叉树中结点的值(字符型或整型)，构造顺序存储的二叉树T
	int i = 0;
   #if CHAR //结点类型为字符
    int l;
	char s[MAX_TREE_SIZE];
	
	InitBiTree(T);//构造空二叉树T
	printf("请按层序输入结点的值(字符),空格表示空结点，结点数<=%d:\n", MAX_TREE_SIZE); 
	gets(s);//输入字符串
    l = strlen(s);//求字符串的长度
    for (; i<l; i++)//将字符串赋值给T
	{
		T[i] = s[i]; 
	} 
   #else  //结点类型为整型
    InitBiTree(T);//构造空二叉树T
	printf("请按层序输入结点的值(整型)，0表示空结点，输999结束。节点数≤%d:\n", MAX_TREE_SIZE);
	while (1)
	{
		scanf("%d", &T[i]);
		if (T[i] == 999)
		{
			T[i] = Nil;
			break;
		}
		i++;
	}
   #endif
     for (i=1; i<MAX_TREE_SIZE; i++)
     {
     	if (T[(i+1)/2-1]==Nil && T[i]!=Nil)//此非根结点(不空)无双亲
		{
			printf("出现无双亲的非根结点”form“\n", T[i]);
			exit(ERROR);
		} 	
     }
 } 
 
 Status BiTreeEmpty(SqBiTree T)
 {
 	//初始条件：二叉树T存在。操作结果：若T为空二叉树,否则返回TRUE,否则FALSE
	if (T[0] == Nil)//根节点为空，则数空
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
 }
 
 int BiTreeDepth(SqBiTree T)
 {
 	//初始条件：二叉树T存在。操作结果：返回T的深度
	int i, j = -1;
	
	for (i=MAX_TREE_SIZE-1; i>=0; i--)//找到最后一个结点
	{
		if (T[i] != Nil)
		{
			break;
		} 
	} 
	i++;//为了便于计算
	do
	{
		j++;
	}while (i>=pow(2, j)); 
	
	return j;
 }
 
 Status Root(SqBiTree T, TElemType *e)
 {
 	//初始条件：二叉树T存在。操作结果：当T不空，用e返回T的根，返回OK；否则返回ERROR，无定义
	if (BiTreeEmpty(T))//T空
	{
		return ERROR; 
	} 
	else
	{
		*e = T[0];
		return OK; 
	} 
 }
 
 
 TElemType Value(SqBiTree T, position e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点(的位置)
	//操作结果：返回处于位置e(层，本层序号)的结点的值
    return T[(int)pow(2, e.level-1) + e.order - 2]; 
 }
 
 Status Assign(SqBiTree T, position e, TElemType value)
 {
 	//初始条件：二叉树T存在，e是T中某个结点(的位置)
	//操作结果：给处于位置e(层，本层序号)的结点赋新值value
	int i = (int)pow(2, e.level-1)+e.order-2;//将层，本层序号转为矩阵的序号
	
	if (value!=Nil && T[(i+1)/2-1]==Nil)//给叶子赋非空值但双亲为空 
	{
		return ERROR;
	}
	else if (value==Nil && (T[i*2+1]!=Nil || T[i*2+2]!=Nil))//给双亲赋空值但有叶子(不空)
	{
		return ERROR; 
	} 
	T[i] = value;
	
	return OK;
 }
 
 TElemType Parent(SqBiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点
	//操作结果：若e是T的非根结点，则返回它的双亲，否则返回空
	int i; 
	
	if (T[0] == Nil)//空树
	{
		return Nil; 
	} 
	for (i=1; i<=MAX_TREE_SIZE-1; i++)
	{
		if (T[i] == e)//找到e 
		{
			return T[(i+1)/2-1]; 
		} 
	}//for
	return Nil;//没找到e 
 }
 
 TElemType LeftChild(SqBiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点。操作结果：返回e的左孩子。若e无左孩子，则返回“空”
	int i;
	
	if (T[0] == Nil)//空树
	{
		return Nil; 
	} 
	
	for (i=0; i<=MAX_TREE_SIZE-1; i++)
	{
		if (T[i] == e)//找到e
		{
			return T[i*2+1]; 
		} 
	}
	
	return Nil;//没找到e 
 } 
 
 TElemType RightChild(SqBiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点。操作结果：返回e的右孩子。若e无右孩子，则返回“空”
	int i;
	
	if (T[0] == Nil)//空树 
	{
		return Nil; 
	} 
	
	for (i=0; i<=MAX_TREE_SIZE-1; i++)
	{
		if (T[i] == e)//找到e
		{
			return T[i*2+2];
		}
	}//for
	return Nil;//没找到e 
 }
 
 TElemType LeftSibling(SqBiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点
	//操作结果：返回e的左兄弟。若e是T的左孩子或无左兄弟，则返回“空”
	int i;
	
	if (T[0] == Nil)//空树
	{
		return Nil; 
	} 
	
	for (i=1; i<=MAX_TREE_SIZE-1; i++)
	{
		if (T[i]==e && i%2==0)//找到e且其序号为偶数（是右孩子）
		{
			return T[i-1]; 
		} 
	}//for
	
	return Nil;//没找到e 	
 }
 
 TElemType RightSibling(SqBiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在,e是T中某个结点
	//操作结果：返回e的右兄弟。若e是T的右孩子或无右兄弟，则返回“空”
    int i;
	
	if (T[0] == Nil)//空树
	{
		return Nil; 
	}
	for (i=1; i<=MAX_TREE_SIZE-1; i++)
	{
		if (T[i]==e && i%2)//找到e且其序号为奇数（是左孩子）
		{
			return T[i+1];
		} 
	}
	
	return Nil;//没找到e 
 } 
 
 void Move(SqBiTree q, int j, SqBiTree T, int i)//InsertChild()用到。加
 {
 	//把从q的j结点开始的子树移为从T的i结点开始的子树
	if (q[2*j+1] != Nil)//q的左子树不空
	{
		Move(q, (2*j+1), T, (2*i+1));//把q的j结点的左子树移为T的i结点的左子树 
	} 
	if (q[2*j+2] != Nil)//q的右子树不空
	{
		Move(q, (2*j+2), T, (2*i+2));//把q的j结点的右子树移为T的i结点的右子树 
	} 
	T[i] = q[j];//把q的j结点移为T的i结点
	q[j] = Nil;//把q的j结点置空 
 } 
 
 void InsertChild(SqBiTree T, TElemType p, int LR, SqBiTree c)
 {
 	//初始条件：二叉树T存在，p是T中某个结点的值，LR为0或1，非空二叉树c与T不相交且右子树为空
    //操作结果：根据LR为0或1，插入c为T中p结点的左或右子树。p结点的原有左或右子树则成为c的右子树
    int j, k, i = 0;
	
	for (j=0; j<(int)pow(2, BiTreeDepth(T))-1; j++)//查找p的序号 
	{
		if (T[j] == p)//j为p的序号
		{
			break; 
		} 
	} 
	k = 2*j+1+LR;//k为p的左或右孩子的序号
	if (T[k] != Nil)//p原来的左或右孩子不空
	{
		Move(T, k, T, 2*k+2);//把从T的k结点开始的子树移为从k结点的右子树开始的子树 
	} 
	Move(c, i, T, k);//把从c的i结点开始的子树移为从T的k结点开始的子树 	 
 }
 
 void InitQueue(LinkQueue *Q)
 {
 	//构造一个空队列Q
	(*Q).front = (*Q).rear = (QueuePtr)malloc(sizeof(QNode));
	
	if (! (*Q).front)
	{
		exit(OVERFLOW);
	}  
	
	(*Q).front->next = NULL;
 } 
 
 void DestroyQueue(LinkQueue *Q)
 {
 	//销毁队列Q(无论空否均可)
	while ( (*Q).front )
	{
		(*Q).rear = (*Q).front->next;
		free( (*Q).front );
		(*Q).front = (*Q).rear;
	}//while 
 } 
 
 void ClearQueue(LinkQueue *Q)
 {
 	//将Q清为空队列
	QueuePtr p, q;
	
	(*Q).rear = (*Q).front;
	p = (*Q).front->next;
	(*Q).front->next = NULL;
	
	while (p)
	{
		q = p;
		p = p->next;
		free(q);
	} //while
 }
 
 Status QueueEmpty(LinkQueue Q)
 {
 	//若Q为空队列，则返回TRUE，否则返回FALSE
	 if (Q.front->next == NULL)
	 {
	 	return TRUE;
 	 }  
 	 else
 	 {
 	 	return FALSE;
 	 }
 }
 
 int QueueLength(LinkQueue Q)
 {
 	//求队列长度
	 int i = 0;
	 QueuePtr p;
	 
	 p = Q.front;
	 
	 while (Q.rear != p)
	 {
	 	i++;
	 	p = p->next;
     } 
     
     return i;
 }
 
 Status GetHead_Q(LinkQueue Q, QElemType *e)//避免重名
 {
 	//若队列不空，则用e返回Q的队头元素，并返回OK，否则返回ERROR
	QueuePtr p;
	
	if (Q.front == Q.rear)
	{
		return ERROR;
	}
	
	p = Q.front->next;
	*e = p->data;
	
	return OK;
 } 
 
 void EnQueue(LinkQueue *Q, QElemType e)
 {
 	//插入元素e为Q的新的队尾元素
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if (! p)//存储分配失败
	{
		exit(OVERFLOW); 
	} 
	p->data = e;
	p->next = NULL;
	(*Q).rear->next = p;
	(*Q).rear = p;
 }
 
 Status DeQueue(LinkQueue *Q, QElemType *e)
 {
 	//队列不为空，删除Q的队头元素，用e返回其值，并返回OK，否则返回ERROR
	QueuePtr p;
	
	if ((*Q).front == (*Q).rear)
	{
		return ERROR;
	} 
	p = (*Q).front->next;
	*e = p->data;
	(*Q).front->next = p->next;
	if ((*Q).rear == p)
	{
		(*Q).rear == (*Q).front;
	}
	free(p);
	
	return OK;	
 }
 
 void QueueTraverse(LinkQueue Q, void(*vi)(QElemType))
 {
 	//从队头到队尾依次对队列Q中每个元素调用函数vi（）
	QueuePtr p;
	
	p = Q.front->next;
	
	while ( p )
	{
		vi(p->data);
		p = p->next;
	} 
	printf("\n");
 }
 
 void print(QElemType i)
 {
 	printf("%d ", i);
 }
 
 Status DeleteChild(SqBiTree T, position p, int LR)
 {
 	//初始条件：二叉树T存在，p指向T中某个结点，LR为1或0
	//操作结果：根据LR为1或0，删除T中p所指结点的左或右子树 
	int i;
	Status k = OK;//队列不空的标志
	LinkQueue q;
	
	InitQueue(&q);//初始化队列，用于存放贷删除的结点
	i = (int)pow(2, p.level-1) + p.order - 2;//将层，本层序号转为矩阵的序号
	if (T[i] == Nil)//此结点空
	{
		return ERROR; 
	} 
	i = i*2 + 1 + LR;//待删除子树的根节点在矩阵中的序号
	
	while (k)
	{
		if (T[2*i+1] != Nil)//左结点不空
		{
			EnQueue(&q, 2*i+1);//入队列左结点的序号 
		} 
		if (T[2*i+2] != Nil)//右结点不空
		{
			EnQueue(&q, 2*i+2);//入队列右结点的序号 
		} 
		T[i] = Nil;//删除此结点
		k = DeQueue(&q, &i);//队列不空 
	} 
	
	return OK; 
 } 
 
 void(*VisitFunc)(TElemType);//函数变量
 
 void PreTraverse(SqBiTree T, int e)
 {
 	//PreOrderTraverse()调用
	VisitFunc(T[e]);
	
	if (T[2*e+1] != Nil)//左子树不空
	{
		PreTraverse(T, 2*e+1); 
	} 
	if (T[2*e+2] != Nil)//右子树不空
	{
		PreTraverse(T, 2*e+2); 
	} 
 } 
 
 void PreOrderTraverse(SqBiTree T, void(*Visit)(TElemType))
 {
 	//初始条件:二叉树存在，Visit是对结点操作的应用函数
	//操作结果：先序遍历T，对每个结点调用函数Visit一次且仅一次
	VisitFunc = Visit;
	
	if (! BiTreeEmpty(T))//树不空 
	{
		PreTraverse(T, 0); 
	} 
	printf("\n");
 }
 
 void InTraverse(SqBiTree T, int e)
 {
 	//InOrderTraverse()调用
	if (T[2*e+1] != Nil)//左子树不空
	{
		InTraverse(T, 2*e+1); 
	} 
	VisitFunc(T[e]);
	
	if (T[2*e+2] != Nil)//右子树不空
	{
		InTraverse(T, 2*e+2); 
	} 
 }
 
 void InOrderTraverse(SqBiTree T, void(*Visit)(TElemType))
 {
 	//初始条件：二叉树 存在，Visit是对结点操作的应用函数
	//操作结果：中序遍历T，对每个结点调用函数Visit一次且仅一次
	VisitFunc = Visit;
	
	if (! BiTreeEmpty(T))//树不空
	{
		InTraverse(T, 0); 
	}  
	printf("\n");
 }
 
 void PostTraverse(SqBiTree T, int e)
 {
 	//PostOrderTraverse（）调用
	if (T[2*e+1] != Nil)//左子树不空
	{
		PostTraverse(T, 2*e+1); 
	} 
	if (T[2*e+2] != Nil)//右子树不空
	{
		PostTraverse(T, 2*e+2); 
	} 
	
	VisitFunc(T[e]);
 }
 
 void PostOrderTraverse(SqBiTree T, void(*Visit)(TElemType))
 {
 	//初始条件：二叉树T存在，Visit是对结点操作的应用函数
	//操作结果：后序遍历T，对每个结点调用函数Visit一次且仅一次
	VisitFunc = Visit;
	if (! BiTreeEmpty(T))//树不空
	{
		PostTraverse(T, 0); 
	}  
	printf("\n");
 }
 
 void LevelOrderTraverse(SqBiTree T, void(*Visit)(TElemType))
 {
 	//层序遍历二叉树
	int i = MAX_TREE_SIZE-1, j;
	
	while (T[i] == Nil)
	{
		i--;//找到最后一个非空节点的序号 
	} 
	for (j=0; j<=0; j++)//从根节点起，按层序遍历二叉树
	{
		if (T[j] != Nil)
		{
			Visit(T[j]);//只遍历非空的结点 
		} 
	} 
	
	printf("\n"); 
 }
 
 void Print(SqBiTree T)
 {
 	//逐层，按本层序号输出二叉树
	int j, k;
	position p;
	TElemType e;
	
	for (j=1; j<=BiTreeDepth(T); j++)
	{
		printf("第%d层： ", j);
		for (k=1; k<=pow(2, j-1); k++)
		{
			p.level = j;
			p.order = k;
			e = Value(T, p);
			if (e != Nil)
			{
				printf("%d:"form" ", k, e);
			}//if
		}//for-in
		printf("\n");
	} //for-out	
 } 
 
 void visit(TElemType e)
 {
   printf(form" ",e);
 }
 
 

 
 
 
  
 
 
 
 
 
 