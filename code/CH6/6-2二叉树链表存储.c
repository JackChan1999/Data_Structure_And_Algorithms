 #define CHAR //字符型
 //#define INT //整型(二者选一)
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
 
 #ifdef CHAR
    typedef char TElemType;
	TElemType Nil = ' ';//字符型以空格符为空
	#define form "%c"//输入输出的格式为%c
 #endif
 #ifdef INT
    typedef int TElemType;
    TElemType Nil = 0;//整型以0为空
	#define form "%d"//输入输出的格式为%d 
 #endif
 
 typedef struct BiTNode
 {
 	TElemType data;
 	struct BiTNode *lchild, *rchild;//左右孩子指针 
 }BiTNode, *BiTree;
 
 #define ClearBiTree DestroyBiTree//清空二叉树和销毁二叉树的操作一样  
 typedef BiTree QElemType;//设队列元素为二叉树的指针类型
 typedef struct QNode
 {
 	QElemType data;
 	struct QNode *next;
 }QNode, * QueuePtr;
 
 typedef struct 
 {
 	QueuePtr front, rear;//队头，队尾指针 
 }LinkQueue;  
 
 typedef BiTree SElemType;//设栈元素为二叉树的指针类型 
 
 #define STACK_INIT_SIZE 10  //存储空间初始分配量
 #define STACK_INCREMENT 2   //存储空间分配增量
 typedef struct SqStack
 {
 	SElemType *base;//在栈构造之前和销毁之后，base的值为NULL
	SElemType *top;//栈顶指针
	int stacksize;//当前已分配的存储空间，以元素为单位 
 }SqStack;//顺序栈
 
void InitBiTree(BiTree *T);
void DestroyBiTree(BiTree *T);
void PreOrderTraverse(BiTree T, void(*Visit)(TElemType));
void InOrderTraverse(BiTree T, void(*Visit)(TElemType));
void CreateBiTree(BiTree *T);
Status BiTreeEmpty(BiTree T);
int BiTreeDepth(BiTree T);
TElemType Root(BiTree T);
TElemType Value(BiTree p);
void Assign(BiTree p, TElemType value);
void InitQueue(LinkQueue *Q);
void DestroyQueue(LinkQueue *Q);
void ClearQueue(LinkQueue *Q);
Status QueueEmpty(LinkQueue Q);
int QueueLength(LinkQueue Q);
Status GetHead_Q(LinkQueue Q, QElemType *e);//避免重名
void EnQueue(LinkQueue *Q, QElemType e);
Status DeQueue(LinkQueue *Q, QElemType *e);
void QueueTraverse(LinkQueue Q, void(*vi)(QElemType));
TElemType Parent(BiTree T, TElemType e);
BiTree Point(BiTree T, TElemType s);
TElemType LeftChild(BiTree T, TElemType e);
TElemType RightChild(BiTree T, TElemType e);
TElemType LeftSibling(BiTree T, TElemType e);
TElemType RightSibling(BiTree T, TElemType e);
 Status InsertChild(BiTree p, int LR, BiTree c);//形参T无用 
