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
 #include<stdarg.h> /* 实现变长参数表要包括的头文件 */
 
 typedef int ElemType;
 ElemType Max(int num,...) /* ...表示变长参数表，位于形参表的最后,前面必须有至少一个固定参数 */
 { /* 函数功能：返回num个数中的最大值 */
   va_list ap; /* 定义ap是变长参数表类型(C语言的数据类型) */
   int i;
   ElemType m,n;
   if(num<1)
     exit(ERROR);
   va_start(ap,num); /* ap指向固定参数num后面的实参表 */
   m=va_arg(ap,ElemType); /* 依次读取ap所指的实参(以逗号为分隔符)作为ElemType类型实参,ap向后移 */
   for(i=1;i<num;++i)
   {
     n=va_arg(ap,ElemType); /* 同上 */
     if(m<n)
       m=n;
   }
   va_end(ap); /* 与va_start()配对，结束对变长参数表的读取，ap不再指向变长参数表 */
   return m;
 }

 int main()
 {
   printf("1.最大值为%d\n",Max(4,7,9,5,8)); /* 在4个数中求最大值，ap最初指向“7,9,5,8” */
   printf("2.最大值为%d\n",Max(3,17,36,25)); /* 在3个数中求最大值，ap最初指向“17,36,25” */
   
   return 0;
 }
 
 
