#include "BitmapTest.h"

/*
int testBitmap(int n) {
  bool* B = new bool[n];
  memset(B, 0, n * sizeof(bool));  //常规位图，创建后随即O(n)时间初始化
  Bitmap M(n);
  for (int i = 0; i < 9 * n; i++) {
    Rank k = dice(n);
    if (dice(2)) {
      printf("set(%d) ...", k);  // set(k)
      B[k] = true;
      M.set(k);
    } else {
      printf("clear(%d) ...", k);  // clear(k)
      B[k] = false;
      M.clear(k);
    }
    printf("done\n CRC: ");
    for (int j = 0; j < n; j++) printf("%6c", B[j] == M.test(j) ? ' ' : '!');
    printf("\n B[]: ");
    for (int j = 0; j < n; j++) printf("%6c", B[j] ? 'x' : '.');
    printf("\n M[]: ");
    for (int j = 0; j < n; j++) printf("%6c", M.test(j) ? 'x' : '.');
    printf("\n\n\n");
  }
  delete[] B;
  return 0;
}
*/