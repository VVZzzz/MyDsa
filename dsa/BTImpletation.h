#ifndef BT_IMPLETATION_H_
#define BT_IMPLETATION_H_
#pragma once
#include "BTree.h"

//����
//ʾ��(3,5)��:
/*								[53,75]

		[19,36]			               [63,69]		           [84,92]

[13,17] [27,31] [38,41,49,51]   [57,59] [65,66] [71,73]		[77,79] [89,91] [93,97,99]
*/

//�ؼ�������:    [0] [1] [2] [3] [4]
//��֧����  :  [0] [1] [2] [3] [4] [5]
//����֧������ؼ����������һ��
template <typename T>
BTNodePosi(T) BTree<T>::search(const T& e) {
  BTNodePosi(T) v = _root;  //�Ӹ��ڵ����
  _hot = NULL;
  while (v) {
    //��v����ڵ�Ĺؼ���vector���ҵ�������e����
    // int r = v->key.search(e);(��ʱ��û��ʵ��Vector�����㷨)
	//�����v�ڵ�Ĺؼ���������Ѱ��,һ��ؼ����Ǽ��ٸ�
	//ʵ�����ʹ��˳����ұȶ��ֲ���Ч��Ҫ����һ��.  
	//�������ʱ�临�Ӷ�����ΪI/O����,I/O��������ΪB���ĸ߶�
    int r = 0;
    for (; r < v->key.size(); r++) {
      if (v->key[r] == e)
        return v;
      else if (v->key[r] > e)
        break;
    }
    _hot = v;
    v = v->child
            [r];  //ע�������ǽ���һ��ڵ��Ӳ�̶����ڴ�(I/O����),ʵ����������ʱ�����.
  }
  //û�ҵ�Ŀ����ָ���ⲿ�ڵ������
  return NULL;
}

//����һ���ؼ���,�������ؼ���֮ǰû��,��϶��ǲ��뵽Ҷ�ӽڵ�
template<typename T>
bool BTree<T>::insert(const T & e) {
  BinNodePosi(T) v = search(e);
  if (v) return false;
  //��_hot�Ĺؼ��������в����µĹؼ���
  //Rank r  = _hot->key.search(e); 
  int r = 0;
  for (; r < _hot->key.size(); r++) {
     if (e < _hot->key[r])
      break;
  }
  _hot->key.insert(_hot->key.begin() + r, e);
  //��_hot�ķ�֧�����в����µķ�֧,ע��λ���±�
  //_hot->child.insert(_hot->child.begin() + (r + 1), NULL);
  //����������Ҷ�ӽڵ�,��֧��������NULL,���൱����ĩβ���һ��NULL
  _hot->child.push_back(NULL);
  
  _size++;

  //���б�Ҫ,��Ҫ������
  solveOverflow(_hot);
  return true;
}

//�������,������һ���¹ؼ���󲻷���([m/2],m)��׼
//��Ҫ���������,eg:5��B��,����37����Ҫ������
//����ֵ�ؼ���(size()/2 ��ȡ��)������ӵ����ڵ�,�������ߵĹؽ������Ϊ�����ӽڵ�
/*  
	[5] [62] [68] [77]
  [17][20][31](37)[41][56]

	[5] (37) [62] [68] [77]
	/              \
  [17][20][31]     [41][56]

  ��������,ֱ����,�����Ҳ�������������,����ֵ�ؽ����Ϊ�µĸ�
            [31]
	    /		  \
	[5]       [62] [68] [77]
	/              \
  [17][20][31]     [41][56]
*/

