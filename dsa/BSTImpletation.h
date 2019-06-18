#ifndef BSTIMPLETATION_H_
#define BSTIMPLETATION_H_
#pragma once

//ע��,���ڵ�ΪNULLҲ��ζ������
#define EQUAL(e, v) (!(v) || ((v)->data) == (e))

//��������
// hotʼ��ָ�����һ��ʧ�ܽڵ�,search������vΪ���ڵ������
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

//�ݹ�����
template <typename T>
    static BinNodePosi(T) &
    _search_recur(BinNodePosi(T) & v, const T& e, BinNodePosi(T) & hot) {
  if (EQUAL(e, v)) return v;
  hot = v;
  BinNodePosi(T)& c = ((e < hot->data) ? hot->lc : hot->rc);
  _search_recur(c, e, hot);
}

/*  3+4�任,��3���ڵ�(abc),����4���ӽڵ�T0-4
 *  ���ǰ�����������
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
 * BST�ڵ���ת�任ͳһ�㷨��3�ڵ� + 4�����������ص���֮��ֲ��������ڵ��λ��
 * ����AVL��ƽ���
 * ע�⣺��������������ȷָ���ϲ�ڵ㣨������ڣ�������������������ϲ㺯�����
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
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) {  //vΪ�ǿ����ӽڵ�
  if (!v) {
    puts("\a\nFail to rotate a null node.\n");
    exit(-1);
  }
  BinNodePosi(T) p = v->parent;
  BinNodePosi(T) g = p->parent;
  if (IsLChild(*p)) {  //zig
    if (IsLChild(*v)) {  //zig-zig
      p->parent = g->parent;  //��������,ע��rotateAt����������(����g->parent->lc/rc = xxx)
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
  //���ҵ����ж�Ӧ����,�򷵻��Ǹ��ڵ�
  if (x) return x;
  x = new BinNode<T>(e, _hot);
  this->_size++;
  this->updateHeightAbove(x);
  return x;
}


/******************************************************************************************
 * BST�ڵ�ɾ���㷨��ɾ��λ��x��ָ�Ľڵ㣨ȫ�־�̬ģ�庯����������AVL��Splay��RedBlack�ȸ���BST��
 * Ŀ��x�ڴ�ǰ�����Ҷ�λ����ȷ�Ϸ�NULL���ʱ�ɾ���ɹ�����searchIn��ͬ������֮ǰ���ؽ�hot�ÿ�
 * ����ֵָ��ʵ�ʱ�ɾ���ڵ�Ľ����ߣ�hotָ��ʵ�ʱ�ɾ���ڵ�ĸ��ס������߾��п�����NULL
 ******************************************************************************************/
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot) {
	//ʵ�ʱ�ɾ���Ľڵ�
  BinNodePosi(T) w = x;
  //�����ɾ���ڵ�λ�õĽڵ�
  BinNodePosi(T) succ = NULL;
  if (!HasLChild(*x)) succ = x = x->rc;
  else if (!HasRChild(*x)) succ = x = x->lc;
  else {
    w = w->succ();
    std::swap(x->data, w->data);
    BinNodePosi(T) tmp = w->parent;
	//����ڵ�w
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
