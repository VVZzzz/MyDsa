#ifndef BSTIMPLETATION_H_
#define BSTIMPLETATION_H_
#pragma once

//注意,若节点为NULL也意味着命中
#define EQUAL(e, v) (!(v) || ((v)->data) == (e))

//迭代搜索
// hot始终指向最后一个失败节点,search搜索以v为根节点的子树
template <typename T>
    static BinNodePosi(T) &
    _search_iter(BinNodePosi(T) & v, const T& e, BinNodePosi(T) & hot) {
  if (EQUAL(e, v)) return v;
  hot = v;
  while (true) {
    BinNodePosi(T)& c = ((e < hot->data) ? hot->lc : hot->rc);
    if (EQUAL(e, c)) return c;
    hot = c;
  }
}

//递归搜索
template <typename T>
    static BinNodePosi(T) &
    _search_recur(BinNodePosi(T) & v, const T& e, BinNodePosi(T) & hot) {
  if (EQUAL(e, v)) return v;
  hot = v;
  BinNodePosi(T)& c = ((e < hot->data) ? hot->lc : hot->rc);
  _search_recur(c, e, hot);
}

/*  3+4变换,即3个节点(abc),和其4个子节点T0-4
 *  他们按照如下排列
 *			b
 *		  /   \
 *		 a     c
 *      / \   / \
 *     T0 T1 T2  T3
 *
 */

template <typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
                         BinNodePosi(T) T0, BinNodePosi(T)T1, BinNodePosi(T)T2,
                         BinNodePosi(T) T3) {
  a->lc = T0;
  if (T0) T0->parent = a;
  a->rc = T1;
  if (T1) T1->parent = a;
  updateHeight(a);
  c->lc = T2;
  if (T2) T2->parent = c;
  c->rc = T3;
  if (T3) T3->parent = c;
  updateHeight(c);
  b->lc = a;
  b->rc = c;
  a->parent = b;
  c->parent = b;
  updateHeight(b);
  return b;
}



/******************************************************************************************
 * BST节点旋转变换统一算法（3节点 + 4子树），返回调整之后局部子树根节点的位置
 * 用于AVL树平衡等
 * 注意：尽管子树根会正确指向上层节点（如果存在），但反向的联接须由上层函数完成
 ******************************************************************************************/

/*
 *      zig-zig    zig-zag	zag-zag		zag-zig
 *		  g		    g		   g			g
 *		 /		   /			\			 \
 *		p		  p				 p			  p
 *     /		   \			  \			 / 
 *	  v				v			   v		v
 *
 */
template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) {  //v为非空孙子节点
  if (!v) {
    puts("\a\nFail to rotate a null node.\n");
    exit(-1);
  }
  BinNodePosi(T) p = v->parent;
  BinNodePosi(T) g = p->parent;
  if (IsLChild(*p)) {  //zig
    if (IsLChild(*v)) {  //zig-zig
      p->parent = g->parent;  //向上连接,注意rotateAt不向下连接(即无g->parent->lc/rc = xxx)
      return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
    } else {   //zig-zag
      v->parent = g->parent;  
      return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
    }
  } else {  //zag
    if (IsRChild(*v)) {  //zag-zag
      p->parent = g->parent;  
      return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
    } else {  //zag-zig
      v->parent = g->parent;  
      return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
    }
  }
}


template <typename T>
BinNodePosi(T) & BST<T>::search(const T& e) {
  return _search_iter(this->_root, e, _hot = NULL);
}

template <typename T>
BinNodePosi(T) BST<T>::insert(const T& e) {
  //puts("BST<T>::insert! \n");
  BinNodePosi(T)& x = search(e);
  //若找到已有对应数据,则返回那个节点
  if (x) return x;
  x = new BinNode<T>(e, _hot);
  this->_size++;
  this->updateHeightAbove(x);
  return x;
}


/******************************************************************************************
 * BST节点删除算法：删除位置x所指的节点（全局静态模板函数，适用于AVL、Splay、RedBlack等各种BST）
 * 目标x在此前经查找定位，并确认非NULL，故必删除成功；与searchIn不同，调用之前不必将hot置空
 * 返回值指向实际被删除节点的接替者，hot指向实际被删除节点的父亲——二者均有可能是NULL
 ******************************************************************************************/
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot) {
	//实际被删除的节点
  BinNodePosi(T) w = x;
  //替代被删除节点位置的节点
  BinNodePosi(T) succ = NULL;
  if (!HasLChild(*x)) succ = x = x->rc;
  else if (!HasRChild(*x)) succ = x = x->lc;
  else {
    w = w->succ();
    std::swap(x->data, w->data);
    BinNodePosi(T) tmp = w->parent;
	//隔离节点w
    ((tmp == x) ? tmp->rc : tmp->lc) = succ = w->rc;
  }
  hot = w->parent;
  if (succ) succ->parent = hot;
  release(w->data);
  release(w);
  return succ;
}


template <typename T>
bool BST<T>::remove(const T& e) {
  BinNodePosi(T)& x = search(e);
  if (!x) return false;
  removeAt(x, _hot);
  this->_size--;
  this->updateHeightAbove(_hot);
  return true;
  
}
#endif
