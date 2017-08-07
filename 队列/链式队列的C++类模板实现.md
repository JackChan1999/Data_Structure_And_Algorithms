链式队列的C++类模板实现

### LinkQueue.hpp

```c++
#pragma  once
#include "LinkList.hpp"

template <typename T>
class LinkQueue
{
public:
	LinkQueue();
	~LinkQueue();
public:
	int clear();
	int append(T &t);
	int retieve(T &t);
	int header(T &t);
	int length();

protected:
	LinkList<T>* m_list;
};

template <typename T>
 LinkQueue<T>::LinkQueue()
 {
	 m_list = new LinkList<T>;
 }

 template <typename T>
 LinkQueue<T>::~LinkQueue()
 {
	 clear();
	 delete m_list;
	 m_list = NULL;
 }

 template <typename T>
 int LinkQueue<T>::clear()
 {
	 T t;
	 while (length() > 0)
	 {
		 retieve(t);
	 }
	 return 0;
 }

 template <typename T>
 int LinkQueue<T>::append(T &t)
 {
	  return m_list->insert(t, m_list->length());
 }

 template <typename T>
 int LinkQueue<T>::retieve(T &t)
 {
	  return m_list->del(0, t);
 }

 template <typename T>
 int LinkQueue<T>::header(T &t)
 {
	  return m_list->get(0, t);
 }

 template <typename T>
 int LinkQueue<T>::length()
 {
	  return m_list->length();
 }
```

### main.cpp

```c++
#include <iostream>
#include "LinkQueue.hpp"
using namespace std;

class Teacher03
{
public:
	char name[32];
	int age;
};

void main31()
{
	Teacher03 t1, t2, t3, tmp;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;

	//创建队列
	LinkQueue<Teacher03> queue;

	queue.append(t1);
	queue.append(t2);
	queue.append(t3);

	while (queue.length() > 0)
	{
		queue.retieve(tmp);
		cout << tmp.age << " ";
	}
	queue.clear();
}

void main33()
{
	main31();
	system("pause");
	return ;
}
```