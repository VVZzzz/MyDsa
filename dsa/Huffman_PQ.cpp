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

//Huffman编码算法,将最小的两个树合并成一个新树
HuffTree* generateTree(HuffForest* forest) {
  while (forest->size()>1) {
    //每次都找到两个最小的树
    HuffTree* T1 = minHChar(forest);
    HuffTree* T2 = minHChar(forest);
    HuffTree* newT = new HuffTree();
    //合并成一个新树
    newT->insertAsRoot(
        HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));
    newT->attachAsLC(newT->root(), T1);
    newT->attachAsRC(newT->root(), T2);
    forest->push_back(newT);
  }  //循环结束时,forest中只有一颗树,即Huffman编码树
  return forest->back();
}

//生成Huffman编码表
//遍历获取字符编码table
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
  //如果是叶子节点,说明已经到了字符位置,转为编码即可
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

//将各字符编码统一存入以散列表实现的编码表中
HuffTable* generateTable(HuffTree* tree) { 
  HuffTable* table = new HuffTable; 
  Bitmap* code = new Bitmap;
  generateCT(code, 0, table, tree->root());
  release(code);
  return table;
}

//按照编码表HuffTable对Bitmap串编码
//即将字符串s的每个字符都进行编码(根据编码表HuffTable)放在bitmap中
//eg:上面的编码表,输入:MACBD  输出: 10|01|11|001|000
int encode(HuffTable* table, Bitmap* codeString, char* s) {
  int n = 0;
  for (int i = 0, m = strlen(s); i < m; i++) {
    char** pCharCode = table->get(s[i]);
    //如果找不到对应的编码值,就将这个字符由小写到大写
    if (!pCharCode) pCharCode = table->get(s[i] + 'A' - 'a');
    //如果还没有说明编码表里没这个字符,一律按照" "空格处理
    if (!pCharCode) pCharCode = table->get(' ');
    printf("%s", *pCharCode);  //输出当前字符编码值
    for (int l = strlen(*pCharCode),j = 0; j < l; j++) {
      '1' == *(*pCharCode + j) ? codeString->set(n++) : codeString->clear(n++);
    }
  }
  printf("\n");
  return n;
}//最终的编码串(暗文串)放在codeString(bitmap)中

//根据编码树对长为n的Bitmap串(已编码的huffman串)进行解码
//暗文->明文,这里解码用huffman树,编码用huffman表
//解除的每个字符直接打印出来,如需要导出可自行修改
void decode(HuffTree* tree, Bitmap* code, int n) { 
  BinNodePosi(HuffChar) x = tree->root();
  for (size_t i = 0; i < n; i++) {
    //即011,指从根->左子树->右子树->右子树(叶子节点,即得到字符).
    x = code->test(i) ? x->rc : x->lc;
    if (IsLeaf(*x)) {
      printf("%c", x->data.ch);
      x = tree->root();
    }
  }
}
