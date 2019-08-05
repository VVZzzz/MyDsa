#ifndef BTREE_H_
#define BTREE_H_
#pragma once
#include "BTNode.h"
//该文件是BTree类
//m阶B树: 对于根节点 2<=n+1 . 对于其他内部节点: [m/2](↑取整)<=n+1
//且n+1<=m
//n:     是该节点的关键码数
//故亦称([m/2],m)树,对于(2,4)树很特殊
//B树高度: 由"外部节点"的深度而决定

//引入B树的目的: 减少I/O次数
//做法: B树的一个节点内有多个关键码,且有多个分支数,这使得一次查找就可以将多个关键码读取进来.
//可以认为B树的节点都存在硬盘(同一级存储层次)上,去查找一次要尽可能减少查找次数并尽可能多将数据读入内存.
//例子: 比如有1G个记录,对于AVL要查找log(2,10^9)次IO操作.且每次只能读入一个关键码
//而对于B树,每个节点都存一组关键码.一组关键码多大由磁盘数据块大小而定,设为几个KB,一个关键码为4B
//则一个节点关键码有200-300个,设为256个.则: 查找相同的数据只需要log(256,10^9) = 4次I/O
//4次I/O 明显优于 30次I/O

//示例(3,5)树:
/*								[53,75]

		[19,36]			               [63,69]		           [84,92]

[13,17] [27,31] [38,41,49,51]   [57,59] [65,66] [71,73]		[77,79] [89,91] [93,97,99]
*/
//失败查找必终止于外部节点
//外部节点也可以是下一级的存储结构

//B树的最大高度分析: 共含N个关键码的m阶B树,最高高度为多少?
//设树根节点所在深度为第0层,外部节点所在深度为第h层.
//内部节点应该尽可能的"瘦",故根节点有1个结点,第1层有2个节点,以此类推,第2层有,2*[m/2]个节点
//第k层有2*[m/2]^(k-1)层节点.
//我们又知道: 内部节点共N个关键码,那么外部节点(即最后一层)共有N+1个节点.
//理解: N个内部节点对应N种查找成功,那么一定会有N+1种失败的情况.
//故有: 第h层最少有2*[m/2]^(h-1) <= N+1
//h <= 1 + log( [m/2],L(N+1)/2| ) = O(log(m,N))
//对比BBST树: log([m/2],N/2) / log(2,N) = 1/(log(2,m)-1)
//取m = 256, B树高度降低为1/7

//B树的最小高度分析: 共含N个关键码的m阶B树,最低高度为多少?
//设树根节点所在深度为第0层,外部节点所在深度为第h层.
//内部节点应该尽可能的"胖",故根节点有1个,第1层有m个,第2层有m^2个,以此类推
//第k层有m^k个.
//第h层有: N+1 <= m^h  
//h >= log(m,(N+1)) = Ω(log(m,N))
//相对于:(log(m,N)-1)/log(2,N) = 1/log(2,m)
//取m=256,树高降低1/8

//由于B树树高上界下界都是log(m,N),故B树高度变化幅度十分有限.

template <typename T>
class BTree {
 protected:
  int _size;                           //存放的关键码数
  int _order;                          // B树的阶次,至少为3
  BTNodePosi(T) _root;                 //根节点
  BTNodePosi(T) _hot;                  // search最后访问的非空节点
  void solveOverflow(BTNodePosi(T));   //因插入而上溢之后的分裂处理
  void solveUnderflow(BTNodePosi(T));  //因删除而下溢之后的合并处理
 public:
  BTree(int order = 3) : _order(order), _size(0) { _root = new BTNode<T>(); }
  ~BTree() {
    if (_root) release(_root);
  }
  int const order() { return _order; }
  int const size() { return _size; }
  BTNodePosi(T) & root() { return _root; }  //树根
  bool empty() const { return !_root; }     //判空
  BTNodePosi(T) search(const T& e);         //查找
  bool insert(const T& e);                  //插入
  bool remove(const T& e);                  //删除
};
#endif
