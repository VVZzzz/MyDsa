#ifndef PRINT_H_
#define PRINT_H_
#pragma once
#include <stdio.h>
#include <typeinfo>
#include "BinNode.h"
#include "BinTree.h"
#include "BST.h"

template <typename T>
static void print(T* x) {
  if (x)
    print(*x);
  else
    printf("<NULL>");
}
template <typename T>
static void print(T& x) {
  UniPrint::p(x);
}
template <typename T>
static void print(const T& x) {
  UniPrint::p(x);
}
static void print(char* x) { printf("%s", x ? x : "<NULL>"); }
static void print(const char* x) { printf("%s", x ? x : "<NULL>"); }

//��������
class UniPrint {
 public:
  static void p(int);
  static void p(float);
  static void p(double);
  static void p(char);
  // static void p ( HuffChar& ); //Huffman�������ַ�
  // static void p ( VStatus ); //ͼ�����״̬
  // static void p ( EType ); //ͼ�ߵ�����

  // template <typename K, typename V> static void p ( Entry<K, V>& ); //Entry
  template <typename T>
  static void p(BinNode<T>&);  // BinTree�ڵ�
  template <typename T>
  static void p(BinTree<T>&);  //������
  template <typename T> static void p ( BST<T>& ); //BST
  /*
  template <typename T> static void p ( BTree<T>& ); //B-��
  template <typename T> static void p ( AVL<T>& ); //AVL
  template <typename T> static void p ( RedBlack<T>& ); //RedBlack
  template <typename T> static void p ( Splay<T>& ); //Splay
  template <typename T> static void p ( Quadlist<T>& ); //Quadlist
  template <typename K, typename V> static void p ( Skiplist<K, V>& );
  //Skiplist template <typename K, typename V> static void p ( Hashtable<K, V>&
  ); //Hashtable template <typename T> static void p ( PQ_List<T>& ); //PQ_List
  template <typename T> static void p ( PQ_ComplHeap<T>& ); //PQ_ComplHeap
  template <typename T> static void p ( PQ_LeftHeap<T>& ); //PQ_LeftHeap
  template <typename Tv, typename Te> static void p ( GraphMatrix<Tv, Te>& );
  //Graph
  */
  template <typename T>
  static void p(T&);  //�������б��֧��traverse()�������������Խṹ
  template <typename T>
  static void p(T* s)  //����ָ��
  {
    s ? p(*s) : print("<NULL>");
  }  //ͳһתΪ����
};

#include "print_implementation.h"
#endif
