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
 
 #define N 5 /* 数据元素个数 */
 typedef long KeyType; /* 设关键字域为长整型 */
 #define key number /* 定义关键字为准考证号 */
 typedef struct
 {
   long number; /* 准考证号，与关键字类型同 */
   char name[9]; /* 姓名(4个汉字加1个串结束标志) */
   int politics; /* 政治 */
   int Chinese; /* 语文 */
   int English; /* 英语 */
   int math; /* 数学 */
   int physics; /* 物理 */
   int chemistry; /* 化学 */
   int biology; /* 生物 */
   int total; /* 总分 */
 }ElemType; /* 数据元素类型(以教科书图9.1高考成绩为例) */
 #define EQ(a,b) ((a)==(b))
 #define LT(a,b) ((a)<(b))
 #define LQ(a,b) ((a)<=(b))
 typedef struct
 {
   ElemType *elem; /* 数据元素存储空间基址，建表时按实际长度分配，0号单元留空 */
   int length; /* 表长度 */
 }SSTable;

void Creat_Seq(SSTable *ST,ElemType r[],int n)
 { /* 操作结果：由含n个数据元素的数组r构造静态顺序查找表ST */
   int i;
   (*ST).elem=(ElemType*)calloc(n+1,sizeof(ElemType)); /* 动态生成n+1个数据元素空间(0号单元不用) */
   if(!(*ST).elem)
     exit(ERROR);
   for(i=1;i<=n;i++)
     (*ST).elem[i]=r[i-1]; /* 将数组r的值依次赋给ST */
   (*ST).length=n;
 }

 void Ascend(SSTable *ST)
 { /* 重建静态查找表为按关键字非降序排序 */
   int i,j,k;
   for(i=1;i<(*ST).length;i++)
   {
     k=i;
     (*ST).elem[0]=(*ST).elem[i]; /* 待比较值存[0]单元 */
     for(j=i+1;j<=(*ST).length;j++)
       if LT((*ST).elem[j].key,(*ST).elem[0].key)
       {
	 k=j;
	 (*ST).elem[0]=(*ST).elem[j];
       }
     if(k!=i) /* 有更小的值则交换 */
     {
       (*ST).elem[k]=(*ST).elem[i];
       (*ST).elem[i]=(*ST).elem[0];
     }
   }
 }

 void Creat_Ord(SSTable *ST,ElemType r[],int n)
 { /* 操作结果：由含n个数据元素的数组r构造按关键字非降序查找表ST */
   Creat_Seq(ST,r,n); /* 建立无序的查找表ST */
   Ascend(ST); /* 将无序的查找表ST重建为按关键字非降序查找表 */
 }

 Status Destroy(SSTable *ST)
 { /* 初始条件：静态查找表ST存在。操作结果：销毁表ST */
   free((*ST).elem);
   (*ST).elem=NULL;
   (*ST).length=0;
   return OK;
 }

 int Search_Seq(SSTable ST,KeyType key)
 { /* 在顺序表ST中顺序查找其关键字等于key的数据元素。若找到，则返回 */
   /* 该元素在表中的位置，否则返回0。算法9.1 */
   int i;
   ST.elem[0].key=key; /* 哨兵 */
   for(i=ST.length;!EQ(ST.elem[i].key,key);--i); /* 从后往前找 */
   return i; /* 找不到时，i为0 */
 }

 int Search_Bin(SSTable ST,KeyType key)
 { /* 在有序表ST中折半查找其关键字等于key的数据元素。若找到，则返回 */
   /* 该元素在表中的位置，否则返回0。算法9.2 */
   int low,high,mid;
   low=1; /* 置区间初值 */
   high=ST.length;
   while(low<=high)
   {
     mid=(low+high)/2;
     if EQ(key,ST.elem[mid].key)  /* 找到待查元素 */
       return mid;
     else if LT(key,ST.elem[mid].key)
       high=mid-1; /* 继续在前半区间进行查找 */
     else
       low=mid+1; /* 继续在后半区间进行查找 */
   }
   return 0; /* 顺序表中不存在待查元素 */
 }

 void Traverse(SSTable ST,void(*Visit)(ElemType))
 { /* 初始条件：静态查找表ST存在，Visit()是对元素操作的应用函数 */
   /* 操作结果：按顺序对ST的每个元素调用函数Visit()一次且仅一次 */
   ElemType *p;
   int i;
   p=++ST.elem; /* p指向第一个元素 */
   for(i=1;i<=ST.length;i++)
     Visit(*p++);
 }

 void print(ElemType c) /* Traverse()调用的函数 */
 {
   printf("%-8ld%-8s%4d%5d%5d%5d%5d%5d%5d%5d\n",c.number,c.name,c.politics,
	   c.Chinese,c.English,c.math,c.physics,c.chemistry,c.biology,c.total);
 }

 int main()
 {
   ElemType r[N]={{179328,"何芳芳",85,89,98,100,93,80,47},
   {179325,"陈红",85,86,88,100,92,90,45},
	{179326,"陆华",78,75,90,80,95,88,37},
	{179327,"张平",82,80,78,98,84,96,40},
	{179324,"赵小怡",76,85,94,57,77,69,44}}; /* 数组不按关键字有序 */
   SSTable st;
   int i;
   long s;
   for(i=0;i<N;i++) /* 计算总分 */
     r[i].total=r[i].politics+r[i].Chinese+r[i].English+r[i].math+r[i].physics+
	 r[i].chemistry+r[i].biology;
   Creat_Seq(&st,r,N); /* 由数组r产生顺序静态查找表st */
   printf("准考证号  姓名  政治 语文 外语 数学 物理 化学 生物 总分\n");
   Traverse(st,print); /* 按顺序输出静态查找表st */
   printf("请输入待查找人的考号: ");
   scanf("%ld",&s);
   i=Search_Seq(st,s); /* 顺序查找 */
   if(i)
     print(st.elem[i]);
   else
     printf("没找到\n");
   Destroy(&st);
   
   return 0;
 }