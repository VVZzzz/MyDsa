#ifndef SPLAY_IMPLETATION_H_
#define SPLAY_IMPLETATION_H_
#pragma once
#include "SPlay.h"
//添加左子树,右子树
template <typename NodePosi>
inline void attachAsLchild(NodePosi p, NodePosi rchild) {
  p->rc = rchild;
  if (rchild) rchild->parent = p;
}

template <typename NodePosi>
inline void attachAsRchild(NodePosi p, NodePosi lchild) {
  p->lc = lchild;
  if (lchild) lchild->parent = p;
}
//将节点v伸展至树根
//伸展策略如下:
//zig-zag 和 zag-zig情形和BST的一样,但是对于zig-zig和zag-zag伸展树有自己的策略
//BST中对于zig-zig和zag-zag都是按照p,g逐层调整,每次都让v上升一个高度,直至到树根
//而伸展树中先对g旋转,在对p旋转,从而让v
/*			BST中对zig-zig做法
	   g   ---旋转p--->    g    ---旋转g--->    v 
	  /	\				 /	\			      /  \
	 p	 z				v	 z			     w	  g
    / \				   / \						/  \
   v   y			  w	  p						p	z
  / \					 / \				  /  \
 w	x					x	y				 x	  y
*/

/*			伸展树中对zig-zig做法
	   g   ---旋转g--->    p    ---旋转p--->    v 
	  /	\				 /	\			      /  \
	 p	 z				v	 g			     w	  p
    / \				   / \	/ \					 / \
   v   y			  w	  x	y  z				x	g
  / \					    				       / \
 w	x					 	 				  	  y   z
*/
//zag-zag一样
template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
  if (!v) return NULL;
  //父亲节点和祖父节点
  //v g p进行伸展
  BinNodePosi(T) g;
  BinNodePosi(T) p;
  while ((p = v->parent) && (g = p->parent)) {
    BinNodePosi(T) gg = g->parent;
    if (IsLChild(*v)) {
      if (IsLChild(*p)) {  // zig-zig
        //直接用类似3+4结构
        attachAsLchild(g, p->rc);
        attachAsLchild(p, v->rc);
        attachAsRchild(p, g);
        attachAsRchild(v, p);
      } else {  // zig-zag
        attachAsRchild(g, v->lc);
        attachAsLchild(p, v->rc);
        attachAsLchild(v, g);
        attachAsRchild(v, p);
      }
    } else {
      if (IsRChild(*p)) {  // zag-zag
        attachAsRchild(g, p->lc);
        attachAsLchild(p, g);
        attachAsRchild(p, v->lc);
        attachAsLchild(v, p);
      } else {             // zag-zig
        attachAsRchild(p, v->lc);
        attachAsLchild(g, v->rc);
        attachAsLchild(v, p);
        attachAsRchild(v, g);
      }
    }
    if (!gg)
      v->parent = NULL;
    else {
      (g == gg->lc) ? attachAsLchild(gg, v) : attachAsRchild(gg, v);
    }
    updateHeight(g);
    updateHeight(p);
    updateHeight(v);
  }
  //如果最后没有祖父,只有父亲,还要再做一次单旋
  if (p = v->parent) {
    if (IsLChild(*v)) {
      attachAsLchild(p, v->rc);
      attachAsRchild(v, p);
    } else {
      attachAsRchild(p, v->lc);
      attachAsLchild(v, p);
    }
    updateHeight(p);
    updateHeight(v);
  }
  //伸展之后,v节点为根节点,并返回
  v->parent = NULL;
  return v;
}

template<typename T>
BinNodePosi(T)& Splay<T>::search(const T & e) {
  BinNodePosi(T) p = _search_iter(_root, e, _hot = NULL);
  _root = splay(p ? p : _hot);
  return _root;
//与其它BST不同，无论查找成功与否，_root都指向最后被访问的节点
}


//为伸展树添加新节点
//首先search,search会将最后访问的节点放入根节点.
//如果现在根节点==e返回,否则
/*    root     ---插入--->     e
      /  \                   /  \
	 A    B				   root  B
						   /  
						  A

*/
template<typename T>
BinNodePosi(T) Splay<T>::insert(const T & e) {
  if (!_root) {
    _size++;
    return (_root = new BinNode<T>(e));
  }
  //找到就直接返回
  if (e == search(e) -> data) return _root;
  //经过search,root已经是最后一次访问的节点了
  _size++;
  BinNodePosi(T) t = _root;
  if (_root->data < e) {
    t->parent = _root = new BinNode<T>(e, NULL, t, t->rc);
	//更新原root的父亲,儿子指针
    if (t->rc) {
      t->rc->parent = _root;
      t->rc = NULL;
    }
  } else {
    t->parent = _root = new BinNode<T>(e, NULL, t->lc, t);
	//更新原root的父亲,儿子指针
    if (t->lc) {
      t->lc->parent = _root;
      t->lc = NULL;
    }
  }
  updateHeightAbove(t);
  //返回插入的节点
  return _root;
}


//删除某一节点
//首先search,会将要删除的节点伸展到根节点上.
//接着,要删除这个根节点,新的根节点规定用右子树的最左结点替代.
//这里找最左节点的方法比较巧妙的是仍用search,将新根节点伸展到root上.
template<typename T>
bool Splay<T>::remove(const T & e) {
	//空树或者没有找到目标节点
  if (!_root || e != search(e)->data) return false;
  //经过search,要删除的节点已至_root
  BinNodePosi(T) toDeleteNode = _root;
  if (!HasLChild(*_root)) {  //无左子树
    _root = _root->rc;
    if (_root) _root->parent = NULL;
  } else if (!HasRChild(*_root)){  //无右子树
    _root = _root->lc;
    if (_root) _root->parent = NULL;
  } else {  //左右子树都有
	  //暂时先摘除左子树
    BinNodePosi(T) lTree = _root->lc;
    lTree->parent = NULL;
	//将_root与其右子树分离
    _root = _root->rc;
    _root->parent = NULL;
	//进行一次必定会失败的查找,目的是将规定的新的继任者伸展至树根
    _root = search(toDeleteNode->data);
	//此时继任者已就位,attch左右子树即可
    _root->lc = lTree;
    lTree->parent = _root;
  }
  release(toDeleteNode->data);
  release(toDeleteNode);
  _size--;
  if (_root) updateHeight(_root);
  return true;
}
#endif
