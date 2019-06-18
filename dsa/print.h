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

//函数对象
class UniPrint {
 public:
  static void p(int);
  static void p(float);
  static void p(double);
  static void p(char);
  // static void p ( HuffChar& ); //Huffman（超）字符
  // static void p ( VStatus ); //图顶点的状态
  // static void p ( EType ); //图边的类型

  // template <typename K, typename V> static void p ( Entry<K, V>& ); //Entry
  template <typename T>
  static void p(BinNode<T>&);  // BinTree节点
  template <typename T>
  static void p(BinTree<T>&);  //二叉树
  template <typename T> static void p ( BST<T>& ); //BST
  /*
  template <typename T> static void p ( BTree<T>& ); //B-树
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
  static void p(T&);  //向量、列表等支持traverse()遍历操作的线性结构
  template <typename T>
  static void p(T* s)  //所有指针
  {
    s ? p(*s) : print("<NULL>");
  }  //统一转为引用
};

#include "print_implementation.h"
#endif
