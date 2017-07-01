 typedef char SElemType;
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
 
  FILE *fp;

 void copy(SElemType c)
 { /* 将字符c送至fp所指的文件中 */
   fputc(c,fp);
 }

 void LineEdit()
 { /* 利用字符栈s，从终端接收一行并送至调用过程的数据区。算法3.2 */
   SqStack s;
   char ch;
   InitStack(&s);
   printf("请输入一个文本文件,^Z结束输入:\n");
   ch=getchar();
   while(ch!=EOF)
   { /* 当全文没结束(EOF为^Z键，全文结束符) */
     while(ch!=EOF&&ch!='\n')
     { /* 当全文没结束且没到行末(不是换行符) */
       switch(ch)
       {
         case '#':if(!StackEmpty(s))
                    Pop(&s,&ch); /* 仅当栈非空时退栈,c可由ch替代 */
                  break;
         case '@':ClearStack(&s); /* 重置s为空栈 */
                  break;
         default :Push(&s,ch); /* 其他字符进栈 */
       }
       ch=getchar(); /* 从终端接收下一个字符 */
     }
     StackTraverse(s,copy); /* 将从栈底到栈顶的栈内字符传送至文件 */
     fputc('\n',fp); /* 向文件输入一个换行符 */
     ClearStack(&s); /* 重置s为空栈 */
     if(ch!=EOF)
       ch=getchar();
   }
   DestroyStack(&s);
 }

 void main()
 {
   fp=fopen("ed.txt","w"); /* 在当前目录下建立ed.txt文件，用于写数据，*/
   if(fp)                  /* 如已有同名文件则先删除原文件 */
   {
     LineEdit();
     fclose(fp); /* 关闭fp所指的文件 */
   }
   else
     printf("建立文件失败!\n");
 }
 
 