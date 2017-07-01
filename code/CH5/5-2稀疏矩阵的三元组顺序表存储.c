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
 
 #define MAX_SIZE 100 //非零元个数的最大值
 typedef struct
 {
 	int i, j;//行下标，列下标
    ElemType e;//非零元素值 
 }Triple;
 
 typedef struct 
 {
 	Triple data[MAX_SIZE+1];//非零元三元组表，data[0]未用
	int mu, nu, tu;//矩阵的行数，列数和非零元个数 
 }TSMatrix; 
 
 Status CreateSMatrix(TSMatrix *M);
 void DestroySMatrix(TSMatrix *M);
 void PrintSMatrix(TSMatrix M);
 void PrintSMatrix1(TSMatrix M);
 void CopySMatrix(TSMatrix M, TSMatrix *T);
 int comp(int c1, int c2);
 Status AddSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q);
 Status SubtSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q);
 void TransposeSMatrix(TSMatrix M, TSMatrix *T);
 Status MultSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q);

int main()
{
   TSMatrix A,B,C;
   printf("创建矩阵A: ");
   CreateSMatrix(&A);
   PrintSMatrix(A);
   printf("由矩阵A复制矩阵B:\n");
   CopySMatrix(A,&B);
   PrintSMatrix1(B);
   DestroySMatrix(&B);
   printf("销毁矩阵B后:\n");
   PrintSMatrix1(B);
   printf("创建矩阵B2:(与矩阵A的行、列数相同，行、列分别为%d,%d)\n",A.mu,A.nu);
   CreateSMatrix(&B);
   PrintSMatrix1(B);
   AddSMatrix(A,B,&C);
   printf("矩阵C1(A+B):\n");
   PrintSMatrix1(C);
   SubtSMatrix(A,B,&C);
   printf("矩阵C2(A-B):\n");
   PrintSMatrix1(C);
   TransposeSMatrix(A,&C);
   printf("矩阵C3(A的转置):\n");
   PrintSMatrix1(C);
   printf("创建矩阵A2: ");
   CreateSMatrix(&A);
   PrintSMatrix1(A);
   printf("创建矩阵B3:(行数应与矩阵A2的列数相同=%d)\n",A.nu);
   CreateSMatrix(&B);
   PrintSMatrix1(B);
   MultSMatrix(A,B,&C);
   printf("矩阵C5(A×B):\n");
   PrintSMatrix1(C);
   
   return 0;
}
 
 
 Status CreateSMatrix(TSMatrix *M)
 {
 	//创建稀疏矩阵M
	int i, m, n;
	ElemType e;
	Status k;
	
	printf("请输入矩阵的行数，列数，非零元素数：");
	scanf("%d,%d,%d", &(*M).mu, &(*M).nu, &(*M).tu);
	
	if ((*M).tu > MAX_SIZE)
	{
		return ERROR; 
	} 
	(*M).data[0].i = 0;//为以下比较顺序做准备
	
	for (i=1; i<=(*M).tu; i++)
	{
		do
		{
			printf("请按行序顺序输入第%d个非零元素所在的行(1-%d),列(1-%d),元素值：",
			       i, (*M).mu, (*M).nu);
            scanf("%d,%d,%d", &m, &n, &e);
            k = 0;
            if (m<1 || m>(*M).mu || n<1 || n>(*M).nu)//行或列超出范围
			{
				k = 1; 
			} 
			if (  m < (*M).data[i-1].i 
			   || m == (*M).data[i-1].i
			   && n <= (*M).data[i-1].j)//行或列的顺序有错
			{
				k = 1; 
			} //if
		}while(k);
		(*M).data[i].i = m;
		(*M).data[i].j = n;
		(*M).data[i].e = e;	
	}//for 
	
	return OK;
 }
 
 void DestroySMatrix(TSMatrix *M)
 {
 	//销毁稀疏矩阵M
	(*M).mu = (*M).nu = (*M).tu = 0; 
 }
 
 void PrintSMatrix(TSMatrix M)
 {
 	//输出稀疏矩阵M
	int i;
	
	printf("%d行%d列%d个非零元素。\n", M.mu, M.nu, M.tu);
	printf("行  列  元素值\n");
	
	for (i=1; i<=M.tu; i++)
	{
		printf("%2d%4d%8d\n", M.data[i].i, M.data[i].j, M.data[i].e); 
	}//for	
 }
 
 void PrintSMatrix1(TSMatrix M)
 {
 	//按矩阵形式输出M
	int i, j, k = 1;
	Triple *p = M.data;
	
	p++;//p指向第1个非零元素
	
	for (i=1; i<=M.mu; i++)
	{
		for (j=1; j<=M.nu; j++)
		{
			if (   k <= M.tu
			    && p->i == i
				&& p->j == j)//p指向非零元，且p所指元素为当前处理元素
			{
				printf("%3d", p->e);//输出p所指元素的值
				p++;//p指向下一个元素 
				k++;//计数器+1 
			} 
			else//p所指元素不是当前处理元素 
			{
				printf("%3d", 0);//输出0 
			}		
		}//for-in
		printf("\n"); 
	}//for-out 
 }
 
 void CopySMatrix(TSMatrix M, TSMatrix *T)
 {
 	//由稀疏矩阵M复制得到T
	(*T) = M; 
 }
 
 int comp(int c1, int c2)
 {
 	//AddSMatrix函数要用到，另加
	if (c1 < c2)
	{
		return -1;
	} 
	if (c1 == c2)
	{
		return 0;
	}
	
	return 1;
 }
 
 Status AddSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
 {
 	//求稀疏矩阵的和Q=M+N
	int m = 1,
	    n = 1,
		q = 0;
	
	if (M.mu!=N.mu || M.nu!=N.nu)//M,N两稀疏矩阵行或列数不同
	{
		return ERROR;
	} 
	(*Q).mu = M.mu;
	(*Q).nu = M.nu;
	
	while (m<=M.tu && n<=N.nu)//矩阵M和N的元素都没处理完
	{
		switch(comp(M.data[m].i, N.data[n].i)) 
		{
			case -1:
			{
				(*Q).data[++q] = M.data[m++];//将矩阵M的当前元素值赋给矩阵Q 
			}
			break;
			case 0:
			{
				switch(comp(M.data[m].j, N.data[n].j))//M,N当前元素的行相等，继续比较列 
				{
					case -1:
					{
						(*Q).data[++q] = M.data[m++];
					} 
					break;
					case 0:
					{
						(*Q).data[++q] = M.data[m++];//M,N矩阵当前非零元素的行列均相等
						(*Q).data[q].e += N.data[n++].e;//矩阵M,N的当前元素值求和并赋给矩阵Q
					    if (0 == (*Q).data[q].e)//元素值为0，不存入压缩矩阵
						{
							q--; 
						}//if 
					}
					break;
					case 1:
					{
						(*Q).data[++q] = N.data[++n];
					}
				}//switch
			}//case 
			break; 
			case 1:
			{
				(*Q).data[++q] = N.data[n++];//将矩阵N的当前元素值赋给矩阵 
			}
		}//switch 
	} //while
	
	while (m <= M.tu)//矩阵N的元素全部处理完毕
	{
		(*Q).data[++q] = M.data[m++];
	} 
	while (n <=N.tu)//矩阵M的元素全部处理完毕
	{
		(*Q).data[++q] = N.data[n++]; 
	}
	(*Q).tu = q;//矩阵Q的非零元素个数
	if (q > MAX_SIZE)//非零元素个数太多
	{
		return ERROR; 
	} 
	
	return OK;
 }
 
 Status SubtSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
 {
 	//求稀疏矩阵的差Q=M-N
	int i;
	
	for (i=1; i<N.tu; i++)
	{
		N.data[i].e *= -1;
	} 
	
	return AddSMatrix(M, N, Q);
 }
 
 void TransposeSMatrix(TSMatrix M, TSMatrix *T)
 {
 	//求稀疏矩阵M的转置矩阵T
	int p, q, col;
	
	(*T).mu = M.nu;
	(*T).nu = M.mu;
	(*T).tu = M.tu;
	
	if ((*T).tu)
	{
		q = 1;
		for (col=1; col<=M.nu; ++col)
		{
			for (p=1; p<=M.tu; ++p)
			{
				if (M.data[p].j == col)
				{
					(*T).data[q].i = M.data[p].j;
					(*T).data[q].j = M.data[p].i;
					(*T).data[q].e = M.data[p].e;
					++q;
				} //if
			}//for-in
		}//for-out
	} //if
 }
 
 Status MultSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
 {
 	//求稀疏矩阵的乘积Q=M*N
	int i, j;
	ElemType *Nc, *Tc;
	TSMatrix T;//临时矩阵
	
	if (M.nu != N.mu)
	{
		return ERROR;
	} 
	T.nu = M.mu;//临时矩阵T是Q的转置矩阵
	T.mu = N.nu;
	T.tu = 0;
	//Nc为矩阵N一列的临时数组（非压缩，[0]不用） 
	Nc = (ElemType*)malloc((N.mu+1) * sizeof(ElemType)); 
	//Tc为矩阵T一行的临时数组（非压缩，[0]不用）
	Tc = (ElemType*)malloc((M.nu+1) * sizeof(ElemType));
	
	if (!Nc || !Tc)//创建临时数组不成功 
	{
		exit(ERROR); 
	} 
	for (i=1; i<=N.nu; i++)//对于N的每一列 
	{
		for (j=1; j<=N.mu; j++)
		{
			Nc[j] = 0;//矩阵Nc的初值为0 
		} 
		for (j=1; j<=M.mu; j++)
		{
			Tc[j] = 0;//临时数组Tc的初值为0，[0]不用 
		} 
		for (j=1; j<=N.tu; j++)//对于N的每一个非零元素 
		{
			if (N.data[j].j == i)//属于第i列
			{
				Nc[N.data[j].i] = N.data[j].e;//根据其所在行将其元素值赋给相应的Nc 		
			} 	
		} 
		
		for (j=1; j<=M.tu; j++)//对于M的每一个值 
		{
			Tc[M.data[j].i] += (M.data[j].e * Nc[M.data[j].j]);//Tc中存N的第i列于M相乘的结果 
		} 
		
		for (j=1; j<=M.mu; j++)
		{
			if (Tc[j] != 0)
			{
				T.data[++T.tu].e = Tc[j];
				T.data[T.tu].i = i;
				T.data[T.tu].j = j; 
			}//if
		}//for
		
		if (T.tu > MAX_SIZE) //非零元素个数太多 
		{
			return ERROR; 
		}		
	} 
	TransposeSMatrix(T, Q);//将T的转置赋给Q
	DestroySMatrix(&T);//销毁临时矩阵T
	free(Tc);//释放动态数组Tc和Nc
	free(Nc);
	return OK;
 }
 
 
 
 
  
  