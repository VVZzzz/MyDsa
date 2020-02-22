#pragma once
//基于BST实现RedBlack
//红黑树定义要满足以下四个条件:
//1. 树根:必为黑色
//2. 外部节点:必须为黑色
//3. 若该节点为红色,则其孩子和其父亲都必须为黑色
//4. 每条从外部节点到根的路径上的黑节点个数必须相同(黑高度相同)
//红黑树 == (2,4)B树 , 这是由lifting(上升变换)确定的
//即将红孩子提升到黑父亲的那一层,即可变为B树

//为啥要出现RB树?
//这是因为对于AVL树来说,要保证平衡,insert一个节点,只会旋转1次就会达到平衡状态O(1)
//而删除一个节点,可能要旋转O(logN)次,也就是说拓扑结构改变了O(logN)次
//红黑树出现的目的就是,让insert delete一个节点,其拓扑结构只会改变O(1)次.(尽管染色会发生O(logN)次

#include "BST.h"
template <typename T> 
class RBTree : public BST<T> {
 public:
   //增删改查
  //可以沿用BST::search();接口
  BinNodePosi(T) insert(const T& e);  //重写插入
  bool remove(const T& e);  //重写删除
 protected:
  void solvedDoubleRed(BinNodePosi(T) x);//双红修正
  void solvedDoubleBlack(BinNodePosi(T) x);//双黑修正
  int updateHeight(BinNodePosi(T) x);//更新x高度
};
#include "RBTree_Impletation.h"
