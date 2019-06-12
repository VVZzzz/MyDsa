#ifndef BINNODE_IMPLETATION_H_
#define BINNODE_IMPLETATION_H_
#pragma once
#include "BinNode.h"
template <typename T>
int BinNode<T>::size() {
  int s = 1;
  if (lc) s += lc->size();
  if (rc) s += rc->size();
  return s;
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(const T &e) {
  return lc = new BinNode<T>(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(const T &e) {
  return rc = new BinNode<T>(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ() {
  BinNodePosi(T) s = this;
  if (rc) {
    //�����������,���̱���������������ڵ�
    s = rc;
    while (HasLChild(*s)) s = s->lc;
  } else {
    //�����ǵ�ǰ�ڵ���������ȵ����ȼ�:
    /*
                    4
                  /
                1
                 \
                  2
                   \
                   (3)
         */
    while (IsRChild(*s)) s = s->parent;
    s = s->parent;
  }

  return s;
}

/**
 *    x1
 *   /
 *  x2
 * / \
 * a  b
 *
 *  x2
 * / \
 *a  x1
 *   /
 *  b
 */

template <typename T>
BinNodePosi(T) BinNode<T>::zig() {
  BinNodePosi(T) lChild = lc;
  lChild->parent = this->parent;
  if (lChild->parent) {
    ((this->parent->lc == this) ? lChild->parent->lc : lChild->parent->rc) =
        lChild;
  }
  lc = lChild->rc;
  if (lc) lc->parent = this;
  lChild->rc = this;
  this->parent = lChild;
  return lChild;
}

/**
 *    x1
 *      \
 *       x2
 *		 / \
 *		 a  b
 *
 *	  x2
 *	 / \
 *	x1  b
 *   \
 *    a
 *
 *
 */

template <typename T>
BinNodePosi(T) BinNode<T>::zag() {
  BinNodePosi(T) rChild = rc;
  rChild->parent = this->parent;
  if (rChild->parent) {
    ((this->parent->lc == this) ? rChild->parent->lc : rChild->parent->rc) =
        rChild;
  }
  rc = rChild->lc;
  if (rc) rc->parent = this;
  rChild->lc = this;
  this->parent = rChild;
  return rChild;
}

/*****�������*****/
// 1. �ݹ�
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST &visit) {
  if (!x) return;
  visit(x->data);
  travPre_R(x->lc, visit);
  travPre_R(x->rc, visit);
}
// 2.����-1
template <typename T, typename VST>
void travPre_I1(BinNodePosi(T) x, VST &visit) {
  std::stack<BinNodePosi(T)> node_stack;
  if (x) node_stack.push(x);
  while (!node_stack.empty()) {
    x = node_stack.top();
    node_stack.pop();
    visit(x->data);
    if (HasRChild(*x)) node_stack.push(x->rc);
    if (HasLChild(*x)) node_stack.push(x->lc);
  }
}

// 3. ����-2
//���ȱ��������,���е����Ϸ����Һ���(ͨ����һ�����Һ�����ջʵ��),����ÿһ���Һ��Ӽ������������
template <typename T, typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) x, VST &visit,
                                 std::stack<BinNodePosi(T)> &node_stack) {
  while (x) {
    visit(x->data);
    node_stack.push(x->rc);
    x = x->lc;
  }
}

template <typename T, typename VST>
void travPre_I2(BinNodePosi(T) x, VST &visit) {
  std::stack<BinNodePosi(T)> node_stack;
  while (true) {
    visitAlongLeftBranch(x, visit, node_stack);
    if (node_stack.empty()) break;
    x = node_stack.top();
    node_stack.pop();
  }
}

template <typename T>
template <typename VST>
void BinNode<T>::travPre(VST &visit) {
  switch (rand() % 3) {
    case 1:
      travPre_I1(this, visit);
      break;
    case 2:
      travPre_I2(this, visit);
      break;
    default:
      travPre_R(this, visit);
      break;
  }
}

/*********�������*************/
// 1.�ݹ�
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST &visit) {
  if (!x) return;
  travPre_R(x->lc, visit);
  visit(x->data);
  travPre_R(x->rc, visit);
}

// 2.����: ͬ�����Ƚ��������ջ(��Ϊ���Ե����Ϸ���)
//   ��ջ��ͬʱ���ʸýڵ�,���Ըýڵ�����ӽڵ㰴ͬ���ķ�����������
template <typename T>
static void goAlongLeftBranch(BinNodePosi(T) x,
                              std::stack<BinNodePosi(T)> &node_stack) {
  while (x) {
    node_stack.push(x);
    x = x->lc;
  }
}

template <typename T, typename VST>
void travIn_I1(BinNodePosi(T) x, VST &visit) {
  std::stack<BinNodePosi(T)> node_stack;
  while (true) {
    goAlongLeftBranch(x, node_stack);
    if (node_stack.empty()) break;
    x = node_stack.top();
    node_stack.pop();
    visit(x->data);
    x = x->rc;
  }
}

// 3.����-2 : Ϊ����-1�ĵȼ۰汾,û���κ�����
// ����-2��Ϊ����-3�Ļ���
template <typename T, typename VST>
void travIn_I2(BinNodePosi(T) x, VST &visit) {
  std::stack<BinNodePosi(T)> node_stack;
  while (true) {
    if (x) {
      node_stack.push(x);
      x = x->lc;
    } else if (!node_stack.empty()) {
      x = node_stack.top();
      node_stack.pop();
      visit(x->data);
      x = x->rc;
    } else
      break;
  }
}

// 4. ����-3 : Ϊ����-2�ĸĽ��汾,������ջ,��ʹ��succ(),ʱ����Ҫ��һ��.
template <typename T, typename VST>
void travIn_I3(BinNodePosi(T) x, VST &visit) {
  //ǰһ���Ƿ��Ǵ�����������������(���Ƿ���ͨ��succ()������
  bool backtrack = false;
  while (true) {
    if (!(backtrack) && HasLChild(*x)) {
      x = x->lc;
    } else {
      //��һ������������������(����ǰ�ڵ�������һ����,����û��������.
      //�ɶ�ͳһ��Ϊ������������
      visit(x->data);
      if (HasRChild(*x)) {
        //��������������
        x = x->rc;
        //��һ�����ǻ���
        backtrack = false;
      } else {
        //��������,�û���ȥ����ֱ�Ӻ�̵��Ǹ��ڵ���
        if (!(x = x->succ())) break;  //ֱ�Ӻ��Ϊnull,˵����������
        backtrack = true;
      }
    }
  }
}

template <typename T>
template <typename VST>
void BinNode<T>::travIn(VST &visit) {
  switch (rand() % 4) {
    case 1:
      travIn_I1(this, visit);
      break;
    case 2:
      travIn_I2(this, visit);
      break;
    case 3:
      travIn_I3(this, visit);
      break;
    default:
      travIn_R(this, visit);
      break;
  }
}
/*****************************************/

/**************�������********************/
// 1. �ݹ�
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST &visit) {
  if (!x) return;
  travPost_R(x->lc, visit);
  travPost_R(x->rc, visit);
  visit(x->data);
}
// 2. ���� : ���������ÿ�ζ����ȱ���������Ҷ�ӽڵ�(HLVFL)
//    �ٷ��ʸýڵ�����ֵܽڵ�(����еĻ�),������ʱ��Ȼ��ͬ���ķ�ʽ.
//    ����ٷ������ǵĸ��ڵ�
/**
 *    3
 *   1 2
 * ����ջ�ǵ�ǰ�ڵ�,Ȼ���������ӽڵ�.�����ӽڵ�
 */
