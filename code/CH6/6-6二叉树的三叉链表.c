#define CHAR /* 字符型 */
 /* #define INT /* 整型(二者选一) */
 #ifdef CHAR
   typedef char TElemType;
   TElemType Nil=' '; /* 字符型以空格符为空 */
   #define form "%c" /* 输入输出的格式为%c */
 #endif
 #ifdef INT
   typedef int TElemType;
   TElemType Nil=0; /* 整型以0为空 */
   #define form "%d" /* 输入输出的格式为%d */
 #endif
 
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
 
 typedef struct BiTPNode
 {
   TElemType data;
   struct BiTPNode *parent, *lchild, *rchild; /* 双亲、左右孩子指针 */
 }BiTPNode, * BiPTree;
 
 #define ClearBiTree DestroyBiTree /* 清空二叉树和销毁二叉树的操作一样 */
 typedef BiPTree QElemType; /* 设队列元素为二叉树的指针类型 */
 typedef struct QNode
 {
 	QElemType data;
 	struct QNode *next;
 }QNode, * QueuePtr;
 
 typedef struct 
 {
 	QueuePtr front, rear;//队头，队尾指针 
 }LinkQueue; 
 
void InitBiTree(BiPTree *T);
void DestroyBiTree(BiPTree *T);
void CreateBiTree(BiPTree *T);
Status BiTreeEmpty(BiPTree T);
int BiTreeDepth(BiPTree T);
TElemType Root(BiPTree T);
TElemType Value(BiPTree p);
void Assign(BiPTree p, TElemType value);
void InitQueue(LinkQueue *Q);
void DestroyQueue(LinkQueue *Q);
void ClearQueue(LinkQueue *Q);
Status QueueEmpty(LinkQueue Q);
int QueueLength(LinkQueue Q);
Status GetHead_Q(LinkQueue Q, QElemType *e);//避免重名
void EnQueue(LinkQueue *Q, QElemType e);
Status DeQueue(LinkQueue *Q, QElemType *e);
void QueueTraverse(LinkQueue Q, void(*vi)(QElemType));
BiPTree Point(BiPTree T, TElemType e);
TElemType Parent(BiPTree T, TElemType e);
TElemType LeftChild(BiPTree T, TElemType e);
TElemType LeftSibling(BiPTree T, TElemType e);
TElemType RightChild(BiPTree T, TElemType e);
TElemType RightSibling(BiPTree T, TElemType e); 
Status InsertChild(BiPTree p, int LR, BiPTree c); /* 形参T无用 */
Status DeleteChild(BiPTree p, int LR); /* 形参T无用 */
void PreOrderTraverse(BiPTree T, void(*Visit)(BiPTree));
void InOrderTraverse(BiPTree T, void(*Visit)(BiPTree));
void PostOrderTraverse(BiPTree T, void(*Visit)(BiPTree));
void LevelOrderTraverse(BiPTree T, void(*Visit)(BiPTree));
void visitT(BiPTree T);

 int main()
 {
   int i;
   BiPTree T,c,q;
   TElemType e1,e2;
   InitBiTree(&T);
   printf("构造空二叉树后,树空否？%d(1:是 0:否)树的深度=%d\n",BiTreeEmpty(T),BiTreeDepth(T));
   e1=Root(T);
   if(e1!=Nil)
     printf("二叉树的根为: "form"\n",e1);
   else
     printf("树空，无根\n");
 #ifdef CHAR
   printf("请按先序输入二叉树(如:ab三个空格表示a为根结点,b为左子树的二叉树)\n");
 #endif
 #ifdef INT
   printf("请按先序输入二叉树(如:1 2 0 0 0表示1为根结点,2为左子树的二叉树)\n");
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
   printf("\n层序遍历二叉树:\n");
   LevelOrderTraverse(T,visitT);
   printf("\n请输入一个结点的值: ");
   scanf("%*c"form,&e1);
   c=Point(T,e1); /* c为e1的指针 */
   printf("结点的值为"form"\n",Value(c));
   printf("欲改变此结点的值，请输入新值: ");
   scanf("%*c"form"%*c",&e2);
   Assign(c,e2);
   printf("层序遍历二叉树:\n");
   LevelOrderTraverse(T,visitT);
   e1=Parent(T,e2);
   if(e1!=Nil)
     printf("\n"form"的双亲是"form"\n",e2,e1);
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
   printf("\n树c插到树T中,请输入树T中树c的双亲结点 c为左(0)或右(1)子树: ");
   scanf("%*c"form"%d",&e1,&i);
   q=Point(T,e1);
   InsertChild(q,i,c);
   printf("先序递归遍历二叉树:\n");
   PreOrderTraverse(T,visitT);
   printf("\n删除子树,请输入待删除子树的双亲结点  左(0)或右(1)子树: ");
   scanf("%*c"form"%d",&e1,&i);
   q=Point(T,e1);
   DeleteChild(q,i);
   printf("先序递归遍历二叉树:\n");
   PreOrderTraverse(T,visitT);
   printf("\n");
   DestroyBiTree(&T);
 	
   return 0;
 } 
 
 void InitBiTree(BiPTree *T)
 { 
   /* 操作结果：构造空二叉树T */
   *T = NULL;
 }

 void DestroyBiTree(BiPTree *T)
 { 
   /* 初始条件：二叉树T存在。操作结果：销毁二叉树T */
   if(*T) /* 非空树 */
   {
     if((*T)->lchild) /* 有左孩子 */
     { 
       DestroyBiTree(&(*T)->lchild); /* 销毁左孩子子树 */
     }
	 if((*T)->rchild) /* 有右孩子 */
     {  
	   DestroyBiTree(&(*T)->rchild); /* 销毁右孩子子树 */
     } 
     
	 free(*T); /* 释放根结点 */
     *T = NULL; /* 空指针赋0 */
   }
 }

 void CreateBiTree(BiPTree *T)
 { 
   /* 按先序次序输入二叉树中结点的值(可为字符型或整型，在主程中定义)，*/
   /* 构造三叉链表表示的二叉树T */
   TElemType ch;
   
   scanf(form, &ch);
   if(ch == Nil) /* 空 */
   {  
     *T=NULL;
   }
   else
   {
     *T = (BiPTree)malloc(sizeof(BiTPNode)); /* 动态生成根结点 */
     if(! *T)
     { 
       exit(OVERFLOW);
     } 
	 (*T)->data = ch; /* 给根结点赋值 */
     (*T)->parent = NULL; /* 根结点无双亲 */
     CreateBiTree(&(*T)->lchild); /* 构造左子树 */
     if((*T)->lchild) /* 有左孩子 */
     {  
	   (*T)->lchild->parent = *T; /* 给左孩子的双亲域赋值 */
     }
	 CreateBiTree(&(*T)->rchild); /* 构造右子树 */
     if((*T)->rchild) /* 有右孩子 */
     {	
       (*T)->rchild->parent = *T; /* 给右孩子的双亲域赋值 */
     }
   }
 }

 Status BiTreeEmpty(BiPTree T)
 { 
   /* 初始条件：二叉树T存在。操作结果：若T为空二叉树，则返回TRUE，否则FALSE */
   if(T)
   {
     return FALSE;
   }
   else
   {
     return TRUE;
   }
 }

 int BiTreeDepth(BiPTree T)
 { 
   /* 初始条件：二叉树T存在。操作结果：返回T的深度 */
   int i, j;
   
   if(! T)
   {
     return 0; /* 空树深度为0 */
   }
   
   if(T->lchild)
   {
     i = BiTreeDepth(T->lchild); /* i为左子树的深度 */
   }
   else
   {
     i = 0;
   }
   
   if(T->rchild)
   {
     j = BiTreeDepth(T->rchild); /* j为右子树的深度 */
   }
   else
   {
     j = 0;
   }
   return i>j ? i+1 : j+1; /* T的深度为其左右子树的深度中的大者+1 */
 }

 TElemType Root(BiPTree T)
 { /* 初始条件：二叉树T存在。操作结果：返回T的根 */
   if(T)
   {
     return T->data;
   }
   else
   {
     return Nil;
   }
 }

 TElemType Value(BiPTree p)
 { 
   /* 初始条件：二叉树T存在，p指向T中某个结点。操作结果：返回p所指结点的值 */
   return p->data;
 }

 void Assign(BiPTree p, TElemType value)
 { 
   /* 给p所指结点赋值为value */
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
 
 BiPTree Point(BiPTree T, TElemType e)
 { /* 返回二叉树T中指向元素值为e的结点的指针。加 */
   LinkQueue q;
   QElemType a;
   
   if(T) /* 非空树 */
   {
     InitQueue(&q); /* 初始化队列 */
     EnQueue(&q, T); /* 根结点入队 */
     while(! QueueEmpty(q)) /* 队不空 */
     {
       DeQueue(&q, &a); /* 出队，队列元素赋给a */
       if(a->data == e)
       {
         return a;
       }
	   if(a->lchild) /* 有左孩子 */
       {
         EnQueue(&q, a->lchild); /* 入队左孩子 */
       }
	   if(a->rchild) /* 有右孩子 */
       {
         EnQueue(&q, a->rchild); /* 入队右孩子 */
       }
	 }//while
   }//if
   
   return NULL;
 }

 TElemType Parent(BiPTree T, TElemType e)
 { 
   /* 初始条件：二叉树T存在，e是T中某个结点 */
   /* 操作结果：若e是T的非根结点，则返回它的双亲，否则返回＂空＂*/
   BiPTree a;
   if(T) /* 非空树 */
   {
     a = Point(T, e); /* a是结点e的指针 */
     if(a && a!=T) /* T中存在结点e且e是非根结点 */
     {
       return a->parent->data; /* 返回e的双亲的值 */
     }
   }
   
   return Nil; /* 其余情况返回空 */
 }

 TElemType LeftChild(BiPTree T, TElemType e)
 { 
   /* 初始条件：二叉树T存在，e是T中某个结点。操作结果：返回e的左孩子。若e无左孩子,则返回"空" */
   BiPTree a;
   
   if(T) /* 非空树 */
   {
     a = Point(T, e); /* a是结点e的指针 */
     if(a && a->lchild) /* T中存在结点e且e存在左孩子 */
     {
       return a->lchild->data; /* 返回e的左孩子的值 */
     }
   }
   
   return Nil; /* 其余情况返回空 */
 }

 TElemType RightChild(BiPTree T, TElemType e)
 { 
   /* 初始条件：二叉树T存在，e是T中某个结点。操作结果：返回e的右孩子。若e无右孩子,则返回"空" */
   BiPTree a;
   
   if(T) /* 非空树 */
   {
     a = Point(T, e); /* a是结点e的指针 */
     if(a && a->rchild) /* T中存在结点e且e存在右孩子 */
     {
       return a->rchild->data; /* 返回e的右孩子的值 */
     }
   }
   
   return Nil; /* 其余情况返回空 */
 }

 TElemType LeftSibling(BiPTree T, TElemType e)
 { 
   /* 初始条件：二叉树T存在，e是T中某个结点 */
   /* 操作结果：返回e的左兄弟。若e是T的左孩子或无左兄弟，则返回＂空＂*/
   BiPTree a;
   
   if(T) /* 非空树 */
   {
     a = Point(T, e); /* a是结点e的指针 */
     if(a && a!=T && a->parent->lchild && a->parent->lchild!=a) /* T中存在结点e且e存在左兄弟 */
     {
       return a->parent->lchild->data; /* 返回e的左兄弟的值 */
     }
   }
   
   return Nil; /* 其余情况返回空 */
 }

 TElemType RightSibling(BiPTree T, TElemType e)
 { 
   /* 初始条件：二叉树T存在，e是T中某个结点 */
   /* 操作结果：返回e的右兄弟。若e是T的右孩子或无右兄弟，则返回＂空＂*/
   BiPTree a;
   
   if(T) /* 非空树 */
   {
     a = Point(T,e); /* a是结点e的指针 */
     if(a && a!=T && a->parent->rchild && a->parent->rchild!=a) /* T中存在结点e且e存在右兄弟 */
     {
       return a->parent->rchild->data; /* 返回e的右兄弟的值 */
     }
   }
   
   return Nil; /* 其余情况返回空 */
 }

 Status InsertChild(BiPTree p, int LR, BiPTree c) /* 形参T无用 */
 { 
   /* 初始条件：二叉树T存在，p指向T中某个结点，LR为0或1，非空二叉树c与T不相交且右子树为空 */
   /* 操作结果：根据LR为0或1，插入c为T中p所指结点的左或右子树。p所指结点 */
   /*           的原有左或右子树则成为c的右子树 */
   if(p) /* p不空 */
   {
     if(LR == 0)
     {
       c->rchild = p->lchild;
       if(c->rchild) /* c有右孩子(p原有左孩子) */
       {
         c->rchild->parent = c;
       }
	   p->lchild = c;
       c->parent = p;
     }
     else /* LR==1 */
     {
       c->rchild = p->rchild;
       if(c->rchild) /* c有右孩子(p原有右孩子) */
       {
         c->rchild->parent = c;
       }
	   p->rchild = c;
       c->parent = p;
     }
     
     return OK;
   }
   
   return ERROR; /* p空 */
 }

 Status DeleteChild(BiPTree p, int LR) /* 形参T无用 */
 { 
   /* 初始条件：二叉树T存在，p指向T中某个结点，LR为0或1 */
   /* 操作结果：根据LR为0或1，删除T中p所指结点的左或右子树 */
   if(p) /* p不空 */
   {
     if(LR == 0) /* 删除左子树 */
     {
       ClearBiTree(&p->lchild);
     }
	 else /* 删除右子树 */
	 {
       ClearBiTree(&p->rchild);
	 }
	 return OK;
   }
   return ERROR; /* p空 */
 }

 void PreOrderTraverse(BiPTree T, void(*Visit)(BiPTree))
 { /* 先序递归遍历二叉树T */
   if(T)
   {
     Visit(T); /* 先访问根结点 */
     PreOrderTraverse(T->lchild, Visit); /* 再先序遍历左子树 */
     PreOrderTraverse(T->rchild, Visit); /* 最后先序遍历右子树 */
   }
 }

 void InOrderTraverse(BiPTree T, void(*Visit)(BiPTree))
 { /* 中序递归遍历二叉树T */
   if(T)
   {
     InOrderTraverse(T->lchild, Visit); /* 中序遍历左子树 */
     Visit(T); /* 再访问根结点 */
     InOrderTraverse(T->rchild, Visit); /* 最后中序遍历右子树 */
   }
 }

 void PostOrderTraverse(BiPTree T, void(*Visit)(BiPTree))
 { /* 后序递归遍历二叉树T */
   if(T)
   {
     PostOrderTraverse(T->lchild, Visit); /* 后序遍历左子树 */
     PostOrderTraverse(T->rchild, Visit); /* 后序遍历右子树 */
     Visit(T); /* 最后访问根结点 */
   }
 }

 void LevelOrderTraverse(BiPTree T, void(*Visit)(BiPTree))
 { /* 层序遍历二叉树T(利用队列) */
   LinkQueue q;
   QElemType a;
   if(T)
   {
     InitQueue(&q);
     EnQueue(&q, T);
     while(! QueueEmpty(q))
     {
       DeQueue(&q, &a);
       Visit(a);
       if(a->lchild != NULL)
       {
         EnQueue(&q, a->lchild);
       }
	   if(a->rchild!=NULL)
	   {
         EnQueue(&q, a->rchild);
	   }
	 }//while
   }//if
 }
 
 void visitT(BiPTree T)
 {
   if(T) /* T非空 */
   {
     printf(form" ", T->data);
   }
 }
 
 