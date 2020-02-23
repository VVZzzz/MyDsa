#pragma once
#include "QuadlistNode.h"
template<typename T>
QListNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(T const & e, QListNodePosi(T) b)
{
  QListNodePosi(T) x = new QuadlistNode(e, this, this->succ, NULL, b);
  succ->pred = x;
  succ = x;
  if (b) b->above = x;
  return x;
}
