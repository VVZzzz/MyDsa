#ifndef BINTREE_IMPLETATION_H_
#define BINTREE_IMPLETATION_H_
#include "BinTree.h"
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) p) {
  return p->height = 1 + std::max(stature(p->lc), stature(p->rc));
}

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) p) {
  int prevh;
  while (p) {
    prevh = p->height;
    updateHeight(p);
    //优化:当高度不变时停止
    if (prevh == p->height) break;
    p = p->parent;
  }
  return;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) {
  _size = 1;
  _root = new BinNode<T>(e);
  return _root;
}

template <typename T>
static int removeAt(BinNodePosi(T) x) {
  if (!x) return 0;  //递归基
  int n = 1 + removeAt(x->lc) + removeAt(x->rc);
  release(x->data);
  release(x);
  return n;
}

template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x) {
  //将x的祖先的子指针设为NULL
  FromParentTo(*x) = NULL;
  updateHeightAbove(x->parent);
  int n = removeAt(x);
  _size -= n;
  return n;
}

template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
  FromParentTo(*x) = NULL;
  updateHeightAbove(x->parent);
  BinTree<T>* S = new BinTree<T>;
  S->_root = x;
  S->_size = x->size();
  _size -= S->_size;
  return S;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) {
  _size++;
  x->insertAsLC(e);
  updateHeightAbove(x);
  return x->lc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e) {
  _size++;
  x->insertAsRC(e);
  updateHeightAbove(x);
  return x->rc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>*& T) {}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>*& T) {}

#endif
