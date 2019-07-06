#ifndef SPLAY_IMPLETATION_H_
#define SPLAY_IMPLETATION_H_
#pragma once
#include "SPlay.h"
//���������,������
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
//���ڵ�v��չ������
//��չ��������:
//zig-zag �� zag-zig���κ�BST��һ��,���Ƕ���zig-zig��zag-zag��չ�����Լ��Ĳ���
//BST�ж���zig-zig��zag-zag���ǰ���p,g������,ÿ�ζ���v����һ���߶�,ֱ��������
//����չ�����ȶ�g��ת,�ڶ�p��ת,�Ӷ���v
/*			BST�ж�zig-zig����
	   g   ---��תp--->    g    ---��תg--->    v 
	  /	\				 /	\			      /  \
	 p	 z				v	 z			     w	  g
    / \				   / \						/  \
   v   y			  w	  p						p	z
  / \					 / \				  /  \
 w	x					x	y				 x	  y
*/

/*			��չ���ж�zig-zig����
	   g   ---��תg--->    p    ---��תp--->    v 
	  /	\				 /	\			      /  \
	 p	 z				v	 g			     w	  p
    / \				   / \	/ \					 / \
   v   y			  w	  x	y  z				x	g
  / \					    				       / \
 w	x					 	 				  	  y   z
*/
//zag-zagһ��
template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
  if (!v) return NULL;
  //���׽ڵ���游�ڵ�
  //v g p������չ
  BinNodePosi(T) g;
  BinNodePosi(T) p;
  while ((p = v->parent) && (g = p->parent)) {
    BinNodePosi(T) gg = g->parent;
    if (IsLChild(*v)) {
      if (IsLChild(*p)) {  // zig-zig
        //ֱ��������3+4�ṹ
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
  //������û���游,ֻ�и���,��Ҫ����һ�ε���
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
  //��չ֮��,v�ڵ�Ϊ���ڵ�,������
  v->parent = NULL;
  return v;
}

template<typename T>
BinNodePosi(T)& Splay<T>::search(const T & e) {
  BinNodePosi(T) p = _search_iter(_root, e, _hot = NULL);
  _root = splay(p ? p : _hot);
  return _root;
//������BST��ͬ�����۲��ҳɹ����_root��ָ����󱻷��ʵĽڵ�
}


//Ϊ��չ������½ڵ�
//����search,search�Ὣ�����ʵĽڵ������ڵ�.
//������ڸ��ڵ�==e����,����
/*    root     ---����--->     e
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
  //�ҵ���ֱ�ӷ���
  if (e == search(e) -> data) return _root;
  //����search,root�Ѿ������һ�η��ʵĽڵ���
  _size++;
  BinNodePosi(T) t = _root;
  if (_root->data < e) {
    t->parent = _root = new BinNode<T>(e, NULL, t, t->rc);
	//����ԭroot�ĸ���,����ָ��
    if (t->rc) {
      t->rc->parent = _root;
      t->rc = NULL;
    }
  } else {
    t->parent = _root = new BinNode<T>(e, NULL, t->lc, t);
	//����ԭroot�ĸ���,����ָ��
    if (t->lc) {
      t->lc->parent = _root;
      t->lc = NULL;
    }
  }
  updateHeightAbove(t);
  //���ز���Ľڵ�
  return _root;
}


//ɾ��ĳһ�ڵ�
//����search,�ὫҪɾ���Ľڵ���չ�����ڵ���.
//����,Ҫɾ��������ڵ�,�µĸ��ڵ�涨�������������������.
//����������ڵ�ķ����Ƚ������������search,���¸��ڵ���չ��root��.
template<typename T>
bool Splay<T>::remove(const T & e) {
	//��������û���ҵ�Ŀ��ڵ�
  if (!_root || e != search(e)->data) return false;
  //����search,Ҫɾ���Ľڵ�����_root
  BinNodePosi(T) toDeleteNode = _root;
  if (!HasLChild(*_root)) {  //��������
    _root = _root->rc;
    if (_root) _root->parent = NULL;
  } else if (!HasRChild(*_root)){  //��������
    _root = _root->lc;
    if (_root) _root->parent = NULL;
  } else {  //������������
	  //��ʱ��ժ��������
    BinNodePosi(T) lTree = _root->lc;
    lTree->parent = NULL;
	//��_root��������������
    _root = _root->rc;
    _root->parent = NULL;
	//����һ�αض���ʧ�ܵĲ���,Ŀ���ǽ��涨���µļ�������չ������
    _root = search(toDeleteNode->data);
	//��ʱ�������Ѿ�λ,attch������������
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
