#include "Huffman.h"

//ͳ���ı��ļ����ַ����ָ���
int* statistics(char* text_file) {
  int* freq = new int[N_CHAR];  //����ͳ����������ʣ����������¼���ַ����ִ���
  memset(freq, 0, sizeof(int) * N_CHAR);  //����
  FILE* fp = fopen(text_file, "r");  // assert: �ļ������ҿ���ȷ��
  for (char ch; 0 < fscanf(fp, "%c", &ch);)  //���ɨ�������ļ��е�ÿ���ַ�
    if (ch >= 0x20) freq[ch - 0x20]++;  //�ۼƶ�Ӧ�ĳ��ִ���
  fclose(fp);
  return freq;
}


//��ʼ��huffmanɭ��
HuffForest* initForest(int* freq) {
  //����Ƶ��ͳ�Ʊ�Ϊÿ���ַ�����һ����
  HuffForest* forest = new HuffForest;
  for (int i = 0; i < N_CHAR; i++) {  //Ϊÿ���ַ�
    forest->push_back(new HuffTree);  //����һ�����������ַ�����Ƶ��
    forest->back()->insertAsRoot(HuffChar(0x20 + i, freq[i]));  //��������
  }
  return forest;
}

//����huffmanɭ��,����Huffman��
//�㷨��,ÿ�κϲ�������С�������ڵ�(Ҳ��������),���յõ�Huffman��
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
  //ע��erase����������delete���ָ�������
  forest->erase(minchar);
  return minTree;
}