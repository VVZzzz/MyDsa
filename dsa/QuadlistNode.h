#pragma once
// QuadListNode
#include "Entry.h"
#define QListNodePosi(T) QuadlistNode<T>*

template <typename T>
struct QuadlistNode {
  T entry;                 //�������
  QListNodePosi(T) pred;   //ǰ��
  QListNodePosi(T) succ;   //���
  QListNodePosi(T) above;  //����
  QListNodePosi(T) below;  //����
  QuadlistNode(T e = T(), QListNodePosi(T) p = NULL, QListNodePosi(T) s = NULL,
            QListNodePosi(T) a = NULL, QListNodePosi(T) b = NULL)
      : entry(e), pred(p), succ(s), above(a), below(b) {}
  //�����½ڵ�,�Ե�ǰ�ڵ�Ϊǰ��,�ڵ�bΪ����
  QListNodePosi(T) insertAsSuccAbove(T const& e, QListNodePosi(T) b = NULL);
};
#include "QuadlistNodeImpletation.h"
