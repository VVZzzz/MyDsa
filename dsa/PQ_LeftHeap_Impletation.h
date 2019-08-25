#pragma once
#include "PQ_LeftHeap.h"
//�ݹ����,�ϲ�������ʽ��a,b
//����Ҫ��,a�ѵĸ�ֵ���Ǵ���b�ѵĸ�ֵ,�Ա㽫��b�ϲ�����a��
/*
 *       a                  b
 *     /   \              /   \ 
 *    aRC   aRL          bRC   bRL
 *
 *       a                       b
 *     /   \                   /   \ 
 *    aRC      ->  aRL  ->    bRC   bRL
 *
 *  ����a���������ݹ�ϲ���b��,aRC = merge(aRL,b)
 *  ����ô�򵥵��㷨,��������Ҫע��һЩϸ��
 */
template <typename T>
BinNodePosi(T) PQ_LeftHeap<T>::merge(BinNodePosi(T) a, BinNodePosi(T) b) {
  //�ݹ�� 
  if (!a) return b;
  if (!b) return a;
  //��֤a��Զָ���Ǹ���ֲ�ϴ����ʽ��
  if (lt(a->data, b->data)) swap(a, b);
  a->rc = merge(a->rc, b);  
  //����ݹ����(�Ѿ��ϲ������,֮��Ҫ��һЩϸ�ڴ���)
}
