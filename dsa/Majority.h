#pragma once
#include "Vector.h"

//ѡȡ�����㷨
//������ָ����һ���������� Ԫ���ظ����� ���� ����Ԫ�ظ�����һ��.
//�� > [n/2] ,ע������û�У���

//�������и����� , ����һ������λ��,����λ����һ��������
//��β�������,�ҿռ临�Ӷ�ΪO(1),ʱ�临�Ӷ�ΪO(n) ,�ж������Ƿ����
//��ѡȡ����?

template <typename T>
bool majEleCheck(Vector<T> A, T maj) {
  int c = 0;
  for (int i = 0; i < A.size(); i++)
    if (A[i] == maj) c++;
  return 2 * c > A.size();
}

template <typename T>
T majEleCandidate(Vector<T> A) {
  T maj;
  for (int i = 0, c = 0; i < A.size(); i++) {
    if (c == 0) {
      maj = A[i];
      c = 1;
    } else {
      maj == A[i] ? c++ : c--;
    }
  }
  return maj;
}

template <typename T>
bool majority(Vector<T> A, T &maj) {
  maj = majEleCandidate(A);  //��Ҫ��,ѡ��������ѡ��
  return majEleCheck(A, maj);  //�����,���ú�ѡ���Ƿ�������
}


