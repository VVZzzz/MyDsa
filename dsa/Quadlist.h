#pragma once
#include "QuadlistNode.h"
//四联链表
template <typename T>
class QuadList {
 public:
  QuadList() { init(); }
  ~QuadList() {
    clear();
    delete header;
    delete tail;
  }
  //只读访问接口
  int size() const { return _size; }
  bool empty() const { return _size <= 0; }
  QListNodePosi(T) first { return header->succ; }  //第一个节点
  QListNodePosi(T) last { return last->pred; }     //末尾节点
//判断节点p是否有效
  bool valid(QListNodePosi(T) p) {
    return p && (p != header) && (p != tail);
  }  
  //可写接口
  //删除（合法）位置p处的节点，返回被删除节点的数值
  T remove(QListNodePosi(T) p);
  //插入一个新节点,作为p的后继,b的上邻
  QListNodePosi(T) insertAfterAbove(T const& e, QListNodePosi(T) p,
                                    QListNodePosi(T) b = NULL);
  //遍历
  //参数为函数指针
  //遍历各节点，依次实施指定操作（函数指针，只读或局部修改）
  void traverse(void (*)(T&));
  //遍历各节点，依次实施指定操作（函数对象，可全局性修改节点）
  template <typename VST>  //操作器
  void traverse(VST&);

 protected:
  void init();
  int clear();  //清除所有节点
 private:
  int _size;                //规模
  QListNodePosi(T) header;  //头哨兵
  QListNodePosi(T) tail;    //尾哨兵
};
#include "QuadlistImpletation.h"
