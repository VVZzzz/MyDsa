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

//关键码向量:    [0] [1] [2] [3] [4]
//分支向量  :  [0] [1] [2] [3] [4] [5]
//即分支向量与关键码向量相错一个
template <typename T>
BTNodePosi(T) BTree<T>::search(const T& e) {
  BTNodePosi(T) v = _root;  //从根节点出发
  _hot = NULL;
  while (v) {
    //在v这个节点的关键码vector中找到不大于e的秩
    // int r = v->key.search(e);(暂时还没有实现Vector及其算法)
	//这个在v节点的关键码向量中寻找,一般关键码是几百个
	//实验表明使用顺序查找比二分查找效率要更高一点.  
	//但总体的时间复杂度主体为I/O操作,I/O操作次数为B树的高度
    int r = 0;
    for (; r < v->key.size(); r++) {
      if (v->key[r] == e)
        return v;
      else if (v->key[r] > e)
        break;
    }
    _hot = v;
    v = v->child
            [r];  //注意这里是将下一层节点从硬盘读入内存(I/O操作),实际中是消耗时间最长的.
  }
  //没找到目标以指向外部节点而结束
  return NULL;
}

//插入一个关键码,如果这个关键码之前没有,则肯定是插入到叶子节点
template<typename T>
bool BTree<T>::insert(const T & e) {
  BinNodePosi(T) v = search(e);
  if (v) return false;
  //在_hot的关键码向量中插入新的关键码
  //Rank r  = _hot->key.search(e); 
  int r = 0;
  for (; r < _hot->key.size(); r++) {
     if (e < _hot->key[r])
      break;
  }
  _hot->key.insert(_hot->key.begin() + r, e);
  //在_hot的分支向量中插入新的分支,注意位置下标
  //_hot->child.insert(_hot->child.begin() + (r + 1), NULL);
  //但是由于是叶子节点,分支向量都是NULL,故相当于在末尾添加一个NULL
  _hot->child.push_back(NULL);
  
  _size++;

  //如有必要,需要做分裂
  solveOverflow(_hot);
  return true;
}

//上溢分裂,即插入一个新关键码后不符合([m/2],m)标准
//需要做上溢调整,eg:5阶B树,插入37后需要做调整
//将中值关键码(size()/2 下取整)上升添加到父节点,左右两边的关节码各成为左右子节点
/*  
	[5] [62] [68] [77]
  [17][20][31](37)[41][56]

	[5] (37) [62] [68] [77]
	/              \
  [17][20][31]     [41][56]

  继续上溢,直至根,如果根也不满足继续上溢,让中值关节码称为新的根
            [31]
	    /		  \
	[5]       [62] [68] [77]
	/              \
  [17][20][31]     [41][56]
*/

template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v) {
  if (_order >= v->child.size()) return;  //递归基,不必继续上溢
  //确定要发生上溢,即这个节点的关键码数达到了_order阶次

  //创建一个新的节点,其分支向量已经有一个NULL元素了
  //这个新节点作为右子节点,原节点作为左子节点
  BTNodePosi(T) u = new BTNode<T>();
  int s = _order / 2;  //中值关键码下标
  u->key.insert(u->key.begin(), v->key.begin() + (s + 1), v->key.end());
  //注意那个要上溢的关键码还在v的key中,且是最后一个
  v->key.erase(v->key.begin(), v->key.begin() + (s + 1), v->key.end());

  //先把之前那个NULL元素删了
  u->child.clear();
  u->child.insert()(u->key.begin(), v->child.begin() + (s + 1), v->child.end());
  v->child.erase(v->child.begin() + (s + 1), v->child.end());

  // u的孩子们更新父节点,指向u
  for (const auto& ele : u->child) ele->parent = u;

  //原v节点的父节点p
  BTNodePosi(T) p = v->parent;
  //如果上溢之后发现没有节点了,就再创建一个新节点作为根节点.
  if (!p) {
    _root = p = new BTNode<T>();
    p->child[0] = v;
    v->parent = p;
  }

  //在p的key向量中应该插入上溢关键码的位置
  int r = 0;
  for (; r < p->key.size(); r++) {
    if (p->key[r] > v->key.back()) break;
  }
  //在父节点中添加上溢元素
  p->key.insert(p->key.begin() + r, v->key.back());
  //注意此时要把v的溢出元素清除
  v->key.pop_back();
  //指向新的右子节点u
  p->child.insert(r + 1, u);
  u->parent = p;

  //向上递归一层,看是否继续上溢
  solveOverflow(p);
}


