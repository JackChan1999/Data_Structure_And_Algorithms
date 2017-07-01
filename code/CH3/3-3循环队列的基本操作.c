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
 
 #define MAX_QSIZE 5 //最大队列长度+1
 typedef struct
 {
 	QElemType *base;//初始化的动态分配存储空间
	int front;//头指针，若队列不空，指向队列头元素 
	int rear; //尾指针，若队列不空，指向队列尾元素的下一个位置 
 }SqQueue;
 
 void InitQueue(SqQueue *Q);
 void DestroyQueue(SqQueue *Q);
 void ClearQueue(SqQueue *Q);
 Status QueueEmpty(SqQueue Q);
 int QueueLength(SqQueue Q);
 Status GetHead(SqQueue Q, QElemType *e);
 Status EnQueue(SqQueue *Q, QElemType e);
 Status DeQueue(SqQueue *Q, QElemType *e);
 void QueueTraverse(SqQueue Q, void(*vi)(QElemType));
 void print(QElemType i);
          
 int main()
 {
   Status j;
   int i=0,l;
   QElemType d;
   SqQueue Q;
   InitQueue(&Q);
   printf("初始化队列后，队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   printf("请输入整型队列元素(不超过%d个),-1为提前结束符: ",MAX_QSIZE-1);
   do
   {
     scanf("%d",&d);
     if(d==-1)
       break;
     i++;
     EnQueue(&Q,d);
   }while(i<MAX_QSIZE-1);
   printf("队列长度为: %d\n",QueueLength(Q));
   printf("现在队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   printf("连续%d次由队头删除元素,队尾插入元素:\n",MAX_QSIZE);
   for(l=1;l<=MAX_QSIZE;l++)
   {
     DeQueue(&Q,&d);
     printf("删除的元素是%d,请输入待插入的元素: ",d);
     scanf("%d",&d);
     EnQueue(&Q,d);
   }
   l=QueueLength(Q);
   printf("现在队列中的元素为: \n");
   QueueTraverse(Q,print);
   printf("共向队尾插入了%d个元素\n",i+MAX_QSIZE);
   if(l-2>0)
     printf("现在由队头删除%d个元素:\n",l-2);
   while(QueueLength(Q)>2)
   {
     DeQueue(&Q,&d);
     printf("删除的元素值为%d\n",d);
   }
   j=GetHead(Q,&d);
   if(j)
     printf("现在队头元素为: %d\n",d);
   ClearQueue(&Q);
   printf("清空队列后, 队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
   DestroyQueue(&Q);
	
 	return 0;
 }
 
 void InitQueue(SqQueue *Q)
 {
 	//构造一个空队列
	(*Q).base = (QElemType *)malloc(MAX_QSIZE * sizeof(QElemType));
	if (! (*Q).base)//存储分配失败
	{
		exit(OVERFLOW); 
	} 
	(*Q).front = (*Q).rear = 0;
 }
 
 void DestroyQueue(SqQueue *Q)
 {
 	//销毁队列Q,Q不再存在
	if ((*Q).base)
	{
		free((*Q).base);
	} 
	(*Q).base = NULL;
	(*Q).front = (*Q).rear = 0;
 }
 
 void ClearQueue(SqQueue *Q)
 {
 	//将Q清为空队列
	(*Q).front = (*Q).rear = 0; 
 }
 
 Status QueueEmpty(SqQueue Q)
 {
 	//若队列Q为空队列，则返回TRUE；否则返回FALSE
	if (Q.front == Q.rear)//队列空的标志
	{
		return TRUE; 
	} 
	else
	{
		return FALSE; 
	}
 }
 
 int QueueLength(SqQueue Q)
 {
 	//返回Q的元素个数，即队列的长度
	return (Q.rear-Q.front+MAX_QSIZE) % MAX_QSIZE;  
 }
 
 Status GetHead(SqQueue Q, QElemType *e)
 {
 	//若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
	if (Q.front == Q.rear)//队列空
	{
		return ERROR;
	} 
	*e = Q.base[Q.front];
	
	return OK;
 }
 
 Status EnQueue(SqQueue *Q, QElemType e)
 {
 	//插入元素e为Q的新的队尾元素
	if ( ((*Q).rear + 1)%MAX_QSIZE == (*Q).front )//队列满 
	{
		return ERROR;
	}
	(*Q).base[ (*Q).rear ] = e;
	(*Q).rear = ( (*Q).rear + 1) % MAX_QSIZE;
	
	return OK;
 }
 
 Status DeQueue(SqQueue *Q, QElemType *e)
 {
 	//若队列不为空，则删除Q的队头元素，用e返回其值，并返回OK；否则返回ERROR
	if ((*Q).front == (*Q).rear)//队列空 
	{
		return ERROR;
	}
	*e = (*Q).base[(*Q).front];
	(*Q).front = ((*Q).front+1)%MAX_QSIZE;
	
	return OK; 
 }
 
 void QueueTraverse(SqQueue Q, void(*vi)(QElemType))
 {
 	//从队头到队尾依次对队列Q中每个元素调用函数vi()
	int i;
	i = Q.front;
	
	while (i != Q.rear)
	{
		vi(Q.base[i]);
		i = (i+1) % MAX_QSIZE;
	}
	printf("\n");
 }
 
 void print(QElemType i)
 {
 	printf("%d ", i);
 }
 
 
 
  
  
 