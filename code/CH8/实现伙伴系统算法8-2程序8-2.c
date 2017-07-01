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
 #define m 10 /* 可利用空间总容量1024字的2的幂次，子表的个数为m+1 */
 typedef struct WORD_b
 {
   struct WORD_b *llink; /* 指向前驱结点 */
   int tag; /* 块标志，0：空闲，1：占用 */
   int kval; /* 块大小，值为2的幂次k */
   struct WORD_b *rlink; /* 头部域，指向后继结点 */
 }WORD_b,head,*Space; /* WORD_b：内存字类型，结点的第一个字也称为head */

 typedef struct HeadNode
 {
   int nodesize; /* 该链表的空闲块的大小 */
   struct WORD_b *first; /* 该链表的表头指针 */
 }FreeList[m+1]; /* 表头向量类型 */
 #define N 100 /* 占用块个数的最大值 */
 Space r; /* r为生成空间的首地址，全局量 */

 Space AllocBuddy(FreeList avail,int n)
 { /* avail[0..m]为可利用空间表，n为申请分配量，若有不小于n的空闲块，*/
   /* 则分配相应的存储块，并返回其首地址；否则返回NULL。算法8.2 */
   int i,k;
   Space pa,pi,pre,suc;
   for(k=0;k<=m&&(avail[k].nodesize<n+1||!avail[k].first);++k); /* 从小到大查找满足分配要求的子表序号k */
   if(k>m) /* 分配失败，返回NULL */
     return NULL;
   else /* 进行分配 */
   {
     pa=avail[k].first; /* pa指向可分配子表的第一个结点 */
     pre=pa->llink; /* pre和suc分别指向pa所指结点的前驱和后继 */
     suc=pa->rlink;
     if(pa==suc) /* 可分配子表只有1个结点 */
       avail[k].first=NULL; /* 分配后该子表变成空表 */
     else /* 从子表中删去pa所指结点(链表的第1个结点) */
     {
       pre->rlink=suc;
       suc->llink=pre;
       avail[k].first=suc; /* 该子表的头指针指向pa所指结点的后继 */
     }
     for(i=1;avail[k-i].nodesize>=n+1;++i)
     { /* 从大到小将剩余块插入相应子表，约定将低地址(最前面)的块作为分配块 */
       pi=pa+(int)pow(2,k-i); /* pi指向再分割的后半块(剩余块) */
       pi->rlink=pi; /* pi是该链表的第1个结点，故左右指针都指向自身 */
       pi->llink=pi;
       pi->tag=0; /* 块标志为空闲 */
       pi->kval=k-i; /* 块大小 */
       avail[k-i].first=pi; /* 插入链表 */
     }
     pa->tag=1; /* 最后剩给pa的是分配块，令其块标志为占用 */
     pa->kval=k-(--i); /* 块大小 */
   }
   return pa; /* 返回分配块的地址 */
 }

 Space buddy(Space p)
 { /* 返回起始地址为p，块大小为pow(2,p->kval)的块的伙伴地址 */
   if((p-r)%(int)pow(2,p->kval+1)==0) /* p为前块 */
     return p+(int)pow(2,p->kval); /* 返回后块地址 */
   else /* p为后块 */
     return p-(int)pow(2,p->kval); /* 返回前块地址 */
 }

 void Reclaim(FreeList pav,Space *p)
 { /* 伙伴系统的回收算法。将p所指的释放块回收到可利用空间表pav中 */
   Space s;
   s=buddy(*p); /* 伙伴块的起始地址 */
   while(s>=r&&s<r+pav[m].nodesize&&s->tag==0&&s->kval==(*p)->kval) /* 归并伙伴块 */
   { /* ,伙伴块起始地址在有效范围内且伙伴块空闲并与p块等大，从链表上删除该伙伴块结点 */
     if(s->rlink==s) /* 链表上仅此一个结点 */
       pav[s->kval].first=NULL; /* 置此链表为空 */
     else /* 链表上不止一个结点 */
     {
       s->llink->rlink=s->rlink; /* 前驱的后继为该结点的后继 */
       s->rlink->llink=s->llink; /* 后继的前驱为该结点的前驱 */
       if(pav[s->kval].first==s) /* s是链表的第一个结点 */
         pav[s->kval].first=s->rlink; /* 修改表头指向下一个结点 */
     } /* 以下修改结点头部 */
     if((*p-r)%(int)pow(2,(*p)->kval+1)==0) /* p为前块 */
       (*p)->kval++; /* 块大小加倍 */ 
     else /* p为后块(s为前块) */
     {
       s->kval=(*p)->kval+1; /* 块大小加倍 */
       *p=s; /* p指向新块首地址 */
     }
     s=buddy(*p); /* 下一个伙伴块的起始地址 */
   } /* 以下将p插到可利用空间表中 */
   (*p)->tag=0; /* 设块标志为空闲 */
   if(pav[(*p)->kval].first==NULL) /* 该链表空 */
     pav[(*p)->kval].first=(*p)->llink=(*p)->rlink=*p; /* 左右指针及表头都指向自身 */
   else /* 该链表不空，插在表头 */
   {
     (*p)->rlink=pav[(*p)->kval].first;
     (*p)->llink=(*p)->rlink->llink;
     (*p)->rlink->llink=*p;
     (*p)->llink->rlink=*p;
     pav[(*p)->kval].first=*p;
   }
   *p=NULL;
 }

 void Print(FreeList p)
 { /* 输出p中所有可利用空间表 */
   int i;
   Space h;
   for(i=0;i<=m;i++)
     if(p[i].first) /* 第i个可利用空间表不空 */
     {
       h=p[i].first; /* h指向链表的第一个结点的头部域(首地址) */
       do
       {
         printf("块的大小=%d 块的首地址=%u ",(int)pow(2,h->kval),h); /* 输出结点信息 */
         printf("块标志=%d(0:空闲 1:占用)\n",h->tag);
         h=h->rlink; /* 指向下一个结点的头部域(首地址) */
       }while(h!=p[i].first); /* 没到循环链表的表尾 */
     }
 }

 void PrintUser(Space p[])
 { /* 输出p数组所指的已分配空间 */
   int i;
   for(i=0;i<N;i++)
     if(p[i]) /* 指针不为0(指向一个占用块) */
     {
       printf("占用块%d的首地址=%u ",i,p[i]); /* 输出结点信息 */
       printf("块的大小=%d",(int)pow(2,p[i]->kval));
       printf(" 块标志=%d(0:空闲 1:占用)\n",p[i]->tag);
     }
 }

 int main()
 {
   int i,n;
   FreeList a;
   Space q[N]={NULL}; /* q数组为占用块的首地址 */
   printf("sizeof(WORD_b)=%u m=%u (int)pow(2,m)=%u\n",sizeof(WORD_b),m,(int)pow(2,m));
   for(i=0;i<=m;i++) /* 初始化a */
   {
     a[i].nodesize=(int)pow(2,i);
     a[i].first=NULL;
   }
   r=a[m].first=(WORD_b*)malloc(a[m].nodesize*sizeof(WORD_b)); /* 在最大链表中生成一个结点 */
   if(r) /* 生成结点成功 */
   {
     r->llink=r->rlink=r; /* 初始化该结点 */
     r->tag=0;
     r->kval=m;
     Print(a);
     n=100;
     q[0]=AllocBuddy(a,n); /* 向a申请100个WORD_b的内存(实际获得128个WORD_b) */
     printf("申请%d个字后，可利用空间为:\n",n);
     Print(a);
     PrintUser(q);
     n=200;
     q[1]=AllocBuddy(a,n); /* 向a申请200个WORD_b的内存(实际获得256个WORD_b) */
     printf("申请%d个字后，可利用空间为:\n",n);
     Print(a);
     PrintUser(q);
      n=220;
     q[2]=AllocBuddy(a,n); /* 向a申请220个WORD_b的内存(实际获得256个WORD_b) */
     printf("申请%d个字后，可利用空间为:\n",n);
     Print(a);
     PrintUser(q);
     Reclaim(a,&q[1]); /* 回收q[1]，伙伴不空闲 */
     printf("回收q[1]后，可利用空间为:\n");
     Print(a);
     PrintUser(q);
     Reclaim(a,&q[0]); /* 回收q[0]，伙伴空闲 */
     printf("回收q[0]后，可利用空间为:\n");
     Print(a);
     PrintUser(q);
     Reclaim(a,&q[2]); /* 回收q[2]，伙伴空闲，生成一个大结点 */
     printf("回收q[2]后，可利用空间为:\n");
     Print(a);
     PrintUser(q);
   }
   else
     printf("ERROR\n");
     
   return 0; 
 }