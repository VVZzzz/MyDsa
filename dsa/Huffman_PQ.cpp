#include "Huffman.h"

//统计文本文件中字符出现概率
int* statistics(char* text_file) {
  int* freq = new int[N_CHAR];  //以下统计需随机访问，故以数组记录各字符出现次数
  memset(freq, 0, sizeof(int) * N_CHAR);  //清零
  FILE* fp = fopen(text_file, "r");  // assert: 文件存在且可正确打开
  for (char ch; 0 < fscanf(fp, "%c", &ch);)  //逐个扫描样本文件中的每个字符
    if (ch >= 0x20) freq[ch - 0x20]++;  //累计对应的出现次数
  fclose(fp);
  return freq;
}


//初始化huffman森林
HuffForest* initForest(int* freq) {
  //根据频率统计表，为每个字符创建一棵树
  HuffForest* forest = new HuffForest;
  for (int i = 0; i < N_CHAR; i++) {  //为每个字符
    forest->push_back(new HuffTree);  //生成一棵树，并将字符及其频率
    forest->back()->insertAsRoot(HuffChar(0x20 + i, freq[i]));  //存入其中
  }
  return forest;
}

//根据huffman森林,创建Huffman树
//算法是,每次合并概率最小的两个节点(也就是树根),最终得到Huffman树
HuffTree* minHChar(HuffForest* forest) {
  auto itr = forest->begin();
  auto minchar = itr;
  int minP = (*itr)->root()->data.weight;
  while (itr!=forest->end()) {
    int temp = (*itr)->root()->data.weight;
    if (minP > temp) {
      minP = temp;
      minchar = itr;
    }
    itr++;
  }
  HuffTree* minTree = *minchar;
  //注意erase并不会真正delete这个指针的内容
  forest->erase(minchar);
  return minTree;
}