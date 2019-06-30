#ifndef AVL_H_
#define AVL_H_
#pragma once
#include "BST.h"

template <typename T>
class AVL : public BST<T> {
 public:
  BinNodePosi(T) insert(const T &e);  //插入某个值
  bool remove(const T &e);  //删除某个值
  //其余接口可沿用
};

#include "BSTImpletation.h"
#endif
