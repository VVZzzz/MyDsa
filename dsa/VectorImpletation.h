#pragma once
#include "Vector.h"
#include "PQ_ComplHeap.h"
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
  _elem = new T[_capacity = 2 * (hi - lo)];
  _size = 0;                   //����ռ䣬��ģ����
  while (lo < hi)              // A[lo, hi)�ڵ�Ԫ����һ
    _elem[_size++] = A[lo++];  //������_elem[0, hi - lo)
}

template <typename T>
inline void Vector<T>::expand() {
  if (_size < _capacity) return;  //��δ��Աʱ����������
  if (_capacity < DEFAULT_CAPACITY)
    _capacity = DEFAULT_CAPACITY;  //��������С����
  T* oldElem = _elem;
  _elem = new T[_capacity <<= 1];  //�����ӱ�
  for (int i = 0; i < _size; i++)
    _elem[i] =
        oldElem[i];  //����ԭ�������ݣ�TΪ�������ͣ��������ظ�ֵ������'='��
  delete[] oldElem;  //�ͷ�ԭ�ռ�
}

template <typename T>
inline void Vector<T>::shrink() {
  if (_capacity < DEFAULT_CAPACITY << 1)
    return;                            //����������DEFAULT_CAPACITY����
  if (_size << 2 > _capacity) return;  //��25%Ϊ��
  T* oldElem = _elem;
  _elem = new T[_capacity >>= 1];                         //��������
  for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];  //����ԭ��������
  delete[] oldElem;                                       //�ͷ�ԭ�ռ�
}

/**
 * һ��ɨ�轻��,�� 5 3 4 2
 * -> 3 5 4 2
 * -> 3 4 5 2
 * -> 3 4 2 5
 * ���������Ǹ�Ԫ��ð�����һ��λ����
 */
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {  //һ��ɨ�轻��
  bool sorted = true;                       //���������־
  while (++lo < hi)  //�������ң���һ����������Ԫ��
    if (_elem[lo - 1] > _elem[lo]) {   //��������
      sorted = false;                  //��ζ����δ�������򣬲���Ҫ
      swap(_elem[lo - 1], _elem[lo]);  //ͨ������ʹ�ֲ�����
    }
  return sorted;  //���������־
}

/**
 * [][][][][] ->һ��ɨ�轻��,�����ķ������λ��
 * [][][][][] -> [][][][][��]
 * ֻ��ǰ�ĸ�ð�� [][][][]  [��] -> [][][][��] [��]
 * ����ɨ�����ð��
 * ...
 * [��][��][��][��][��]
 */
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
  while (!bubble(lo, hi--))
    ;  //ע��ÿ�ζ���hi--,Ȼ����ɨ��һ��,�пɸĽ��ĵط�
}

/*
 * �Ż���ɨ�轻��,eg: 6 4 3 2 7 8 9
 * ->6 4 3 2 7 8 9
 * ->4 6 3 2 7 8 9
 * ->4 3 6 2 7 8 9
 * ->4 3 2 6 7 8 9
 * ��ʱ��һ�˵�ɨ�轻������,Ȼ�󷵻����һ�ε�����Ե�index
 * Ҳ����˵index֮���Ԫ��ȫ�����������,�����´�ֻ��Ҫ��[lo,index)����
 */
template <typename T>
Rank Vector<T>::bubbleFast(Rank lo, Rank hi) {
  Rank last = lo; //���Ҳ������Գ�ʼ��Ϊ[lo - 1, lo]
  while (++lo<hi) { //�������ң���һ����������Ԫ��
    if (_elem[lo - 1] > _elem[lo]) { //��������
      last = lo; //�������Ҳ������λ�ü�¼����
      swap(_elem[lo - 1], _elem[lo]); //ͨ������ʹ�ֲ�����
    }
  }
  return last; //�������Ҳ�������λ��
}

/*
 * �Ż���ð������,ƽ����ð������������ɨ�轻��һ��,��ÿ����[lo,hi--]
 * �����Ļ�,��ʹ�����ĳһ�����Ѿ������˻��ǻ���
 * eg: 6 4 3 2 7 8 9 , 7 8 9�Ѿ�����,��������hi--����Ҫ���һ��.
 * �����Ϳ��ԸĽ�,һ��������¼����������Եĵط�
 * -> 4 3 2 [6] 7 8 9,������һ�˾�ֻ��4 3 2����.
 */
template <typename T>
void Vector<T>::bubbleSortFast(Rank lo, Rank hi) {
  while (lo < (hi = bubbleFast(lo, hi)))
    ;
}

template <typename T>
inline Rank Vector<T>::max(Rank lo, Rank hi) { //��[lo, hi]���ҳ������
   Rank mx = hi;
   while ( lo < hi-- ) //����ɨ��
      if ( _elem[hi] > _elem[mx] ) //���ϸ�Ƚ�
         mx = hi; //������max�ж��ʱ��֤�������ȣ�������֤selectionSort�ȶ�
   return mx;
}

/*
 * ÿ�ζ��ҵ�����Ĳ��ֵ����ֵ,�ŵ�δ���򲿷ֵ����
 */
template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {
     while ( lo < --hi )
      swap ( _elem[max ( lo, hi ) ], _elem[hi] ); //��[hi]��[lo, hi]�е�����߽���
}

