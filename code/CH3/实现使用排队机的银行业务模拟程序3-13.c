#define Qu 4 /* 窗口数 */
#define Khjg 5 /* 两相邻到达的客户的时间间隔最大值 */
#define Blsj 30 /* 每个客户办理业务的时间最大值 */
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

 typedef struct /* 定义ElemType为结构体类型 */
 {
   int OccurTime; /* 事件发生时刻 */
   int NType; /* 事件类型，Qu表示到达事件，0至Qu-1表示Qu个窗口的离开事件 */
 }Event,ElemType; /* 事件类型，有序链表LinkList的数据元素类型 */

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
 /* 从实际应用角度出发重新定义的线性链表结构 */
 typedef LinkList EventList; /* 事件链表指针类型，定义为有序链表 */
 typedef struct
 {
   int ArrivalTime; /* 到达时刻 */
   int Duration; /* 办理事务所需时间 */
 }QElemType; /* 定义队列的数据元素类型QElemType为结构体类型 */
 
   /* 程序中用到的主要变量(全局) */
 EventList ev; /* 事件表头指针 */
 Event en,et; /* 事件,临时变量 */
 /*FILE *fp; /* 文件型指针，用于指向b.txt或d.txt文件 */
 typedef struct QNode
 {
   QElemType data;
   struct QNode *next;
 }QNode,*QueuePtr;

 typedef struct
 {
   QueuePtr front,rear; /* 队头、队尾指针 */
 }LinkQueue;
 

 long int TotalTime=0; /* 累计客户逗留时间(初值为0) */
 int CloseTime,CustomerNum=0; /* 银行营业时间(单位是分),客户数(初值为0) */

 
  LinkQueue q; /* 排队机队列q */
 QElemType customer[Qu]; /* Qu个客户队列元素, 存放正在窗口办理业务的客户的信息 */
 /*FILE *fq; /* 文件型指针，用于指向c.txt文件 */
 /*int j=0; /* 计数器，产生c.txt文件用到 */
 Boolean chk[Qu]; /* 窗口状态，1为闲，0为忙 */
 
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

 int cmp(Event a,Event b)
 { /* 依事件a的发生时刻<、=或>事件b的发生时刻分别返回-1、0或1 */
   if(a.OccurTime==b.OccurTime)
     return 0;
   else
     return (a.OccurTime-b.OccurTime)/abs(a.OccurTime-b.OccurTime);
 }

 void Random(int *d,int *i)
 { /* 生成两个随机数 */
   *d=rand()%Blsj+1; /* 1到Blsj之间的随机数(办理业务的时间) */
   *i=rand()%(Khjg+1); /* 0到Khjg之间的随机数(客户到达的时间间隔) */
 }
 
 void OpenForDay();
 void CustomerArrived();
 void CustomerDeparture();

 void Bank_Simulation()
 { /* 银行业务模拟函数 */
   Link p;
   OpenForDay(); /* 初始化事件表ev且插入第1个到达事件，初始化队列 */
   while(!ListEmpty(ev)) /* 事件表ev不空 */
   {
     DelFirst(&ev,ev.head,&p); /* 删除事件表ev的第1个结点，并由p返回其指针，在bo2-6.c中 */
 /*  if(p->data.OccurTime<50) /* 输出前50分钟内发生的事件到文件d.txt中 */
 /*    fprintf(fp,"p->data.OccurTime=%3d p->data.NType=%d\n",p->data.OccurTime,p->data.NType); */
     en.OccurTime=GetCurElem(p).OccurTime; /* GetCurElem()在bo2-6.c中，返回p->data(ElemType类型) */
     en.NType=GetCurElem(p).NType;
     if(en.NType==Qu) /* 到达事件 */
       CustomerArrived(); /* 处理客户到达事件 */
     else /* 由某窗口离开的事件 */
       CustomerDeparture(); /* 处理客户离开事件 */
   } /* 计算并输出平均逗留时间 */
   printf("窗口数=%d 两相邻到达的客户的时间间隔=0～%d分钟 每个客户办理业务的时间=1～%d分钟\n",Qu,Khjg,Blsj);
   printf("客户总数:%d, 所有客户共耗时:%ld分钟,平均每人耗时:%d分钟,",CustomerNum,TotalTime,TotalTime/CustomerNum);
   printf("最后一个客户离开的时间:%d分\n",en.OccurTime);
 }

 void OpenForDay()
 { /* 初始化事件链表ev且插入第1个到达事件，初始化排队机q，初始化Qu个窗口为1(空闲) */
   int i;
   InitList(&ev); /* 初始化事件链表ev为空 */
   en.OccurTime=0; /* 设定第1位客户到达时间为0(银行一开门，就有客户来) */
   en.NType=Qu; /* 到达 */
   OrderInsert(&ev,en,cmp); /* 将第1个到达事件en有序插入事件表ev中，在bo2-6.c中 */
   InitQueue(&q); /* 初始化排队机队列q */
   for(i=0;i<Qu;i++)
     chk[i]=1; /* 初始化Qu个窗口为1(空闲) */
 }

 int ChuangKou()
 { /* 返回空闲窗口的序号(0～Qu-1)，若有多个窗口空闲，返回序号最小的。若无空闲窗口，返回Qu */
   int i;
   for(i=0;i<Qu;i++)
     if(chk[i])
       return i;
   return i;
 }

 void CustomerArrived()
 { /* 处理客户到达事件en(en.NType=Qu)，与algo3-12.c不同 */
   QElemType f;
   int durtime,intertime,i;
   ++CustomerNum; /* 客户数加1 */
   Random(&durtime,&intertime); /* 生成当前客户办理业务的时间和下一个客户到达的时间间隔2个随机数 */
   et.OccurTime=en.OccurTime+intertime; /* 下一客户et到达时刻=当前客户en的到达时间+时间间隔 */
   et.NType=Qu; /* 下一客户到达事件 */
   if(et.OccurTime<CloseTime) /* 下一客户到达时银行尚未关门 */
     OrderInsert(&ev,et,cmp); /* 按升序将下一客户到达事件et插入事件表ev中，在bo2-6.c中 */
   f.ArrivalTime=en.OccurTime; /* 将当前客户到达事件en赋给队列元素f */
   f.Duration=durtime;
   EnQueue(&q,f); /* 将当前客户f入队到排队机 */
   i=ChuangKou(); /* 求空闲窗口的序号 */
   if(i<Qu) /* 有空闲窗口 */
   {
     DeQueue(&q,&customer[i]); /* 删去排队机的排头客户(也就是刚入队的f由排队机到i号窗口) */
 /*  if(j++<20) /* 输出前20个客户到达信息及处理业务窗口信息到文件c.txt中 */
 /*    fprintf(fq,"客户到达时刻=%3d,办理业务的时间=%2d,所在窗口=%d\n",customer[i].ArrivalTime,customer[i].Duration,i); */
     et.OccurTime=en.OccurTime+customer[i].Duration; /* 设定一个i号窗口的离开事件et */
     et.NType=i; /* 第i号窗口的离开事件 */
     OrderInsert(&ev,et,cmp); /* 将此离开事件et按升序插入事件表ev中 */
     chk[i]=0; /* i号窗口状态变忙 */
   }
 }

 void CustomerDeparture()
 { /* 处理客户离开事件en(en.NType<Qu)，与algo3-12.c不同 */
   int i;
   i=en.NType; /* 确定离开事件en发生的窗口序号i */
   chk[i]=1; /* i号窗口状态变闲 */
   TotalTime+=en.OccurTime-customer[i].ArrivalTime; /* 客户逗留时间=离开事件en的发生时刻-该客户的到达时间 */
   if(!QueueEmpty(q))
   { /* 第i窗口的客户离开后，排队机仍不空 */
     DeQueue(&q,&customer[i]); /* 删去排队机的排头客户并将其赋给customer[i](排队机的排头客户去i窗口办理业务) */
 /*  if(j++<20) /* 输出前20个客户到达信息及处理业务窗口信息到文件c.txt中 */
 /*    fprintf(fq,"客户到达时刻=%3d,办理业务的时间=%2d,所在窗口=%d\n",customer[i].ArrivalTime,customer[i].Duration,i); */
     chk[i]=0; /* i号窗口状态变忙 */
     et.OccurTime=en.OccurTime+customer[i].Duration; /* 设定customer[i]的离开事件et,客户的离开时间=原客户的离开时间+当前客户办理业务的时间 */
     et.NType=i; /* 第i号窗口的离开事件 */
     OrderInsert(&ev,et,cmp); /* 将此离开事件et按升序插入事件表ev中 */
   }
 }

 int main()
 {
 /*fq=fopen("c.txt","w"); /* 打开c.txt文件，用于写入客户到达信息 */
 /*fp=fopen("d.txt","w"); /* 打开d.txt文件，用于写入有序事件表的历史记录 */
   printf("请输入银行营业时间长度(单位:分): ");
   scanf("%d",&CloseTime);
 /*srand(time(0)); /* 设置随机数种子,以使每次运行程序产生的随机数不同(time(0)是长整型数,与调用时间有关) */
   Bank_Simulation();
 /*fclose(fq); /* 关闭c.txt */
 /*fclose(fp); /* 关闭d.txt */
 
 return 0; 
 }