template <typename T, typename VST>
static void gotoHLVFL(std::stack<BinNodePosi(T)> &node_stack) {
  BinNodePosi(T) x;
  while (x = node_stack.top()) {
    if (HasLChild(*x)) {
      if (HasRChild(*x)) {
        node_stack.push(x->rc);
      }
      node_stack.push(x->lc);
    } else {
      node_stack.push(x->rc);
    }
  }
  node_stack.pop();  //��ջ����null�ڵ㵯��
}

template <typename T>
template <typename VST>
void BinNode<T>::travPost(VST &visit) {
  std::stack<BinNodePosi(T)> node_stack;
  node_stack.push(this);
  BinNodePosi(T) x = this;
  while (!node_stack.empty()) {
    //�����ջ���ڵ�����һ���ڵ�����ֵܵĻ�(��������һ���ڵ�ĸ��ڵ�,���������)
    //�ͼ���������HLVFL��ջ
    if (x->parent != node_stack.top()) {
      gotoHLVFL<T,VST>(node_stack);
    }
    //���Ǿͷ���
    x = node_stack.top();
    node_stack.pop();
    visit(x->data);
  }
}

/***************�������*******************/
template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST &visit) {
  std::queue<BinNodePosi(T)> node_queue;
  BinNodePosi(T) x = this;
  node_queue.push(x);
  while (!node_queue.empty()) {
    x = node_queue.front();
    node_queue.pop();
    visit(x->data);
    if (HasLChild(*x)) node_queue.push(x->lc);
    if (HasRChild(*x)) node_queue.push(x->rc);
  }
}

/********************����ͨ�ú���**********************/
//��x������Ϊֻ������������

template <typename T>
void strecthByZag(BinNodePosi(T) & x) {
  int c = 0;  //��¼��ת����
  int h = 0;
  BinNodePosi(T) p = x;
  while (p->rc) p = p->rc;  // pΪ�����ӽڵ�,Ҳ�����յĸ�
  while (x->lc) x = x->lc;  // x��Ϊ�����ӽڵ�,��߶ȱ�Ϊ0
  x->height = h++;
  for (; x != p; x = x->parent, x->height = h++) {
    //��������Ϊ��,������һ��,ͬʱ����height.
    while (x->rc) {
      x->zag();
      c++;
    }
  }  //��x==pʱ,�����ӽڵ���Ǹ�
  printf("\nh=%d, c = %d\n\n", h, c);
}

//��x����ͨ��zig�����ֻ���������ĵ���
// hΪ����x�Ĺ�ģ(size),���������ĸ߶�Ϊsize-1
template <typename T>
void strecthByZig(BinNodePosi(T) & x, int h) {
  int c = 0;
  for (BinNodePosi(T) p = x; p = p->rc;) {
    while (p->lc) {
      p = p->zig();     //ע��˴���strectchByZag��ͬ,p =
                        //p->zig.��p��һֱ���ϲ���ֱ������
      p->height = --h;  //֮��ֻʣ������,��Ӧ��h��1
    }
  }
  printf("\nh=%d, c = %d\n\n", h, c);
}

#endif
