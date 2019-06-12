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
    //如果有右子树,其后继必在右子树的最左节点
    s = rc;
    while (HasLChild(*s)) s = s->lc;
  } else {
    //否则是当前节点的最左祖先的祖先即:
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

/*****先序遍历*****/
// 1. 递归
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST &visit) {
  if (!x) return;
  visit(x->data);
  travPre_R(x->lc, visit);
  travPre_R(x->rc, visit);
}
// 2.迭代-1
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

// 3. 迭代-2
//首先遍历左侧链,在有底向上访问右孩子(通过上一步将右孩子入栈实现),并对每一个右孩子继续遍历左侧连
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

/*********中序遍历*************/
// 1.递归
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST &visit) {
  if (!x) return;
  travPre_R(x->lc, visit);
  visit(x->data);
  travPre_R(x->rc, visit);
}

// 2.迭代: 同样是先将左侧链入栈(因为是自底向上访问)
//   出栈的同时访问该节点,并对该节点的右子节点按同样的方法继续访问
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

// 3.迭代-2 : 为迭代-1的等价版本,没有任何区别
// 迭代-2作为迭代-3的基础
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

// 4. 迭代-3 : 为迭代-2的改进版本,不借助栈,而使用succ(),时间上要慢一点.
template <typename T, typename VST>
void travIn_I3(BinNodePosi(T) x, VST &visit) {
  //前一步是否是从右子树回溯上来的(即是否是通过succ()找来的
  bool backtrack = false;
  while (true) {
    if (!(backtrack) && HasLChild(*x)) {
      x = x->lc;
    } else {
      //上一步是右子树回溯上来(即当前节点左子树一访问,或者没有左子树.
      //可都统一认为不存在左子树
      visit(x->data);
      if (HasRChild(*x)) {
        //继续访问右子树
        x = x->rc;
        //上一步并非回溯
        backtrack = false;
      } else {
        //无右子树,该回溯去访问直接后继的那个节点了
        if (!(x = x->succ())) break;  //直接后继为null,说明遍历完了
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

/**************后序遍历********************/
// 1. 递归
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST &visit) {
  if (!x) return;
  travPost_R(x->lc, visit);
  travPost_R(x->rc, visit);
  visit(x->data);
}
// 2. 迭代 : 后序遍历即每次都是先遍历最高左侧叶子节点(HLVFL)
//    再访问该节点的右兄弟节点(如果有的话),访问它时仍然是同样的方式.
//    最后再访问他们的父节点
/**
 *    3
 *   1 2
 * 故入栈是当前节点,然后优先右子节点.再左子节点
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
  node_stack.pop();  //将栈顶的null节点弹出
}

template <typename T>
template <typename VST>
void BinNode<T>::travPost(VST &visit) {
  std::stack<BinNodePosi(T)> node_stack;
  node_stack.push(this);
  BinNodePosi(T) x = this;
  while (!node_stack.empty()) {
    //如果是栈顶节点是上一个节点的右兄弟的话(即不是上一个节点的父节点,就两种情况)
    //就继续将它的HLVFL入栈
    if (x->parent != node_stack.top()) {
      gotoHLVFL<T,VST>(node_stack);
    }
    //不是就访问
    x = node_stack.top();
    node_stack.pop();
    visit(x->data);
  }
}

/***************层序遍历*******************/
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

/********************树的通用函数**********************/
//将x子树变为只有左子树的链

template <typename T>
void strecthByZag(BinNodePosi(T) & x) {
  int c = 0;  //记录旋转次数
  int h = 0;
  BinNodePosi(T) p = x;
  while (p->rc) p = p->rc;  // p为最右子节点,也是最终的根
  while (x->lc) x = x->lc;  // x置为最左子节点,其高度必为0
  x->height = h++;
  for (; x != p; x = x->parent, x->height = h++) {
    //若右子树为空,则上升一层,同时更新height.
    while (x->rc) {
      x->zag();
      c++;
    }
  }  //当x==p时,最右子节点就是根
  printf("\nh=%d, c = %d\n\n", h, c);
}

//将x子树通过zig拉伸成只有右子树的单链
// h为子树x的规模(size),最终子树的高度为size-1
template <typename T>
void strecthByZig(BinNodePosi(T) & x, int h) {
  int c = 0;
  for (BinNodePosi(T) p = x; p = p->rc;) {
    while (p->lc) {
      p = p->zig();     //注意此处和strectchByZag不同,p =
                        //p->zig.即p会一直往上层走直到顶端
      p->height = --h;  //之后只剩右子树,相应的h减1
    }
  }
  printf("\nh=%d, c = %d\n\n", h, c);
}

#endif
