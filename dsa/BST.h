#ifndef BST_H_
#define BST_H_
#pragma once
#include "BinTree.h"

template <typename T>
class BST : public BinTree<T> {
 protected:
  BinNodePosi(T) _hot;  //���нڵ�ĸ���
  /*
		    B
		  /  \
		 A    C
		/ \  / \
		1 2  3  4
		3+4�ṹ
   */
  BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
                BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
  //��x �� ���� �游�ڵ���ת(����AVLƽ��)
  BinNodePosi(T) rotateAt(BinNodePosi(T) x);

  //�����ӿ�
 public:
  virtual BinNodePosi(T) & search(const T& e);
  virtual BinNodePosi(T) insert(const T& e);
  virtual bool remove(const T& e);
};

#include "BSTImpletation.h"
#endif
