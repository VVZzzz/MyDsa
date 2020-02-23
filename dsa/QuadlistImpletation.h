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
  //��˹���������������κ�ʵ�ʵĽڵ㣬����ʱ�������������໥����
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
  //����һ���½ڵ�,��Ϊp�ĺ��,b������
  _size++;
  return p->insertAsSuccAbove(e, b);  //�����½ڵ�λ��
}
template <typename T>
void QuadList<T>::traverse(void (*visit)(T&)) {
  //���ú���ָ����ƣ�ֻ����ֲ����޸�
  QlistNodePosi(T) p = header;
  while ((p = p->succ) != trailer) visit(p->data);
}
template <typename T>
template <typename VST>
void QuadList<T>::traverse(VST& visit) {
  //���ú���������ƣ���ȫ�����޸�
  QlistNodePosi(T) p = header;
  while ((p = p->succ) != trailer) visit(p->data);
}
