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
 typedef char AtomType;//定义原子类型为字符型 
 
 typedef enum{ATOM, LIST}ElemTag;//ATOM==0:原子，LIST==1：子表
 typedef struct GLNode1
 {
 	ElemTag tag;//公共部分，用于区分原子结点和表结点
    union//原子结点和表结点的联合部分
	{
		AtomType atom;//原子结点的值域
		struct GLNode1 *hp;//表结点的表头指针 	
	}a;  
	struct GLNode1 *tp;//相当于线性链表的next，指向下一个元素结点 
 }*GList1, GLNode1;//广义表类型GList是一种扩展的线性链表 
 
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
void sever(SString str, SString hstr);//SString是数组，不需引用类型
void InitGList(GList1 *L);
void CreateGList(GList1 *L, SString S);
void DestroyGList(GList1 *L);
void CopyGList(GList1 *T, GList1 L);
int GListLength(GList1 L);
int GListDepth(GList1 L);
Status GListEmpty(GList1 L);
GList1 GetHead(GList1 L);
GList1 GetTail(GList1 L);
void InsertFirst_GL(GList1 *L, GList1 e);
void DeleteFirst_GL(GList1 *L, GList1 *e);
void Traverse_GL(GList1 L, void(*v)(AtomType));
void visit(AtomType e);
 
 
 int main()
 {
   char p[80];
   GList1 l,m;
   SString t;
   InitGList(&l); /* 建立空的广义表l */
   printf("空广义表l的深度=%d l是否空？%d(1:是 0:否)\n",GListDepth(l),GListEmpty(l));
   printf("请输入广义表l(书写形式：空表:(),单原子:(a),其它:(a,(b),c)):\n");
   gets(p);
   StrAssign(t,p);
   CreateGList(&l,t);
   printf("广义表l的长度=%d\n",GListLength(l));
   printf("广义表l的深度=%d l是否空？%d(1:是 0:否)\n",GListDepth(l),GListEmpty(l));
   printf("遍历广义表l：\n");
   Traverse_GL(l,visit);
   printf("\n复制广义表m=l\n");
   CopyGList(&m,l);
   printf("广义表m的长度=%d\n",GListLength(m));
   printf("广义表m的深度=%d\n",GListDepth(m));
   printf("遍历广义表m：\n");
   Traverse_GL(m,visit);
   DestroyGList(&m);
   m=GetHead(l);
   printf("\nm是l的表头元素，遍历m：\n");
   Traverse_GL(m,visit);
   DestroyGList(&m);
   m=GetTail(l);
   printf("\nm是由l的表尾形成的广义表，遍历广义表m：\n");
   Traverse_GL(m,visit);
   InsertFirst_GL(&m,l);
   printf("\n插入广义表l为m的表头，遍历广义表m：\n");
   Traverse_GL(m,visit);
   DeleteFirst_GL(&m,&l);
   printf("\n删除m的表头，遍历广义表m：\n");
   Traverse_GL(m,visit);
   printf("\n");
   DestroyGList(&m);
   
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
 
 void sever(SString str, SString hstr)//SString是数组，不需引用类型
 {
 	//将非空串str分割成两部分：hstr为第一个‘，’之前的子串，str为之后的子串
	int n, k, i;//k记尚未配对的左括号个数
	SString ch, c1, c2, c3;
	
	n = StrLength(str);//n为串str的长度
	StrAssign(c1, ",");//c1=','
	StrAssign(c2, "(");//c2='('
	StrAssign(c3, ")");//c3=')'
	SubString(ch, str, 1, 1);//ch为串str的第1个字符
	
	for (i=1, k=0; i<=n && StrCompare(ch, c1) || k!=0; ++i)//i小于串长且ch不是‘，’
	{
		//搜索最外层的第一个逗号
		SubString(ch, str, i, 1);//ch为串str的第i个字符
		if (! StrCompare(ch, c2))//ch='('
		{
			++k;//左括号个数+1 
		}
		else if (! StrCompare(ch, c3))//ch=')'
		{
			--k;//左括号个数-1 
		}	 
	} 
	
	if (i<=n)//串str中存在‘，’,它是第i-1个字符
	{
		SubString(hstr, str, 1, i-2);//hstr返回串str‘，’前的字符
		SubString(str, str, i, n-i+1);//str返回串str','后的字符 
	} 
	else//串str中不存在‘，’ 
	{
		StrCopy(hstr, str);//串hstr就是串str
		ClearString(str);//','后面是空串 	
	}//else	 
 } 
 
 
 void InitGList(GList1 *L)
 {
 	//创建空的广义表L
	*L = NULL; 
 } 
 
 void CreateGList(GList1 *L, SString S)
 {
 	//采用扩展线性链表存储结构，由广义表的书写形式串S创建广义表L。设emp=“（）”
	SString emp, sub, hsub;
	GList1 p;
	
	StrAssign(emp, "()");//设emp="()"
	*L = (GList1)malloc(sizeof(GLNode1));
	if (! *L)//建表结点不成功
	{
		exit(OVERFLOW); 
	} 
	if (! StrCompare(S, emp))//创建空表
	{
		(*L)->tag = LIST;
		(*L)->a.hp = (*L)->tp = NULL;
	} 
	else if (StrLength(S) == 1)//创建单原子广义表 
	{
		(*L)->tag = ATOM;
		(*L)->a.atom = S[1];
		(*L)->tp = NULL; 
	}
	else//创建一般表 
	{
		(*L)->tag = LIST;
		(*L)->tp = NULL;
		SubString(sub, S, 2, StrLength(S)-2);//脱外层括号 (去掉第1个字符和最后1个字符)给串sub 
		sever(sub, hsub);//从sub中分离出表头串hsub
		CreateGList(&(*L)->a.hp, hsub); 
		p = (*L)->a.hp;
		while (! StrEmpty(sub))//表尾不空，则重复建n个子表
		{
			sever(sub, hsub);//从sub中分离出表头串hsub 
			CreateGList(&p->tp, hsub);
			p = p->tp; 
		}//while 	
	}//else	
 }
 
 void DestroyGList(GList1 *L)
 {
 	//初始条件：广义表L存在。操作结果：销毁广义表L
	GList1 ph, pt;
	
	if (*L) //L不为空表 
	{
		//由ph和pt接替L的两个指针
		if ((*L)->tag)//是子表
		{
			ph = (*L)->a.hp; 
		} 
		else//是原子 
		{
			ph = NULL; 	
		}
		pt = (*L)->tp;
		DestroyGList(&ph);//递归销毁表ph
		DestroyGList(&pt);//递归销毁表pt
		free(*L);//释放L所指结点 
		*L = NULL;//令L为空 
	} //if 
 }
 
 void CopyGList(GList1 *T, GList1 L)
 {
 	//初始条件：广义表L存在。操作结果：由广义表L复制得到广义表T
	*T = NULL;
	
	if (L)//L不空
	{
		*T = (GList1)malloc(sizeof(GLNode1));
		
		if (!*T)
		{
			exit(OVERFLOW);
		}  
		(*T)->tag = L->tag;//复制枚举变量
		if (L->tag == ATOM)//复制共用体部分
		{
			(*T)->a.atom = L->a.atom;//复制单原子 	
		} 
		else
		{
			CopyGList(&(*T)->a.hp, L->a.hp);//复制子表 
		} 
		if (L->tp == NULL)//到表尾
		{
			(*T)->tp = L->tp;
		} 
		else
		{
			CopyGList(&(*T)->tp, L->tp);//复制子表 
		}//else 
	}//if 	 
 } 
 
 int GListLength(GList1 L)
 {
 	//初始条件：广义表L存在。操作结果：求广义表L的长度，即元素个数
	int len = 0;
	GList1 p = L->a.hp;//p指向第1个元素
	
	while (p)
	{
		len++;
		p = p->tp;
	} 
	
	return len;
 }
 
 int GListDepth(GList1 L)
 {
 	//初始条件：广义表L存在。操作结果：求广义表L的深度
	int max, dep;
	GList1 pp;
	
	if (L==NULL || L->tag==LIST && !L->a.hp)
	{
		return 1;//空表深度为1 
	}
	else if (L->tag == ATOM)
	{
		return 0;//单原子表深度为0，只会出现在递归调用中 
	} 
	else//求一般表的深度 
	{
		for (max=0, pp=L->a.hp; pp; pp=pp->tp)
		{
			dep = GListDepth(pp);//求以pp为头指针的子表深度
			if (dep > max)
			{
				max = dep;
			} 
		} //for	
	} 
	return max+1;//非空表的深度是各元素的深度的最大值加1	 
 }
 
 Status GListEmpty(GList1 L)
 {
 	//初始条件：广义表L存在。操作结果：判定广义表L是否为空
	if (!L || L->tag==LIST && !L->a.hp)
	{
		return OK;
	} 
	else
	{
		return ERROR;
	}
 } 
 
 GList1 GetHead(GList1 L)
 {
 	//生成广义表L的表头元素，返回指向这个元素的指针
	GList1 h, p;
	
	if (!L || L->tag==LIST && !L->a.hp)//空表无表头
	{
		return NULL; 
	} 
	p = L->a.hp->tp;//p指向L的表尾
	L->a.hp->tp = NULL;//截去L的表尾部分
	CopyGList(&h, L->a.hp);//将表头元素复制给h 
	L->a.hp->tp = p;//回复L的表尾（保持原L不变）
	
	return h; 
 }
 
 GList1 GetTail(GList1 L)
 {
 	//将广义表L的表尾生成为广义表,返回指向这个新广义表的指针
	GList1 t, p;
	
	if (!L || L->tag==LIST && !L->a.hp)//空表无表尾
	{
		return NULL; 
	} 
	p = L->a.hp;//p指向表头
	L->a.hp = p->tp;//在L中删去表头
	CopyGList(&t, L);//将L的表尾拷给t
	L->a.hp = p;//恢复L的表头（保持原L不变）
	
	return t; 
 }
 
 void InsertFirst_GL(GList1 *L, GList1 e)
 {
 	//初始条件：广义表存在。操作结果：插入元素e(也可能是子表)
	//作为广义表L的第1元素(表头)
	GList1 p = (*L)->a.hp;
	(*L)->a.hp = e;
	e->tp = p; 
 }
 
 void DeleteFirst_GL(GList1 *L, GList1 *e)
 {
 	//初始条件：广义表L存在。操作结果：删除广义表L的第一元素，并用e返回其值
	if (*L && (*L)->a.hp)
	{
		*e = (*L)->a.hp;
		(*L)->a.hp = (*e)->tp;
		(*e)->tp = NULL;
	} 
	else
	{
		*e = *L;
	}
 }
 
 void Traverse_GL(GList1 L, void(*v)(AtomType))
 {
 	//利用递归算法遍历广义表L
	GList1 hp;
	
	if (L)//L不空
	{
		if (L->tag == ATOM)//L为单原子
		{
			v(L->a.atom);
			hp = NULL; 
		} 
		else//L为子表 
		{
			hp = L->a.hp; 
		}
		
		Traverse_GL(hp, v);
		Traverse_GL(L->tp, v);
	} 
 }
 
 void visit(AtomType e)
 {
   printf("%c ", e);
 }
 
 
 
 
 
 