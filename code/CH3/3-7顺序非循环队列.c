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
 #define QUEUE_INCREMENT 2  //队列存储空间的分配增量
 
 typedef struct 
 {
 	QElemType *base;//初始化的动态分配存储空间
	int rear;//尾指针，若队列不空，指向队列尾元素的下一个位置 
	int queuesize;//当前分配的存储容量（以sizeof(QElemType)为单位） 
 }SqQueue1;
 
 void InitQueue(SqQueue1 *Q);
 void DestroyQueue(SqQueue1 *Q);
 void ClearQueue(SqQueue1 *Q);
 Status QueueEmpty(SqQueue1 Q);
 int QueueLength(SqQueue1 Q);
 Status GetHead(SqQueue1 Q, QElemType *e);
 void EnQueue(SqQueue1 *Q, QElemType e);
 Status DeQueue(SqQueue1 *Q, QElemType *e);
 void QueueTraverse(SqQueue1 Q, void(*vi)(QElemType));
 void print(QElemType i);
 
 int main()
 {
   Status j;
   int i,k=5;
   QElemType d;
   SqQueue1 Q;
   InitQueue(&Q);
   printf("初始化队列后，队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   for(i=1;i<=k;i++)
     EnQueue(&Q,i); /* 依次入队k个元素 */
   printf("依次入队%d个元素后，队列中的元素为: ",k);
   QueueTraverse(Q,print);
   printf("队列长度为%d，队列空否？%u(1:空 0:否)\n",QueueLength(Q),QueueEmpty(Q));
   DeQueue(&Q,&d);
   printf("出队一个元素，其值是%d\n",d);
   j=GetHead(Q,&d);
   if(j)
     printf("现在队头元素是%d\n",d);
   ClearQueue(&Q);
   printf("清空队列后, 队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   DestroyQueue(&Q);
 	
 	return 0;
 }
 
 void InitQueue(SqQueue1 *Q)
 {
 	//构造一个空队列Q 
 	(*Q).base = (QElemType*)malloc(QUEUE_INIT_SIZE * sizeof(QElemType));
 	if (! (*Q).base)
 	{
 		exit(ERROR);//存储分配失败 
    }
    (*Q).rear = 0;//空队列，尾指针为0
	(*Q).queuesize = QUEUE_INIT_SIZE;//初始存储容量 
 } 
 
 void DestroyQueue(SqQueue1 *Q)
 {
 	//销毁队列Q,Q不存在
	free((*Q).base);//释放存储空间
	(*Q).base = NULL;
	(*Q).rear = (*Q).queuesize = 0; 
 } 
 
 void ClearQueue(SqQueue1 *Q)
 {
 	//将Q清为空队列
	(*Q).rear = 0; 
 }
 
 Status QueueEmpty(SqQueue1 Q)
 {
 	//若队列Q为空队列，则返回TRUE，否则返回FALSE
	if (Q.rear == 0)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}	
 }
 
 int QueueLength(SqQueue1 Q)
 {
 	//返回Q的元素个数，即队列的长度
	return Q.rear; 
 }
 
 Status GetHead(SqQueue1 Q, QElemType *e)
 {
 	//若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
	if (Q.rear)
	{
		*e = *Q.base;
		return OK; 
	} 
	else
	{
		return ERROR;
	}
 }
 
 void EnQueue(SqQueue1 *Q, QElemType e)
 {
 	//插入元素e为Q的新的队尾元素
	if ((*Q).rear == (*Q).queuesize)//当前存储空间已满
	{
		//增加分配
		(*Q).base = (QElemType*)realloc((*Q).base, ( (*Q).queuesize + QUEUE_INCREMENT) * sizeof(QElemType));
		if (! (*Q).base)//分配失败
		{
			exit(ERROR); 
		} 
		(*Q).queuesize += QUEUE_INCREMENT;//增加存储容量 
	} 
	(*Q).base[(*Q).rear++] = e;//入队新元素，队尾指针+1 	 
 }
 
 Status DeQueue(SqQueue1 *Q, QElemType *e)
 {
 	//若队列不空，则删除Q的队头元素用e返回其值，并返回OK；否则返回ERROR
	int i;
	
	if ( (*Q).rear )//队列不空
	{
		*e = *(*Q).base;
		for (i=1; i<(*Q).rear; i++) 
		{
			(*Q).base[i-1] = (*Q).base[i];//依次前移队列元素 
		}
		(*Q).rear--;//尾指针前移
		
		return OK; 
	} 
	else
	{
		return ERROR;
	}
 } 
 
 void QueueTraverse(SqQueue1 Q, void(*vi)(QElemType))
 {
 	//从队头到队尾依次对队列 Q中每个元素 调用函数vi()
 	int i;
 	
 	for (i=0; i<Q.rear; i++)
 	{
 		vi(Q.base[i]);	
    }
    printf("\n");
 }
 
 void print(QElemType i)
 {
 	printf("%d ", i);
 }
 
 