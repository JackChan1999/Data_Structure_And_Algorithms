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
 #define LIST_INIT_SIZE 10 /* 线性表存储空间的初始分配量 */
 #define LIST_INCREMENT 2 /* 线性表存储空间的分配增量 */
 typedef struct
 {
   ElemType *elem; /* 存储空间基址 */
   int length; /* 当前长度 */
   int listsize; /* 当前分配的存储容量(以sizeof(ElemType)为单位) */
 }SqList;
 
 void InitList(SqList *L) /* 算法2.3 */
 { /* 操作结果：构造一个空的顺序线性表L */
   (*L).elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
   if(!(*L).elem)
     exit(OVERFLOW); /* 存储分配失败 */
   (*L).length=0; /* 空表长度为0 */
   (*L).listsize=LIST_INIT_SIZE; /* 初始存储容量 */
 }

 void DestroyList(SqList *L)
 { /* 初始条件：顺序线性表L已存在。操作结果：销毁顺序线性表L */
   free((*L).elem);
   (*L).elem=NULL;
   (*L).length=0;
   (*L).listsize=0;
 }

 void ClearList(SqList *L)
 { /* 初始条件：顺序线性表L已存在。操作结果：将L重置为空表 */
   (*L).length=0;
 }

 Status ListEmpty(SqList L)
 { /* 初始条件：顺序线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
   if(L.length==0)
     return TRUE;
   else
     return FALSE;
 }

 int ListLength(SqList L)
 { /* 初始条件：顺序线性表L已存在。操作结果：返回L中数据元素个数 */
   return L.length;
 }

 Status GetElem(SqList L,int i,ElemType *e)
 { /* 初始条件：顺序线性表L已存在,1≤i≤ListLength(L)。操作结果：用e返回L中第i个数据元素的值 */
   if(i<1||i>L.length)
     return ERROR;
   *e=*(L.elem+i-1);
   return OK;
 }

 int LocateElem(SqList L,ElemType e,Status(*compare)(ElemType,ElemType))
 { /* 初始条件：顺序线性表L已存在，compare()是数据元素判定函数(满足为1，否则为0) */
   /* 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。 */
   /*           若这样的数据元素不存在，则返回值为0。算法2.6 */
   ElemType *p;
   int i=1; /* i的初值为第1个元素的位序 */
   p=L.elem; /* p的初值为第1个元素的存储位置 */
   while(i<=L.length&&!compare(*p++,e))
     ++i;
   if(i<=L.length)
     return i;
   else
     return 0;
 }

 Status PriorElem(SqList L,ElemType cur_e,ElemType *pre_e)
 { /* 初始条件：顺序线性表L已存在 */
   /* 操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱， */
   /*           否则操作失败，pre_e无定义 */
   int i=2;
   ElemType *p=L.elem+1;
   while(i<=L.length&&*p!=cur_e)
   {
     p++;
     i++;
   }
   if(i>L.length)
     return INFEASIBLE; /* 操作失败 */
   else
   {
     *pre_e=*--p;
     return OK;
   }
 }

 Status NextElem(SqList L,ElemType cur_e,ElemType *next_e)
 { /* 初始条件：顺序线性表L已存在 */
   /* 操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继， */
   /*           否则操作失败，next_e无定义 */
   int i=1;
   ElemType *p=L.elem;
   while(i<L.length&&*p!=cur_e)
   {
     i++;
     p++;
   }
   if(i==L.length)
     return INFEASIBLE; /* 操作失败 */
   else
   {
     *next_e=*++p;
     return OK;
   }
 }

 Status ListInsert(SqList *L,int i,ElemType e) /* 算法2.4 */
 { /* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L)+1 */
   /* 操作结果：在L中第i个位置之前插入新的数据元素e，L的长度加1 */
   ElemType *newbase,*q,*p;
   if(i<1||i>(*L).length+1) /* i值不合法 */
     return ERROR;
   if((*L).length>=(*L).listsize) /* 当前存储空间已满,增加分配 */
   {
     newbase=(ElemType *)realloc((*L).elem,((*L).listsize+LIST_INCREMENT)*sizeof(ElemType));
     if(!newbase)
       exit(OVERFLOW); /* 存储分配失败 */
     (*L).elem=newbase; /* 新基址 */
     (*L).listsize+=LIST_INCREMENT; /* 增加存储容量 */
   }
   q=(*L).elem+i-1; /* q为插入位置 */
   for(p=(*L).elem+(*L).length-1;p>=q;--p) /* 插入位置及之后的元素右移 */
     *(p+1)=*p;
   *q=e; /* 插入e */
   ++(*L).length; /* 表长增1 */
   return OK;
 }

 Status ListDelete(SqList *L,int i,ElemType *e) /* 算法2.5 */
 { /* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
   /* 操作结果：删除L的第i个数据元素，并用e返回其值，L的长度减1 */
   ElemType *p,*q;
   if(i<1||i>(*L).length) /* i值不合法 */
     return ERROR;
   p=(*L).elem+i-1; /* p为被删除元素的位置 */
   *e=*p; /* 被删除元素的值赋给e */
   q=(*L).elem+(*L).length-1; /* 表尾元素的位置 */
   for(++p;p<=q;++p) /* 被删除元素之后的元素左移 */
     *(p-1)=*p;
   (*L).length--; /* 表长减1 */
   return OK;
 }

 void ListTraverse(SqList L,void(*vi)(ElemType*))
 { /* 初始条件：顺序线性表L已存在 */
   /* 操作结果：依次对L的每个数据元素调用函数vi() */
   /*           vi()的形参加'&'，表明可通过调用vi()改变元素的值 */
   ElemType *p;
   int i;
   p=L.elem;
   for(i=1;i<=L.length;i++)
     vi(p++);
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
 
 //修改算法2.7的第一个循环语句中的条件语句为开关语句，且当*pa = *pb时，只将
 //两者中之一插入Lc。 
 void MergeList(SqList La,SqList Lb,SqList *Lc)
 { /* 另一种合并线性表的方法(根据算法2.7下的要求修改算法2.7)，La、Lb和Lc均为按递增排列的表 */
   ElemType  *pa,*pa_last,*pb,*pb_last,*pc;
   pa=La.elem;
   pb=Lb.elem;
   (*Lc).listsize=La.length+Lb.length; /* 此句与算法2.7不同 */
   pc=(*Lc).elem=(ElemType *)malloc((*Lc).listsize*sizeof(ElemType));
   if(!(*Lc).elem)
     exit(OVERFLOW);
   pa_last=La.elem+La.length-1;
   pb_last=Lb.elem+Lb.length-1;
   while(pa<=pa_last&&pb<=pb_last) /* 表La和表Lb均非空 */
     switch(comp(*pa,*pb)) /* 此句与算法2.7不同 */
     {
       case  0: pb++;
       case -1: *pc++=*pa++;
                break;
       case  1: *pc++=*pb++;
     }
   while(pa<=pa_last) /* 表La非空且表Lb空 */
     *pc++=*pa++;
   while(pb<=pb_last) /* 表Lb非空且表La空 */
     *pc++=*pb++;
   (*Lc).length=pc-(*Lc).elem; /* 加此句 */
 }

 void main()
 {
   SqList La,Lb,Lc;
   int j;
   InitList(&La); /* 创建空表La */
   for(j=1;j<=5;j++) /* 在表La中插入5个元素，依次为1、2、3、4、5 */
     ListInsert(&La,j,j);
   printf("La= "); /* 输出表La的内容 */
   ListTraverse(La,print1);
   InitList(&Lb); /* 创建空表Lb */
   for(j=1;j<=5;j++) /* 在表Lb中插入5个元素，依次为2、4、6、8、10 */
     ListInsert(&Lb,j,2*j);
   printf("Lb= "); /* 输出表Lb的内容 */
   ListTraverse(Lb,print1);
   MergeList(La,Lb,&Lc); /* 由按递增排列的表La、Lb得到按递增排列的表Lc */
   printf("Lc= "); /* 输出表Lc的内容 */
   ListTraverse(Lc,print1);
 }