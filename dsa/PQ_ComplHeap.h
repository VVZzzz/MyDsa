#pragma once
//���ȶ���,��ȫ�����(��vectorʵ��)
//�����ǵ���vectorʵ��: insert:O(n),getmax:O(n),delmax:O(n)
//����������vectorʵ��: insert:O(n),getmax:O(1),delmax:O(1)
//��������listʵ��: insert: O(1),getmax: O(n),delmax:O(1)
//����������listʵ��: insert:O(n),getmax:O(1),delmax:O(1)
//��������BBST(AVL,SPlay,Red-BlackTree),insert: O(logn),getmax: O(1),delmax:O(logn)
//����BBST�Ĺ���ԶԶ������PQ������,ɱ����ţ��
//��ΪPQֻ��Ҫ�����ȼ������Ǹ��ڵ����,����ά��ȫ��,ֻ��ƫ�򼴿�

//�������߼��ṹ: ʹ����ȫ������, ��������ṹ(ʵ��): ʹ��һ��Vector
//insert: O(logn),getmax: O(1),delmax:O(logn)
/*
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          /
 *         1
 * 
 *       vec: 10 8 6 5 4 3 2 1
 */

#include "Vector.h"
#include "PQ.h"
template <typename T>
class PQ_ComplHeap : public PQ<T>, public Vector<T> {  //��ȫ�����
  //��̳�,������vector��˽�г�ԱT *_ele
 protected:
  Rank percolateDown(Rank n, Rank i);  //��ǰn�������еĵ�i����������,i<n
  Rank percolateUp(Rank i);  //�Ե�i�������������� i<_size
  void heapify(Rank n);      //Floyd���ѷ�
 public:
   PQ_ComplHeap() { } //Ĭ�Ϲ���
   PQ_ComplHeap ( T* A, Rank n ) { copyFrom ( A, 0, n ); heapify ( n ); } //��������
   void insert ( T ); //���ձȽ���ȷ�������ȼ����򣬲������
   T getMax(); //��ȡ���ȼ���ߵĴ���
   T delMax(); //ɾ�����ȼ���ߵĴ���

};
