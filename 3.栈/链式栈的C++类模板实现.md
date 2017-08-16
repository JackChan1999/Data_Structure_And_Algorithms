链式栈的C++类模板实现

### LinkStack.hpp

```c++
#pragma  once
#include "LinkList.hpp"

template <typename T>
class LinkStack  
{
public:
	LinkStack();
	~LinkStack();
public:
	int clear();
	int push(T &t);
	int pop(T &t);
	int top(T &t);
	int size();
protected:
	LinkList<T> *m_list;
};

template <typename T>
LinkStack<T>::LinkStack()
{
	m_list = new LinkList<T>;
}

template <typename T>
LinkStack<T>::~LinkStack()
{
	clear();
	delete m_list;
	m_list = NULL;
}

template <typename T>
int LinkStack<T>::clear()
{
	T t;
	while (m_list->length() > 0)
	{
		m_list->del(0, t);
	}
	return 0;
}

template <typename T>
int LinkStack<T>::push(T &t)
{
	return m_list->insert(t, 0);
}

template <typename T>
int LinkStack<T>::pop(T &t)
{
	return m_list->del(0, t);
}

template <typename T>
int LinkStack<T>::top(T &t)
{
	return m_list->get(0, t);
}

template <typename T>
int LinkStack<T>::size()
{
	return m_list->length();
}
```

### main.cpp

```c++
#include <iostream>
#include "LinkStack.hpp"
using namespace std;

class Teacher02
{
public:
	char name[32];
	int age;
};

void main21()
{
	Teacher02 t1, t2, t3, tmp;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;

	//创建栈 
	LinkStack<Teacher02> stack;

	//压栈
	stack.push(t1);
	stack.push(t2);
	stack.push(t3);

	//遍历
	stack.top(tmp);
	cout << tmp.age << "\n";

	//出栈
	while (stack.size() > 0)
	{
		stack.pop(tmp);
		cout << tmp.age << " ";
	}
	stack.clear();
}

void main22()
{
	main21();
	system("pause");
	return ;
}
```