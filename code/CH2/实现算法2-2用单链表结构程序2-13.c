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
 typedef struct LNode *LinkList; /* 另一种定义LinkList的方法 */
 
  void InitList(LinkList *L)
 { /* 操作结果：构造一个空的线性表L */
   *L=(LinkList)malloc(sizeof(struct LNode)); /* 产生头结点，并使L指向此头结点 */
   if(!*L) /* 存储分配失败 */
     exit(OVERFLOW);
   (*L)->next=NULL; /* 指针域为空 */
 }

 void DestroyList(LinkList *L)
 { /* 初始条件：线性表L已存在。操作结果：销毁线性表L */
   LinkList q;
   while(*L)
   {
     q=(*L)->next;
     free(*L);
     *L=q;
   }
 }

 void ClearList(LinkList L) /* 不改变L */
 { /* 初始条件：线性表L已存在。操作结果：将L重置为空表 */
   LinkList p,q;
   p=L->next; /* p指向第一个结点 */
   while(p) /* 没到表尾 */
   {
     q=p->next;
     free(p);
     p=q;
   }
   L->next=NULL; /* 头结点指针域为空 */
 }

 Status ListEmpty(LinkList L)
 { /* 初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
   if(L->next) /* 非空 */
     return FALSE;
   else
     return TRUE;
 }

 int ListLength(LinkList L)
 { /* 初始条件：线性表L已存在。操作结果：返回L中数据元素个数 */
   int i=0;
   LinkList p=L->next; /* p指向第一个结点 */
   while(p) /* 没到表尾 */
   {
     i++;
     p=p->next;
   }
   return i;
 }

 Status GetElem(LinkList L,int i,ElemType *e) /* 算法2.8 */
 { /* L为带头结点的单链表的头指针。当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR */
   int j=1; /* j为计数器 */
   LinkList p=L->next; /* p指向第一个结点 */
   while(p&&j<i) /* 顺指针向后查找，直到p指向第i个元素或p为空 */
   {
     p=p->next;
     j++;
   }
   if(!p||j>i) /* 第i个元素不存在 */
     return ERROR;
   *e=p->data; /* 取第i个元素 */
   return OK;
 }

 int LocateElem(LinkList L,ElemType e,Status(*compare)(ElemType,ElemType))
 { /* 初始条件: 线性表L已存在，compare()是数据元素判定函数(满足为1，否则为0) */
   /* 操作结果: 返回L中第1个与e满足关系compare()的数据元素的位序。 */
   /*           若这样的数据元素不存在，则返回值为0 */
   int i=0;
   LinkList p=L->next;
   while(p)
   {
     i++;
     if(compare(p->data,e)) /* 找到这样的数据元素 */
       return i;
     p=p->next;
   }
   return 0;
 }

 Status PriorElem(LinkList L,ElemType cur_e,ElemType *pre_e)
 { /* 初始条件: 线性表L已存在 */
   /* 操作结果: 若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱， */
   /*           返回OK；否则操作失败，pre_e无定义，返回INFEASIBLE */
   LinkList q,p=L->next; /* p指向第一个结点 */
   while(p->next) /* p所指结点有后继 */
   {
     q=p->next; /* q为p的后继 */
     if(q->data==cur_e)
     {
       *pre_e=p->data;
       return OK;
     }
     p=q; /* p向后移 */
   }
   return INFEASIBLE;
 }

 Status NextElem(LinkList L,ElemType cur_e,ElemType *next_e)
 { /* 初始条件：线性表L已存在 */
   /* 操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继， */
   /*           返回OK;否则操作失败，next_e无定义，返回INFEASIBLE */
   LinkList p=L->next; /* p指向第一个结点 */
   while(p->next) /* p所指结点有后继 */
   {
     if(p->data==cur_e)
     {
       *next_e=p->next->data;
       return OK;
     }
     p=p->next;
   }
   return INFEASIBLE;
 }

 Status ListInsert(LinkList L,int i,ElemType e) /* 算法2.9。不改变L */
 { /* 在带头结点的单链线性表L中第i个位置之前插入元素e */
   int j=0;
   LinkList p=L,s;
   while(p&&j<i-1) /* 寻找第i-1个结点 */
   {
     p=p->next;
     j++;
   }
   if(!p||j>i-1) /* i小于1或者大于表长 */
     return ERROR;
   s=(LinkList)malloc(sizeof(struct LNode)); /* 生成新结点 */
   s->data=e; /* 插入L中 */
   s->next=p->next;
   p->next=s;
   return OK;
 }

 Status ListDelete(LinkList L,int i,ElemType *e) /* 算法2.10。不改变L */
 { /* 在带头结点的单链线性表L中，删除第i个元素，并由e返回其值 */
   int j=0;
   LinkList p=L,q;
   while(p->next&&j<i-1) /* 寻找第i个结点，并令p指向其前岖 */
   {
     p=p->next;
     j++;
   }
   if(!p->next||j>i-1) /* 删除位置不合理 */
     return ERROR;
   q=p->next; /* 删除并释放结点 */
   p->next=q->next;
   *e=q->data;
   free(q);
   return OK;
 }

 void ListTraverse(LinkList L,void(*vi)(ElemType))
 /* vi的形参类型为ElemType，与bo2-1.c中相应函数的形参类型ElemType&不同 */
 { /* 初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi() */
   LinkList p=L->next;
   while(p)
   {
     vi(p->data);
     p=p->next;
   }
   printf("\n");
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
 
  void MergeList(LinkList La,LinkList Lb,LinkList *Lc) /* 算法2.2,此句与algo2-2.c不同 */
 { /* 已知线性表La和Lb中的数据元素按值非递减排列。*/
   /* 归并La和Lb得到新的线性表Lc，Lc的数据元素也按值非递减排列 */
   int i=1,j=1,k=0;
   int La_len,Lb_len;
   ElemType ai,bj;
   InitList(Lc); /* 创建空表Lc */
   La_len=ListLength(La);
   Lb_len=ListLength(Lb);
   while(i<=La_len&&j<=Lb_len) /* 表La和表Lb均非空 */
   {
     GetElem(La,i,&ai);
     GetElem(Lb,j,&bj);
     if(ai<=bj)
     {
       ListInsert(*Lc,++k,ai);
       ++i;
     }
     else
     {
       ListInsert(*Lc,++k,bj);
       ++j;
     }
   }
   while(i<=La_len) /* 表La非空且表Lb空 */
   {
     GetElem(La,i++,&ai);
     ListInsert(*Lc,++k,ai);
   }
   while(j<=Lb_len) /* 表Lb非空且表La空 */
   {
     GetElem(Lb,j++,&bj);
     ListInsert(*Lc,++k,bj);
   }
 }

 int main()
 {
   LinkList La,Lb,Lc; /* 此句与algo2-2.c不同 */
   int j,a[4]={3,5,8,11},b[7]={2,6,8,9,11,15,20}; /* 教科书例2–2的数据 */
   InitList(&La); /* 创建空表La */
   for(j=1;j<=4;j++) /* 在表La中插入4个元素 */
     ListInsert(La,j,a[j-1]);
   printf("La= "); /* 输出表La的内容 */
   ListTraverse(La,print);
   InitList(&Lb); /* 创建空表Lb */
   for(j=1;j<=7;j++) /* 在表Lb中插入7个元素 */
     ListInsert(Lb,j,b[j-1]);
   printf("Lb= "); /* 输出表Lb的内容 */
   ListTraverse(Lb,print);
   MergeList(La,Lb,&Lc);
   printf("Lc= "); /* 输出表Lc的内容 */
   ListTraverse(Lc,print);
   
   return 0; 
 }
 
 