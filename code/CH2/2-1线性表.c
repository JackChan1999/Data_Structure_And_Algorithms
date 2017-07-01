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
 #define LIST_INIT_SIZE 10 /* 线性表存储空间的初始分配量 */
 #define LIST_INCREMENT 2 /* 线性表存储空间的分配增量 */
 typedef struct
 {
   ElemType *elem; /* 存储空间基址 */
   int length; /* 当前长度 */
   int listsize; /* 当前分配的存储容量(以sizeof(ElemType)为单位) */
 }SqList;
 
 
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
 
 
 Status sq(ElemType c1,ElemType c2)
 { /* 数据元素判定函数(平方关系)，LocateElem()调用的函数 */
   if(c1==c2*c2)
     return TRUE;
   else
     return FALSE;
 }

 void dbl(ElemType *c)
 { /* ListTraverse()调用的另一函数(元素值加倍) */
   *c*=2;
 }

 void InitList(SqList *L);
 void DestroyList(SqList *L);
 void ClearList(SqList *L);
 Status ListEmpty(SqList L);
 int ListLength(SqList L);
 Status GetElem(SqList L,int i,ElemType *e);
 int LocateElem(SqList L,ElemType e,Status(*compare)(ElemType,ElemType));
 Status PriorElem(SqList L,ElemType cur_e,ElemType *pre_e);
 Status NextElem(SqList L,ElemType cur_e,ElemType *next_e);
 Status ListInsert(SqList *L,int i,ElemType e); 
 Status ListDelete(SqList *L,int i,ElemType *e);
 void ListTraverse(SqList L,void(*vi)(ElemType*));
 void Union(SqList *La, SqList Lb);
 void MergeList(SqList La, SqList Lb, SqList *Lc);
 
 
 int main()
 {
   SqList L;
   ElemType e,e0;
   Status i;
   int j,k;
   
   InitList(&L);
   printf("初始化L后：L.elem=%u L.length=%d L.listsize=%d\n",L.elem,L.length,L.listsize);
   for(j=1;j<=5;j++)
     i=ListInsert(&L,1,j);
   printf("在L的表头依次插入1～5后：*L.elem=");
   for(j=1;j<=5;j++)
     printf("%d ",*(L.elem+j-1));
   printf("\n");
   printf("L.elem=%u L.length=%d L.listsize=%d ",L.elem,L.length,L.listsize);
   i=ListEmpty(L);
   printf("L是否空：i=%d(1:是 0:否)\n",i);
   ClearList(&L);
   printf("清空L后：L.elem=%u L.length=%d L.listsize=%d ",L.elem,L.length,L.listsize);
   i=ListEmpty(L);
   printf("L是否空：i=%d(1:是 0:否)\n",i);
   for(j=1;j<=10;j++)
     ListInsert(&L,j,j);
   printf("在L的表尾依次插入1～10后：*L.elem=");
   for(j=1;j<=10;j++)
     printf("%d ",*(L.elem+j-1));
   printf("\n");
   printf("L.elem=%u L.length=%d L.listsize=%d\n",L.elem,L.length,L.listsize);
   ListInsert(&L,1,0);
   printf("在L的表头插入0后：*L.elem=");
   for(j=1;j<=ListLength(L);j++) /* ListLength(L)为元素个数 */
     printf("%d ",*(L.elem+j-1));
   printf("\n");
   printf("L.elem=%u(有可能改变) L.length=%d(改变) L.listsize=%d(改变)\n",L.elem,L.length,L.listsize);
   GetElem(L,5,&e);
   printf("第5个元素的值为：%d\n",e);
   for(j=10;j<=11;j++)
   {
     k=LocateElem(L,j,equal);
     if(k) /* k不为0，表明有符合条件的元素，且其位序为k */
       printf("第%d个元素的值为%d\n",k,j);
     else
       printf("没有值为%d的元素\n",j);
   }
   for(j=3;j<=4;j++)
   {
     k=LocateElem(L,j,sq);
     if(k) /* k不为0，表明有符合条件的元素，且其位序为k */
       printf("第%d个元素的值为%d的平方\n",k,j);
     else
       printf("没有值为%d的平方的元素\n",j);
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
   ListTraverse(L,print1); /* 依次对元素调用print1()，输出元素的值 */
   printf("L的元素值加倍后：");
   ListTraverse(L,dbl); /* 依次对元素调用dbl()，元素值乘2 */
   ListTraverse(L,print1);
   DestroyList(&L);
   printf("销毁L后：L.elem=%u L.length=%d L.listsize=%d\n",L.elem,L.length,L.listsize);
   return 0;
 }

 
 

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
   ElemType *p = L.elem+1;
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
   {
     return INFEASIBLE; /* 操作失败 */
   }
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


 void Union(SqList *La, SqList Lb)
 {
 	ElemType e;
 	int La_len, Lb_len;
 	int i;

 	//将所有在线性表Lb中但不在La中的数据元素插入La中
     La_len = ListLength(*La);
	 Lb_len = ListLength(Lb); 
	 
	 for (i=0; i<=Lb_len; i++)
	 {
	 	GetElem(Lb, i, &e);
	 	
	 	if (!LocateElem(*La, e, equal))
	 	{
	 		ListInsert(La, ++La_len, e);
	 	}
	 	//La中不存在和e相同的元素则插入之	 	
	 }
 }
 
  void MergeList(SqList La, SqList Lb, SqList *Lc)
  {
  	//已知线性表La和Lb中的数据元素按值非递减排列。
    //归并La和Lb得到新的线性表Lc，Lc的数据元素也按值非递减排列。
	int i = 1;
	int j = 1;
	int k = 0;
	int La_len, Lb_len;
	ElemType ai, bj;
	
	InitList(Lc);
    La_len = ListLength(La);
	Lb_len = ListLength(Lb);
	
	while ( (i <= La_len) && (j <= Lb_len) )
	{
		//La和Lb均非空
		GetElem(La, i, &ai);
		GetElem(Lb, j, &bj);
		
		if (ai <= bj)
		{
			ListInsert(Lc,++k, ai);
			++i;
		}
		else
		{
			ListInsert(Lc, ++k, bj);
			++j;
		}		 
	}
	
	while (i <= La_len)
	{
		GetElem(La, i++, &ai);
		ListInsert(Lc, ++k, ai);
	}
	while (j <= Lb_len)
	{
		GetElem(Lb, j++, &bj);
		ListInsert(Lc, ++k, bj);	
	}	   
  }
 



