## Linux内核链表

### LinkList.h

```c
#ifndef _MYLINKLIST_H_
#define _MYLINKLIST_H_

typedef void LinkList;
/*
typedef struct _tag_LinkListNode LinkListNode;
struct _tag_LinkListNode
{
	LinkListNode* next;
};
*/

typedef struct _tag_LinkListNode
{
	struct _tag_LinkListNode* next;
}LinkListNode;

//创建链表
LinkList* LinkList_Create();

//销毁链表
void LinkList_Destroy(LinkList* list);

//清空链表 
void LinkList_Clear(LinkList* list);

//清空获取的长度
int LinkList_Length(LinkList* list);

//向链表的某一个位置插入数据
int LinkList_Insert(LinkList* list, LinkListNode* node, int pos);

//从链表的某一个位置获取数据
LinkListNode* LinkList_Get(LinkList* list, int pos);

//从链表的某一个位置删除数据
LinkListNode* LinkList_Delete(LinkList* list, int pos);

#endif
```

### LinkList.c

```c
#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LinkList.h"

// Linux内核链表，非传统链表
typedef struct _tag_LinkList
{
	LinkListNode header; // 第一个域（第一个元素）
	int length;
}TLinkList;
// 创建链表
LinkList* LinkList_Create()
{
	TLinkList *ret = (TLinkList *)malloc(sizeof(TLinkList));
	if (ret == NULL)
	{
		return NULL;
	}
	memset(ret, 0, sizeof(TLinkList) );
	ret->header.next = NULL;
	ret->length = 0;
	return ret;
}

// 销毁链表
void LinkList_Destroy(LinkList* list)
{
	if (list != NULL)
	{
		free(list);
	}
	return ;
}

//清空链表 
void LinkList_Clear(LinkList* list)
{
	TLinkList *tlist = (LinkList *)list;
	if (tlist == NULL)
	{
		return ;
	}
	tlist->header.next = NULL;
	tlist->length = 0;
	return ;
}

//清空获取的长度
int LinkList_Length(LinkList* list)
{
	TLinkList *tlist = (LinkList *)list;
	if (tlist == NULL)
	{
		return -1;
	}
	return tlist->length;
}


// 向链表的某一个位置插入数据
int LinkList_Insert(LinkList* list, LinkListNode* node, int pos)
{
	int ret = 0, i = 0;
	LinkListNode *current = NULL;
	TLinkList *tlist = NULL;

	tlist = (LinkList *)list;
	if (list==NULL || node==NULL || pos<0)
	{
		return -1;
	}
	current = &(tlist->header);

	for (i=0; i<pos; i++)
	{
		current = current->next;
	}
	//让node连接后续结点
	node->next = current->next;
	//让2号位置 连接 node
	current->next = node;
	tlist->length ++;

	return 0;
}

//从链表的某一个位置获取数据
LinkListNode* LinkList_Get(LinkList* list, int pos)
{
	int ret = 0, i = 0;
	LinkListNode *current = NULL;
	TLinkList *tlist = NULL;

	tlist = (LinkList *)list;
	if (list==NULL || pos<0)
	{
		return NULL;
	}
	current = &(tlist->header);

	for (i=0; i<pos; i++)
	{
		current = current->next;
	}
	return current->next;
}

//从链表的某一个位置删除数据
LinkListNode* LinkList_Delete(LinkList* list, int pos)
{
	//分析赋值指针变量
	int  i = 0;
	LinkListNode *current = NULL;
	LinkListNode *ret = NULL;

	TLinkList *tlist = NULL;

	tlist = (LinkList *)list;
	if (list==NULL || pos<0)
	{
		return NULL;
	}
	current = &(tlist->header);

	for (i=0; i<pos; i++)
	{
		current = current->next;
	}

	//假设删除位置3 
	ret = current->next;
	current->next = ret->next;
	tlist->length --;
	
	return ret;
}
```
### 企业级链表的设计与实现集成测试.c

```c
#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LinkList.h"

typedef struct _Teacher
{
	LinkListNode node;
	int age;
	char name[64];
	int title;
}Teacher;

void main()
{
	int	ret = 0, i = 0;
	LinkList *list = NULL;
	Teacher t1, t2, t3, t4, t5;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	t4.age = 34;
	t5.age = 35;

	list = LinkList_Create();
	if (list ==NULL)
	{
		return ;
	}

	//向链表中插入结点
	//请问如何把老师的业务节点插入到链表中
	//好像：链表业务逻辑不支持老师业务节点的插入
	//效果 实现具体业务数据和链表算法的分离的
	ret = LinkList_Insert(list, (LinkListNode*) &t1, LinkList_Length(list)); //尾插法
	ret = LinkList_Insert(list, (LinkListNode*) &t2, LinkList_Length(list)); //尾插法
	ret = LinkList_Insert(list, (LinkListNode*) &t3, LinkList_Length(list)); //尾插法
	ret = LinkList_Insert(list, (LinkListNode*) &t4, LinkList_Length(list)); //尾插法
	ret = LinkList_Insert(list, (LinkListNode*) &t5, LinkList_Length(list)); //尾插法

	//遍历链表
	for (i=0; i<LinkList_Length(list); i++)
	{
		Teacher *tmp = (Teacher *)LinkList_Get(list, i);
		if (tmp == NULL)
		{
			return ;
		}
		printf("age:%d ", tmp->age);
	}

	//链表结点删除
	while (LinkList_Length(list) > 0)
	{
		Teacher *tmp = (Teacher *)LinkList_Delete(list, 0);
		if (tmp == NULL)
		{
			return ;
		}
		printf("age:%d ", tmp->age);
	}

	LinkList_Destroy(list);

	system("pause");
	return ;
}
```