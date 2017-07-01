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
 
 #define NAMELEN 8 /* 姓名最大长度 */
 #define CLASSLEN 4 /* 班级名最大长度 */
 typedef struct
 {
   char name[NAMELEN+1]; /* 包括'\0' */
   long num;
   char sex;
   int age;
   char Class[CLASSLEN+1]; /* 包括'\0' */
   int health;
 }stud; /* 记录的结构 */
 typedef stud ElemType; /* 链表结点元素类型为结构体 */ 
 typedef struct LNode
 {
   ElemType data;
   struct LNode *next;
 };
 typedef struct LNode *LinkList; /* 另一种定义LinkList的方法 */
 
 #define DestroyList ClearList /* DestroyList()和ClearList()的操作是一样的 */
 void InitList(LinkList *L)
 { /* 操作结果：构造一个空的线性表L */
   *L=NULL; /* 指针为空 */
 }

 void ClearList(LinkList *L)
 { /* 初始条件：线性表L已存在。操作结果：将L重置为空表 */
   LinkList p;
   while(*L) /* L不空 */
   {
     p=*L; /* p指向首元结点 */
     *L=(*L)->next; /* L指向第2个结点(新首元结点) */
     free(p); /* 释放首元结点 */
   }
 }

 Status ListEmpty(LinkList L)
 { /* 初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
   if(L)
     return FALSE;
   else
     return TRUE;
 }

 int ListLength(LinkList L)
 { /* 初始条件：线性表L已存在。操作结果：返回L中数据元素个数 */
   int i=0;
   LinkList p=L;
   while(p) /* p指向结点(没到表尾) */
   {
     p=p->next; /* p指向下一个结点 */
     i++;
   }
   return i;
 }

 Status GetElem(LinkList L,int i,ElemType *e)
 { /* L为不带头结点的单链表的头指针。当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR */
   int j=1;
   LinkList p=L;
   if(i<1) /* i值不合法 */
     return ERROR;
   while(j<i&&p) /* 没到第i个元素，也没到表尾 */
   {
     j++;
     p=p->next;
   }
   if(j==i) /* 存在第i个元素 */
   {
     *e=p->data;
     return OK;
   }
   else
     return ERROR;
 }

 int LocateElem(LinkList L,ElemType e,Status(*compare)(ElemType,ElemType))
 { /* 初始条件：线性表L已存在，compare()是数据元素判定函数(满足为1，否则为0) */
   /* 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。 */
   /*           若这样的数据元素不存在，则返回值为0 */
   int i=0;
   LinkList p=L;
   while(p)
   {
     i++;
     if(compare(p->data,e)) /* 找到这样的数据元素 */
       return i;
     p=p->next;
   }
   return 0;
 }

 Status ListInsert(LinkList *L,int i,ElemType e)
 { /* 在不带头结点的单链线性表L中第i个位置之前插入元素e */
   int j=1;
   LinkList p=*L,s;
   if(i<1) /* i值不合法 */
     return ERROR;
   s=(LinkList)malloc(sizeof(struct LNode)); /* 生成新结点 */
   s->data=e; /* 给s的data域赋值 */
   if(i==1) /* 插在表头 */
   {
     s->next=*L;
     *L=s; /* 改变L */
   }
   else
   { /* 插在表的其余处 */
     while(p&&j<i-1) /* 寻找第i-1个结点 */
     {
       p=p->next;
       j++;
     }
     if(!p) /* i大于表长+1 */
       return ERROR;
     s->next=p->next;
     p->next=s;
   }
   return OK;
 }

 Status ListDelete(LinkList *L,int i,ElemType *e)
 { /* 在不带头结点的单链线性表L中，删除第i个元素，并由e返回其值 */
   int j=1;
   LinkList p=*L,q;
   if(i==1) /* 删除第1个结点 */
   {
     *L=p->next; /* L由第2个结点开始 */
     *e=p->data;
     free(p); /* 删除并释放第1个结点 */
   }
   else
   {
     while(p->next&&j<i-1) /* 寻找第i个结点，并令p指向其前驱 */
     {
       p=p->next;
       j++;
     }
     if(!p->next||j>i-1) /* 删除位置不合理 */
       return ERROR;
     q=p->next; /* 删除并释放结点 */
     p->next=q->next;
     *e=q->data;
     free(q);
   }
   return OK;
 }

 void ListTraverse(LinkList L,void(*vi)(ElemType))
 { /* 初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi() */
   LinkList p=L;
   while(p)
   {
     vi(p->data);
     p=p->next;
   }
   printf("\n");
 }
 
  void InsertAscend(LinkList *L,ElemType e,int(*compare)(ElemType,ElemType))
 { /* 按关键字非降序将e插入表L。函数compare()返回值为：形参1的关键字-形参2的关键字 */
   LinkList q=*L;
   if(!*L||compare(e,(*L)->data)<=0) /* 链表空或e的关键字小于等于首结点的关键字 */
     ListInsert(L,1,e); /* 将e插在表头，在bo2-8.c中 */
   else
   {
     while(q->next&&compare(q->next->data,e)<0) /* q不是尾结点且q的下一结点关键字<e的关键字 */
       q=q->next;
     ListInsert(&q,2,e); /* 插在q所指结点后(将q作为头指针) */
   }
 }

 LinkList Point(LinkList L,ElemType e,Status(*equal)(ElemType,ElemType),LinkList *p)
 { /* 查找表L中满足条件的结点。如找到，返回指向该结点的指针，p指向该结点的前驱(若该结点是 */
   /* 首元结点，则p=NULL)。如表L中无满足条件的结点，则返回NULL，p无定义。*/
   /* 函数equal()的两形参的关键字相等，返回OK；否则返回ERROR */
   int i,j;
   i=LocateElem(L,e,equal);
   if(i) /* 找到 */
   {
     if(i==1) /* 是首元结点 */
     {
       *p=NULL;
       return L;
     }
     *p=L;
     for(j=2;j<i;j++)
       *p=(*p)->next;
     return (*p)->next;
   }
   return NULL; /* 没找到 */
 }

 Status DeleteElem(LinkList *L,ElemType *e,Status(*equal)(ElemType,ElemType))
 { /* 删除表L中满足条件的结点，并返回TRUE；如无此结点，则返回FALSE。 */
   /* 函数equal()的两形参的关键字相等，返回OK；否则返回ERROR */
   LinkList p,q;
   q=Point(*L,*e,equal,&p);
   if(q) /* 找到此结点，且q指向该结点 */
   {
     if(p) /* 该结点不是首元结点，p指向其前驱 */
       ListDelete(&p,2,e); /* 将p作为头指针，删除第2个结点 */
     else /* 该结点是首元结点 */
       ListDelete(L,1,e);
     return TRUE;
   }
   return FALSE;
 }
 
 char sta[3][9]={"健康  ","一般  ","神经衰弱"}; /* 健康状况(3类) */
 FILE *fp; /* 全局变量 */

 void Print(stud e)
 { /* 显示记录e的内容 */
   printf("%-8s %6ld",e.name,e.num);
   if(e.sex=='m')
     printf(" 男");
   else
     printf(" 女");
   printf("%5d  %-4s",e.age,e.Class);
   printf("%9s\n",sta[e.health]);
 }

 void ReadIn(stud *e)
 { /* 由键盘输入结点信息 */
   printf("请输入姓名(<=%d个字符): ",NAMELEN);
   scanf("%s",(*e).name);
   printf("请输入学号: ");
   scanf("%ld",&(*e).num);
   printf("请输入性别(m:男 f:女): ");
   scanf("%*c%c",&(*e).sex);
   printf("请输入年龄: ");
   scanf("%d",&(*e).age);
   printf("请输入班级(<=%d个字符): ",CLASSLEN);
   scanf("%s",(*e).Class);
   printf("请输入健康状况(0:%s 1:%s 2:%s):",sta[0],sta[1],sta[2]);
   scanf("%d",&(*e).health);
 }

 void WriteToFile(stud e)
 { /* 将结点信息写入fp指定的文件 */
   fwrite(&e,sizeof(stud),1,fp);
 }

 Status ReadFromFile(stud *e)
 { /* 由fp指定的文件读取结点信息到e */
   int i;
   i=fread(e,sizeof(stud),1,fp);
   if(i==1) /* 读取文件成功 */
     return OK;
   else
     return ERROR;
 }

 int cmp(ElemType c1,ElemType c2)
 {
   return (int)(c1.num-c2.num);
 }

 void Modify(LinkList *L,ElemType e)
 { /* 修改结点内容，并按学号将结点非降序插入链表L */
   char s[80];
   Print(e); /* 显示原内容 */
   printf("请输入待修改项的内容，不修改的项按回车键保持原值:\n");
   printf("请输入姓名(<=%d个字符): ",NAMELEN);
   gets(s);
   if(strlen(s))
     strcpy(e.name,s);
   printf("请输入学号: ");
   gets(s);
   if(strlen(s))
     e.num=atol(s);
   printf("请输入性别(m:男 f:女): ");
   gets(s);
   if(strlen(s))
     e.sex=s[0];
   printf("请输入年龄: ");
   gets(s);
   if(strlen(s))
     e.age=atoi(s);
   printf("请输入班级(<=%d个字符): ",CLASSLEN);
   gets(s);
   if(strlen(s))
     strcpy(e.Class,s);
   printf("请输入健康状况(0:%s 1:%s 2:%s):",sta[0],sta[1],sta[2]);
   gets(s);
   if(strlen(s))
     e.health=atoi(s); /* 修改完毕 */
   InsertAscend(L,e,cmp); /* 把q所指结点的内容按学号非降序插入L(func2-1.c) */
 }

 #define N 4 /* student记录的个数 */

 Status EqualNum(ElemType c1,ElemType c2)
 {
   if(c1.num==c2.num)
     return OK;
   else
     return ERROR;
 }

 Status EqualName(ElemType c1,ElemType c2)
 {
   if(strcmp(c1.name,c2.name))
     return ERROR;
   else
     return OK;
 }

 int main()
 { /* 表的初始记录 */
   stud student[N]={{"王小林",790631,'m',18,"计91",0},{"陈红",790632,'f',20,"计91",1},
                    {"刘建平",790633,'m',21,"计91",0},{"张立立",790634,'m',17,"计91",2}};
   int i,j,flag=1;
   char filename[13];
   ElemType e;
   LinkList T,p,q;
   InitList(&T); /* 初始化链表 */
   while(flag)
   {
     printf("1:将结构体数组student中的记录按学号非降序插入链表\n");
     printf("2:将文件中的记录按学号非降序插入链表\n");
     printf("3:键盘输入新记录，并将其按学号非降序插入链表\n");
     printf("4:删除链表中第一个有给定学号的记录\n");
     printf("5:删除链表中第一个有给定姓名的记录\n");
     printf("6:修改链表中第一个有给定学号的记录\n");
     printf("7:修改链表中第一个有给定姓名的记录\n");
     printf("8:查找链表中第一个有给定学号的记录\n");
     printf("9:查找链表中第一个有给定姓名的记录\n");
     printf("10:显示所有记录 11:将链表中的所有记录存入文件 12:结束\n");
     printf("请选择操作命令: ");
     scanf("%d",&i);
     switch(i)
     {
       case 1: for(j=0;j<N;j++)
		 InsertAscend(&T,student[j],cmp); /* 在func2-1.c中 */
               break;
       case 2: printf("请输入文件名: ");
               scanf("%s",filename);
               if((fp=fopen(filename,"rb"))==NULL)
                 printf("打开文件失败!\n");
               else
               {
                 while(ReadFromFile(&e))
		   InsertAscend(&T,e,cmp); /* 在func2-1.c中 */
                 fclose(fp);
               }
               break;
       case 3: ReadIn(&e);
	       InsertAscend(&T,e,cmp); /* 在func2-1.c中 */
               break;
       case 4: printf("请输入待删除记录的学号: ");
               scanf("%ld",&e.num);
               if(!DeleteElem(&T,&e,EqualNum)) /* 在func2-1.c中 */
                 printf("没有学号为%ld的记录\n",e.num);
               break;
       case 5: printf("请输入待删除记录的姓名: ");
               scanf("%*c%s",e.name); /* %*c吃掉回车符 */
               if(!DeleteElem(&T,&e,EqualName)) /* 在func2-1.c中 */
                 printf("没有姓名为%s的记录\n",e.name);
               break;
       case 6: printf("请输入待修改记录的学号: ");
               scanf("%ld%*c",&e.num);
	       if(!DeleteElem(&T,&e,EqualNum)) /* 在链表中删除该结点，并由e返回(func2-1.c) */
                 printf("没有学号为%ld的记录\n",e.num);
               else
                 Modify(&T,e); /* 修改e并按学号插入链表T */
               break;
       case 7: printf("请输入待修改记录的姓名: ");
               scanf("%*c%s%*c",e.name); /* %*c吃掉回车符 */
               if(!DeleteElem(&T,&e,EqualName)) /* func2-1.c */
                 printf("没有姓名为%s的记录\n",e.name);
               else
                 Modify(&T,e);
               break;
       case 8: printf("请输入待查找记录的学号: ");
               scanf("%ld",&e.num);
               q=Point(T,e,EqualNum,&p); /* func2-1.c */
               if(!q)
                 printf("没有学号为%ld的记录\n",e.num);
               else
                 Print(q->data);
               break;
       case 9: printf("请输入待查找记录的姓名: ");
               scanf("%*c%s",e.name);
               q=Point(T,e,EqualName,&p); /* func2-1.c */
               if(!q)
                 printf("没有姓名为%s的记录\n",e.name);
               else
                 Print(q->data);
               break;
       case 10:printf("  姓名    学号 性别 年龄 班级 健康状况\n");
               ListTraverse(T,Print);
               break;
       case 11:printf("请输入文件名: ");
               scanf("%s",filename);
               if((fp=fopen(filename,"wb"))==NULL)
                 printf("打开文件失败!\n");
               else
                 ListTraverse(T,WriteToFile);
               fclose(fp);
               break;
       case 12:flag=0;
     }
   }
   
   return 0; 
 }
 
 
 
 
 
 