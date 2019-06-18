#ifndef BST_H_
#define BST_H_
#pragma once
#include "BinTree.h"

template <typename T>
class BST : public BinTree<T> {
 protected:
  BinNodePosi(T) _hot;  //命中节点的父亲
  /*
		    B
		  /  \
		 A    C
		/ \  / \
		1 2  3  4
		3+4结构
   */
  BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
                BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
  //将x 和 父亲 祖父节点旋转(用于AVL平衡)
  BinNodePosi(T) rotateAt(BinNodePosi(T) x);

  //基本接口
 public:
  virtual BinNodePosi(T) & search(const T& e);
  virtual BinNodePosi(T) insert(const T& e);
  virtual bool remove(const T& e);
};

#include "BSTImpletation.h"
#endif
