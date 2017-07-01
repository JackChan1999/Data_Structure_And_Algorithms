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
 
 #define N 10 /* 数据元素个数 */
 typedef int KeyType; /* 设关键字域为整型 */
 typedef struct
 {
   KeyType key;
   int others;
 } ElemType; /* 数据元素类型 */
 #define EQ(a,b) ((a)==(b))
 #define LT(a,b) ((a)<(b))
 #define LQ(a,b) ((a)<=(b))
 typedef ElemType TElemType;
 typedef struct BiTNode
 {
   TElemType data;
   struct BiTNode *lchild,*rchild; /* 左右孩子指针 */
 }BiTNode,*BiTree; /* 二叉树的存储结构 */
 
 void InitBiTree(BiTree *T)
 { /* 操作结果：构造空二叉树T */
   *T=NULL;
 }

 void DestroyBiTree(BiTree *T)
 { /* 初始条件：二叉树T存在。操作结果：销毁二叉树T */
   if(*T) /* 非空树 */
   {
     if((*T)->lchild) /* 有左孩子 */
       DestroyBiTree(&(*T)->lchild); /* 销毁左孩子子树 */
     if((*T)->rchild) /* 有右孩子 */
       DestroyBiTree(&(*T)->rchild); /* 销毁右孩子子树 */
     free(*T); /* 释放根结点 */
     *T=NULL; /* 空指针赋0 */
   }
 }
 
 void PreOrderTraverse(BiTree T,void(*Visit)(TElemType))
 { /* 初始条件：二叉树T存在，Visit是对结点操作的应用函数。算法6.1，有改动 */
   /* 操作结果：先序递归遍历T，对每个结点调用函数Visit一次且仅一次 */
   if(T) /* T不空 */
   {
     Visit(T->data); /* 先访问根结点 */
     PreOrderTraverse(T->lchild,Visit); /* 再先序遍历左子树 */
     PreOrderTraverse(T->rchild,Visit); /* 最后先序遍历右子树 */
   }
 }

 void InOrderTraverse(BiTree T,void(*Visit)(TElemType))
 { /* 初始条件：二叉树T存在，Visit是对结点操作的应用函数 */
   /* 操作结果：中序递归遍历T，对每个结点调用函数Visit一次且仅一次 */
   if(T)
   {
     InOrderTraverse(T->lchild,Visit); /* 先中序遍历左子树 */
     Visit(T->data); /* 再访问根结点 */
     InOrderTraverse(T->rchild,Visit); /* 最后中序遍历右子树 */
   }
 }
 
 /* 包括InitBiTree()、DestroyBiTree()、PreOrderTraverse()和InOrderTraverse()4函数 */
 #define InitDSTable InitBiTree /* 与初始化二叉树的操作同 */
 #define DestroyDSTable DestroyBiTree /* 与销毁二叉树的操作同 */
 #define TraverseDSTable InOrderTraverse /* 与中序遍历二叉树的操作同 */

 BiTree SearchBST(BiTree T,KeyType key)
 { /* 在根指针T所指二叉排序树中递归地查找某关键字等于key的数据元素， */
   /* 若查找成功，则返回指向该数据元素结点的指针，否则返回空指针。算法9.5(a) */
   if(!T||EQ(key,T->data.key))
     return T; /* 查找结束 */
   else if LT(key,T->data.key) /* 在左子树中继续查找 */
     return SearchBST(T->lchild,key);
   else
     return SearchBST(T->rchild,key); /* 在右子树中继续查找 */
 }

 Status SearchBST1(BiTree *T,KeyType key,BiTree f,BiTree *p) /* 算法9.5(b) */
 { /* 在根指针T所指二叉排序树中递归地查找其关键字等于key的数据元素，若查找 */
   /* 成功，则指针p指向该数据元素结点，并返回TRUE，否则指针p指向查找路径上 */
   /* 访问的最后一个结点并返回FALSE，指针f指向T的双亲，其初始调用值为NULL */
   if(!*T) /* 查找不成功 */
   {
     *p=f;
     return FALSE;
   }
   else if EQ(key,(*T)->data.key) /*  查找成功 */
   {
     *p=*T;
     return TRUE;
   }
   else if LT(key,(*T)->data.key)
     return SearchBST1(&(*T)->lchild,key,*T,p); /* 在左子树中继续查找 */
   else
     return SearchBST1(&(*T)->rchild,key,*T,p); /*  在右子树中继续查找 */
 }

 Status InsertBST(BiTree *T, ElemType e)
 { /* 当二叉排序树T中不存在关键字等于e.key的数据元素时，插入e并返回TRUE，否则返回FALSE。算法9.6 */
   BiTree p,s;
   if(!SearchBST1(T,e.key,NULL,&p)) /* 查找不成功 */
   {
     s=(BiTree)malloc(sizeof(BiTNode));
     s->data=e;
     s->lchild=s->rchild=NULL;
     if(!p)
       *T=s; /* 被插结点*s为新的根结点 */
     else if LT(e.key,p->data.key)
       p->lchild=s; /* 被插结点*s为左孩子 */
     else
       p->rchild=s; /* 被插结点*s为右孩子 */
     return TRUE;
   }
   else
     return FALSE; /* 树中已有关键字相同的结点，不再插入 */
 }

 void Delete(BiTree *p)
 { /* 从二叉排序树中删除结点p，并重接它的左或右子树。算法9.8 */
   BiTree q,s;
   if(!(*p)->rchild) /* p的右子树空则只需重接它的左子树(待删结点是叶子也走此分支) */
   {
     q=*p;
     *p=(*p)->lchild;
     free(q);
   }
   else if(!(*p)->lchild) /* p的左子树空，只需重接它的右子树 */
   {
     q=*p;
     *p=(*p)->rchild;
     free(q);
   }
   else /* p的左右子树均不空 */
   {
     q=*p;
     s=(*p)->lchild;
     while(s->rchild) /* 转左，然后向右到尽头(找待删结点的前驱) */
     {
       q=s;
       s=s->rchild;
     }
     (*p)->data=s->data; /* s指向被删结点的＂前驱＂(将被删结点前驱的值取代被删结点的值) */
     if(q!=*p) /* 情况1 */
       q->rchild=s->lchild; /* 重接*q的右子树 */
     else /* 情况2 */
       q->lchild=s->lchild; /* 重接*q的左子树 */
     free(s);
   }
 }

 Status DeleteBST(BiTree *T,KeyType key)
 { /* 若二叉排序树T中存在关键字等于key的数据元素时，则删除该数据元素结点， */
   /* 并返回TRUE；否则返回FALSE。算法9.7 */
   if(!*T) /* 不存在关键字等于key的数据元素 */
     return FALSE;
   else
   {
     if EQ(key,(*T)->data.key) /* 找到关键字等于key的数据元素 */
       Delete(T);
     else if LT(key,(*T)->data.key)
       DeleteBST(&(*T)->lchild,key);
     else
       DeleteBST(&(*T)->rchild,key);
     return TRUE;
   }
 }

 void print(ElemType c)
 {
   printf("(%d,%d) ",c.key,c.others);
 }

 int main()
 {
   BiTree dt,p;
   int i;
   KeyType j;
   ElemType r[N]={{45,1},{12,2},{53,3},{3,4},{37,5},{24,6},{100,7},{61,8},{90,9},{78,10}};
   /* 以教科书图9.7(a)为例，另加除关键字之外的其他信息 */
   InitDSTable(&dt); /* 构造空表 */
   for(i=0;i<N;i++)
     InsertBST(&dt,r[i]); /* 依次插入数据元素 */
   TraverseDSTable(dt,print);
   printf("\n请输入待查找的值: ");
   scanf("%d",&j);
   p=SearchBST(dt,j);
   if(p)
   {
     printf("表中存在此值。");
     DeleteBST(&dt,j);
     printf("删除此值后:\n");
     TraverseDSTable(dt,print);
     printf("\n");
   }
   else
     printf("表中不存在此值\n");
   DestroyDSTable(&dt);
   
   return 0; 
 }