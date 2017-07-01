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
 typedef int SElemType;//定义栈元素的类型
 typedef SElemType ElemType;//栈结点类型和链表结点类型一致
  
  
 typedef struct LNode 
 {
 	ElemType data;
 	struct LNode *next;
 };
 
 typedef struct LNode *LinkList;//另一种定义方法
 typedef LinkList LinkStack;//LinkStack是指向栈结点的指针类型
 #define InitStack InitList // InitStack()与InitList()作用相同
 #define DestroyStack DestroyList //DestroyStack()与DestroyList()作用相同
 #define ClearStack ClearList
 #define StackEmpty ListEmpty
 #define StackLength ListLength
 #define DestroyList ClearList
 
 void InitList(LinkList *L);
 void ClearList(LinkList *L);
 Status ListEmpty(LinkList L);
 int ListLength(LinkList L);
 Status GetElem(LinkList L, int i, ElemType *e);
 int LocateElem(LinkList L, ElemType e, Status(*coompare)(ElemType, ElemType));
 Status ListInsert(LinkList *L, int i, ElemType e);
 Status ListDelete(LinkList *L, int i, ElemType *e);
 void ListTraverse(LinkList L, void(*vi)(ElemType));
 Status GetTop(LinkStack S, SElemType *e);
 Status Push(LinkStack *S, SElemType e);
 Status Pop(LinkStack *S, SElemType *e);
 void StackTraverse(LinkStack S, void(*visit)(SElemType));
 void print(SElemType c);
 
 int main()
 {
   int j;
   LinkStack s;
   SElemType e;
   InitStack(&s); /* 初始化栈s */
   for(j=1;j<=5;j++) /* 将2,4,6,8,10入栈 */
     Push(&s,2*j);
   printf("栈中的元素从栈底到栈顶依次为: ");
   StackTraverse(s,print);
   Pop(&s,&e);
   printf("弹出的栈顶元素为%d\n",e);
   printf("栈空否: %d(1:空 0:否)\n",StackEmpty(s));
   GetTop(s,&e);
   printf("当前栈顶元素为%d，栈的长度为%d\n",e,StackLength(s));
   ClearStack(&s);
   printf("清空栈后，栈空否: %d(1:空 0:否)，栈的长度为%d\n",StackEmpty(s),StackLength(s));
   DestroyStack(&s);
 	
 	return 0;
 }
 
 
 void InitList(LinkList *L)
 {
 	//操作结果：构造一个空的线性表L
	*L = NULL;//指针为空 
 } 
 
 void ClearList(LinkList *L)
 {
 	//初始条件：线性表L已存在。操作结果： 将 L重置为空表 
 	LinkList p;
	
	while (*L)//L不为空
	{
		p = *L;//p指向首元结点
		*L = (*L)->next;//L指向第2个结点(新首元结点)
		free(p);//释放首元结点 
	} 
 } 
 
 Status ListEmpty(LinkList L)
 {
 	//初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回 FALSE
	if (L)
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
 	//初始条件：线性表L已存在。操作结果：返回L中数据元素个数
	int i = 0;
	LinkList p = L;
	
	while (p)//p指向结点(没到表尾)
	{
		p = p->next;//p指向下一个结点 
		i++; 
	} 
	
	return i; 
 }
 
 Status GetElem(LinkList L, int i, ElemType *e)
 {
 	//L为不带头结点的单链表的头指针。当第i个元素存在时，其值赋给e并返回OK，
	//否则返回ERROR
	int j = 1;
	LinkList p = L;
	
	if (i < 1)//i值不合法 
	{
		return ERROR; 
	} 
	while ( (j<i) && p )//没到第i个元素，也没到表尾
	{
		j++;
		p = p->next; 
	} 
	if (j == i)//存在第i个元素
	{
		*e = p->data;
		return OK; 
	} 
	else
	{
		return ERROR; 
	}	 
 }
 
 int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType, ElemType))
 {
 	//初始条件：线性表L已存在，compare()是数据元素判定函数（满足为1，否则为0）
	//操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。
	//若这样的 数据元素不存在，则返回值为0
	int i = 0;
	LinkList p = L;
	
	while (p)
	{
		i++;
		if (compare(p->data, e))//找到这样的数据元素
		{
			return i; 
		} 
		p = p->next;
	} 
	
	return 0;
 }
 
 Status ListInsert(LinkList *L, int i, ElemType e)
 {
 	//在不带头结点的单链线性表L中第i个位置之前插入元素e
    int j = 1;
	LinkList p = *L, s;
	
	if (i < 1)//i值不合法
	{
		return ERROR; 
	}	 
	s = (LinkList)malloc(sizeof(struct LNode));//生成新节点
	s->data = e;//给s的data域赋值
	if (i == 1)//插在表头 
	{
		s->next = *L;
		*L = s;//改变L 
	} 
	else
	{
		//插在表的其余处
		while ( p && (j < i-1) )//寻找第i-1个结点
		{
			p = p->next; 
			j++;
		} 
		
		if (! p)//i大于表长+1
		{
			return ERROR;
		} 
		s->next = p->next;
		p->next = s;	
	}//else
	
	return OK; 
 }
 
 Status ListDelete(LinkList *L, int i, ElemType *e)
 {
 	//在不带头结点的单链线性表L中，删除第i个元素，，并由e返回其值
	int j = 1;
	LinkList p = *L, q;
	
	if (1 == i)//删除第1个结点
	{
		*L = p->next;//L由第二个结点开始
		*e = p->data;
		free(p);//删除并释放第1个结点 
	} 
	else
	{
		while ( (p->next) && (j < i-1))//寻找第i个结点，并令p指向其前驱
		{
			p = p->next;
			j++; 
		} 
		if ( (!p->next) || (j > i-1) )//删除位置不合理 
		{
			return ERROR; 
		} 
		q = p->next;//删除并释放结点
		p->next = q->next;
		*e = q->data;
		free(q); 
	} //else
	
	return OK;	
 }
 
 void ListTraverse(LinkList L, void(*vi)(ElemType))
 {
 	//初始条件：线性表L已存在。操作结果：依次 对L的每个数据元素调用函数 vi()
	LinkList p = L;
	
	while (p)
	{
		vi(p->data);
		p = p->next;
	} 
	printf("\n");
 }
 
 Status GetTop(LinkStack S, SElemType *e)
 {
 	//若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR
	return GetElem(S, 1, e); 
 }
 
 Status Push(LinkStack *S, SElemType e)
 {
 	//插入元素e为新的栈顶元素 
 	return ListInsert(S, 1, e); 
 }
 
 Status Pop(LinkStack *S, SElemType *e)
 {
 	//若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
	return ListDelete(S, 1, e); 
 }
 
 void StackTraverse(LinkStack S, void(*visit)(SElemType))
 {
 	//从栈底到栈顶依次对栈中每个元素调用函数visit()
	LinkStack temp, p = S;//p指向栈顶元素
	
	InitStack(&temp);//初始化临时栈temp
	
	while (p)
	{
		Push(&temp, p->data);//由S栈顶到栈底，依次将栈元素入栈到temp栈
		p = p->next; 
	} 
	
	ListTraverse(temp, visit);//遍历temp线性表 
 }
 
 void print(SElemType c)
 {
 	printf("%d ", c);
 } 
 
 
 
 
 
 
 
  
  