#ifndef BITMAP_H_
#define BITMAP_H_
#include <stdio.h>
#include <stdlib.h>
#include <memory>

//��һάbitmap
/*
//Bitmap��: ����һ������ 3 6 2 5 4 1 7 0
//F : 7 5 2 0 4 3 1 6
//    0 1 2 3 4 5 6 7 (index)
//T : 3 6 2 5 4 1 7 0
//T��top����Ϊ8

typedef int Rank;
class Bitmap {  //λͼ��,�����ռ��ʡ��ʼ��ʱ��,�������֧��ɾ��
 private:
  Rank* F;
  Rank N;  //��ģΪN������F����¼[k]����ǵĴ��򣨼�����ջT[]�е��ȣ�
  Rank* T;
  Rank top;  //����ΪN��ջT����¼����Ǹ�λ�ȵ�ջ���Լ�ջ��ָ��
 protected:
  inline bool valid(Rank r) { return (0 <= r) && (r < top); }
  inline bool erased(Rank r)  //�ж�[k]�Ƿ�������ǹ�,�Һ����ֱ����.
  {
    return valid(r) && !(T[F[r]] + 1 + r);
  }

 public:
  Bitmap(Rank n =
             8)  //��ָ������Ĭ�ϣ���ģ��������ͼ��Ϊ������ʱѡ�ý�С��Ĭ��ֵ��
  {
    N = n;
    F = new Rank[N];
    T = new Rank[N];
    top = 0;
  }  //��O(1)ʱ������ʽ�س�ʼ��
  ~Bitmap() {
    delete[] F;
    delete[] T;
  }  //����ʱ�ͷſռ�

  // �ӿ�
  inline void set(Rank k) {        //����
    if (test(k)) return;           //�����Ѵ���ǵ�λ
    if (!erased(k)) F[k] = top++;  //��ϵ���α�ǣ��򴴽���У�黷
    T[F[k]] = k;  //��ϵ������Ǻ��������ָ�ԭУ�黷
  }
  inline void clear(Rank k)  //ɾ��
  {
    if (test(k)) T[F[k]] = -1 - k;
  }                         //���Բ�����ǵ�λ
  inline bool test(Rank k)  //����
  {
    return valid(F[k]) && (k == T[F[k]]);
  }
};
*/

class Bitmap {  //λͼBitmap��
 private:
  //����ͼ��ſռ�M,����ΪN*(sizeof(char))*8bit
  char *M;
  int N;

 protected:
  void init(int n) {
    //��1~8 -> 1��char,9~16 ->2��char
    M = new char[N = (n + 1) / 8];
    memset(M, 0, N);
  }

 public:
  Bitmap(int n = 8) { init(n); }
  //��ָ���ļ��ж�ȡbitmap
  Bitmap(char *file, int n = 8) {
    init(n);
    FILE *fp;
    fopen_s(&fp,file, "r");
    fread(M, sizeof(char), N, fp);
    fclose(fp);
  }
  ~Bitmap() { delete[] M; }

  //����k����bitmap��(��λ)
  void set(int k) {
    expand(k);
    M[k >> 3] |= (0x80 >> (k & 0x07));
  }

  //��Ӧ��λ����
  void clear(int k) {
    expand(k);
    M[k >> 3] &= ~(0x80 >> (k & 0x07));
  }

  //�鿴�Ƿ���λ
  bool test(int k) {
    expand(k);
    return (M[k >> 3] & (0x80 >> (k & 0x07)));
  }

  //��λͼ���嵼����ָ�����ļ����Ա�Դ˺����λͼ������ʼ��
  void dump(char *file) {
    FILE *fp;
    fopen_s(&fp,file, "w");
    fread(M, sizeof(char), N, fp);
    fclose(fp);
  }

  //��ǰnλתΪstring
  char *bits2string(int n) {
    expand(n - 1);
    char *s = new char[n + 1];
    s[n] = '\0';
    for (int i = 0; i < n; i++) s[i] = (test(i) ? '1' : '0');
    return s;
  }

  void expand(int k) {  // Bitmap[k]�ڷ�Χ��,��������
    if (k < 8 * N) return;
    //����,��Ҫ����
    int oldN = N;
    char *oldM = M;
    init(2 * k);
    memcpy_s(M, N, oldM, oldN);
    delete[] oldM;
  }

  //��ӡ(�Ǳ���ӿ�)
  void print(int n) {
    expand(n);
    for (int i = 0; i < n; i++) printf(((test(i)) ? "1" : "0"));
  }
};

#endif  // !BITMAP_H_
