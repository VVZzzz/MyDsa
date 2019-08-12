#pragma once
//�ʵ�ģ������Ϊ�������
template <typename K,typename V>
struct Dictionary {
  virtual int size() const = 0;  //��ǰ��������
  virtual bool put(K, V) = 0;    //�������(��ֹ�ظ�����ʱ����ʧ��)
  virtual V *get(K k) = 0;       //��ȡ����
  virtual bool remove(K k) = 0;  //ɾ������
};


