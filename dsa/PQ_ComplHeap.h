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
//���ȶ���,���ڵ�����ȼ����Ǳ����ĺ������ȼ���!
/*
 *                  10[0]
 *               /         \
 *              8[1]        6[2]
 *             /    \       /  \
 *            5[3]   3[4]  4[5] 2[6]
 *          /
 *         1[7]
 * 
 *       vec: 10 8 6 5 4 3 2 1
*/


#include "Vector.h"
#include "PQ.h"
#include "comparator.h"
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
   void insert ( T ); //���ձȽ���ȷ�������ȼ����򣬲������,O(logn)
   T getMax() { return _elem[0]; }  //��ȡ���ȼ���ߵĴ���,O(1)
   T delMax(); //ɾ�����ȼ���ߵĴ���,O(logn)
  private:
//properparent��Ϊ�����˵ĸ�ֵ�����õĹ��ߺ���
//����val����PQ[i]��ȥ���бȽ�
   Rank properparentV(Rank n, Rank i, T val);

};

#define  InHeap(n, i)      ( ( ( -1 ) < ( i ) ) && ( ( i ) < ( n ) ) ) //�ж�PQ[i]�Ƿ�Ϸ�
#define  Parent(i)         ( ( i - 1 ) >> 1 ) //PQ[i]�ĸ��ڵ㣨floor((i-1)/2)��i����������
#define  LastInternal(n)   Parent( n - 1 ) //���һ���ڲ��ڵ㣨��ĩ�ڵ�ĸ��ף�
#define  LChild(i)         ( 1 + ( ( i ) << 1 ) ) //PQ[i]������
#define  RChild(i)         ( ( 1 + ( i ) ) << 1 ) //PQ[i]���Һ���
#define  ParentValid(i)    ( 0 < i ) //�ж�PQ[i]�Ƿ��и���
#define  LChildValid(n, i) InHeap( n, LChild( i ) ) //�ж�PQ[i]�Ƿ���һ�����󣩺���
#define  RChildValid(n, i) InHeap( n, RChild( i ) ) //�ж�PQ[i]�Ƿ�����������
#define  Bigger(PQ, i, j)  ( lt( PQ[i], PQ[j] ) ? j : i ) //ȡ���ߣ���ʱǰ�����ȣ�
#define  ProperParent(PQ, n, i) /*���ӣ����ࣩ�����еĴ���*/ \
            ( RChildValid(n, i) ? Bigger( PQ, Bigger( PQ, i, LChild(i) ), RChild(i) ) : \
            ( LChildValid(n, i) ? Bigger( PQ, i, LChild(i) ) : i \
            ) \
            ) //���ʱ���ڵ����ȣ���˿ɱ��ⲻ��Ҫ�Ľ���

//BiggerV��Ϊ�����˵ĸ�ֵ�����õĹ��ߺ���
//����val����PQ[i]��ȥ���бȽ�
#define  BiggerV(PQ, i, j,val)  ( lt( val, PQ[j] ) ? j : i ) //ȡ���ߣ���ʱǰ�����ȣ�
