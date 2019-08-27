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
  //����ע��,����˼���ݹ�ʱ,���Զ�������εݹ����,����Ҫ��ʲô����
  a->rc->parent = a;  //���¸��ӹ�ϵ
  //����ϲ���,��������nplС��������npl,�򽻻�
  //ʼ�ձ�֤������npl >= ������npl
  if (!a->lc || a->lc->npl < a->rc->npl) swap(a->rc, a->lc);
  //���ºϲ����a��npl
  a->npl = (a->rc ? a->rc->npl : 0) + 1;
  //���غϲ���ĶѶ�
  return a;
}//���㷨ֻʵ�ֽṹ�ϵĺϲ����ѵĹ�ģ�����ϲ�����߸������

//���뼴�Ǻϲ�,����e,���Կ����Ǻ�һ��ֻ�и��ڵ�Ϊe����ʽ�Ѻϲ�
//O(logn)
template <typename T>
void PQ_LeftHeap<T>::insert(T e) {
  BinNodePosi(T) v = new BinNode(e);
  _root = merge(_root, v);
  _root->parent = NULL;
  _size++;   //���¹�ģ
}

//ɾ���Ѷ����ڵ����,����Ҳ�Ǻϲ�
/*
 *           root
 *          /    \
 *         LC     RC
 */
//����LC��RC�ϲ�
template <typename T>
T PQ_LeftHeap<T>::delMax() {
  BinNodePosi(T) lc = _root -> lc;
  BinNodePosi(T) rc = _root->rc;
  T e = _root->data;
  delete _root;
  _size--;
  _root = merge(lc, rc);
  if (_root) _root->parent = NULL;
  return e;
}
