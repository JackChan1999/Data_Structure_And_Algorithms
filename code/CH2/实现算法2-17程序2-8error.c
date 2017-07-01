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
 
 #define N 2
 typedef char ElemType;
 #define MAX_SIZE 100 /* 链表的最大长度 */
 typedef struct
 {
   ElemType data;
   int cur;
 }component,SLinkList[MAX_SIZE];
 
  int Malloc(SLinkList space) /* 算法2.15 */
 { /* 若备用链表非空，则返回分配的结点下标(备用链表的第一个结点)，否则返回0 */
   int i=space[0].cur;
   if(i) /* 备用链表非空 */
     space[0].cur=space[i].cur; /* 备用链表的头结点指向原备用链表的第二个结点 */
   return i; /* 返回新开辟结点的坐标 */
 }

 void Free(SLinkList space,int k) /* 算法2.16 */
 { /* 将下标为k的空闲结点回收到备用链表(成为备用链表的第一个结点) */
   space[k].cur=space[0].cur; /* 回收结点的＂游标＂指向备用链表的第一个结点 */
   space[0].cur=k; /* 备用链表的头结点指向新回收的结点 */
 }
 
  #define DestroyList ClearList // DestroyList()和ClearList()的操作是一样的
 void InitSpace(SLinkList L) /* 算法2.14。另加 */
 { /* 将一维数组L中各分量链成一个备用链表，L[0].cur为头指针。“0”表示空指针 */
   int i;
   for(i=0;i<MAX_SIZE-1;i++)
     L[i].cur=i+1;
   L[MAX_SIZE-1].cur=0;
 }

 int InitList(SLinkList L)
 { /* 构造一个空链表，返回值为空表在数组中的位序 */
   int i;
   i=Malloc(L); /* 调用Malloc()，简化程序 */
   L[i].cur=0; /* 空链表的表头指针为空(0) */
   return i;
 }

 void ClearList(SLinkList L,int n)
 { /* 初始条件：L中表头位序为n的静态链表已存在。操作结果：将此表重置为空表 */
   int j,k,i=L[n].cur; /* 链表第一个结点的位置 */
   L[n].cur=0; /* 链表空 */
   k=L[0].cur; /* 备用链表第一个结点的位置 */
   L[0].cur=i; /* 把链表的结点连到备用链表的表头 */
   while(i) /* 没到链表尾 */
   {
     j=i;
     i=L[i].cur; /* 指向下一个元素 */
   }
   L[j].cur=k; /* 备用链表的第一个结点接到链表的尾部 */
 }

 Status ListEmpty(SLinkList L,int n)
 { /* 判断L中表头位序为n的链表是否空，若是空表返回TRUE；否则返回FALSE */
   if(L[n].cur==0) /* 空表 */
     return TRUE;
   else
     return FALSE;
 }

 int ListLength(SLinkList L,int n)
 { /* 返回L中表头位序为n的链表的数据元素个数 */
   int j=0,i=L[n].cur; /* i指向第一个元素 */
   while(i) /* 没到静态链表尾 */
   {
     i=L[i].cur; /* 指向下一个元素 */
     j++;
   }
   return j;
 }

 Status GetElem(SLinkList L,int n, int i,ElemType *e)
 { /* 用e返回L中表头位序为n的链表的第i个元素的值 */
   int l,k=n; /* k指向表头序号 */
   if(i<1||i>ListLength(L,n)) /* i值不合理 */
     return ERROR;
   for(l=1;l<=i;l++) /* 移动i-1个元素 */
     k=L[k].cur;
   *e=L[k].data;
   return OK;
 }

 int LocateElem(SLinkList L,int n,ElemType e) /* 算法2.13(有改动) */
 { /* 在L中表头位序为n的静态单链表中查找第1个值为e的元素。*/
   /* 若找到，则返回它在L中的位序，否则返回0。(与其它LocateElem()的定义不同) */
   int i=L[n].cur; /* i指示表中第一个结点 */
   while(i&&L[i].data!=e) /* 在表中顺链查找(e不能是字符串) */
     i=L[i].cur;
   return i;
 }

 Status PriorElem(SLinkList L,int n,ElemType cur_e,ElemType *pre_e)
 { /* 初始条件：在L中表头位序为n的静态单链表已存在 */
   /* 操作结果：若cur_e是此单链表的数据元素，且不是第一个， */
   /*           则用pre_e返回它的前驱，否则操作失败，pre_e无定义 */
   int j,i=L[n].cur; /* i为链表第一个结点的位置 */
   do
   { /* 向后移动结点 */
     j=i;
     i=L[i].cur;
   }while(i&&cur_e!=L[i].data);
   if(i) /* 找到该元素 */
   {
     *pre_e=L[j].data;
     return OK;
   }
   return ERROR;
 }

 Status NextElem(SLinkList L,int n,ElemType cur_e,ElemType *next_e)
 { /* 初始条件：在L中表头位序为n的静态单链表已存在 */
   /* 操作结果：若cur_e是此单链表的数据元素，且不是最后一个， */
   /* 则用next_e返回它的后继，否则操作失败，next_e无定义 */
   int i;
   i=LocateElem(L,n,cur_e); /* 在链表中查找第一个值为cur_e的元素的位置 */
   if(i) /* 在静态单链表中存在元素cur_e */
   {
     i=L[i].cur; /* cur_e的后继的位置 */
     if(i) /* cur_e有后继 */
     {
       *next_e=L[i].data;
       return OK; /* cur_e元素有后继 */
     }
   }
   return ERROR; /* L不存在cur_e元素,cur_e元素无后继 */
 }

 Status ListInsert(SLinkList L,int n,int i,ElemType e)
 { /* 在L中表头位序为n的链表的第i个元素之前插入新的数据元素e */
   int l,j,k=n; /* k指向表头 */
   if(i<1||i>ListLength(L,n)+1)
     return ERROR;
   j=Malloc(L); /* 申请新单元 */
   if(j) /* 申请成功 */
   {
     L[j].data=e; /* 赋值给新单元 */
     for(l=1;l<i;l++) /* 游标向后移动i-1个元素 */
       k=L[k].cur;
     L[j].cur=L[k].cur;
     L[k].cur=j;
     return OK;
   }
   return ERROR;
 }

 Status ListDelete(SLinkList L,int n,int i,ElemType *e)
 { /* 删除在L中表头位序为n的链表的第i个数据元素e，并返回其值 */
   int j,k=n; /* k指向表头 */
   if(i<1||i>ListLength(L,n))
     return ERROR;
   for(j=1;j<i;j++) /* 游标向后移动i-1个元素 */
     k=L[k].cur;
   j=L[k].cur;
   L[k].cur=L[j].cur;
   *e=L[j].data;
   Free(L,j);
   return OK;
 }

 void ListTraverse(SLinkList L,int n,void(*vi)(ElemType))
 { /* 依次对L中表头位序为n的链表的每个数据元素调用函数vi() */
   int i=L[n].cur; /* 指向第一个元素 */
   while(i) /* 没到静态链表尾 */
   {
     vi(L[i].data); /* 调用vi() */
     i=L[i].cur; /* 指向下一个元素 */
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
 
  void difference(SLinkList space,int *S) /* 算法2.17 */
 { /* 依次输入集合A和B的元素，在一维数组space中建立表示集合(A-B)∪(B-A) */
   /* 的静态链表，S为其头指针。假设备用空间足够大，space[0].cur为备用空间的头指针 */
   int r,p,m,n,i,j,k;
   ElemType b;
   InitSpace(space); /* 初始化备用空间 */
   *S=Malloc(space); /* 生成S的头结点 */
   r=*S; /* r指向S的当前最后结点 */
   printf("请输入集合A和B的元素个数m,n:");
   scanf("%d,%d%*c",&m,&n); /* %*c吃掉回车符 */
   printf("请输入集合A的元素（共%d个）:",m);
   for(j=1;j<=m;j++) /* 建立集合A的链表 */
   {
     i=Malloc(space); /* 分配结点 */
     scanf("%c",&space[i].data); /* 输入A的元素值 */
     space[r].cur=i; /* 插入到表尾 */
     r=i;
   }
   scanf("%*c"); /* %*c吃掉回车符 */
   space[r].cur=0; /* 尾结点的指针为空 */
   printf("请输入集合B的元素（共%d个）:",n);
   for(j=1;j<=n;j++)
   { /* 依次输入B的元素，若不在当前表中，则插入，否则删除 */
     scanf("%c",&b);
     p=*S;
     k=space[*S].cur; /* k指向集合A中的第一个结点 */
     while(k!=space[r].cur&&space[k].data!=b)
     { /* 在当前表中查找 */
       p=k;
       k=space[k].cur;
     }
     if(k==space[r].cur)
     { /* 当前表中不存在该元素，插入在r所指结点之后，且r的位置不变 */
       i=Malloc(space);
       space[i].data=b;
       space[i].cur=space[r].cur;
       space[r].cur=i;
     }
     else /* 该元素已在表中，删除之 */
     {
       space[p].cur=space[k].cur;
       Free(space,k);
       if(r==k)
         r=p; /* 若删除的是尾元素，则需修改尾指针 */
     }
   }
 }

 int main()
 {
   int k;
   SLinkList s;
   difference(s,&k);
   ListTraverse(s,k,print2);
    
   return 0; 
 }

 
 
 

 
 