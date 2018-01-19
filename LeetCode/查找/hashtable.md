### 249. Intersection of Two Arrays

给定两个数组nums，求两个数组的公共元素。

方法1：遍历数组1，将数组1的元素插入到set集合中record，遍历数组2，查找数组2的每一个元素在set集合record是否存在，存在则将公共元素插入到set集合resultSet中。

```c++
vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {

    set<int> record; // unordered_set
    for( int i = 0 ; i < nums1.size() ; i ++ )
        record.insert(nums1[i]);

    set<int> resultSet;
    for( int i = 0 ; i < nums2.size() ; i ++ )
        if( record.find( nums2[i] ) != record.end() )
            resultSet.insert( nums2[i] );

    vector<int> resultVector;
    for(set<int>::iterator iter = resultSet.begin() ; iter != resultSet.end() ; iter ++ )
        resultVector.push_back( *iter );

    return resultVector;
}
```

方法2

```c++
vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {

    set<int> record(nums1.begin(), nums1.end());

    set<int> resultSet;
    for( int i = 0 ; i < nums2.size() ; i ++ )
        if( record.find( nums2[i] ) != record.end() )
            resultSet.insert( nums2[i] );

    return vector<int>(resultSet.begin(), resultSet.end());
}
```

哈希表的缺点是失去了数据的顺序性，在C++语言中map和set的底层实现为平衡二叉树，unordered_map和unordered_set的底层实现为哈希表。

### 350. Intersection of Two Arrays

给定两个数组nums，求两个数组的交集。

```c++
vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {

    map<int, int> record; // unordered_map
    for( int i = 0 ; i < nums1.size() ; i ++ )
        record[nums1[i]] += 1;

    vector<int> resultVector;
    for( int i = 0 ; i < nums2.size() ; i ++ )
        if( record[ nums2[i] ] > 0 ){
            resultVector.push_back( nums2[i] );
            record[nums2[i]] --;
        }

    return resultVector;
}
```

方法2

```c++
vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {

    map<int, int> record;
    for( int i = 0 ; i < nums1.size() ; i ++ )
        if( record.find(nums1[i]) == record.end() )
            record.insert( make_pair(nums1[i],1));
        else
            record[nums1[i]] += 1;

    vector<int> resultVector;
    for( int i = 0 ; i < nums2.size() ; i ++ )
        if( record.find(nums2[i]) != record.end() &&
            record[ nums2[i] ] > 0 ){
            resultVector.push_back( nums2[i] );
            record[nums2[i]] --;
            if( record[nums2[i]] == 0 )
                record.erase( nums2[i] );
        }

    return resultVector;
}
```

### 1. Two Sum

给出一个整型数组nums。返回这个数组中两个数字的索引值i和j，使得nums[i] + nums[j] 等于一个给定的target值。两个索引不能相等。

- 如 nums = [2, 7, 11, 15], target = 9
- 返回 [0, 1] 

方法1：排序后，使用双索引对撞。

方法2：查找表。将所有元素放入查找表，之后对于每一个元素a，查找 target - a 是否存在。

```c++
// 一次性把所有的元素放入map中会导致相同的元素覆盖的问题
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int,int> record; // 键是数组元素，值是元素在数组中的索引。
    for( int i = 0 ; i < nums.size() ; i ++ ){
        int complement = target - nums[i];
        if( record.find(complement) != record.end() ){
            int res[] = {i, record[complement]};
            return vector<int>(res, res + 2);
        }
        record[nums[i]] = i;
    }
    throw invalid_argument("the input has no solution");
}
```

### 454. 4Sum II

给出四个整形数组A,B,C,D，寻找有多少i,j,k,l的组合，使得A[i] + B[j] + C[k] + D[l] == 0。其中，A,B,C,D中均含有相同的元素个数N，且0<=N<=500。

方法1：使用一个map

将C+D的每一种可能放入查找表：O(n^2)

```c++
int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {

    assert( A.size() == B.size() && B.size() == C.size() && C.size() == D.size() );
    unordered_map<int,int> hashtable;
    for( int i = 0 ; i < C.size() ; i ++ )
        for( int j = 0 ; j < D.size() ; j ++ )
            hashtable[C[i]+D[j]] += 1;

    int res = 0;
    for( int i = 0 ; i < A.size() ; i ++ )
        for( int j = 0 ; j < B.size() ; j ++ )
            if( hashtable.find(-A[i]-B[j]) != hashtable.end() )
                res += hashtable[-A[i]-B[j]];

    return res;
}
```

