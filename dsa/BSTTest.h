#ifndef BSTTEST_H_
#define BSTTEST_H_
#pragma once
#include "BST.h"
#include "funcobj.h"
#include "print.h"
#include "rand.h"
template <typename T>
void testBST(int n) {
  BST<T> bst;
  while (bst.size() < n) bst.insert(dice((T)n * 3));
  print(bst);  //随机创建
  bst.stretchToLPath();
  print(bst);                                         //伸直成撇
  while (!bst.empty()) {
    BinNodePosi(T) tmp = bst.root();
    bst.remove(tmp->data);
  } 
	  //bst.remove(bst.root()->data);  //清空
  while (bst.size() < n) bst.insert(dice((T)n * 3));
  print(bst);  //随机创建
  bst.stretchToRPath();
  print(bst);                                         //伸直成捺
  while (!bst.empty()) bst.remove(bst.root()->data);  //清空
  while (bst.size() < n) {  //随机插入、查询、删除
    T e = dice((T)n * 3);   //[0, 3n)范围内的e
    switch (dice(3)) {
      case 0: {  //查找，成功率 <= 33.3%
        printf("Searching for ");
        print(e);
        printf(" ... ");
        BinNodePosi(T)& p = bst.search(e);
        p ? printf("Found with"), print(p->data),
            printf("\n") : printf("not found\n");
        break;
      }
      case 1: {  //删除，成功率 <= 33.3%
        printf("Removing ");
        print(e);
        printf(" ... ");
        if (bst.remove(e)) {
          printf("Done\n");
          print(bst);
        } else
          printf("not exists\n");
        break;
      }
      default: {  //插入，成功率 == 100%
        printf("Inserting ");
        print(e);
        printf(" ... ");
        printf("Done with"), print(bst.insert(e)->data), printf("\n"),
            print(bst);
        break;
      }
    }
  }
  while (bst.size() > 0) {  //清空
    T e = dice((T)n * 3);   //[0, 3n)范围内的e
    printf("Removing ");
    print(e);
    printf(" ... ");
    if (bst.remove(e)) {
      printf("Done\n");
      print(bst);
    } else
      printf("not exists\n");
  }
}

#endif
