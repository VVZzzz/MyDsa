#pragma once
#include "SkipList.h"
#include <iterator>
#include <list>
using namespace std;
/******************************************************************************************
 * Skiplist���������㷨�����ڲ����ã�
 * ��ڣ�qlistΪ�����б�pΪqlist���׽ڵ�
 * ���ڣ����ɹ���pΪ���йؼ����������Ķ����ڵ㣬qlistΪp�����б�
 *       ����pΪ�������Ļ�����������Ӧ�ڲ�����k���������ҹؼ��룬qlistΪ��
 * Լ���������������ʱ����������ȡ�����
 * ʱ�临�Ӷ�: O(logN)
 ******************************************************************************************/
template <typename K, typename V>
bool Skiplist<K, V>::skipSearch(list<QuadList<Entry<K, V> >* >::iterator &itr,
                                QuadlistNode<Entry<K, V>> *&p, K &k) {
  
  //itrΪָ�����QList
  //pΪ�ò�list���׽ڵ�
  //�������²��ҹؼ���
  while (true) {
    //�Ըò���в���
    while (p->succ&&(p->entry.key<=k)) {
      p = p->succ;
    }
    p = p->pred;
    if (p->pred && (p->entry.key == k)) return true;  //����
    //������뵽��һ��
    itr++;
    //���ѱ��������в�,��ʧ��
    //����end()����list��list.end()
    if (itr == end()) return false;
    //����ת����һ��
    p = (p->pred) ? p->below : itr->first();
  }
}


//����:ʱ�临�Ӷ�O(logN)
template <typename K, typename V>
V *Skiplist<K, V>::get(K k) {
     if ( empty() ) return NULL;
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //�Ӷ���Quadlist��
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //�׽ڵ㿪ʼ
   return skipSearch ( qlist, p, k ) ? & ( p->entry.value ) : NULL; //���Ҳ�����
   //�ж������ʱ����������
}


//����:ʱ��O(logN)
template <typename K, typename V>
bool Skiplist<K, V>::put(K, V) {
     Entry<K, V> e = Entry<K, V> ( k, v ); //������Ĵ�������������ز�����������
   if ( empty() ) insertAsFirst ( new Quadlist<Entry<K, V>> ); //�����׸�Entry
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //�Ӷ����������
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //�׽ڵ����
   if ( skipSearch ( qlist, p, k ) ) //�����ʵ��Ĳ���λ�ã������ڹؼ���k�����һ���ڵ�p��
      while ( p->below ) p = p->below; //��������ͬ����������ǿ��ת������
   qlist = last(); //���£�������p���Ҳ࣬һ���������Ե׶����������
   QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove ( e, p ); //�½ڵ�b����������
   while ( rand() & 1 ) { //��Ͷ��Ӳ�ң���ȷ��������Ҫ�ٳ���һ�㣬��
      while ( qlist->data->valid ( p ) && !p->above ) p = p->pred; //�ҳ������ڴ˸߶ȵ����ǰ��
      if ( !qlist->data->valid ( p ) ) { //����ǰ����header
         if ( qlist == first() ) //�ҵ�ǰ������㣬����ζ�ű���
            insertAsFirst ( new Quadlist<Entry<K, V>> ); //���ȴ����µ�һ�㣬Ȼ��
         p = qlist->pred->data->first()->pred; //��pת����һ��Skiplist��header
      } else //���򣬿ɾ���
         p = p->above; //��p�������ø߶�
      qlist = qlist->pred; //����һ�㣬���ڸò�
      b = qlist->data->insertAfterAbove ( e, p, b ); //���½ڵ����p֮��b֮��
   }
   return true; //Dictionary�����ظ�Ԫ�أ��ʲ���سɹ���������Hashtable��Map���в���
}

//ɾ��: ʱ��O(logN)
template <typename K, typename V>
bool Skiplist<K, V>::remove(K k) {
     if ( empty() ) return false; //�ձ����
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //�Ӷ���Quadlist��
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //�׽ڵ㿪ʼ
   if ( !skipSearch ( qlist, p, k ) ) return false; //Ŀ����������ڣ�ֱ�ӷ���
   do { //��Ŀ��������ڣ����������֮��Ӧ����
      QuadlistNode<Entry<K, V>>* lower = p->below; //��ס��һ��ڵ㣬��
      qlist->data->remove ( p ); //ɾ����ǰ��ڵ㣬��
      p = lower; qlist = qlist->succ; //ת����һ��
   } while ( qlist->succ ); //���ϲ����ظ���ֱ������
   while ( !empty() && first()->data->empty() ) //��һ��
      List::remove ( first() ); //����ѿ��ܲ��������Ķ���Quadlist
   return true; //ɾ�������ɹ����
}
