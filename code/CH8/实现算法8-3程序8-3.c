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
 typedef char AtomType; /* 定义原子类型为字符型 */
 typedef enum{ATOM,LIST}ElemTag; /* ATOM==0：原子，LIST==1：子表 */
 typedef struct GLNode
 {
   int mark; /* 加此域，其余同c5-5.h */
   ElemTag tag; /* 公共部分，用于区分原子结点和表结点 */
   union /* 原子结点和表结点的联合部分 */
   {
     AtomType atom; /* atom是原子结点的值域，AtomType由用户定义 */
     struct
     {
       struct GLNode *hp,*tp;
     }ptr; /* ptr是表结点的指针域，prt.hp和ptr.tp分别指向表头和表尾 */
   }a;
 }*GList,GLNode; /* 广义表类型 */
 
 #define MAX_STR_LEN 40 /* 用户可在255(1个字节)以内定义最大串长 */
 typedef char SString[MAX_STR_LEN+1]; /* 0号单元存放串的长度 */
 #define DestroyString ClearString /* DestroyString()与ClearString()作用相同 */

 Status StrAssign(SString T,char *chars)
 { /* 生成一个其值等于chars的串T */
   int i;
   if(strlen(chars)>MAX_STR_LEN)
     return ERROR;
   else
   {
     T[0]=strlen(chars);
     for(i=1;i<=T[0];i++)
       T[i]=*(chars+i-1);
     return OK;
   }
 }

 void StrCopy(SString T,SString S)
 { /* 由串S复制得串T */
   int i;
   for(i=0;i<=S[0];i++)
     T[i]=S[i];
 }

 Status StrEmpty(SString S)
 { /* 若S为空串，则返回TRUE，否则返回FALSE */
   if(S[0]==0)
     return TRUE;
   else
     return FALSE;
 }

 int StrCompare(SString S,SString T)
 { /* 初始条件：串S和T存在。操作结果：若S>T，则返回值>0；若S=T，则返回值=0；若S<T，则返回值<0 */
   int i;
   for(i=1;i<=S[0]&&i<=T[0];++i)
     if(S[i]!=T[i])
       return S[i]-T[i];
   return S[0]-T[0];
 }

 int StrLength(SString S)
 { /* 返回串S的元素个数 */
   return S[0];
 }

 void ClearString(SString S)
 { /* 初始条件：串S存在。操作结果：将S清为空串 */
   S[0]=0; /* 令串长为零 */
 }

 Status Concat(SString T,SString S1,SString S2) /* 算法4.2改 */
 { /* 用T返回S1和S2联接而成的新串。若未截断，则返回TRUE，否则FALSE */
   int i;
   if(S1[0]+S2[0]<=MAX_STR_LEN)
   { /* 未截断 */
     for(i=1;i<=S1[0];i++)
       T[i]=S1[i];
     for(i=1;i<=S2[0];i++)
       T[S1[0]+i]=S2[i];
     T[0]=S1[0]+S2[0];
     return TRUE;
   }
   else
   { /* 截断S2 */
     for(i=1;i<=S1[0];i++)
       T[i]=S1[i];
     for(i=1;i<=MAX_STR_LEN-S1[0];i++)
       T[S1[0]+i]=S2[i];
     T[0]=MAX_STR_LEN;
     return FALSE;
   }
 }

 Status SubString(SString Sub,SString S,int pos,int len)
 { /* 用Sub返回串S的第pos个字符起长度为len的子串。算法4.3 */
   int i;
   if(pos<1||pos>S[0]||len<0||len>S[0]-pos+1)
     return ERROR;
   for(i=1;i<=len;i++)
     Sub[i]=S[pos+i-1];
   Sub[0]=len;
   return OK;
 }

 int Index(SString S,SString T,int pos)
 { /* 返回子串T在主串S中第pos个字符之后的位置。若不存在，则函数值为0。 */
   /* 其中，T非空，1≤pos≤StrLength(S)。算法4.5 */
   int i,j;
   if(1<=pos&&pos<=S[0])
   {
     i=pos;
     j=1;
     while(i<=S[0]&&j<=T[0])
       if(S[i]==T[j]) /* 继续比较后继字符 */
       {
         ++i;
         ++j;
       }
       else /* 指针后退重新开始匹配 */
       {
         i=i-j+2;
         j=1;
       }
     if(j>T[0])
       return i-T[0];
     else
       return 0;
   }
   else
     return 0;
 }

 Status StrInsert(SString S,int pos,SString T)
 { /* 初始条件：串S和T存在，1≤pos≤StrLength(S)+1 */
   /* 操作结果：在串S的第pos个字符之前插入串T。完全插入返回TRUE，部分插入返回FALSE */
   int i;
   if(pos<1||pos>S[0]+1)
     return ERROR;
   if(S[0]+T[0]<=MAX_STR_LEN)
   { /* 完全插入 */
     for(i=S[0];i>=pos;i--)
       S[i+T[0]]=S[i];
     for(i=pos;i<pos+T[0];i++)
       S[i]=T[i-pos+1];
     S[0]+=T[0];
     return TRUE;
   }
   else
   { /* 部分插入 */
     for(i=MAX_STR_LEN;i>=pos+T[0];i--)
       S[i]=S[i-T[0]];
     for(i=pos;i<pos+T[0]&&i<=MAX_STR_LEN;i++)
       S[i]=T[i-pos+1];
     S[0]=MAX_STR_LEN;
     return FALSE;
   }
 }

 Status StrDelete(SString S,int pos,int len)
 { /* 初始条件：串S存在，1≤pos≤StrLength(S)-len+1 */
   /* 操作结果：从串S中删除第pos个字符起长度为len的子串 */
   int i;
   if(pos<1||pos>S[0]-len+1||len<0)
     return ERROR;
   for(i=pos+len;i<=S[0];i++)
     S[i-len]=S[i];
   S[0]-=len;
   return OK;
 }

 Status Replace(SString S,SString T,SString V) /* 此函数与串的存储结构无关 */
 { /* 初始条件：串S，T和V存在，T是非空串 */
   /* 操作结果：用V替换主串S中出现的所有与T相等的不重叠的子串 */
   int i=1; /* 从串S的第一个字符起查找串T */
   Status k;
   if(StrEmpty(T)) /* T是空串 */
     return ERROR;
   do
   {
     i=Index(S,T,i); /* 结果i为从上一个i之后找到的子串T的位置 */
     if(i) /* 串S中存在串T */
     {
       StrDelete(S,i,StrLength(T)); /* 删除该串T */
       k=StrInsert(S,i,V); /* 在原串T的位置插入串V */
       if(!k) /* 不能完全插入 */
         return ERROR;
       i+=StrLength(V); /* 在插入的串V后面继续查找串T */
     }
   }while(i);
   return OK;
 }

 void StrPrint(SString T)
 { /* 输出字符串T。另加 */
   int i;
   for(i=1;i<=T[0];i++)
     printf("%c",T[i]);
   printf("\n");
 }
 
 void sever(SString str,SString hstr) /* 算法5.8改。SString是数组，不需引用类型 */
 { /* 将非空串str分割成两部分：hstr为第一个','之前的子串，str为之后的子串 */
   int n,k,i; /* k记尚未配对的左括号个数 */
   SString ch,c1,c2,c3;
   n=StrLength(str); /* n为串str的长度 */
   StrAssign(c1,","); /* c1=',' */
   StrAssign(c2,"("); /* c2='(' */
   StrAssign(c3,")"); /* c3=')' */
   SubString(ch,str,1,1); /* ch为串str的第1个字符 */
   for(i=1,k=0;i<=n&&StrCompare(ch,c1)||k!=0;++i) /* i小于串长且ch不是',' */
   { /* 搜索最外层的第一个逗号 */
     SubString(ch,str,i,1); /* ch为串str的第i个字符 */
     if(!StrCompare(ch,c2)) /* ch='(' */
       ++k; /* 左括号个数+1 */
     else if(!StrCompare(ch,c3)) /* ch=')' */
       --k; /* 左括号个数-1 */
   }
   if(i<=n) /* 串str中存在','，它是第i-1个字符 */
   {
     SubString(hstr,str,1,i-2); /* hstr返回串str','前的字符 */
     SubString(str,str,i,n-i+1); /* str返回串str','后的字符 */
   }
   else /* 串str中不存在',' */
   {
     StrCopy(hstr,str); /* 串hstr就是串str */
     ClearString(str); /* ','后面是空串 */
   }
 }
 
 void InitGList(GList *L)
 { /* 创建空的广义表L */
   *L=NULL;
 }

 void CreateGList(GList *L,SString S) /* 算法5.7 */
 { /* 采用头尾链表存储结构，由广义表的书写形式串S创建广义表L。设emp="()" */
   SString sub,hsub,emp;
   GList p,q;
   StrAssign(emp,"()"); /* 空串emp="()" */
   if(!StrCompare(S,emp)) /* S="()" */
     *L=NULL; /* 创建空表 */
   else /* S不是空串 */
   {
     *L=(GList)malloc(sizeof(GLNode));
     if(!*L) /* 建表结点 */
       exit(OVERFLOW);
     if(StrLength(S)==1) /* S为单原子，只会出现在递归调用中 */
     {
       (*L)->tag=ATOM;
       (*L)->a.atom=S[1]; /* 创建单原子广义表 */
     }
     else /* S为表 */
     {
       (*L)->tag=LIST;
       p=*L;
       SubString(sub,S,2,StrLength(S)-2); /* 脱外层括号(去掉第1个字符和最后1个字符)给串sub */
       do
       { /* 重复建n个子表 */
         sever(sub,hsub); /* 从sub中分离出表头串hsub */
         CreateGList(&p->a.ptr.hp,hsub);
         q=p;
         if(!StrEmpty(sub)) /* 表尾不空 */
         {
           p=(GLNode *)malloc(sizeof(GLNode));
           if(!p)
             exit(OVERFLOW);
           p->tag=LIST;
           q->a.ptr.tp=p;
         }
       }while(!StrEmpty(sub));
       q->a.ptr.tp=NULL;
     }
   }
 }
 void DestroyGList(GList *L)
 { /* 销毁广义表L */
   GList q1,q2;
   if(*L)
   {
     if((*L)->tag==LIST) /* 删除表结点 */
     {
       q1=(*L)->a.ptr.hp; /* q1指向表头 */
       q2=(*L)->a.ptr.tp; /* q2指向表尾 */
       DestroyGList(&q1); /* 销毁表头 */
       DestroyGList(&q2); /* 销毁表尾 */
     }
     free(*L);
     *L=NULL;
   }
 }

 void CopyGList(GList *T,GList L)
 { /* 采用头尾链表存储结构，由广义表L复制得到广义表T。算法5.6 */
   if(!L) /* 复制空表 */
     *T=NULL;
   else
   {
     *T=(GList)malloc(sizeof(GLNode)); /* 建表结点 */
     if(!*T)
       exit(OVERFLOW);
     (*T)->tag=L->tag;
     if(L->tag==ATOM)
       (*T)->a.atom=L->a.atom; /* 复制单原子 */
     else
     {
       CopyGList(&((*T)->a.ptr.hp),L->a.ptr.hp); /* 递归复制子表 */
       CopyGList(&((*T)->a.ptr.tp),L->a.ptr.tp);
     }
   }
 }

 int GListLength(GList L)
 { /* 返回广义表的长度，即元素个数 */
   int len=0;
   while(L)
   {
     L=L->a.ptr.tp;
     len++;
   }
   return len;
 }

 int GListDepth(GList L)
 { /* 采用头尾链表存储结构，求广义表L的深度。算法5.5 */
   int max,dep;
   GList pp;
   if(!L)
     return 1; /* 空表深度为1 */
   if(L->tag==ATOM)
     return 0; /* 原子深度为0，只会出现在递归调用中 */
   for(max=0,pp=L;pp;pp=pp->a.ptr.tp)
   {
     dep=GListDepth(pp->a.ptr.hp); /* 递归求以pp->a.ptr.hp为头指针的子表深度 */
     if(dep>max)
       max=dep;
   }
   return max+1; /* 非空表的深度是各元素的深度的最大值加1 */
 }

 Status GListEmpty(GList L)
 { /* 判定广义表是否为空 */
   if(!L)
     return TRUE;
   else
     return FALSE;
 }

 GList GetHead(GList L)
 { /* 生成广义表L的表头元素，返回指向这个元素的指针 */
   GList h,p;
   if(!L) /* 空表无表头 */
     return NULL;
   p=L->a.ptr.hp; /* p指向L的表头元素 */
   CopyGList(&h,p); /* 将表头元素复制给h */
   return h;
 }

 GList GetTail(GList L)
 { /* 将广义表L的表尾生成为广义表，返回指向这个新广义表的指针 */
   GList t;
   if(!L) /* 空表无表尾 */
     return NULL;
   CopyGList(&t,L->a.ptr.tp); /* 将L的表尾拷给t */
   return t;
 }

 void InsertFirst_GL(GList *L,GList e)
 { /* 初始条件：广义表存在。操作结果：插入元素e(也可能是子表)作为广义表L的第1元素(表头) */
   GList p=(GList)malloc(sizeof(GLNode)); /* 生成新结点 */
   if(!p)
     exit(OVERFLOW);
   p->tag=LIST; /* 结点的类型是表 */
   p->a.ptr.hp=e; /* 表头指向e */
   p->a.ptr.tp=*L; /* 表尾指向原表L */
   *L=p; /* L指向新结点 */
 }

 void DeleteFirst_GL(GList *L,GList *e)
 { /* 初始条件：广义表L存在。操作结果：删除广义表L的第一元素，并用e返回其值 */
   GList p=*L; /* p指向第1个结点 */
   *e=(*L)->a.ptr.hp; /* e指向L的表头 */
   *L=(*L)->a.ptr.tp; /* L指向原L的表尾 */
   free(p); /* 释放第1个结点 */
 }

 void Traverse_GL(GList L,void(*v)(AtomType))
 { /* 利用递归算法遍历广义表L */
   if(L) /* L不空 */
     if(L->tag==ATOM) /* L为单原子 */
       v(L->a.atom);
     else /* L为广义表 */
     {
       Traverse_GL(L->a.ptr.hp,v); /* 递归遍历L的表头 */
       Traverse_GL(L->a.ptr.tp,v); /* 递归遍历L的表尾 */
     }
 }

 void MarkList(GList GL) /* 算法8.3改 */
 { /* 遍历非空广义表GL(GL!=NULL且GL->mark不定)，对表中所有未加标志的结点加标志 */
   GList q,p=GL,t=NULL; /* t指示p的母表 */
   Status finished=FALSE;
   if(GL==NULL)
     return;
   while(!finished)
   {
     while(p->mark!=1)
     {
       p->mark=1; /* MarkHead(p)的细化 */
       q=p->a.ptr.hp; /* q指向*p的表头 */
       if(q&&q->mark!=1)
         if(q->tag==0)
	   q->mark=1; /* ATOM，表头为原子结点 */
         else
         { /* 继续遍历子表 */
           p->a.ptr.hp=t;
           p->tag=ATOM;
           t=p;
           p=q;
         }
     } /* 完成对表头的标志 */
     q=p->a.ptr.tp; /* q指向*p的表尾 */
     if(q&&q->mark!=1)
     { /* 继续遍历表尾 */
       p->a.ptr.tp=t;
       t=p;
       p=q;
     }
     else /* BackTrack(finished)的细化 */
     {
       while(t&&t->tag==1) /* LIST，表结点，从表尾回溯 */
       {
         q=t;
         t=q->a.ptr.tp;
         q->a.ptr.tp=p;
         p=q;
       }
       if(!t)
         finished=TRUE; /* 结束 */
       else
       { /* 从表头回溯 */
         q=t;
         t=q->a.ptr.hp;
         q->a.ptr.hp=p;
         p=q;
         p->tag=LIST;
       } /* 继续遍历表尾 */
     }
   }
 }

 void Traverse_GL1(GList L,void(*v)(GList))
 { /* 利用递归算法遍历广义表L，由bo5-5.c改 */
   if(L) /* L不空 */
     if(L->tag==ATOM) /* L为单原子 */
       v(L);
     else /* L为广义表 */
     {
       v(L);
       Traverse_GL1(L->a.ptr.hp,v);
       Traverse_GL1(L->a.ptr.tp,v);
     }
 }

 void visit(GList p)
 {
   if(p->tag==ATOM)
     printf("mark=%d %c\n",p->mark,p->a.atom);
   else
     printf("mark=%d list\n",p->mark);
 }

 int main()
 {
   char p[80];
   SString t;
   GList l;
   printf("请输入广义表l(书写形式：空表:(),单原子:a,其它:(a,(b),c)):\n");
   gets(p);
   StrAssign(t,p);
   CreateGList(&l,t); /* 在bo5-5.c中 */
   MarkList(l); /* 加标志 */
   Traverse_GL1(l,visit);
   
   return 0; 
 }