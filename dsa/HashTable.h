#ifndef HASHTABLE_H
#define HASHTABLE_H
#pragma once
#include "Dictionary.h"
#include "Entry.h"
#include "Bitmap.h"

/*
 * ��ϣ��ʹ��������ַ�ռ������ʵ��,��: [k0,v0] [k1,v1]... [Km-1,Vm-1]
 * M���Ϊ����,��ֹ%Mȡ��ʱ��ƽ��.
 * ɾ��ʱ����������ɾ��(��������ͼ),ֱ��ɾ���Ļ�����ɶ������.(����˼��һ��Ϊɶֱ��ɾ����)
 */
template <typename K,typename V>
class HashTable : public Dictionary<K, V> {
 private:
  Entry<K, V>** hashtable;  //Ͱ(��ϣ��)����
  int M;                    //Ͱ��������
  int N;                    //��������
  Bitmap* lazyRemoval;      //����ɾ�����
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
 protected:
  int probe4Hit(const K& k);  //�عؼ���,�ҵ�����ƥ���Ͱ
  int probe4Free(const K& k); //�عؼ���,�ҵ��׸����õ�Ͱ
  void rehash();              //��ɢ��: ����Ͱ����,��֤װ�������ھ�����һ��
 public:
  HashTable(int c = 5);
  ~HashTable();
  int size() const { return N; }
  bool put(K, V);   //�������
  V* get(K k);      //��ȡ
  bool remove(K k); //ɾ��
  
};

#include "HashTableImpletation.h"

#endif
