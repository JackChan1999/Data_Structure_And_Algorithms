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
 #define N 8
 
 void bubble_sort(int a[],int n)
 { /* 将a中整数序列重新排列成自小至大有序的整数序列(起泡排序) */
   int i,j,t;
   Status change;
   for(i=n-1,change=TRUE;i>1&&change;--i)
   {
     change=FALSE;
     for(j=0;j<i;++j)
       if(a[j]>a[j+1])
       {
         t=a[j];
         a[j]=a[j+1];
         a[j+1]=t;
         change=TRUE;
       }
   }
 }

 void print(int r[],int n)
 {
   int i;
   for(i=0;i<n;i++)
     printf("%d ",r[i]);
   printf("\n");
 }

 int main()
 {
   int d[N]={49,38,65,97,76,13,27,49};
   printf("排序前:\n");
   print(d,N);
   bubble_sort(d,N);
   printf("排序后:\n");
   print(d,N);
   
   return 0; 
 }
