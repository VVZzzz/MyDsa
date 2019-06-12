#ifndef BINTREE_H_
#define BINTREE_H_
#pragma once
#include "BinNode.h"
#include "comparator.h"
#include "release.h"
template <typename T>
class BinTree {
 protected:
  //成员
  int _size;
  BinNodePosi(T) _root;
  virtual int updateHeight(BinNodePosi(T) p);  //更新节点p高度
  void updateHeightAbove(BinNodePosi(T) p);    //更新节点及其祖先高度
 public:
  BinTree() : _size(0), _root(NULL) {}
  ~BinTree() {
    if (_size > 0) remove(_root);
  }
  int size() const { return _size; }
  bool empty() const { return !_root; }
  BinNodePosi(T) root() const { return _root; }
  BinNodePosi(T) insertAsRoot(T const& e);  //插入根节点
  BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);  // e作为x的左孩子（原无）插入
  BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);  // e作为x的右孩子（原无）插入
  BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& T);  // T作为x左子树接入
  BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& T);  // T作为x右子树接入

  int remove(BinNodePosi(T) x);  //删除x的子树,返回之前的规模
  BinTree<T>* secede(BinNodePosi(T) x);  //将子树x从当前树中摘除，并将其转换为一棵独立子树

  //遍历操作器
  template <typename VST>
  void travPre(VST& visit) {
    if (_root) _root->travPre(visit);
  }
  template <typename VST>
  void travIn(VST& visit) {
    if (_root) _root->travIn(visit);
  }
  template <typename VST>
  void travPost(VST& visit) {
    if (_root) _root->travPost(visit);
  }
  template <typename VST>
  void travLevel(VST& visit) {
    if (_root) _root->travLevel(visit);
  }

  //判断
  bool operator<(BinTree<T> const& rhs) {
    return _root && rhs._root && lt(_root, rhs._root);
  }
  bool operator==(BinTree<T> const& rhs) {
    return _root && rhs._root && eq(_root, rhs._root);
  }

  void stretchToRPath() { strecthByZig(_root, _size); }
  void stretchToLPath() { strecthByZag(_root); }
};

#include "BinTreeImpletation.h"

#endif  // !BINTREE_H_
