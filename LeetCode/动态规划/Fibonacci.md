## 斐波那契数列

### 递归实现

递归是低效的，出现了大量重复的计算。

```c++
#include <iostream>
#include <ctime>

using namespace std;

int num = 0;

int fib( int n ){

    num ++;

    if( n == 0 )
        return 0;

    if( n == 1 )
        return 1;

    return fib(n-1) + fib(n-2);
}

int main() {

    num = 0;

    int n = 42;
    time_t startTime = clock();
    int res = fib(n);
    time_t endTime = clock();

    cout<<"fib("<<n<<") = "<<res<<endl;
    cout<<"time : "<<double(endTime-startTime)/CLOCKS_PER_SEC<<" s"<<endl;
    cout<<"run function fib() "<<num<<"times."<<endl;

    return 0;
}
```

### 迭代实现

使用记忆化搜索的思想，将计算结果保存到vector中，避免重复的计算。

```c++
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

vector<int> memo;
int num = 0;

// 记忆化搜索，自上向下的解决问题
int fib( int n ){

    num ++;

    if( n == 0 )
        return 0;

    if( n == 1 )
        return 1;

    if( memo[n] == -1 )
        memo[n] = fib(n-1) + fib(n-2);

    return memo[n];
}

int main() {

    num = 0;

    //int n = 42;
    int n = 1000;
    memo = vector<int>(n+1,-1);

    time_t startTime = clock();
    int res = fib(n);
    time_t endTime = clock();

    cout<<"fib("<<n<<") = "<<res<<endl;
    cout<<"time : "<<double(endTime-startTime)/CLOCKS_PER_SEC<<" s"<<endl;
    cout<<"run function fib() "<<num<<"times."<<endl;

    return 0;
}
```

### 动态规划

自下向上的解决问题

```c++
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

// 动态规划
int fib( int n ){

    vector<int> memo(n+1, -1);

    memo[0] = 0;
    memo[1] = 1;
    for( int i = 2 ; i <= n ; i ++ )
        memo[i] = memo[i-1] + memo[i-2];

    return memo[n];
}

int main() {

    int n = 1000;

    time_t startTime = clock();
    int res = fib(n);
    time_t endTime = clock();

    cout<<"fib("<<n<<") = "<<res<<endl;
    cout<<"time : "<<double(endTime-startTime)/CLOCKS_PER_SEC<<" s"<<endl;

    return 0;
}
```