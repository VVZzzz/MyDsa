#ifndef BITMAP_H_
#define BITMAP_H_
#include <memory>
#include <stdio.h>
#include <stdlib.h>

//简单一维bitmap
/*
//Bitmap即: 比如一批数据 3 6 2 5 4 1 7 0
//F : 7 5 2 0 4 3 1 6
//    0 1 2 3 4 5 6 7 (index)
//T : 3 6 2 5 4 1 7 0
//T的top现在为8

typedef int Rank;
class Bitmap {  //位图类,牺牲空间节省初始化时间,允许插入支持删除
 private:
  Rank* F;
  Rank N;  //规模为N的向量F，记录[k]被标记的次序（即其在栈T[]中的秩）
  Rank* T;
  Rank top;  //容量为N的栈T，记录被标记各位秩的栈，以及栈顶指针
 protected:
  inline bool valid(Rank r) { return (0 <= r) && (r < top); }
  inline bool erased(Rank r)  //判断[k]是否曾被标记过,且后来又被清除.
  {
    return valid(r) && !(T[F[r]] + 1 + r);
  }

 public:
  Bitmap(Rank n =
             8)  //按指定（或默认）规模创建比特图（为测试暂时选用较小的默认值）
  {
    N = n;
    F = new Rank[N];
    T = new Rank[N];
    top = 0;
  }  //在O(1)时间内隐式地初始化
  ~Bitmap() {
    delete[] F;
    delete[] T;
  }  //析构时释放空间

  // 接口
  inline void set(Rank k) {        //插入
    if (test(k)) return;           //忽略已带标记的位
    if (!erased(k)) F[k] = top++;  //若系初次标记，则创建新校验环
    T[F[k]] = k;  //若系曾经标记后被清除，则恢复原校验环
  }
  inline void clear(Rank k)  //删除
  {
    if (test(k)) T[F[k]] = -1 - k;
  }                         //忽略不带标记的位
  inline bool test(Rank k)  //测试
  {
    return valid(F[k]) && (k == T[F[k]]);
  }
};
*/

class Bitmap {  //位图Bitmap类
 private:
  //比特图存放空间M,容量为N*(sizeof(char))*8bit
  char *M;
  int N;

 protected:
  void init(int n) {
    //即1~8 -> 1个char,9~16 ->2个char
    M = new char[N = (n + 1) / 8];
    memset(M, 0, N);
  }

 public:
  Bitmap(int n = 8) { init(n); }
  //从指定文件中读取bitmap
  Bitmap(char *file,)
};

#endif  // !BITMAP_H_
