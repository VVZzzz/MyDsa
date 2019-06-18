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
  print(bst);  //�������
  bst.stretchToLPath();
  print(bst);                                         //��ֱ��Ʋ
  while (!bst.empty()) {
    BinNodePosi(T) tmp = bst.root();
    bst.remove(tmp->data);
  } 
	  //bst.remove(bst.root()->data);  //���
  while (bst.size() < n) bst.insert(dice((T)n * 3));
  print(bst);  //�������
  bst.stretchToRPath();
  print(bst);                                         //��ֱ����
  while (!bst.empty()) bst.remove(bst.root()->data);  //���
  while (bst.size() < n) {  //������롢��ѯ��ɾ��
    T e = dice((T)n * 3);   //[0, 3n)��Χ�ڵ�e
    switch (dice(3)) {
      case 0: {  //���ң��ɹ��� <= 33.3%
        printf("Searching for ");
        print(e);
        printf(" ... ");
        BinNodePosi(T)& p = bst.search(e);
        p ? printf("Found with"), print(p->data),
            printf("\n") : printf("not found\n");
        break;
      }
      case 1: {  //ɾ�����ɹ��� <= 33.3%
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
      default: {  //���룬�ɹ��� == 100%
        printf("Inserting ");
        print(e);
        printf(" ... ");
        printf("Done with"), print(bst.insert(e)->data), printf("\n"),
            print(bst);
        break;
      }
    }
  }
  while (bst.size() > 0) {  //���
    T e = dice((T)n * 3);   //[0, 3n)��Χ�ڵ�e
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
