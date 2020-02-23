#pragma once
#include <list>
#include "Dictionary.h"
#include "Entry.h"
#include "Quadlist.h"
using std::list;

//����Dictionary�ӿڵ�SkipList��
template <typename K, typename V>
class Skiplist : public Dictionary<K, V> , public list<QuadList<Entry<K, V>>*> {
 public:
  // empty() ��back()����list,size()����QuadList::size()
  int size() const { return empty() ? 0 : back()->size(); }
  int level() const { return list::size(); }  //��� == #Quadlist����һ��Ҫ����
  bool put(K, V);  //���루ע����Map�б𡪡�Skiplist��������ظ����ʱ�Ȼ�ɹ���
  V* get(K k);     //��ȡ
  bool remove(K k);  //ɾ��

 protected:
  bool skipSearch(list<QuadList<Entry<K, V>>*>::iterator &itr,
                  QuadlistNode<Entry<K, V>>*& p, K& k);
};
#include "SkipListImpletation.h"
