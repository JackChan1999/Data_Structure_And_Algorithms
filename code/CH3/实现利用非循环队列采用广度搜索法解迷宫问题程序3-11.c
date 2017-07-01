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
 
 typedef struct
 {
   int x; /* 行值 */
   int y; /* 列值 */
 }PosType; /* 迷宫坐标位置类型 */

 #define MAXLENGTH 25 /* 设迷宫的最大行列为25 */
 typedef int MazeType[MAXLENGTH][MAXLENGTH]; /* 迷宫数组类型[行][列] */

 /* 全局变量 */
 MazeType m; /* 迷宫数组 */
 int x,y; /* 迷宫的行数，列数 */
 PosType begin,end; /* 迷宫的入口坐标,出口坐标 */
 
 #define D 8 /* 移动方向数，只能取4和8。(8个，可斜行；4个，只可直走) */

 typedef struct /* 定义队列元素和栈元素为同类型的结构体 */
 {
   PosType seat; /* 当前点的行值，列值 */
   int pre; /* 前一点在队列中的序号 */
 }QElemType,SElemType; /* 栈元素和队列元素 */
 
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
 
 #define QUEUE_INIT_SIZE 10 /* 队列存储空间的初始分配量 */
 #define QUEUE_INCREMENT 2 /* 队列存储空间的分配增量 */
 typedef struct
 {
   QElemType *base; /* 初始化的动态分配存储空间 */
   int front; /* 头指针，若队列不空,指向队列头元素 */
   int rear; /* 尾指针，若队列不空,指向队列尾元素的下一个位置 */
   int queuesize; /* 当前分配的存储容量(以sizeof(QElemType)为单位) */
 }SqQueue2;
 
 void Print()
 { /* 输出迷宫的解(m数组) */
   int i,j;
   for(i=0;i<x;i++)
   {
     for(j=0;j<y;j++)
       printf("%3d",m[i][j]);
     printf("\n");
   }
 }

 void Init(int k)
 { /* 设定迷宫布局(墙为值0,通道值为k) */
   int i,j,x1,y1;
   printf("请输入迷宫的行数,列数(包括外墙)：");
   scanf("%d,%d",&x,&y);
   for(i=0;i<x;i++) /* 定义周边值为0(外墙) */
   {
     m[0][i]=0; /* 行周边 */
     m[x-1][i]=0;
   }
   for(i=0;i<y-1;i++)
   {
     m[i][0]=0; /* 列周边 */
     m[i][y-1]=0;
   }
   for(i=1;i<x-1;i++)
     for(j=1;j<y-1;j++)
       m[i][j]=k; /* 定义除外墙，其余都是通道，初值为k */
   printf("请输入迷宫内墙单元数：");
   scanf("%d",&j);
   printf("请依次输入迷宫内墙每个单元的行数,列数：\n");
   for(i=1;i<=j;i++)
   {
     scanf("%d,%d",&x1,&y1);
     m[x1][y1]=0; /* 修改墙的值为0 */
   }
   printf("迷宫结构如下:\n");
   Print();
   printf("请输入入口的行数,列数：");
   scanf("%d,%d",&begin.x,&begin.y);
   printf("请输入出口的行数,列数：");
   scanf("%d,%d",&end.x,&end.y);
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
 
  void InitQueue(SqQueue2 *Q)
 { /* 构造一个空队列Q */
   (*Q).base=(QElemType *)malloc(QUEUE_INIT_SIZE*sizeof(QElemType));
   if(!(*Q).base) /* 存储分配失败 */
     exit(ERROR);
   (*Q).front=(*Q).rear=0;
   (*Q).queuesize=QUEUE_INIT_SIZE;
 }

 void DestroyQueue(SqQueue2 *Q)
 { /* 销毁队列Q，Q不再存在 */
   if((*Q).base)
     free((*Q).base);
   (*Q).base=NULL;
   (*Q).front=(*Q).rear=(*Q).queuesize=0;
 }

 void ClearQueue(SqQueue2 *Q)
 { /* 将Q清为空队列 */
   (*Q).front=(*Q).rear=0;
 }

 Status QueueEmpty(SqQueue2 Q)
 { /* 若队列Q为空队列，则返回TRUE；否则返回FALSE */
   if(Q.front==Q.rear) /* 队列空的标志 */
     return TRUE;
   else
     return FALSE;
 }

 Status GetHead(SqQueue2 Q,QElemType *e)
 { /* 若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR */
   if(Q.front==Q.rear) /* 队列空 */
     return ERROR;
   *e=Q.base[Q.front];
   return OK;
 }
 
  int QueueLength(SqQueue2 Q)
 { /* 返回Q的元素个数，即队列的长度 */
   return(Q.rear-Q.front);
 }

 void EnQueue(SqQueue2 *Q,QElemType e)
 { /* 插入元素e为Q的新的队尾元素 */
   if((*Q).rear==(*Q).queuesize)
   { /* 队列满，增加存储单元 */
     (*Q).base=(QElemType *)realloc((*Q).base,((*Q).queuesize+QUEUE_INCREMENT)*sizeof(QElemType));
     if(!(*Q).base) /* 增加单元失败 */
       exit(ERROR);
   }
   (*Q).base[(*Q).rear++]=e;
 }

 Status DeQueue(SqQueue2 *Q,QElemType *e)
 { /* 若队列不空，则删除Q的队头元素，用e返回其值，并返回OK；否则返回ERROR */
   if((*Q).front==(*Q).rear) /* 队列空 */
     return ERROR;
   *e=(*Q).base[(*Q).front++];
   return OK;
 }

 void QueueTraverse(SqQueue2 Q,void(*vi)(QElemType))
 { /* 从队头到队尾依次对队列Q中每个元素调用函数vi() */
   int i=Q.front;
   while(i!=Q.rear)
     vi(Q.base[i++]);
   printf("\n");
 }
 
  struct /* 移动数组，移动方向由正东起顺时针转 */
 {
   int x,y;
 }move[D]={
 #if D==8
	   {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
 #endif
 #if D==4
	   {0,1},{1,0},{0,-1},{-1,0}};
 #endif

 void Path()
 { /* 广度搜索法求一条迷宫路径 */
   SqQueue2 q; /* 采用非循环顺序队列 */
   QElemType qf,qt; /* 当前点和下一点 */
   SqStack s; /* 采用顺序栈 */
   int i,flag=1; /* 当找到出口，flag=0 */
   qf.seat.x=begin.x; /* 将入口作为当前点 */
   qf.seat.y=begin.y;
   qf.pre=-1; /* 设入口(第一点)的上一点的序号=-1 */
   m[qf.seat.x][qf.seat.y]=-1; /* 初始点设为-1(标记已访问过) */
   InitQueue(&q);
   EnQueue(&q,qf); /* 起点入队 */
   while(!QueueEmpty(q)&&flag)
   { /* 队列中还有没被广度搜索过的点且还没找到出口 */
     DeQueue(&q,&qf); /* 出队qf为当前点 */
     for(i=0;i<D;i++) /* 向各个方向尝试 */
     {
       qt.seat.x=qf.seat.x+move[i].x; /* 下一点的坐标 */
       qt.seat.y=qf.seat.y+move[i].y;
       if(m[qt.seat.x][qt.seat.y]==1)
       { /* 此点是通道且不曾被访问过 */
         m[qt.seat.x][qt.seat.y]=-1; /* 标记已访问过 */
         qt.pre=q.front-1; /* qt的前一点处于队列中现队头减1的位置(没删除) */
	 EnQueue(&q,qt); /* 入队qt */
         if(qt.seat.x==end.x&&qt.seat.y==end.y) /* 到达终点 */
         {
           flag=0;
           break;
         }
       }
     }
   }
   if(flag) /* 搜索完整个队列还没到达终点 */
     printf("没有路径可到达终点！\n");
   else /* 到达终点 */
   {
     InitStack(&s); /* 初始化s栈 */
     i=q.rear-1; /* i为待入栈元素在队列中的位置 */
     while(i>=0) /* 没到入口 */
     {
       Push(&s,q.base[i]); /* 将队列中的路径入栈(栈底为出口，栈顶为入口) */
       i=q.base[i].pre; /* i为前一元素在队列中的位置 */
     }
     i=0; /* i为走出迷宫的足迹 */
     while(!StackEmpty(s))
     {
       Pop(&s,&qf); /* 依照由入口到出口的顺序弹出路径 */
       i++;
       m[qf.seat.x][qf.seat.y]=i; /* 标记路径为足迹(标记前的值为-1) */
     }
     printf("走出迷宫的一个方案：\n");
     Print(); /* 输出m数组 */
   }
 }

 int main()
 {
   Init(1); /* 初始化迷宫，通道值为1 */
   Path(); /* 求一条迷宫路径 */
   
   return 0; 
 }
 
 
 

 
 
 
