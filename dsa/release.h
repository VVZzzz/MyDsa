#pragma once
#ifndef RELEASE_H_
#define RELEASE_H_

template <typename T>
struct Cleaner {
  static void clean(T x) {}
};

//ƫ�ػ�
template <typename T>
struct Cleaner<T *> {
  static void clean(T *x) {
    if (x) delete x;  //�������ָ��,�ݹ��ͷ�
  }
};

template <typename T>
void release(T x) {
  Cleaner<T>::clean(x);
}

#endif
