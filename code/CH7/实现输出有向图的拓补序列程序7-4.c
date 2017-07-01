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
 
 #define MAX_NAME 5 /* 顶点字符串的最大长度 */
 typedef int InfoType;
 typedef char VertexType[MAX_NAME]; /* 字符串类型 */
 
 #define MAX_VERTEX_NUM 20
 typedef enum{DG,DN,UDG,UDN}GraphKind; /* {有向图,有向网,无向图,无向网} */
 typedef struct /* 加 */
 {
   int adjvex; /* 该弧所指向的顶点的位置 */
   InfoType *info; /* 网的权值指针 */
 }ElemType;

 typedef struct ArcNode /* 改 */
 {
   ElemType data; /* 除指针以外的部分都属于ElemType */
   struct ArcNode *nextarc; /* 指向下一条弧的指针 */
 }ArcNode; /* 表结点 */

 typedef struct
 {
   VertexType data; /* 顶点信息 */
   ArcNode *firstarc; /* 第一个表结点的地址,指向第一条依附该顶点的弧的指针 */
 }VNode,AdjList[MAX_VERTEX_NUM]; /* 头结点 */

 typedef struct
 {
   AdjList vertices;
   int vexnum,arcnum; /* 图的当前顶点数和弧数 */
   GraphKind kind; /* 图的种类标志 */
 }ALGraph;
 #define LNode ArcNode /* 加，定义单链表的结点类型是图的表结点的类型 */
 #define next nextarc /* 加，定义单链表结点的指针域是表结点指向下一条弧的指针域 */
 typedef ArcNode *LinkList; /* 加，定义指向单链表结点的指针是指向图的表结点的指针 */
 #define DestroyList ClearList /* DestroyList()和ClearList()的操作是一样的 */
 Boolean visited[MAX_VERTEX_NUM]; /* 访问标志数组(全局量) */
 void(*VisitFunc)(char* v); /* 函数变量(全局量) */
 typedef int QElemType; /* 队列元素类型 */
 typedef struct QNode
 {
   QElemType data;
   struct QNode *next;
 }QNode,*QueuePtr;

 typedef struct
 {
   QueuePtr front,rear; /* 队头、队尾指针 */
 }LinkQueue; 
 typedef int SElemType; /* 栈元素类型 */
 #define STACK_INIT_SIZE 10 
 /* 存储空间初始分配量 */
 #define STACK_INCREMENT 2 
 /* 存储空间分配增量 */
 typedef struct SqStack
 {
   SElemType *base; /* 在栈构造之前和销毁之后，base的值为NULL */
   SElemType *top; /* 栈顶指针 */
   int stacksize; /* 当前已分配的存储空间，以元素为单位 */
 }SqStack; /* 顺序栈 */

 void InitList(LinkList *L)
 { /* 操作结果：构造一个空的线性表L */
   *L=NULL; /* 指针为空 */
 }

 void ClearList(LinkList *L)
 { /* 初始条件：线性表L已存在。操作结果：将L重置为空表 */
   LinkList p;
   while(*L) /* L不空 */
   {
     p=*L; /* p指向首元结点 */
     *L=(*L)->next; /* L指向第2个结点(新首元结点) */
     free(p); /* 释放首元结点 */
   }
 }

 Status ListEmpty(LinkList L)
 { /* 初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
   if(L)
     return FALSE;
   else
     return TRUE;
 }

 int ListLength(LinkList L)
 { /* 初始条件：线性表L已存在。操作结果：返回L中数据元素个数 */
   int i=0;
   LinkList p=L;
   while(p) /* p指向结点(没到表尾) */
   {
     p=p->next; /* p指向下一个结点 */
     i++;
   }
   return i;
 }

 Status GetElem(LinkList L,int i,ElemType *e)
 { /* L为不带头结点的单链表的头指针。当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR */
   int j=1;
   LinkList p=L;
   if(i<1) /* i值不合法 */
     return ERROR;
   while(j<i&&p) /* 没到第i个元素，也没到表尾 */
   {
     j++;
     p=p->next;
   }
   if(j==i) /* 存在第i个元素 */
   {
     *e=p->data;
     return OK;
   }
   else
     return ERROR;
 }

 int LocateElem(LinkList L,ElemType e,Status(*compare)(ElemType,ElemType))
 { /* 初始条件：线性表L已存在，compare()是数据元素判定函数(满足为1，否则为0) */
   /* 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。 */
   /*           若这样的数据元素不存在，则返回值为0 */
   int i=0;
   LinkList p=L;
   while(p)
   {
     i++;
     if(compare(p->data,e)) /* 找到这样的数据元素 */
       return i;
     p=p->next;
   }
   return 0;
 }

 Status ListInsert(LinkList *L,int i,ElemType e)
 { /* 在不带头结点的单链线性表L中第i个位置之前插入元素e */
   int j=1;
   LinkList p=*L,s;
   if(i<1) /* i值不合法 */
     return ERROR;
   s=(LinkList)malloc(sizeof(struct LNode)); /* 生成新结点 */
   s->data=e; /* 给s的data域赋值 */
   if(i==1) /* 插在表头 */
   {
     s->next=*L;
     *L=s; /* 改变L */
   }
   else
   { /* 插在表的其余处 */
     while(p&&j<i-1) /* 寻找第i-1个结点 */
     {
       p=p->next;
       j++;
     }
     if(!p) /* i大于表长+1 */
       return ERROR;
     s->next=p->next;
     p->next=s;
   }
   return OK;
 }

 Status ListDelete(LinkList *L,int i,ElemType *e)
 { /* 在不带头结点的单链线性表L中，删除第i个元素，并由e返回其值 */
   int j=1;
   LinkList p=*L,q;
   if(i==1) /* 删除第1个结点 */
   {
     *L=p->next; /* L由第2个结点开始 */
     *e=p->data;
     free(p); /* 删除并释放第1个结点 */
   }
   else
   {
     while(p->next&&j<i-1) /* 寻找第i个结点，并令p指向其前驱 */
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
   }
   return OK;
 }

 void ListTraverse(LinkList L,void(*vi)(ElemType))
 { /* 初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi() */
   LinkList p=L;
   while(p)
   {
     vi(p->data);
     p=p->next;
   }
   printf("\n");
 }
 
 void InsertAscend(LinkList *L,ElemType e,int(*compare)(ElemType,ElemType))
 { /* 按关键字非降序将e插入表L。函数compare()返回值为：形参1的关键字-形参2的关键字 */
   LinkList q=*L;
   if(!*L||compare(e,(*L)->data)<=0) /* 链表空或e的关键字小于等于首结点的关键字 */
     ListInsert(L,1,e); /* 将e插在表头，在bo2-8.c中 */
   else
   {
     while(q->next&&compare(q->next->data,e)<0) /* q不是尾结点且q的下一结点关键字<e的关键字 */
       q=q->next;
     ListInsert(&q,2,e); /* 插在q所指结点后(将q作为头指针) */
   }
 }

 LinkList Point(LinkList L,ElemType e,Status(*equal)(ElemType,ElemType),LinkList *p)
 { /* 查找表L中满足条件的结点。如找到，返回指向该结点的指针，p指向该结点的前驱(若该结点是 */
   /* 首元结点，则p=NULL)。如表L中无满足条件的结点，则返回NULL，p无定义。*/
   /* 函数equal()的两形参的关键字相等，返回OK；否则返回ERROR */
   int i,j;
   i=LocateElem(L,e,equal);
   if(i) /* 找到 */
   {
     if(i==1) /* 是首元结点 */
     {
       *p=NULL;
       return L;
     }
     *p=L;
     for(j=2;j<i;j++)
       *p=(*p)->next;
     return (*p)->next;
   }
   return NULL; /* 没找到 */
 }

 Status DeleteElem(LinkList *L,ElemType *e,Status(*equal)(ElemType,ElemType))
 { /* 删除表L中满足条件的结点，并返回TRUE；如无此结点，则返回FALSE。 */
   /* 函数equal()的两形参的关键字相等，返回OK；否则返回ERROR */
   LinkList p,q;
   q=Point(*L,*e,equal,&p);
   if(q) /* 找到此结点，且q指向该结点 */
   {
     if(p) /* 该结点不是首元结点，p指向其前驱 */
       ListDelete(&p,2,e); /* 将p作为头指针，删除第2个结点 */
     else /* 该结点是首元结点 */
       ListDelete(L,1,e);
     return TRUE;
   }
   return FALSE;
 }
 
 int LocateVex(ALGraph G,VertexType u)
 { /* 初始条件：图G存在，u和G中顶点有相同特征 */
   /* 操作结果：若G中存在顶点u，则返回该顶点在图中位置；否则返回-1 */
   int i;
   for(i=0;i<G.vexnum;++i)
     if(strcmp(u,G.vertices[i].data)==0)
       return i;
   return -1;
 }

 void CreateGraph(ALGraph *G)
 { /* 采用邻接表存储结构，构造没有相关信息图或网G(用一个函数构造4种图) */
   int i,j,k,w; /* w是权值 */
   VertexType va,vb; /* 连接边或弧的2顶点 */
   ElemType e;
   printf("请输入图的类型(有向图:0,有向网:1,无向图:2,无向网:3): ");
   scanf("%d",&(*G).kind);
   printf("请输入图的顶点数,边数: ");
   scanf("%d,%d",&(*G).vexnum,&(*G).arcnum);
   printf("请输入%d个顶点的值(<%d个字符):\n",(*G).vexnum,MAX_NAME);
   for(i=0;i<(*G).vexnum;++i) /* 构造顶点向量 */
   {
     scanf("%s",(*G).vertices[i].data);
     (*G).vertices[i].firstarc=NULL; /* 初始化与该顶点有关的出弧链表 */
   }
   if((*G).kind%2) /* 网 */
     printf("请输入每条弧(边)的权值、弧尾和弧头(以空格作为间隔):\n");
   else /* 图 */
     printf("请输入每条弧(边)的弧尾和弧头(以空格作为间隔):\n");
   for(k=0;k<(*G).arcnum;++k) /* 构造相关弧链表 */
   {
     if((*G).kind%2) /* 网 */
       scanf("%d%s%s",&w,va,vb);
     else /* 图 */
       scanf("%s%s",va,vb);
     i=LocateVex(*G,va); /* 弧尾 */
     j=LocateVex(*G,vb); /* 弧头 */
     e.info=NULL; /* 给待插表结点e赋值，图无权 */
     e.adjvex=j; /* 弧头 */
     if((*G).kind%2) /* 网 */
     {
       e.info=(int *)malloc(sizeof(int)); /* 动态生成存放权值的空间 */
       *(e.info)=w;
     }
     ListInsert(&(*G).vertices[i].firstarc,1,e); /* 插在第i个元素(出弧)的表头，在bo2-8.c中 */
     if((*G).kind>=2) /* 无向图或网，产生第2个表结点，并插在第j个元素(入弧)的表头 */
     {
       e.adjvex=i; /* e.info不变，不必再赋值 */
       ListInsert(&(*G).vertices[j].firstarc,1,e); /* 插在第j个元素的表头，在bo2-8.c中 */
     }
   }
 }

 void CreateGraphF(ALGraph *G)
 { /* 采用邻接表 存储结构，由文件构造没有相关信息图或网G(用一个函数构造4种图) */
   int i,j,k,w; /* w是权值 */
   VertexType va,vb; /* 连接边或弧的2顶点 */
   ElemType e;
   char filename[13];
   FILE *graphlist;
   printf("请输入数据文件名(f7-1.txt或f7-2.txt)：");
   scanf("%s",filename);
   printf("请输入图的类型(有向图:0,有向网:1,无向图:2,无向网:3): ");
   scanf("%d",&(*G).kind);
   graphlist=fopen(filename,"r"); /* 以读的方式打开数据文件，并以graphlist表示 */
   fscanf(graphlist,"%d",&(*G).vexnum);
   fscanf(graphlist,"%d",&(*G).arcnum);
   for(i=0;i<(*G).vexnum;++i) /* 构造顶点向量 */
   {
     fscanf(graphlist,"%s",(*G).vertices[i].data);
     (*G).vertices[i].firstarc=NULL; /* 初始化与该顶点有关的出弧链表 */
   }
   for(k=0;k<(*G).arcnum;++k) /* 构造相关弧链表 */
   {
     if((*G).kind%2) /* 网 */
       fscanf(graphlist,"%d%s%s",&w,va,vb);
     else /* 图 */
       fscanf(graphlist,"%s%s",va,vb);
     i=LocateVex(*G,va); /* 弧尾 */
     j=LocateVex(*G,vb); /* 弧头 */
     e.info=NULL; /* 给待插表结点e赋值，图无权 */
     e.adjvex=j; /* 弧头 */
     if((*G).kind%2) /* 网 */
     {
       e.info=(int *)malloc(sizeof(int)); /* 动态生成存放权值的空间 */
       *(e.info)=w;
     }
     ListInsert(&(*G).vertices[i].firstarc,1,e); /* 插在第i个元素(出弧)的表头，在bo2-8.c中 */
     if((*G).kind>=2) /* 无向图或网，产生第2个表结点，并插在第j个元素(入弧)的表头 */
     {
       e.adjvex=i; /* e.info不变，不必再赋值 */
       ListInsert(&(*G).vertices[j].firstarc,1,e); /* 插在第j个元素的表头，在bo2-8.c中 */
     }
   }
   fclose(graphlist); /* 关闭数据文件 */
 }

 void DestroyGraph(ALGraph *G)
 { /* 初始条件：图G存在。操作结果：销毁图G */
   int i;
   ElemType e;
   for(i=0;i<(*G).vexnum;++i) /* 对于所有顶点 */
     if((*G).kind%2) /* 网 */
       while((*G).vertices[i].firstarc) /* 对应的弧或边链表不空 */
       {
	 ListDelete(&(*G).vertices[i].firstarc,1,&e); /* 删除链表的第1个结点，并将值赋给e */
         if(e.adjvex>i) /* 顶点序号>i(保证动态生成的权值空间只释放1次) */
           free(e.info);
       }
     else /* 图 */
       DestroyList(&(*G).vertices[i].firstarc); /* 销毁弧或边链表，在bo2-8.c中 */
   (*G).vexnum=0; /* 顶点数为0 */
   (*G).arcnum=0; /* 边或弧数为0 */
 }

 VertexType* GetVex(ALGraph G,int v)
 { /* 初始条件：图G存在，v是G中某个顶点的序号。操作结果：返回v的值 */
   if(v>=G.vexnum||v<0)
     exit(ERROR);
   return &G.vertices[v].data;
 }

 Status PutVex(ALGraph *G,VertexType v,VertexType value)
 { /* 初始条件：图G存在，v是G中某个顶点。操作结果：对v赋新值value */
   int i;
   i=LocateVex(*G,v);
   if(i>-1) /* v是G的顶点 */
   {
     strcpy((*G).vertices[i].data,value);
     return OK;
   }
   return ERROR;
 }

 int FirstAdjVex(ALGraph G,VertexType v)
 { /* 初始条件：图G存在，v是G中某个顶点 */
   /* 操作结果：返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点，则返回-1 */
   LinkList p;
   int v1;
   v1=LocateVex(G,v); /* v1为顶点v在图G中的序号 */
   p=G.vertices[v1].firstarc;
   if(p)
     return p->data.adjvex;
   else
     return -1;
 }

 Status equalvex(ElemType a,ElemType b)
 { /* DeleteArc()、DeleteVex()和NextAdjVex()要调用的函数 */
   if(a.adjvex==b.adjvex)
     return OK;
   else
     return ERROR;
 }

 int NextAdjVex(ALGraph G,VertexType v,VertexType w)
 { /* 初始条件：图G存在，v是G中某个顶点，w是v的邻接顶点 */
   /* 操作结果：返回v的(相对于w的)下一个邻接顶点的序号。若w是v的最后一个邻接点，则返回-1 */
   LinkList p,p1; /* p1在Point()中用作辅助指针，Point()在func2-1.c中 */
   ElemType e;
   int v1;
   v1=LocateVex(G,v); /* v1为顶点v在图G中的序号 */
   e.adjvex=LocateVex(G,w); /* e.adjvex为顶点w在图G中的序号 */
   p=Point(G.vertices[v1].firstarc,e,equalvex,&p1); /* p指向顶点v的链表中邻接顶点为w的结点 */
   if(!p||!p->next) /* 没找到w或w是最后一个邻接点 */
     return -1;
   else /* p->data.adjvex==w */
     return p->next->data.adjvex; /* 返回v的(相对于w的)下一个邻接顶点的序号 */
 }

 void InsertVex(ALGraph *G,VertexType v)
 { /* 初始条件：图G存在，v和图中顶点有相同特征 */
   /* 操作结果：在图G中增添新顶点v(不增添与顶点相关的弧，留待InsertArc()去做) */
   strcpy((*G).vertices[(*G).vexnum].data,v); /* 构造新顶点向量 */
   (*G).vertices[(*G).vexnum].firstarc=NULL;
   (*G).vexnum++; /* 图G的顶点数加1 */
 }

 Status DeleteVex(ALGraph *G,VertexType v)
 { /* 初始条件：图G存在，v是G中某个顶点。操作结果：删除G中顶点v及其相关的弧 */
   int i,j,k;
   ElemType e;
   LinkList p,p1;
   j=LocateVex(*G,v); /* j是顶点v的序号 */
   if(j<0) /* v不是图G的顶点 */
     return ERROR;
   i=ListLength((*G).vertices[j].firstarc); /* 以v为出度的弧或边数，在bo2-8.c中 */
   (*G).arcnum-=i; /* 边或弧数-i */
   if((*G).kind%2) /* 网 */
     while((*G).vertices[j].firstarc) /* 对应的弧或边链表不空 */
     {
       ListDelete(&(*G).vertices[j].firstarc,1,&e); /* 删除链表的第1个结点，并将值赋给e */
       free(e.info); /* 释放动态生成的权值空间 */
     }
   else /* 图 */
     DestroyList(&(*G).vertices[i].firstarc); /* 销毁弧或边链表，在bo2-8.c中 */
   (*G).vexnum--; /* 顶点数减1 */
   for(i=j;i<(*G).vexnum;i++) /* 顶点v后面的顶点前移 */
     (*G).vertices[i]=(*G).vertices[i+1];
   for(i=0;i<(*G).vexnum;i++) /* 删除以v为入度的弧或边且必要时修改表结点的顶点位置值 */
   {
     e.adjvex=j;
     p=Point((*G).vertices[i].firstarc,e,equalvex,&p1); /* Point()在func2-1.c中 */
     if(p) /* 顶点i的邻接表上有v为入度的结点 */
     {
       if(p1) /* p1指向p所指结点的前驱 */
         p1->next=p->next; /* 从链表中删除p所指结点 */
       else /* p指向首元结点 */
         (*G).vertices[i].firstarc=p->next; /* 头指针指向下一结点 */
       if((*G).kind<2) /* 有向 */
       {
         (*G).arcnum--; /* 边或弧数-1 */
         if((*G).kind==1) /* 有向网 */
           free(p->data.info); /* 释放动态生成的权值空间 */
       }
       free(p); /* 释放v为入度的结点 */
     }
     for(k=j+1;k<=(*G).vexnum;k++) /* 对于adjvex域>j的结点，其序号-1 */
     {
       e.adjvex=k;
       p=Point((*G).vertices[i].firstarc,e,equalvex,&p1); /* Point()在func2-1.c中 */
       if(p)
         p->data.adjvex--; /* 序号-1(因为前移) */
     }
   }
   return OK;
 }

 Status InsertArc(ALGraph *G,VertexType v,VertexType w)
 { /* 初始条件：图G存在，v和w是G中两个顶点 */
   /* 操作结果：在G中增添弧<v,w>，若G是无向的，则还增添对称弧<w,v> */
   ElemType e;
   int i,j;
   i=LocateVex(*G,v); /* 弧尾或边的序号 */
   j=LocateVex(*G,w); /* 弧头或边的序号 */
   if(i<0||j<0)
     return ERROR;
   (*G).arcnum++; /* 图G的弧或边的数目加1 */
   e.adjvex=j;
   e.info=NULL; /* 初值 */
   if((*G).kind%2) /* 网 */
   {
     e.info=(int *)malloc(sizeof(int)); /* 动态生成存放权值的空间 */
     printf("请输入弧(边)%s→%s的权值: ",v,w);
     scanf("%d",e.info);
   }
   ListInsert(&(*G).vertices[i].firstarc,1,e); /* 将e插在弧尾的表头，在bo2-8.c中 */
   if((*G).kind>=2) /* 无向，生成另一个表结点 */
   {
     e.adjvex=i; /* e.info不变 */
     ListInsert(&(*G).vertices[j].firstarc,1,e); /* 将e插在弧头的表头 */
   }
   return OK;
 }

 Status DeleteArc(ALGraph *G,VertexType v,VertexType w)
 { /* 初始条件：图G存在，v和w是G中两个顶点 */
   /* 操作结果：在G中删除弧<v,w>，若G是无向的，则还删除对称弧<w,v> */
   int i,j;
   Status k;
   ElemType e;
   i=LocateVex(*G,v); /* i是顶点v(弧尾)的序号 */
   j=LocateVex(*G,w); /* j是顶点w(弧头)的序号 */
   if(i<0||j<0||i==j)
     return ERROR;
   e.adjvex=j;
   k=DeleteElem(&(*G).vertices[i].firstarc,&e,equalvex); /* 在func2-1.c中 */
   if(k) /* 删除成功 */
   {
     (*G).arcnum--; /* 弧或边数减1 */
     if((*G).kind%2) /* 网 */
       free(e.info);
     if((*G).kind>=2) /* 无向，删除对称弧<w,v> */
     {
       e.adjvex=i;
       DeleteElem(&(*G).vertices[j].firstarc,&e,equalvex);
     }
     return OK;
   }
   else /* 没找到待删除的弧 */
     return ERROR;
 }

 void DFS(ALGraph G,int v)
 { /* 从第v个顶点出发递归地深度优先遍历图G。算法7.5 */
   int w;
   visited[v]=TRUE; /* 设置访问标志为TRUE(已访问) */
   VisitFunc(G.vertices[v].data); /* 访问第v个顶点 */
   for(w=FirstAdjVex(G,G.vertices[v].data);w>=0;w=NextAdjVex(G,G.vertices[v].data,G.vertices[w].data))
     if(!visited[w])
       DFS(G,w); /* 对v的尚未访问的邻接点w递归调用DFS */
 }

 void DFSTraverse(ALGraph G,void(*Visit)(char*))
 { /* 对图G作深度优先遍历。算法7.4 */
   int v;
   VisitFunc=Visit; /* 使用全局变量VisitFunc，使DFS不必设函数指针参数 */
   for(v=0;v<G.vexnum;v++)
     visited[v]=FALSE; /* 访问标志数组初始化 */
   for(v=0;v<G.vexnum;v++)
     if(!visited[v])
       DFS(G,v); /* 对尚未访问的顶点调用DFS */
   printf("\n");
 }

 void InitQueue(LinkQueue *Q)
 { /* 构造一个空队列Q */
   (*Q).front=(*Q).rear=(QueuePtr)malloc(sizeof(QNode));
   if(!(*Q).front)
     exit(OVERFLOW);
   (*Q).front->next=NULL;
 }

 void DestroyQueue(LinkQueue *Q)
 { /* 销毁队列Q(无论空否均可) */
   while((*Q).front)
   {
     (*Q).rear=(*Q).front->next;
     free((*Q).front);
     (*Q).front=(*Q).rear;
   }
 }

 void ClearQueue(LinkQueue *Q)
 { /* 将Q清为空队列 */
   QueuePtr p,q;
   (*Q).rear=(*Q).front;
   p=(*Q).front->next;
   (*Q).front->next=NULL;
   while(p)
   {
     q=p;
     p=p->next;
     free(q);
   }
 }

 Status QueueEmpty(LinkQueue Q)
 { /* 若Q为空队列，则返回TRUE，否则返回FALSE */
   if(Q.front->next==NULL)
     return TRUE;
   else
     return FALSE;
 }

 int QueueLength(LinkQueue Q)
 { /* 求队列的长度 */
   int i=0;
   QueuePtr p;
   p=Q.front;
   while(Q.rear!=p)
   {
     i++;
     p=p->next;
   }
   return i;
 }

 Status GetHead_Q(LinkQueue Q,QElemType *e) /* 避免与bo2-6.c重名 */
 { /* 若队列不空，则用e返回Q的队头元素，并返回OK，否则返回ERROR */
   QueuePtr p;
   if(Q.front==Q.rear)
     return ERROR;
   p=Q.front->next;
   *e=p->data;
   return OK;
 }

 void EnQueue(LinkQueue *Q,QElemType e)
 { /* 插入元素e为Q的新的队尾元素 */
   QueuePtr p=(QueuePtr)malloc(sizeof(QNode));
   if(!p) /* 存储分配失败 */
     exit(OVERFLOW);
   p->data=e;
   p->next=NULL;
   (*Q).rear->next=p;
   (*Q).rear=p;
 }

 Status DeQueue(LinkQueue *Q,QElemType *e)
 { /* 若队列不空，删除Q的队头元素，用e返回其值，并返回OK，否则返回ERROR */
   QueuePtr p;
   if((*Q).front==(*Q).rear)
     return ERROR;
   p=(*Q).front->next;
   *e=p->data;
   (*Q).front->next=p->next;
   if((*Q).rear==p)
     (*Q).rear=(*Q).front;
   free(p);
   return OK;
 }

 void QueueTraverse(LinkQueue Q,void(*vi)(QElemType))
 { /* 从队头到队尾依次对队列Q中每个元素调用函数vi() */
   QueuePtr p;
   p=Q.front->next;
   while(p)
   {
     vi(p->data);
     p=p->next;
   }
   printf("\n");
 }
 
 void BFSTraverse(ALGraph G,void(*Visit)(char*))
 {/*按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。算法7.6 */
   int v,u,w;
   LinkQueue Q;
   for(v=0;v<G.vexnum;++v)
     visited[v]=FALSE; /* 置初值 */
   InitQueue(&Q); /* 置空的辅助队列Q */
   for(v=0;v<G.vexnum;v++) /* 如果是连通图，只v=0就遍历全图 */
     if(!visited[v]) /* v尚未访问 */
     {
       visited[v]=TRUE;
       Visit(G.vertices[v].data);
       EnQueue(&Q,v); /* v入队列 */
       while(!QueueEmpty(Q)) /* 队列不空 */
       {
         DeQueue(&Q,&u); /* 队头元素出队并置为u */
         for(w=FirstAdjVex(G,G.vertices[u].data);w>=0;w=NextAdjVex(G,G.vertices[u].data,G.vertices[w].data))
           if(!visited[w]) /* w为u的尚未访问的邻接顶点 */
           {
             visited[w]=TRUE;
             Visit(G.vertices[w].data);
             EnQueue(&Q,w); /* w入队 */
           }
       }
     }
   printf("\n");
 }

 void DFS1(ALGraph G,int v,void(*Visit)(char*))
 { /* 从第v个顶点出发递归地深度优先遍历图G。仅适用于邻接表存储结构 */
   ArcNode *p; /* p指向表结点 */
   visited[v]=TRUE; /* 设置访问标志为TRUE(已访问) */
   Visit(G.vertices[v].data); /* 访问该顶点 */
   for(p=G.vertices[v].firstarc;p;p=p->next) /* p依次指向v的邻接顶点 */
     if(!visited[p->data.adjvex])
       DFS1(G,p->data.adjvex,Visit); /* 对v的尚未访问的邻接点递归调用DFS1 */
 }

 void DFSTraverse1(ALGraph G,void(*Visit)(char*))
 { /* 对图G作深度优先遍历。DFS1设函数指针参数 */
   int v;
   for(v=0;v<G.vexnum;v++)
     visited[v]=FALSE; /* 访问标志数组初始化，置初值为未被访问 */
   for(v=0;v<G.vexnum;v++) /* 如果是连通图，只v=0就遍历全图 */
     if(!visited[v]) /* v尚未被访问 */
       DFS1(G,v,Visit); /* 对v调用DFS1 */
   printf("\n");
 }

 void BFSTraverse1(ALGraph G,void(*Visit)(char*))
 { /* 按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。仅适用于邻接表结构 */
   int v,u;
   ArcNode *p; /* p指向表结点 */
   LinkQueue Q; /* 链队列类型 */
   for(v=0;v<G.vexnum;++v)
     visited[v]=FALSE; /* 置初值为未被访问 */
   InitQueue(&Q); /* 初始化辅助队列Q */
   for(v=0;v<G.vexnum;v++) /* 如果是连通图，只v=0就遍历全图 */
     if(!visited[v]) /* v尚未被访问 */
     {
       visited[v]=TRUE; /* 设v为已被访问 */
       Visit(G.vertices[v].data); /* 访问v */
       EnQueue(&Q,v); /* v入队列 */
       while(!QueueEmpty(Q)) /* 队列不空 */
       {
         DeQueue(&Q,&u); /* 队头元素出队并置为u */
         for(p=G.vertices[u].firstarc;p;p=p->next) /* p依次指向u的邻接顶点 */
           if(!visited[p->data.adjvex]) /* u的邻接顶点尚未被访问 */
           {
             visited[p->data.adjvex]=TRUE; /* 该邻接顶点设为已被访问 */
             Visit(G.vertices[p->data.adjvex].data); /* 访问该邻接顶点 */
             EnQueue(&Q,p->data.adjvex); /* 入队该邻接顶点序号 */
           }
       }
     }
   printf("\n");
 }

 void Display(ALGraph G)
 { /* 输出图的邻接矩阵G */
   int i;
   LinkList p;
   switch(G.kind)
   {
     case DG: printf("有向图\n");
	      break;
     case DN: printf("有向网\n");
              break;
     case UDG:printf("无向图\n");
              break;
     case UDN:printf("无向网\n");
   }
   printf("%d个顶点：\n",G.vexnum);
   for(i=0;i<G.vexnum;++i)
     printf("%s ",G.vertices[i].data);
   printf("\n%d条弧(边):\n",G.arcnum);
   for(i=0;i<G.vexnum;i++)
   {
     p=G.vertices[i].firstarc;
     while(p)
     {
       if(G.kind<=1||i<p->data.adjvex) /* 有向或无向两次中的一次 */
       {
         printf("%s→%s ",G.vertices[i].data,G.vertices[p->data.adjvex].data);
	 if(G.kind%2) /* 网 */
           printf(":%d ",*(p->data.info));
       }
       p=p->nextarc;
     }
     printf("\n");
   }
 }
 
 void FindInDegree(ALGraph G,int indegree[])
 { /* 求顶点的入度，算法7.12、7.13调用 */
   int i;
   ArcNode *p;
   for(i=0;i<G.vexnum;i++)
     indegree[i]=0; /* 赋初值 */
   for(i=0;i<G.vexnum;i++)
   {
     p=G.vertices[i].firstarc;
     while(p)
     {
       indegree[p->data.adjvex]++;
       p=p->nextarc;
     }
   }
 }
 
 void InitStack(SqStack *S)
 { /* 构造一个空栈S */
   (*S).base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
   if(!(*S).base)
     exit(OVERFLOW); /* 存储分配失败 */
   (*S).top=(*S).base;
   (*S).stacksize=STACK_INIT_SIZE;
 }

 void DestroyStack(SqStack *S)
 { /* 销毁栈S，S不再存在 */
   free((*S).base);
   (*S).base=NULL;
   (*S).top=NULL;
   (*S).stacksize=0;
 }

 void ClearStack(SqStack *S)
 { /* 把S置为空栈 */
   (*S).top=(*S).base;
 }

 Status StackEmpty(SqStack S)
 { /* 若栈S为空栈，则返回TRUE，否则返回FALSE */
   if(S.top==S.base)
     return TRUE;
   else
     return FALSE;
 }

 int StackLength(SqStack S)
 { /* 返回S的元素个数，即栈的长度 */
   return S.top-S.base;
 }

 Status GetTop(SqStack S,SElemType *e)
 { /* 若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR */
   if(S.top>S.base)
   {
     *e=*(S.top-1);
     return OK;
   }
   else
     return ERROR;
 }

 void Push(SqStack *S,SElemType e)
 { /* 插入元素e为新的栈顶元素 */
   if((*S).top-(*S).base>=(*S).stacksize) /* 栈满，追加存储空间 */
   {
     (*S).base=(SElemType *)realloc((*S).base,((*S).stacksize+STACK_INCREMENT)*sizeof(SElemType));
     if(!(*S).base)
       exit(OVERFLOW); /* 存储分配失败 */
     (*S).top=(*S).base+(*S).stacksize;
     (*S).stacksize+=STACK_INCREMENT;
   }
   *((*S).top)++=e;
 }

 Status Pop(SqStack *S,SElemType *e)
 { /* 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR */
   if((*S).top==(*S).base)
     return ERROR;
   *e=*--(*S).top;
   return OK;
 }

 void StackTraverse(SqStack S,void(*visit)(SElemType))
 { /* 从栈底到栈顶依次对栈中每个元素调用函数visit() */
   while(S.top>S.base)
     visit(*S.base++);
   printf("\n");
 } 


 Status TopologicalSort(ALGraph G)
 { /* 有向图G采用邻接表存储结构。若G无回路，则输出G的顶点的一个拓扑序列并返回OK，*/
   /* 否则返回ERROR。算法7.12 */
   int i,k,count=0; /* 已输出顶点数，初值为0 */
   int indegree[MAX_VERTEX_NUM]; /* 入度数组，存放各顶点当前入度数 */
   SqStack S;
   ArcNode *p;
   FindInDegree(G,indegree); /* 对各顶点求入度indegree[]，在func7-1.c中 */
   InitStack(&S); /* 初始化零入度顶点栈S */
   for(i=0;i<G.vexnum;++i) /* 对所有顶点i */
     if(!indegree[i]) /* 若其入度为0 */
       Push(&S,i); /* 将i入零入度顶点栈S */
   while(!StackEmpty(S)) /* 当零入度顶点栈S不空 */
   {
     Pop(&S,&i); /* 出栈1个零入度顶点的序号，并将其赋给i */
     printf("%s ",G.vertices[i].data); /* 输出i号顶点 */
     ++count; /* 已输出顶点数+1 */
     for(p=G.vertices[i].firstarc;p;p=p->nextarc)
     { /* 对i号顶点的每个邻接顶点 */
       k=p->data.adjvex; /* 其序号为k */
       if(!(--indegree[k])) /* k的入度减1，若减为0，则将k入栈S */
         Push(&S,k);
     }
   }
   if(count<G.vexnum) /* 零入度顶点栈S已空，图G还有顶点未输出 */
   {
     printf("此有向图有回路\n");
     return ERROR;
   }
   else
   {
     printf("为一个拓扑序列。\n");
     return OK;
   }
 }

 void main()
 {
   ALGraph f;
   printf("请选择有向图\n");
   CreateGraph(&f); /* 构造有向图f，在bo7-2.c中 */
   Display(f); /* 输出有向图f，在bo7-2.c中 */
   TopologicalSort(f); /* 输出有向图f的1个拓扑序列 */
 }
 
 
 
 
