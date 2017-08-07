#### 类模板LinkList.h

```C++
#pragma once

template<typename T>
struct Node
{
	T t;
	Node<T> *next;
};

template<typename T>
class LinkList
{
public:
	LinkList(void);
	~LinkList(void);
	//api

	//清空获取的长度
	int getLen();

	//向链表的某一个位置插入数据
	int insert(T &t, int pos);

	//从链表的某一个位置获取数据
	int get(int pos, T &t);

	//从链表的某一个位置删除数据
	int del(int pos, T &t);

private:
	int len;
	Node<T> *header;
};
```

#### 类模板LinkList.cpp

```C++
#include "LinkList.h"

template<typename T>
LinkList<T>::LinkList(void)
{
	header = new Node<T>;
	header->next = NULL;
	this->len = 0;
}

template<typename T>
LinkList<T>::~LinkList(void)
{
	Node<T> *tmp = NULL;
	
	while (header != NULL)
	{
      	//删除头节点之前 把下一个结点的位置给缓存下来，因为链表是单向的
		tmp = header->next; 
		delete header;
		header = tmp;
	}
	len = 0;
	header = NULL; //避免指针
}

//清空获取的长度
template<typename T>
int LinkList<T>::getLen()
{
	return len;
}

//向链表的某一个位置插入数据
template<typename T>
int  LinkList<T>::insert(T &t, int pos)
{
	//底层函数还没有结点的定义
	Node<T> *current = NULL;
	current = header;

	for (int i=0; i<pos; i++)
	{
		current = current->next;
	}
	//打造node 缓存用户的t
	Node<T> *node = new Node<T>;
	node->t = t; //缓存用户的数据  //注意 在这个地方 使用了=号操作 
	node->next = NULL;
	
	node->next = current->next;
	current->next = node;
	len ++;
	return 0;
}

//从链表的某一个位置获取数据
template<typename T>
int  LinkList<T>::get(int pos, T &t)
{
	//底层函数还没有结点的定义
	Node<T> *current = NULL;
	current = header;

	for (int i=0; i<pos; i++)
	{
		current = current->next;
	}
	
	t = current->next->t; //把缓存的t =号给引用t

	return 0;
}


//从链表的某一个位置删除数据
template<typename T>
int  LinkList<T>::del(int pos, T &t)
{
	Node<T> *current = NULL;
	Node<T> *ret = NULL;
	current = header;

	for (int i=0; i<pos; i++)
	{
		current = current->next;
	}
	ret = current->next;
	t = ret->t; //把要删除的结点传出去

	//指向删除操作
	current->next = ret->next;
	len -- ;
	//bug
	delete ret;

	return 0;
}
```

#### 链表模板类集成测试.cpp

```C++

#include <iostream>
using namespace std;
#include "LinkList.cpp"  //抛砖。。。

//1 出现深拷贝和浅copy的问题
//2 操作符重载 = 
//  copy构造函数

struct Teacher
{
	int age;
	char name[64];
};

void main01()
{
	Teacher t1, t2, t3;
	//t1.p = (char *)malloc(100);
	Teacher tmp;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;

	LinkList<Teacher> list;
	//插入结点
	list.insert(t1, 0 ); //头插法
	list.insert(t2, 0 ); //头插法
	list.insert(t3, 0 ); //头插法

	//遍历链表
	for (int i=0; i<3; i++)
	{
		list.get(i, tmp);
		cout<<tmp.age<<" ";
	}

	//删除链表

	while (list.getLen() > 0)
	{
		list.del(0, tmp);
		cout<<tmp.age<<" ";

	}
	
	
	return ;
}
void main02()
{
	Teacher t1, t2, t3;
	Teacher *tmp, *p1, *p2, *p3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	p1 = &t1;
	p2 = &t2;
	p3 = &t3;

	LinkList<Teacher *> list;
	//插入结点
	list.insert(p1, 0 ); //头插法
	list.insert(p2, 0 ); //头插法
	list.insert(p3, 0 ); //头插法

	//遍历链表
	for (int i=0; i<3; i++)
	{
		list.get(i, tmp);
		cout<<tmp->age<<" ";
	}

	//删除链表

	while (list.getLen() > 0)
	{
		list.del(0, tmp);
		cout<<tmp->age<<" ";
	}
	return ;
}

void main()
{
	//main01();
	main02();
	system("pause");
}
```