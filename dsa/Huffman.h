#pragma once
#include <list>
#include "Dictionary.h"
#include "Bitmap.h"
#include "HashTable.h"
#include "BinTree.h"

//Huffman字符
#define  N_CHAR  (0x80 - 0x20) //仅以可打印字符为例
struct HuffChar {   //Huffman字符
  char ch;          //字符
  int weight;       //频率
  HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {}
  //比较器
   bool operator< ( HuffChar const& hc ) { return weight > hc.weight; } //此处故意大小颠倒
   bool operator== ( HuffChar const& hc ) { return weight == hc.weight; }
   bool operator> ( HuffChar const& hc ) { return weight < hc.weight; } //此处故意大小颠倒
   bool operator>= ( HuffChar const& hc ) { return weight <= hc.weight; } //此处故意大小颠倒
   bool operator<= ( HuffChar const& hc ) { return weight >= hc.weight; } //此处故意大小颠倒
};

//Huffman树(用二叉树实现)
#define HuffTree BinTree<HuffChar> //Huffman树，由BinTree派生，节点类型为HuffChar

//Huffman编码表(用hash表实现)
typedef HashTable<char, char*> HuffTable;

//Huffman二进制编码(用Bitmap实现)
typedef Bitmap HuffCode;

//Huffman森林(直接用List实现)
typedef std::list<HuffTree *> HuffForest;


//Huffman操作等见"Huffman_PQ.cpp"

//由于创建Huffman树需要每次都选取最小值,所以可以用优先队列(用左式堆等实现)

