#pragma once
#ifndef RELEASE_H_
#define RELEASE_H_

template <typename T>
struct Cleaner {
  static void clean(T x) {}
};

//偏特化
template <typename T>
struct Cleaner<T *> {
  static void clean(T *x) {
    if (x) delete x;  //如果包含指针,递归释放
  }
};

template <typename T>
void release(T x) {
  Cleaner<T>::clean(x);
}

#endif
