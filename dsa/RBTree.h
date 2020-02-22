#pragma once
//����BSTʵ��RedBlack
//���������Ҫ���������ĸ�����:
//1. ����:��Ϊ��ɫ
//2. �ⲿ�ڵ�:����Ϊ��ɫ
//3. ���ýڵ�Ϊ��ɫ,���亢�Ӻ��丸�׶�����Ϊ��ɫ
//4. ÿ�����ⲿ�ڵ㵽����·���ϵĺڽڵ����������ͬ(�ڸ߶���ͬ)
//����� == (2,4)B�� , ������lifting(�����任)ȷ����
//�����캢���������ڸ��׵���һ��,���ɱ�ΪB��

//ΪɶҪ����RB��?
//������Ϊ����AVL����˵,Ҫ��֤ƽ��,insertһ���ڵ�,ֻ����ת1�ξͻ�ﵽƽ��״̬O(1)
//��ɾ��һ���ڵ�,����Ҫ��תO(logN)��,Ҳ����˵���˽ṹ�ı���O(logN)��
//��������ֵ�Ŀ�ľ���,��insert deleteһ���ڵ�,�����˽ṹֻ��ı�O(1)��.(����Ⱦɫ�ᷢ��O(logN)��

#include "BST.h"
template <typename T> 
class RBTree : public BST<T> {
 public:
   //��ɾ�Ĳ�
  //��������BST::search();�ӿ�
  BinNodePosi(T) insert(const T& e);  //��д����
  bool remove(const T& e);  //��дɾ��
 protected:
  void solvedDoubleRed(BinNodePosi(T) x);//˫������
  void solvedDoubleBlack(BinNodePosi(T) x);//˫������
  int updateHeight(BinNodePosi(T) x);//����x�߶�
};
#include "RBTree_Impletation.h"
