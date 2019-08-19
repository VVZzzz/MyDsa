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
/*
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          /
 *         1
 * 
 *       vec: 10 8 6 5 4 3 2 1
 */

#include "Vector.h"
#include "PQ.h"
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
   void insert ( T ); //按照比较器确定的优先级次序，插入词条
   T getMax(); //读取优先级最高的词条
   T delMax(); //删除优先级最高的词条

};