方法2：使用2个map

将A+B和C+D的每一种可能放入两个查找表：O(n^2)

```c++
int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {

    unordered_map<int,int> hashtable1;
    unordered_map<int,int> hashtable2;

    for( int i = 0 ; i < A.size() ; i ++ )
        for( int j = 0 ; j < B.size() ; j ++ )
            hashtable1[A[i]+B[j]] += 1;

    for( int i = 0 ; i < C.size() ; i ++ )
        for( int j = 0 ; j < D.size() ; j ++ )
            hashtable2[C[i]+D[j]] += 1;

    int res = 0;
    for( unordered_map<int,int>::iterator iter = hashtable1.begin() ; iter != hashtable1.end() ; iter ++ )
        if( hashtable2.find(-(iter->first)) != hashtable2.end() )
            res += iter->second * hashtable2[-(iter->first)];

    return res;
}
```

### 447. Number of Boomerangs

给出一个平面上的n个点，寻找存在多少个由这些点构成的三元组（i, j, k），使得i,  j两点的距离等于i, k两点的距离。其中n最多为500，且所有的点坐标的范围在[-10000, 10000]之间。

- 如[ [0,0] , [1,0] , [2,0] ]，则结果为2
- 两个结果为 [ [1,0], [0,0], [2,0] ] 和 [ [1,0], [2,0], [0,0] ]

观察到 i 是一个“枢纽”，对于每个点i，遍历其余点到i的距离，O(n^2)

```c++
int numberOfBoomerangs(vector<pair<int, int>>& points) {
    int res = 0;
    for( int i = 0 ; i < points.size() ; i ++ ){
        // record中存储 点i 到所有其他点的距离出现的频次
        unordered_map<int, int> record;
        for( int j = 0 ; j < points.size() ; j ++ )
            if( j != i )
                record[dis(points[i], points[j])] += 1;
        for( unordered_map<int, int>::iterator iter = record.begin() ; 
            iter != record.end() ; iter ++ )
          	// if( iter->second >= 2 )
            res += (iter->second)*(iter->second-1);
    }
    return res;
}

// 两点间的距离有可能是一个浮点型，对于浮点型的距离由于浮点型的误差，
// 用查找表这种准确的查找是不安全的，因此计算距离的公式不开根号以避免浮点型的误差
int dis( const pair<int,int> &pa, const pair<int,int> &pb){
    return (pa.first - pb.first) * (pa.first - pb.first) +
           (pa.second - pb.second) * (pa.second - pb.second);
}
```

### 219. Contains Duplicate II

给出一个整形数组nums和一个整数k，是否存在索引i和j，使得nums[i] == nums[j] 且i和j之间的差不超过k。

滑动窗口 + 查找表

```c++
bool containsNearbyDuplicate(vector<int>& nums, int k) {
    if( nums.size() <= 1 )
        return false;
    if( k <= 0 )
        return false;
    unordered_set<int> record;
    for( int i = 0 ; i < nums.size() ; i ++ ){
        if( record.find( nums[i] ) != record.end() )
            return true;
        record.insert( nums[i] );
        // 保持record中最多有k个元素
        // 因为在下一次循环中会添加一个新元素,使得总共考虑k+1个元素
        if( record.size() == k + 1 )
            record.erase( nums[i-k] );
    }
    return false;
}
```

### 220. Contains Duplicate III  

给出一个整形数组nums，是否存在索引i和j，使得nums[i]和nums[j]之间的差别不超过给定的整数t，且i和j之间的差别不超过给定的整数k。

```c++
bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
    set<long long> record;
    for( int i = 0 ; i < nums.size() ; i ++ ){
      	// lower_bound(nums[i]-t) 寻找大于nums[i]-t的数中最小的值
        if( record.lower_bound( (long long)nums[i] - (long long)t ) != record.end() &&
            *record.lower_bound( (long long)nums[i] - (long long)t ) <= (long long)nums[i] + (long long)t )
            return true;
        record.insert( nums[i] );
        if( record.size() == k + 1 )
            record.erase( nums[i-k] );
    }
    return false;
}
```