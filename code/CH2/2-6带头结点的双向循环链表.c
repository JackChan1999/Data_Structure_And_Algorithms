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
 
 typedef struct DuLNode
 {
 	ElemType data;
 	struct DuLNode *prior, *next;
 }DuLNode, *DuLinkList;
 
  void InitList(DuLinkList *L);
  void DestroyList(DuLinkList *L);
  void ClearList(DuLinkList L);
  Status ListEmpty(DuLinkList L);
  int ListLength(DuLinkList L);
  Status GetElem(DuLinkList L, int i, ElemType *e);
  int LocateElem(DuLinkList L, ElemType e, Status(*compare)(ElemType, ElemType));
  Status PriorElem(DuLinkList L, ElemType cur_e, ElemType *pre_e);
  Status NextElem(DuLinkList L, ElemType cur_e, ElemType *next_e);
  DuLinkList GetElemP(DuLinkList L, int i);
  Status ListInsert(DuLinkList L, int i, ElemType e);
  Status ListDelete(DuLinkList L, int i, ElemType *e);
  void ListTraverse(DuLinkList L, void(*visit)(ElemType));
  void ListTraverseBack(DuLinkList L, void(*visit)(ElemType));
  Status equal(ElemType c1, ElemType c2);
  int comp(ElemType a, ElemType b);
  void print(ElemType c);
  void print2(ElemType c);
  void print1(ElemType *c); 
 
 int main()
 {
   DuLinkList L;
   int i,n;
   Status j;
   ElemType e;
   
   InitList(&L);
   
   for(i=1;i<=5;i++)
   { 
	 ListInsert(L,i,i); /* 在第i个结点之前插入i */
   }
   printf("正序输出链表：");
   ListTraverse(L,print); /* 正序输出 */
   printf("逆序输出链表：");
   ListTraverseBack(L,print); /* 逆序输出 */
   n=2;
   ListDelete(L,n,&e); /* 删除并释放第n个结点 */
   printf("删除第%d个结点，值为%d，其余结点为：",n,e);
   ListTraverse(L,print); /* 正序输出 */
   printf("链表的元素个数为%d\n",ListLength(L));
   printf("链表是否空：%d(1:是 0:否)\n",ListEmpty(L));
   ClearList(L); /* 清空链表 */
   printf("清空后，链表是否空：%d(1:是 0:否)\n",ListEmpty(L));
   for(i=1;i<=5;i++)
   {
     ListInsert(L,i,i); /* 重新插入5个结点 */
   }
   ListTraverse(L,print); /* 正序输出 */
   n=3;
   j=GetElem(L,n,&e); /* 将链表的第n个元素赋值给e */
   if(j)
   {
     printf("链表的第%d个元素值为%d\n",n,e);
   }
   else
   {
     printf("不存在第%d个元素\n",n);
   }
   n=4;
   i=LocateElem(L,n,equal);
   if(i)
   {
     printf("等于%d的元素是第%d个\n",n,i);
   }
   else
   {
     printf("没有等于%d的元素\n",n);
   }
   j=PriorElem(L,n,&e);
   if(j)
   {
     printf("%d的前驱是%d\n",n,e);
   }
   else
   {
     printf("不存在%d的前驱\n",n);
   }
   j=NextElem(L,n,&e);
   if(j)
   {
     printf("%d的后继是%d\n",n,e);
   }
   else
   {
     printf("不存在%d的后继\n",n);
   }
   DestroyList(&L);
   
   return 0;
 }
 
 void InitList(DuLinkList *L)
 {
 	//产生空的双向循环链表L
	 *L = (DuLinkList)malloc(sizeof(DuLNode));
	 
	 if (*L)
	 {
	 	(*L)->next = (*L)->prior = *L;
	 }
	 else
	 {
	 	exit(OVERFLOW);
	 }  
 }
 
 void DestroyList(DuLinkList *L)
 {
 	//操作结果：销毁双向循环链表L
    DuLinkList q, p = (*L)->next;//p指向第一个结点
	
	while (p != *L)//p没到表头
	{
		q = p->next;
		free(p);
		p = q; 
	} 
	free(*L);
	*L = NULL; 
 }
 
 void ClearList(DuLinkList L)//不改变L
 {
 	//初始条件：L已存在。操作结果：将L重置为空表 
 	DuLinkList q, p = L->next;//p指向第一个结点
	
	while (p != L)//p没到表头
	{
		q = p->next;
		free(p);
		p = q;
	}
	
	L->next = L->prior = L;//头结点的两个指针域均指向自身 
 }
 
 Status ListEmpty(DuLinkList L)
 {
 	//初始条件：线性表L已存在。
    //操作结果：若L为空表，则返回TRUE，否则返回FALSE
	if ( (L->next==L) && (L->prior==L) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
 }
 
 int ListLength(DuLinkList L)
 {
 	//初始条件：L已存在。操作结果：返回L中数据元素个数
	int i = 0;
	
	DuLinkList p = L->next;//p指向第一个结点
	
	while (p != L)//p没到表头
	{
		i++;
		p = p->next; 
	} 
	
	return i;	 
 }
 
 Status GetElem(DuLinkList L, int i, ElemType *e)
 {
 	//当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR
 	int j = 1;//j为计数器
	DuLinkList p = L->next;//p指向第一个结点
	
	while ( (p!=L) && (j<i) )
	//顺指针向后查找,直到p指向第i个元素或p指向头结点 
 	{
 		p = p->next;
		j++; 
    } 
    
    if ( (p==L) || (j>i) )//第i个元素不存在 
    {
    	return ERROR; 	
    }
    
    *e = p->data;//取第i个元素
	
	return OK;	 
 }
 
 int LocateElem(DuLinkList L, ElemType e, Status(*compare)(ElemType, ElemType))
 {
 	//初始条件：L已存在，compare()是数据元素判定函数
	//操作结果：返回L中第1个与e满足关系compare()的数据元素的位序
	//若这样的数据元素不存在，则返回值为0 
	int i = 0;
	DuLinkList p = L->next;//p指向第i个元素
	
	while (p != L)
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
 
 Status PriorElem(DuLinkList L, ElemType cur_e, ElemType *pre_e)
 {
 	//操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，
	//否则操作失败，pre_e无定义
	DuLinkList p = L->next->next;//p指向第2个元素
	
	while (p != L)//p没到表头
	{
		if (p->data == cur_e)
		{
			*pre_e = p->prior->data;
			return TRUE;
		}
		p = p->next;
	}  
	
	return FALSE;
 }
 
 Status NextElem(DuLinkList L, ElemType cur_e, ElemType *next_e)
 {
 	//操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继，
	//否则操作失败，next_e无定义
	DuLinkList p = L->next->next;//p指向第2个元素
	while (p != L)//p没到表头 
	{
		if (p->prior->data == cur_e)
		{
			*next_e = p->data;
			return TRUE;
		}
		p = p->next;	
	}//while
	
	return FALSE;
 }
 
 DuLinkList GetElemP(DuLinkList L, int i)//另外加的
 {
 	//在双向链表L中返回第i个元素的地址。
	//i为0，返回头结点的地址。若第i个元素不存在
	//返回NULL
	int j;
	
	DuLinkList p = L;//p指向头结点
	
	if ( (i<0) || (i>ListLength(L)) )//i值不合法
	{
		return NULL; 
	} 
	
	for (j=1; j<=i; j++)
	{
		p = p->next;
	}
	
	return p;	 
 }
 
 Status ListInsert(DuLinkList L, int i, ElemType e)
 {
 	//在带头结点的双循环线性表L中第i个位置之前插入元素e，
	//i的合法值为1<=i<=表长+1
	//对书上的进行了改进否则无法再第表长+1个结点之前插入元素
	DuLinkList p, s;
	
	if ( (i<1) || (i > ListLength(L)+1))//i值不合法
	{
		return ERROR; 
	} 
	
	p = GetElemP(L, i-1);//在L中确定第i个元素前驱的位置指针p
	if (!p)//p=NULL,即第i个元素的前驱不存在（设头结点为第1个元素的前驱） 
	{
		return ERROR;
	}
	s = (DuLinkList)malloc(sizeof(DuLNode));
	if (!s)
	{
		return OVERFLOW;
	}
	s->data = e;
	s->prior = p;//在第i-1个元素之后插入
	s->next = p->next;
	p->next->prior = s;
	p->next = s;
	
	return OK; 	
 }
 
 Status ListDelete(DuLinkList L, int i, ElemType *e)
 {
 	//删除带头结点的双链循环线性表L的第i个元素，
    //i的合法值为1<=i<=表长
	DuLinkList p;
	if (i<1)//i值不合法
	{
		return ERROR; 
	} 
	p = GetElemP(L, i);//在L中确定第i个元素的位置指针p
	if (!p)//p=NULL，即第i个元素不存在
	{
		return ERROR; 
	} 
	*e = p->data;
	p->prior->next = p->next;
	p->next->prior = p->prior;
	free(p);
	
	return OK;
 }
 
 void ListTraverse(DuLinkList L, void(*visit)(ElemType))
 {
 	//由双循环线性表L的头结点出发，正序对每个数据元素调用函数visit()
	DuLinkList p = L->next;//p指向头结点
	
	while (p != L)
	{
		visit(p->data);
		p = p->next;
	}
	printf("\n");	
 }
 
 void ListTraverseBack(DuLinkList L, void(*visit)(ElemType))
 {
 	//由双链循环线性表L的头结点出发，逆序对每个数据元素调用函数visit()。
	//另加
	DuLinkList p = L->prior;//p指向尾结点
	
	while (p != L)
	{
		visit(p->data);
		p = p->prior;
	}
	printf("\n");	 
 }
 
 Status equal(ElemType c1, ElemType c2)
 {
 	//判断是否相等的函数
	 if (c1 == c2)
	 {
	 	return TRUE;
	 }
	 else
	 {
	 	return FALSE;
	 }
 }
 
 int comp(ElemType a, ElemType b)
 {
 	//根据a<、=或>b，分别返回-1、0或1
 	if (a == b)
 	{
 		return 0;
 	}
 	else
 	{
 		return (a-b) / abs(a-b);
 	}
 }
 
 void print(ElemType c)
 {
 	printf("%d ", c);
 }
 
 void print2(ElemType c)
 {
 	printf("%c ", c);
 }
 
 void print1(ElemType *c)
 {
 	printf("%d ", *c);
 }
 
 
 
 
 
 
 
 
  

