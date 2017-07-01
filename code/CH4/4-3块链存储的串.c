char blank = '#';//全局变量，用于填补空余
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
 
 #define CHUNK_SIZE 4//由用户定义块的大小
 typedef struct Chunk
 {
 	char ch[CHUNK_SIZE];
 	struct Chunk *next;
 }Chunk;
 typedef struct
 {
 	Chunk *head, *tail;//串的头和尾指针
	int curlen;//串的当前长度 
 }LString;
 #define DestroyString ClearString//DestroyString()与ClearString()作用相同
 
 void InitString(LString *T);
 Status StrAssign(LString *T, char *chars);
 Status ToChars(LString T, char* *chars);
 Status StrCopy(LString *T, LString S);
 Status StrEmpty(LString S);
 int StrCompare(LString S, LString T);
 int StrLength(LString S);
 void ClearString(LString *S);
 Status Concat(LString *T, LString S1, LString S2);
 Status SubString(LString *Sub, LString S, int pos, int len);
 int Index(LString S, LString T, int pos);
 Status StrInsert(LString *S, int pos, LString T);
 Status StrDelete(LString *S, int pos, int len);
 Status Replace(LString *S, LString T, LString V);//此函数与串的存储结构无关
 void StrPrint(LString T);
 
 int main()
 {
   char *s1="ABCDEFGHI",*s2="12345",*s3="",*s4="asd#tr",*s5="ABCD";
   Status k;
   int pos,len;
   LString t1,t2,t3,t4;
   InitString(&t1);
   InitString(&t2);
   printf("初始化串t1后，串t1空否？%d(1:空 0:否) 串长=%d\n",StrEmpty(t1),StrLength(t1));
   k=StrAssign(&t1,s3);
   if(k==ERROR)
     printf("出错\n"); /* 不能生成空串 */
   k=StrAssign(&t1,s4);
   if(k==ERROR)
     printf("出错\n"); /* 不能生成含有变量blank所代表的字符的串 */
   k=StrAssign(&t1,s1);
   if(k==OK)
   {
     printf("串t1为: ");
     StrPrint(t1);
   }
   else
     printf("出错\n");
   printf("串t1空否？%d(1:空 0:否) 串长=%d\n",StrEmpty(t1),StrLength(t1));
   StrAssign(&t2,s2);
   printf("串t2为: ");
   StrPrint(t2);
   StrCopy(&t3,t1);
   printf("由串t1拷贝得到串t3,串t3为: ");
   StrPrint(t3);
   InitString(&t4);
   StrAssign(&t4,s5);
   printf("串t4为: ");
   StrPrint(t4);
   Replace(&t3,t4,t2);
   printf("用t2取代串t3中的t4串后，串t3为: ");
   StrPrint(t3);
   ClearString(&t1);
   printf("清空串t1后，串t1空否？%d(1:空 0:否) 串长=%d\n",StrEmpty(t1),StrLength(t1));
   Concat(&t1,t2,t3);
   printf("串t1(=t2+t3)为: ");
   StrPrint(t1);
   pos=Index(t1,t3,1);
   printf("pos=%d\n",pos);
   printf("在串t1的第pos个字符之前插入串t2，请输入pos: ");
   scanf("%d",&pos);
   k=StrInsert(&t1,pos,t2);
   if(k)
   {
     printf("插入串t2后，串t1为: ");
     StrPrint(t1);
   }
   else
     printf("插入失败！\n");
   printf("求从t1的第pos个字符起,长度为len的子串t2,请输入pos,len: ");
   scanf("%d,%d",&pos,&len);
   SubString(&t2,t1,pos,len);
   printf("串t2为: ");
   StrPrint(t2);
   printf("StrCompare(t1,t2)=%d\n",StrCompare(t1,t2));
   printf("删除串t1中的子字符串：从第pos个字符起删除len个字符。请输入pos,len：");
   scanf("%d,%d",&pos,&len);
   k=StrDelete(&t1,pos,len);
   if(k)
   {
     printf("从第%d位置起删除%d个元素后串t1为:",pos,len);
     StrPrint(t1);
   }
   DestroyString(&t1); /* 销毁操作同清空 */
 	
 	
 	return 0;
 }
 
 void InitString(LString *T)
 {
 	//初始化(产生空串)字符串T。另加
	(*T).curlen = 0;
	(*T).head = (*T).tail = NULL; 
 }
 
 Status StrAssign(LString *T, char *chars)
 {
 	//生成一个其值等于chars的串T（要求chars中不包含填补空余的字符）。
	//成功返回OK，否则返回ERROR
	int i, j, k, m;
	Chunk *p, *q;
	i = strlen(chars);//i为串的长度
	if ( !i || strchr(chars, blank) )//串长为0或chars包含填补空余的字符
	{
		return ERROR; 
	}
	(*T).curlen = i;
	j = i / CHUNK_SIZE;//j为块链的结点数
	
	if (i % CHUNK_SIZE)
	{
		j++;
	}
	
	for (k=0; k<j; k++)
	{
		p = (Chunk*)malloc(sizeof(Chunk));//生成块结点
		if (!p)//生成块结点失败
		{
			return ERROR; 
		} 
		for (m=0; m<CHUNK_SIZE && *chars; m++)//给块结点的数据域赋值
		{
			*(p->ch+m) = *chars++; 
		} 
		
		if (k == 0)//第一个链块 
		{
			(*T).head = q = p;//头指针指向第一个链块 
		}
		else
		{
			q->next = p;
			q = p;
		} 
		
		if (!*chars)//最后一个链块
		{
			(*T).tail = q;
			q->next = NULL;
			for (; m < CHUNK_SIZE; m++)//用填补空余的字符填满链表 
			{
				*(q->ch+m) = blank; 
				
			}//for 
		}//if
	}//for 
	
	return OK;
 }
 
 Status ToChars(LString T, char* *chars)
 {
 	//将串T的内容转换为字符串，chars为头指针。成功返回OK，否则返回ERROR。另加
	Chunk *p = T.head;//p指向第1个块结点
	int i;
	char *q;
	*chars = (char*)malloc((T.curlen+1) * sizeof(char));
	
	if ( !chars || !T.curlen)//生成字符串数组失败或串T长为0
	{
		return ERROR;
	}  
	q = *chars;//q指向chars的第1个字符 
	while (p)//块链没结束
	{
		for (i=0; i<CHUNK_SIZE; i++)
		{
			if (p->ch[i] != blank)//当前字符不是填补空余的字符
			{
				*q++ = (p->ch[i]);//赋给q所指字符空间 
			}//if 		 
		}//for
		p = p->next; 
	} 
	(*chars)[T.curlen] = 0;//串结束符
	
	return OK; 	
 } 
 
 Status StrCopy(LString *T, LString S)
 {
 	//初始条件：串S存在
	//操作结果：由串S复制得串T,去掉填补空余的字符。成功返回OK，否则返回ERROR
	char *c;
	Status i;
	
	if (!ToChars(S, &c))//c为串S的内容
	{
		return ERROR; 
	} 
	i = StrAssign(T, c);//将串S的内容赋给T
	free(c);//释放c的空间
	
	return i; 
 }
 
 Status StrEmpty(LString S)
 {
 	//初始条件：串S存在。操作结果：若S为空串，则返回TRUE，否则返回FALSE
	if (S.curlen)//非空
	{
		return FALSE; 
	} 
	else
	{
		return TRUE; 
	} 	
 }
 
 int StrCompare(LString S, LString T)
 {
 	//若S>T,则返回值>0;若S=T,则返回值=0；若S<T,则返回值<0
	char *s, *t;
	Status i;
	
	if (!ToChars(S, &s))//s为串S的内容
	{
		return ERROR; 
	} 
	if (!ToChars(T, &t))//t为串T的内容
	{
		return ERROR; 
	} 
	i = strcmp(s, t);//利用C的库函数
	free(s);//释放s，t的空间
	free(t);
	
	return i; 
 } 
 
 int StrLength(LString S)
 {
 	//返回S的元素个数，称为串的长度
	return S.curlen; 
 }
 
 void ClearString(LString *S)
 {
 	//初始条件 ：串S存在。操作结果：将S清为空串
 	Chunk *p, *q;
	
	p = (*S).head;
	while (p)
	{
		q = p->next;
		free(p);
		p = q;
	} 
	(*S).head = (*S).tail = NULL;
	(*S).curlen = 0;	  
 }
 
 Status Concat(LString *T, LString S1, LString S2)
 {
 	//用T返回由S1和S2连接而成的新串（中间可能有填补空余的字符）
	LString a1, a2;
	Status i, j;
	
	InitString(&a1);
	InitString(&a2);
	
	i = StrCopy(&a1, S1);
	j = StrCopy(&a2, S2);
	
	if (!i || !j)//至少有1个串拷贝不成功
	{
		return ERROR; 
	} 
	(*T).curlen = S1.curlen + S2.curlen;//生成串T
	(*T).head = a1.head;
	a1.tail->next = a2.head;//a1,a2两串首尾相连
	(*T).tail = a2.tail;
	
	return OK; 	
 }
 
 Status SubString(LString *Sub, LString S, int pos, int len)
 {
 	//用Sub返回串S的第pos个字符起长度为len的子串
	//其中，1<=pos<=StrLength(S)且0<=len<=StrLength(S)-pos+1
	char *b, *c;
	Status i;
	
	if (pos<1 || pos>S.curlen || len<0 || len>S.curlen-pos+1)//pos或len值不合法
	{
		return ERROR; 
	} 
	if (!ToChars(S, &c))//c为串S的内容
	{
		return ERROR; 
	} 
	b = c+pos-1;//b指向串c中串Sub内容的首地址
	b[len] = 0;//Sub结束处赋0（字符串结束符）
	i = StrAssign(Sub, b);//将串b的内容赋给Sub
	free(c); 
	
	return i;	 
 }
 
 int Index(LString S, LString T, int pos)
 {
 	//T为非空字符串。若主串S中 第pos个字符之后存在与T相等的子串
	//则返回第一个这样的子串在S中的位置，否则返回0
	int i, n, m;
	LString sub;
	
	if (pos>=1 && pos<=StrLength(S))//pos满足条件
	{
		n = StrLength(S);//主串长度
		m = StrLength(T);//串T长度
		i = pos;
		while (i <= n-m+1)
		{
			SubString(&sub, S, i, m);//sub为从S的第i个字符起，长度为m的子串
			if (StrCompare(sub, T))//sub不等于T
			{
				++i; 
			} 
			else
			{
				return i; 
			}//else 
		}//while 
	} //if
	
	return 0;
 } 
 
 Status StrInsert(LString *S, int pos, LString T)
 {
 	//1<=pos<=StrLength(S)+1。在串S的第pos个字符之前插入串T
	char *b, *c;
	int i, j;
	Status k;
	
	if ( pos<1 || (pos>(*S).curlen+1) )//pos的值超出范围
	{
		return ERROR;
	}
	if (!ToChars(*S, &c))//c为串S的内容
	{
		return ERROR; 
	} 
	if (!ToChars(T, &b))//b为串T的内容
	{
		return ERROR;
	} 
	j = (int)strlen(c);//j为串S的最初长度
	c = (char*)realloc(c, (j+strlen(b)+1)*sizeof(char));//增加c的存储空间
	
	for (i=j; i>=pos-1; i--)
	{
		c[i+strlen(b)] = c[i];//为插入串b腾出空间 
	} 
	
	for (i=0; i<(int)strlen(b); i++)//在串c中插入串b
	{
		c[pos+i-1] = b[i]; 
	} 
	InitString(S);//释放S的原有存储空间
	k = StrAssign(S, c);//由c生成新的串S
	free(b);
	free(c);
	
	return k;  
 }
 
 Status StrDelete(LString *S, int pos, int len)
 {
 	//从串S中删除第pos个字符起长度为len的子串
	char *c;
	int i;
	Status k;
	
	if (pos<1 || pos>(*S).curlen-len+1 || len<0)//pos,len的值超出范围
	{
		return ERROR; 
	} 
	if (! ToChars(*S, &c))//c为串S的内容
	{
		return ERROR; 
	} 
	for (i=pos+len-1; i<=(int)strlen(c); i++)
	{
		c[i-len] = c[i];//c为删除后串S的内容 
	}
	InitString(S);//释放S的原有存储空间
	k = StrAssign(S, c);//由c生成新的串S
	free(c);
	
	return k; 
 }
 
 Status Replace(LString *S, LString T, LString V)//此函数与串的存储结构无关
 {
 	//初始条件:串S,T和V存在，T是非空串
	//操作结果：用V替换主串S中出现的所有与T相等的不重叠的子串
	int i = 1;//从串S的第一个字符起查找串T
	
	if (StrEmpty(T))//T是空串
	{
		return ERROR;
	} 
	do
	{
		i = Index(*S, T, i);//结果i为从上一个i之后找到的子串T的位置
		if (i)//串S中存在串T
		{
			StrDelete(S, i, StrLength(T));//删除该串T
			StrInsert(S, i, V);//在原串T的位置插入串V
			i += StrLength(V);//在插入的串V后面继续查找串T 
		} //if 	
	}while(i);
	
	return OK; 
 } 
 
 void StrPrint(LString T)
 {
 	//输出字符串T。令加
	int i = 0, j;
	Chunk *h;
	
	h = T.head;
	
	while (i < T.curlen)
	{
		for (j=0; j<CHUNK_SIZE; j++)
		{
			if (*(h->ch+j) != blank)//不是填补空余的字符 
			{
				printf("%c", *(h->ch+j));
				i++; 
			}//if
		}//for 
		h = h->next;
	} 
	
	printf("\n");
 }
 
 
 
  
  