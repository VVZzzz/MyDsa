#pragma once
//�ʵ���Ŀ��
template <typename K,typename V>
struct Entry {
  K key;
  V value;
  Entry(K k = K(), V v = V()) : key(k), value(v) {}
  Entry(const Entry<K, V>& rhs) : key(rhs.key), value(rhs.value) {}

  //�Ƚ���
  bool operator<(const Entry<K, V>& rhs) { return key < rhs.key; }
  bool operator>(const Entry<K, V>& rhs) { return key > rhs.key; }
  bool operator==(const Entry<K, V>& rhs) { return key == rhs.key; }
  bool operator!=(const Entry<K, V>& rhs) { return key != rhs.key; }
};
