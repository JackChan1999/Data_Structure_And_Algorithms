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
 typedef struct //项的表示，多项式的项作为LinkList的数据元素 
 {
 	float coef;//系数
    int expn;//指数 
 }term, ElemType;//两个类型名：term用于这个ADT（抽象数据类型）
 //ElemType为LinkList的数据对象名

  typedef struct LNode /* 结点类型 */
 {
   ElemType data;
   struct LNode *next;
 }LNode,*Link,*Position;

 typedef struct LinkList /* 链表类型 */
 {
   Link head,tail; /* 分别指向线性链表中的头结点和最后一个结点 */
   int len; /* 指示线性链表中数据元素的个数 */
 }LinkList; 
 
 typedef LinkList polynomial;
 #define DestroyPolyn DestroyList //函数同义不同名
 #define PolynLength ListLength //函数同义不同名
 
 void OrderInsertMerge(LinkList *L, ElemType e, int(* compare)(term, term));
 int cmp(term a, term b);//CreatPolyn()的 实参 
 void CreatPolyn(polynomial *P, int m);
 void PrintPolyn(polynomial P);
 void AddPolyn(polynomial *Pa, polynomial *Pb);
 void AddPolyn1(polynomial *Pa, polynomial *Pb);
 void Opposite(polynomial Pa);
 void SubtractPolyn(polynomial *Pa, polynomial *Pb);
 void MultiplyPolyn(polynomial *Pa, polynomial *Pb);
 void MakeNode(Link *p,ElemType e);
 void FreeNode(Link *p);
 void InitList(LinkList *L);
 void ClearList(LinkList *L);
 void DestroyList(LinkList *L);
 void InsFirst(LinkList *L,Link h,Link s); /* 形参增加L,因为需修改L */
 Status DelFirst(LinkList *L,Link h,Link *q); /* 形参增加L,因为需修改L */
 void Append(LinkList *L,Link s);
 Position PriorPos(LinkList L,Link p);
 Status Remove(LinkList *L,Link *q);
 void InsBefore(LinkList *L,Link *p,Link s);
 void InsAfter(LinkList *L,Link *p,Link s);
 void SetCurElem(Link p,ElemType e);
 ElemType GetCurElem(Link p);
 Status ListEmpty(LinkList L);
 int ListLength(LinkList L);
 Position GetHead(LinkList L);
 Position GetLast(LinkList L);
 Position NextPos(Link p);
 Status LocatePos(LinkList L,int i,Link *p);
 Position LocateElem(LinkList L,ElemType e,Status (*compare)(ElemType,ElemType));
 void ListTraverse(LinkList L,void(*visit)(ElemType));
 void OrderInsert(LinkList *L,ElemType e,int (*comp)(ElemType,ElemType));
 Status LocateElemP(LinkList L,ElemType e,Position *q,int(*compare)(ElemType,ElemType));
 
 int main()
 {
   polynomial p,q;
   int m;
   printf("请输入第1个一元多项式的非零项的个数：");
   scanf("%d",&m);
   CreatPolyn(&p,m);
   printf("请输入第2个一元多项式的非零项的个数：");
   scanf("%d",&m);
   CreatPolyn(&q,m);
   AddPolyn(&p,&q);
   printf("2个一元多项式相加的结果：\n");
   PrintPolyn(p);
   printf("请输入第3个一元多项式的非零项的个数：");
   scanf("%d",&m);
   CreatPolyn(&q,m);
   AddPolyn1(&p,&q);
   printf("2个一元多项式相加的结果(另一种方法)：\n");
   PrintPolyn(p);
   printf("请输入第4个一元多项式的非零项的个数：");
   scanf("%d",&m);
   CreatPolyn(&q,m);
   SubtractPolyn(&p,&q);
   printf("2个一元多项式相减的结果：\n");
   PrintPolyn(p);
   printf("请输入第5个一元多项式的非零项的个数：");
   scanf("%d",&m);
   CreatPolyn(&q,m);
   MultiplyPolyn(&p,&q);
   printf("2个一元多项式相乘的结果：\n");
   PrintPolyn(p);
   DestroyPolyn(&p);
   
   return 0;
 }
 
 void OrderInsertMerge(LinkList *L, ElemType e, int(* compare)(term, term))
 {
 	//按有序判定函数compare()的约定，将值为e的结点插入或合并到升序链表L的适当位置
	Position q, s;
	if ( LocateElemP(*L, e, &q, compare) )//L中存在该指数项
	{
		q->data.coef += e.coef;//改变当前结点系数的值 
		
		if (! q->data.coef)//系数为0
		{
			//删除多项式L中当前结点
			s = PriorPos(*L, q);//s为当前结点的前驱
			if (!s)//q无前驱
			{
				s = (*L).head;
			}
			DelFirst(L, s, &q);
			FreeNode(&q);
		}//if	 
	} //if
	else//生成该指数项并插入链表 
	{
		MakeNode(&s, e);//生成结点
		InsFirst(L, q, s); 		
	}	 
 }
 
 int cmp(term a, term b)//CreatPolyn()的 实参 
 {
 	//依a的指数值< ,=或>b的指数值，分别返回-1，0或+1
	if (a.expn == b.expn)
	{
		return 0;
	} 
	else
	{
		return (a.expn-b.expn) / abs(a.expn-b.expn);
	}
 }
 
 void CreatPolyn(polynomial *P, int m)
 {
 	//输入m项的系数和指数，建立表示一元多项式的有序链表P
	Position q, s;
	term e;
	int i;
    
    InitList(P);
    printf("请依次输入%d个系数，指数：\n", m);
    
    for (i=1; i<=m; ++i)
	{
		//依次输入m个非零项（可按任意顺序）
		scanf("%f,%d", &e.coef, &e.expn);
		
		if (! LocateElemP(*P, e, &q, cmp))//当前链表中不存在该指数项，cmp是实参 
		{
			MakeNode(&s, e);//生成结点并插入链表
			InsFirst(P, q, s); 
		} 	 
	} //for
 }
 
 void PrintPolyn(polynomial P)
 {
 	//打印输出一元多项式P
	Link q;
	
	q = P.head->next;//q指向第1个结点 
	printf(" 系数   指数\n");
	
	while (q)
	{
		printf("%f  %d\n", q->data.coef, q->data.expn);
		q = q->next;	
	} 	
 }
 
 void AddPolyn(polynomial *Pa, polynomial *Pb)
 {
 	Position ha, hb, qa, qb;
 	term a, b;
 	
 	ha = GetHead(*Pa);
 	hb = GetHead(*Pb);//ha和hb分别指向Pa和Pb的头结点
	//qa和qb分别指向Pa和Pb中当前结点（现在是第一个结点） 
	qa = NextPos(ha);
	qb = NextPos(hb);
	
	while ( (!ListEmpty(*Pa)) && (!ListEmpty(*Pb)) && qa )
	{
		//Pa和Pb均非空且ha没指向尾结点(qa!=0)
		a = GetCurElem(qa);
		b = GetCurElem(qb);//a和b为两表中当前比较元素
		
		switch ( cmp(a, b) )
		{
			case -1:
			{
				ha = qa;//多项式 Pa中当前结点的指数值小
				qa = NextPos(ha);//ha和qa均向后移一个结点 
			}
			break; 
			case 0:
			{
				//两者的指数值相等 ，修改Pa当前结点的系数值 
				qa->data.coef += qb->data.coef;
				if ( (qa->data.coef) == 0 )//删除多项式Pa中当前结点 
				{
					DelFirst(Pa, ha, &qa);
					FreeNode(&qa); 
				} 
				else
				{
					ha = qa;
				}
				
				DelFirst(Pb, hb, &qb);
				FreeNode(&qb);
				qb = NextPos(hb);
				qa = NextPos(ha);	
			}
			break;
			case 1:
			{
				DelFirst(Pb, hb, &qb);//多项式Pb中当前结点的指数值小
				InsFirst(Pa, ha, qb);
				ha = ha->next;
				qb = NextPos(hb); 
			}
			break;
		} //switch
	} //while
	if (! ListEmpty(*Pb))
	{
		(*Pb).tail = hb;
		Append(Pa, qb);//链接Pb中剩余结点 
	}
	DestroyPolyn(Pb);//销毁Pb 
 }
 
 void AddPolyn1(polynomial *Pa, polynomial *Pb)
 {
 	//另一种多项式加法的算法：Pa=Pa+Pb， 并销毁一元多项式Pb
	Position qb;
	term b;
	
	qb = GetHead(*Pb);//qb指向Pb的头结点
	qb = qb->next;//qb指向Pb的第1个结点 
	
	while (qb)
	{
		b = GetCurElem(qb);
		OrderInsertMerge(Pa, b, cmp);
		qb = qb->next;	
	} 
	DestroyPolyn(Pb);//销毁Pb 
 } 
 
 void Opposite(polynomial Pa)
 {
 	//一元多项式Pa系数取反
	 Position p;
	 
	 p = Pa.head;
	 
	 while (p->next)
	 {
	 	p = p->next;
	 	p->data.coef *= -1; 
 	 } 
 } 
 
 void SubtractPolyn(polynomial *Pa, polynomial *Pb)
 {
 	//多项式减法：Pa=Pa-Pb，并销毁一元多项式Pb
	Opposite(*Pb);
	AddPolyn(Pa, Pb); 
 }
 
 void MultiplyPolyn(polynomial *Pa, polynomial *Pb)
 {
 	//多项式乘法：Pa=Pa*Pb,并销毁一元多项式Pb
	polynomial Pc;
	Position qa, qb;
	term a, b, c;
	
	InitList(&Pc);
	qa = GetHead(*Pa);
	qa = qa->next;
	
	while (qa)
	{
		a = GetCurElem(qa);
		qb = GetHead(*Pb);
		qb = qb->next; 
		while (qb)
		{
		    b = GetCurElem(qb);
		    c.coef = a.coef * b.coef;
			c.expn = a.expn + b.expn;
			OrderInsertMerge(&Pc, c, cmp);
			qb = qb->next;	
		}
		qa = qa->next;
	}//while
	DestroyPolyn(Pb);//销毁Pb
	ClearList(Pa);//将Pa重置为空表
	(*Pa).head = Pc.head;
	(*Pa).tail = Pc.tail;
	(*Pa).len = Pc.len;
 }
 
 void MakeNode(Link *p,ElemType e)
 { /* 分配由p指向的值为e的结点。若分配失败，则退出 */
   *p=(Link)malloc(sizeof(LNode));
   if(!*p)
     exit(ERROR);
   (*p)->data=e;
 }

 void FreeNode(Link *p)
 { /* 释放p所指结点 */
   free(*p);
   *p=NULL;
 }

 void InitList(LinkList *L)
 { /* 构造一个空的线性链表L */
   Link p;
   p=(Link)malloc(sizeof(LNode)); /* 生成头结点 */
   if(p)
   {
     p->next=NULL;
     (*L).head=(*L).tail=p;
     (*L).len=0;
   }
   else
     exit(ERROR);
 }

 void ClearList(LinkList *L)
 { /* 将线性链表L重置为空表，并释放原链表的结点空间 */
   Link p,q;
   if((*L).head!=(*L).tail) /* 不是空表 */
   {
     p=q=(*L).head->next;
     (*L).head->next=NULL;
     while(p!=(*L).tail)
     {
       p=q->next;
       free(q);
       q=p;
     }
     free(q);
     (*L).tail=(*L).head;
     (*L).len=0;
   }
 }

 void DestroyList(LinkList *L)
 { /* 销毁线性链表L，L不再存在 */
   ClearList(L); /* 清空链表 */
   FreeNode(&(*L).head);
   (*L).tail=NULL;
   (*L).len=0;
 }

 void InsFirst(LinkList *L,Link h,Link s) /* 形参增加L,因为需修改L */
 { /* h指向L的一个结点，把h当做头结点，将s所指结点插入在第一个结点之前 */
   s->next=h->next;
   h->next=s;
   if(h==(*L).tail) /* h指向尾结点 */
     (*L).tail=h->next; /* 修改尾指针 */
   (*L).len++;
 }

 Status DelFirst(LinkList *L,Link h,Link *q) /* 形参增加L,因为需修改L */
 { /* h指向L的一个结点，把h当做头结点，删除链表中的第一个结点并以q返回。 */
   /* 若链表为空(h指向尾结点)，q=NULL，返回FALSE */
   *q=h->next;
   if(*q) /* 链表非空 */
   {
     h->next=(*q)->next;
     if(!h->next) /* 删除尾结点 */
       (*L).tail=h; /* 修改尾指针 */
     (*L).len--;
     return OK;
   }
   else
     return FALSE; /* 链表空 */
 }

 void Append(LinkList *L,Link s)
 { /* 将指针s(s->data为第一个数据元素)所指(彼此以指针相链，以NULL结尾)的 */
   /* 一串结点链接在线性链表L的最后一个结点之后，并改变链表L的尾指针指向新的尾结点 */
   int i=1;
   (*L).tail->next=s;
   while(s->next)
   {
     s=s->next;
     i++;
   }
   (*L).tail=s;
   (*L).len+=i;
 }

 Position PriorPos(LinkList L,Link p)
 { /* 已知p指向线性链表L中的一个结点，返回p所指结点的直接前驱的位置。若无前驱，则返回NULL */
   Link q;
   q=L.head->next;
   if(q==p) /* 无前驱 */
     return NULL;
   else
   {
     while(q->next!=p) /* q不是p的直接前驱 */
       q=q->next;
     return q;
   }
 }

 Status Remove(LinkList *L,Link *q)
 { /* 删除线性链表L中的尾结点并以q返回，改变链表L的尾指针指向新的尾结点 */
   Link p=(*L).head;
   if((*L).len==0) /* 空表 */
   {
     *q=NULL;
     return FALSE;
   }
   while(p->next!=(*L).tail)
     p=p->next;
   *q=(*L).tail;
   p->next=NULL;
   (*L).tail=p;
   (*L).len--;
   return OK;
 }

 void InsBefore(LinkList *L,Link *p,Link s)
 { /* 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之前， */
   /* 并修改指针p指向新插入的结点 */
   Link q;
   q=PriorPos(*L,*p); /* q是p的前驱 */
   if(!q) /* p无前驱 */
     q=(*L).head;
   s->next=*p;
   q->next=s;
   *p=s;
   (*L).len++;
 }

 void InsAfter(LinkList *L,Link *p,Link s)
 { /* 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之后， */
   /* 并修改指针p指向新插入的结点 */
   if(*p==(*L).tail) /* 修改尾指针 */
     (*L).tail=s;
   s->next=(*p)->next;
   (*p)->next=s;
   *p=s;
   (*L).len++;
 }

 void SetCurElem(Link p,ElemType e)
 { /* 已知p指向线性链表中的一个结点，用e更新p所指结点中数据元素的值 */
   p->data=e;
 }

 ElemType GetCurElem(Link p)
 { /* 已知p指向线性链表中的一个结点，返回p所指结点中数据元素的值 */
   return p->data;
 }

 Status ListEmpty(LinkList L)
 { /* 若线性链表L为空表，则返回TRUE，否则返回FALSE */
   if(L.len)
     return FALSE;
   else
     return TRUE;
 }

 int ListLength(LinkList L)
 { /* 返回线性链表L中元素个数 */
   return L.len;
 }

 Position GetHead(LinkList L)
 { /* 返回线性链表L中头结点的位置 */
   return L.head;
 }

 Position GetLast(LinkList L)
 { /* 返回线性链表L中最后一个结点的位置 */
   return L.tail;
 }

 Position NextPos(Link p)
 { /* 已知p指向线性链表L中的一个结点，返回p所指结点的直接后继的位置。若无后继，则返回NULL */
   return p->next;
 }

 Status LocatePos(LinkList L,int i,Link *p)
 { /* 返回p指示线性链表L中第i个结点的位置，并返回OK，i值不合法时返回ERROR。i=0为头结点 */
   int j;
   if(i<0||i>L.len)
     return ERROR;
   else
   {
     *p=L.head;
     for(j=1;j<=i;j++)
       *p=(*p)->next;
     return OK;
   }
 }

 Position LocateElem(LinkList L,ElemType e,Status (*compare)(ElemType,ElemType))
 { /* 返回线性链表L中第1个与e满足函数compare()判定关系的元素的位置， */
   /* 若不存在这样的元素，则返回NULL */
   Link p=L.head;
   do
     p=p->next;
   while(p&&!(compare(p->data,e))); /* 没到表尾且没找到满足关系的元素 */
   return p;
 }

 void ListTraverse(LinkList L,void(*visit)(ElemType))
 { /* 依次对L的每个数据元素调用函数visit() */
   Link p=L.head->next;
   int j;
   for(j=1;j<=L.len;j++)
   {
     visit(p->data);
     p=p->next;
   }
   printf("\n");
 }

 void OrderInsert(LinkList *L,ElemType e,int (*comp)(ElemType,ElemType))
 { /* 已知L为有序线性链表，将元素e按非降序插入在L中。(用于一元多项式) */
   Link o,p,q;
   q=(*L).head;
   p=q->next;
   while(p!=NULL&&comp(p->data,e)<0) /* p不是表尾且元素值小于e */
   {
     q=p;
     p=p->next;
   }
   o=(Link)malloc(sizeof(LNode)); /* 生成结点 */
   o->data=e; /* 赋值 */
   q->next=o; /* 插入 */
   o->next=p;
   (*L).len++; /* 表长加1 */
   if(!p) /* 插在表尾 */
     (*L).tail=o; /* 修改尾结点 */
 }

 Status LocateElemP(LinkList L,ElemType e,Position *q,int(*compare)(ElemType,ElemType))
 { /* 若升序链表L中存在与e满足判定函数compare()取值为0的元素，则q指示L中 */
   /* 第一个值为e的结点的位置，并返回TRUE；否则q指示第一个与e满足判定函数 */
   /* compare()取值>0的元素的前驱的位置。并返回FALSE。(用于一元多项式) */
   Link p=L.head,pp;
   do
   {
     pp=p;
     p=p->next;
   }while(p&&(compare(p->data,e)<0)); /* 没到表尾且p->data.expn<e.expn */
   if(!p||compare(p->data,e)>0) /* 到表尾或compare(p->data,e)>0 */
   {
     *q=pp;
     return FALSE;
   }
   else /* 找到 */
   {
     *q=p;
     return TRUE;
   }
 }
 
 
 