//删除某个关键码
//要删除的关键码若不是叶子节点
//则就找其中序遍历顺序的后继关键码替换上去
//然后再删除那个被缓过来的叶子关键码
template<typename T>
bool BTree<T>::remove(const T & e) {
  BTNodePosi(T) v = search(e);
  if (!v) return false;
  //找到要删除的关键码的秩
  int r = 0;
  for (; r < v->key.size(); r++) {
    if (v->key[r] == e) break;
  }

  //如果不是叶子节点,就找其中序遍历顺序的后继关键码(必定是某叶子节点的第一个关键码)替换上去
  //然后再删除那个被缓过来的叶子关键码
  /*         
  删除68关键码,由于所在节点并非是叶子节点,故它就与后继关键码交换,然后删除后继叶子节点
			[5]      [62]    [68]          [77]
			/     |        |        \            \
		[17][20] [31][37] [41][56]  [69][70][72]  [80][82]


				即删除[*68*][70][72]这个叶子节点的最左关键码
			[5]      [62]    [69]          [77]
			/     |        |        \            \
		[17][20] [31][37] [41][56]  [*68*][70][72]  [80][82]
   */
  if (v->child[0]) {
    BTNodePosi(T) u = v->child[r + 1];
    //这个关键码的右子树的最左关键码即为替换目标
    while (u->child[0]) u = u->child[0];
	//将两个关键码交换位置,下面就是删除这个叶子节点的最左关键码了.
    v->key[r] = u->key[0];
    v = u;
    r = 0;
  }
  //至此,v总是最底层的了,删除叶子节点v的[r]关键码了.
  v->key.erase(v->key.begin() + r);
  v->child.erase(v->child.begin() + (r + 1));
  _size--;

  //但有可能会发生: 删除一个关键码后,其分支数 < [m/2]上取整了
  //所以要上溢合并调整
  solveUnderflow(v);
}

//总的说来,出现分支数< [m/2](上取整)有两种情况,分别对应两种操作: 旋转,合并
/**
 * 1. 旋转: 此时节点V已删除了一个关键码,但分支数小于[m/2]个,而此时它的兄弟节点(L,R)
			的关键码很多,可以从它的兄弟节点借一个出来,这样它的兄弟节点仍满足要求.
			而自己也满足了要求.他们的父节点也满足要求.
			但是不能直接借,因为这会破坏中序的顺序,应该是V借父亲,父亲借兄弟.这样就OK了
			也就是意义上的旋转操作:
			
 *                   p: [] []  [y] [] []
							  /   \
 *       L: [>= m/2个关键码][x]     V: [m/2-2个关键码]

			旋转:
 *                   p: [] []  [*x*] [] []
							   /   \
 *       L: [>= m/2个关键码]        V: [*y*][m/2-2个关键码]


 *		注意: 如果L兄弟不够借,可以找R兄弟借,同样的.
 */

/**
 * 2. 合并: 如果V的兄弟L R节点的关键码都不够借,或者是LR兄弟不存在那怎么办呢? 
		    注意此时,LR兄弟必有其一存在,且恰好含[m/2]-1个关键码.(以L为例)
			那么就让父节点P的对应关键码下来,与V节点和兄弟节点合并!
			
 *                   p: [] []  [y] [] []
							  /   \
 *			L: [m/2 - 1个关键码]     V: [m/2-2个关键码]

			合并:

 *                   p: [] []      [] []
							    ↓
 *			L: [m/2 - 1个关键码][y][m/2-2个关键码]  :V
			zh
			注意此时,合并后的大节点,同样满足要求,分支数不会大于m.

			注意: 如果此时,父节点P由于下溢,也不满足[m/2]<=分支数<=m.
			那"如法炮制",依次逐层下溢.直至根节点.如果到了根节点发生这种情况:
				          root:   [y]
							    /     \
					L: [m/2-1个关键码]   V: [m/2-2个关键码]

					合并: 

				          root:   NULL
							       ↓
					L: [m/2-1个关键码][y][m/2-2个关键码] :V
					
			那么,就让这个大节点成为新的root,这样是B树降低高度的唯一方式


 */
template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T)) {
	
}
#endif
