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

template <typename T>
T majEleCandidate(Vector<T> A) {
  T maj;
  for (int i = 0, c = 0; i < A.size(); i++) {
    if (c == 0) {
      maj = A[i];
      c = 1;
    } else {
      maj == A[i] ? c++ : c--;
    }
  }
  return maj;
}

template <typename T>
bool majority(Vector<T> A, T &maj) {
  maj = majEleCandidate(A);  //必要性,选出众数候选者
  return majEleCheck(A, maj);  //充分性,检查该候选者是否是众数
}


