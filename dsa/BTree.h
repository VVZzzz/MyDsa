#ifndef BTREE_H_
#define BTREE_H_
#pragma once
#include "BTNode.h"
//���ļ���BTree��
//m��B��: ���ڸ��ڵ� 2<=n+1 . ���������ڲ��ڵ�: [m/2](��ȡ��)<=n+1
//n:     �Ǹýڵ�Ĺؼ�����
//�����([m/2],m)��,����(2,4)��������
//B���߶�: ��"�ⲿ�ڵ�"����ȶ�����

//����B����Ŀ��: ����I/O����
//����: B����һ���ڵ����ж���ؼ���,���ж����֧��,��ʹ��һ�β��ҾͿ��Խ�����ؼ����ȡ����.
//������ΪB���Ľڵ㶼����Ӳ��(ͬһ���洢���)��,ȥ����һ��Ҫ�����ܼ��ٲ��Ҵ����������ܶཫ���ݶ����ڴ�.
//����: ������1G����¼,����AVLҪ����log(2,10^9)��IO����.��ÿ��ֻ�ܶ���һ���ؼ���
//������B��,ÿ���ڵ㶼��һ��ؼ���.һ��ؼ������ɴ������ݿ��С����,��Ϊ����KB,һ���ؼ���Ϊ4B
//��һ���ڵ�ؼ�����200-300��,��Ϊ256��.��: ������ͬ������ֻ��Ҫlog(256,10^9) = 4��I/O
//4��I/O �������� 30��I/O

//ʾ��(3,5)��:
/*								[53,75]

		[19,36]			               [63,69]		           [84,92]

[13,17] [27,31] [38,41,49,51]   [57,59] [65,66] [71,73]		[77,79] [89,91] [93,97,99]
*/
//ʧ�ܲ��ұ���ֹ���ⲿ�ڵ�
//�ⲿ�ڵ�Ҳ��������һ���Ĵ洢�ṹ

//B�������߶ȷ���: ����N���ؼ����m��B��,��߸߶�Ϊ����?
//�������ڵ��������Ϊ��0��,�ⲿ�ڵ��������Ϊ��h��.
//�ڲ��ڵ�Ӧ�þ����ܵ�"��",�ʸ��ڵ���1�����,��1����2���ڵ�,�Դ�����,��2����,2*[m/2]���ڵ�
//��k����2*[m/2]^(k-1)��ڵ�.
//������֪��: �ڲ��ڵ㹲N���ؼ���,��ô�ⲿ�ڵ�(�����һ��)����N+1���ڵ�.
//����: N���ڲ��ڵ��ӦN�ֲ��ҳɹ�,��ôһ������N+1��ʧ�ܵ����.
//����: ��h��������2*[m/2]^(h-1) <= N+1
//h <= 1 + log( [m/2],L(N+1)/2| ) = O(log(m,N))
//�Ա�BBST��: log([m/2],N/2) / log(2,N) = 1/(log(2,m)-1)
//ȡm = 256, B���߶Ƚ���Ϊ1/7

//B������С�߶ȷ���: ����N���ؼ����m��B��,��͸߶�Ϊ����?
//�������ڵ��������Ϊ��0��,�ⲿ�ڵ��������Ϊ��h��.
//�ڲ��ڵ�Ӧ�þ����ܵ�"��",�ʸ��ڵ���1��,��1����m��,��2����m^2��,�Դ�����
//��k����m^k��.
//��h����: N+1 <= m^h  
//h >= log(m,(N+1)) = ��(log(m,N))
//�����:(log(m,N)-1)/log(2,N) = 1/log(2,m)
//ȡm=256,���߽���1/8

//����B�������Ͻ��½綼��log(m,N),��B���߶ȱ仯����ʮ������.

template <typename T>
class BTree {
 protected:
  int _size;                           //��ŵĹؼ�����
  int _order;                          // B���Ľ״�,����Ϊ3
  BTNodePosi(T) _root;                 //���ڵ�
  BTNodePosi(T) _hot;                  // search�����ʵķǿսڵ�
  void solveOverflow(BTNodePosi(T));   //����������֮��ķ��Ѵ���
  void solveUnderflow(BTNodePosi(T));  //��ɾ��������֮��ĺϲ�����
 public:
  BTree(int order = 3) : _order(order), _size(0) { _root = new BTNode<T>(); }
  ~BTree() {
    if (_root) release(_root);
  }
  int const order() { return _order; }
  int const size() { return _size; }
  BTNodePosi(T) & root() { return _root; }  //����
  bool empty() const { return !_root; }     //�п�
  BTNodePosi(T) search(const T& e);         //����
  bool insert(const T& e);                  //����
  bool remove(const T& e);                  //ɾ��
};
#endif