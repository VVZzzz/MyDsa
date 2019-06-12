#ifndef BINTREE_H_
#define BINTREE_H_
#pragma once
#include "BinNode.h"
#include "comparator.h"
#include "release.h"
template <typename T>
class BinTree {
 protected:
  //��Ա
  int _size;
  BinNodePosi(T) _root;
  virtual int updateHeight(BinNodePosi(T) p);  //���½ڵ�p�߶�
  void updateHeightAbove(BinNodePosi(T) p);    //���½ڵ㼰�����ȸ߶�
 public:
  BinTree() : _size(0), _root(NULL) {}
  ~BinTree() {
    if (_size > 0) remove(_root);
  }
  int size() const { return _size; }
  bool empty() const { return !_root; }
  BinNodePosi(T) root() const { return _root; }
  BinNodePosi(T) insertAsRoot(T const& e);  //������ڵ�
  BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);  // e��Ϊx�����ӣ�ԭ�ޣ�����
  BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);  // e��Ϊx���Һ��ӣ�ԭ�ޣ�����
  BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& T);  // T��Ϊx����������
  BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& T);  // T��Ϊx����������

  int remove(BinNodePosi(T) x);  //ɾ��x������,����֮ǰ�Ĺ�ģ
  BinTree<T>* secede(BinNodePosi(T) x);  //������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������

  //����������
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

  //�ж�
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
