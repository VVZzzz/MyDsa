#include <stdlib.h>
#include "BSTTest.h"
int testID = 0;

int main(int argc, char** argv) {
  srand((unsigned)time(NULL));
  //testBinTree<int>(5);
  testBST<int>(5);
  
  system("pause");
  return 0;
}