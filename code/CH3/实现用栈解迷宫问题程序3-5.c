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
 
  int curstep=1; /* 当前足迹，初值(在入口处)为1 */
 typedef struct
 {
   int ord; /* 通道块在路径上的＂序号＂ */
   PosType seat; /* 通道块在迷宫中的＂坐标位置＂ */
   int di; /* 从此通道块走向下一通道块的＂方向＂(0～3表示东～北) */
 }SElemType; /* 栈的元素类型 */
 
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
 
 

 /* 定义墙元素值为0，可通过路径为1，不能通过路径为-1，通过路径为足迹 */
 Status Pass(PosType b)
 { /* 当迷宫m的b点的序号为1(可通过路径)，返回OK；否则，返回ERROR */
   if(m[b.x][b.y]==1)
     return OK;
   else
     return ERROR;
 }

 void FootPrint(PosType a)
 { /* 使迷宫m的a点的值变为足迹(curstep) */
   m[a.x][a.y]=curstep;
 }

 void NextPos(PosType *c,int di)
 { /* 根据当前位置及移动方向，求得下一位置 */
   PosType direc[4]={{0,1},{1,0},{0,-1},{-1,0}}; /* {行增量,列增量},移动方向,依次为东南西北 */
   (*c).x+=direc[di].x;
   (*c).y+=direc[di].y;
 }

 void MarkPrint(PosType b)
 { /* 使迷宫m的b点的序号变为-1(不能通过的路径) */
   m[b.x][b.y]=-1;
 }

 Status MazePath(PosType start,PosType end) /* 算法3.3 */
 { /* 若迷宫m中存在从入口start到出口end的通道，则求得一条 */
   /* 存放在栈中(从栈底到栈顶)，并返回TRUE；否则返回FALSE */
   SqStack S; /* 顺序栈 */
   PosType curpos; /* 当前位置 */
   SElemType e; /* 栈元素 */
   InitStack(&S); /* 初始化栈 */
   curpos=start; /* 当前位置在入口 */
   do
   {
     if(Pass(curpos))
     { /* 当前位置可以通过，即是未曾走到过的通道块 */
       FootPrint(curpos); /* 留下足迹 */
       e.ord=curstep;
       e.seat=curpos;
       e.di=0;
       Push(&S,e); /* 入栈当前位置及状态 */
       curstep++; /* 足迹加1 */
       if(curpos.x==end.x&&curpos.y==end.y) /* 到达终点(出口) */
         return TRUE;
       NextPos(&curpos,e.di); /* 由当前位置及移动方向，确定下一个当前位置 */
     }
     else
     { /* 当前位置不能通过 */
       if(!StackEmpty(S)) /* 栈不空 */
       {
         Pop(&S,&e); /* 退栈到前一位置 */
         curstep--; /* 足迹减1 */
         while(e.di==3&&!StackEmpty(S)) /* 前一位置处于最后一个方向(北) */
         {
           MarkPrint(e.seat); /* 在前一位置留下不能通过的标记(-1) */
           Pop(&S,&e); /* 再退回一步 */
           curstep--; /* 足迹再减1 */
         }
         if(e.di<3) /* 没到最后一个方向(北) */
         {
           e.di++; /* 换下一个方向探索 */
           Push(&S,e); /* 入栈该位置的下一个方向 */
           curstep++; /* 足迹加1 */
           curpos=e.seat; /* 确定当前位置 */
           NextPos(&curpos,e.di); /* 确定下一个当前位置是该新方向上的相邻块 */
         }
       }
     }
   }while(!StackEmpty(S));
   return FALSE;
 }

 int main()
 {
   Init(1); /* 初始化迷宫，通道值为1 */
   if(MazePath(begin,end)) /* 有通路 */
   {
     printf("此迷宫从入口到出口的一条路径如下:\n");
     Print(); /* 输出此通路 */
   }
   else
     printf("此迷宫没有从入口到出口的路径\n");
 
   return 0;
 }
 