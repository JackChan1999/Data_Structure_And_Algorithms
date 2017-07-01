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
 
 #define N 6 /* 字符串长度 */
 typedef char ElemType[N];
 
 #define MAX_SIZE 100 /* 链表的最大长度 */
 typedef struct
 {
   ElemType data;
   int cur;
 }component,SLinkList[MAX_SIZE];
 
 int main()
 {
   SLinkList s={{"",1},{"ZHAO",2},{"QIAN",3},{"SUN",4},{"LI",5},{"ZHOU",6},{"WU",7},{"ZHENG",8},{"WANG",0}}; /* 教科书中图2.10(a)的状态 */
   int i;
   i=s[0].cur; /* i指示第1个结点的位置 */
   while(i)
   { /* 输出教科书中图2.10(a)的状态 */
     printf("%s ",s[i].data); /* 输出链表的当前值 */
     i=s[i].cur; /* 找到下一个 */
   }
   printf("\n");
   s[4].cur=9; /* 按教科书中图2.10(b)修改(在"LI"之后插入"SHI") */
   s[9].cur=5;
   strcpy(s[9].data,"SHI");
   s[6].cur=8; /* 删除"ZHENG" */
   i=s[0].cur; /* i指示第1个结点的位置 */
   while(i)
   { /* 输出教科书中图2.10(b)的状态 */
     printf("%s ",s[i].data); /* 输出链表的当前值 */
     i=s[i].cur; /* 找到下一个 */
   }
   printf("\n");
   
   return 0; 
 }
 
 