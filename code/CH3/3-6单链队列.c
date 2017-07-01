 //用单链表的基本操作实现链队列 
 
 #include<string.h>
 #include<ctype.h>
 #include<malloc.h> /* malloc()等 */
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
 typedef int QElemType;
 typedef struct QNode
 {
 	QElemType data;
	struct QNode *next; 
 } QNode, *QueuePtr;
 
 typedef struct 
 {
 	QueuePtr front, rear;//队头，队尾指针
 }LinkQueue; 
 typedef QElemType ElemType;
 #define LinkList QueuePtr  //定义单链表的类型与相应的链队列的类型相同
 #define LNode QNode
 
 void InitList(LinkList *L);
 void DestroyList(LinkList *L);
 void ClearList(LinkList L);
 Status ListEmpty(LinkList L);
 int ListLength(LinkList L);
 Status GetElem(LinkList L, int i, ElemType *e);
 int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType, ElemType));
 Status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e);
 Status NextElem(LinkList L, ElemType cur_e, ElemType *next_e);
 Status ListInsert(LinkList L, int i, ElemType e);
 Status ListDelete(LinkList L, int i, ElemType *e);
 void ListTraverse(LinkList L, void (*vi)(ELemType));
 void InitQueue(LinkQueue *Q);
 void DestroyQueue(LinkQueue *Q);
 void ClearQueue(LinkQueue *Q);
 Status QueueEmpty(LinkQueue Q);
 int QueueLength(LinkQueue Q);
 Status GetHead_Q(LinkQueue Q, QElemType *e);
 void EnQueue(LinkQueue *Q, QElemType e);
 Status DeQueue(LinkQueue *Q, QElemType *e);
 void QueueTraverse(LinkQueue Q, void(*vi)(QElemType));
 void print(QElemType i);
 
 int main()
 {
   int i;
   QElemType d;
   LinkQueue q;
   InitQueue(&q);
   printf("成功地构造了一个空队列!\n");
   printf("是否空队列？%d(1:空 0:否)  ",QueueEmpty(q));
   printf("队列的长度为%d\n",QueueLength(q));
   EnQueue(&q,-5);
   EnQueue(&q,5);
   EnQueue(&q,10);
   printf("插入3个元素(-5,5,10)后,队列的长度为%d\n",QueueLength(q));
   printf("是否空队列？%d(1:空 0:否)  ",QueueEmpty(q));
   printf("队列的元素依次为：");
   QueueTraverse(q,print);
   i=GetHead_Q(q,&d);
   if(i==OK)
     printf("队头元素是：%d\n",d);
   DeQueue(&q,&d);
   printf("删除了队头元素%d\n",d);
   i=GetHead_Q(q,&d);
   if(i==OK)
     printf("新的队头元素是：%d\n",d);
   ClearQueue(&q);
   printf("清空队列后,q.front=%u q.rear=%u q.front->next=%u\n",q.front,q.rear,q.front->next);
   DestroyQueue(&q);
   printf("销毁队列后,q.front=%u q.rear=%u\n",q.front, q.rear);
   	
   return 0;
 }
 
 
 void InitList(LinkList *L)
{
	/*操作结果：构造一个空的线性表*/ 
	*L = (LinkList)malloc(sizeof(struct LNode));/*产生头结点，并使L指向此头结点*/ 
	
	if (!*L)
	{
		exit(OVERFLOW);/*存储分配失败*/ 
	}
	(*L)->next = NULL;/*指针域为空*/	
}

void DestroyList(LinkList *L)
{
	/*初始条件：线性表L已存在。操作结果：销毁线性表L*/
	LinkList q;
	
	while (*L)
	{
		q = (*L)->next;
		free(*L);
		*L = q;
	}
}

void ClearList(LinkList L)/*不改变L*/
{
	LinkList p, q;
	
	p = L->next;/*p指向第一个节点*/
	while (p)/*没到尾表*/
	{
		q = p->next;
		free(p);
		p = q;
	}	
	
	L->next = NULL;/*头结点指针域为空*/
}

Status ListEmpty(LinkList L)
{
	/*初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE*/
	if (L->next)/*非空*/
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int ListLength(LinkList L)
{
	/*初始条件：线性表L已存在。操作结果：返回L中数据元素个数*/
	int i = 0;
	LinkList p = L->next;/*p指向第一个结点*/
	
	while (p)/*没到表尾*/
	{
		i++;
		p = p->next;
	}
	return i; 	
}

Status GetElem(LinkList L, int i, ElemType *e)
{
	/*L为带头结点的单链表的头指针。当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR*/
	int j = 1;/*j为计数器*/
	LinkList p = L->next;/*p指向第一个结点*/
	
	while (p && j<i)/*顺指针向后查找，直到p指向第i个元素或p为空*/
	{
		p = p->next;
		j++;
	}
	
	if (!p || j>i)/*第i个元素不存在*/
	{
		return ERROR;
	}
	
	*e = p->data;/*取第i个元素*/
	
	return OK; 	
}

int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType, ElemType))
{
	/*初始条件：线性表L已存在，compare()是数据元素判定函数（满足为1，否则为0）*/
	/*操作结果：返回L中第1个与e满足关系compare（）的数据元素的位序。*/
	/*若这样的数据元素不存在，则返回值为0*/
	
	int i = 0;
	LinkList p = L->next;
	
	while (p)
	{
		i++;
		if ( compare(p->data, e) )/*找到这样的元素*/
		{
			return i;
		}
		p = p->next;  
	}
	
	return 0;
}

