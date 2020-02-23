#pragma once
#include "SkipList.h"
#include <iterator>
#include <list>
using namespace std;
/******************************************************************************************
 * Skiplist词条查找算法（供内部调用）
 * 入口：qlist为顶层列表，p为qlist的首节点
 * 出口：若成功，p为命中关键码所属塔的顶部节点，qlist为p所属列表
 *       否则，p为所属塔的基座，该塔对应于不大于k的最大且最靠右关键码，qlist为空
 * 约定：多个词条命中时，沿四联表取最靠后者
 * 时间复杂度: O(logN)
 ******************************************************************************************/
template <typename K, typename V>
bool Skiplist<K, V>::skipSearch(list<QuadList<Entry<K, V> >* >::iterator &itr,
                                QuadlistNode<Entry<K, V>> *&p, K &k) {
  
  //itr为指定层的QList
  //p为该层list的首节点
  //向右向下查找关键码
  while (true) {
    //对该层进行查找
    while (p->succ&&(p->entry.key<=k)) {
      p = p->succ;
    }
    p = p->pred;
    if (p->pred && (p->entry.key == k)) return true;  //命中
    //否则进入到下一层
    itr++;
    //若已遍历完所有层,则失败
    //这里end()代表list的list.end()
    if (itr == end()) return false;
    //否则转到下一层
    p = (p->pred) ? p->below : itr->first();
  }
}


//查找:时间复杂度O(logN)
template <typename K, typename V>
V *Skiplist<K, V>::get(K k) {
     if ( empty() ) return NULL;
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //从顶层Quadlist的
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //首节点开始
   return skipSearch ( qlist, p, k ) ? & ( p->entry.value ) : NULL; //查找并报告
   //有多个命中时靠后者优先
}


//插入:时间O(logN)
template <typename K, typename V>
bool Skiplist<K, V>::put(K, V) {
     Entry<K, V> e = Entry<K, V> ( k, v ); //待插入的词条（将被随机地插入多个副本）
   if ( empty() ) insertAsFirst ( new Quadlist<Entry<K, V>> ); //插入首个Entry
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //从顶层四联表的
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //首节点出发
   if ( skipSearch ( qlist, p, k ) ) //查找适当的插入位置（不大于关键码k的最后一个节点p）
      while ( p->below ) p = p->below; //若已有雷同词条，则需强制转到塔底
   qlist = last(); //以下，紧邻于p的右侧，一座新塔将自底而上逐层生长
   QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove ( e, p ); //新节点b即新塔基座
   while ( rand() & 1 ) { //经投掷硬币，若确定新塔需要再长高一层，则
      while ( qlist->data->valid ( p ) && !p->above ) p = p->pred; //找出不低于此高度的最近前驱
      if ( !qlist->data->valid ( p ) ) { //若该前驱是header
         if ( qlist == first() ) //且当前已是最顶层，则意味着必须
            insertAsFirst ( new Quadlist<Entry<K, V>> ); //首先创建新的一层，然后
         p = qlist->pred->data->first()->pred; //将p转至上一层Skiplist的header
      } else //否则，可径自
         p = p->above; //将p提升至该高度
      qlist = qlist->pred; //上升一层，并在该层
      b = qlist->data->insertAfterAbove ( e, p, b ); //将新节点插入p之后、b之上
   }
   return true; //Dictionary允许重复元素，故插入必成功——这与Hashtable等Map略有差异
}

//删除: 时间O(logN)
template <typename K, typename V>
bool Skiplist<K, V>::remove(K k) {
     if ( empty() ) return false; //空表情况
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //从顶层Quadlist的
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //首节点开始
   if ( !skipSearch ( qlist, p, k ) ) return false; //目标词条不存在，直接返回
   do { //若目标词条存在，则逐层拆除与之对应的塔
      QuadlistNode<Entry<K, V>>* lower = p->below; //记住下一层节点，并
      qlist->data->remove ( p ); //删除当前层节点，再
      p = lower; qlist = qlist->succ; //转入下一层
   } while ( qlist->succ ); //如上不断重复，直到塔基
   while ( !empty() && first()->data->empty() ) //逐一地
      List::remove ( first() ); //清除已可能不含词条的顶层Quadlist
   return true; //删除操作成功完成
}
