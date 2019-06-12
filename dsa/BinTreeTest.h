#ifndef BINTREE_TEST_H_
#define BINTREE_TEST_H_
//测试BinTree文件
#pragma once
#include <stdio.h>
#include "BinNode.h"
#include "funcobj.h"
#include "rand.h"
#include "print.h"
#include "release.h"
#include "BinTree.h"

extern int testID;

template <typename T>
bool randomBinTree(BinTree<T>& bt, BinNodePosi(T) x, int h) {
  if (0 >= h) return false;
  // 1/h概率使该分支终止生长
  if (0 < dice(h))
    randomBinTree(bt, bt.insertAsLC(x, dice((T)h * h * h)), h - 1);
  // 1/h概率使该分支终止生长
  if (0 < dice(h))
    randomBinTree(bt, bt.insertAsRC(x, dice((T)h * h * h)), h - 1);
  return true;
}

//在二叉树中随机确定一个节点位置
template <typename T>
BinNodePosi(T) randomPosiInBinTree(BinNodePosi(T) root) {
  if (!HasChild(*root)) return root;
  if (!HasLChild(*root)) return dice(6) ? randomPosiInBinTree(root->rc) : root;
  if (!HasRChild(*root)) return dice(6) ? randomPosiInBinTree(root->lc) : root;
  return dice(2) ? randomPosiInBinTree(root->lc)
                 : randomPosiInBinTree(root->rc);
}

template <typename T>
void testBinTree(int h) {
  printf("\n  ==== Test %2d. Generate a binTree of height <= %d \n", testID++,
         h);
  BinTree<T> bt;
  print(bt);
  bt.insertAsRoot(dice((T)h * h * h));
  print(bt);
  randomBinTree<T>(bt, bt.root(), h);
  print(bt);
  printf("\n  ==== Test %2d. Double and increase all nodes by traversal\n",
         testID++);
  auto doubleFunc = Double<T>();
  auto increaseFunc = Increase<T>();

  bt.travPre(doubleFunc);
  bt.travPre(increaseFunc);
  print(bt);
  bt.travIn(doubleFunc);
  bt.travIn(increaseFunc);
  print(bt);
  bt.travPost(doubleFunc);
  bt.travPost(increaseFunc);
  print(bt);
  bt.travLevel(doubleFunc);
  bt.travLevel(increaseFunc);
  print(bt);
  Hailstone<T> he;
  bt.travIn(he);
  print(bt);
  printf("\n  ==== Test %2d. Remove/release subtrees in the Tree\n", testID++);
  while (!bt.empty()) {
    BinNodePosi(T) p = randomPosiInBinTree(bt.root());  //随机选择一个节点
    if (dice(2)) {
      printf("removing ");
      print(p->data);
      printf(" ...\n");
      printf("%d node(s) removed\n", bt.remove(p));
      print(bt);
    } else {
      printf("releasing ");
      print(p->data);
      printf(" ...\n");
      BinTree<T>* S = bt.secede(p);
      print(S);
      printf("%d node(s) released\n", S->size());
      release(S);
      print(bt);
    }
  }
}

template <typename T>
void test() {
  printf("This is test.\n");
}

#endif