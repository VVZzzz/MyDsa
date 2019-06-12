//���ļ���һЩͨ�õĺ�������
#ifndef FUNCOBJ_H_
#define FUNCOBJ_H_

#pragma once
// HailStone����
/*
��1������һ��������n��
��2�����n=1�������
��3�����n����������n��Ϊ3n+1������n��Ϊn/2��
��4��ת��ڣ�2������
 ����û����֤�������е��������ù��̶���ֹ
*/
template <typename T>
struct Hailstone {  //�������󣺰���Hailstone����ת��һ��T�����
  virtual void operator()(T& e) {  //����T��ֱ������������
    int step = 0;                  //ת�����貽��
    while (1 != e) {               //���桢ż��ת����ֱ��Ϊ1
      (e % 2) ? e = 3 * e + 1 : e /= 2;
      step++;
    }
    e = step;  //����ת����������
  }
};

// Double����
template <typename T>
struct Double  //�������󣺱���һ��T�����
{
  virtual void operator()(T& e) { e *= 2; }
};  //����T��ֱ�ӱ���

// Increase����
template <typename T>
struct Increase  //�������󣺵���һ��T�����
{
  virtual void operator()(T& e) { e++; }
};  //����T��ֱ�ӵ�����������++

#endif
