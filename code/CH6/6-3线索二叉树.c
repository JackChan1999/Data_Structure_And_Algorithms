#define CHAR 1  //字符型
//#define CHAR 0  //整型（二者选一）
#if CHAR 
  typedef char TElemType;
  TElemType Nil = ' ';//字符型以空格符为空
  #define form "%c"//输入输出的格式为%c
#else
  typedef int TElemType;
  TElemType Nil = 0;//整型以0为空 
  #define form "%d"//输入输出的格式为%d
#endif

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
 
 typedef enum{Link, Thread}PointerTag;//Link(0):指针，Thread(1):线索
 typedef struct BiThrNode
 {
 	TElemType data;
 	struct BiThrNode *lchild, *rchild;//左右孩子指针
	PointerTag LTag, RTag;//左右标志 
 }BiThrNode, *BiThrTree;
 
 BiThrTree pre;//全局变量，始终指向刚刚访问过的结点 
 
void CreateBiThrTree(BiThrTree *T);
void InThreading(BiThrTree p);
void InOrderThreading(BiThrTree *Thrt, BiThrTree T);
void InOrderTraverse_Thr(BiThrTree T, void(*Visit)(TElemType));
void PreThreading(BiThrTree p);
void PreOrderThreading(BiThrTree *Thrt, BiThrTree T);
void PreOrderTraverse_Thr(BiThrTree T, void(*Visit)(TElemType));
void PostThreading(BiThrTree p);
void PostOrderThreading(BiThrTree *Thrt, BiThrTree T);
void DestroyBiTree(BiThrTree *T);
void DestroyBiThrTree(BiThrTree *Thrt);
void vi(TElemType c);
 
 int main()
 {
   BiThrTree H,T;
 #if CHAR
   printf("请按先序输入二叉树(如:ab三个空格表示a为根结点,b为左子树的二叉树)\n");
 #else
   printf("请按先序输入二叉树(如:1 2 0 0 0表示1为根结点,2为左子树的二叉树)\n");
 #endif
   CreateBiThrTree(&T); /* 按先序产生二叉树 */
   InOrderThreading(&H,T); /* 在中序遍历的过程中，中序线索化二叉树 */
   printf("中序遍历(输出)线索二叉树:\n");
   InOrderTraverse_Thr(H,vi); /* 中序遍历(输出)线索二叉树 */
   printf("\n");
   DestroyBiThrTree(&H); /* 销毁线索二叉树 */
 #if CHAR
   printf("请按先序输入二叉树(如:ab三个空格表示a为根结点,b为左子树的二叉树)\n");
 #else
   printf("请按先序输入二叉树(如:1 2 0 0 0表示1为根结点,2为左子树的二叉树)\n");
 #endif
   scanf("%*c"); /* 吃掉回车符 */
   CreateBiThrTree(&T); /* 按先序产生二叉树T */
   PreOrderThreading(&H,T); /* 在先序遍历的过程中，先序线索化二叉树 */
   printf("先序遍历(输出)线索二叉树:\n");
   PreOrderTraverse_Thr(H,vi);
   DestroyBiThrTree(&H); /* 销毁线索二叉树 */
 #if CHAR
   printf("\n请按先序输入二叉树(如:ab三个空格表示a为根结点,b为左子树的二叉树)\n");
 #else
   printf("\n请按先序输入二叉树(如:1 2 0 0 0表示1为根结点,2为左子树的二叉树)\n");
 #endif
   scanf("%*c"); /* 吃掉回车符 */
   CreateBiThrTree(&T); /* 按先序产生二叉树T */
   PostOrderThreading(&H,T); /* 在后序遍历的过程中，后序线索化二叉树 */
   DestroyBiThrTree(&H); /* 销毁线索二叉树 */
 	
 	return 0;
 } 
 
 
 void CreateBiThrTree(BiThrTree *T)
 {
 	//按先序输入线索二叉树中结点的值，构造线索二叉树T。
    //0（整型）/空格（字符型）表示空结点
	TElemType ch;
	
	scanf(form, &ch);
	if (ch == Nil)
	{
		*T = NULL;
	} 
	else
	{
		*T = (BiThrTree)malloc(sizeof(BiThrNode));//生成根节点(先序)
		if (!*T)
		{
			exit(OVERFLOW);
		} 
		(*T)->data = ch;//给根结点赋值
		CreateBiThrTree(&(*T)->lchild);//递归构造左子树
		if ((*T)->lchild)//有左孩子
		{
			(*T)->LTag = Link;//给左标志赋值（指针） 
		} 
		CreateBiThrTree(&(*T)->rchild);//递归构造右子树 
		if ((*T)->rchild)//有右孩子
		{
			(*T)->RTag = Link;//给右标志赋值(指针) 
		} 	
	}//else 
 } 
 
 void InThreading(BiThrTree p)
 {
 	//通过中序遍历进行中序线索化，线索化后pre指向最后一个结点。
	if (p)//线索二叉树不空
	{
		InThreading(p->lchild);//递归左子树线索化
		if (! p->lchild)//没有左孩子
		{
			p->LTag = Thread;//左标志为线索(前驱)
			p->lchild = pre;//左孩子指针指向前驱 
		} 
		if (! pre->rchild)//前驱没有右孩子
		{
			pre->RTag = Thread;//前驱的右标志为线索（后继）
			pre->rchild = p;//前驱右孩子指针指向其后继（当前结点p） 
		} 
		pre = p;//保持pre指向p的前驱 
		InThreading(p->rchild);//递归右子树线索化 
	} 
 } 
 
 void InOrderThreading(BiThrTree *Thrt, BiThrTree T)
 {
 	//中序遍历二叉树T,并将其中序线索化，Thrt指向头结点。
	*Thrt = (BiThrTree)malloc(sizeof(BiThrNode));
	if (! *Thrt)//生成头结点不成功
	{
		exit(OVERFLOW); 
	}
	(*Thrt)->LTag = Link;//建头结点，左标志为指针
	(*Thrt)->RTag = Thread;//右标志为线索
	(*Thrt)->rchild = *Thrt;//右指针回指 
	if (!T)//若二叉树为空，则左指针回指
	{
		(*Thrt)->lchild = *Thrt;
	} 
	else
	{
		(*Thrt)->lchild = T;//头结点的左指针指向根节点
		pre = *Thrt;//pre（前驱）的初值指向头结点
		InThreading(T);//中序遍历进行中序线索化，pre指向中序遍历的最后一个结点
		pre->rchild = *Thrt;//最后一个结点的右指针指向头结点
		pre->RTag = Thread;//最后一个结点的右标志为线索
		(*Thrt)->rchild = pre;//头结点的右指针指向中序遍历的最后一个结点 	
	}
 } 
 
 void InOrderTraverse_Thr(BiThrTree T, void(*Visit)(TElemType))
 {
 	//中序遍历线索二叉树T(头结点)的非递归算法。
	BiThrTree p;
	
	p = T->lchild;//p指向根节点
	while (p != T)
	{
		//空树或遍历结束时，p == T
		while (p->LTag == Link)//由根节点一直找到二叉树的最左结点
		{
			p = p->lchild; 
		} 
		Visit(p->data);//访问此结点
		while (p->RTag==Thread && p->rchild!=T)//p->rchild是线索（后继），且不是遍历的最后一个结点
		{
			p = p->rchild;
			Visit(p->data);//访问后继结点 
		} 
		p = p->rchild;//若p->rchild不是线索（是右孩子），p指向右孩子，返回循环 
		              //找这棵子树中序遍历的第1个结点	 
	} 
 } 
 
 void PreThreading(BiThrTree p)
 {
 	//PreOrderThreading()调用的递归函数
	if (! pre->rchild)//p的前驱没有右孩子
	{
		pre->rchild = p;//p前驱的后继指向p
		pre->RTag = Thread;//pre的右孩子为线索 
	} 
	if (! p->lchild)//p没有左孩子
	{
		p->LTag = Thread;//p的左孩子为线索
		p->lchild = pre;//p的左孩子指向前驱 
	} 
	pre = p;//移动前驱
	if (p->LTag == Link)//p有左孩子
	{
		PreThreading(p->lchild);//对p的左孩子递归调用preThreading() 
	} 
	if (p->RTag == Link)//p有右孩子
	{
		PreThreading(p->rchild);//对p的右孩子递归调用preThreading() 
	} 
 }  
 
 void PreOrderThreading(BiThrTree *Thrt, BiThrTree T)
 {
 	//先序线索化二叉树T，头结点的右指针指向先序遍历的最后1个结点
	*Thrt = (BiThrTree)malloc(sizeof(BiThrNode));
	
	if (! *Thrt)//生成头结点
	{
		exit(OVERFLOW); 
	} 
	(*Thrt)->LTag = Link;//头结点的左指针为孩子
	(*Thrt)->RTag = Thread;//头结点的右指针为线索
	(*Thrt)->rchild = *Thrt;//头结点的右指针指向自身 
	
	if (! T)//空树
	{
		(*Thrt)->lchild = *Thrt;//头结点的左指针也指向自身 
	} 
	else
	{
		//非空树
		(*Thrt)->lchild = T;//头结点的左指针指向根结点
		pre = *Thrt;//前驱为头结点 
		PreThreading(T);//从头结点开始先序递归线索化
		pre->rchild = *Thrt;//最后一个结点的后继指向头结点
		pre->RTag = Thread;
		(*Thrt)->rchild = pre;//头结点的后继指向最后一个结点 	
	} //else 
 }
 
 void PreOrderTraverse_Thr(BiThrTree T, void(*Visit)(TElemType))
 {
 	//先序遍历线索二叉树T(头结点)的非递归算法
	BiThrTree p = T->lchild;//p指向根结点
	
	while (p != T)//p没指向头结点(遍历的最后1个结点的后继指向头结点)
	{
		Visit(p->data);//访问根结点
		if (p->LTag == Link)//p有左孩子
		{
			p = p->lchild;//p指向左孩子(后继) 
		} 
		else
		{
			p = p->rchild;//p指向右孩子或后继 
		} 
	} 
 }
 
 void PostThreading(BiThrTree p)
 {
 	//PostOrderThreading()调用的递归函数
	if (p)//p不空
	{
		PostThreading(p->lchild);//对p的左孩子递归调用PostThreading()
		PostThreading(p->rchild);//对p的右孩子递归调用PostThreading()
	    
	    if (! p->lchild)//p没有左孩子
		{
			p->LTag = Thread;//p的左孩子为线索
			p->lchild = pre;//p的左孩子指向前驱 
		} 
		if (! pre->rchild)//p的前驱没有右孩子
		{
			pre->RTag = Thread;//p前驱的右孩子为线索
			pre->rchild = p;//p前驱的后继指向p 
		} 
		pre = p;//移动前驱 
	} 	
 } 
 
 void PostOrderThreading(BiThrTree *Thrt, BiThrTree T)
 {
 	//后序递归线索化二叉树
	*Thrt = (BiThrTree)malloc(sizeof(BiThrNode));
	
	if (! *Thrt)//生成头结点
	{
		exit(OVERFLOW); 
	} 
	(*Thrt)->LTag = Link;//头结点的左指针为孩子
	(*Thrt)->RTag = Thread;//头结点的右指针为线索
	
	if (! T)//空树
	{
		(*Thrt)->lchild = (*Thrt)->rchild = *Thrt;//头指针的左右指针指向自身 
	} 
	else
	{
		//非空树
		(*Thrt)->lchild = (*Thrt)->rchild = T;//头结点的左右指针指向根结点(最后一个结点)
		pre = *Thrt;//前驱为头结点
		PostThreading(T);//从头结点开始后序递归线索化
		if (pre->RTag != Link)//最后一个结点没有右孩子
		{
			pre->rchild = *Thrt;//最后一个结点的后继指向头结点
			pre->RTag = Thread; 
		} 
	}//else 
 } 
 
 void DestroyBiTree(BiThrTree *T)
 {
 	//DestroyBiThrTree调用的递归函数，T指向根结点
	if (*T)//非空树
	{
		if ((*T)->LTag == 0)//有左孩子
		{
			DestroyBiTree(&(*T)->lchild);//销毁左孩子子树 
		} 
		if ((*T)->RTag == 0)//有右孩子
		{
			DestroyBiTree(&(*T)->rchild);//销毁右孩子子树 
		} 
		free(*T);//释放根结点
		T = NULL;//空指针赋0 
	} 
 }
 
 void DestroyBiThrTree(BiThrTree *Thrt)
 {
 	//初始条件：线索二叉树Thrt存在。操作结果：销毁线索二叉树Thrt
	if (*Thrt)//头结点存在
	{
		if ((*Thrt)->lchild)//根结点存在 
		{
			DestroyBiTree(&(*Thrt)->lchild);//递归销毁头结点lchild所指二叉树 
		} 
		free(*Thrt);//释放头结点
		*Thrt = NULL;//线索二叉树Thrt指针赋0 
	} 
 } 
 
 void vi(TElemType c)
 {
   printf(form" ",c);
 }
 
 
 
 
 
 
 
 
   
 