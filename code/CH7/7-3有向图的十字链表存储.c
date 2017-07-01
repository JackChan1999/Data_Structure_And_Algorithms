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
 
 typedef int InfoType; /* 权值类型 */
 #define MAX_VERTEX_NAME 3  /* 顶点字符串最大长度+1 */
 typedef char  VertexType[MAX_VERTEX_NAME];
 
 #define MAX_VERTEX_NUM 20
 typedef struct ArcBox1 /* 用来定义hlink的类型 */
 {
   int tailvex,headvex; /* 该弧的尾和头顶点的位置 */
   InfoType *info; /* 该弧相关信息的指针，可指向权值或其他信息 */
   struct ArcBox1 *hlink,*tlink; /* 分别为弧头相同弧尾相同的弧的链域 */
 }ArcBox1;
 typedef struct
 {
   int tailvex,headvex; /* 该弧的尾和头顶点的位置 */
   InfoType *info; /* 该弧相关信息的指针，可指向权值或其他信息 */
   ArcBox1 *hlink;
 }ElemType;
 typedef struct ArcBox
 {
   ElemType data;
   struct ArcBox *tlink;
 }ArcBox;
 typedef struct /* 顶点结点 */
 {
   VertexType data;
   ArcBox1 *firstin; /* 指向该顶点第一条入弧 */
   ArcBox *firstout; /* 指向该顶点第一条出弧 */
 }VexNode;
 typedef struct
 {
   VexNode xlist[MAX_VERTEX_NUM]; /* 表头向量(数组) */
   int vexnum,arcnum; /* 有向图的当前顶点数和弧数 */
 }OLGraph;
 #define LNode ArcBox /* 加，定义单链表的结点类型是图的表结点的类型 */
 #define next tlink /* 加，定义单链表结点的指针域是表结点指向下一条出弧的指针域 */
 typedef ArcBox *LinkList; /* 加，定义指向单链表结点的指针是指向图的表结点的指针 */
 
 #define DestroyList ClearList /* DestroyList()和ClearList()的操作是一样的 */
 Boolean visited[MAX_VERTEX_NUM]; /* 访问标志数组 */
 void(*VisitFunc)(VertexType); /* 函数变量 */
 
 typedef int QElemType; /* 队列元素类型 */
 typedef struct QNode
 {
 	QElemType data;
 	struct QNode *next;
 }QNode, * QueuePtr;
 typedef struct 
 {
 	QueuePtr front, rear;//队头，队尾指针 
 }LinkQueue;
 
 
