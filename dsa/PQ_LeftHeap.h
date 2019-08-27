#pragma once
//左式堆的结构
//目的: 为了提高在合并两个优先堆时的时间效率,以达到O(logn)
//合并两个堆,假设一个堆A规模为n,一个堆B为m,(m<n)以A为基准,将B合并到A上.
//方法1 : A.insert(B.delMax())
//这样会需要O(m*(log(n+m)+log(m)),即O(m*logn)时间复杂读,显然这是不能接受的
//方法2: 用Floyd建堆法进行Union(A,B).heapify()
//即把AB无序的融合在一起,在统一建堆
//这样需要O(n+m)时间复杂度,但是这样丢失了一开始的AB的偏序性,时间复杂度看来还能进一步提高

//左式堆的出现使得合并的时间复杂度降为O(logn)
//我们知道完全二叉堆实现的优先队列有俩性质: 
//1. 结构性(即完全二叉堆) 2. 优先性: 即优先队列的性质
//左式堆的出现,不要结构性,最重要的是优先性

//首先是空节点路径长度npl(Nullnode Path Length)
//若x节点是空节点,npl(x) = 0
//npl(x) = 1 + min( npl(x->lc) + npl(x->rc) )
//可以发现这和某个节点的高度的递推式很像,只是将max换为min.
//也可以发现 npl(x)等于以x为根节点的满二叉树的高度.
/*
 *           8
 *       6       4
 *     3   2
 *   null  null
 */
//此时节点8的npl就是 8 6 4这个满二叉树的高度为2

//左式堆的要求: npl(x->lc) >= npl(x->rc)
//即左子树的npl要不小于右子树的npl

//右侧连: 沿着这个堆的最右子节点一直向下
/*
 *        X
        /   \
       /     \
      /       \
      -------- *
     /        /
    /        /
   /________/
 *   
 */
//即从X到*的那一条链,如果这条链高度为d,则这个左式堆至少包含2^d - 1个内部节点,2^(d+1)-1个节点
//也就是说,规模为n的左式堆中,右侧连长度d = O(logn)


//结构实现: 左式堆结构的优先队列我们用 "二叉树"结构实现
#include "BinTree.h"
#include "PQ.h"
#include "comparator.h"
template <typename T>
class PQ_LeftHeap : public PQ<T>, public BinTree<T> {
 public:
  PQ_LeftHeap() {}
  void insert(T);  //按照比较器确定的优先级次序插入词条
  //取出优先级最高的词条
  T getMax() { return _root->data; }
  T delMax();      //删除优先级最高的词条

 private:
   //合并两个左式堆的merge算法
  BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b);
  
  
};
