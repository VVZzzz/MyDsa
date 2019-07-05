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
  } while (s != (v = (++v) % n));  //�������ж���
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
  } while (s != (v = (++v) % n));
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

/**********����DFS����������*************/
//(BFSͬ��Ҳ���Եõ���������)
//��s��������DFS,���õ��������������ջstack��,ջ������˳��Ϊ��������
//���л�,������������,stackΪ��
template <typename Tv, typename Te>
stack<Tv> *Graph<Tv, Te>::tSort(int s) {
  reset();
  int v = s;
  int clock = 0;
  stack<Tv> *pSortStack = new stack<Tv>;
  do {
    if (status(v) == UNDISCOVERED) {
      //��������л�,�����ջ,����ѭ��.
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
        //ֻ��UNDISCOVERED�Ķ���ŵݹ�DFS
      case UNDISCOVERED:
        type(v, u) = TREE;
        parent(u) = v;
        if (!TSort(u, clock, pSortStack)) {
          return false;
        }
        break;
        //�����,�Ӵ�ָ����,˵���л�.
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
  //ע��ֻ��VISTED�Ķ���Ž�ջ
  status(v) = VISITED;
  pSortStack->push(vertex(v));
  return true;
}

/***********����DFS��˫��ͨ��ָ�(���Ҹ���з�֧��)**************/
//��ֻ����һ����ͨ����Ѱ�Ҹ���������
//����˼·:
// 1.�����������:
// ������ͨ��BFS���м�����ͨ��,Ȼ����ö��ÿ������,��������ɾ����,��ͨ�������ı䲻��.O(n(e+n))
// 2.������,��DFSģ����Ȼ��O(n+e)
//��������:Ҷ�ӽڵ㲻�����ǹؽڵ�,ʣ�µľ��ǿ��ڲ��ڵ��Ƿ��Ǹ��.
//�ڲ��ڵ����
// A---B---C,�鿴B�Ƿ��Ǹ��Ϳ�C�Ƿ��к��������B������,�������B���Ǹ��,û�о��Ǹ��.
//��ô���Ǿ���ʱ���ǩdTime()��hca()�����ж�C�Ƿ�������A,����˵�鿴C��hca(���ӵ��������)�Ƿ�ΪA��С��A(��A���ߵ�����)
template <typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s) {
  reset();
  int clock;
  int v = s;
  stack<int> vertexStack;
  do {
    if (UNDISCOVERED == status(v)) {
      BCC(v, clock, vertexStack);
      vertexStack.pop();  //�������һ������--����ǰ��ͨ������
    }
  } while (s != (v = (++v) % n));
}

//����ͼ,�˴���fTime(x)��Ϊhca(x) ,
// hca(x)�ĺ�����ָ��ǰ����ͨ��������ܵ�����ߵ����� ���� A->B->C
// hca(x)ֵԽСԽ��.
//���C�к����ָ��A,��hca(C)=A
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
        //���ڽӵ���еݹ�
        BCC(u, clock, vertexStack);
        //�ڽӵ�u�ݹ鷵�غ�,u��hca�����,��ô��ǰ�ڵ�ҲҪ����hca
        if (hca(u) < dTime(v)) {
          hca(v) = min(hca(u), hca(v));
        } else {
          //������º��u��hca��Ȼ����dTime(v),˵��v����һ���ؽڵ�(��Ϊ���ĺ��û����backward����������)
          //�򵯳���һ������,��c---(D---E---G---D),C�ǹؽڵ�,����DEG�������,��ΪDEG��hca��>dTime(c)
          while (vertexStack.top() != v)
            vertexStack.pop();  //����������������;
        }
        //�ݹ鷵�غ�
      case DISCOVERED:
        //����ڽӵ㱻����,˵���ڽӵ��Ǹýڵ��ĳ������,�Ǻ����.�ʸ���hca
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

/************************���ȼ�����PFS���***************************/
//���ȼ��������,֮ǰ��bfs,dfs����·��ÿ�ε�����ѡһ���¶�����з���.bfsѡdTime���,dfsѡfTime���
//Ҳ����˵���Ǹ��ݶ�������ȼ�,ÿ�ζ�ѡһ�����ȼ���ߵĵ�������.�������˼·bfs
// dfs������Ϊ������
//Ŀǰ���ʱ�临�Ӷ�ΪO(n*(n+e)),��O(n^2),����������ȶ��н����Ż�
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

//��priority��ʾ���ȼ�,ֵԽ�����ȼ�Խ��.
//ͨ����ͬ�����ȼ�������,��ʵ�ֲ�ͬ���㷨(BFS,DFS,Dijkstra,Prim)
//��ע��PFS���ʱ�临�Ӷ���O(n^2),(�������ȼ����¸��Ӷ�Ϊ����),�������ȶ��иĽ�
//��PFS���ʵ�ֵ�DFS��BFS,û�е�����DFS BFSʱ�临�Ӷ�O(n+e)
template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater) {
  //��ʼ����ǰ����,��ǰ�������PFS����
  priority(s) = 0;
  status(s) = VISITED;
  parent(s) = -1;
  //����һ������ͱ߼��뵽PFS����
  while (true) {
    for (int w = firstNbr(s); - 1 < w; w = nextNbr(s, w))  //ö��s�������ھ�w
      prioUpdater(this, s, w);  //���¶���w�����ȼ����丸����
    for (int shortest = INT_MAX, w = 0; w < n; w++)
      if (UNDISCOVERED == status(w))  //����δ����������Ķ�����
        if (shortest > priority(w))   //ѡ����һ�����ȼ���ߵĶ���s
        {
          shortest = priority(w);
          s = w;
        }
    if (status(s) == VISITED) break;  //���ж�����Ѽ���
    status(s) = VISITED;
    type(parent(s), s) = TREE;  //��s�����丸�����߼��������
  }
}

