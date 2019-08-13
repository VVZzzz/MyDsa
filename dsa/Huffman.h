#pragma once
#include <list>
#include "Dictionary.h"
#include "Bitmap.h"
#include "HashTable.h"
#include "BinTree.h"

//Huffman�ַ�
#define  N_CHAR  (0x80 - 0x20) //���Կɴ�ӡ�ַ�Ϊ��
struct HuffChar {   //Huffman�ַ�
  char ch;          //�ַ�
  int weight;       //Ƶ��
  HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {}
  //�Ƚ���
   bool operator< ( HuffChar const& hc ) { return weight > hc.weight; } //�˴������С�ߵ�
   bool operator== ( HuffChar const& hc ) { return weight == hc.weight; }
   bool operator> ( HuffChar const& hc ) { return weight < hc.weight; } //�˴������С�ߵ�
   bool operator>= ( HuffChar const& hc ) { return weight <= hc.weight; } //�˴������С�ߵ�
   bool operator<= ( HuffChar const& hc ) { return weight >= hc.weight; } //�˴������С�ߵ�
};

//Huffman��(�ö�����ʵ��)
#define HuffTree BinTree<HuffChar> //Huffman������BinTree�������ڵ�����ΪHuffChar

//Huffman�����(��hash��ʵ��)
typedef HashTable<char, char*> HuffTable;

//Huffman�����Ʊ���(��Bitmapʵ��)
typedef Bitmap HuffCode;

//Huffmanɭ��(ֱ����Listʵ��)
typedef std::list<HuffTree *> HuffForest;


//Huffman�����ȼ�"Huffman_PQ.cpp"

//���ڴ���Huffman����Ҫÿ�ζ�ѡȡ��Сֵ,���Կ��������ȶ���(����ʽ�ѵ�ʵ��)

