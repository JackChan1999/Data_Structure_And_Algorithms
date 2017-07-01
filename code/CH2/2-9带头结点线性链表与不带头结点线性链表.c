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
 typedef int ElemType;
 
 typedef struct LNode 
 {
 	ElemType data;
	struct LNode *next; 
 };
 typedef struct LNode *LinkList;//另一种定义LinkList的方法
 
 #define DestroyList ClearList//DestroyList()和 ClearList()的操作是一样的
 
 void InitList(LinkList *L); 
 void ClearList(LinkList *L);
 Status ListEmpty(LinkList L);
 int ListLength(LinkList L);
 Status GetElem(LinkList L, int i, ElemType *e);
 int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType, ElemType));
 Status ListInsert(LinkList *L, int i, ElemType e);
 Status ListDelete(LinkList *L, int i, ElemType *e);
 void ListTraverse(LinkList L, void(*vi)(ElemType));
 Status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e);
 Status NextElem(LinkList L, ElemType cur_e, ElemType *next_e);
 Status equal(ElemType c1,ElemType c2); 
 int comp(ElemType a,ElemType b);
 void print(ElemType c);
 void print2(ElemType c);
 void print1(ElemType *c);
 
 int main()
 {
 	LinkList L;
   ElemType e,e0;
   Status i;
   int j,k;
   InitList(&L);
   for(j=1;j<=5;j++)
   {
     i=ListInsert(&L,1,j);
     if(!i) /* 插入失败 */
       exit(ERROR);
   }
   printf("在L的表头依次插入1～5后：L=");
   ListTraverse(L,print); /* 依次对元素调用print()，输出元素的值 */
   i=ListEmpty(L);
   printf("L是否空：i=%d(1:是 0:否)\n",i);
   ClearList(&L);
   printf("清空L后：L=");
   ListTraverse(L,print);
   i=ListEmpty(L);
   printf("L是否空：i=%d(1:是 0:否)\n",i);
   for(j=1;j<=10;j++)
     ListInsert(&L,j,j);
   printf("在L的表尾依次插入1～10后：L=");
   ListTraverse(L,print);
   i=GetElem(L,5,&e);
   if(i==OK)
     printf("第5个元素的值为：%d\n",e);
   for(j=0;j<=1;j++)
   {
     k=LocateElem(L,j,equal);
     if(k)
       printf("第%d个元素的值为%d\n",k,j);
     else
       printf("没有值为%d的元素\n",j);
   }
   for(j=1;j<=2;j++) /* 测试头两个数据 */
   {
     GetElem(L,j,&e0); /* 把第j个数据赋给e0 */
     i=PriorElem(L,e0,&e); /* 求e0的前驱 */
     if(i==INFEASIBLE)
       printf("元素%d无前驱\n",e0);
     else
       printf("元素%d的前驱为：%d\n",e0,e);
   }
   for(j=ListLength(L)-1;j<=ListLength(L);j++) /* 最后两个数据 */
   {
     GetElem(L,j,&e0); /* 把第j个数据赋给e0 */
     i=NextElem(L,e0,&e); /* 求e0的后继 */
     if(i==INFEASIBLE)
       printf("元素%d无后继\n",e0);
     else
       printf("元素%d的后继为：%d\n",e0,e);
   }
   k=ListLength(L); /* k为表长 */
   for(j=k+1;j>=k;j--)
   {
     i=ListDelete(&L,j,&e); /* 删除第j个数据 */
     if(i==ERROR)
       printf("删除第%d个元素失败\n",j);
     else
       printf("删除第%d个元素成功，其值为：%d\n",j,e);
   }
   printf("依次输出L的元素：");
   ListTraverse(L,print);
   DestroyList(&L);
   printf("销毁L后：L=%u\n",L);
 	return 0;
 }
 
 void InitList(LinkList *L)
 {
 	//操作结果：构造一个空的线性表L
	*L = NULL;//指针为空 
 } 
 
 void ClearList(LinkList *L)
 {
 	//初始条件：线性表L已存在。操作结果：将L重置为空表
	LinkList p;
	
	while (*L)//L不空
	{
		p = *L;//p指向首元结点
		*L = (*L)->next;//L指向第2个结点（新首元结点）
		free(p);//释放首元结点 
	}//while 
 }
 
 Status ListEmpty(LinkList L)
 {
 	//初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE
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
 	//初始条件：线性表L已存在 。操作结果：返回L中数据元素个数
	int i = 0;
	LinkList p = L;
	
	while (p)//p指向结点（没到表尾）
	{
		p = p->next;//p指向下一个结点
		i++; 
	} 
	
	return i;	
 }
 
 Status GetElem(LinkList L, int i, ElemType *e)
 {
 	//L为不带头结点的单链表的头指针。当第i个元素存在时，
    //其值赋给e并返回OK，否则返回ERROR
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
	//操作结果：返回L中第1个与e满足 关系compare()的数据元素的位序
	//若这样的数据元素不存在，则返回值为0
	
	int i = 0;
	LinkList p = L;
	
	while (p)
	{
		i++;
		
		if ( compare(p->data, e) )//找到这样的数据元素
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
	
	if (1 == i)
	{
		s->next = *L;
		*L = s;	
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
	} 
	
	return OK;	
 }
 
 Status ListDelete(LinkList *L, int i, ElemType *e)
 {
 	//在不带头结点的单链线性表L中，删除第i个元素，并由e返回其值
	int j = 1;
	LinkList p = *L, q;
	
	if (1 == i)//删除第1个结点
	{
		*L = p->next;//L由第2个结点开始
		*e = p->data;
		free(p);//删除并释放第1个结点 
	} 
	else
	{
		while ( (p->next) && (j < i-1) )//寻找第i个结点，并令p指向其前驱 
		{
			p = p->next;
			j++;	
		}//while
		if ( (!p->next) || (j > i-1) )//删除位置不合理
		{
			return ERROR; 
		} 
		q = p->next;//删除并释放结点
		p->next = q->next;
		*e = q->data;
		
		free(q); 	
	}//else 
	
	return OK;
 }
 
 void ListTraverse(LinkList L, void(*vi)(ElemType))
 {
 	//初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi()
	LinkList p = L;
	
	while (p)
	{
		vi(p->data);
		p = p->next;	
	} 
	printf("\n");	
 }
 
 Status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e)
 {
 	//初始条件：线性表L已存在
	//操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，
	//返回OK；否则操作失败，pre_e无定义，返回INFEASIBLE
	LinkList q, p = L;//p指向第一个结点 
	
	while (p->next)//p所指结点有后继
	{
		q = p->next;/*q为p的后继*/
		
		if (q->data == cur_e)
		{
			*pre_e = p->data;
			return OK;	
		} 
		p = q;//p向后移 
	} 
	
	return INFEASIBLE; 
 }
 
 Status NextElem(LinkList L, ElemType cur_e, ElemType *next_e)
 {
 	//初始条件：线性表L已存在
	//操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继，
	//返回OK；否则操作失败，next_e无定义，返回INFEASIBLE
	LinkList p = L;//p指向第一个结点
	
	while ( p->next )//p所指结点有后继
	{
		if (p->data == cur_e)
		{
			*next_e = p->next->data;
			return OK;	
		} 
		p = p->next;
	}//while 
	
	return INFEASIBLE;	
 }
 
 Status equal(ElemType c1,ElemType c2)
 { /* 判断是否相等的函数 */
   if(c1==c2)
     return TRUE;
   else
     return FALSE;
 }

 int comp(ElemType a,ElemType b)
 { /* 根据a<、=或>b，分别返回-1、0或1 */
   if(a==b)
     return 0;
   else
     return (a-b)/abs(a-b);
 }

 void print(ElemType c)
 {
   printf("%d ",c);
 }

 void print2(ElemType c)
 {
   printf("%c ",c);
 }

 void print1(ElemType *c)
 {
   printf("%d ",*c);
 }