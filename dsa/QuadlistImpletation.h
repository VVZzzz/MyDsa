#pragma once
#include "Quadlist.h"
template <typename T>
void QuadList<T>::init() {
  header = new QuadlistNode<T>();
  tail = new QuadlistNode<T>();
  header->pred = tail->succ = nullptr;
  header->succ = tail;
  tail->pred = header;
  header->below = header->above = nullptr;
  tail->below = tail->above = nullptr;
  _size = 0;
  //如此构造的四联表，不含任何实质的节点，且暂时与其它四联表相互独立
}

template <typename T>
T QuadList<T>::remove(QListNodePosi(T) p) {
  p->pred->succ = p->succ;
  p->succ->pred = p->pred;
  _size--;
  T e = p->entry;
  delete p;
  return e;
}

template <typename T>
int QuadList<T>::clear() {
  int oldsz = _size;
  while (0 < _size) {
    remove(header->succ);
  }
  return oldsz;
}

template <typename T>
QListNodePosi(T) QuadList<T>::insertAfterAbove(T const& e, QListNodePosi(T) p,
                                               QListNodePosi(T) b) {
  //插入一个新节点,作为p的后继,b的上邻
  _size++;
  return p->insertAsSuccAbove(e, b);  //返回新节点位置
}
template <typename T>
void QuadList<T>::traverse(void (*visit)(T&)) {
  //利用函数指针机制，只读或局部性修改
  QlistNodePosi(T) p = header;
  while ((p = p->succ) != trailer) visit(p->data);
}
template <typename T>
template <typename VST>
void QuadList<T>::traverse(VST& visit) {
  //利用函数对象机制，可全局性修改
  QlistNodePosi(T) p = header;
  while ((p = p->succ) != trailer) visit(p->data);
}
