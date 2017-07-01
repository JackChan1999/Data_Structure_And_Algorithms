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
 
 typedef struct LNode //结点类型
 {
 	ElemType data;
 	struct LNode *next;
 }LNode, *Link, *Position;
 
 typedef struct LinkList//链表类型
 {
 	Link head, tail;//分别指向线性链表中的头结点和最后一个结点
	int len;//指示线性链表中数据元素的个数 
 }LinkList;
 
 void MakeNode(Link *p, ElemType e);
 void FreeNode(Link *p);
 void InitList(LinkList *L);
 void ClearList(LinkList *L);
 void DestroyList(LinkList *L);
 void InsFirst(LinkList *L, Link h, Link s);//形参增加L，因为需修改L
 Status DelFirst(LinkList *L, Link h, Link *q);//形参增加L，因为需修改L 
 void Append(LinkList *L, Link s);
 Position PriorPos(LinkList L, Link p);
 Status Remove(LinkList *L, Link *q);
 void InsBefore(LinkList *L, Link *p, Link s);
 void InsAfter(LinkList *L, Link *p, Link s);
 void SetCurElem(Link p, ElemType e);
 ElemType GetCurElem(Link p);
 Status ListEmpty(LinkList L);
 int ListLength(LinkList L);
 Position GetHead(LinkList L);
 Position GetLast(LinkList L);
 Position NextPos(Link p);
 Status LocatePos(LinkList L, int i, Link *p);
 Position LocateElem(LinkList L, ElemType e, Status (*compare)(ElemType, ElemType));
 void ListTraverse(LinkList L, void(*visit)(ElemType));
 void OrderInsert(LinkList *L, ElemType e, int (*comp)(ElemType, ElemType));
 Status LocateElemP(LinkList L, ElemType e, Position *q, int(*compare)(ElemType, ElemType));
 Status equal(ElemType c1, ElemType c2);
 int comp(ElemType a, ElemType b);
 void print(ElemType c);
 void print2(ElemType c);
 void print1(ElemType *c);
 
 int main()
 {
   Link p,h;
   LinkList L;
   Status i;
   int j,k;
   
   InitList(&L); /* 初始化空的线性表L */
   
   for(j=1;j<=2;j++)
   {
     MakeNode(&p,j); /* 生成由p指向、值为j的结点 */
     InsFirst(&L,L.tail,p); /* 插在表尾 */
   }
   OrderInsert(&L,0,comp); /* 按升序插在有序表头 */
   for(j=0;j<=3;j++)
   {
     i=LocateElemP(L,j,&p,comp);
     if(i)
     {
       printf("链表中有值为%d的元素。\n",p->data);
     }
	 else
	 {
       printf("链表中没有值为%d的元素。\n",j);
	 }
   }
   printf("输出链表：");
   ListTraverse(L,print); /* 输出L */
   for(j=1;j<=4;j++)
   {
     printf("删除表头结点：");
     DelFirst(&L,L.head,&p); /* 删除L的首结点，并以p返回 */
     if(p)
     {
       printf("%d\n",GetCurElem(p));
     }
	 else
	 {
       printf("表空，无法删除 p=%u\n",p);
	 }
   }
   printf("L中结点个数=%d L是否空 %d(1:空 0:否)\n",ListLength(L),ListEmpty(L));
   MakeNode(&p,10);
   p->next=NULL; /* 尾结点 */
   for(j=4;j>=1;j--)
   {
     MakeNode(&h,j*2);
     h->next=p;
     p=h;
   } /* h指向一串5个结点，其值依次是2 4 6 8 10 */
   Append(&L,h); /* 把结点h链接在线性链表L的最后一个结点之后 */
   OrderInsert(&L,12,comp); /* 按升序插在有序表尾头 */
   OrderInsert(&L,7,comp); /* 按升序插在有序表中间 */
   printf("输出链表：");
   ListTraverse(L,print); /* 输出L */
   for(j=1;j<=2;j++)
   {
     p=LocateElem(L,j*5,equal);
     if(p)
     {
       printf("L中存在值为%d的结点。\n",j*5);
     }
	 else
	 {
       printf("L中不存在值为%d的结点。\n",j*5);
	 }
   }
   for(j=1;j<=2;j++)
   {
     LocatePos(L,j,&p); /* p指向L的第j个结点 */
     h=PriorPos(L,p); /* h指向p的前驱 */
     if(h)
     {
       printf("%d的前驱是%d。\n",p->data,h->data);
     }
	 else
	 {
       printf("%d没前驱。\n",p->data);
	 }
   }
   k=ListLength(L);
   for(j=k-1;j<=k;j++)
   {
     LocatePos(L,j,&p); /* p指向L的第j个结点 */
     h=NextPos(p); /* h指向p的后继 */
     if(h)
     {
       printf("%d的后继是%d。\n",p->data,h->data);
	 }
	 else
	 {
       printf("%d没后继。\n",p->data);
	 }
   }
   printf("L中结点个数=%d L是否空 %d(1:空 0:否)\n",ListLength(L),ListEmpty(L));
   p=GetLast(L); /* p指向最后一个结点 */
   SetCurElem(p,15); /* 将最后一个结点的值变为15 */
   printf("第1个元素为%d 最后1个元素为%d\n",GetCurElem(GetHead(L)->next),GetCurElem(p));
   MakeNode(&h,10);
   InsBefore(&L,&p,h); /* 将10插到尾结点之前，p指向新结点 */
   p=p->next; /* p恢复为尾结点 */
   MakeNode(&h,20);
   InsAfter(&L,&p,h); /* 将20插到尾结点之后 */
   k=ListLength(L);
   printf("依次删除表尾结点并输出其值：");
   for(j=0;j<=k;j++)
   {
     i=Remove(&L,&p);
     if(!i) /* 删除不成功 */
     {
       printf("删除不成功 p=%u\n",p);
     }
	 else
	 {
       printf("%d ",p->data);
	 }

   }
   return 0;
 }
 
 void MakeNode(Link *p, ElemType e)
 {
 	//分配有p指向的值为e的结点。若分配失败，则退出
	*p = (Link)malloc(sizeof(LNode));
	if (!*p)
	{
		exit(ERROR);
	}
	(*p)->data = e;
 }
 
 void FreeNode(Link *p)
 {
 	//释放p所指结点
	 free(*p);
	 *p = NULL; 
 }
 
 void InitList(LinkList *L)
 {
 	//构造一个空的线性链表L
	Link p;
	p = (Link)malloc(sizeof(LNode));//生成头结点
	
	if (p)
	{
		p->next = NULL;
		(*L).head = (*L).tail = p;
		(*L).len = 0;
	}
	else
	{
		exit(ERROR);
	}
 }
 
 void ClearList(LinkList *L)
 {
 	//将线性链表L重置为空表，并释放原链表的结点空间
	Link p, q;
	
	if ( (*L).head != (*L).tail )//不是空表
	{
		p = q = (*L).head->next;
		(*L).head->next = NULL;
		
		while ( p != (*L).tail )
		{
			p = q->next;
			free(q);
			q = p;
		}
		free(q);
		(*L).tail = (*L).head;
		(*L).len = 0;
	}//if 	
 }

 void DestroyList(LinkList *L)
 {
 	//销毁线性链表L，L不再存在
	ClearList(L);//清空链表
	FreeNode( &(*L).head );
	(*L).tail = NULL;
	(*L).len = 0; 
 }
 
 void InsFirst(LinkList *L, Link h, Link s)//形参增加L，因为需修改L
 {
 	//h指向L的一个结点，
    //把h当做头结点，将s所指结点插入在第一个结点之前 
    s->next = h->next;
    h->next = s;
    
    if (h == (*L).tail)//h指向尾结点
	{
		(*L).tail = h->next;//修改尾指针 	
	} 
	(*L).len++; 
 } 
 
 Status DelFirst(LinkList *L, Link h, Link *q)//形参增加L，因为需修改L 
 {
 	//h指向L的一个结点，把h当做头结点，删除链表中的第一个结点并以q返回。
	//若链表为空(h指向尾结点)，q=NULL,返回FALSE
	*q = h->next;
	if (*q)//链表非空 
	{
		h->next = (*q)->next;
		if (!h->next)//删除尾结点 
		{
			(*L).tail = h;//修改尾指针 
		}
		(*L).len--; 
		return OK;
	}
	else
	{
		return FALSE;//链表空 
	}
	
 } 
 
 void Append(LinkList *L, Link s)
 {
 	//将指针s(s->data为第一个数据元素)所指(彼此以指针相连以NULL结尾)的 
    //一串结点链接在线性链表L的最后一个结点之后，并改变链表L的尾指针指向新的尾结点
	int i = 1;
	
	(*L).tail->next = s;
	
	while (s->next)
	{
		s = s->next;
		i++;
	}
	(*L).tail = s;
	(*L).len += i;
 }
 
 Position PriorPos(LinkList L, Link p)
 {
 	//已知p指向线性链表L中的一个结点，返回p所指结点的直接前驱
	//的位置。若无前驱，则返回NULL
	Link q;
	
	q = L.head->next;
	if (q == p)//无前驱 
	{
		return NULL; 
	}
	else
	{
		while (q->next != p)//q不是p的直接前驱 
		{
			q = q->next;
		}
		return q; 
	}	
 } 
 
 Status Remove(LinkList *L, Link *q)
 {
 	//删除线性表L中的尾结点并以q返回，改变链表L的尾指针指向新的尾结点
	Link p = (*L).head;
	
	if ( (*L).len == 0 )//空表
	{
		*q = NULL;
		return FALSE; 
	}
	while (p->next != (*L).tail)
	{
		p = p->next;
	}
	*q = (*L).tail;
	p->next = NULL;
	(*L).tail = p;
	(*L).len--;
	
	return OK;	
 }
 
 void InsBefore(LinkList *L, Link *p, Link s)
 {
 	//已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之前
	//并修改指针p指向新插入的结点 
	Link q;
	
	q = PriorPos(*L, *p);//q是p的前驱
	if (!q)//p无前驱
	{
		q = (*L).head; 
	}
	s->next = *p;
	q->next = s;
	*p = s;
	(*L).len++; 
 }
 
 void InsAfter(LinkList *L, Link *p, Link s)
 {
 	//已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之后
	//并修改指针p指向新插入的结点
    if (*p == (*L).tail)//修改尾指针
	{
		(*L).tail = s; 
	} 
	s->next = (*p)->next;
	(*p)->next = s;
	*p = s;
	(*L).len++;	
 }
 
 void SetCurElem(Link p, ElemType e)
 {
 	//已经p指向线性链表中的一个结点，用e更新p所指结点中数据元素的值
	p->data = e; 
 }
 
 ElemType GetCurElem(Link p)
 {
 	//已知p指向线性链表中的一个结点，返回p所指结点中数据元素的值
	return p->data; 
 }
 
 Status ListEmpty(LinkList L)
 {
 	//若线性链表L为空表，则返回TRUE，否则返回FALSE
	if (L.len)
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
 	//返回线性链表L中元素个数
	return L.len; 
 } 
 
 Position GetHead(LinkList L)
 {
 	//返回线性链表L中头结点的位置
	 return L.head; 
 }
 
 Position GetLast(LinkList L)
 {
 	//返回线性链表L中最后一个结点的位置
	return L.tail; 
 }
 
 Position NextPos(Link p)
 {
 	//已知p指向线性链表L中的一个结点，返回p所指结点的直接后继的位置。
    //若无后继，则返回NULL
	return p->next; 
 }
 
 Status LocatePos(LinkList L, int i, Link *p)
 {
 	//返回p指示线性链表L中第i个结点的位置，并返回OK，
	//i值不合法是返回ERROR。i=0为头结点
	int j;
	
	if ((i<0) || (i>L.len))
	{
		return ERROR;
	}
	else
	{
		*p = L.head;
		for(j=1; j<=i; j++)
		{
			*p = (*p)->next;
		}
		return OK;
	}	 
 }
 
 Position LocateElem(LinkList L, ElemType e, Status (*compare)(ElemType, ElemType))
 {
 	//返回线性链表L中第1个与e满足函数compare()判定关系的元素的位置
	//若不存在这样的元素，则返回NULL
	Link p = L.head;
	do
	{
		p = p->next;
	}
	while( p && (compare(p->data, e)) );//没到表尾且没找到满足关系的元素
	
	return p; 	 
 }
 
 void ListTraverse(LinkList L, void(*visit)(ElemType))
 {
 	//依次对L的每个数据元素调用函数visit()
	Link p = L.head->next;
	int j; 
	
	for (j=1; j<=L.len; j++)
	{
		visit(p->data);
		p = p->next;	
	}
	printf("\n");
 }
 
 void OrderInsert(LinkList *L, ElemType e, int (*comp)(ElemType, ElemType))
 {
 	//已知L为有序线性链表，将元素e按非降序插入在L中。（用于一元多项式）
	Link o, p, q;
	
	q = (*L).head;
	p = q->next;
	
	while ( (p!=NULL) && (comp(p->data, e)<0) )//p不是尾表且元素值小于e 
	{
		q = p;
		p = p->next;
	}
	o = (Link)malloc(sizeof(LNode));//生成结点 
	o->data = e;//赋值 
	q->next = o;//插入
	o->next = p;
	(*L).len++;//表长加1
	if(!p)//插在表尾
	{
		(*L).tail = o;//修改尾结点 
	} 	
 }
 
 Status LocateElemP(LinkList L, ElemType e, Position *q, int(*compare)(ElemType, ElemType)) 
 {
 	//若升序链表L中存在与e满足判定函数compare()取值为0的元素，则q指示L中
	//第一个值为e的结点的位置，并返回TRUE，否则q指示第一个与e满足判定函数
	//compare()取值>0的元素的前驱的位置。并返回FALSE。(用于一元多项式) 
	Link p = L.head, pp;
	
	do
	{
		pp = p;
		p = p->next;
	}while( p && (compare(p->data, e) < 0) );
	//没到表尾且p->data.expn<e.expn
	
	if ( (!p) || (compare(p->data, e) > 0) )//到表尾或compare(p->data, e)>0
	{
		*q = pp;
		return FALSE; 
	} 
	else//找到 
	{
		*q = p;
		return TRUE;
	}	
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

