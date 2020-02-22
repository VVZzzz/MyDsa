#pragma once
#include "RBTree.h"
template <typename T>
BinNodePosi(T) RBTree<T>::insert(const T& e) {
  // hotʼ��ָ�����һ��ʧ�ܽڵ�,search������vΪ���ڵ������
  //_hotΪ���нڵ�ĸ��ڵ�
  BinNodePosi(T)& x = search(e);
  if (x) return x;  //ȷ��Ŀ�겻����
  x = new BinNode<T>(e, _hot, NULL, NULL, -1);
  _size++;             //������ڵ�x,��_hotΪ���ڵ�,�ڸ߶�Ϊ-1
  solvedDoubleRed(x);  //˫������
  return x ? x : _hot->parent;
  //����e�Ƿ������ԭ���У�����ʱ����x->data == e
}

template <typename T>
bool RBTree<T>::remove(const T& e) {
  BinNodePosi(T)& x = search(e);
  if (!x) return false;  //û���ҵ��ýڵ�
  BinNodePosi(T) r = removeAt(x, _hot);
  if (!(--_size)) return true;  //ɾ�������нڵ�
  // _hotĳһ���Ӹձ�ɾ�����ұ�r��ָ�ڵ㣨������NULL�����档���¼���Ƿ�ʧ�⣬������Ҫ����
  if (!_hot)  //���ձ�ɾ�����Ǹ��ڵ㣬�����úڣ������ºڸ߶�
  {
    _root->color = RB_BLACK;
    updateHeight(_root);
    return true;
  }
  //��ɾ���Ĳ��Ǹ��ڵ�,_hot�ǿ�,��鿴r�����ȵĸ߶��Ƿ�ƽ��,��ƽ��Ͳ��ü�������
  if (BlackHeightUpdated(*_hot)) return true;
  //����ƽ����r�Ǻ��,��ֻ��תΪ��ɫ,�����¸߶ȼ���
  if (IsRed(r)) {
    r->color = RB_BLACK;
    r->height++;
    return true;
  }
  //��r���Ǻ��,��Ҫ���˫��ȱ��
  solvedDoubleBlack(r);
  //����󷵻�
  return true;
}

/******************************************************************************************
 * RedBlack˫������㷨������ڵ�x���丸��Ϊ��ɫ�����⡣��Ϊ�����������
 *    RR-1��2����ɫ��ת��2�κڸ߶ȸ��£�1~2����ת�����ٵݹ�
 *    RR-2��3����ɫ��ת��3�κڸ߶ȸ��£�0����ת����Ҫ�ݹ�
 * ���ۺ�����,Ⱦɫ���ܷ���O(logn)����Ϊ�ɵݹ�,�����˽ṹֻ��ı�һ��(����ת1��)
 *
 ******************************************************************************************/
template <typename T>
void RBTree<T>::solvedDoubleRed(BinNodePosi(T) x) {
  //����Ľڵ�x��Ϊ��ɫ
  //���ѵݹ�������,����ת��,���Һڸ߶�+1
  if (IsRoot(*x)) {
    _root->color = RB_BLACK;
    _root->height++;
    return;
  }
  BinNodePosi(T) p = x->parent;
  //�������ڵ�ĸ��ڵ�Ϊ��ɫ,��return
  if (p->color == RB_BLACK) return;
  //��pΪ��ɫ,���丸�ڵ�g��Ϊ��
  BinNodePosi(T) g = p->parent;
  // u��Ϊg����һ������
  BinNodePosi(T) u = uncle(x);
  //����u����ɫ���������
  if (IsBlack(u)) {
    // uΪ��ɫ(����NULL)
    // x��p��ͬ��(zig-zig , zag-zag)
    if (IsLChild(*x) == IsLChild(*p)) {
      p->color = RB_BLACK;  // p�ɺ���
    } else {
      x->color = RB_BLACK;
    }
    // g�ɺڱ��
    g->color = RB_RED;
    //���游
    BinNodePosi(T) gg = g->parent;
    BinNodePosi(T) r = FromParentTo(*g) =
        rotateAt(x);  //��x-p-g������ת,��"3+4"�ع�
    r->parent = gg;
  } else {
    //��uΪ��ɫ
    //��p,u�ɺ�ת��
    p->color = RB_BLACK;
    p->height++;
    u->color = RB_BLACK;
    u->height++;
    //����ʱg���Ǹ�,���Ϊ��ɫ
    if (!IsRoot(*g)) g->color = RB_RED;
    //��ʱ����g�ĸ��ڵ�Ҳ�Ǻ��,��Ҫ���˫��ȱ��
    //�ݹ��������g
    solvedDoubleRed(g);
  }
}