//������,�͵��㷨�ռ�O(1),ʱ��ΪO(nlogn)
//��������floyd����,����ʱ��O(n),֮������delMax(),�����ֵ���������ĩβ.��ΪO(nlogn)
/*
 * 4 2 5 1 3
 *        4
 *      2   5
 *    1  3
 * ���Ѻ�: 
 *        5
 *      3   4
 *    1  2
 * 
 * [5 3 4 1 2] -> delMax
 *        4
 *      3   2
 *    1  
 *  
 * [4 3 2 1] 5  -> ����5�Ѵ�����ȷλ����...��������
 * 1 2 3 4 5
 * ��Ҳ��Ϊ�δ�С��������,��Ҫ����ѵ�ԭ��
 */
//[lo,hi)
template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {
  PQ_ComplHeap<T> H(_elem + lo, hi - lo);
  while (H.empty()) {  //������ժ�����Ԫ������������ĺ�׺��ֱ���ѿ�
    _elem[--hi] = H.delMax();
  }
  
}

template<typename T>
int Vector<T>::disordered() const { //������������������Ԫ�ضԵ�����
   int n = 0; //������
   for ( int i = 1; i < _size; i++ ) //��һ���_size - 1������Ԫ��
      if ( _elem[i - 1] > _elem[i] ) n++; //���������
   return n; //���������ҽ���n = 0
}

template <typename T> //����������˳����ң��������һ��Ԫ��e��λ�ã�ʧ��ʱ������lo - 1
Rank Vector<T>::find ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size
   while ( ( lo < hi-- ) && ( e != _elem[hi] ) ); //�Ӻ���ǰ��˳�����
   return hi; //��hi < lo������ζ��ʧ�ܣ�����hi������Ԫ�ص���
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
  return binSearch(_elem, lo, hi);
}

template <typename T>
Rank Vector<T>::binSearch(T* A, T const& e, Rank lo, Rank hi) {
  /*
  //�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�����ʧ��ʱ���򵥵ط���-1��������ָʾʧ�ܵ�λ��
  while (lo<hi) {
    Rank mi = (lo + hi) >> 1;
    if (e < A[mi]) hi = mi;
    else if (A[mi] < e)
      lo = mi;
    else
      return mi;
  }
  return -1;  //����ʧ��
  */
  while (lo<hi) {
    Rank mi = (lo + hi) >> 1;
    (e < A[mi]) hi = mi : lo = mi + 1;
  }  //���ҳɹ�ʱ����ֹͣ,ֱ��lo==hi
  return --lo;  //ѭ������ʱ��loΪ����e��Ԫ�ص���С�ȣ���lo - 1��������e��Ԫ�ص������
  //�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�����ʧ��ʱ���ܹ�����ʧ�ܵ�λ��
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const&) {
   if ( _elem ) delete [] _elem; //�ͷ�ԭ������
   copyFrom ( V._elem, 0, V.size() ); //���帴��
   return *this; //���ص�ǰ��������ã��Ա���ʽ��ֵ
}

template <typename T>
T Vector<T>::remove(Rank r) {
  T e = _elem[r]; //���ݱ�ɾ��Ԫ��
  remove ( r, r + 1 ); //��������ɾ���㷨����Ч�ڶ�����[r, r + 1)��ɾ��
  return e; //���ر�ɾ��Ԫ��
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
  if ( lo == hi ) return 0; //����Ч�ʿ��ǣ����������˻����������remove(0, 0)
  while ( hi < _size ) _elem[lo++] = _elem[hi++]; //[hi, _size)˳��ǰ��hi - lo����Ԫ
  _size = lo; //���¹�ģ��ֱ�Ӷ���β��[lo, _size = hi)����
  shrink(); //���б�Ҫ��������
  return hi - lo; //���ر�ɾ��Ԫ�ص���Ŀ
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
  expand(); //���б�Ҫ������
  for ( int i = _size; i > r; i-- ) _elem[i] = _elem[i-1]; //�Ժ���ǰ�����Ԫ��˳�κ���һ����Ԫ
  _elem[r] = e; _size++; //������Ԫ�ز���������
  return r; //������
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
  bubbleSortFast(lo, hi);
  //
}

template <typename T>
inline void Vector<T>::unsort(Rank lo, Rank hi) {}

//ɾ���ظ�Ԫ��
template <typename T>
int Vector<T>::deduplicate() {
  int oldsize = _size;
  Rank i = 1;//��elem[i]��ʼ
  while (i<_size) { //��ǰ�����һ�����Ԫ��_elem[i]
    (find(_elem[i], 0, i) < 0) ? ++i : remove(i); //����ǰ׺��Ѱ����֮��ͬ�ߣ�����һ����
    //������ͬ������������̣�����ɾ����ͬ��
   return oldsize - _size; //������ģ�仯��������ɾ��Ԫ������
  }
}

template <typename T>
int Vector<T>::uniquify() {
  Rank i = 0, j = 0; //���Ի��조���ڡ�Ԫ�ص���
  while (++j<_size) { //��һɨ�裬ֱ��ĩԪ��
    if (_elem[i] != _elem[j]) {//������ͬ��
      _elem[++i] = _elem[j]; //���ֲ�ͬԪ��ʱ����ǰ����������ǰ���Ҳ�
    }
  }
  _size = ++i;
  shrink(); //ֱ�ӽس�β������Ԫ��
  return j - i; //������ģ�仯��������ɾ��Ԫ������
}


template <typename T> void Vector<T>::traverse ( void ( *visit ) ( T& ) ) //��������ָ�����
{ for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); } //��������

template <typename T> template <typename VST> //Ԫ�����͡�������
void Vector<T>::traverse ( VST& visit ) //���������������
{ for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); } //��������

    
