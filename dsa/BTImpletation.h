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
template<typename T>
BTNodePosi(T) BTree<T>::search(const T & e) {
  BTNodePosi(T) v = _root;  //�Ӹ��ڵ����
  _hot = NULL;
  while (v) {
	//��v����ڵ�Ĺؼ���vector���ҵ�������e����
    //int r = v->key.search(e);(��ʱ��û��ʵ��Vector�����㷨)
        int r = 0;
        for (; r < v->key.size(); r++) {
          if (v->key[r] >= e)
        }
  }

	
}
#endif
