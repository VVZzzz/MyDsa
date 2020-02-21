#pragma once
#include "Vector.h"

//选取众数算法
//众数是指在任一无序向量中 元素重复次数 大于 整体元素个数的一半.
//即 > [n/2] ,注意这里没有＝号

//众数还有个性质 , 众数一定是中位数,而中位数不一定是众数
//如何不用排序,且空间复杂度为O(1),时间复杂度为O(n) ,判断众数是否存在
//并选取众数?

template <typename T>
bool majEleCheck(Vector<T> A, T maj) {
  int c = 0;
  for (int i = 0; i < A.size(); i++)
    if (A[i] == maj) c++;
  return 2 * c > A.size();
}

//选出众数候选者,减而治之策略
/*
 * | ..... A ..... |
 * | P | ...A-P... |
 * 对于长度为2m的子序列P,若元素x在P序列中恰好出现m次.则A有众数仅当后缀A-P有
 * 众数,且A-P的众数就是A的众数 (即是必要条件)
 *
 * 若A的众数就是x,则减去P,A的众数元素个数减少量 = 与非众数元素个数减少量.
 * 即A-P中众数元素个数 与 非众数元素个数差值仍然相等.
 *
 * 若A的众数y≠x,则减去P,A中的众数元素减少个数 <= 非众数元素减少个数.
 * 即A-P中众数元素个数 与 非众数元素个数差值不会缩小.
 *
 * 所以我们用c变量记录,众数候选者个数与其他元素的个数差值
 */
template <typename T>
T majEleCandidate(Vector<T> A) {
  T maj;
  // 线性扫描：借助计数器c，记录maj与其它元素的数量差额
  for (int i = 0, c = 0; i < A.size(); i++) {
    if (c == 0) { //每当c归零，都意味着此时的前缀P可以剪除
      maj = A[i];
      c = 1;  //众数候选者改为新的当前元素
    } else {
      maj == A[i] ? c++ : c--;  //相应地更新差额计数器
    }
  }
  return maj;  //至此,若原序列有众数, 则只能是maj - 反之不然(因为只是必要条件)
}

template <typename T>
bool majority(Vector<T> A, T &maj) {
  maj = majEleCandidate(A);  //必要性,选出众数候选者
  return majEleCheck(A, maj);  //充分性,检查该候选者是否是众数
}


