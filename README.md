# MyDsa
DataStructures-Algorithms -cpp
本项目用c++实现常用的数据结构模板类和一些算法,并作了测试
## 已实现数据结构模板类
1. 数组MyVector
   - 构造,拷贝,析构
   - operator [ ] = 
   - pop_back
   - push_back
   - 迭代器与迭代器操作:begin(), end(), back(), erase(), insert(), resize()等
   - expand,shrink
   - 排序算法(快排,冒泡,选择,归并,堆排序)
2. (双向)链表MyList
   - 构造,拷贝,析构
   - operator =
   - push_back
   - push_front
   - insert
   - erase
   - pop_back()
   - pop_front()
   - 迭代器:back front() 反向迭代器
   - 懒惰操作:lazy_erase,懒惰删除.
3. 二叉查找树BinarySearchTree
   - 构造,拷贝,析构
   - insert(),remove()
   - findMax(),findMin(),contains()
   - remove(),remove_leaves(),makeEmpty()
   - countNodes(),countLeaves(),countFullNodes()
   - 前中后递归遍历
   - 前中后非递归遍历
   - 层序遍历
   - 懒惰操作:lazy_insert(),lazy_remove(),lazy_findMin...
   - 判断是否为BST函数
4. 平衡查找树AVL
   - 构造,拷贝,析构函数
   - 插入节点 insert(data)
   - 单旋,双旋singleRotate doubleRotate()
   - 右(左)子树过高调整函数rightBalance()
   - 删除节点 remove(data)
   - 得到高度 getHeight()
   - 判断是否平衡isBalanced()
   - 生成最小高度H的AVL树函数 generate_min_H()
5. Hash散列表
   - Hash分离链接法散列表
   - 线性探测解决冲突
   - 平方探测解决冲突
   - 双散列法解决冲突
   - 构造,析构,拷贝
   - insert(x)
   - remove(x)
   - makeEmpty()
   - rehash()
   - contains(x)
6. 二叉堆
   - 含参(vector)构造函数
   - insert
   - deleteMin
   - remove
   - increaseKey
   - decreaseKey
7. 左式堆
   - 构造,拷贝,析构
   - insert() merge()
   - findMin() deleteMin()
   - buildHeap(vector<T>)建堆策略
   - lazyDeleteMin懒惰策略
8. SkipList
   - size()
   - level()//层高
   - put(K, V);  //插入（注意与Map有别——Skiplist允许词条重复，故必然成功）
   - get(K k);  //读取
   - remove(K k);  //删除
9. B树
   - 增,删,改,查
10. Huffman编码树
11. 优先队列
