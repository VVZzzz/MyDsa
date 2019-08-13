#pragma once
#include <math.h>
#include "HashTable.h"

//ɢ���뷽��
static size_t hashCode(char c) { return (size_t)c; }  //�ַ�
static size_t hashCode(int k) { return (size_t)k; }
//����64λ,���ǽ���32λ�͵�32λ���
static size_t hashCode(long long int i) { return (size_t)((i >> 32) + int(i)); }
static size_t hashCode(
    char s[]) {  //�����ַ�����ѭ����λɢ���루cyclic shift hash code��
  int h = 0;                                     //ɢ����
  for (size_t n = strlen(s), i = 0; i < n; i++)  //�������ң��������ÿһ�ַ�
  {
    h = (h << 5) | (h >> 27);
    h += (int)s[i];
  }                  //ɢ����ѭ������5λ�����ۼӵ�ǰ�ַ�
  return (size_t)h;  //������õ�ɢ���룬ʵ���Ͽ����Ϊ���Ƶġ�����ʽɢ���롱
  //����ʽɢ���뼴a0 + a1*x+ a2*x^2 + a3 * x^3 +.........
  // a0.....a1��ÿ���ַ���ֵ,xΪ��ϵ��
}  //����Ӣ�ﵥ�ʣ�"ѭ������5λ"��ʵ��ͳ�Ƶó������ֵ

bool isPrime(int n) { 
  int p = sqrt(n); 
  for (int i = 2; i < p; i++)
    if (n % i == 0) return false;
  return true;
}

//�õ�һ�����n������,�����и��ص�,����������6x+1����6x-1
static int getPrime(int n) { 
  int j = 0;
  int mul = 0;
  while (true) {
    if ((mul = j * 6 + 1) >= n)
      if (isPrime(mul)) return mul;
    if ((mul = j * 6 + 5) >= n)
      if (isPrime(mul)) return mul;
  }
}

template <typename K, typename V>
HashTable<K, V>::HashTable(int c) {
  M = c;
  N = 0;
  hashtable = new Entry<K, V>*[M](nullptr);
  lazyRemoval = new Bitmap(M);  //��������ɾ����ǵı���ͼ
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
  for (int i = 0; i < M; i++) {
    if (hashtable[i]) release(hashtable[i]);
  }
  release(hashtable);
  release(lazyRemoval);
}

/******************************************************************************************
 * �عؼ���k��Ӧ�Ĳ��������ҵ���֮ƥ���Ͱ�������Һ�ɾ������ʱ���ã�
 * ��̽���Զ��ֶ����������ѡȡ���������������̽����Ϊ��
 ******************************************************************************************/
template <typename K, typename V>
int HashTable<K, V>::probe4Hit(const K& k) {
  int r = hashCode(k) % M;
  while ((hashtable[r] && (k != hashtable[r]->key)) ||
         ((!hashtable[r]) && (lazilyRemoved(r)))) {
    //������ͻ�Ĵ���,������ɾ����ǵ�Ͱ
    r = (r + 1) % M;  //����̽��,������ɾۼ�����
  }

  return r;  //��ʱ��hashtable[r]Ϊ��,�����ʧ��.��Ϊ�ղ��ҳɹ�.
}

template <typename K, typename V>
int HashTable<K, V>::probe4Free(const K& k) {
  int r = hashCode(k) % M;
  //ֻҪhashtable[r]Ϊ��,�ͷ������λ��(�����Ƿ���ɾ�����)
  while (hashtable[r]) {
    r = (r + 1) % M;
  }
  return r;  //Ϊ��֤��Ͱ�����ҵ���װ�����Ӽ�ɢ�б���Ҫ��������
}

/******************************************************************************************
 * ��ɢ���㷨��װ�����ӹ���ʱ����ȡ����һȡ���ٲ��롱�����ز��ԣ���Ͱ��������
 * ���ɼ򵥵أ�ͨ��memcpy()����ԭͰ���鸴�Ƶ���Ͱ���飨����ǰ�ˣ�����������������⣺
 * 1����̳�ԭ�г�ͻ��2�����ܵ��²������ں�˶��ѡ�������Ϊ��������Ͱ��������ɾ����־Ҳ�޼�����
 ******************************************************************************************/
template <typename K, typename V>
void HashTable<K, V>::rehash() {
  int old_capacity = M;
  Entry<K, V>** old_ht = hashtable;
  M = getPrime(M);
  N = 0;
  hashtable = new Entry<K, V>*[M](nullptr);
  release(lazyRemoval);
  lazyRemoval = new Bitmap(M);  //�����µ������Ǳ���ͼ
  for (int i = 0; i < old_capacity; i++) {
    if (old_ht[i]) put(old_ht[i]->key, old_ht[i]->value);
  }
  release(old_ht);
}

template <typename K, typename V>
bool HashTable<K, V>::put(K k, V v) {
  if (hashtable[probe4Hit(k)]) return false;  //�����ظ���key
  int r = probe4Free(k);
  hashtable[r] = new Entry<K, V>(k, v);
  ++N;                      //����: ע�����������踴λ
  if (N * 2 > M) rehash();  //���װ������(N/M)����50%,����ɢ��.
  return true;
}

template <typename K, typename V>
V* HashTable<K, V>::get(K k) {
  int r = probe4Hit(k);
  return hashtable[r] ? &(hashtable[r]->value) : nullptr;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(K k) {
  int r = probe4Hit(k);
  if (!hashtable[r]) return false;  //ɾ��ʧ��
  release(hashtable[r]);
  hashtable[r] = nullptr;
  markAsRemoved(r);  //�ͷ�hashtable[r],�������λ����������ɾ�����
  --N;
  return true;
}
