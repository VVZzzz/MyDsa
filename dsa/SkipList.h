#pragma once
#include <list>
#include "Dictionary.h"
#include "Entry.h"
#include "Quadlist.h"
using std::list;

//符合Dictionary接口的SkipList类
template <typename K, typename V>
class Skiplist : public Dictionary<K, V> , public list<QuadList<Entry<K, V>>*> {
 public:
  // empty() 和back()来自list,size()来自QuadList::size()
  int size() const { return empty() ? 0 : back()->size(); }
  int level() const { return list::size(); }  //层高 == #Quadlist，不一定要开放
  bool put(K, V);  //插入（注意与Map有别——Skiplist允许词条重复，故必然成功）
  V* get(K k);     //读取
  bool remove(K k);  //删除

 protected:
  bool skipSearch(list<QuadList<Entry<K, V>>*>::iterator &itr,
                  QuadlistNode<Entry<K, V>>*& p, K& k);
};
#include "SkipListImpletation.h"
