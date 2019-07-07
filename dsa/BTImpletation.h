#ifndef BT_IMPLETATION_H_
#define BT_IMPLETATION_H_
#pragma once
#include "BTree.h"

//查找
//示例(3,5)树:
/*								[53,75]

		[19,36]			               [63,69]		           [84,92]

[13,17] [27,31] [38,41,49,51]   [57,59] [65,66] [71,73]		[77,79] [89,91] [93,97,99]
*/
template<typename T>
BTNodePosi(T) BTree<T>::search(const T & e) {
  BTNodePosi(T) v = _root;  //从根节点出发
  _hot = NULL;
  while (v) {
	//在v这个节点的关键码vector中找到不大于e的秩
    //int r = v->key.search(e);(暂时还没有实现Vector及其算法)
        int r = 0;
        for (; r < v->key.size(); r++) {
          if (v->key[r] >= e)
        }
  }

	
}
#endif
