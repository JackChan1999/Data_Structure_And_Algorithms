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
 #define N 5 /* 数据元素个数 */
 typedef char KeyType; /* 设关键字域为字符型 */
 typedef struct
 {
   KeyType key;
   int order;
 }ElemType; /* 数据元素类型 */

 #define EQ(a,b) ((a)==(b))
 #define LT(a,b) ((a)<(b))
 #define LQ(a,b) ((a)<=(b))
 typedef struct BSTNode
 {
   ElemType data;
   int bf; /* 结点的平衡因子 */
   struct BSTNode *lchild,*rchild; /* 左、右孩子指针 */
 }BSTNode, *BSTree;
 
 typedef ElemType TElemType; /* 定义二叉树基本操作的树元素类型 */
 typedef BSTree BiTree; /* 定义二叉树基本操作的指针类型 */
 #define InitDSTable InitBiTree /* 与初始化二叉树的操作同 */
 #define DestroyDSTable DestroyBiTree /* 与销毁二叉树的操作同 */
 #define TraverseDSTable InOrderTraverse /* 与中序遍历二叉树的操作同 */
 #define LH +1 /* 左高 */
 #define EH 0  /* 等高 */
 #define RH -1 /* 右高 */
 
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

 void R_Rotate(BSTree *p)
 { /* 对以*p为根的二叉排序树作右旋处理，处理之后p指向新的树 */
   /* 根结点，即旋转处理之前的左子树的根结点。算法9.9 */
   BSTree lc;
   lc=(*p)->lchild; /* lc指向p的左子树根结点 */
   (*p)->lchild=lc->rchild; /* lc的右子树挂接为p的左子树 */
   lc->rchild=*p;
   *p=lc; /* p指向新的根结点 */
 }

 void L_Rotate(BSTree *p)
 { /* 对以*p为根的二叉排序树作左旋处理，处理之后p指向新的树 */
   /* 根结点，即旋转处理之前的右子树的根结点。算法9.10 */
   BSTree rc;
   rc=(*p)->rchild; /* rc指向p的右子树根结点 */
   (*p)->rchild=rc->lchild; /* rc的左子树挂接为p的右子树 */
   rc->lchild=*p;
   *p=rc; /* p指向新的根结点 */
 }

 void LeftBalance(BSTree *T)
 { /* 对以指针T所指结点为根的二叉树作左平衡旋转处理，本算法结束时， */
   /* 指针T指向新的根结点。算法9.12 */
   BSTree lc,rd;
   lc=(*T)->lchild; /* lc指向*T的左子树根结点 */
   switch(lc->bf)
   { /* 检查*T的左子树的平衡度，并作相应平衡处理 */
     case LH: /* 新结点插入在*T的左孩子的左子树上，要作单右旋处理 */
              (*T)->bf=lc->bf=EH;
              R_Rotate(T);
              break;
     case RH: /* 新结点插入在*T的左孩子的右子树上，要作双旋处理 */
              rd=lc->rchild; /* rd指向*T的左孩子的右子树根 */
              switch(rd->bf)
              { /* 修改*T及其左孩子的平衡因子 */
                case LH: (*T)->bf=RH;
                         lc->bf=EH;
                         break;
                case EH: (*T)->bf=lc->bf=EH;
                         break;
                case RH: (*T)->bf=EH;
                         lc->bf=LH;
              }
              rd->bf=EH;
              L_Rotate(&(*T)->lchild); /* 对*T的左子树作左旋平衡处理 */
              R_Rotate(T); /* 对*T作右旋平衡处理 */
   }
 }

 void RightBalance(BSTree *T)
 { /* 对以指针T所指结点为根的二叉树作右平衡旋转处理，本算法结束时， */
   /* 指针T指向新的根结点 */
   BSTree rc,rd;
   rc=(*T)->rchild; /* rc指向*T的右子树根结点 */
   switch(rc->bf)
   { /* 检查*T的右子树的平衡度，并作相应平衡处理 */
     case RH: /* 新结点插入在*T的右孩子的右子树上，要作单左旋处理 */
              (*T)->bf=rc->bf=EH;
              L_Rotate(T);
              break;
     case LH: /* 新结点插入在*T的右孩子的左子树上，要作双旋处理 */
              rd=rc->lchild; /* rd指向*T的右孩子的左子树根 */
              switch(rd->bf)
              { /* 修改*T及其右孩子的平衡因子 */
                case RH: (*T)->bf=LH;
                         rc->bf=EH;
                         break;
                case EH: (*T)->bf=rc->bf=EH;
                         break;
                case LH: (*T)->bf=EH;
                         rc->bf=RH;
              }
              rd->bf=EH;
              R_Rotate(&(*T)->rchild); /* 对*T的右子树作右旋平衡处理 */
              L_Rotate(T); /* 对*T作左旋平衡处理 */
   }
 }

 Status InsertAVL(BSTree *T,ElemType e,Status *taller)
 { /* 若在平衡的二叉排序树T中不存在和e有相同关键字的结点，则插入一个 */
   /* 数据元素为e的新结点，并返回1，否则返回0。若因插入而使二叉排序树 */
   /* 失去平衡，则作平衡旋转处理，布尔变量taller反映T长高与否。算法9.11 */
   if(!*T)
   { /* 插入新结点，树“长高”，置taller为TRUE */
     *T=(BSTree)malloc(sizeof(BSTNode));
     (*T)->data=e;
     (*T)->lchild=(*T)->rchild=NULL;
     (*T)->bf=EH;
     *taller=TRUE;
   }
   else
   {
     if EQ(e.key,(*T)->data.key)
     { /* 树中已存在和e有相同关键字的结点则不再插入 */
       *taller=FALSE;
       return FALSE;
     }
     if LT(e.key,(*T)->data.key)
     { /* 应继续在*T的左子树中进行搜索 */
       if(!InsertAVL(&(*T)->lchild,e,taller)) /* 未插入 */
         return FALSE;
       if(*taller) /*  已插入到*T的左子树中且左子树“长高” */
         switch((*T)->bf) /* 检查*T的平衡度 */
         {
           case LH: /* 原本左子树比右子树高，需要作左平衡处理 */
                    LeftBalance(T);
                    *taller=FALSE;
                    break;
           case EH: /* 原本左、右子树等高，现因左子树增高而使树增高 */
                    (*T)->bf=LH;
                    *taller=TRUE;
                    break;
           case RH: (*T)->bf=EH; /* 原本右子树比左子树高，现左、右子树等高 */
                    *taller=FALSE;
         }
     }
     else
     { /* 应继续在*T的右子树中进行搜索 */
       if(!InsertAVL(&(*T)->rchild,e,taller)) /* 未插入 */
         return FALSE;
       if(*taller) /* 已插入到T的右子树且右子树“长高” */
         switch((*T)->bf) /* 检查T的平衡度 */
         {
           case LH: (*T)->bf=EH; /* 原本左子树比右子树高，现左、右子树等高 */
                    *taller=FALSE;
                    break;
           case EH: /* 原本左、右子树等高，现因右子树增高而使树增高 */
                    (*T)->bf=RH;
                    *taller=TRUE;
                    break;
           case RH: /* 原本右子树比左子树高，需要作右平衡处理 */
                    RightBalance(T);
                    *taller=FALSE;
         }
     }
   }
   return TRUE;
 }

 void print(ElemType c)
 {
   printf("(%d,%d)",c.key,c.order);
 }

 int main()
 {
   BSTree dt,p;
   Status k;
   int i;
   KeyType j;
   ElemType r[N]={{13,1},{24,2},{37,3},{90,4},{53,5}}; /* (以教科书图9.12为例) */
   InitDSTable(&dt); /* 初始化空树 */
   for(i=0;i<N;i++)
     InsertAVL(&dt,r[i],&k); /* 建平衡二叉树 */
   PreOrderTraverse(dt,print); /* 先序遍历平衡二叉树 */
   printf("先序遍历平衡二叉树\n");
   TraverseDSTable(dt,print); /* 按关键字顺序遍历二叉树 */
   printf("\n请输入待查找的关键字: ");
   scanf("%d",&j);
   p=SearchBST(dt,j); /* 查找给定关键字的记录 */
   if(p)
     print(p->data);
   else
     printf("表中不存在此值");
   printf("\n");
   DestroyDSTable(&dt);
   
   return 0; 
 }