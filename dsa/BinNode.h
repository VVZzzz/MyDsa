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
  int height;  //高度,通用
  int npl;  //Null Path Length,左式堆用
  //构造函数
  BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0),npl(1) {}
  BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) l = NULL,
          BinNodePosi(T) r = NULL, int h = 0,int l = 1)
      : data(e), parent(p), lc(l), rc(r), height(h),npl(l) {}
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

/******************AVL树需要的宏**********************/
//更新x节点高度,空树高度为-1 , 叶子节点高度为0
#define HeightUpdated(x) \
  ((x).height == 1 + max(stature((x).lc), stature((x).rc)))
// x节点是否平衡
#define Balanced(x) (stature((x).lc) == stature((x).rc))
//平衡因子
#define BalFac(x) (stature((x).lc) - stature((x).rc))
// AVL平衡条件
#define AvlBalanced(x) (-2 < BalFac(x)) && (BalFac(x) < 2)
//找到高度较高的子树
#define tallerChild(x) ( \
   stature( (x)->lc ) > stature( (x)->rc ) ? (x)->lc : ( /*左高*/ \
   stature( (x)->lc ) < stature( (x)->rc ) ? (x)->rc : ( /*右高*/ \
   IsLChild( * (x) ) ? (x)->lc : (x)->rc /*等高：与父亲x同侧者（zIg-zIg或zAg-zAg）优先*/ \
   ) \
   ) \
)


#include "BinNodeImpletation.h"
#endif
