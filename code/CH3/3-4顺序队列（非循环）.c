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
 typedef int QElemType;
 
 #define QUEUE_INIT_SIZE 10 //队列存储空间的初始分配量
 #define QUEUE_INCREMENT 2 //队列存储空间的分配增量
 
 typedef struct
 {
 	QElemType *base;//初始化的动态分配存储空间
	int front;//头指针，若队列不空，指向队列头元素
	int rear;//尾指针，若队列不空，指向队列尾元素的下一个位置
	int queuesize;//当前分配的存储容量(以sizeof(QElemType)为单位) 
 }SqQueue2;
 
 
 void InitQueue(SqQueue2 *Q);
 void DestroyQueue(SqQueue2 *Q);
 void ClearQueue(SqQueue2 *Q);
 Status QueueEmpty(SqQueue2 Q);
 Status GetHead(SqQueue2 Q, QElemType *e);
 int QueueLength(SqQueue2 Q);
 void EnQueue(SqQueue2 *Q, QElemType e);
 Status DeQueue(SqQueue2 *Q, QElemType *e);
 void QueueTraverse(SqQueue2 Q, void(*vi)(QElemType));
 void print(QElemType i);
 
 int main()
 {
   Status j;
   int i,n=11;
   QElemType d;
   SqQueue2 Q;
   InitQueue(&Q);
   printf("初始化队列后，队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   printf("队列长度为：%d\n",QueueLength(Q));
   printf("请输入%d个整型队列元素:\n",n);
   for(i=0;i<n;i++)
   {
     scanf("%d",&d);
     EnQueue(&Q,d);
   }
   printf("队列长度为：%d\n",QueueLength(Q));
   printf("现在队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   printf("现在队列中的元素为: \n");
   QueueTraverse(Q,print);
   DeQueue(&Q,&d);
   printf("删除队头元素%d\n",d);
   printf("队列中的元素为: \n");
   QueueTraverse(Q,print);
   j=GetHead(Q,&d);
   if(j)
     printf("队头元素为: %d\n",d);
   else
     printf("无队头元素(空队列)\n");
   ClearQueue(&Q);
   printf("清空队列后, 队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   j=GetHead(Q,&d);
   if(j)
     printf("队头元素为: %d\n",d);
   else
     printf("无队头元素(空队列)\n");
   DestroyQueue(&Q);
   
   return 0;
 }
 
 
 
 
 
 
 
 void InitQueue(SqQueue2 *Q)
 {
 	//构造一个空队列Q
	(*Q).base = (QElemType *)malloc(QUEUE_INIT_SIZE * sizeof(QElemType));
	if (! (*Q).base)//存储分配失败
	{
		exit(ERROR); 
	}
	(*Q).front = (*Q).rear = 0;
	(*Q).queuesize = QUEUE_INIT_SIZE;
 } 
 
 void DestroyQueue(SqQueue2 *Q)
 {
 	//销毁队列Q,Q不再存在 
 	if ((*Q).base)
 	{
 		free((*Q).base);
    }
    (*Q).base = NULL;
    (*Q).front = (*Q).rear = (*Q).queuesize = 0;
 }
 
 void ClearQueue(SqQueue2 *Q)
 {
 	//将Q清为空队列
	(*Q).front = (*Q).rear = 0; 
 }
 
 Status QueueEmpty(SqQueue2 Q)
 {
 	//若队列Q为空队列，则返回TRUE；否则返回 FALSE
 	if (Q.front == Q.rear)//队列空的标志 
 	{
 		return TRUE; 
    } 
    else
    {
    	return FALSE;
    }
 }
 
 Status GetHead(SqQueue2 Q, QElemType *e)
 {
 	//若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
	if (Q.front == Q.rear)//队列
	{
		return ERROR; 
	} 
	*e = Q.base[Q.front];
	
	return OK;	
 }
 
 int QueueLength(SqQueue2 Q)
 {
 	//返回Q的元素个数，即队列的长度
	return (Q.rear-Q.front); 
 }
 
 void EnQueue(SqQueue2 *Q, QElemType e)
 {
 	//插入元素e为Q的新的队尾元素
	if ((*Q).rear == (*Q).queuesize)
	{
		//队列满，增加存储单元
		(*Q).base = (QElemType *)realloc((*Q).base, 
		                                 ( (*Q).queuesize + QUEUE_INCREMENT) * sizeof(QElemType) );
		if (! (*Q).base)//增加单元失败 
		{
			exit(ERROR);
		}//if								  
	} //if
	
	(*Q).base[(*Q).rear++] = e;
 }
 
 Status DeQueue(SqQueue2 *Q, QElemType *e)
 {
 	//若队列不空，则删除Q的队头元素，用e返回其值，并返回OK；否则返回ERROR
	if ((*Q).front == (*Q).rear)//空队列
	{
		return ERROR; 
	} 
	*e = (*Q).base[(*Q).front++];
	
	return OK;
 }
 
 void QueueTraverse(SqQueue2 Q, void(*vi)(QElemType))
 {
 	//从队头到队尾依次对队列Q中每个元素调用函数vi()
	int i = Q.front;
	while (i != Q.rear)
	{
		vi(Q.base[i++]);
	} 
	printf("\n");	
 }
 
 void print(QElemType i)
 {
 	printf("%d ", i);
 }
 
 
 