template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v) {
  if (_order >= v->child.size()) return;  //�ݹ��,���ؼ�������
  //ȷ��Ҫ��������,������ڵ�Ĺؼ������ﵽ��_order�״�

  //����һ���µĽڵ�,���֧�����Ѿ���һ��NULLԪ����
  //����½ڵ���Ϊ���ӽڵ�,ԭ�ڵ���Ϊ���ӽڵ�
  BTNodePosi(T) u = new BTNode<T>();
  int s = _order / 2;  //��ֵ�ؼ����±�
  u->key.insert(u->key.begin(), v->key.begin() + (s + 1), v->key.end());
  //ע���Ǹ�Ҫ����Ĺؼ��뻹��v��key��,�������һ��
  v->key.erase(v->key.begin(), v->key.begin() + (s + 1), v->key.end());

  //�Ȱ�֮ǰ�Ǹ�NULLԪ��ɾ��
  u->child.clear();
  u->child.insert()(u->key.begin(), v->child.begin() + (s + 1), v->child.end());
  v->child.erase(v->child.begin() + (s + 1), v->child.end());

  // u�ĺ����Ǹ��¸��ڵ�,ָ��u
  for (const auto& ele : u->child) ele->parent = u;

  //ԭv�ڵ�ĸ��ڵ�p
  BTNodePosi(T) p = v->parent;
  //�������֮����û�нڵ���,���ٴ���һ���½ڵ���Ϊ���ڵ�.
  if (!p) {
    _root = p = new BTNode<T>();
    p->child[0] = v;
    v->parent = p;
  }

  //��p��key������Ӧ�ò�������ؼ����λ��
  int r = 0;
  for (; r < p->key.size(); r++) {
    if (p->key[r] > v->key.back()) break;
  }
  //�ڸ��ڵ����������Ԫ��
  p->key.insert(p->key.begin() + r, v->key.back());
  //ע���ʱҪ��v�����Ԫ�����
  v->key.pop_back();
  //ָ���µ����ӽڵ�u
  p->child.insert(r + 1, u);
  u->parent = p;

  //���ϵݹ�һ��,���Ƿ��������
  solveOverflow(p);
}


//ɾ��ĳ���ؼ���
//Ҫɾ���Ĺؼ���������Ҷ�ӽڵ�
//��������������˳��ĺ�̹ؼ����滻��ȥ
//Ȼ����ɾ���Ǹ�����������Ҷ�ӹؼ���
template<typename T>
bool BTree<T>::remove(const T & e) {
  BTNodePosi(T) v = search(e);
  if (!v) return false;
  //�ҵ�Ҫɾ���Ĺؼ������
  int r = 0;
  for (; r < v->key.size(); r++) {
    if (v->key[r] == e) break;
  }

  //�������Ҷ�ӽڵ�,�������������˳��ĺ�̹ؼ���(�ض���ĳҶ�ӽڵ�ĵ�һ���ؼ���)�滻��ȥ
  //Ȼ����ɾ���Ǹ�����������Ҷ�ӹؼ���
  /*         
  ɾ��68�ؼ���,�������ڽڵ㲢����Ҷ�ӽڵ�,���������̹ؼ��뽻��,Ȼ��ɾ�����Ҷ�ӽڵ�
			[5]      [62]    [68]          [77]
			/     |        |        \            \
		[17][20] [31][37] [41][56]  [69][70][72]  [80][82]


				��ɾ��[*68*][70][72]���Ҷ�ӽڵ������ؼ���
			[5]      [62]    [69]          [77]
			/     |        |        \            \
		[17][20] [31][37] [41][56]  [*68*][70][72]  [80][82]
   */
  if (v->child[0]) {
    BTNodePosi(T) u = v->child[r + 1];
    //����ؼ����������������ؼ��뼴Ϊ�滻Ŀ��
    while (u->child[0]) u = u->child[0];
	//�������ؼ��뽻��λ��,�������ɾ�����Ҷ�ӽڵ������ؼ�����.
    v->key[r] = u->key[0];
    v = u;
    r = 0;
  }
  //����,v������ײ����,ɾ��Ҷ�ӽڵ�v��[r]�ؼ�����.
  v->key.erase(v->key.begin() + r);
  v->child.erase(v->child.begin() + (r + 1));
  _size--;

  //���п��ܻᷢ��: ɾ��һ���ؼ����,���֧�� < [m/2]��ȡ����
  //����Ҫ����ϲ�����
  solveUnderflow(v);
}

