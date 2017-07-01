typedef int SElemType; /* 栈元素类型为整型，改algo3-6.c */
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

char Precede(SElemType t1,SElemType t2)
{ /* 根据教科书表3.1，判断t1，t2两符号的优先关系('#'用'\n'代替) */
char f;
switch(t2)
{
 case '+':
 case '-':if(t1=='('||t1=='\n')
            f='<'; /* t1<t2 */
          else
            f='>'; /* t1>t2 */
          break;
 case '*':
 case '/':if(t1=='*'||t1=='/'||t1==')')
            f='>'; /* t1>t2 */
          else
            f='<'; /* t1<t2 */
          break;
 case '(':if(t1==')')
          {
            printf("括号不匹配\n");
            exit(ERROR);
          }
          else
            f='<'; /* t1<t2 */
          break;
 case ')':switch(t1)
          {
            case '(':f='='; /* t1=t2 */
                      break;
            case'\n':printf("缺乏左括号\n");
                      exit(ERROR);
            default :f='>'; /* t1>t2 */
          }
          break;
 case'\n':switch(t1)
          {
            case'\n':f='='; /* t1=t2 */
                     break;
            case'(' :printf("缺乏右括号\n");
                     exit(ERROR);
            default :f='>'; /* t1>t2 */
          }
}
return f;
}

Status In(SElemType c)
{ /* 判断c是否为7种运算符之一 */
switch(c)
{
 case'+' :
 case'-' :
 case'*' :
 case'/' :
 case'(' :
 case')' :
 case'\n':return TRUE;
 default :return FALSE;
}
}

SElemType Operate(SElemType a,SElemType theta,SElemType b)
{ /* 做四则运算a theta b，返回运算结果 */
switch(theta)
{
 case'+':return a+b;
 case'-':return a-b;
 case'*':return a*b;
}
return a/b;
}

 SElemType EvaluateExpression()
 { /* 算术表达式求值的算符优先算法。设OPTR和OPND分别为运算符栈和运算数栈 */
   SqStack OPTR,OPND;
   SElemType a,b,d,x; /* 改algo3-6.c */
   char c; /* 存放由键盘接收的字符，改algo3-6.c */
   char z[11]; /* 存放整数字符串，改algo3-6.c */
   int i; /* 改algo3-6.c */
   InitStack(&OPTR); /* 初始化运算符栈OPTR和运算数栈OPND */
   InitStack(&OPND);
   Push(&OPTR,'\n'); /* 将换行符压入运算符栈OPTR的栈底(改) */
   c=getchar(); /* 由键盘读入1个字符到c */
   GetTop(OPTR,&x); /* 将运算符栈OPTR的栈顶元素赋给x */
   while(c!='\n'||x!='\n') /* c和x不都是换行符 */
   {
     if(In(c)) /* c是7种运算符之一 */
       switch(Precede(x,c)) /* 判断x和c的优先权 */
       {
	 case'<' :Push(&OPTR,c); /* 栈顶元素x的优先权低，入栈c */
                  c=getchar();  /* 由键盘读入下一个字符到c */
                  break;
	 case'=' :Pop(&OPTR,&x); /* x='('且c=')'情况，弹出'('给x(后又扔掉) */
                  c=getchar(); /* 由键盘读入下一个字符到c(扔掉')') */
                  break;
	 case'>' :Pop(&OPTR,&x); /* 栈顶元素x的优先权高，弹出运算符栈OPTR的栈顶元素给x(改) */
		  Pop(&OPND,&b); /* 依次弹出运算数栈OPND的栈顶元素给b，a */
		  Pop(&OPND,&a);
		  Push(&OPND,Operate(a,x,b)); /* 做运算a x b，并将运算结果入运算数栈 */
       }
     else if(c>='0'&&c<='9') /* c是操作数，此语句改algo3-6.c */
     {
       i=0;
       while(c>='0'&&c<='9') /* 是连续数字 */
       {
         z[i++]=c;
         c=getchar();
       }
       z[i]=0; /* 字符串结束符 */
       d=atoi(z); /* 将z中保存的数值型字符串转为整型存于d */
       Push(&OPND,d); /* 将d压入运算数栈OPND */
     }
     else /* c是非法字符，以下同algo3-6.c */
     {
       printf("出现非法字符\n");
       exit(ERROR);
     }
     GetTop(OPTR,&x); /* 将运算符栈OPTR的栈顶元素赋给x */
   }
   Pop(&OPND,&x); /* 弹出运算数栈OPND的栈顶元素(运算结果)给x(改此处) */
   if(!StackEmpty(OPND)) /* 运算数栈OPND不空(运算符栈OPTR仅剩'\n') */
   {
     printf("表达式不正确\n");
     exit(ERROR);
   }
   return x;
 }

 int main()
 {
   printf("请输入算术表达式,负数要用(0-正数)表示\n");
   printf("%d\n",EvaluateExpression());
   
   return 0; 
 }


