#ifndef BINNODE_H_
#define BINNODE_H_
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>
//���ļ��Ƕ������ڵ���
#define BinNodePosi(T) BinNode<T> *
#define stature(p) ((p) ? (p)->height : -1)

template <typename T>
struct BinNode {
  //��Ա����
  T data;
  BinNodePosi(T) parent;
  BinNodePosi(T) lc;
  BinNodePosi(T) rc;
  int height;  //�߶�,ͨ��
  int npl;  //Null Path Length,��ʽ����
  //���캯��
  BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0),npl(1) {}
  BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) l = NULL,
          BinNodePosi(T) r = NULL, int h = 0,int l = 1)
      : data(e), parent(p), lc(l), rc(r), height(h),npl(l) {}
  //�����ӿ�
  int size();
  BinNodePosi(T) insertAsLC(const T &);  //���뵽��ǰ�ڵ�����ӽڵ�
  BinNodePosi(T) insertAsRC(const T &);  //���뵽��ǰ�ڵ�����ӽڵ�
  BinNodePosi(T) succ();                 //ֱ�Ӻ��
  template <typename VST>
  void travLevel(VST &);  //�����������
  template <typename VST>
  void travPre(VST &);  //�����������
  template <typename VST>
  void travIn(VST &);  //�����������
  template <typename VST>
  void travPost(VST &);  //�����������

  //�Ƚ���
  bool operator<(BinNode const &bn) { return data < bn.data; }
  bool operator==(BinNode const &bn) { return data == bn.data; }

  //˳ʱ����ת
  BinNodePosi(T) zig();
  //��ʱ����ת
  BinNodePosi(T) zag();
};

/****************�������ڵ�ĺ�*************************/
//�ڵ�x
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) ((!IsRoot(x)) && (&(x) == (x).parent->lc))
#define IsRChild(x) ((!IsRoot(x)) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))

//�ڵ�ָ��p
#define sibling(p) /*�ֵ�*/ (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->rc)
#define uncle(p) /*����*/ \
  (IsLChild(*((p)->parent)) ? (p)->parent->parent->rc : (p)->parent->parent->lc)
//���Ը��ڵ������
#define FromParentTo(x) \
  (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))

/********************����ͨ�ú���**********************/
//��x������Ϊֻ������������
template <typename T>
void strecthByZag(BinNodePosi(T) & x);

//��x������Ϊֻ������������,�ڶ�������Ϊ����x�Ĺ�ģ
template <typename T>
void strecthByZig(BinNodePosi(T) & x, int h);

/******************AVL����Ҫ�ĺ�**********************/
//����x�ڵ�߶�,�����߶�Ϊ-1 , Ҷ�ӽڵ�߶�Ϊ0
#define HeightUpdated(x) \
  ((x).height == 1 + max(stature((x).lc), stature((x).rc)))
// x�ڵ��Ƿ�ƽ��
#define Balanced(x) (stature((x).lc) == stature((x).rc))
//ƽ������
#define BalFac(x) (stature((x).lc) - stature((x).rc))
// AVLƽ������
#define AvlBalanced(x) (-2 < BalFac(x)) && (BalFac(x) < 2)
//�ҵ��߶Ƚϸߵ�����
#define tallerChild(x) ( \
   stature( (x)->lc ) > stature( (x)->rc ) ? (x)->lc : ( /*���*/ \
   stature( (x)->lc ) < stature( (x)->rc ) ? (x)->rc : ( /*�Ҹ�*/ \
   IsLChild( * (x) ) ? (x)->lc : (x)->rc /*�ȸߣ��븸��xͬ���ߣ�zIg-zIg��zAg-zAg������*/ \
   ) \
   ) \
)


#include "BinNodeImpletation.h"
#endif