Status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e)
{
	/*初始条件：线性表L已存在*/
	/*操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，*/
	/*           返回OK；否则操作失败，pre_e无定义，返回INFEASIBLE*/
	LinkList q, p = L->next;/*p指向第一个结点*/
	
	while (p->next)/*p所指结点有后继*/ 
	{
		q = p->next;/*q为p的后继*/
		
		if (q->data == cur_e)
		{
			*pre_e = p->data;
			return OK;
		}
		p = q;/*p向后移*/
	}
	return INFEASIBLE; 	 
}

Status NextElem(LinkList L, ElemType cur_e, ElemType *next_e)
{
	/*初始条件：线性表L已存在*/
	/*操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继，*/
	/*          返回OK；否则操作失败，next_e无定义，返回INFEASIBLE*/
	LinkList p = L->next;/*p指向第一个结点*/
	
	while (p->next)/*p所指结点有后继*/
	{
		if (p->data == cur_e)
		{
			*next_e = p->next->data;
			return OK;
		}
		p = p->next;
	} 
	
	return INFEASIBLE;
}

Status ListInsert(LinkList L, int i, ElemType e)/*不改变L*/
{
	/*在带头结点的单链线性表L中第i个位置之前插入元素e*/
	int j = 0;
	LinkList p = L, s;
	
	while ( p && j<(i-1) )/*寻找第i-1个结点*/ 
	{
		p = p->next;
		j++;
	}
	
	if ( !p || j>(i-1) )/*i小于1或者大于表长*/
	{
		return ERROR; 
	} 
	s = (LinkList)malloc( sizeof(struct LNode) );/*生成新结点*/
	s->data = e;/*插入L中*/
	s->next = p->next;
	p->next = s;
	
	return OK; 	 
}

Status ListDelete(LinkList L, int i, ElemType *e)/*不改变L*/
{
	/*在带头结点的单链线性表L中，删除第i个元素，并由e返回其值*/
	int j = 0;
	LinkList p = L, q;
	
	while ( p->next && j>(i-1) )/*寻找第i个结点，并令p指向其前驱*/
	{
		p = p->next;
		j++;		 
	}
	
	if ( (!p->next) || j>(i-1) )/*删除位置不合理*/
	{
		return ERROR;
	} 
	
	q = p->next;/*删除并释放结点*/
	p->next = q->next;
	*e = q->data;
	free(q); 
	
	return OK;
}

void ListTraverse(LinkList L, void (*vi)(ELemType))
{
	/*vi的形参类型为ElemType*/
	/*初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi()*/
	
	LinkList p = L->next;
	
	while (p)
	{
		vi(p->data);
		p = p->next;
	}
	
	printf("\n");
}
 
 void InitQueue(LinkQueue *Q)
 {
 	//构造一个空队列Q
	InitList(&(*Q).front);//调用单链表的基本操作 
	(*Q).rear = (*Q).front; 
 } 
 
 void DestroyQueue(LinkQueue *Q)
 {
 	//销毁队列Q（无论空否均可）
	DestroyList(&(*Q).front);
	(*Q).rear = (*Q).front; 	
 }
 
 void ClearQueue(LinkQueue *Q)
 {
 	//将Q清为空队列 
 	ClearList((*Q).front);
    (*Q).rear = (*Q).front; 
 }
 
 Status QueueEmpty(LinkQueue Q)
 {
 	//若Q为空队列，则返回TRUE，否则返回FALSE
	return ListEmpty(Q.front); 
 }  
 
 int QueueLength(LinkQueue Q)
 {
 	//求队列的长度
	return ListLength(Q.front); 
 }
 
 Status GetHead_Q(LinkQueue Q, QElemType *e)
 {
 	//若队列不空，则用e返回Q的队头元素，并返回OK，否则返回ERROR
	return GetElem(Q.front, 1, e); 
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
 	//若队列不空，删除Q的队头元素，用e返回其值，并返回OK，否则返回ERROR
	if ( ((*Q).front->next) == (*Q).rear )//队列仅有1个元素 （删除的也是队尾元素）
	{
		(*Q).rear = (*Q).front;//令队尾指针指向头结点 
	}
	
	return ListDelete((*Q).front, 1, e); 
 }
 
 void QueueTraverse(LinkQueue Q, void(*vi)(QElemType))
 {
 	//从队头到队尾依次对队列Q中每个元素调用函数vi()
	ListTraverse(Q.front, vi); 
 }
 
 void print(QElemType i)
 {
   printf("%d ",i);
 }
 
  
 
 
  

 
 