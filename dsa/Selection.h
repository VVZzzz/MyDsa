#pragma once
#include "Vector.h"

//K-Selection问题(即选取从小到大排序秩为K的元素)
//算法1: 平凡思路:先对队列排序,之后选取A[k]

//算法2: 借助堆进行选取
//A: 先建立一个小顶堆,然后delMin() K次,此时的堆顶元素即为 K-Selection
//   复杂度为: O(n) + k*O(logn) = O( n + klogn) O(n)为Floyd批量建堆法

//B: 先用K个元素建立一个大根堆, 然后将剩下的(n-k)个元素插入到大根堆中,
//   每插入一个,就delMax()一次,操作完毕之后剩下的堆顶元素即为 K-Selection
//   原理是因为你去除了(n-k)个最大值之后,剩下的最大值就是 K-Selection
//   复杂度: O(k) + 2*(n-k)*O(logk) = O( k+2(n-k)logk)

//C : 借助一个大根堆和一个小根堆,如下图
/*        -------
 *       \  n-k个/
 *        \  G  /
 *         \ g /
 *          
 *         / h \
 *        /  H  \
 *       /   k个 \
 *       ---------
 */
//  一旦g<h,就交换这两个堆顶,然后各自下滤,直到g>=h
//  此时的h即为 "K-Selection" , 原理是   [..H..] <= h <= [..G..]
//  复杂度为: O(k) + O(n-k) + min(k,n-k)*2*O(logk + log(n-k))

//但这些基于堆的K-Selection,一旦K = n/2 , 就又退化为O(nlogn)

//算法思想3: 基于快速划分(构建轴点)的K选取算法,尽管最坏情况为O(n^2)
//但使用减而治之策略,比较快的
template <typename T> 
void quickSelect(Vector<T> &A, Rank K) {  
  //外层循环次数不定
  for (Rank lo = 0, hi = A.size() - 1; lo < hi; ++) {
    Rank i = lo, j = hi;
    T pivot = A[lo];
    //这个while和后面的赋值语句实际上就是partition,即构建轴点算法
    //该while时间复杂度为O(n)
    while (i < j) {
      while (i < j && pivot <= A[j]) j--;
      while (i < j && A[i] <= pivot) i++;
    }  // assert i == j
    A[i] = pivot;

    // | ... K ... |i| ... ...|
    if (K <= i) hi = i - 1;
    // | ... ... |i| ...K...|
    if (i <= K) lo = lo + 1;
  }
}

//算法思想: LinearSelect,即使最坏情况下也到达O(n),但是它的常系数太大,为200多,
//这导致实际上不用它, 只作为理论算法.
