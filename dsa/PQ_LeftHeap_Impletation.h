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
  //这里注意,我们思考递归时,可以都假设这次递归完成,而后要做什么处理
  a->rc->parent = a;  //更新父子关系
  //如果合并后,左子树的npl小于右子树npl,则交换
  //始终保证左子树npl >= 右子树npl
  if (!a->lc || a->lc->npl < a->rc->npl) swap(a->rc, a->lc);
  //更新合并后的a的npl
  a->npl = (a->rc ? a->rc->npl : 0) + 1;
  //返回合并后的堆顶
  return a;
}//本算法只实现结构上的合并，堆的规模须由上层调用者负责更新

//插入即是合并,插入e,可以看成是和一个只有根节点为e的左式堆合并
//O(logn)
template <typename T>
void PQ_LeftHeap<T>::insert(T e) {
  BinNodePosi(T) v = new BinNode(e);
  _root = merge(_root, v);
  _root->parent = NULL;
  _size++;   //更新规模
}

//删除堆顶根节点操作,本质也是合并
/*
 *           root
 *          /    \
 *         LC     RC
 */
//即将LC和RC合并
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
