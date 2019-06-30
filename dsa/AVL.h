#ifndef AVL_H_
#define AVL_H_
#pragma once
#include "BST.h"

template <typename T>
class AVL : public BST<T> {
 public:
  BinNodePosi(T) insert(const T &e);  //����ĳ��ֵ
  bool remove(const T &e);  //ɾ��ĳ��ֵ
  //����ӿڿ�����
};

#include "BSTImpletation.h"
#endif