//�ܵ�˵��,���ַ�֧��< [m/2](��ȡ��)���������,�ֱ��Ӧ���ֲ���: ��ת,�ϲ�
/**
 * 1. ��ת: ��ʱ�ڵ�V��ɾ����һ���ؼ���,����֧��С��[m/2]��,����ʱ�����ֵܽڵ�(L,R)
			�Ĺؼ���ܶ�,���Դ������ֵܽڵ��һ������,���������ֵܽڵ�������Ҫ��.
			���Լ�Ҳ������Ҫ��.���ǵĸ��ڵ�Ҳ����Ҫ��.
			���ǲ���ֱ�ӽ�,��Ϊ����ƻ������˳��,Ӧ����V�踸��,���׽��ֵ�.������OK��
			Ҳ���������ϵ���ת����:
			
 *                   p: [] []  [y] [] []
							  /   \
 *       L: [>= m/2���ؼ���][x]     V: [m/2-2���ؼ���]

			��ת:
 *                   p: [] []  [*x*] [] []
							   /   \
 *       L: [>= m/2���ؼ���]        V: [*y*][m/2-2���ؼ���]


 *		ע��: ���L�ֵܲ�����,������R�ֵܽ�,ͬ����.
 */

/**
 * 2. �ϲ�: ���V���ֵ�L R�ڵ�Ĺؼ��붼������,������LR�ֵܲ���������ô����? 
		    ע���ʱ,LR�ֵܱ�����һ����,��ǡ�ú�[m/2]-1���ؼ���.(��LΪ��)
			��ô���ø��ڵ�P�Ķ�Ӧ�ؼ�������,��V�ڵ���ֵܽڵ�ϲ�!
			
 *                   p: [] []  [y] [] []
							  /   \
 *			L: [m/2 - 1���ؼ���]     V: [m/2-2���ؼ���]

			�ϲ�:

 *                   p: [] []      [] []
							    ��
 *			L: [m/2 - 1���ؼ���][y][m/2-2���ؼ���]  :V
			zh
			ע���ʱ,�ϲ���Ĵ�ڵ�,ͬ������Ҫ��,��֧���������m.

			ע��: �����ʱ,���ڵ�P��������,Ҳ������[m/2]<=��֧��<=m.
			��"�編����",�����������.ֱ�����ڵ�.������˸��ڵ㷢���������:
				          root:   [y]
							    /     \
					L: [m/2-1���ؼ���]   V: [m/2-2���ؼ���]

					�ϲ�: 

				          root:   NULL
							       ��
					L: [m/2-1���ؼ���][y][m/2-2���ؼ���] :V
					
			��ô,���������ڵ��Ϊ�µ�root,������B�����͸߶ȵ�Ψһ��ʽ


 */
