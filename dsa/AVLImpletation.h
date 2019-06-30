#ifndef AVL_IMPLETATION_H_
#define AVL_IMPLETATION_H_
#pragma once
#include "AVL.h"
template <typename T>
BinNodePosi(T) AVL<T>::insert(const T& e) {
  BinNodePosi(T)& x = search(e);
  //�ҵ��ͷ���,û�ҵ�xҲ��ָ��Ӧ�����λ��
  if (x) return x;
  BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);
  _size++;
  for (BinNodePosi(T) g = _hot; g; g = g->parent) {
    if (!AvlBalanced(*g)) {
      //����g,g�Ķ���,g������
      FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
      // g����󣬾ֲ������߶ȱ�Ȼ��ԭ�������������ˣ��ʵ����漴����
      //ע�������AVL��remove������,AVL��һ��ժ����ĳ���ڵ����ʧ��,���丸�ڵ��游�ȶ�����ʧ��.
      break;
    } else {
      //��ʹƽ��,ҲҪ���¸߶�(ע����Ҫ���Ǹղ����½ڵ�ʱ�����,���ζ��丸�׸��¸߶�)
      updateHeight(g);
    }
  }
  //���ز���ڵ�
  return xx;
}
template <typename T>
bool AVL<T>::remove(const T& e) {
  BinNodePosi(T)& x = search(e);
  if (NULL == x) return false;
  //����BST�е�remove����ժ���ڵ�,�ټ�������.
  removeAt(x, _hot);
  _size--;
  for (BinNodePosi(T) g = _hot; g; g = g->parent) {
    if (!AvlBalanced(*g)) {
      g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
    }
	//��ʹ��ʧ��,g�߶�Ҳ���ܽ���
    updateHeight(g);
  }//�ʿ���ҪOmege(logn)
  return true;
}
#endif