/*************���ȼ�������ܵ�Ӧ��-prim�㷨����С֧����****************/
//��ͨͼG��ĳһ�޻���ͨ��ͼT���������ж���,��Ϊ֧����.eg:
// A-B-C-A,��A-B-CΪ֧���� ֧��������n������,n-1����
//��֧�����ıߵ�Ȩ�غ���С,��Ϊ��С֧����.��С֧�������ܲ�Ψһ.
//����˼·:
// 1.�������: �ҵ�ÿ��֧����,ö�������ҵ���С��.��ҪO(n^(n-2))
// 2. prim�㷨:
//����ѡһ������A,��ô�������ͼGraph����Ϊ{A;Graph-A}������,֮������Ҫ���ľ���
//��һ����̵ı�����A��Graph-A,��ô���ھ���(A,B);Graph-(A,B)������,����ѭ��,���յõ��ĵ�һ�����־�����С֧����

// prim�㷨�Ķ������ȼ���������㵽֧�����ļ��ϵľ���
//�����: TREE�ı���֧����
template <typename Tv, typename Te>
void Graph<Tv, Te>::prim(int s) {
  reset();
  priority(s) = 0;  //��s�����ȼ�Ϊ0,������ȼ�
  //֧����,����n������n-1����
  for (int i = 0; i < n; i++) {
    //���뵱ǰ�Ķ���,�ͱ�
    status(s) = VISITED;
    if (-1 != parent(s)) type(parent(s), s) = TREE;
    for (int u = firstNbr(s); - 1 < u; u = nextNbr(s, u)) {
      if ((status(u) == UNDISCOVERED) && (priority(u) > weight(s, u))) {
        priority(u) = weight(s, u);  //
        parent(u) = s;               //��������Dijkstra�㷨��֮ͬ��
      }
    }
    //������δ���ֶ������ҵ����ȼ���ߵĵ�,��������ֵ��s������һ��ѭ��
    int minComp = INT_MAX;
    for (int w = 0; w < n; w++) {
      if ((status(w) == UNDISCOVERED) && (priority(w) < minComp)) {
        minComp = priority(w);
        s = w;
      }
    }
  }
}

