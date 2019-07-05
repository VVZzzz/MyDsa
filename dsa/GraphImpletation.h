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
  } while (s != (v = (++v) % n));  //遍历所有顶点
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
  } while (s != (v = (++v) % n));
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

/**********基于DFS的拓扑排序*************/
//(BFS同样也可以得到拓扑排序)
//从s出发进行DFS,将得到的拓扑排序存入栈stack中,栈弹出的顺序即为拓扑排序
//若有环,则无拓扑排序,stack为空
template <typename Tv, typename Te>
stack<Tv> *Graph<Tv, Te>::tSort(int s) {
  reset();
  int v = s;
  int clock = 0;
  stack<Tv> *pSortStack = new stack<Tv>;
  do {
    if (status(v) == UNDISCOVERED) {
      //如果发现有环,则清空栈,跳出循环.
      if (!TSort(v, clock, pSortStack)) {
        while (!pSortStack->empty()) {
          pSortStack->pop();
        }
        break;
      }
    }
  } while (s != (v = (++v) % n));
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int s, int &clock, stack<Tv> *pSortStack) {
  int v = s;
  status(v) = DISCOVERED;
  dTime(v) = ++clock;
  for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) {
    switch (status(u)) {
        //只有UNDISCOVERED的顶点才递归DFS
      case UNDISCOVERED:
        type(v, u) = TREE;
        parent(u) = v;
        if (!TSort(u, clock, pSortStack)) {
          return false;
        }
        break;
        //后向边,子代指向父亲,说明有环.
      case DISCOVERED:
        type(v, u) = BACKWORD;
        return false;
        break;
      case VISITED:
        type(v, u) = (dTime(v) < dTime(u) ? FORWARD : CROSS);
        break;
      default:
        break;
    }
  }
  //注意只有VISTED的顶点才进栈
  status(v) = VISITED;
  pSortStack->push(vertex(v));
  return true;
}

/***********基于DFS的双连通域分割(即找割点或叫分支点)**************/
//即只有在一个连通域中寻找割点才有意义
//基本思路:
// 1.暴力搜索割点:
// 即我先通过BFS找有几个连通域,然后再枚举每个顶点,看看将其删除后,连通域个数会改变不会.O(n(e+n))
// 2.本方法,是DFS模板仍然是O(n+e)
//本方法是:叶子节点不可能是关节点,剩下的就是看内部节点是否是割点.
//内部节点比如
// A---B---C,查看B是否是割点就看C是否有后向边练到B的祖先,如果有则B不是割点,没有就是割点.
//那么我们就用时间标签dTime()和hca()进行判断C是否能练到A,或者说查看C的hca(连接的最高祖先)是否为A或小于A(比A更高的祖先)
template <typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s) {
  reset();
  int clock;
  int v = s;
  stack<int> vertexStack;
  do {
    if (UNDISCOVERED == status(v)) {
      BCC(v, clock, vertexStack);
      vertexStack.pop();  //弹出最后一个顶点--即当前连通域的起点
    }
  } while (s != (v = (++v) % n));
}

//无向图,此处用fTime(x)作为hca(x) ,
// hca(x)的含义是指当前顶点通过后向边能到达最高的祖先 比如 A->B->C
// hca(x)值越小越高.
//如果C有后向边指向A,则hca(C)=A
#define hca(x) fTime(x)

template <typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int s, int &clock, stack<int> &vertexStack) {
  int v = s;
  status(v) = DISCOVERED;
  dTime(v) = hca(x) = ++clock;
  vertexStack.push(v);
  for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) {
    switch (status(u)) {
      case UNDISCOVERED:
        status(u) = DISCOVERED;
        parent(u) = v;
        type(v, u) = TREE;
        //对邻接点进行递归
        BCC(u, clock, vertexStack);
        //邻接点u递归返回后,u的hca会更新,那么当前节点也要更新hca
        if (hca(u) < dTime(v)) {
          hca(v) = min(hca(u), hca(v));
        } else {
          //如果更新后的u的hca仍然大于dTime(v),说明v就是一个关节点(因为他的后代没有能backward到它的祖先)
          //则弹出这一个区域,即c---(D---E---G---D),C是关节点,弹出DEG这个区域,因为DEG的hca都>dTime(c)
          while (vertexStack.top() != v)
            vertexStack.pop();  //弹出可用作其他用途
        }
        //递归返回后
      case DISCOVERED:
        //如果邻接点被发现,说明邻接点是该节点的某个祖先,是后向边.故更新hca
        type(v, u) = BACKWORD;
        if (u != parent(v)) hca(v) = min(hca(v), dTime(u));
        break;
      case VISITED:
        type(v, u) = (dTime(v) < dTime(u) ? FORWARD : CROSS);
        break;
      default:
        break;
    }
  }
  status(v) = VISITED;
}

