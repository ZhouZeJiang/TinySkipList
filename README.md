# TinySkipList
C++实现的基于跳表实现的轻量级键值型存储引擎
# 前言

本项目是使用C++实现的基于跳表实现的轻量级键值型存储引擎

Google开源的KV存储引擎LevelDB他们内部都使用了跳表这种数据结构

非关系型数据库redis核心存储引擎的数据结构就是<u>skiplist</u>（跳表）



# 理解跳表

## 跳表图示

![三层索引：最底层为0级索引，最顶层为2级索引](https://zhoumouren-image.oss-cn-shenzhen.aliyuncs.com/img/202208071529080.jpg)

## 为什么不使用数组存储数据

- 数组容量有限，申请大容量空间，未使用的空间会带来不必要的浪费；申请空间不足，容易频繁出现扩容操作，导致性能降低

- 增加元素时效率低（二分查找插入+移动元素），最坏时间复杂度达到O(n)，而数据库的存储数据操作是非常频繁的，故考虑链表

## 原始链表的不足

原始链表虽然插入和删除性能大大提升，但查找元素时间复杂度为O(n)，比如我要找key为5对应的value，路径为1-2-3-4-5，但是我利用跳表来找的话，路径就变成了1-5(数据量大的时候性能大大提升)

从跳表图示可以看出跳表就是在链表基础上改过来的，跳表一种有序多层链表，跳表的出现让插入、查找、删除元素降到了时间O(logn)，<u>比肩红黑树的效率，却比红黑树容易实现</u>

## redis为啥使用跳表而不是红黑树来实现有序集合

- 跳表更容易实现，且两者查找插入删除效率相同

- 按照范围区间查找节点的话跳表效率比红黑树高

  因为跳表只要定位到起始区间然后按顺序查找就可以了

## 跳表查找时间复杂度

跳表总高度为h = logn（底数为2），且每层比较次数其实不会超过三次，**时间复杂度**为O(3logn)，即**O(logn)**

## 跳表空间复杂度

一般来说一级索引为n/2个节点，以此类推，最高级索引元素为2个节点，所以n/2+n/4+...+2 = n-2,故**空间复杂度为O(n)**

一般的索引节点属性只要在原始链表存储一份就行，>=1级的索引节点一般只存key这样就可以，可以避免不必要的空间浪费

## 插入数据

插入数据如果键已存在，则更新值

插入数据不可能每次都重建索引，也不能不管不顾，这两种情况都会导致插入性能退化为O(n)

正常来说我们不可能精确控制到一级索引元素个数为n/2，二级索引元素个数为n/4，n/2，n/4的目的是为了让元素分布尽可能均匀到这种程度，

以便实现O(logn)时间复杂度的查找、插入等操作，这时我们可以引入**概率**

**重点：随机性插入**

插入0级索引的概率是1/2

插入1级索引的概率是1/4

插入2级索引的概率是1/8

..........................................

> 为什么插入0级索引的概率是1/2？
>
> 因为插入1级索引到顶级索引的概率总和为1/4+1/8+...=1/2，且插入1级索引时其实会顺便把小于1级的索引也给建立起来，故插入n级索引会把小于n级的索引也一起建立起来,故其实0级索引建立起来的概率为1/2+1/2=1

这样就可以解决插入数据的性能问题了

## 删除数据

查找加删除log(n)个数据的时间复杂度为O(2logn)，即O(logn)

他的删除操作如图所示

![](https://zhoumouren-image.oss-cn-shenzhen.aliyuncs.com/img/202208081500979.png)

和单链表的删除操作大致相同，不过改变next指向后，需要释放删除节点（红色部分）内存



# 项目接口

| 功能     |     函数接口      |
| -------- | :---------------: |
| 数据规模 |      size()       |
| 查找数据 |  search_node(K)   |
| 插入数据 | insert_node(K, V) |
| 删除数据 |  delete_node(K)   |
| 数据写入 |    dump_file()    |
| 加载数据 |    load_file()    |



# 运行结果

**插入查找删除**

```cpp
E:\C++\skiplist\cmake-build-debug\main.exe
Successfully inserted key:32079, value:zhou
Successfully inserted key:12116, value:mou
Successfully inserted key:5465, value:want
Successfully inserted key:22921, value:to
Successfully inserted key:2736, value:work
Successfully inserted key:50, value:please

-------skipList size-------//跳表数据规模
6

---------dump_file---------//写入dumpfile文件
50:please;
2736:work;
5465:want;
12116:mou;
22921:to;
32079:zhou;
---------search_node---------//查找节点
Found key: 50, value: please

---------search_node---------
Not Found Key:100

---------SkipList---------//跳表
Level 0: 50:please;2736:work;5465:want;12116:mou;22921:to;32079:zhou;
Level 1: 50:please;2736:work;5465:want;12116:mou;22921:to;32079:zhou;
Level 2: 50:please;5465:want;32079:zhou;
Level 3: 50:please;
Level 4: 50:please;
Level 5: 50:please;
Deleted successfully: 50
No node to delete was found: 100

---------SkipList---------
Level 0: 2736:work;5465:want;12116:mou;22921:to;32079:zhou;
Level 1: 2736:work;5465:want;12116:mou;22921:to;32079:zhou;
Level 2: 5465:want;32079:zhou;

进程已结束,退出代码0
```

**读取dumpfile数据**

```cpp
E:\C++\skiplist\cmake-build-debug\main.exe
---------load_file---------
Successfully inserted key:50, value:please
key:50    value:please
Successfully inserted key:2736, value:work
key:2736    value:work
Successfully inserted key:5465, value:want
key:5465    value:want
Successfully inserted key:12116, value:mou
key:12116    value:mou
Successfully inserted key:22921, value:to
key:22921    value:to
Successfully inserted key:32079, value:zhou
key:32079    value:zhou

进程已结束,退出代码0
```



# 性能测试

环境：win11+i7-9700K+内存32G

IDE：Clion

跳表高度：20

**插入数据性能测试**

| 插入数据规模 | 耗时（秒） |
| ------------ | ---------- |
| 10W          | 0.0385894  |
| 100W         | 0.384135   |
| 1000W        | 3.28737    |

**查找数据性能测试**

| 查找数据规模 | 耗时（秒） |
| ------------ | ---------- |
| 10W          | 0.014556   |
| 100W         | 0.105424   |
| 1000W        | 0.952653   |

