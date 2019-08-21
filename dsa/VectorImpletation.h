#pragma once
#include "Vector.h"
#include "PQ_ComplHeap.h"
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
  _elem = new T[_capacity = 2 * (hi - lo)];
  _size = 0;                   //分配空间，规模清零
  while (lo < hi)              // A[lo, hi)内的元素逐一
    _elem[_size++] = A[lo++];  //复制至_elem[0, hi - lo)
}

template <typename T>
inline void Vector<T>::expand() {
  if (_size < _capacity) return;  //尚未满员时，不必扩容
  if (_capacity < DEFAULT_CAPACITY)
    _capacity = DEFAULT_CAPACITY;  //不低于最小容量
  T* oldElem = _elem;
  _elem = new T[_capacity <<= 1];  //容量加倍
  for (int i = 0; i < _size; i++)
    _elem[i] =
        oldElem[i];  //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
  delete[] oldElem;  //释放原空间
}

template <typename T>
inline void Vector<T>::shrink() {
  if (_capacity < DEFAULT_CAPACITY << 1)
    return;                            //不致收缩到DEFAULT_CAPACITY以下
  if (_size << 2 > _capacity) return;  //以25%为界
  T* oldElem = _elem;
  _elem = new T[_capacity >>= 1];                         //容量减半
  for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];  //复制原向量内容
  delete[] oldElem;                                       //释放原空间
}

/**
 * 一趟扫描交换,即 5 3 4 2
 * -> 3 5 4 2
 * -> 3 4 5 2
 * -> 3 4 2 5
 * 即把最大的那个元素冒到最后一个位置上
 */
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {  //一趟扫描交换
  bool sorted = true;                       //整体有序标志
  while (++lo < hi)  //自左向右，逐一检查各对相邻元素
    if (_elem[lo - 1] > _elem[lo]) {   //若逆序，则
      sorted = false;                  //意味着尚未整体有序，并需要
      swap(_elem[lo - 1], _elem[lo]);  //通过交换使局部有序
    }
  return sorted;  //返回有序标志
}

/**
 * [][][][][] ->一趟扫描交换,将最大的放置最后位置
 * [][][][][] -> [][][][][√]
 * 只对前四个冒泡 [][][][]  [√] -> [][][][√] [√]
 * 逐趟扫描进行冒泡
 * ...
 * [√][√][√][√][√]
 */
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
  while (!bubble(lo, hi--))
    ;  //注意每次都是hi--,然后再扫描一趟,有可改进的地方
}

/*
 * 优化的扫描交换,eg: 6 4 3 2 7 8 9
 * ->6 4 3 2 7 8 9
 * ->4 6 3 2 7 8 9
 * ->4 3 6 2 7 8 9
 * ->4 3 2 6 7 8 9
 * 此时这一趟的扫描交换结束,然后返回最后一次的逆序对的index
 * 也就是说index之后的元素全部都是有序的,所以下次只需要排[lo,index)即可
 */
template <typename T>
Rank Vector<T>::bubbleFast(Rank lo, Rank hi) {
  Rank last = lo; //最右侧的逆序对初始化为[lo - 1, lo]
  while (++lo<hi) { //自左向右，逐一检查各对相邻元素
    if (_elem[lo - 1] > _elem[lo]) { //若逆序，则
      last = lo; //更新最右侧逆序对位置记录，并
      swap(_elem[lo - 1], _elem[lo]); //通过交换使局部有序
    }
  }
  return last; //返回最右侧的逆序对位置
}

/*
 * 优化的冒泡排序,平凡的冒泡排序是逐趟扫描交换一次,即每次是[lo,hi--]
 * 这样的话,即使后面的某一部分已经有序了还是会检查
 * eg: 6 4 3 2 7 8 9 , 7 8 9已经有序,但由于是hi--还是要检查一遍.
 * 这样就可以改进,一趟下来记录最后出现逆序对的地方
 * -> 4 3 2 [6] 7 8 9,这样下一趟就只拍4 3 2即可.
 */
template <typename T>
void Vector<T>::bubbleSortFast(Rank lo, Rank hi) {
  while (lo < (hi = bubbleFast(lo, hi)))
    ;
}

template <typename T>
inline Rank Vector<T>::max(Rank lo, Rank hi) { //在[lo, hi]内找出最大者
   Rank mx = hi;
   while ( lo < hi-- ) //逆向扫描
      if ( _elem[hi] > _elem[mx] ) //且严格比较
         mx = hi; //故能在max有多个时保证后者优先，进而保证selectionSort稳定
   return mx;
}

/*
 * 每次都找到排序的部分的最大值,放到未排序部分的最后
 */
