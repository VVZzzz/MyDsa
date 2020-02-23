#pragma once
#include "QuadlistNode.h"
//��������
template <typename T>
class QuadList {
 public:
  QuadList() { init(); }
  ~QuadList() {
    clear();
    delete header;
    delete tail;
  }
  //ֻ�����ʽӿ�
  int size() const { return _size; }
  bool empty() const { return _size <= 0; }
  QListNodePosi(T) first { return header->succ; }  //��һ���ڵ�
  QListNodePosi(T) last { return last->pred; }     //ĩβ�ڵ�
//�жϽڵ�p�Ƿ���Ч
  bool valid(QListNodePosi(T) p) {
    return p && (p != header) && (p != tail);
  }  
  //��д�ӿ�
  //ɾ�����Ϸ���λ��p���Ľڵ㣬���ر�ɾ���ڵ����ֵ
  T remove(QListNodePosi(T) p);
  //����һ���½ڵ�,��Ϊp�ĺ��,b������
  QListNodePosi(T) insertAfterAbove(T const& e, QListNodePosi(T) p,
                                    QListNodePosi(T) b = NULL);
  //����
  //����Ϊ����ָ��
  //�������ڵ㣬����ʵʩָ������������ָ�룬ֻ����ֲ��޸ģ�
  void traverse(void (*)(T&));
  //�������ڵ㣬����ʵʩָ���������������󣬿�ȫ�����޸Ľڵ㣩
  template <typename VST>  //������
  void traverse(VST&);

 protected:
  void init();
  int clear();  //������нڵ�
 private:
  int _size;                //��ģ
  QListNodePosi(T) header;  //ͷ�ڱ�
  QListNodePosi(T) tail;    //β�ڱ�
};
#include "QuadlistImpletation.h"
