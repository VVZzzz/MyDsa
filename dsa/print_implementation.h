#ifndef PRINT_IMPLEMENTATION_H_
#define PRINT_IMPLEMENTATION_H_
#pragma once
#include "Bitmap.h"
#include "print.h"

#define ROOT 0
#define L_CHILD 1
#define R_CHILD -1 * L_CHILD

//��������
void UniPrint::p(int e) { printf(" %04d", e); }
void UniPrint::p(float e) { printf(" %4.1f", e); }
void UniPrint::p(double e) { printf(" %4.1f", e); }
void UniPrint::p(char e) { printf(" %c", (31 < e) && (e < 128) ? e : '$'); }

// BinNode�ڵ�
template <typename T>
void UniPrint::p(BinNode<T>& node) {
  p(node.data);  //��ֵ
#if defined(DSA_LEFTHEAP)
  printf("(%-2d)", node.npl);  // NPL
#elif defined(DSA_BST)
  printf("(%-2d)", node.height);  //�߶�
#elif defined(DSA_AVL)
  printf("(%-2d)", node.height);                 //�߶�
#elif defined(DSA_REDBLACK)
  printf("(%-2d)", node.height);  //�߶�
#elif defined(DSA_SPLAY)
  printf("(%-2d)", node.height);  //�߶�
#endif
  /******************************************************************************************
   * ��������ָ��
   ******************************************************************************************/
  printf(((node.lc && &node != node.lc->parent) ||
          (node.rc && &node != node.rc->parent))
             ? "@"
             : " ");
  /******************************************************************************************
   * �ڵ���ɫ
   ******************************************************************************************/
#if defined(DSA_REDBLACK)
  printf(node.color == RB_BLACK ? "B" : " ");  //�����Ժ�ڵ㣩
#endif
  /******************************************************************************************
   * ���ӣ��ڣ��߶ȡ�NPLƥ��
   ******************************************************************************************/
#if defined(DSA_PQ_COMPLHEAP)
  //�߶Ȳ���ƥ��
#elif defined(DSA_PQ_LEFTHEAP)
  printf(                         // NPL
      (node.rc && node.npl != 1 + node.rc->npl) ||
              (node.lc && node.npl > 1 + node.lc->npl)
          ? "%%"
          : " ");
#elif defined(DSA_REDBLACK)
  printf(BlackHeightUpdated(node) ? " " : "!");  //�ڸ߶�
#else
                                  // printf(HeightUpdated(node) ? " " : "!");  //�����棩�߶�
#endif
  /******************************************************************************************
   * ����ƽ��
   ******************************************************************************************/
#if defined(DSA_AVL)
  if (!AvlBalanced(node))
    printf("X");  // AVLƽ��
  else if (0 < BalFac(node))
    printf("\\");  // AVLƽ��
  else if (BalFac(node) < 0)
    printf("/");  // AVLƽ��
  else
    printf("-");  // AVLƽ��
#elif defined(DSA_REDBLACK)
  if (!Balanced(node))
    printf("X");  // RBƽ��
  else if (0 < BalFac(node))
    printf("\\");  // RBƽ��
  else if (BalFac(node) < 0)
    printf("/");  // RBƽ��
  else
    printf("-");  // RBƽ��
#else
                                                 //ƽ������ν
#endif
}

/******************************************************************************************
 * ����BinTree
 ******************************************************************************************/
template <typename T>
void UniPrint::p(BinTree<T>& bt) {
  printf("%s[%d]*%d:\n", typeid(bt).name(), &bt, bt.size());  //������Ϣ
  Bitmap* branchType = new Bitmap;  //��¼��ǰ�ڵ����ȵķ���
  printBinTree(bt.root(), -1, ROOT, branchType);  //��״�ṹ
  release(branchType);
  printf("\n");
}

/******************************************************************************************
 * ����BinTreeʵ�ֵ�BST
 ******************************************************************************************/
template <typename T>
void UniPrint::p(BST<T>& bt) {
   printf ( "%s[%d]*%d:\n", typeid ( bt ).name(), &bt, bt.size() ); //������Ϣ
   Bitmap* branchType = new Bitmap; //��¼��ǰ�ڵ����ȵķ���
   printBinTree ( bt.root(), -1, ROOT, branchType ); //��״�ṹ
   release ( branchType ); printf ( "\n" );

}

/******************************************************************************************
 * �����������������ͳһ��ӡ
 ******************************************************************************************/
template <typename T>  //Ԫ������
static void printBinTree(BinNodePosi(T) bt, int depth, int type,
                         Bitmap* bType) {
  if (!bt) return;
  if (-1 < depth)  //���õ�ǰ��Ĺ����־
    R_CHILD == type ? bType->set(depth) : bType->clear(depth);
  printBinTree(bt->rc, depth + 1, R_CHILD, bType);  //�����������ϣ�
  print(bt);
  printf(" *");
  for (int i = -1; i < depth; i++)  //�������ڸ���
    if ((0 > i) ||
        bType->test(i) == bType->test(i + 1))  //�Ĺ����Ƿ�һ�£�����ȷ��
      printf("      ");                        //�Ƿ�Ӧ��
    else
      printf("��    ");  //��ӡ����
  switch (type) {
    case R_CHILD:
      printf("����");
      break;
    case L_CHILD:
      printf("����");
      break;
    default:
      printf("����");
      break;  // root
  }
  print(bt);
#if defined(DSA_HUFFMAN)
  if (IsLeaf(*bt)) bType->print(depth + 1);  //���Huffman����
#endif
  printf("\n");
  printBinTree(bt->lc, depth + 1, L_CHILD, bType);  //�����������£�
}

#endif
