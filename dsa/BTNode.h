#ifndef BTNODE_H_
#define BTNODE_H_
#pragma once
#include <vector>
using std::vector;
#define BTNodePosi(T) BTNode<T> *
//该文件是B-Tree超级节点类
//B树节点: n个关键码,n+1个分支.
//故用vector存储关键码和分支指针
template <typename T>
struct BTNode {
  BTNodePosi(T) parent;         //父节点
  vector<T> key;                //关键码向量
  vector<BTNodePosi(T)> child;	//孩子向量(分支数总比关键码数多一个)
  //构造函数,注意BTNode只能作为根节点创建,而且初始化时有0个关键码和1个空孩子指针
  BTNode() { 
	  parent = NULL;
    child.insert(child.begin(), NULL);
  }
  //根节点,只有一个关键码以及两个孩子
  BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL) {
    parent = NULL;
    key.insert(key.begin(), e);
    child.insert(child.begin(), lc);
    child.insert(child.begin() + 1, lc);
    if (lc) lc->parent = this;
    if (rc) rc->parent = this;
  }

};
#endif