/************************优先级搜索PFS框架***************************/
//优先级搜索框架,之前的bfs,dfs的套路是每次迭代都选一个新顶点进行访问.bfs选dTime早的,dfs选fTime早的
//也就是说都是根据顶点的优先级,每次都选一个优先级最高的点来访问.按照这个思路bfs
// dfs都可纳为这个框架
//目前这个时间复杂度为O(n*(n+e)),即O(n^2),后面可用优先队列进行优化
template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater) {
  reset();
  int v = s;
  do {
    if (status(v) == UNDISCOVERED) {
      PFS(v, prioUpdater);
    }
  } while (s ! = (v = (++v) % n));
}

//用priority表示优先级,值越低优先级越高.
//通过不同的优先级更新器,可实现不同的算法(BFS,DFS,Dijkstra,Prim)
//但注意PFS框架时间复杂度是O(n^2),(假设优先级更新复杂度为常数),可用优先队列改进
//用PFS框架实现的DFS和BFS,没有单独的DFS BFS时间复杂度O(n+e)
template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater) {
  //初始化当前顶点,当前顶点加入PFS树中
  priority(s) = 0;
  status(s) = VISITED;
  parent(s) = -1;
  //将下一个顶点和边加入到PFS树中
  while (true) {
    for (int w = firstNbr(s); - 1 < w; w = nextNbr(s, w))  //枚举s的所有邻居w
      prioUpdater(this, s, w);  //更新顶点w的优先级及其父顶点
    for (int shortest = INT_MAX, w = 0; w < n; w++)
      if (UNDISCOVERED == status(w))  //从尚未加入遍历树的顶点中
        if (shortest > priority(w))   //选出下一个优先级最高的顶点s
        {
          shortest = priority(w);
          s = w;
        }
    if (status(s) == VISITED) break;  //所有顶点均已加入
    status(s) = VISITED;
    type(parent(s), s) = TREE;  //将s及与其父的联边加入遍历树
  }
}

/*************优先级搜索框架的应用-prim算法求最小支撑树****************/
//连通图G的某一无环联通子图T若覆盖所有顶点,则为支撑树.eg:
// A-B-C-A,则A-B-C为支撑树 支撑树总是n个顶点,n-1条边
//当支撑树的边的权重和最小,则为最小支撑树.最小支撑树可能不唯一.
//基本思路:
// 1.暴力求解: 找到每个支撑树,枚举他们找到最小的.需要O(n^(n-2))
// 2. prim算法:
//先任选一个顶点A,那么现在这个图Graph被分为{A;Graph-A}两部分,之后我们要做的就是
//找一条最短的边连接A和Graph-A,那么现在就是(A,B);Graph-(A,B)两部分,依次循环,最终得到的第一个部分就是最小支持树

// prim算法的顶点优先级即这个顶点到支撑树的集合的距离
//最后结果: TREE的边是支撑树
template <typename Tv, typename Te>
void Graph<Tv, Te>::prim(int s) {
  reset();
  priority(s) = 0;  //设s的优先级为0,最高优先级
  //支撑树,引入n个顶点n-1条边
  for (int i = 0; i < n; i++) {
    //引入当前的顶点,和边
    status(s) = VISITED;
    if (-1 != parent(s)) type(parent(s), s) = TREE;
    for (int u = firstNbr(s); - 1 < u; u = nextNbr(s, u)) {
      if ((status(u) == UNDISCOVERED) && (priority(u) > weight(s, u))) {
        priority(u) = weight(s, u);  //
        parent(u) = s;               //这里是与Dijkstra算法不同之处
      }
    }
    //从所有未发现顶点中找到优先级最高的点,并将它赋值给s开启下一轮循环
    int minComp = INT_MAX;
    for (int w = 0; w < n; w++) {
      if ((status(w) == UNDISCOVERED) && (priority(w) < minComp)) {
        minComp = priority(w);
        s = w;
      }
    }
  }
}