//����PFS�������,���Prim�㷨�Ķ������ȼ�������(�������Prim�㷨����һ��)
template <typename Tv, typename Te>
struct PrimPU {
  virtual void operator()(Graph<Tv, Te> *g, int s, int u) {
    //����s���ڽӵ�u
    if (g->status(u) == UNDISCOVERED) {
      //�������ȼ�
      if (g->priority(u) > g->weight(s, u)) {
        g->priority(u) = g->weight(s, u);
        g->parent(u) = s;
      }
    }
  }
};

/*************���ȼ������㷨Ӧ��--���·��(Dijkstra�㷨)*******************/
//ͬ�����·�����ܲ�Ψһ,���������ȡ���ұߵ�Ȩֵ��Ϊ����
//��Ȼ��,���·��û�л�
// Dijkstra�㷨��Prim�㷨��������,ֻ���ڸ��¸��������ȼ�ʱ.
// Dijkstra�㷨�������ȼ���,�ö��㵽Դ��s����̾���.
// �����Ǹö��㵽֧�����������̾���
//ͬ�����������ȶ��н���ʱ�临�Ӷ�
template <typename Tv, typename Te>
void Graph<Tv, Te>::dijkstra(int s) {
  reset();
  priority(s) = 0;  //��������s���ȼ����,��s������������Ϊ0
  //û�л�,����n������,n-1����
  for (int i = 0; i < n; i++) {
    //����(parent(s), s)��
    status(s) = VISITED;
    if (-1 != parent(s)) type(parent(s), s) = TREE;
    //����s���ڽӵ�
    for (int u = firstNbr(s); - 1 < u; u = nextNbr(s, u)) {
      //�˴����¶�������ȼ���Prim�㷨��һ��,Prim�㷨��
      // priority(u) > (weight(s,u))
      if ((UNDISCOVERED == status(u)) &&
          (priority(u) > (priority(s) + weight(s, u)))) {
        priority(u) = priority(s) + weight(s, u);
        parent(u) = s;
      }
    }

    //�����ж����������ȼ���ߵ�(ֵ��С���Ǹ�),����������һ��ѭ��
    int minComp = INT_MAX;
    for (int w = 0; w < n; w++) {
      if ((UNDISCOVERED == status(w)) && (priority(w) < minComp)) {
        minComp = priority(w);
        s = w;
      }
    }
  }
} //ʱ�临�Ӷ���Ȼ��O(n^2)

//����PFS�������,���Dijkstra�㷨�Ķ������ȼ�������(�������Dijkstra�㷨����һ��)
template <typename Tv,typename Te>
struct DijkstraPU {
  void operator()(Graph<Tv, Te> *g, int s, int u) {
    if (g->status(u) == UNDISCOVERED) {
      if (g->priority(u) > (g->priority(s) + g->weight(s, u))) {
		  //�������ȼ��͸��ڵ�
        g->priority(u) = (g->priority(s) + g->weight(s, u));
        g->parent(u) = s;
      }
    }
  }
};

/***************����PFS�������: BFS������********************/
template <typename Tv,typename Te>
struct BfsPU {
  virtual void operator()(Graph<Tv, Te> *g, int s, int u) {
    if (g->status(u) == UNDISCOVERED) {
		//��������Ȩ�ض���1,�ȱ����ֵĶ���,���ȼ���
      if (g->priority(u) > g->priority(s) + 1) {
		  //�������ȼ��͸��ڵ�
        g->priority(u) = g->priority(s) + 1;
        g->parent(u) = s;
      }
    }
	  
  }

};

/**************����PFS�������: DFS������***********************/
template <typename Tv,typename Te>
struct DfsPU {
  virtual void operator()(Graph<Tv, Te> *g, int s, int u) {
    if (g->status(u) == UNDISCOVERED) {
		//��������Ȩ�ض���-1,�ȱ����ֵĶ���,���ȼ�Խ��
		//��󱻷��ֵĵ����ȼ����
      if (g->priority(u) > g->priority(s) - 1) {
		  //�������ȼ��͸��ڵ�
        g->priority(u) = g->priority(s) - 1;
        g->parent(u) = s;
		return; //ע�⣺��BfsPU()��ͬ������ֻҪ��һ���ڽӶ���ɸ��£�������������
		//wrh: ���returnӦ��û��ʵ��������. 
      }
    }
	  
  }
};

#endif
