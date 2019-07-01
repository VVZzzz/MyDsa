#ifndef GRAPH_IMPLETATION_H_
#define GRAPH_IMPLETATION_H_
#pragma once
#include <queue>
#include "Graph.h"
using std::queue;

/*********BFS********/
template <typename Tv, typename Te>
//从顶点s开始进行bfs (0<=s<n),遍历全图
void Graph<Tv, Te>::bfs(int s) {
  reset();
  int v = s;
  int clock = 0;  // dTime vTime时间点
  do {
    if (status(v) == UNDISCOVERED) BFS(v, clock);
  } while (s != (++v) % n);  //遍历所有顶点
}

//只遍历s的连通域
template <typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int s, int &clock) {
  queue<int> vertexQueue;
  vertexQueue.push(s);
  status(s) = DISCOVERED;
  //只要队列不空
  while (!vertexQueue.empty()) {
    int v = vertexQueue.front();
    vertexQueue.pop();
    dTime(v) = ++clock;
    for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) {
      if (status(u) == UNDISCOVERED) {
        status(u) = DISCOVERED;
        type(v, u) = TREE;  // TREE边拓展树
        vertexQueue.push(u);
        parent(u) = v;  // u的父节点
      } else {
        status(v, u) = CROSS;  //跨边
      }
    }
    status(v) = VISITED;  //如果所有邻接点都是DISCOVERED,则认为已遍历完成
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

//递归DFS
template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int s, int &clock) {
  int v = s;
  status(v) = DISCOVERED;
  dTime(v) == ++clock;
  for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) {
    //注意只有UNDISCOVERED的邻接点才会继续DFS
    switch (status(u)) {
      case UNDISCOVERED:
        type(v, u) = TREE;
        parent(u) = v;
        DFS(u, clock);  //递归
        break;
      case DISCOVERED:
        //是属于后代指向父亲,故是后向边
        type(v, u) = BACKWORD;
        break;
      case VISITED:
        //该邻接点已被VISTED
        //如果这个邻接点是当前点的后代,则边为FORWARD,否则是跨边,即是表兄弟
        type(v, u) = (dTime(v) < dTime(u) ? FORWARD : CROSS);
        break;
      default:
        break;
    }
  }
  //已遍历所有邻接点
  status(v) = VISITED;
  fTime(v) = ++clock;
}

#endif
