//该文件是一些通用的函数对象
#ifndef FUNCOBJ_H_
#define FUNCOBJ_H_

#pragma once
// HailStone序列
/*
（1）输入一个正整数n；
（2）如果n=1则结束；
（3）如果n是奇数，则n变为3n+1，否则n变为n/2；
（4）转入第（2）步。
 至今没有人证明对所有的正整数该过程都终止
*/
template <typename T>
struct Hailstone {  //函数对象：按照Hailstone规则转化一个T类对象
  virtual void operator()(T& e) {  //假设T可直接做算术运算
    int step = 0;                  //转换所需步数
    while (1 != e) {               //按奇、偶逐步转换，直至为1
      (e % 2) ? e = 3 * e + 1 : e /= 2;
      step++;
    }
    e = step;  //返回转换所经步数
  }
};

// Double函数
template <typename T>
struct Double  //函数对象：倍增一个T类对象
{
  virtual void operator()(T& e) { e *= 2; }
};  //假设T可直接倍增

// Increase函数
template <typename T>
struct Increase  //函数对象：递增一个T类对象
{
  virtual void operator()(T& e) { e++; }
};  //假设T可直接递增或已重载++

#endif