template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v) {
	//���v�ķ�֧��>=(m+1)/2,�ͷ���. ��δ����(�ݹ��)
  if ((_order + 1) / 2 <= v->child.size()) return;
  BTNodePosi(T) p = v->parent;
  //pΪnull,��˵���Ѿ������˸��ڵ�
  if (!p) {
	  //������������ڵ����Ѿ�û�йؼ�����,��ֻ��Ψһһ������(������ע�͵����)
    if (!v->key.size() && v->child[0]) {
      _root = v->child[0];
      _root->parent = NULL;
      v->child[0] = NULL;
      release(v);
	  //����,Ҳ��B���߶ȼ�һ��Ψһ���.
    }
    return;
  }
  //��Ϊ�п��ܽڵ�v֮ǰ��һ���ؼ���,��ɾ����,��ʱû�йؼ���
  //�ʲ��ܰ��չؼ�����v,���밴��ָ��
  int r = 0;
  while (p->child[r] != v) r++;
  
  //���1: �����ֵܽ�ؼ���
  if (0 < r) {  //�����v���ǵ�һ������,���������ֵ�
    BTNodePosi(T) ls = p->child[r - 1];

	//������ֵ��г���Ĺؼ���
    if ((_order + 1) / 2 < ls->child.size()) {
		//ע����ת
      v->key.insert(v->key.begin(), p->key[r - 1]);
      p->key[r - 1] = ls->key.back();
      ls->key.pop_back();
	  //ͬʱ���ֵܵ����һ�����ӹ��̸�v,��Ϊv�ĵ�һ������
      v->child.insert(v->child.begin(), ls->child.back());
      ls->child.pop_back();
	  //�ı�������ӵĸ���
      if (v->child[0]) v->child[0].parent = v;
      return;  //����,����ɸò������
    }
  }  //����,���ֵ�Ϊ�ջ�����̫��

  //���2: �����ֵܽ�ؼ���
  if (p->child.size() - 1 > r) {  //�����ֵ�
    BTNodePosi(T) rs = p->child[r + 1];
	//�����ֵ��г���Ĺؼ���
    if ((_order + 1) / 2 < rs->child.size()) {
		//ע����ת,��p�Ķ�Ӧ�ؼ�����Ϊv�����ҹؼ���
      v->key.push_back(p->key[r]);
      p->key[r] = rs->key[0];
      rs->key.erase(rs->key.begin());

      //ͬʱ�����ֵܵĵ�һ�����ӹ��̸�v,��Ϊv�����һ������
      v->child.push_back(rs->child[0]);
      rs->child.pop_back();
      if (v->child.back()) v->child.back()->parent = v;
      return;  //����,����ɸò������
    }
  }//����,���ֵ�Ϊ�ջ�����̫��

  //���3: �����ֵܶ�û���㹻�Ĺؼ���,ֻ���ø��ڵ�Ĺؼ����������Һϲ�.(���������ֵ�Ϊ��(������ͬʱ))
  if (0 < r) {  //�����ֵܺϲ�
    BTNodePosi(T) ls = p->child[r - 1];
    //��p��[r-1]�ؼ���ת�����ֵ�,����p��[r]����(Ҳ����v)������p��[r]����
    ls->key.push_back(p->key[r - 1]);
    p->key.erase(p->key.begin() + (r - 1));
    p->child.erase(p->child.begin() + r);

    //ͬʱv���������(��һ������)���̸���ʱ�����ֵ�,��Ϊ���ֵܵ�
    ls->child.push_back(v->child[0]);
    v->child.erase(v->child.begin());
    if (ls->child.back()) ls->child.back()->parent = ls;
    
    //vʣ��Ĺؼ������������ת�����ֵ�
    for (auto& ele : v->key) ls->key.push_back(ele);
    for (auto& elechild : v->child) {
      ls->child.push_back(elechild);
      if (ls->child.back()) ls->child.back()->parent = ls;
    }
    v->key.clear();
    v->child.clear();
    release(v);
  } else {  //�����ֵܺϲ�
    BTNodePosi(T) rs = p->child[r + 1];
    //��p��[r]�ؼ���ת�����ֵ�,����p��[r]����(Ҳ����v)������p��[r]����
    rs->key.insert(rs->key.begin(), p->key[r]);
    p->key.erase(p->key.begin() + r);
    p->child.erase(p->child.begin() + r);

    //v�����Һ��ӹ��̸�rs,��Ϊrs��������
    rs->child.insert(rs->child.begin(), v->child.back());
    v->child.pop_back();
    if (rs->child[0]) rs->child[0]->parent = rs;

    //vʣ��Ĺؼ���ͺ�������ת�����ֵ�
    for (auto itr = v->key.rbegin(); itr != v->key.rend(); itr++)
      rs->key.insert(rs->key.begin(), *itr);
    for (auto itr = v->child.rbegin(); itr != v->child.rend(); itr++) {
      rs->child.insert(rs->child.begin(), *itr);
      if (rs->child[0]) rs->child[0]->parent = rs;
    }

    release(v);
  }

  solveUnderflow(p);  //����һ���������(�������ϲ�ʱ,����p�ڵ�ҲҪ����,����Ҫ�ݹ���һ��)
  return;
}
#endif
