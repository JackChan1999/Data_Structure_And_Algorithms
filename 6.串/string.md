### string.h

```c
#ifndef _STRING_H_
#define _STRING_H_

typedef struct string
{
	char * str;
	int length;
}String;

void strAssign(String* s, char* p); //将p串的值赋值给s
int strLength(String* s); //求串的长度
int strEqual(String* s, String* t);//判断两个串是否相等
int strCompete(String* s, String* t); //比较两串大小
void strConnect(String* s, String* t); //连接两个字符串
void strCopy(String* s, String* t); //将串t复制到串s中
void Insert(String *s, int pos, char* p); //在pos位置插入串p
void Delete(String* s, int pos, int len); //删除pos位置及后面长度为len的串

void print(String* s); //打印串

#endif
```

### string.c

```c
#include <stdio.h>

typedef struct string
{
	char * str; //指向串的指针
	int length; //串的长度
}String;
void strAssign(String* s, char* p) //赋值
{
	//先计算p串的长度，然后按照这个长度为s中的str开辟空间
	int i = 0;
	while (p[i] != '\0')
		i++;
	s->str = (char*)malloc((i + 1)*sizeof(char));
	//然后将p串赋值给s串
	int j = 0;
	for (j = 0; j <= i; j++)
		s->str[j] = p[j];
	s->str[j + 1] = '\0'; //末尾赋值\0
	s->length = i; //串长度记录
}
int strLength(String* s) //求串的长度
{
	return s->length;
}
void strCopy(String* s, String* t) //将串t复制到串s中
{
	//重新为s串分配内存，大小等于t串
	s->str = (char*)malloc((t->length)*sizeof(char));
	memset(s->str, '\0', t->length + 1);
	for (int i = 0; i <= t->length; i++)
		s->str[i] = t->str[i];
}
int strEqual(String* s, String* t) //判断两串是否相等
{
	if (s->length == t->length)
	{
		while (*(s->str++) != *(t->str++))
		{
			printf("两个串不相等！\n");
			return 0;
		}
		printf("两个串相等！\n");
		return 1;
	}
	printf("两个串不相等！\n");
	return 0;
}
void strConnect(String* s, String* t) //连接两个串
{
	int len = s->length + t->length;
	char* temp = (char*)malloc((s->length + t->length)*sizeof(char)); //申请缓存
	memset(temp, '\0', s->length);
	int i, j;
	for (i = 0; i < s->length; i++) //将s串先存入缓存中
		temp[i] = s->str[i];
	for (j = 0; j < t->length; j++, i++) //再将t串存入进来
		temp[i] = t->str[j];
	temp[i] = '\0';
	s->str = temp; //将s串指向temp
	s->length = s->length + t->length;
}

int strCompete(String* s, String* t) //比较串大小
{
	if (s && t) //传入的两个串都不为空（健壮性判断）
	{
		int sl = s->length, tl = t->length; //记录两个串的长度
		while (*(s->str) != '\0' && *(t->str) != '\0')
		{
			if (*(s->str) < *(t->str))
				return -1;
			if (*(s->str) == *(t->str))
			{
				s->str++;
				t->str++;
			}
			if (*(s->str) > *(t->str))
				return 1;
			s->str++;
			t->str++;
		}
		//如果循环结束，即有一个串已经到尾部
		//那么这种情况，必有一个是子串一个是主串
		//此时比较两个串的长度大小
		if (sl < tl)
			return -1;
		if (sl == tl)
			return 0;
		if (sl > tl)
			return 1;
	}
}
void Insert(String *s, int pos, char* p) //插入串
{
	if (pos < 0 || pos > s->length)
		return;
	//先求出串p的长度
	int plen = 0;
	while (p[plen] != '\0')
	{
		plen++;
	}
	//循环结束后，plen就记录了串p的长度
	//分配一个临时缓存，将串s与串p存入进来
	char* temp = (char*)malloc((plen + s->length)*sizeof(char));
	int i;
	for (i = 0; i < pos; i++) //将s串pos位置前的存入进来
	{
		temp[i] = s->str[i];
	}
	//然后将p串存入到pos位置后
	int j = 0;
	while (p[j] != '\0')
	{
		temp[i] = p[j];
		i++, j++;
	}
	//将p串插入进来后，接着将剩下的s串补到后面
	for (; pos < s->length; pos++)
	{
		temp[i] = s->str[pos];
		i++;
	}
	temp[i] = '\0';
	s->str = temp;
	s->length = s->length + plen;
	printf("插入之后：%s\n", temp);
}
void Delete(String* s, int pos, int len) //删除某一个子串
{
	if (pos < 0 || pos > s->length || len > s->length - pos)
		return;
	//先创建一个缓存，大小为s->length - len
	char* temp = (char*)malloc((s->length - len)*sizeof(char));
	int i;
	for (i = 0; i < pos; i++) //将s串中pos位置前的串复制到temp缓存中
	{
		temp[i] = s->str[i];
	}

	int j = pos + len;
	for (; j < s->length; j++)//将去掉子串后面剩余的串复制到temp中
	{
		temp[i] = s->str[j];
		i++;
	}
	temp[i] = '\0';
	s->str = temp;
	s->length = s->length - len;
	printf("删除子串后：%s\n", temp);
}
void print(String* s)
{
	if (s)
		printf("%s\n", s->str);
}
```

### main.c

```c
#define _CRT_SECURE_NO_WARNINGS
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int main()
{
	String s;
	char arr[1024];
	printf("请输入要赋值给s的串：\n");
	scanf("%s", arr);
	strAssign(&s, arr);

	printf("s:");
	print(&s);

	printf("长度：%d\n", strLength(&s));

	//再创建一个串t，来比较两个串的大小
	String t;
	printf("请输入要赋值给t的串：\n");
	scanf("%s", arr);
	strAssign(&t, arr);
	printf("t:");
	print(&t);
	printf("长度：%d\n", strLength(&t));

	printf("判断两个串是否相等：");
	strEqual(&s, &t);
	
	printf("比较两个串大小：");
	int ret = strCompete(&s, &t); //比较大小
	if (ret < 0)
		printf("串t较大！\n");
	if (ret == 0)
		printf("两个串一样大！\n");
	if (ret > 0)
		printf("串s较大！\n");

	//将两个串连接起来
	printf("将串s与t连接起来：");
	strConnect(&s, &t); //返回值为char*类型
	print(&s);
	printf("连接两个串后，s的长度：%d\n", strLength(&s));
	//将串s复制到串t中
	//strCopy(&s, &t); //将串t复制到串s中
	//printf("将串t复制到串s，则串s的值：");
	//print(&s);

	//在串s中插入一个串
	printf("请输入要插入的子串：\n");
	scanf("%s", arr);
	Insert(&s, 3, arr);
	printf("插入串后，s的长度：%d\n", strLength(&s));

	//删除串s中的某一个子串
	Delete(&s, 3, 5);
	printf("删除子串后，s的长度：%d\n", strLength(&s));

	system("pause");
	return 0;
}
```