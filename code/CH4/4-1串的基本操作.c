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
 
 #define MAX_STR_LEN 40 //可以在255(1个字节)以内定义最大串长
 typedef char SString[MAX_STR_LEN+1];//0号单元存放串的长度
  
 #define DestroyString ClearString //DestroyString()与ClearString()作用相同
 
Status StrAssign(SString T, char *chars);
void StrCopy(SString T, SString S);
Status StrEmpty(SString S);
int StrCompare(SString S, SString T);
int StrLength(SString S);
void ClearString(SString S);
Status Concat(SString T, SString S1, SString S2);
Status SubString(SString Sub, SString S, int pos, int len);
int Index(SString S, SString T, int pos);
Status StrInsert(SString S, int pos, SString T);
Status StrDelete(SString S, int pos, int len);
Status Replace(SString S, SString T, SString V);//此函数与串的存储解构无关
void StrPrint(SString T);
 
 int main()
 {
   int i,j;
   Status k;
   char s,c[MAX_STR_LEN+1];
   SString t,s1,s2;
   printf("请输入串s1: ");
   gets(c);
   k=StrAssign(s1,c);
   if(!k)
   {
     printf("串长超过MAX_STR_LEN(=%d)\n",MAX_STR_LEN);
     exit(0);
   }
   printf("串长为%d 串空否？%d(1:是 0:否)\n",StrLength(s1),StrEmpty(s1));
   StrCopy(s2,s1);
   printf("拷贝s1生成的串为: ");
   StrPrint(s2);
   printf("请输入串s2: ");
   gets(c);
   k=StrAssign(s2,c);
   if(!k)
   {
     printf("串长超过MAX_STR_LEN(%d)\n",MAX_STR_LEN);
     exit(0);
   }
   i=StrCompare(s1,s2);
   if(i<0)
     s='<';
   else if(i==0)
     s='=';
   else
     s='>';
   printf("串s1%c串s2\n",s);
   k=Concat(t,s1,s2);
   printf("串s1联接串s2得到的串t为: ");
   StrPrint(t);
   if(k==FALSE)
     printf("串t有截断\n");
   ClearString(s1);
   printf("清为空串后,串s1为: ");
   StrPrint(s1);
   printf("串长为%d 串空否？%d(1:是 0:否)\n",StrLength(s1),StrEmpty(s1));
   printf("求串t的子串,请输入子串的起始位置,子串长度: ");
   scanf("%d,%d",&i,&j);
   k=SubString(s2,t,i,j);
   if(k)
   {
     printf("子串s2为: ");
     StrPrint(s2);
   }
   printf("从串t的第pos个字符起,删除len个字符，请输入pos,len: ");
   scanf("%d,%d",&i,&j);
   StrDelete(t,i,j);
   printf("删除后的串t为: ");
   StrPrint(t);
   i=StrLength(s2)/2;
   StrInsert(s2,i,t);
   printf("在串s2的第%d个字符之前插入串t后,串s2为:\n",i);
   StrPrint(s2);
   i=Index(s2,t,1);
   printf("s2的第%d个字母起和t第一次匹配\n",i);
   SubString(t,s2,1,1);
   printf("串t为：");
   StrPrint(t);
   Concat(s1,t,t);
   printf("串s1为：");
   StrPrint(s1);
   k=Replace(s2,t,s1);
   if(k) /* 替换成功 */
   {
     printf("用串s1取代串s2中和串t相同的不重叠的串后,串s2为: ");
     StrPrint(s2);
   }
   DestroyString(s2); /* 销毁操作同清空 */
   
 	return 0; 	
 }
 
 
 Status StrAssign(SString T, char *chars)
 {
 	//生成一个其值等于chars的串T
	int i;
	
	if (strlen(chars) > MAX_STR_LEN)
	{
		return ERROR; 
	} 
	else
	{
		T[0] = strlen(chars);
		for (i=1; i<=T[0]; i++)
		{
			T[i] = *(chars+i-1);
		}
		return OK;
	}//else
 } 
 
 void StrCopy(SString T, SString S)
 {
 	//由串S复制得串T
	int i;
	
	for (i=0; i<=S[0]; i++)
	{
		T[i] = S[i];
	} 
 } 
 
 Status StrEmpty(SString S)
 {
 	//若S为空串，则返回TRUE，否则返回FALSE
	if (S[0] == 0)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}	
 }
 
 int StrCompare(SString S, SString T)
 {
 	//初始条件：串S和T存在。操作结果：若S>T,则返回值>0;
	//若S=T,则返回值=0；若S<T,则返回值<0
	int i;
	
	for (i=1; i<=S[0] && i<=T[0]; ++i)
	{
		if (S[i] != T[i])
		{
			return S[i] - T[i];
		}
	} 
	
	return S[0] - T[0];
 }
 
 int StrLength(SString S)
 {
 	//返回串S的元素个数
	return S[0]; 
 }
 
 void ClearString(SString S)
 {
 	//初始条件：串S存在。操作结果：将S清为空串
	S[0] = 0;//令串长为零 
 }
 
 Status Concat(SString T, SString S1, SString S2)
 {
 	//用 T返回S1和S2联接而成的新串。若未截断，则返回TRUE，否则FALSE
	int i;
	
	if (S1[0]+S2[0] <= MAX_STR_LEN)
	{
		//未截断 
		for (i=1; i<=S1[0]; i++)
		{
			T[i] = S1[i];
		} 
		for (i=1; i<=S2[0]; i++)
		{
			T[S1[0]+i] = S2[i];
		}
		T[0] = S1[0] +S2[0];
		
		return TRUE;	
	}//if
	else
	{
		//截断S2
		for (i=1; i<=S1[0]; i++)
		{
			T[i] = S1[i];
		} 
		for (i=1; i <= MAX_STR_LEN-S1[0]; i++)
		{
			T[S1[0]+i] = S2[i];
		}
		T[0] = MAX_STR_LEN;
		
		return FALSE;
	} 
 } 
 
 Status SubString(SString Sub, SString S, int pos, int len)
 {
 	//用Sub返回串S的第pos个字符起长度为len的子串。
	int i;
	
	if ( pos<1 || pos>S[0] || len<0 || len>(S[0]-pos+1) )
	{
		return ERROR;
	} 
	for (i=1; i<=len; i++)
	{
		Sub[i] = S[pos+i-1];
	}
	Sub[0] = len;
	
	return OK;
 }
 
 int Index(SString S, SString T, int pos)
 {
 	//返回子串T在主串S中第pos个字符之后的位置。若不存在，则函数值为0
	//其中，T非空，1<=pos<=StrLength(S)。
	int i, j;
	
	if ( 1<=pos && pos<=S[0] )
	{
		i = pos;
		j = 1;
		
		while ( i<=S[0] && j<=T[0] )
		{
			if (S[i] == T[j])//继续比较后继字符
			{
				++i;
				++j; 
			} 
			else//指针后退重新开始匹配 
			{
				i = i-j+2;
				j = 1; 
				
			}
		}//while
		if (j > T[0])
		{
			return i - T[0];
		}
		else
		{
			return 0;
		}
	}//if
	else
	{
		return 0;
	}//else 
 }
 
 Status StrInsert(SString S, int pos, SString T)
 {
 	//初始条件：串S和T存在，1<=pos<=StrLength(S)+1
	//操作结果：在串S的第pos个字符之前插入串T。完全插入返回TRUE，
	//部分插入返回FALSE
	int i;
	
	if (pos<1 || pos>(S[0]+1))
	{
		return ERROR;
	} 
	if (S[0]+T[0] <= MAX_STR_LEN)
	{
		//完全插入 
		for (i=S[0]; i>=pos; i--)
		{
			S[i+T[0]] = S[i];
		}
		for (i=pos; i<pos+T[0]; i++)
		{
			S[i] = T[i-pos+1];
		}
		S[0] += T[0];
		
		return TRUE;
	}//if
	else
	{
		//部分插入
		for (i=MAX_STR_LEN; i>=pos+T[0]; i--)
		{
			S[i] = S[i-T[0]];
		} 
		for(i=pos; i<pos+T[0] && i<=MAX_STR_LEN; i++)
		{
			S[i] = T[i-pos+1];
		}
		S[0] = MAX_STR_LEN;
		
		return FALSE;
	}//else
	
 }
 
 Status StrDelete(SString S, int pos, int len)
 {
 	//初始条件：串S存在，1<=pos<=StrLength(S)-len+1
	//操作结果：从串S中删除第pos个字符起长度为len的子串
	int i;
	
	if (pos<1 || pos>S[0]-len+1 || len<0)
	{
		return ERROR;
	} 
	for(i=pos+len; i<=S[0]; i++)
	{
		S[i-len] = S[i];
	}
	S[0] -= len;
	
	return OK;
 }
 
 Status Replace(SString S, SString T, SString V)//此函数与串的存储解构无关
 {
 	//初始条件：串S，T和V存在，T是非空串
	//操作结果：用V替换主串S中出现的所有与T相等的不重叠的子串
	int i = 1;//从串S的第一个字符起查找串T
	Status k;
	
	if ( StrEmpty(T) )//T是空串
	{
		return ERROR; 
	} 
	do
	{
		i = Index(S, T, i);//结果i为从上一个i之后找到的子串T的位置
		if (i)//串S中存在串T
		{
			StrDelete(S, i, StrLength(T));//删除该串T
			k = StrInsert(S, i, V);//在原串T的位置插入串V
			if (!k)//不能完全插入
			{
				return ERROR; 
			} 
			i += StrLength(V);//在插入的串V后面继续查找串T 
		}//if 
	}while (i);
	
	return OK;
 } 
 
 void StrPrint(SString T)
 {
 	//输出字符串T。另加
	int i;
	for (i=1; i<=T[0]; i++)
	{
		printf("%c", T[i]);
	} 
	printf("\n");
 }
 
 
 
 
 
 
 
 
 