Status DeleteChild(BiTree p, int LR);//形参T无用
void InitStack(SqStack *S);
void DestroyStack(SqStack *S);
void ClearStack(SqStack *S);
Status StackEmpty(SqStack S);
int StackLength(SqStack S);
Status GetTop(SqStack S, SElemType *e);
void Push(SqStack *S, SElemType e);
Status Pop(SqStack *S, SElemType *e);
void StackTraverse(SqStack S, void(*visit)(SElemType));
void InOrderTraverse1(BiTree T, void(*Visit)(TElemType));
void InOrderTraverse2(BiTree T, void(*Visit)(TElemType));
void PostOrderTraverse(BiTree T, void(*Visit)(TElemType));
void LevelOrderTraverse(BiTree T, void(*Visit)(TElemType));
void visitT(TElemType e);
 
 int main()
 {
   int i;
   BiTree T,p,c;
   TElemType e1,e2;
   InitBiTree(&T);
   printf("构造空二叉树后,树空否？%d(1:是 0:否)树的深度=%d\n",BiTreeEmpty(T),BiTreeDepth(T));
   e1=Root(T);
   if(e1!=Nil)
     printf("二叉树的根为: "form"\n",e1);
   else
     printf("树空，无根\n");
 #ifdef CHAR
   printf("请先序输入二叉树(如:ab三个空格表示a为根结点,b为左子树的二叉树)\n");
 #endif
 #ifdef INT
   printf("请先序输入二叉树(如:1 2 0 0 0表示1为根结点,2为左子树的二叉树)\n");
 #endif
   CreateBiTree(&T);
   printf("建立二叉树后,树空否？%d(1:是 0:否) 树的深度=%d\n",BiTreeEmpty(T),BiTreeDepth(T));
   e1=Root(T);
   if(e1!=Nil)
     printf("二叉树的根为: "form"\n",e1);
   else
     printf("树空，无根\n");
   printf("中序递归遍历二叉树:\n");
   InOrderTraverse(T,visitT);
   printf("\n后序递归遍历二叉树:\n");
   PostOrderTraverse(T,visitT);
   printf("\n请输入一个结点的值: ");
   scanf("%*c"form,&e1);
   p=Point(T,e1); /* p为e1的指针 */
   printf("结点的值为"form"\n",Value(p));
   printf("欲改变此结点的值，请输入新值: ");
   scanf("%*c"form"%*c",&e2); /* 后一个%*c吃掉回车符，为调用CreateBiTree()做准备 */
   Assign(p,e2);
   printf("层序遍历二叉树:\n");
   LevelOrderTraverse(T,visitT);
   e1=Parent(T,e2);
   if(e1!=Nil)
     printf("%c的双亲是"form"\n",e2,e1);
   else
     printf(form"没有双亲\n",e2);
   e1=LeftChild(T,e2);
   if(e1!=Nil)
     printf(form"的左孩子是"form"\n",e2,e1);
   else
     printf(form"没有左孩子\n",e2);
   e1=RightChild(T,e2);
   if(e1!=Nil)
     printf(form"的右孩子是"form"\n",e2,e1);
   else
     printf(form"没有右孩子\n",e2);
   e1=LeftSibling(T,e2);
   if(e1!=Nil)
     printf(form"的左兄弟是"form"\n",e2,e1);
   else
     printf(form"没有左兄弟\n",e2);
   e1=RightSibling(T,e2);
   if(e1!=Nil)
     printf(form"的右兄弟是"form"\n",e2,e1);
   else
     printf(form"没有右兄弟\n",e2);
   InitBiTree(&c);
   printf("构造一个右子树为空的二叉树c:\n");
 #ifdef CHAR
   printf("请先序输入二叉树(如:ab三个空格表示a为根结点,b为左子树的二叉树)\n");
 #endif
 #ifdef INT
   printf("请先序输入二叉树(如:1 2 0 0 0表示1为根结点,2为左子树的二叉树)\n");
 #endif
   CreateBiTree(&c);
   printf("先序递归遍历二叉树c:\n");
   PreOrderTraverse(c,visitT);
   printf("\n层序遍历二叉树c:\n");
   LevelOrderTraverse(c,visitT);
   printf("树c插到树T中,请输入树T中树c的双亲结点 c为左(0)或右(1)子树: ");
   scanf("%*c"form"%d",&e1,&i);
   p=Point(T,e1); /* p是T中树c的双亲结点指针 */
   InsertChild(p,i,c);
   printf("先序递归遍历二叉树:\n");
   PreOrderTraverse(T,visitT);
   printf("\n中序非递归遍历二叉树:\n");
   InOrderTraverse1(T,visitT);
   printf("删除子树,请输入待删除子树的双亲结点  左(0)或右(1)子树: ");
   scanf("%*c"form"%d",&e1,&i);
   p=Point(T,e1);
   DeleteChild(p,i);
   printf("先序递归遍历二叉树:\n");
   PreOrderTraverse(T,visitT);
   printf("\n中序非递归遍历二叉树(另一种方法):\n");
   InOrderTraverse2(T,visitT);
   DestroyBiTree(&T);
 	
 	return 0;
 }
   
 
 void InitBiTree(BiTree *T)
 {
 	//操作结果：构造空二叉树T
	*T = NULL; 
 } 
 
 void DestroyBiTree(BiTree *T)
 {
 	//初始条件：二叉树T存在。操作结果：销毁二叉树T
	if (*T)//非空树
	{
		if ((*T)->lchild)//有左孩子
		{
			DestroyBiTree(&(*T)->lchild);//销毁左孩子子树 
		} 
		if ((*T)->rchild)//有右孩子
		{
			DestroyBiTree(&(*T)->rchild);//销毁右孩子子树 
		}
		free(*T);//释放根结点
		*T = NULL;//空指针 		
	} //if 
 }
 
 void PreOrderTraverse(BiTree T, void(*Visit)(TElemType))
 {
 	//初始条件：二叉树T存在，Visit是对结点操作的应用函数。
	//操作结果：先序递归遍历T,对每个结点调用函数Visit一次且仅一次
	if (T)//T不空
	{
		Visit(T->data);//先访问根节点
		PreOrderTraverse(T->lchild, Visit);//再先序遍历左子树
		PreOrderTraverse(T->rchild, Visit);//最后先序遍历右子树 
	}//if 
 }
 
 void InOrderTraverse(BiTree T, void(*Visit)(TElemType))
 {
 	//初始条件：二叉树T存在，Visit是对结点操作的应用函数
	//操作结果：中序递归遍历T，对每个结点调用Visit一次且仅一次
	if (T)
	{
		InOrderTraverse(T->lchild, Visit);//先中序遍历左子树
		Visit(T->data);//再访问根结点
		InOrderTraverse(T->rchild, Visit);//最后中序遍历右子树 		 
	} //if 
 }
 
 void CreateBiTree(BiTree *T)
 {
 	//按先序次序输入二叉树中结点的值（可为字符型或整型，在主程中定义），
	//构造二叉链表表示的二叉树T。变量Nil表示空(子)树。有改动
	TElemType ch;
	
	scanf(form, &ch);
	
	if (ch == Nil)//空
	{
		*T = NULL; 
	} 
	else
	{
		*T = (BiTree)malloc(sizeof(BiTree));//生成根结点
		if (!*T)
		{
			exit(OVERFLOW);
		} 
		(*T)->data = ch;
		CreateBiTree(&(*T)->lchild);//构造左子树 
		CreateBiTree(&(*T)->rchild);//构造右子树 
	}
 }
 
 Status BiTreeEmpty(BiTree T)
 {
 	//初始条件：二叉树T存在。操作结果：若T为空二叉树，则返回TRUE，否则FALSE
	if (T)
	{
		return FALSE;
	} 
	else
	{
		return TRUE;
	}
 }
 
 int BiTreeDepth(BiTree T)
 {
 	//初始条件：二叉树T存在。操作结果：返回T的深度
	int i, j;
	
	if (!T)
	{
		return 0;//空树深度为0 
	} 
	if (T->lchild)
	{
		i = BiTreeDepth(T->lchild);//i为左子树的深度 
	} 
	else
	{
		i = 0;  
	} 
	
	if (T->rchild)
	{
		j = BiTreeDepth(T->rchild);//j为右子树的深度 
	}
	else
	{
		j=0;
	} 
	
	return i>j? i+1 : j+1;//T的深度为其左右子树的深度中的大者+1 
 } 
 
 TElemType Root(BiTree T)
 {
 	//初始条件：二叉树T存在。操作结果：返回T的根
	if (BiTreeEmpty(T))
	{
		return Nil;
	} 
	else
	{
		return T->data;
	}
 }
 
 TElemType Value(BiTree p)
 {
 	//初始条件：二叉树T存在，p指向T中某个结点。操作结果：返回p所指结点的值
	return p->data; 
 }
 
 void Assign(BiTree p, TElemType value)
 {
 	//给p所指结点赋值为value
	p->data = value; 
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
 
 TElemType Parent(BiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点
	//操作结果：若e是T的非根结点，则返回它的双亲，否则返回“空”
	LinkQueue q;
	QElemType a;
	
	if (T)//非空树 
	{
		InitQueue(&q);//初始化队列
		EnQueue(&q, T);//树根指针入队 
		
		while (! QueueEmpty(q))//队不空 
		{
			DeQueue(&q, &a);//出队，队列元素赋给a
			if (a->lchild && a->lchild->data==e || a->rchild && a->rchild->data==e)//找到e（是其左或右孩子） 
			{
				return a->data;//返回e的双亲的值 
			} 
			else//没找到e,则入队其左右孩子指针(如果非空) 
			{
				if (a->lchild)
				{
					EnQueue(&q, a->lchild); 
				} 
				if (a->rchild)
				{
					EnQueue(&q, a->rchild);
				}//if
			} //else
		} //while
	} //if
	
	return Nil;//树空或没找到e 
 } 
 
 BiTree Point(BiTree T, TElemType s)
 {
 	//返回二叉树T中指向元素值为s的结点的指针。另加
	LinkQueue q;
	QElemType a;
	
	if (T)//非空树
	{
		InitQueue(&q);//初始化队列
		EnQueue(&q, T);//根指针入队
		
		while (! QueueEmpty(q))//队不空
		{
			DeQueue(&q, &a);//出队，队列元素赋给a
			if (a->data == s)
			{
				return a;
			} 
			if (a->lchild)//有左孩子 
			{
				EnQueue(&q, a->lchild);//入队左孩子 
			} 
			if (a->rchild)//有右孩子
			{
				EnQueue(&q, a->rchild);//入队右孩子 
			} //if
		} //while
		
	} //if
	
	return NULL;
 } 
 
 TElemType LeftChild(BiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点。操作结果：返回e的左孩子。若e无左孩子，则返回“空” 
 	BiTree a;
 	
 	if (T)//非空树
    {
    	a = Point(T, e);//a是结点e的指针
		if (a && a->lchild)//T中存在结点e且e存在左孩子
		{
			return a->lchild->data;//返回e的左孩子的值 
		}//if 
	} //if
	return Nil;//其余情况返回空 
 }
 
 TElemType RightChild(BiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点。操作结果：返回e的右孩子。若e无右孩子，则返回“空”
	BiTree a;
	
	if (T)//非空树
	{
		a = Point(T, e);//a是结点e的指针
		
		if (a && a->rchild)//T中存在结点e且e存在右孩子
		{
			return a->rchild->data;//返回e的右孩子的值 
		}//if	
	} //if
	
	return Nil;//其余情况返回空 
 } 
 
 TElemType LeftSibling(BiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点
    //操作结果：返回e的左兄弟。若e是T的左孩子或无左兄弟，则返回“空”
	TElemType a;
	BiTree p;
	
	if (T)//非空树
	{
		a = Parent(T, e);//a为e的双亲
		if (a != Nil)//找到e的双亲
		{
			p = Point(T, a);//p为指向结点a的指针
			if (p->lchild && p->rchild && p->rchild->data==e)//p存在左右孩子且右孩子是e
			{
				return p->lchild->data;//返回p的左孩子（e的左兄弟） 
			} //if
		} //if
	} //if
	
	return Nil;//其余情况返回空 	
 } 
 
 TElemType RightSibling(BiTree T, TElemType e)
 {
 	//初始条件：二叉树T存在，e是T中某个结点
	//操作结果：返回e的右兄弟。若e是T的右孩子或无右兄弟，则返回"空"
	TElemType a;
	BiTree p;
	
	if (T)//非空树 
	{
		a = Parent(T, e);//a为e的双亲
		if (a != Nil)//找到e的双亲
		{
			p = Point(T, a);//p为指向结点a的指针
			if (p->lchild && p->rchild && p->lchild->data==e)//p存在左右孩子且左孩子是e
			{
				return p->rchild->data;//返回p的右孩子(e的右兄弟) 
			} //if
		} //if		
	} //if
	
	return Nil;//其余情况返回空 
 } 
 
 Status InsertChild(BiTree p, int LR, BiTree c)//形参T无用 
 {
 	//初始条件：二叉树T存在，p指向T中某个结点，LR为0或1，非空二叉树c与T不想交且右子树为空
	//操作结果：根据LR为0或1，插入c为T中p所指结点的左或右子树。p所指结点的原有左或右子树。
	          //原有左或右子树则成为c的右子树
    if (p)//p不空
	{
		if (LR == 0)
		{
			c->rchild = p->lchild;
			p->lchild = c;
		}
		else//LR==1
		{
			c->rchild = p->rchild;
			p->rchild = c;
			
		}
		return OK;
	} 
	
	return ERROR;//p空 
 } 
 
 Status DeleteChild(BiTree p, int LR)//形参T无用
 {
 	//初始条件：二叉树T存在，p指向T中某个结点，LR为0或1
	//操作结果：根据LR为0或1，删除T中p所指结点的左或右子树
	if (p)//p不空
	{
		if (LR == 0)//删除左子树
		{
			ClearBiTree(&p->lchild); 
		} 
		else
		{
			ClearBiTree(&p->rchild);
		}
		
		return OK;
	} //if
	return ERROR;//p空 
 } 
 
  void InitStack(SqStack *S)
 {
 	//构造一个空栈S
    (*S).base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (! (*S).base)
	{
		exit(OVERFLOW);//存储分配失败 
	} 
	(*S).top = (*S).base;
	(*S).stacksize = STACK_INIT_SIZE; 	
 } 
 
 void DestroyStack(SqStack *S)
 {
 	//销毁栈S，S不再存在
	 free( (*S).base );
	 (*S).base = NULL;
	 (*S).top = NULL;
	 (*S).stacksize = 0; 
 }
 
 void ClearStack(SqStack *S)
 {
 	//把S置为空栈
    (*S).top = (*S).base; 
 }
 
 Status StackEmpty(SqStack S)
 {
 	//若栈S为空栈，则返回TRUE，否则返回FALSE
	if (S.top == S.base)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}	
 }
 
 int StackLength(SqStack S)
 {
 	//返回S的元素个数，即栈的长度
	return S.top - S.base; 
 }
 
 Status GetTop(SqStack S, SElemType *e)
 {
 	//若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR
	if (S.top > S.base)
	{
		*e = *(S.top - 1);
		return OK;
	} 
	else
	{
		return ERROR;
	}
 }
 
 void Push(SqStack *S, SElemType e)
 {
 	//插入元素e为新的栈顶元素
	if ( ((*S).top - (*S).base) >= (*S).stacksize )//栈满，追加存储空间
	{
	  (*S).base = (SElemType *)realloc( (*S).base, ((*S).stacksize +STACK_INCREMENT)
                                                        * sizeof(SElemType) );
	  if (! (*S).base)
	  {
	    exit(OVERFLOW);//存储分配失败 
	  } 
			
	  (*S).top = (*S).base + (*S).stacksize;
	  (*S).stacksize += STACK_INCREMENT; 
	} 
		
	*((*S).top)++ = e;
 }
 
 Status Pop(SqStack *S, SElemType *e)
 {
 	//若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；
	//否则返回ERROR
	if ( (*S).top == (*S).base )
	{
		return ERROR;
	} 
	
	*e = *--(*S).top ;
	return OK;
 }
 
 void StackTraverse(SqStack S, void(*visit)(SElemType))
 {
 	//从栈顶到栈底依次对栈中每个元素调用函数visit()
	while (S.top > S.base)
	{
		visit( *S.base++ );
	} 
	
	printf("\n");
 }
 
 void InOrderTraverse1(BiTree T, void(*Visit)(TElemType))
 {
 	//采用二叉链表存储结构，Visit是对数据元素操作的应用函数。有改动
	//中序遍历二叉树T的非递归算法(利用栈)，对每个数据元素调用函数Visit 
	SqStack S;
	
	InitStack(&S);
	
	while (T || StackEmpty(S))
	{
		if (T)
		{
			//根指针进栈，遍历左子树
			Push(&S, T);
			T = T->lchild; 
		}
		else
		{
			//根指针退栈，访问根节点，遍历右子树
			Pop(&S, &T); 
			Visit(T->data);
			T = T->rchild;
		}
	}//while
	
	printf("\n"); 
 } 
 
 void InOrderTraverse2(BiTree T, void(*Visit)(TElemType))
 {
 	//采用二叉链表存储结构。Visit是对数据元素操作的应用函数。
	//中序遍历二叉树T的非递归算法（利用栈），对每个数据元素调用函数Visit
	SqStack S;
	BiTree p;
	
	InitStack(&S); 
	Push(&S, T);//根指针进栈
	
	while (! StackEmpty(S))
	{
		while (GetTop(S, &p) && p)
		{
			Push(&S, p->lchild);//向左走到尽头 
		}
		Pop(&S, &p);//空指针退栈 
		if (! StackEmpty(S))
		{
			//访问结点，向右一步
			Pop(&S, &p);
			Visit(p->data);
			Push(&S, p->rchild); 
		} 
	} //while
	
	printf("\n");
 }
 
 void PostOrderTraverse(BiTree T, void(*Visit)(TElemType))
 {
 	//初始条件：二叉树T存在，Visit是对结点操作的应用函数
	//操作结果：后序递归遍历T，对每个结点调用函数Visit一次且仅一次
	if (T)//T不空
	{
		PostOrderTraverse(T->lchild, Visit);//先后序遍历左子树
		PostOrderTraverse(T->rchild, Visit);//再后续遍历右子树
		Visit(T->data);//最后访问根节点 
	}  
 }
 
 void LevelOrderTraverse(BiTree T, void(*Visit)(TElemType))
 {
 	//初始条件：二叉树T存在，Visit是对结点操作的应用函数
	//操作结果:按层序递归遍历T(利用队列)，对每个结点调用函数Visit一次且仅一次 
	LinkQueue q;
	QElemType a;
	
	if (T)
	{
		InitQueue(&q);//初始化队列q
		EnQueue(&q, T);//根指针入队
		
		while (! QueueEmpty(q))//队列不空
		{
			DeQueue(&q, &a);//出队元素（指针），赋给a
			Visit(a->data);//访问a所指结点
			if (a->lchild != NULL)//a有左孩子 
			{
				EnQueue(&q, a->lchild);//入队a的左孩子 
			} 
            if (a->rchild != NULL)//a有右孩子 
			{
				EnQueue(&q, a->rchild);//入队a的右孩子 
			} 		
		} //while 
		printf("\n");
	} 
 } 
 
 
 
 
 
 
 
 
 void visitT(TElemType e)
 {
 	printf(form" ", e);
 }
 
 
 
 
 
 
  
 
 
 
	 