void InitList(LinkList *L);
void ClearList(LinkList *L);
Status ListEmpty(LinkList L);
int ListLength(LinkList L);
Status GetElem(LinkList L, int i, ElemType *e);
int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType, ElemType));
Status ListInsert(LinkList *L, int i, ElemType e);
Status ListDelete(LinkList *L, int i, ElemType *e);
void ListTraverse(LinkList L, void(*vi)(ElemType));
int LocateVex(OLGraph G, VertexType u);
void CreateDG(OLGraph *G);
void DestroyGraph(OLGraph *G);
VertexType* GetVex(OLGraph G, int v);
Status PutVex(OLGraph *G, VertexType v, VertexType value);
int FirstAdjVex(OLGraph G, VertexType v);
int NextAdjVex(OLGraph G, VertexType v, VertexType w);
void InsertVex(OLGraph *G, VertexType v);
Status equal(ElemType c1, ElemType c2);
Status DeleteVex(OLGraph *G, VertexType v);
Status InsertArc(OLGraph *G, VertexType v, VertexType w);
Status DeleteArc(OLGraph *G, VertexType v, VertexType w);
void DFS(OLGraph G, int i); /* DFSTraverse()调用 */
void DFSTraverse(OLGraph G, void(*Visit)(VertexType));
void InitQueue(LinkQueue *Q);
void DestroyQueue(LinkQueue *Q);
void ClearQueue(LinkQueue *Q);
Status QueueEmpty(LinkQueue Q);
int QueueLength(LinkQueue Q);
Status GetHead_Q(LinkQueue Q, QElemType *e);//避免重名
void EnQueue(LinkQueue *Q, QElemType e);
Status DeQueue(LinkQueue *Q, QElemType *e);
void QueueTraverse(LinkQueue Q, void(*vi)(QElemType));
void BFSTraverse(OLGraph G, void(*Visit)(VertexType));
void Display(OLGraph G);
void visit(VertexType v);

 int main()
 {
 	int j,k,n;
   OLGraph g;
   VertexType v1,v2;
   CreateDG(&g);
   Display(g);
   printf("修改顶点的值，请输入原值 新值: ");
   scanf("%s%s",v1,v2);
   PutVex(&g,v1,v2);
   printf("插入新顶点，请输入顶点的值: ");
   scanf("%s",v1);
   InsertVex(&g,v1);
   printf("插入与新顶点有关的弧，请输入弧数: ");
   scanf("%d",&n);
   for(k=0;k<n;k++)
   {
     printf("请输入另一顶点的值 另一顶点的方向(0:弧头 1:弧尾): ");
     scanf("%s%d",v2,&j);
     if(j)
       InsertArc(&g,v2,v1);
     else
       InsertArc(&g,v1,v2);
   }
   Display(g);
   printf("删除一条弧，请输入待删除弧的弧尾 弧头：");
   scanf("%s%s",v1,v2);
   DeleteArc(&g,v1,v2);
   Display(g);
   printf("删除顶点及相关的弧，请输入顶点的值: ");
   scanf("%s",v1);
   DeleteVex(&g,v1);
   Display(g);
   printf("深度优先搜索的结果:\n");
   DFSTraverse(g,visit);
   printf("广度优先搜索的结果:\n");
   BFSTraverse(g,visit);
   DestroyGraph(&g);
   
 	return 0;
 }

 
 
 
 
 
 void InitList(LinkList *L)
 { 
   /* 操作结果：构造一个空的线性表L */
   *L = NULL; /* 指针为空 */
 }

 void ClearList(LinkList *L)
 { 
   /* 初始条件：线性表L已存在。操作结果：将L重置为空表 */
   LinkList p;
   while(*L) /* L不空 */
   {
     p = *L; /* p指向首元结点 */
     *L = (*L)->next; /* L指向第2个结点(新首元结点) */
     free(p); /* 释放首元结点 */
   }
 }

 Status ListEmpty(LinkList L)
 { 
   /* 初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
   if(L)
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
   /* 初始条件：线性表L已存在。操作结果：返回L中数据元素个数 */
   int i = 0;
   LinkList p = L;
   
   while(p) /* p指向结点(没到表尾) */
   {
     p = p->next; /* p指向下一个结点 */
     i++;
   }
   return i;
 }

 Status GetElem(LinkList L, int i, ElemType *e)
 { 
   /* L为不带头结点的单链表的头指针。当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR */
   int j = 1;
   LinkList p = L;
   
   if(i < 1) /* i值不合法 */
   { 
     return ERROR;
   } 
   while(j<i && p) /* 没到第i个元素，也没到表尾 */
   {
     j++;
     p = p->next;
   }
   if(j == i) /* 存在第i个元素 */
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
   /* 初始条件：线性表L已存在，compare()是数据元素判定函数(满足为1，否则为0) */
   /* 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。 */
   /*           若这样的数据元素不存在，则返回值为0 */
   int i = 0;
   LinkList p = L;
   
   while(p)
   {
     i++;
     if(compare(p->data, e)) /* 找到这样的数据元素 */
     { 
       return i;
     } 
	 p = p->next;
   }
   return 0;
 }

 Status ListInsert(LinkList *L, int i, ElemType e)
 { 
   /* 在不带头结点的单链线性表L中第i个位置之前插入元素e */
   int j = 1;
   LinkList p = *L, s;
   if(i < 1) /* i值不合法 */
   { 
     return ERROR;
   } 
   s = (LinkList)malloc(sizeof(struct LNode)); /* 生成新结点 */
   s->data = e; /* 给s的data域赋值 */
   if(i == 1) /* 插在表头 */
   {
     s->next = *L;
     *L = s; /* 改变L */
   }
   else
   { 
     /* 插在表的其余处 */
     while(p && j<i-1) /* 寻找第i-1个结点 */
     {
       p = p->next;
       j++;
     }
     if(! p) /* i大于表长+1 */
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
   /* 在不带头结点的单链线性表L中，删除第i个元素，并由e返回其值 */
   int j = 1;
   LinkList p = *L, q;
   if(i == 1) /* 删除第1个结点 */
   {
     *L = p->next; /* L由第2个结点开始 */
     *e = p->data;
     free(p); /* 删除并释放第1个结点 */
   }
   else
   {
     while(p->next && j<i-1) /* 寻找第i个结点，并令p指向其前驱 */
     {
       p = p->next;
       j++;
     }
     if(!p->next || j>i-1) /* 删除位置不合理 */
     { 
       return ERROR;
     } 
	 q = p->next; /* 删除并释放结点 */
     p->next = q->next;
     *e = q->data;
     free(q);
   }
   
   return OK;
 }

 void ListTraverse(LinkList L, void(*vi)(ElemType))
 { 
   /* 初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi() */
   LinkList p = L;
   while(p)
   {
     vi(p->data);
     p = p->next;
   }
   printf("\n");
 }
 
 int LocateVex(OLGraph G, VertexType u)
 { 
   /* 返回顶点u在有向图G中的位置(序号)，如不存在则返回-1 */
   int i;
   for(i=0; i<G.vexnum; ++i) /* 用循环查找该结点 */
   {  
     if(! strcmp(G.xlist[i].data,u))
     { 
       return i;
     } 
   } 
   return -1;
 }

 void CreateDG(OLGraph *G)
 { 
   /* 采用十字链表存储表示，构造有向图G。算法7.3 */
   int i, j, k;
   int IncInfo;
   ArcBox *p;
   VertexType v1, v2;
   printf("请输入有向图的顶点数,弧数,是否为带权图(是:1，否:0): ");
   scanf("%d,%d,%d", &(*G).vexnum, &(*G).arcnum, &IncInfo);
   printf("请输入%d个顶点的值(<%d个字符):\n", (*G).vexnum, MAX_VERTEX_NAME);
   
   for(i=0; i<(*G).vexnum; ++i)
   { 
     /* 构造表头向量 */
     scanf("%s", &(*G).xlist[i].data); /* 输入顶点值 */
     (*G).xlist[i].firstin = NULL; /* 初始化入弧的链表头指针 */
     (*G).xlist[i].firstout = NULL; /* 初始化出弧的链表头指针 */
   }
   printf("请输入%d条弧的弧尾和弧头(空格为间隔):\n", (*G).arcnum);
   for(k=0; k<(*G).arcnum; ++k)
   { 
     /* 输入各弧并构造十字链表 */
     scanf("%s%s", &v1, &v2);
     i = LocateVex(*G, v1); /* 确定v1和v2在G中的位置 */
     j = LocateVex(*G, v2);
     p = (ArcBox *)malloc(sizeof(ArcBox)); /* 产生弧结点(假定有足够空间) */
     p->data.tailvex = i; /* 对弧结点赋值 */
     p->data.headvex = j;
     p->data.hlink = (*G).xlist[j].firstin; /* 完成在入弧和出弧链表表头的插入 */
     p->tlink = (*G).xlist[i].firstout;
     (*G).xlist[j].firstin = (ArcBox1 *)p; /* 强制类型转换 */
     (*G).xlist[i].firstout = p;
     if(IncInfo)
     { 
	   /* 是网 */
       p->data.info = (InfoType *)malloc(sizeof(InfoType));
       printf("请输入该弧的权值: ");
       scanf("%d", p->data.info);
     }
     else
	 { /* 弧不含有相关信息 */
       p->data.info = NULL;
     }  
   }
 }

 void DestroyGraph(OLGraph *G)
 { /* 初始条件：有向图G存在。操作结果：销毁有向图G */
   int i;
   ElemType e;
   
   for(i=0; i<(*G).vexnum; i++) /* 对所有顶点 */
   { 
     while( (*G).xlist[i].firstout ) /* 出弧链表不空 */
     {
       ListDelete(&(*G).xlist[i].firstout, 1, &e); /* 删除其第1个结点，其值赋给e，在bo2-8.c中 */
       if(e.info) /* 带权 */
       { 
	     free(e.info); /* 释放动态生成的权值空间 */
       } 
	 }
   } 
   
   (*G).arcnum = 0;
   (*G).vexnum = 0;
 }

 VertexType* GetVex(OLGraph G, int v)
 { /* 初始条件：有向图G存在，v是G中某个顶点的序号。操作结果：返回v的值 */
   if(v>=G.vexnum || v<0)
   { 
     exit(ERROR);
   } 
   return &G.xlist[v].data;
 }

 Status PutVex(OLGraph *G, VertexType v, VertexType value)
 { 
   /* 初始条件：有向图G存在，v是G中某个顶点。操作结果：对v赋新值value */
   int i;
   
   i = LocateVex(*G, v);
   if(i < 0) /* v不是G的顶点 */
   { 
     return ERROR;
   } 
   strcpy((*G).xlist[i].data, value);
   
   return OK;
 }

 int FirstAdjVex(OLGraph G, VertexType v)
 { 
   /* 初始条件：有向图G存在，v是G中某个顶点 */
   /* 操作结果：返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点，则返回-1 */
   int i;
   ArcBox *p;
   
   i = LocateVex(G, v);
   p = G.xlist[i].firstout; /* p指向顶点v的第1个出弧 */
   if(p)
   { 
     return p->data.headvex;
   } 
   else
   { 
     return -1;
   } 
 }

 int NextAdjVex(OLGraph G, VertexType v, VertexType w)
 { 
   /* 初始条件：有向图G存在，v是G中某个顶点，w是v的邻接顶点 */
   /* 操作结果：返回v的(相对于w的)下一个邻接顶点的序号，若w是v的最后一个邻接顶点，则返回-1 */
   int i, j;
   ArcBox *p;
   i = LocateVex(G, v); /* i是顶点v的序号 */
   j = LocateVex(G, w); /* j是顶点w的序号 */
   p = G.xlist[i].firstout; /* p指向顶点v的第1个出弧 */
   while(p && p->data.headvex!=j)
   { 
     p = p->tlink;
   } 
   if( p ) /* w不是v的最后一个邻接顶点 */
   { 
     p = p->tlink; /* p指向相对于w的下一个邻接顶点 */
   } 
   if(p) /* 有下一个邻接顶点 */
   { 
     return p->data.headvex;
   } 
   else
   { 
     return -1;
   } 
 }

 void InsertVex(OLGraph *G, VertexType v)
 { 
   /* 初始条件：有向图G存在，v和有向图G中顶点有相同特征 */
   /* 操作结果：在有向图G中增添新顶点v(不增添与顶点相关的弧，留待InsertArc()去做) */
   strcpy((*G).xlist[(*G).vexnum].data, v); /* 拷贝顶点名称 */
   (*G).xlist[(*G).vexnum].firstin = NULL; /* 初始化入弧链表 */
   (*G).xlist[(*G).vexnum].firstout = NULL; /* 初始化出弧链表 */
   (*G).vexnum++; /* 顶点数+1 */
 }

 Status equal(ElemType c1, ElemType c2)
 {
   if(c1.headvex == c2.headvex)
   { 
     return TRUE;
   } 
   else
   { 
     return FALSE;
   } 
 }

 Status DeleteVex(OLGraph *G, VertexType v)
 { 
   /* 初始条件：有向图G存在，v是G中某个顶点。操作结果：删除G中顶点v及其相关的弧 */
   int i, j, k;
   ElemType e1, e2;
   ArcBox *p;
   ArcBox1 *p1, *p2;
   
   k = LocateVex(*G, v); /* k是顶点v的序号 */
   if(k < 0) /* v不是图G的顶点 */
   { 
     return ERROR; /* 以下删除顶点v的入弧 */
   } 
   e1.headvex = k; /* e1作为LocateElem()的比较元素 */
   for(j=0; j<(*G).vexnum; j++) /* 顶点v的入弧是其它顶点的出弧 */
   {
     i = LocateElem((*G).xlist[j].firstout, e1, equal);
     if( i ) /* 顶点v是顶点j的出弧 */
     {
       ListDelete(&(*G).xlist[j].firstout, i, &e2); /* 删除该弧结点，其值赋给e2 */
       (*G).arcnum--; /* 弧数-1 */
       if(e2.info) /* 带权 */
       { 
	     free(e2.info); /* 释放动态生成的权值空间 */
       } 
     }//if 
   }//for 
   /* 以下删除顶点v的出弧 */
   for(j=0; j<(*G).vexnum; j++) /* 顶点v的出弧是其它顶点的入弧 */
   {
     p1 = (*G).xlist[j].firstin;
     while(p1 && p1->tailvex!=k)
     {
       p2 = p1;
       p1 = p1->hlink;
     }
     if( p1 ) /* 找到顶点v的出弧 */
     {
       if(p1 == (*G).xlist[j].firstin) /* 是首结点 */
       {
	     (*G).xlist[j].firstin = p1->hlink; /* 入弧指针指向下一个结点 */
       }
	   else /* 不是首结点 */
	   {
	     p2->hlink = p1->hlink; /* 在链表中移去p1所指结点 */
	   }
	   if(p1->info) /* 带权 */
	   {
	     free(p1->info); /* 释放动态生成的权值空间 */
	   }
	   free(p1); /* 释放p1所指结点 */
       (*G).arcnum--; /* 弧数-1 */
     }//if
   }//for
   
   for(j=k+1; j<(*G).vexnum; j++) /* 序号>k的顶点依次向前移 */
   {
     (*G).xlist[j-1] = (*G).xlist[j];
   }
   (*G).vexnum--; /* 顶点数减1 */
   for(j=0; j<(*G).vexnum; j++) /* 结点序号>k的要减1 */
   {
     p = (*G).xlist[j].firstout; /* 处理出弧 */
     while(p)
     {
       if(p->data.tailvex > k)
       {
         p->data.tailvex--; /* 序号-1 */
       }
	   if(p->data.headvex > k)
	   {
	     p->data.headvex--; /* 序号-1 */
	   }
	   p = p->tlink;
     }//while
   }//for
   
   return OK;
 }

 Status InsertArc(OLGraph *G, VertexType v, VertexType w)
 { 
   /* 初始条件：有向图G存在，v和w是G中两个顶点。操作结果：在G中增添弧<v,w> */
   int i, j;
   int IncInfo;
   ArcBox *p;
   
   i = LocateVex(*G, v); /* 弧尾的序号 */
   j = LocateVex(*G, w); /* 弧头的序号 */
   if(i<0 || j<0)
   {
     return ERROR;
   }
   p = (ArcBox *)malloc(sizeof(ArcBox)); /* 生成新结点 */
   p->data.tailvex = i; /* 给新结点赋值 */
   p->data.headvex = j;
   p->data.hlink = (*G).xlist[j].firstin; /* 插在入弧和出弧的链头 */
   p->tlink = (*G).xlist[i].firstout;
   (*G).xlist[j].firstin = (ArcBox1*)p;
   (*G).xlist[i].firstout = p;
   (*G).arcnum++; /* 弧数加1 */
   printf("要插入的弧是否带权(是: 1,否: 0): ");
   scanf("%d", &IncInfo);
   if(IncInfo) /* 带权 */
   {
     p->data.info = (InfoType *)malloc(sizeof(InfoType)); /* 动态生成权值空间 */
     printf("请输入该弧的权值: ");
     scanf("%d", p->data.info);
   }
   else
   { 
     p->data.info = NULL;
   } 
   
   return OK;
 }

 Status DeleteArc(OLGraph *G, VertexType v, VertexType w)
 { 
   /* 初始条件：有向图G存在，v和w是G中两个顶点。操作结果：在G中删除弧<v,w> */
   int i, j, k;
   ElemType e;
   ArcBox1 *p1, *p2;
   i = LocateVex(*G, v); /* 弧尾的序号 */
   j = LocateVex(*G, w); /* 弧头的序号 */
   if(i<0 || j<0 || i==j)
   { 
     return ERROR;
   } 
   p1 = (*G).xlist[j].firstin; /* p1指向w的入弧链表 */
   while(p1 && p1->tailvex!=i) /* 使p1指向待删结点 */
   {
     p2 = p1;
     p1 = p1->hlink;
   }
   if(p1 == (*G).xlist[j].firstin) /* 首结点是待删结点 */
   { 
     (*G).xlist[j].firstin = p1->hlink; /* 入弧指针指向下一个结点 */
   } 
   else /* 首结点不是待删结点 */
   { 
     p2->hlink = p1->hlink; /* 在链表中移去p1所指结点(该结点仍在出弧链表中) */
   } 
   e.headvex = j; /* 待删弧结点的弧头顶点序号为j，e作为LocateElem()的比较元素 */
   k = LocateElem((*G).xlist[i].firstout, e, equal); /* 在出弧链表中的位序 */
   ListDelete(&(*G).xlist[i].firstout, k, &e); /* 在出弧链表中删除该结点，其值赋给e */
   if(e.info) /* 带权 */
   { 
     free(e.info); /* 释放动态生成的权值空间 */
   } 
   (*G).arcnum--; /* 弧数-1 */
   
   return OK;
 }

 void DFS(OLGraph G, int i) /* DFSTraverse()调用 */
 {
   ArcBox *p;
   
   visited[i] = TRUE; /* 访问标志数组置1(已被访问) */
   VisitFunc(G.xlist[i].data); /* 遍历第i个顶点 */
   p = G.xlist[i].firstout; /* p指向第i个顶点的出度 */
   while(p && visited[p->data.headvex]) /* p没到表尾且该弧的头顶点已被访问 */
   { 
     p = p->tlink; /* 查找下一个结点 */
   } 
   if(p && !visited[p->data.headvex]) /* 该弧的头顶点未被访问 */
   { 
     DFS(G, p->data.headvex); /* 递归调用DFS() */
   } 
 }

 void DFSTraverse(OLGraph G, void(*Visit)(VertexType))
 { 
   /* 初始条件：有向图G存在，v是G中某个顶点，Visit是顶点的应用函数(算法7.4) */
   /* 操作结果:从第1个顶点起,按深度优先递归遍历有向图G,并对每个顶点调用函数Visit一次且仅一次 */
   int v;
   
   VisitFunc=Visit;
   for(v=0; v<G.vexnum; v++)
   { 
     visited[v] = FALSE; /* 访问标志数组置初值(未被访问) */
   } 
   for(v=0; v<G.vexnum; v++) /* 由序号0开始，继续查找未被访问过的顶点 */
   {  
     if(! visited[v])
     { 
       DFS(G, v);
     } 
   } 
   printf("\n");
 }

 void InitQueue(LinkQueue *Q)
 {
 	//构造一个空队列Q
	(*Q).front = (*Q).rear = (QueuePtr)malloc(sizeof(QNode));
	
	if (! (*Q).front)
	{
		exit(OVERFLOW);
	}  
	
	(*Q).front->next = NULL;
 } 
 
 void DestroyQueue(LinkQueue *Q)
 {
 	//销毁队列Q(无论空否均可)
	while ( (*Q).front )
	{
		(*Q).rear = (*Q).front->next;
		free( (*Q).front );
		(*Q).front = (*Q).rear;
	}//while 
 } 
 
 void ClearQueue(LinkQueue *Q)
 {
 	//将Q清为空队列
	QueuePtr p, q;
	
	(*Q).rear = (*Q).front;
	p = (*Q).front->next;
	(*Q).front->next = NULL;
	
	while (p)
	{
		q = p;
		p = p->next;
		free(q);
	} //while
 }
 
 Status QueueEmpty(LinkQueue Q)
 {
 	//若Q为空队列，则返回TRUE，否则返回FALSE
	 if (Q.front->next == NULL)
	 {
	 	return TRUE;
 	 }  
 	 else
 	 {
 	 	return FALSE;
 	 }
 }
 
 int QueueLength(LinkQueue Q)
 {
 	//求队列长度
	 int i = 0;
	 QueuePtr p;
	 
	 p = Q.front;
	 
	 while (Q.rear != p)
	 {
	 	i++;
	 	p = p->next;
     } 
     
     return i;
 }
 
 Status GetHead_Q(LinkQueue Q, QElemType *e)//避免重名
 {
 	//若队列不空，则用e返回Q的队头元素，并返回OK，否则返回ERROR
	QueuePtr p;
	
	if (Q.front == Q.rear)
	{
		return ERROR;
	}
	
	p = Q.front->next;
	*e = p->data;
	
	return OK;
 } 
 
 void EnQueue(LinkQueue *Q, QElemType e)
 {
 	//插入元素e为Q的新的队尾元素
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if (! p)//存储分配失败
	{
		exit(OVERFLOW); 
	} 
	p->data = e;
	p->next = NULL;
	(*Q).rear->next = p;
	(*Q).rear = p;
 }
 
 Status DeQueue(LinkQueue *Q, QElemType *e)
 {
 	//队列不为空，删除Q的队头元素，用e返回其值，并返回OK，否则返回ERROR
	QueuePtr p;
	
	if ((*Q).front == (*Q).rear)
	{
		return ERROR;
	} 
	p = (*Q).front->next;
	*e = p->data;
	(*Q).front->next = p->next;
	if ((*Q).rear == p)
	{
		(*Q).rear == (*Q).front;
	}
	free(p);
	
	return OK;	
 }
 
 void QueueTraverse(LinkQueue Q, void(*vi)(QElemType))
 {
 	//从队头到队尾依次对队列Q中每个元素调用函数vi（）
	QueuePtr p;
	
	p = Q.front->next;
	
	while ( p )
	{
		vi(p->data);
		p = p->next;
	} 
	printf("\n");
 }
 
 void BFSTraverse(OLGraph G, void(*Visit)(VertexType))
 { 
   /* 初始条件：有向图G存在，Visit是顶点的应用函数。算法7.6 */
   /* 操作结果：从第1个顶点起，按广度优先非递归遍历有向图G，并对每个顶点调用函数Visit */
   /*           一次且仅一次。使用辅助队列Q和访问标志数组visited */
   int v, u, w;
   LinkQueue Q;
   
   for(v=0; v<G.vexnum; v++)
   { 
     visited[v]=FALSE;
   } 
   InitQueue(&Q);
   for(v=0; v<G.vexnum; v++)
   { 
     if(! visited[v])
     {
       visited[v] = TRUE;
       Visit(G.xlist[v].data);
       EnQueue(&Q, v);
       while(! QueueEmpty(Q))
       {
	     DeQueue(&Q,&u);
	     for(w=FirstAdjVex(G,G.xlist[u].data); w>=0; w=NextAdjVex(G,G.xlist[u].data,G.xlist[w].data))
	     { 
	       if(!visited[w]) /* w为u的尚未访问的邻接顶点的序号 */
	       {
	         visited[w]=TRUE;
	         Visit(G.xlist[w].data);
	         EnQueue(&Q,w);
	       }
	     } //for
       }//while
     }//if
   } //for 
   
   printf("\n");
 }

 void Display(OLGraph G)
 { 
   /* 输出有向图G */
   int i;
   ArcBox *p;
   
   printf("共%d个顶点: ", G.vexnum);
   for(i=0; i<G.vexnum; i++) /* 输出顶点 */
   {
     printf("%s ", G.xlist[i].data);
   }
   printf("\n%d条弧:\n", G.arcnum);
   for(i=0; i<G.vexnum; i++) /* 顺出弧链表输出 */
   {
     p = G.xlist[i].firstout;
     while( p )
     {
       printf("%s→%s ",G.xlist[i].data,G.xlist[p->data.headvex].data);
       if(p->data.info) /* 该弧有相关信息(权值) */
       {
	     printf("权值: %d ",*p->data.info);
       }
	   p = p->tlink;
     }
     printf("\n");
   }
 }
 
 void visit(VertexType v)
 {
   printf("%s ",v);
 }
 