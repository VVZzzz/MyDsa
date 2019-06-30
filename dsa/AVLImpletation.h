#ifndef AVL_IMPLETATION_H_
#define AVL_IMPLETATION_H_
#pragma once
#include "AVL.h"
template <typename T>
BinNodePosi(T) AVL<T>::insert(const T& e) {
  BinNodePosi(T)& x = search(e);
  //找到就返回,没找到x也是指向应插入的位置
  if (x) return x;
  BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);
  _size++;
  for (BinNodePosi(T) g = _hot; g; g = g->parent) {
    if (!AvlBalanced(*g)) {
      //调整g,g的儿子,g的孙子
      FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
      // g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束
      //注意这里和AVL的remove的区别,AVL中一旦摘除了某个节点造成失衡,则其父节点祖父等都可能失衡.
      break;
    } else {
      //即使平衡,也要更新高度(注意是要考虑刚插入新节点时的情况,依次对其父亲更新高度)
      updateHeight(g);
    }
  }
  //返回插入节点
  return xx;
}
template <typename T>
bool AVL<T>::remove(const T& e) {
  BinNodePosi(T)& x = search(e);
  if (NULL == x) return false;
  //先用BST中的remove方法摘除节点,再继续调整.
  removeAt(x, _hot);
  _size--;
  for (BinNodePosi(T) g = _hot; g; g = g->parent) {
    if (!AvlBalanced(*g)) {
      g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
    }
	//即使不失衡,g高度也可能降低
    updateHeight(g);
  }//故可能要Omege(logn)
  return true;
}
#endif
