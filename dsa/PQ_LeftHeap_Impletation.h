#pragma once
#include "PQ_LeftHeap.h"
//递归策略,合并两个左式堆a,b
//这里要求,a堆的根值总是大于b堆的根值,以便将堆b合并到堆a上
/*
 *       a                  b
 *     /   \              /   \ 
 *    aRC   aRL          bRC   bRL
 *
 *       a                       b
 *     /   \                   /   \ 
 *    aRC      ->  aRL  ->    bRC   bRL
 *
 *  即将a的右子树递归合并到b上,aRC = merge(aRL,b)
 *  就这么简单的算法,但是我们要注意一些细节
 */
template <typename T>
BinNodePosi(T) PQ_LeftHeap<T>::merge(BinNodePosi(T) a, BinNodePosi(T) b) {
  //递归基 
  if (!a) return b;
  if (!b) return a;
  //保证a永远指向那个根植较大的左式堆
  if (lt(a->data, b->data)) swap(a, b);
  a->rc = merge(a->rc, b);  
  //假设递归完成(已经合并完成了,之后要做一些细节处理)
}
