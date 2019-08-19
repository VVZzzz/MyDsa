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

//Huffman�����㷨,����С���������ϲ���һ������
HuffTree* generateTree(HuffForest* forest) {
  while (forest->size()>1) {
    //ÿ�ζ��ҵ�������С����
    HuffTree* T1 = minHChar(forest);
    HuffTree* T2 = minHChar(forest);
    HuffTree* newT = new HuffTree();
    //�ϲ���һ������
    newT->insertAsRoot(
        HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));
    newT->attachAsLC(newT->root(), T1);
    newT->attachAsRC(newT->root(), T2);
    forest->push_back(newT);
  }  //ѭ������ʱ,forest��ֻ��һ����,��Huffman������
  return forest->back();
}

//����Huffman�����
//������ȡ�ַ�����table
/*              11
           /(0)       \(1)
          4             7
       /(0) \(1)     /(0) \(1)
       2    A       M     C
   (0)/  \(1)
     D    B
 */
static void generateCT(Bitmap* code, int length, HuffTable* table,
                       BinNodePosi(HuffChar) v) {
  //�����Ҷ�ӽڵ�,˵���Ѿ������ַ�λ��,תΪ���뼴��
  if (IsLeaf(*v)) {
    table->put(v->data.ch, code->bits2string(length));
    return;
  }
  //Left = 0
  if (HasLChild(*v)) {
    code->clear(length);
    generateCT(code, length + 1, table, v->lc);
  }

  //Right = 1
  if (HasRChild(*v)) {
    code->set(length);
    generateCT(code, length + 1, table, v->rc);
  }

}

//�����ַ�����ͳһ������ɢ�б�ʵ�ֵı������
HuffTable* generateTable(HuffTree* tree) { 
  HuffTable* table = new HuffTable; 
  Bitmap* code = new Bitmap;
  generateCT(code, 0, table, tree->root());
  release(code);
  return table;
}

//���ձ����HuffTable��Bitmap������
//�����ַ���s��ÿ���ַ������б���(���ݱ����HuffTable)����bitmap��
//eg:����ı����,����:MACBD  ���: 10|01|11|001|000
int encode(HuffTable* table, Bitmap* codeString, char* s) {
  int n = 0;
  for (int i = 0, m = strlen(s); i < m; i++) {
    char** pCharCode = table->get(s[i]);
    //����Ҳ�����Ӧ�ı���ֵ,�ͽ�����ַ���Сд����д
    if (!pCharCode) pCharCode = table->get(s[i] + 'A' - 'a');
    //�����û��˵���������û����ַ�,һ�ɰ���" "�ո���
    if (!pCharCode) pCharCode = table->get(' ');
    printf("%s", *pCharCode);  //�����ǰ�ַ�����ֵ
    for (int l = strlen(*pCharCode),j = 0; j < l; j++) {
      '1' == *(*pCharCode + j) ? codeString->set(n++) : codeString->clear(n++);
    }
  }
  printf("\n");
  return n;
}//���յı��봮(���Ĵ�)����codeString(bitmap)��

//���ݱ������Գ�Ϊn��Bitmap��(�ѱ����huffman��)���н���
//����->����,���������huffman��,������huffman��
//�����ÿ���ַ�ֱ�Ӵ�ӡ����,����Ҫ�����������޸�
void decode(HuffTree* tree, Bitmap* code, int n) { 
  BinNodePosi(HuffChar) x = tree->root();
  for (size_t i = 0; i < n; i++) {
    //��011,ָ�Ӹ�->������->������->������(Ҷ�ӽڵ�,���õ��ַ�).
    x = code->test(i) ? x->rc : x->lc;
    if (IsLeaf(*x)) {
      printf("%c", x->data.ch);
      x = tree->root();
    }
  }
}
