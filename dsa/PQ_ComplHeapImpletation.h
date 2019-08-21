#pragma once
#include "PQ_ComplHeap.h"
/*
 * ��ȫ����ѵĲ���:
 * �����Ƚ�Ҫ�����Ԫ�������_ele����ĩβ,������������
 * eg:  ����11
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   11
 *
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           11   3  4   2
 *          / \
 *         1   5
 *                 11
 *               /  \
 *             10    6
 *            /  \  /  \
 *           8   3  4   2
 *          / \
 *         1   5
 */
template <typename T>
void PQ_ComplHeap<T>::insert(T e) {
  Vector<T>::insert(e);
  percolateUP(_size - 1);
}

//���˲���,����ͼ,��������Ϊ�����Ч��,���ǲ��ý���,����ֱ�Ӹ�ֵ
//�����Ϳ��Դﵽlogn + 1 ������ 3logn(������Ҫ3��)
/*
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   11
 *                 10
 *               /  \
 *        (��ʱ8��11�Ƚ�,��С����8���������5)
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   5
 */
template <typename T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i) {
  /*  //��������,3logn
  while (ParentValid(i)) {  //���i�и���,��δ�ﵽ��
    int j = Parent(i);
    if (lt(_elem[i],_elem[j])) break;  //�������С�ڸ���,��break
    swap(_elem[i], _elem[j]);   //���򽻻�
    i = j;
  }
  */
  //��ֵ����,logn+1
  int dst = _elem[i];
  while (ParentValid(i)) {
    int j = Parent(i);
    if (lt(dst, _elem[j])) {
      _elem[i] = dst;
      break;
    }
    _elem[i] = _elem[j];
    i = j;
  }
}


//ɾ������,ֻ��ɾ�����ڵ�(�����)
//�����㷨�ǽ���ĩβ��Ԫ�ظ����Ѷ�Ԫ��,Ȼ��ʹ�����˼���
template <typename T>
T PQ_ComplHeap<T>::delMax() {
  T maxElem = _elem[0];
  _elem[0] = _elem[--_size];  //ժ���Ѷ�
  percolateDown(_size, 0);   //���¸�ִ������
  return maxElem;
}

template <typename T>
inline Rank PQ_ComplHeap<T>::properparentV(Rank n, Rank i, T val) {
  if (RChildValid(n, i)) {
    if (val < _elem[LChild(i)])
      return Bigger(_elem, LChild(i), RChild(i));
    else
      return BiggerV(_elem, i, RChild(i), val);
  } else {
    if (LChildValid(n, i)) return BiggerV(_elem, i, LChild(i), val);
    else
      return i;
  }
}

/*
 * ɾ��10
 *                 10
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / \
 *         1   0
 *                 0
 *               /  \
 *              8    6
 *            /  \  /  \
 *           5   3  4   2
 *          / 
 *         1   
 * ����,�� �����������Ǹ��滻0,��max(8,6)�滻0
 *                 8
 *               /  \
 *              0    6
 *            /  \  /  \
 *           5   3  4   2
 *          / 
 *         1   
 */
template<typename T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i) {
  /*  ��������
  Rank j;
  //ֻҪi������,_elem[i]���������������������Ǹ��±�
  //��ֻҪ�ӽڵ����ȼ����ڸ��ڵ�
  while (i!=(j=ProperParent(_elem,n,i))) {
    swap(_elem[i], _elem[j]);//��������
    i = j;  //��ʱj�������������������Ǹ�
  }
  return i;  //�������˵ִ��λ�ã���i��j��
  */

  //��ֵ����
  Rank j;
  int val = _elem[i];
  while (i!=(j=properparentV(_elem,n,i,val))) {
    _elem[i] = _elem[j]
    i = j;  //��ʱj�������������������Ǹ�
  }
  _elem[i] = val;
  return i;
}

//floyd���ѷ�,��������,������һ������,���������ǽ���һ����ȫ�����(���ȶ���)
//ƽ��˼·��: ÿ�ζ�����insert,����ÿ�ζ�Ҫ�ӶѶ�����,�һ�����_elem������
//floyd������: �ȿ�����_elem��,���Ŵ����һ����Ҷ�ӽڵ㿪ʼ����,ֱ�����ڵ�
//ʱ�临�Ӷ�: O(n),��Ϊ��ȫ����ѵ�Ҷ�ӽڵ�ռ��һ��,���Һ��ٽڵ��һֱ���˵���
/*
 *
 * ��4��ʼ��������
 *                 8
 *               /  \
 *             16    6
 *            /  \  /  \
 *           *4    9 5   20
 *          / 
 *         14   
 *                 8
 *               /   \
 *             16     6*
 *            /  \   /  \
 *           14    9 5   20
 *          / 
 *         4   
 *                 8
 *               /   \
 *             *16     20
 *            /  \   /  \
 *           14    9 5   6
 *          / 
 *         4   
 *                 *20
 *               /   \
 *             16     8
 *            /  \   /  \
 *           14    9 5   6
 *          / 
 *         4   
 */
template <typename T>
void PQ_ComplHeap<T>::heapify(Rank n) {
  
}
