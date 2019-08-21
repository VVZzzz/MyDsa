#pragma once
//优先队列,完全二叉堆(用vector实现)
//若考虑单纯vector实现: insert:O(n),getmax:O(n),delmax:O(n)
//若考虑有序vector实现: insert:O(n),getmax:O(1),delmax:O(1)
//若考虑用list实现: insert: O(1),getmax: O(n),delmax:O(1)
//若考虑有序list实现: insert:O(n),getmax:O(1),delmax:O(1)
//若考虑用BBST(AVL,SPlay,Red-BlackTree),insert: O(logn),getmax: O(1),delmax:O(logn)
//但是BBST的功能远远超出了PQ的需求,杀鸡用牛刀
//因为PQ只需要对优先级最大的那个节点操作,不必维护全序,只需偏序即可

//故我们逻辑结构: 使用完全二叉树, 但是物理结构(实现): 使用一个Vector
//insert: O(logn),getmax: O(1),delmax:O(logn)
//优先队列,父节点的优先级总是比它的孩子优先级大!
/*
 *                  10[0]
 *               /         \
 *              8[1]        6[2]
 *             /    \       /  \
 *            5[3]   3[4]  4[5] 2[6]
 *          /
 *         1[7]
 * 
 *       vec: 10 8 6 5 4 3 2 1
*/


#include "Vector.h"
#include "PQ.h"
#include "comparator.h"
template <typename T>
class PQ_ComplHeap : public PQ<T>, public Vector<T> {  //完全二叉堆
  //多继承,有来自vector的私有成员T *_ele
 protected:
  Rank percolateDown(Rank n, Rank i);  //对前n个词条中的第i个进行下滤,i<n
  Rank percolateUp(Rank i);  //对第i个词条进行上滤 i<_size
  void heapify(Rank n);      //Floyd建堆法
 public:
   PQ_ComplHeap() { } //默认构造
   PQ_ComplHeap ( T* A, Rank n ) { copyFrom ( A, 0, n ); heapify ( n ); } //批量构造
   void insert ( T ); //按照比较器确定的优先级次序，插入词条,O(logn)
   T getMax() { return _elem[0]; }  //读取优先级最高的词条,O(1)
   T delMax(); //删除优先级最高的词条,O(logn)
  private:
//properparent是为了下滤的赋值策略用的工具函数
//即将val当作PQ[i]而去进行比较
   Rank properparentV(Rank n, Rank i, T val);

};

#define  InHeap(n, i)      ( ( ( -1 ) < ( i ) ) && ( ( i ) < ( n ) ) ) //判断PQ[i]是否合法
#define  Parent(i)         ( ( i - 1 ) >> 1 ) //PQ[i]的父节点（floor((i-1)/2)，i无论正负）
#define  LastInternal(n)   Parent( n - 1 ) //最后一个内部节点（即末节点的父亲）
#define  LChild(i)         ( 1 + ( ( i ) << 1 ) ) //PQ[i]的左孩子
#define  RChild(i)         ( ( 1 + ( i ) ) << 1 ) //PQ[i]的右孩子
#define  ParentValid(i)    ( 0 < i ) //判断PQ[i]是否有父亲
#define  LChildValid(n, i) InHeap( n, LChild( i ) ) //判断PQ[i]是否有一个（左）孩子
#define  RChildValid(n, i) InHeap( n, RChild( i ) ) //判断PQ[i]是否有两个孩子
#define  Bigger(PQ, i, j)  ( lt( PQ[i], PQ[j] ) ? j : i ) //取大者（等时前者优先）
#define  ProperParent(PQ, n, i) /*父子（至多）三者中的大者*/ \
            ( RChildValid(n, i) ? Bigger( PQ, Bigger( PQ, i, LChild(i) ), RChild(i) ) : \
            ( LChildValid(n, i) ? Bigger( PQ, i, LChild(i) ) : i \
            ) \
            ) //相等时父节点优先，如此可避免不必要的交换

//BiggerV是为了下滤的赋值策略用的工具函数
//即将val当作PQ[i]而去进行比较
#define  BiggerV(PQ, i, j,val)  ( lt( val, PQ[j] ) ? j : i ) //取大者（等时前者优先）
