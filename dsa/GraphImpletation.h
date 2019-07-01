#ifndef GRAPH_IMPLETATION_H_
#define GRAPH_IMPLETATION_H_
#pragma once
#include <queue>
#include "Graph.h"
using std::queue;

/*********BFS********/
template <typename Tv, typename Te>
//�Ӷ���s��ʼ����bfs (0<=s<n),����ȫͼ
void Graph<Tv, Te>::bfs(int s) {
  reset();
  int v = s;
  int clock = 0;  // dTime vTimeʱ���
  do {
    if (status(v) == UNDISCOVERED) BFS(v, clock);
  } while (s != (++v) % n);  //�������ж���
}

//ֻ����s����ͨ��
template <typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int s, int &clock) {
  queue<int> vertexQueue;
  vertexQueue.push(s);
  status(s) = DISCOVERED;
  //ֻҪ���в���
  while (!vertexQueue.empty()) {
    int v = vertexQueue.front();
    vertexQueue.pop();
    dTime(v) = ++clock;
    for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) {
      if (status(u) == UNDISCOVERED) {
        status(u) = DISCOVERED;
        type(v, u) = TREE;  // TREE����չ��
        vertexQueue.push(u);
        parent(u) = v;  // u�ĸ��ڵ�
      } else {
        status(v, u) = CROSS;  //���
      }
    }
    status(v) = VISITED;  //��������ڽӵ㶼��DISCOVERED,����Ϊ�ѱ������
  }
}

/**********DFS***********/
template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s) {
  reset();
  int v = s;
  int clock = 0;
  do {
    if (status(v) == UNDISCOVERED) DFS(v, clock);
  } while (s != (++v) % n);
}

//�ݹ�DFS
template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int s, int &clock) {
  int v = s;
  status(v) = DISCOVERED;
  dTime(v) == ++clock;
  for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) {
    //ע��ֻ��UNDISCOVERED���ڽӵ�Ż����DFS
    switch (status(u)) {
      case UNDISCOVERED:
        type(v, u) = TREE;
        parent(u) = v;
        DFS(u, clock);  //�ݹ�
        break;
      case DISCOVERED:
        //�����ں��ָ����,���Ǻ����
        type(v, u) = BACKWORD;
        break;
      case VISITED:
        //���ڽӵ��ѱ�VISTED
        //�������ڽӵ��ǵ�ǰ��ĺ��,���ΪFORWARD,�����ǿ��,���Ǳ��ֵ�
        type(v, u) = (dTime(v) < dTime(u) ? FORWARD : CROSS);
        break;
      default:
        break;
    }
  }
  //�ѱ��������ڽӵ�
  status(v) = VISITED;
  fTime(v) = ++clock;
}

#endif
