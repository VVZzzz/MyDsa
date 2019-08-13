#pragma once
#include <math.h>
#include "HashTable.h"

//散列码方法
static size_t hashCode(char c) { return (size_t)c; }  //字符
static size_t hashCode(int k) { return (size_t)k; }
//对于64位,我们将高32位和低32位相加
static size_t hashCode(long long int i) { return (size_t)((i >> 32) + int(i)); }
static size_t hashCode(
    char s[]) {  //生成字符串的循环移位散列码（cyclic shift hash code）
  int h = 0;                                     //散列码
  for (size_t n = strlen(s), i = 0; i < n; i++)  //自左向右，逐个处理每一字符
  {
    h = (h << 5) | (h >> 27);
    h += (int)s[i];
  }                  //散列码循环左移5位，再累加当前字符
  return (size_t)h;  //如此所得的散列码，实际上可理解为近似的“多项式散列码”
  //多项式散列码即a0 + a1*x+ a2*x^2 + a3 * x^3 +.........
  // a0.....a1即每个字符的值,x为常系数
}  //对于英语单词，"循环左移5位"是实验统计得出的最佳值

bool isPrime(int n) { 
  int p = sqrt(n); 
  for (int i = 2; i < p; i++)
    if (n % i == 0) return false;
  return true;
}

//得到一个最靠近n的素数,素数有个特点,就是它等于6x+1或者6x-1
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
  lazyRemoval = new Bitmap(M);  //用来懒惰删除标记的比特图
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
 * 沿关键码k对应的查找链，找到与之匹配的桶（供查找和删除词条时调用）
 * 试探策略多种多样，可灵活选取；这里仅以线性试探策略为例
 ******************************************************************************************/
template <typename K, typename V>
int HashTable<K, V>::probe4Hit(const K& k) {
  int r = hashCode(k) % M;
  while ((hashtable[r] && (k != hashtable[r]->key)) ||
         ((!hashtable[r]) && (lazilyRemoved(r)))) {
    //跳过冲突的词条,和已有删除标记的桶
    r = (r + 1) % M;  //线性探测,容易造成聚集问题
  }

  return r;  //此时若hashtable[r]为空,则查找失败.不为空查找成功.
}

template <typename K, typename V>
int HashTable<K, V>::probe4Free(const K& k) {
  int r = hashCode(k) % M;
  //只要hashtable[r]为空,就返回这个位置(无论是否有删除标记)
  while (hashtable[r]) {
    r = (r + 1) % M;
  }
  return r;  //为保证空桶总能找到，装填因子及散列表长需要合理设置
}

/******************************************************************************************
 * 重散列算法：装填因子过大时，采取“逐一取出再插入”的朴素策略，对桶数组扩容
 * 不可简单地（通过memcpy()）将原桶数组复制到新桶数组（比如前端），否则存在两个问题：
 * 1）会继承原有冲突；2）可能导致查找链在后端断裂——即便为所有扩充桶设置懒惰删除标志也无济于事
 ******************************************************************************************/
template <typename K, typename V>
void HashTable<K, V>::rehash() {
  int old_capacity = M;
  Entry<K, V>** old_ht = hashtable;
  M = getPrime(M);
  N = 0;
  hashtable = new Entry<K, V>*[M](nullptr);
  release(lazyRemoval);
  lazyRemoval = new Bitmap(M);  //创建新的懒惰标记比特图
  for (int i = 0; i < old_capacity; i++) {
    if (old_ht[i]) put(old_ht[i]->key, old_ht[i]->value);
  }
  release(old_ht);
}

template <typename K, typename V>
bool HashTable<K, V>::put(K k, V v) {
  if (hashtable[probe4Hit(k)]) return false;  //已有重复的key
  int r = probe4Free(k);
  hashtable[r] = new Entry<K, V>(k, v);
  ++N;                      //插入: 注意懒惰标记无需复位
  if (N * 2 > M) rehash();  //如果装填因子(N/M)大于50%,则重散列.
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
  if (!hashtable[r]) return false;  //删除失败
  release(hashtable[r]);
  hashtable[r] = nullptr;
  markAsRemoved(r);  //释放hashtable[r],并在这个位置设置懒惰删除标记
  --N;
  return true;
}
