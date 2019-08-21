#pragma once
#include "PQ_ComplHeap.h"
/*
 * 完全二叉堆的插入:
 * 首先先将要插入的元素添加至_ele数组末尾,接着让它上滤
 * eg:  插入11
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   11
 *
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           11   3  4   2
 *          / \
 *         1   5
 *                 11
 *               /  \
 *             10    6
 *            /  \  /  \
 *           8   3  4   2
 *          / \
 *         1   5
 */
template <typename T>
void PQ_ComplHeap<T>::insert(T e) {
  Vector<T>::insert(e);
  percolateUP(_size - 1);
}

//上滤操作,如上图,但是这里为了提高效率,我们不用交换,而是直接赋值
//这样就可以达到logn + 1 而不是 3logn(交换需要3步)
/*
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   11
 *                 10
 *               /  \
 *        (此时8与11比较,若小于则将8赋给下面的5)
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   5
 */
template <typename T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i) {
  /*  //交换策略,3logn
  while (ParentValid(i)) {  //如果i有父亲,尚未达到顶
    int j = Parent(i);
    if (lt(_elem[i],_elem[j])) break;  //如果儿子小于父亲,则break
    swap(_elem[i], _elem[j]);   //否则交换
    i = j;
  }
  */
  //赋值策略,logn+1
  int dst = _elem[i];
  while (ParentValid(i)) {
    int j = Parent(i);
    if (lt(dst, _elem[j])) {
      _elem[i] = dst;
      break;
    }
    _elem[i] = _elem[j];
    i = j;
  }
}


//删除操作,只能删除根节点(即最大)
//具体算法是将堆末尾的元素赋给堆顶元素,然后使他下滤即可
template <typename T>
T PQ_ComplHeap<T>::delMax() {
  T maxElem = _elem[0];
  _elem[0] = _elem[--_size];  //摘除堆顶
  percolateDown(_size, 0);   //从新根执行下滤
  return maxElem;
}

template <typename T>
inline Rank PQ_ComplHeap<T>::properparentV(Rank n, Rank i, T val) {
  if (RChildValid(n, i)) {
    if (val < _elem[LChild(i)])
      return Bigger(_elem, LChild(i), RChild(i));
    else
      return BiggerV(_elem, i, RChild(i), val);
  } else {
    if (LChildValid(n, i)) return BiggerV(_elem, i, LChild(i), val);
    else
      return i;
  }
}

/*
 * 删除10
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   0
 *                 0
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / 
 *         1   
 * 下滤,由 儿子中最大的那个替换0,即max(8,6)替换0
 *                 8
 *               /  \
 *              0    6
 *            /  \  /  \
 *           5   3  4   2
 *          / 
 *         1   
 */
template<typename T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i) {
  /*  交换策略
  Rank j;
  //只要i不等于,_elem[i]和他儿子们三者中最大的那个下标
  //即只要子节点优先级大于父节点
  while (i!=(j=ProperParent(_elem,n,i))) {
    swap(_elem[i], _elem[j]);//交换策略
    i = j;  //此时j就是两个儿子中最大的那个
  }
  return i;  //返回下滤抵达的位置（亦i亦j）
  */

  //赋值策略
  Rank j;
  int val = _elem[i];
  while (i!=(j=properparentV(_elem,n,i,val))) {
    _elem[i] = _elem[j]
    i = j;  //此时j就是两个儿子中最大的那个
  }
  _elem[i] = val;
  return i;
}

//floyd建堆法,批量建堆,即给你一个数组,让你用他们建立一个完全二叉堆(优先队列)
//平凡思路是: 每次都调用insert,这样每次都要从堆顶上滤,且会引起_elem的扩容
//floyd建堆是: 先拷贝到_elem中,接着从最后一个非叶子节点开始下滤,直至根节点
//时间复杂度: O(n),因为完全二叉堆的叶子节点占了一半,并且很少节点会一直下滤到底
/*
 *
 * 从4开始依次下滤
 *                 8
 *               /  \
 *             16    6
 *            /  \  /  \
 *           *4    9 5   20
 *          / 
 *         14   
 *                 8
 *               /   \
 *             16     6*
 *            /  \   /  \
 *           14    9 5   20
 *          / 
 *         4   
 *                 8
 *               /   \
 *             *16     20
 *            /  \   /  \
 *           14    9 5   6
 *          / 
 *         4   
 *                 *20
 *               /   \
 *             16     8
 *            /  \   /  \
 *           14    9 5   6
 *          / 
 *         4   
 */
template <typename T>
void PQ_ComplHeap<T>::heapify(Rank n) {
  
}
