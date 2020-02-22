#pragma once
#include "RBTree.h"
template <typename T>
BinNodePosi(T) RBTree<T>::insert(const T& e) {
  // hot始终指向最后一个失败节点,search搜索以v为根节点的子树
  //_hot为命中节点的父节点
  BinNodePosi(T)& x = search(e);
  if (x) return x;  //确认目标不存在
  x = new BinNode<T>(e, _hot, NULL, NULL, -1);
  _size++;             //创建红节点x,以_hot为父节点,黑高度为-1
  solvedDoubleRed(x);  //双红修正
  return x ? x : _hot->parent;
  //无论e是否存在于原树中，返回时总有x->data == e
}

template <typename T>
bool RBTree<T>::remove(const T& e) {
  BinNodePosi(T)& x = search(e);
  if (!x) return false;  //没有找到该节点
  BinNodePosi(T) r = removeAt(x, _hot);
  if (!(--_size)) return true;  //删除了所有节点
  // _hot某一孩子刚被删除，且被r所指节点（可能是NULL）接替。以下检查是否失衡，并做必要调整
  if (!_hot)  //若刚被删除的是根节点，则将其置黑，并更新黑高度
  {
    _root->color = RB_BLACK;
    updateHeight(_root);
    return true;
  }
  //若删除的不是根节点,_hot非空,则查看r其祖先的高度是否平衡,若平衡就不用继续处理
  if (BlackHeightUpdated(*_hot)) return true;
  //若不平衡且r是红的,则只需转为黑色,并更新高度即可
  if (IsRed(r)) {
    r->color = RB_BLACK;
    r->height++;
    return true;
  }
  //若r不是红的,则要解决双黑缺陷
  solvedDoubleBlack(r);
  //解决后返回
  return true;
}

/******************************************************************************************
 * RedBlack双红调整算法：解决节点x与其父均为红色的问题。分为两大类情况：
 *    RR-1：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 *    RR-2：3次颜色翻转，3次黑高度更新，0次旋转，需要递归
 * 即综合来看,染色可能发生O(logn)次因为由递归,但拓扑结构只会改变一次(即旋转1次)
 *
 ******************************************************************************************/
template <typename T>
void RBTree<T>::solvedDoubleRed(BinNodePosi(T) x) {
  //插入的节点x必为红色
  //若已递归至树根,则将其转黑,并且黑高度+1
  if (IsRoot(*x)) {
    _root->color = RB_BLACK;
    _root->height++;
    return;
  }
  BinNodePosi(T) p = x->parent;
  //如果插入节点的父节点为黑色,则return
  if (p->color == RB_BLACK) return;
  //若p为红色,则其父节点g必为黑
  BinNodePosi(T) g = p->parent;
  // u即为g的另一个儿子
  BinNodePosi(T) u = uncle(x);
  //根据u的颜色分情况处理
  if (IsBlack(u)) {
    // u为黑色(包含NULL)
    // x和p在同侧(zig-zig , zag-zag)
    if (IsLChild(*x) == IsLChild(*p)) {
      p->color = RB_BLACK;  // p由红变黑
    } else {
      x->color = RB_BLACK;
    }
    // g由黑变红
    g->color = RB_RED;
    //曾祖父
    BinNodePosi(T) gg = g->parent;
    BinNodePosi(T) r = FromParentTo(*g) =
        rotateAt(x);  //将x-p-g进行旋转,即"3+4"重构
    r->parent = gg;
  } else {
    //若u为红色
    //将p,u由红转黑
    p->color = RB_BLACK;
    p->height++;
    u->color = RB_BLACK;
    u->height++;
    //若此时g不是根,则变为红色
    if (!IsRoot(*g)) g->color = RB_RED;
    //此时可能g的父节点也是红的,仍要解决双红缺陷
    //递归继续调整g
    solvedDoubleRed(g);
  }
}

/******************************************************************************************
 * RedBlack双黑调整算法：解决节点x与被其替代的节点均为黑色的问题
 * 分为三大类共四种情况：
 *    BB-1 ：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 *    BB-2R：2次颜色翻转，2次黑高度更新，0次旋转，不再递归
 *    BB-2B：1次颜色翻转，1次黑高度更新，0次旋转，需要递归
 *    BB-3 ：2次颜色翻转，2次黑高度更新，1次旋转，转为BB-1或BB2R
 ******************************************************************************************/
template <typename T>
void RBTree<T>::solvedDoubleBlack(BinNodePosi(T) r) {
  // r的父节点p
  BinNodePosi(T) p = (r ? r->parent : _hot);
  if (!p) return;
  // r的兄弟节点s
  BinNodePosi(T) s = ((r == p->lc) ? p->rc : p->lc);
  if (IsBlack(s)) {
    // s的红孩子（若左、右孩子皆红，左者优先；皆黑时为NULL）
    BinNodePosi(T) t = NULL;
    if (IsRed(s->rc)) t = s->rc;
    if (IsRed(s->lc)) t = s->lc;
    if (t) {
      // BB-1: 黑s有至少一个红孩子(无须递归)
      RBColor oldColor = p->color;  //备份原子树p的颜色
      //将t p 染成黑色,并对t - s - p做以下旋转
      BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);  //旋转(即3+4重构)
      if (HasLChild(*b)) {
        b->lc->color = RB_BLACK;
        updateHeight(b->lc);
      }
      if (HasRChild(*b)) {
        b->rc->color = RB_BLACK;
        updateHeight(b->rc);
      }
      //新子树根节点继承原根节点的颜色
      b->color = oldColor;
      updateHeight(b);
    } else {
      //黑s无红孩子
      if (IsRed(p)) {
        // BB-2r: p为红色(无须递归)
        // s转为红,p转为黑
        s->color = RB_RED;
        s->height--;
        //p的黑高度不变
        p->color = RB_BLACK;  
      } else {
        //BB-2B: p为黑色
        // s转为红
        s->color = RB_RED;
        s->height--;
        //p的黑高度--,这是因为在B树中这个P节点下溢导致上层下溢,故黑高度--且要递归上一层
        p->height--;
        solvedDoubleBlack(p);
      }
    }
  } else {
    //兄弟节点s为红: BB-3
    //s由红转黑,p由黑砖红
    s->color = RB_BLACK;
    p->color = RB_RED;
    //得到和s在一侧的儿子t,用来做zig-zig旋转
    BinNodePosi(T) t = (IsLChild(s) ? s->lc : s->rc);
    _hot = p;
    FromParentTo(*p) = rotateAt(t);
    //rotateAt中有更新高度
    //此时情况转为BB-1或者BB-2R,绝不可能是BB-2B(因为p是红色)
    /**
         S(b)
        /  \
    t1(b)   P(r)
           / \
        t2(b)  x
               \
                r
     */
    solvedDoubleBlack(r);
  }
}

template <typename T>
int RBTree<T>::updateHeight(BinNodePosi(T) x) {
  x->height =
      max(stature(x->lc), stature(x->rc));  //孩子高度一般相等,除非出现双黑
  /*DSA*/  // 红黑树中各节点左、右孩子的黑高度通常相等
  /*DSA*/  // 这里之所以取更大值，是便于在删除节点后的平衡调整过程中，正确更新被删除节点父亲的黑高度
  /*DSA*/  // 否则，rotateAt()会根据被删除节点的替代者（高度小一）设置父节点的黑高度
  return IsBlack(x) ? x->height++ : x->height;
  //因统一定义stature(NULL) =
  //-1，故height比黑高度少一，好在不致影响到各种算法中的比较判断
}
