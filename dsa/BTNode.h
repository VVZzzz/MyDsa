#ifndef BTNODE_H_
#define BTNODE_H_
#pragma once
#include <vector>
using std::vector;
#define BTNodePosi(T) BTNode<T> *
//���ļ���B-Tree�����ڵ���
//B���ڵ�: n���ؼ���,n+1����֧.
//����vector�洢�ؼ���ͷ�ָ֧��
template <typename T>
struct BTNode {
  BTNodePosi(T) parent;         //���ڵ�
  vector<T> key;                //�ؼ�������
  vector<BTNodePosi(T)> child;	//��������(��֧���ܱȹؼ�������һ��)
  //���캯��,ע��BTNodeֻ����Ϊ���ڵ㴴��,���ҳ�ʼ��ʱ��0���ؼ����1���պ���ָ��
  BTNode() { 
	  parent = NULL;
    child.insert(child.begin(), NULL);
  }
  //���ڵ�,ֻ��һ���ؼ����Լ���������
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
