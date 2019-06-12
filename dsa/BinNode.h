#ifndef BINNODE_H_
#define BINNODE_H_
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>
//此文件是二叉树节点类
#define BinNodePosi(T) BinNode<T> *
#define stature(p) ((p) ? (p)->height : -1)

template <typename T>
struct BinNode {
  //成员变量
  T data;
  BinNodePosi(T) parent;
  BinNodePosi(T) lc;
  BinNodePosi(T) rc;
  int height;
  //构造函数
  BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0) {}
  BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) l = NULL,
          BinNodePosi(T) r = NULL, int h = 0)
      : data(e), parent(p), lc(l), rc(r), height(h) {}
  //操作接口
  int size();
  BinNodePosi(T) insertAsLC(const T &);  //插入到当前节点的左子节点
  BinNodePosi(T) insertAsRC(const T &);  //插入到当前节点的右子节点
  BinNodePosi(T) succ();                 //直接后继
  template <typename VST>
  void travLevel(VST &);  //子树层序遍历
  template <typename VST>
  void travPre(VST &);  //子树先序遍历
  template <typename VST>
  void travIn(VST &);  //子树中序遍历
  template <typename VST>
  void travPost(VST &);  //子树后序遍历

  //比较器
  bool operator<(BinNode const &bn) { return data < bn.data; }
  bool operator==(BinNode const &bn) { return data == bn.data; }

  //顺时针旋转
  BinNodePosi(T) zig();
  //逆时针旋转
  BinNodePosi(T) zag();
};

/****************二叉树节点的宏*************************/
//节点x
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) ((!IsRoot(x)) && (&(x) == (x).parent->lc))
#define IsRChild(x) ((!IsRoot(x)) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))

//节点指针p
#define sibling(p) /*兄弟*/ (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->rc)
#define uncle(p) /*叔叔*/ \
  (IsLChild(*((p)->parent)) ? (p)->parent->parent->rc : (p)->parent->parent->lc)
//来自父节点的引用
#define FromParentTo(x) \
  (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))

/********************树的通用函数**********************/
//将x子树变为只有左子树的链
template <typename T>
void strecthByZag(BinNodePosi(T) & x);

//将x子树变为只有右子树的链,第二个参数为子树x的规模
template <typename T>
void strecthByZig(BinNodePosi(T) & x, int h);

#include "BinNodeImpletation.h"
#endif