//根据PFS搜索框架,针对Prim算法的顶点优先级更新器(和上面的Prim算法几乎一致)
template <typename Tv, typename Te>
struct PrimPU {
  virtual void operator()(Graph<Tv, Te> *g, int s, int u) {
    //对于s的邻接点u
    if (g->status(u) == UNDISCOVERED) {
      //更新优先级
      if (g->priority(u) > g->weight(s, u)) {
        g->priority(u) = g->weight(s, u);
        g->parent(u) = s;
      }
    }
  }
};

/*************优先级搜索算法应用--最短路径(Dijkstra算法)*******************/
//同样最短路径可能不唯一,这里假设任取并且边的权值都为正数
//显然的,最短路径没有环
// Dijkstra算法和Prim算法及其类似,只是在更新各顶点优先级时.
// Dijkstra算法顶点优先级是,该顶点到源点s的最短距离.
// 而不是该顶点到支撑树区域的最短距离
//同样可以用优先队列降低时间复杂度
template <typename Tv, typename Te>
void Graph<Tv, Te>::dijkstra(int s) {
  reset();
  priority(s) = 0;  //首先设置s优先级最高,即s到自身距离最短为0
  //没有环,遍历n个顶点,n-1条边
  for (int i = 0; i < n; i++) {
    //引入(parent(s), s)边
    status(s) = VISITED;
    if (-1 != parent(s)) type(parent(s), s) = TREE;
    //遍历s的邻接点
    for (int u = firstNbr(s); - 1 < u; u = nextNbr(s, u)) {
      //此处更新顶点的优先级和Prim算法不一样,Prim算法是
      // priority(u) > (weight(s,u))
      if ((UNDISCOVERED == status(u)) &&
          (priority(u) > (priority(s) + weight(s, u)))) {
        priority(u) = priority(s) + weight(s, u);
        parent(u) = s;
      }
    }

    //在所有顶点中找优先级最高的(值最小的那个),让它继续下一次循环
    int minComp = INT_MAX;
    for (int w = 0; w < n; w++) {
      if ((UNDISCOVERED == status(w)) && (priority(w) < minComp)) {
        minComp = priority(w);
        s = w;
      }
    }
  }
} //时间复杂度仍然是O(n^2)

//根据PFS搜索框架,针对Dijkstra算法的顶点优先级更新器(和上面的Dijkstra算法几乎一致)
template <typename Tv,typename Te>
struct DijkstraPU {
  void operator()(Graph<Tv, Te> *g, int s, int u) {
    if (g->status(u) == UNDISCOVERED) {
      if (g->priority(u) > (g->priority(s) + g->weight(s, u))) {
		  //更新优先级和父节点
        g->priority(u) = (g->priority(s) + g->weight(s, u));
        g->parent(u) = s;
      }
    }
  }
};

/***************基于PFS搜索框架: BFS更新器********************/
template <typename Tv,typename Te>
struct BfsPU {
  virtual void operator()(Graph<Tv, Te> *g, int s, int u) {
    if (g->status(u) == UNDISCOVERED) {
		//即看作边权重都是1,先被发现的顶点,优先级高
      if (g->priority(u) > g->priority(s) + 1) {
		  //更新优先级和父节点
        g->priority(u) = g->priority(s) + 1;
        g->parent(u) = s;
      }
    }
	  
  }

};

/**************基于PFS搜索框架: DFS更新器***********************/
template <typename Tv,typename Te>
struct DfsPU {
  virtual void operator()(Graph<Tv, Te> *g, int s, int u) {
    if (g->status(u) == UNDISCOVERED) {
		//即看作边权重都是-1,先被发现的顶点,优先级越低
		//最后被发现的点优先级最高
      if (g->priority(u) > g->priority(s) - 1) {
		  //更新优先级和父节点
        g->priority(u) = g->priority(s) - 1;
        g->parent(u) = s;
		return; //注意：与BfsPU()不同，这里只要有一个邻接顶点可更新，即可立即返回
		//wrh: 这个return应该没有实质性作用. 
      }
    }
	  
  }
};

#endif