template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {
     while ( lo < --hi )
      swap ( _elem[max ( lo, hi ) ], _elem[hi] ); //将[hi]与[lo, hi]中的最大者交换
}

//堆排序,就地算法空间O(1),时间为O(nlogn)
//步骤是先floyd建堆,消耗时间O(n),之后依次delMax(),将最大值放在数组的末尾.故为O(nlogn)
/*
 * 4 2 5 1 3
 *        4
 *      2   5
 *    1  3
 * 建堆后: 
 *        5
 *      3   4
 *    1  2
 * 
 * [5 3 4 1 2] -> delMax
 *        4
 *      3   2
 *    1  
 *  
 * [4 3 2 1] 5  -> 其中5已处在正确位置上...依次类推
 * 1 2 3 4 5
 * 这也是为何从小到大排序,需要大根堆的原因
 */
//[lo,hi)
template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {
  PQ_ComplHeap<T> H(_elem + lo, hi - lo);
  while (H.empty()) {  //反复地摘除最大元并归入已排序的后缀，直至堆空
    _elem[--hi] = H.delMax();
  }
  
}

template<typename T>
int Vector<T>::disordered() const { //返回向量中逆序相邻元素对的总数
   int n = 0; //计数器
   for ( int i = 1; i < _size; i++ ) //逐一检查_size - 1对相邻元素
      if ( _elem[i - 1] > _elem[i] ) n++; //逆序则计数
   return n; //向量有序当且仅当n = 0
}

template <typename T> //无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
Rank Vector<T>::find ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size
   while ( ( lo < hi-- ) && ( e != _elem[hi] ) ); //从后向前，顺序查找
   return hi; //若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
  return binSearch(_elem, lo, hi);
}

template <typename T>
Rank Vector<T>::binSearch(T* A, T const& e, Rank lo, Rank hi) {
  /*
  //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置
  while (lo<hi) {
    Rank mi = (lo + hi) >> 1;
    if (e < A[mi]) hi = mi;
    else if (A[mi] < e)
      lo = mi;
    else
      return mi;
  }
  return -1;  //查找失败
  */
  while (lo<hi) {
    Rank mi = (lo + hi) >> 1;
    (e < A[mi]) hi = mi : lo = mi + 1;
  }  //查找成功时不能停止,直到lo==hi
  return --lo;  //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
  //有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const&) {
   if ( _elem ) delete [] _elem; //释放原有内容
   copyFrom ( V._elem, 0, V.size() ); //整体复制
   return *this; //返回当前对象的引用，以便链式赋值
}

template <typename T>
T Vector<T>::remove(Rank r) {
  T e = _elem[r]; //备份被删除元素
  remove ( r, r + 1 ); //调用区间删除算法，等效于对区间[r, r + 1)的删除
  return e; //返回被删除元素
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
  if ( lo == hi ) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
  while ( hi < _size ) _elem[lo++] = _elem[hi++]; //[hi, _size)顺次前移hi - lo个单元
  _size = lo; //更新规模，直接丢弃尾部[lo, _size = hi)区间
  shrink(); //若有必要，则缩容
  return hi - lo; //返回被删除元素的数目
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
  expand(); //若有必要，扩容
  for ( int i = _size; i > r; i-- ) _elem[i] = _elem[i-1]; //自后向前，后继元素顺次后移一个单元
  _elem[r] = e; _size++; //置入新元素并更新容量
  return r; //返回秩
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
  bubbleSortFast(lo, hi);
  //
}

template <typename T>
inline void Vector<T>::unsort(Rank lo, Rank hi) {}

//删除重复元素
template <typename T>
int Vector<T>::deduplicate() {
  int oldsize = _size;
  Rank i = 1;//从elem[i]开始
  while (i<_size) { //自前向后逐一考查各元素_elem[i]
    (find(_elem[i], 0, i) < 0) ? ++i : remove(i); //在其前缀中寻找与之雷同者（至多一个）
    //若无雷同则继续考查其后继，否则删除雷同者
   return oldsize - _size; //向量规模变化量，即被删除元素总数
  }
}

template <typename T>
int Vector<T>::uniquify() {
  Rank i = 0, j = 0; //各对互异“相邻”元素的秩
  while (++j<_size) { //逐一扫描，直至末元素
    if (_elem[i] != _elem[j]) {//跳过雷同者
      _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
    }
  }
  _size = ++i;
  shrink(); //直接截除尾部多余元素
  return j - i; //向量规模变化量，即被删除元素总数
}


template <typename T> void Vector<T>::traverse ( void ( *visit ) ( T& ) ) //借助函数指针机制
{ for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); } //遍历向量

template <typename T> template <typename VST> //元素类型、操作器
void Vector<T>::traverse ( VST& visit ) //借助函数对象机制
{ for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); } //遍历向量

    
