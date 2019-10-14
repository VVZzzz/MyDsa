#pragma once
#include "Vector.h"

//K-Selection����(��ѡȡ��С����������ΪK��Ԫ��)
//�㷨1: ƽ��˼·:�ȶԶ�������,֮��ѡȡA[k]

//�㷨2: �����ѽ���ѡȡ
//A: �Ƚ���һ��С����,Ȼ��delMin() K��,��ʱ�ĶѶ�Ԫ�ؼ�Ϊ K-Selection
//   ���Ӷ�Ϊ: O(n) + k*O(logn) = O( n + klogn) O(n)ΪFloyd�������ѷ�

//B: ����K��Ԫ�ؽ���һ�������, Ȼ��ʣ�µ�(n-k)��Ԫ�ز��뵽�������,
//   ÿ����һ��,��delMax()һ��,�������֮��ʣ�µĶѶ�Ԫ�ؼ�Ϊ K-Selection
//   ԭ������Ϊ��ȥ����(n-k)�����ֵ֮��,ʣ�µ����ֵ���� K-Selection
//   ���Ӷ�: O(k) + 2*(n-k)*O(logk) = O( k+2(n-k)logk)

//C : ����һ������Ѻ�һ��С����,����ͼ
/*        -------
 *       \  n-k��/
 *        \  G  /
 *         \ g /
 *          
 *         / h \
 *        /  H  \
 *       /   k�� \
 *       ---------
 */
//  һ��g<h,�ͽ����������Ѷ�,Ȼ���������,ֱ��g>=h
//  ��ʱ��h��Ϊ "K-Selection" , ԭ����   [..H..] <= h <= [..G..]
//  ���Ӷ�Ϊ: O(k) + O(n-k) + min(k,n-k)*2*O(logk + log(n-k))

//����Щ���ڶѵ�K-Selection,һ��K = n/2 , �����˻�ΪO(nlogn)

//�㷨˼��3: ���ڿ��ٻ���(�������)��Kѡȡ�㷨,��������ΪO(n^2)
//��ʹ�ü�����֮����,�ȽϿ��
template <typename T> 
void quickSelect(Vector<T> &A, Rank K) {  
  //���ѭ����������
  for (Rank lo = 0, hi = A.size() - 1; lo < hi; ++) {
    Rank i = lo, j = hi;
    T pivot = A[lo];
    //���while�ͺ���ĸ�ֵ���ʵ���Ͼ���partition,����������㷨
    //��whileʱ�临�Ӷ�ΪO(n)
    while (i < j) {
      while (i < j && pivot <= A[j]) j--;
      while (i < j && A[i] <= pivot) i++;
    }  // assert i == j
    A[i] = pivot;

    // | ... K ... |i| ... ...|
    if (K <= i) hi = i - 1;
    // | ... ... |i| ...K...|
    if (i <= K) lo = lo + 1;
  }
}

//�㷨˼��: LinearSelect,��ʹ������Ҳ����O(n),�������ĳ�ϵ��̫��,Ϊ200��,
//�⵼��ʵ���ϲ�����, ֻ��Ϊ�����㷨.
