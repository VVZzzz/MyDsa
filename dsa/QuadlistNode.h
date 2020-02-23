#pragma once
// QuadListNode
#include "Entry.h"
#define QListNodePosi(T) QuadlistNode<T>*

template <typename T>
struct QuadlistNode {
  T entry;                 //所存词条
  QListNodePosi(T) pred;   //前驱
  QListNodePosi(T) succ;   //后继
  QListNodePosi(T) above;  //上邻
  QListNodePosi(T) below;  //下邻
  QuadlistNode(T e = T(), QListNodePosi(T) p = NULL, QListNodePosi(T) s = NULL,
            QListNodePosi(T) a = NULL, QListNodePosi(T) b = NULL)
      : entry(e), pred(p), succ(s), above(a), below(b) {}
  //插入新节点,以当前节点为前驱,节点b为下邻
  QListNodePosi(T) insertAsSuccAbove(T const& e, QListNodePosi(T) b = NULL);
};
#include "QuadlistNodeImpletation.h"
