#ifndef SPLAY_H_
#define SPLAY_H_
#pragma once
#include "BST.h"
//��չ����
template <typename T>
class Splay : public BST<T> {
 public:
  BinNodePosi(T) & search(const T& e) override;
  BinNodePosi(T) insert(const T& e) override;
  bool remove(const T& e);

 protected:
	 //��v��չ������
  BinNodePosi(T) splay(BinNodePosi(T) v);
};
#endif
