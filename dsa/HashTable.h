#ifndef HASHTABLE_H
#define HASHTABLE_H
#pragma once
#include "Dictionary.h"
#include "Entry.h"
#include "Bitmap.h"

/*
 * 哈希表使用连续地址空间的数组实现,即: [k0,v0] [k1,v1]... [Km-1,Vm-1]
 * M最好为素数,防止%M取余时不平均.
 * 删除时采用懒惰标记删除(借助比特图),直接删除的话会造成多次问题.(可以思考一下为啥直接删不行)
 */
template <typename K,typename V>
class HashTable : public Dictionary<K, V> {
 private:
  Entry<K, V>** hashtable;  //桶(哈希表)数组
  int M;                    //桶数组容量
  int N;                    //词条数量
  Bitmap* lazyRemoval;      //懒惰删除标记
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
 protected:
  int probe4Hit(const K& k);  //沿关键码,找到词条匹配的桶
  int probe4Free(const K& k); //沿关键码,找到首个可用的桶
  void rehash();              //重散列: 扩充桶数组,保证装填因子在警戒线一下
 public:
  HashTable(int c = 5);
  ~HashTable();
  int size() const { return N; }
  bool put(K, V);   //插入词条
  V* get(K k);      //读取
  bool remove(K k); //删除
  
};

#include "HashTableImpletation.h"

#endif