/******************************************************************************************
 * RedBlack˫�ڵ����㷨������ڵ�x�뱻������Ľڵ��Ϊ��ɫ������
 * ��Ϊ�����๲���������
 *    BB-1 ��2����ɫ��ת��2�κڸ߶ȸ��£�1~2����ת�����ٵݹ�
 *    BB-2R��2����ɫ��ת��2�κڸ߶ȸ��£�0����ת�����ٵݹ�
 *    BB-2B��1����ɫ��ת��1�κڸ߶ȸ��£�0����ת����Ҫ�ݹ�
 *    BB-3 ��2����ɫ��ת��2�κڸ߶ȸ��£�1����ת��תΪBB-1��BB2R
 ******************************************************************************************/
template <typename T>
void RBTree<T>::solvedDoubleBlack(BinNodePosi(T) r) {
  // r�ĸ��ڵ�p
  BinNodePosi(T) p = (r ? r->parent : _hot);
  if (!p) return;
  // r���ֵܽڵ�s
  BinNodePosi(T) s = ((r == p->lc) ? p->rc : p->lc);
  if (IsBlack(s)) {
    // s�ĺ캢�ӣ������Һ��ӽԺ죬�������ȣ��Ժ�ʱΪNULL��
    BinNodePosi(T) t = NULL;
    if (IsRed(s->rc)) t = s->rc;
    if (IsRed(s->lc)) t = s->lc;
    if (t) {
      // BB-1: ��s������һ���캢��(����ݹ�)
      RBColor oldColor = p->color;  //����ԭ����p����ɫ
      //��t p Ⱦ�ɺ�ɫ,����t - s - p��������ת
      BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);  //��ת(��3+4�ع�)
      if (HasLChild(*b)) {
        b->lc->color = RB_BLACK;
        updateHeight(b->lc);
      }
      if (HasRChild(*b)) {
        b->rc->color = RB_BLACK;
        updateHeight(b->rc);
      }
      //���������ڵ�̳�ԭ���ڵ����ɫ
      b->color = oldColor;
      updateHeight(b);
    } else {
      //��s�޺캢��
      if (IsRed(p)) {
        // BB-2r: pΪ��ɫ(����ݹ�)
        // sתΪ��,pתΪ��
        s->color = RB_RED;
        s->height--;
        //p�ĺڸ߶Ȳ���
        p->color = RB_BLACK;  
      } else {
        //BB-2B: pΪ��ɫ
        // sתΪ��
        s->color = RB_RED;
        s->height--;
        //p�ĺڸ߶�--,������Ϊ��B�������P�ڵ����絼���ϲ�����,�ʺڸ߶�--��Ҫ�ݹ���һ��
        p->height--;
        solvedDoubleBlack(p);
      }
    }
  } else {
    //�ֵܽڵ�sΪ��: BB-3
    //s�ɺ�ת��,p�ɺ�ש��
    s->color = RB_BLACK;
    p->color = RB_RED;
    //�õ���s��һ��Ķ���t,������zig-zig��ת
    BinNodePosi(T) t = (IsLChild(s) ? s->lc : s->rc);
    _hot = p;
    FromParentTo(*p) = rotateAt(t);
    //rotateAt���и��¸߶�
    //��ʱ���תΪBB-1����BB-2R,����������BB-2B(��Ϊp�Ǻ�ɫ)
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
      max(stature(x->lc), stature(x->rc));  //���Ӹ߶�һ�����,���ǳ���˫��
  /*DSA*/  // ������и��ڵ����Һ��ӵĺڸ߶�ͨ�����
  /*DSA*/  // ����֮����ȡ����ֵ���Ǳ�����ɾ���ڵ���ƽ����������У���ȷ���±�ɾ���ڵ㸸�׵ĺڸ߶�
  /*DSA*/  // ����rotateAt()����ݱ�ɾ���ڵ������ߣ��߶�Сһ�����ø��ڵ�ĺڸ߶�
  return IsBlack(x) ? x->height++ : x->height;
  //��ͳһ����stature(NULL) =
  //-1����height�Ⱥڸ߶���һ�����ڲ���Ӱ�쵽�����㷨�еıȽ��ж�
}
