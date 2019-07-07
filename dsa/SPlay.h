#ifndef SPLAY_H_
#define SPLAY_H_
#pragma once
#include "BST.h"
//伸展树类
//m次操作共O(mlogn),故均摊时间复杂度为O(logn)
//伸展树的目的是不想像AVL树那样严格,故无需记录平衡因子等
template <typename T>
class Splay : public BST<T> {
 public:
  BinNodePosi(T) & search(const T& e) override;
  BinNodePosi(T) insert(const T& e) override;
  bool remove(const T& e);

 protected:
	 //将v伸展至树根
  BinNodePosi(T) splay(BinNodePosi(T) v);
};
#endif
