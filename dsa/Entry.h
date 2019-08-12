#pragma once
//词典条目类
template <typename K,typename V>
struct Entry {
  K key;
  V value;
  Entry(K k = K(), V v = V()) : key(k), value(v) {}
  Entry(const Entry<K, V>& rhs) : key(rhs.key), value(rhs.value) {}

  //比较器
  bool operator<(const Entry<K, V>& rhs) { return key < rhs.key; }
  bool operator>(const Entry<K, V>& rhs) { return key > rhs.key; }
  bool operator==(const Entry<K, V>& rhs) { return key == rhs.key; }
  bool operator!=(const Entry<K, V>& rhs) { return key != rhs.key; }
};
