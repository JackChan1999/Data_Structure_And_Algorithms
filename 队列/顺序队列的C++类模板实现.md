顺序队列的C++类模板实现

### SeqQueue.hpp

```c++
#pragma once
#include "SeqList.hpp"

template <typename T>
class SeqQueue
{
public:
	SeqQueue(int capacity=20);
	~SeqQueue();
public:
	int clear();
	int append(T &t);
	int retrieve(T &t);
	int header(T &t);
	int length();
	int capacity();
protected:
	SeqList<T> *m_list;
};

template <typename T>
SeqQueue<T>::SeqQueue(int capacity=20)
{
	m_list = new SeqList<T>(capacity); //node: 不要忘记有参函数的调用
}

template <typename T>
SeqQueue<T>::~SeqQueue()
{
	delete m_list;
}

template <typename T>
int SeqQueue<T>::clear()
{
	return m_list->clear();
}

template <typename T>
int SeqQueue<T>::append(T &t)
{
	return m_list->insert(t, m_list->length());
}

template <typename T>
int SeqQueue<T>::retrieve(T &t)
{
	return m_list->del(0, t);
}

template <typename T>
int SeqQueue<T>::header(T &t)
{
	return m_list->get(0, t);
}

template <typename T>
int SeqQueue<T>::length()
{
	return m_list->length();
}

template <typename T>
int SeqQueue<T>::capacity()
{
	return m_list->capacity();
}
```

### main.cpp

```c++
#include <iostream>
#include "SeqQueue.hpp"
using namespace std;

class Teacher06
{
public:
	char name[32];
	int age;
};

void main61()
{
	Teacher06 t1, t2, t3, tmp;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	
	//创建队列
	SeqQueue<Teacher06> queue(11);

	queue.append(t1);
	queue.append(t2);
	queue.append(t3);

	while (queue.length() > 0)
	{
		queue.retrieve(tmp);
		cout << tmp.age << " ";
	}
	queue.clear();
}

void main()
{
	main61();
	system("pause");
	return ;
}
```