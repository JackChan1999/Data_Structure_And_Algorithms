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
 
 typedef int ElemType;
 
 typedef struct LNode /* 结点类型 */
 {
   ElemType data;
   struct LNode *next;
 }LNode,*Link,*Position;

 typedef struct LinkList /* 链表类型 */
 {
   Link head,tail; /* 分别指向线性链表中的头结点和最后一个结点 */
   int len; /* 指示线性链表中数据元素的个数 */
 }LinkList;
 
 typedef struct
 {
   char *ch; /* 若是非空串，则按串长分配存储区，否则ch为NULL */
   int length; /* 串长度 */
 }HString;
 
 #define DestroyString ClearString /* DestroyString()与ClearString()作用相同 */
  #define MaxBookNum 10 /* 假设只对10个书名建索引表 */
 #define MaxKeyNum 25 /* 索引表的最大容量(关键词的最大数目) */
 #define MaxLineLen 46 /* 书名的最大长度 */

 typedef struct
 {
   HString key; /* 关键词(堆分配类型，c4-2.h) */
   LinkList bnolist; /* 存放书号索引的链表(c2-5.h) */
 }IdxTermType; /* 索引项类型 */

 typedef struct
 {
   IdxTermType item[MaxKeyNum+1]; /* 索引项数组类型 */
   int last; /* 关键词的个数 */
 }IdxListType; /* 索引表类型(有序表) */

 typedef struct
 {
   char bookname[MaxLineLen+1]; /* 书名串(包括'\0') */
   int bookno; /* 书号 */
 }BookTermType; /* 书目项类型 */

 typedef struct
 {
   BookTermType item[MaxBookNum]; /* 书目项数组类型 */
   int last; /* 书目的数量 */
 }BookListType; /* 书目表类型(有序表) */
 
 void MakeNode(Link *p,ElemType e)
 { /* 分配由p指向的值为e的结点。若分配失败，则退出 */
   *p=(Link)malloc(sizeof(LNode));
   if(!*p)
     exit(ERROR);
   (*p)->data=e;
 }

 void FreeNode(Link *p)
 { /* 释放p所指结点 */
   free(*p);
   *p=NULL;
 }

 void InitList(LinkList *L)
 { /* 构造一个空的线性链表L */
   Link p;
   p=(Link)malloc(sizeof(LNode)); /* 生成头结点 */
   if(p)
   {
     p->next=NULL;
     (*L).head=(*L).tail=p;
     (*L).len=0;
   }
   else
     exit(ERROR);
 }

 void ClearList(LinkList *L)
 { /* 将线性链表L重置为空表，并释放原链表的结点空间 */
   Link p,q;
   if((*L).head!=(*L).tail) /* 不是空表 */
   {
     p=q=(*L).head->next;
     (*L).head->next=NULL;
     while(p!=(*L).tail)
     {
       p=q->next;
       free(q);
       q=p;
     }
     free(q);
     (*L).tail=(*L).head;
     (*L).len=0;
   }
 }

 void DestroyList(LinkList *L)
 { /* 销毁线性链表L，L不再存在 */
   ClearList(L); /* 清空链表 */
   FreeNode(&(*L).head);
   (*L).tail=NULL;
   (*L).len=0;
 }

 void InsFirst(LinkList *L,Link h,Link s) /* 形参增加L,因为需修改L */
 { /* h指向L的一个结点，把h当做头结点，将s所指结点插入在第一个结点之前 */
   s->next=h->next;
   h->next=s;
   if(h==(*L).tail) /* h指向尾结点 */
     (*L).tail=h->next; /* 修改尾指针 */
   (*L).len++;
 }

 Status DelFirst(LinkList *L,Link h,Link *q) /* 形参增加L,因为需修改L */
 { /* h指向L的一个结点，把h当做头结点，删除链表中的第一个结点并以q返回。 */
   /* 若链表为空(h指向尾结点)，q=NULL，返回FALSE */
   *q=h->next;
   if(*q) /* 链表非空 */
   {
     h->next=(*q)->next;
     if(!h->next) /* 删除尾结点 */
       (*L).tail=h; /* 修改尾指针 */
     (*L).len--;
     return OK;
   }
   else
     return FALSE; /* 链表空 */
 }

 void Append(LinkList *L,Link s)
 { /* 将指针s(s->data为第一个数据元素)所指(彼此以指针相链，以NULL结尾)的 */
   /* 一串结点链接在线性链表L的最后一个结点之后，并改变链表L的尾指针指向新的尾结点 */
   int i=1;
   (*L).tail->next=s;
   while(s->next)
   {
     s=s->next;
     i++;
   }
   (*L).tail=s;
   (*L).len+=i;
 }

 Position PriorPos(LinkList L,Link p)
 { /* 已知p指向线性链表L中的一个结点，返回p所指结点的直接前驱的位置。若无前驱，则返回NULL */
   Link q;
   q=L.head->next;
   if(q==p) /* 无前驱 */
     return NULL;
   else
   {
     while(q->next!=p) /* q不是p的直接前驱 */
       q=q->next;
     return q;
   }
 }

 Status Remove(LinkList *L,Link *q)
 { /* 删除线性链表L中的尾结点并以q返回，改变链表L的尾指针指向新的尾结点 */
   Link p=(*L).head;
   if((*L).len==0) /* 空表 */
   {
     *q=NULL;
     return FALSE;
   }
   while(p->next!=(*L).tail)
     p=p->next;
   *q=(*L).tail;
   p->next=NULL;
   (*L).tail=p;
   (*L).len--;
   return OK;
 }

 void InsBefore(LinkList *L,Link *p,Link s)
 { /* 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之前， */
   /* 并修改指针p指向新插入的结点 */
   Link q;
   q=PriorPos(*L,*p); /* q是p的前驱 */
   if(!q) /* p无前驱 */
     q=(*L).head;
   s->next=*p;
   q->next=s;
   *p=s;
   (*L).len++;
 }

 void InsAfter(LinkList *L,Link *p,Link s)
 { /* 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之后， */
   /* 并修改指针p指向新插入的结点 */
   if(*p==(*L).tail) /* 修改尾指针 */
     (*L).tail=s;
   s->next=(*p)->next;
   (*p)->next=s;
   *p=s;
   (*L).len++;
 }

 void SetCurElem(Link p,ElemType e)
 { /* 已知p指向线性链表中的一个结点，用e更新p所指结点中数据元素的值 */
   p->data=e;
 }

 ElemType GetCurElem(Link p)
 { /* 已知p指向线性链表中的一个结点，返回p所指结点中数据元素的值 */
   return p->data;
 }

 Status ListEmpty(LinkList L)
 { /* 若线性链表L为空表，则返回TRUE，否则返回FALSE */
   if(L.len)
     return FALSE;
   else
     return TRUE;
 }

 int ListLength(LinkList L)
 { /* 返回线性链表L中元素个数 */
   return L.len;
 }

 Position GetHead(LinkList L)
 { /* 返回线性链表L中头结点的位置 */
   return L.head;
 }

 Position GetLast(LinkList L)
 { /* 返回线性链表L中最后一个结点的位置 */
   return L.tail;
 }

 Position NextPos(Link p)
 { /* 已知p指向线性链表L中的一个结点，返回p所指结点的直接后继的位置。若无后继，则返回NULL */
   return p->next;
 }

 Status LocatePos(LinkList L,int i,Link *p)
 { /* 返回p指示线性链表L中第i个结点的位置，并返回OK，i值不合法时返回ERROR。i=0为头结点 */
   int j;
   if(i<0||i>L.len)
     return ERROR;
   else
   {
     *p=L.head;
     for(j=1;j<=i;j++)
       *p=(*p)->next;
     return OK;
   }
 }

 Position LocateElem(LinkList L,ElemType e,Status (*compare)(ElemType,ElemType))
 { /* 返回线性链表L中第1个与e满足函数compare()判定关系的元素的位置， */
   /* 若不存在这样的元素，则返回NULL */
   Link p=L.head;
   do
     p=p->next;
   while(p&&!(compare(p->data,e))); /* 没到表尾且没找到满足关系的元素 */
   return p;
 }

 void ListTraverse(LinkList L,void(*visit)(ElemType))
 { /* 依次对L的每个数据元素调用函数visit() */
   Link p=L.head->next;
   int j;
   for(j=1;j<=L.len;j++)
   {
     visit(p->data);
     p=p->next;
   }
   printf("\n");
 }

 void OrderInsert(LinkList *L,ElemType e,int (*comp)(ElemType,ElemType))
 { /* 已知L为有序线性链表，将元素e按非降序插入在L中。(用于一元多项式) */
   Link o,p,q;
   q=(*L).head;
   p=q->next;
   while(p!=NULL&&comp(p->data,e)<0) /* p不是表尾且元素值小于e */
   {
     q=p;
     p=p->next;
   }
   o=(Link)malloc(sizeof(LNode)); /* 生成结点 */
   o->data=e; /* 赋值 */
   q->next=o; /* 插入 */
   o->next=p;
   (*L).len++; /* 表长加1 */
   if(!p) /* 插在表尾 */
     (*L).tail=o; /* 修改尾结点 */
 }

 Status LocateElemP(LinkList L,ElemType e,Position *q,int(*compare)(ElemType,ElemType))
 { /* 若升序链表L中存在与e满足判定函数compare()取值为0的元素，则q指示L中 */
   /* 第一个值为e的结点的位置，并返回TRUE；否则q指示第一个与e满足判定函数 */
   /* compare()取值>0的元素的前驱的位置。并返回FALSE。(用于一元多项式) */
   Link p=L.head,pp;
   do
   {
     pp=p;
     p=p->next;
   }while(p&&(compare(p->data,e)<0)); /* 没到表尾且p->data.expn<e.expn */
   if(!p||compare(p->data,e)>0) /* 到表尾或compare(p->data,e)>0 */
   {
     *q=pp;
     return FALSE;
   }
   else /* 找到 */
   {
     *q=p;
     return TRUE;
   }
 }
 
  void StrAssign(HString *T,char *chars)
 { /* 生成一个其值等于串常量chars的串T */
   int i,j;
   if((*T).ch)
     free((*T).ch); /* 释放T原有空间 */
   i=strlen(chars); /* 求chars的长度i */
   if(!i)
   { /* chars的长度为0 */
     (*T).ch=NULL;
     (*T).length=0;
   }
   else
   { /* chars的长度不为0 */
     (*T).ch=(char*)malloc(i*sizeof(char)); /* 分配串空间 */
     if(!(*T).ch) /* 分配串空间失败 */
       exit(OVERFLOW);
     for(j=0;j<i;j++) /* 拷贝串 */
       (*T).ch[j]=chars[j];
     (*T).length=i;
   }
 }

 void StrCopy(HString *T,HString S)
 { /* 初始条件：串S存在。操作结果：由串S复制得串T */
   int i;
   if((*T).ch)
     free((*T).ch); /* 释放T原有空间 */
   (*T).ch=(char*)malloc(S.length*sizeof(char)); /* 分配串空间 */
   if(!(*T).ch) /* 分配串空间失败 */
     exit(OVERFLOW);
   for(i=0;i<S.length;i++) /* 拷贝串 */
     (*T).ch[i]=S.ch[i];
   (*T).length=S.length;
 }

 Status StrEmpty(HString S)
 { /* 初始条件：串S存在。操作结果：若S为空串，则返回TRUE，否则返回FALSE */
   if(S.length==0&&S.ch==NULL)
     return TRUE;
   else
     return FALSE;
 }

 int StrCompare(HString S,HString T)
 { /* 若S>T，则返回值>0；若S=T，则返回值=0；若S<T，则返回值<0 */
   int i;
   for(i=0;i<S.length&&i<T.length;++i)
     if(S.ch[i]!=T.ch[i])
       return S.ch[i]-T.ch[i];
   return S.length-T.length;
 }

 int StrLength(HString S)
 { /* 返回S的元素个数，称为串的长度 */
   return S.length;
 }

 void ClearString(HString *S)
 { /* 将S清为空串 */
   free((*S).ch);
   (*S).ch=NULL;
   (*S).length=0;
 }

 void Concat(HString *T,HString S1,HString S2)
 { /* 用T返回由S1和S2联接而成的新串 */
   int i;
   if((*T).ch)
     free((*T).ch); /* 释放旧空间 */
   (*T).length=S1.length+S2.length;
   (*T).ch=(char *)malloc((*T).length*sizeof(char));
   if(!(*T).ch)
     exit(OVERFLOW);
   for(i=0;i<S1.length;i++)
     (*T).ch[i]=S1.ch[i];
   for(i=0;i<S2.length;i++)
     (*T).ch[S1.length+i]=S2.ch[i];
 }

 Status SubString(HString *Sub, HString S,int pos,int len)
 { /* 用Sub返回串S的第pos个字符起长度为len的子串。 */
   /* 其中，1≤pos≤StrLength(S)且0≤len≤StrLength(S)-pos+1 */
   int i;
   if(pos<1||pos>S.length||len<0||len>S.length-pos+1)
     return ERROR;
   if((*Sub).ch)
     free((*Sub).ch); /* 释放旧空间 */
   if(!len) /* 空子串 */
   {
     (*Sub).ch=NULL;
     (*Sub).length=0;
   }
   else
   { /* 完整子串 */
     (*Sub).ch=(char*)malloc(len*sizeof(char));
     if(!(*Sub).ch)
       exit(OVERFLOW);
     for(i=0;i<=len-1;i++)
       (*Sub).ch[i]=S.ch[pos-1+i];
     (*Sub).length=len;
   }
   return OK;
 }

 void InitString(HString *T)
 { /* 初始化(产生空串)字符串T。另加 */
   (*T).length=0;
   (*T).ch=NULL;
 }

 int Index(HString S,HString T,int pos) /* 算法4.1 */
 { /* T为非空串。若主串S中第pos个字符之后存在与T相等的子串，*/
   /* 则返回第一个这样的子串在S中的位置，否则返回0 */
   int n,m,i;
   HString sub;
   InitString(&sub);
   if(pos>0)
   {
     n=StrLength(S);
     m=StrLength(T);
     i=pos;
     while(i<=n-m+1)
     {
       SubString(&sub,S,i,m);
       if(StrCompare(sub,T)!=0)
         ++i;
       else
	 return i;
     }
   }
   return 0;
  }

 Status StrInsert(HString *S,int pos,HString T) /* 算法4.4 */
 { /* 1≤pos≤StrLength(S)+1。在串S的第pos个字符之前插入串T */
   int i;
   if(pos<1||pos>(*S).length+1) /* pos不合法 */
     return ERROR;
   if(T.length) /* T非空，则重新分配空间，插入T */
   {
     (*S).ch=(char*)realloc((*S).ch,((*S).length+T.length)*sizeof(char));
     if(!(*S).ch)
       exit(OVERFLOW);
     for(i=(*S).length-1;i>=pos-1;--i) /* 为插入T而腾出位置 */
       (*S).ch[i+T.length]=(*S).ch[i];
     for(i=0;i<T.length;i++)
       (*S).ch[pos-1+i]=T.ch[i]; /* 插入T */
     (*S).length+=T.length;
   }
   return OK;
 }

 Status StrDelete(HString *S,int pos,int len)
 { /* 从串S中删除第pos个字符起长度为len的子串 */
   int i;
   if((*S).length<pos+len-1)
     return ERROR;
   for(i=pos-1;i<=(*S).length-len;i++)
     (*S).ch[i]=(*S).ch[i+len];
   (*S).length-=len;
   (*S).ch=(char*)realloc((*S).ch,(*S).length*sizeof(char));
   return OK;
 }

 Status Replace(HString *S,HString T,HString V) /* 此函数与串的存储结构无关 */
 { /* 初始条件：串S，T和V存在，T是非空串 */
   /* 操作结果：用V替换主串S中出现的所有与T相等的不重叠的子串 */
   int i=1; /* 从串S的第一个字符起查找串T */
   if(StrEmpty(T)) /* T是空串 */
     return ERROR;
   do
   {
     i=Index(*S,T,i); /* 结果i为从上一个i之后找到的子串T的位置 */
     if(i) /* 串S中存在串T */
     {
       StrDelete(S,i,StrLength(T)); /* 删除该串T */
       StrInsert(S,i,V); /* 在原串T的位置插入串V */
       i+=StrLength(V); /* 在插入的串V后面继续查找串T */
     }
   }while(i);
   return OK;
 }

 void StrPrint(HString T)
 { /* 输出T字符串。另加 */
   int i;
   for(i=0;i<T.length;i++)
     printf("%c",T.ch[i]);
   printf("\n");
 }
 
 int main()
 {
   FILE *f; /* 任何时间最多打开一个文件 */
   IdxListType idxlist; /* 索引表 */
   BookListType booklist; /* 书目表 */
   char buf[MaxLineLen+5]; /* 当前书目串(包括书号和'\0') */
   HString ch; /* 索引字符串 */
   int BookNo; /* 书号 */
   Link p; /* 链表指针 */
   int i,j,k,flag=1; /* flag是继续查询的标志 */
   InitString(&ch); /* 初始化HString类型的变量 */
   f=fopen("BookIdx.txt","r"); /* 打开书名关键词索引表文件 */
   if(!f)
     exit(OVERFLOW);
   fscanf(f,"%d",&idxlist.last); /* 书名关键词个数 */
   for(k=0;k<idxlist.last;k++) /* 把关键词文件的内容拷到idxlist中 */
   {
     fscanf(f,"%s",buf);
     i=0;
     while(buf[i])
       buf[i++]=tolower(buf[i]); /* 字母转为小写 */
     InitString(&idxlist.item[k].key);
     StrAssign(&idxlist.item[k].key,buf);
     InitList(&idxlist.item[k].bnolist); /* 初始化书号链表，bo2-6.c */
     fscanf(f,"%d",&i);
     for(j=0;j<i;j++)
     {
       fscanf(f,"%d",&BookNo);
       MakeNode(&p,BookNo); /* 产生新的书号结点，bo2-6.c */
       p->next=NULL; /* 给书号结点的指针域赋值 */
       Append(&idxlist.item[k].bnolist,p); /* 在表尾插入新的书号结点，bo2-6.c */
     }
   }
   fclose(f);
   f=fopen("BookInfo.txt","r"); /* 打开书目文件 */
   if(!f)
     exit(FALSE);
   i=0;
   while(fgets(buf,MaxLineLen,f))
   { /* 把书目文件的内容拷到booklist中 */
     booklist.item[i].bookno=atoi(buf); /* 前几位数字为书号 */
     strcpy(booklist.item[i++].bookname,&buf[4]); /* 将buf由书名开始的字符串拷贝到booklist中 */
   }
   booklist.last=i;
   while(flag)
   {
     printf("请输入书目的关键词(一个)：");
     scanf("%s",buf);
     i=0;
     while(buf[i])
       buf[i++]=tolower(buf[i]); /* 字母转为小写 */
     StrAssign(&ch,buf);
     i=0;
     do
     {
       k=StrCompare(ch,idxlist.item[i++].key); /* bo4-2.c */
     }while(k&&i<=idxlist.last);
     if(!k) /* 索引表中有此关键词 */
     {
       p=idxlist.item[--i].bnolist.head->next; /* p指向索引表中此关键词相应链表的首元结点 */
       while(p)
       {
         j=0;
         while(j<booklist.last&&p->data!=booklist.item[j].bookno) /* 在booklist中找相应的书号 */
           j++;
         if(j<booklist.last)
           printf("%3d %s",booklist.item[j].bookno,booklist.item[j].bookname);
         p=p->next; /* 继续向后找 */
       }
     }
     else
       printf("没找到\n");
     printf("继续查找请输入1，退出查找请输入0：");
     scanf("%d",&flag);
   }
   
   return 0;
 }

 
 