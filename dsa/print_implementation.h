#ifndef PRINT_IMPLEMENTATION_H_
#define PRINT_IMPLEMENTATION_H_
#pragma once
#include "Bitmap.h"
#include "print.h"

#define ROOT 0
#define L_CHILD 1
#define R_CHILD -1 * L_CHILD

//基本类型
void UniPrint::p(int e) { printf(" %04d", e); }
void UniPrint::p(float e) { printf(" %4.1f", e); }
void UniPrint::p(double e) { printf(" %4.1f", e); }
void UniPrint::p(char e) { printf(" %c", (31 < e) && (e < 128) ? e : '$'); }

// BinNode节点
template <typename T>
void UniPrint::p(BinNode<T>& node) {
  p(node.data);  //数值
#if defined(DSA_LEFTHEAP)
  printf("(%-2d)", node.npl);  // NPL
#elif defined(DSA_BST)
  printf("(%-2d)", node.height);  //高度
#elif defined(DSA_AVL)
  printf("(%-2d)", node.height);                 //高度
#elif defined(DSA_REDBLACK)
  printf("(%-2d)", node.height);  //高度
#elif defined(DSA_SPLAY)
  printf("(%-2d)", node.height);  //高度
#endif
  /******************************************************************************************
   * 父子链接指针
   ******************************************************************************************/
  printf(((node.lc && &node != node.lc->parent) ||
          (node.rc && &node != node.rc->parent))
             ? "@"
             : " ");
  /******************************************************************************************
   * 节点颜色
   ******************************************************************************************/
#if defined(DSA_REDBLACK)
  printf(node.color == RB_BLACK ? "B" : " ");  //（忽略红节点）
#endif
  /******************************************************************************************
   * 父子（黑）高度、NPL匹配
   ******************************************************************************************/
#if defined(DSA_PQ_COMPLHEAP)
  //高度不必匹配
#elif defined(DSA_PQ_LEFTHEAP)
  printf(                         // NPL
      (node.rc && node.npl != 1 + node.rc->npl) ||
              (node.lc && node.npl > 1 + node.lc->npl)
          ? "%%"
          : " ");
#elif defined(DSA_REDBLACK)
  printf(BlackHeightUpdated(node) ? " " : "!");  //黑高度
#else
                                  // printf(HeightUpdated(node) ? " " : "!");  //（常规）高度
#endif
  /******************************************************************************************
   * 左右平衡
   ******************************************************************************************/
#if defined(DSA_AVL)
  if (!AvlBalanced(node))
    printf("X");  // AVL平衡
  else if (0 < BalFac(node))
    printf("\\");  // AVL平衡
  else if (BalFac(node) < 0)
    printf("/");  // AVL平衡
  else
    printf("-");  // AVL平衡
#elif defined(DSA_REDBLACK)
  if (!Balanced(node))
    printf("X");  // RB平衡
  else if (0 < BalFac(node))
    printf("\\");  // RB平衡
  else if (BalFac(node) < 0)
    printf("/");  // RB平衡
  else
    printf("-");  // RB平衡
#else
                                                 //平衡无所谓
#endif
}

/******************************************************************************************
 * 基础BinTree
 ******************************************************************************************/
template <typename T>
void UniPrint::p(BinTree<T>& bt) {
  printf("%s[%d]*%d:\n", typeid(bt).name(), &bt, bt.size());  //基本信息
  Bitmap* branchType = new Bitmap;  //记录当前节点祖先的方向
  printBinTree(bt.root(), -1, ROOT, branchType);  //树状结构
  release(branchType);
  printf("\n");
}

/******************************************************************************************
 * 基于BinTree实现的BST
 ******************************************************************************************/
template <typename T>
void UniPrint::p(BST<T>& bt) {
   printf ( "%s[%d]*%d:\n", typeid ( bt ).name(), &bt, bt.size() ); //基本信息
   Bitmap* branchType = new Bitmap; //记录当前节点祖先的方向
   printBinTree ( bt.root(), -1, ROOT, branchType ); //树状结构
   release ( branchType ); printf ( "\n" );

}

/******************************************************************************************
 * 二叉树各种派生类的统一打印
 ******************************************************************************************/
template <typename T>  //元素类型
static void printBinTree(BinNodePosi(T) bt, int depth, int type,
                         Bitmap* bType) {
  if (!bt) return;
  if (-1 < depth)  //设置当前层的拐向标志
    R_CHILD == type ? bType->set(depth) : bType->clear(depth);
  printBinTree(bt->rc, depth + 1, R_CHILD, bType);  //右子树（在上）
  print(bt);
  printf(" *");
  for (int i = -1; i < depth; i++)  //根据相邻各层
    if ((0 > i) ||
        bType->test(i) == bType->test(i + 1))  //的拐向是否一致，即可确定
      printf("      ");                        //是否应该
    else
      printf("│    ");  //打印横线
  switch (type) {
    case R_CHILD:
      printf("┌─");
      break;
    case L_CHILD:
      printf("└─");
      break;
    default:
      printf("──");
      break;  // root
  }
  print(bt);
#if defined(DSA_HUFFMAN)
  if (IsLeaf(*bt)) bType->print(depth + 1);  //输出Huffman编码
#endif
  printf("\n");
  printBinTree(bt->lc, depth + 1, L_CHILD, bType);  //左子树（在下）
}

#endif
