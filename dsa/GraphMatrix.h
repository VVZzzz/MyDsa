#ifndef GRAPH_MATRIX_H_
#define GRAPH_MATRIX_H_
#pragma once
//此文件是用邻接矩阵进行对图的表示
#include <vector>
#include "Graph.h"
using std::vector;
template <typename Tv>
struct Vertex {
  Tv data;
  int inDegree, outDegree;
  VStatus status;
  //时间标签,dtime是该节点被发现的时间点,ftime是该节点被访问完毕的时间点
  //访问完毕指,这个节点的所有邻接点都被发现
  int dtime, ftime;
  //遍历树中的父节点,优先数
  //遍历树指用DFS搜索得到的树
  int parent, priority;
  Vertex(Tv const& d = (Tv)0)
      :  //构造新顶点
        data(d),
        inDegree(0),
        outDegree(0),
        status(UNDISCOVERED),
        dTime(-1),
        fTime(-1),
        parent(-1),
        priority(INT_MAX) {}  //暂不考虑权重溢出
};

template <typename Te>
struct Edge {
  Te data;                                                              //数据
  int weight;                                                           //权重
  EType type;                                                           //类型
  Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {}  //构造
};

//用邻接矩阵表示的图
template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te> {
 private:
  vector<Vertex<Tv>> V;         //顶点集
  vector<vector<Edge<Te>*>> E;  //边集
 public:
  GraphMatrix() { n = e = 0; }
  ~GraphMatrix() {                 //析构
    for (int j = 0; j < n; j++)    //所有动态创建的
      for (int k = 0; k < n; k++)  //边记录
        delete E[j][k];            //逐条清除
  }
  // 顶点的基本操作：查询第i个顶点（0 <= i < n）
  virtual Tv& vertex(int i) { return V[i].data; }          //数据
  virtual int inDegree(int i) { return V[i].inDegree; }    //入度
  virtual int outDegree(int i) { return V[i].outDegree; }  //出度
  virtual int firstNbr(int i) { return nextNbr(i, n); }    //首个邻接顶点
  //相对于顶点j的下一邻接顶点（改用邻接表可提高效率）
  virtual int nextNbr(int i, int j) {
    while ((-1 < j) && (!exists(i, --j)))
      ;
    return j;
  }                                                       //逆向线性试探
  virtual VStatus& status(int i) { return V[i].status; }  //状态
  virtual int& dTime(int i) { return V[i].dTime; }        //时间标签dTime
  virtual int& fTime(int i) { return V[i].fTime; }        //时间标签fTime
  virtual int& parent(int i) { return V[i].parent; }  //在遍历树中的父亲
  virtual int& priority(int i) { return V[i].priority; }  //在遍历树中的优先级数

  //顶点的动态操作
  //添加一个新顶点
  virtual int insert(Tv const& ver) {  //插入一个新的节点,返回其序号
    for (int i = 0; i < n; i++)
      E[i].push_back(NULL);  //首先每个节点的边集应加一个
    n++;
    //再增添一组这个新的节点的边集合
    E.push_back(vector<Edge<Te*>>(n, NULL));
    //添加进顶点集合
    V.push_back(Vertex<Tv>(ver));
    return n - 1;
  }

  //删除第i个顶点,0<=i<n
  virtual Tv remove(int i) {
    //删除这个顶点的所有出边
    for (int j = 0; j < n; j++) {
      if (exists(i, j)) {
        delete E[i][j];
        V[j].inDegree--;
      }
    }
    //删除第i行
    E.erase(E.begin() + i);
    //在顶点集中删除那个顶点
    Tv eraseV = V[i].data;
    V.erase(V.begin() + i);
    n--;
    //删除这个顶点的所有入边
    for (int j = 0; j < n; j++) {
      if (E[j][i]) {
        delete E[j][i];
        V[j].outDegree--;
        E[j].erase(E[j].begin() + i);
      }
    }
    //返回被删除的顶点信息
    return eraseV;
  }

  // 边的确认操作
  virtual bool exists(int i, int j)  //边(i, j)是否存在
  {
    return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL;
  }
  // 边的基本操作：查询顶点i与j之间的联边（0 <= i, j < n且exists(i, j)）
  virtual EType& type(int i, int j) { return E[i][j]->type; }    //边(i, j)的类型
  virtual Te& edge(int i, int j) { return E[i][j]->data; }       //边(i, j)的数据
  virtual int& weight(int i, int j) { return E[i][j]->weight; }  //边(i, j)的权重
  //边的动态操作
  //添加一个权重为w,e(i,j)的边
  virtual void insert(Te const& edge, int w, int i, int j) {
    if (exists(i, j)) return;
    E[i][j] = new Edge<Te>(edge, w);
    V[i].outDegree++;
    V[j].inDegree++;
    e++;
  }
  //删除e(i,j)
  virtual Te remove(int i, int j) {
    Te res = edge(i, j);
    delete E[i][j];
    E[i][j] = NULL;
    e--;
    V[i].outDegree--;
    V[j].inDegree--;
    return res;
  }
};

#endif
