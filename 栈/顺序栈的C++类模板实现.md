顺序栈的C++类模板实现

### SeqStack.hpp

```c++
#pragma  once
#include "SeqList.hpp"

template <typename T>
class SeqStack
{
public:
	SeqStack(int capacity = 20);
	~SeqStack();
public:
	int clear();
	int push(T &t);
	int pop(T &t);
	int top(T &t);
	int size();
	int capacity();
protected:
	SeqList<T> *m_list;
};

template <typename T>
SeqStack<T>::SeqStack(int capacity = 20)
{
	m_list = new SeqList<T>(capacity);
}

template <typename T>
SeqStack<T>::~SeqStack()
{
	clear();
	delete m_list;
	m_list = NULL;
}

template <typename T>
int SeqStack<T>::clear()
{
	T t;
	while (size() > 0 )
	{
		pop(t);
	}
	return 0;
}

template <typename T>
int SeqStack<T>::push(T &t)
{
	return m_list->insert(t, m_list->length());
}

template <typename T>
int SeqStack<T>::pop(T &t)
{
	return m_list->del(m_list->length()-1, t); //10个元素 数组下表为9
}

template <typename T>
int SeqStack<T>::top(T &t)
{
	return m_list->get(m_list->length()-1, t);
}

template <typename T>
int SeqStack<T>::size()
{
	return m_list->length();
}

template <typename T>
int SeqStack<T>::capacity()
{
	return m_list->capacity();
}
```

### main.cpp

```c++
#include <iostream>
#include "SeqStack.hpp"
using namespace std;

class Teacher05
{
public:
	char name[32];
	int age;
};

void main51()
{
	Teacher05 t1, t2, t3, tmp;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;

	//创建栈 
	SeqStack<Teacher05> stack;

	stack.push(t1);
	stack.push(t2);
	stack.push(t3);

	//出栈
	while (stack.size() > 0)
	{
		stack.pop(tmp);
		cout << tmp.age << " ";
	}
	stack.clear();
}

void main55()
{
	main51();
	system("pause");
	return ;
}
```