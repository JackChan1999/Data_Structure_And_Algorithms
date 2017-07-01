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
 
  void Try(PosType cur,int curstep)
 { /* 由当前位置cur、当前步骤curstep试探下一点 */
   int i;
   PosType next; /* 下一个位置 */
   PosType direc[4]={{0,1},{1,0},{0,-1},{-1,0}}; /* {行增量,列增量},移动方向,依次为东南西北 */
   for(i=0;i<=3;i++) /* 依次试探东南西北四个方向 */
   {
     next.x=cur.x+direc[i].x; /* 根据移动方向，给下一位置赋值 */
     next.y=cur.y+direc[i].y;
     if(m[next.x][next.y]==-1) /* 下一个位置是通路 */
     {
       m[next.x][next.y]=++curstep; /* 将下一个位置设为足迹 */
       if(next.x!=end.x||next.y!=end.y) /* 没到终点 */
         Try(next,curstep); /* 由下一个位置继续试探(降阶递归调用，离终点更近) */
       else /* 到终点 */
       {
         Print(); /* 输出结果(出口，不再递归调用) */
         printf("\n");
       }
       m[next.x][next.y]=-1; /* 恢复为通路，以便在另一个方向试探另一条路 */
       curstep--; /* 足迹也减1 */
     }
   }
 }

 int main()
 {
   Init(-1); /* 初始化迷宫，通道值为-1 */
   printf("此迷宫从入口到出口的路径如下:\n");
   m[begin.x][begin.y]=1; /* 入口的足迹为1 */
   Try(begin,1); /* 由第1步入口试探起 */
   
   return 0; 
 }
 
 