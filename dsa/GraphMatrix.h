#ifndef GRAPH_MATRIX_H_
#define GRAPH_MATRIX_H_
#pragma once
//���ļ������ڽӾ�����ж�ͼ�ı�ʾ
#include <vector>
#include "Graph.h"
using std::vector;
template <typename Tv>
struct Vertex {
  Tv data;
  int inDegree, outDegree;
  VStatus status;
  //ʱ���ǩ,dtime�Ǹýڵ㱻���ֵ�ʱ���,ftime�Ǹýڵ㱻������ϵ�ʱ���
  //�������ָ,����ڵ�������ڽӵ㶼������
  int dtime, ftime;
  //�������еĸ��ڵ�,������
  //������ָ��DFS�����õ�����
  int parent, priority;
  Vertex(Tv const& d = (Tv)0)
      :  //�����¶���
        data(d),
        inDegree(0),
        outDegree(0),
        status(UNDISCOVERED),
        dTime(-1),
        fTime(-1),
        parent(-1),
        priority(INT_MAX) {}  //�ݲ�����Ȩ�����
};

template <typename Te>
struct Edge {
  Te data;                                                              //����
  int weight;                                                           //Ȩ��
  EType type;                                                           //����
  Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {}  //����
};

//���ڽӾ����ʾ��ͼ
template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te> {
 private:
  vector<Vertex<Tv>> V;         //���㼯
  vector<vector<Edge<Te>*>> E;  //�߼�
 public:
  GraphMatrix() { n = e = 0; }
  ~GraphMatrix() {                 //����
    for (int j = 0; j < n; j++)    //���ж�̬������
      for (int k = 0; k < n; k++)  //�߼�¼
        delete E[j][k];            //�������
  }
  // ����Ļ�����������ѯ��i�����㣨0 <= i < n��
  virtual Tv& vertex(int i) { return V[i].data; }          //����
  virtual int inDegree(int i) { return V[i].inDegree; }    //���
  virtual int outDegree(int i) { return V[i].outDegree; }  //����
  virtual int firstNbr(int i) { return nextNbr(i, n); }    //�׸��ڽӶ���
  //����ڶ���j����һ�ڽӶ��㣨�����ڽӱ�����Ч�ʣ�
  virtual int nextNbr(int i, int j) {
    while ((-1 < j) && (!exists(i, --j)))
      ;
    return j;
  }                                                       //����������̽
  virtual VStatus& status(int i) { return V[i].status; }  //״̬
  virtual int& dTime(int i) { return V[i].dTime; }        //ʱ���ǩdTime
  virtual int& fTime(int i) { return V[i].fTime; }        //ʱ���ǩfTime
  virtual int& parent(int i) { return V[i].parent; }  //�ڱ������еĸ���
  virtual int& priority(int i) { return V[i].priority; }  //�ڱ������е����ȼ���

  //����Ķ�̬����
  //���һ���¶���
  virtual int insert(Tv const& ver) {  //����һ���µĽڵ�,���������
    for (int i = 0; i < n; i++)
      E[i].push_back(NULL);  //����ÿ���ڵ�ı߼�Ӧ��һ��
    n++;
    //������һ������µĽڵ�ı߼���
    E.push_back(vector<Edge<Te*>>(n, NULL));
    //��ӽ����㼯��
    V.push_back(Vertex<Tv>(ver));
    return n - 1;
  }

  //ɾ����i������,0<=i<n
  virtual Tv remove(int i) {
    //ɾ�������������г���
    for (int j = 0; j < n; j++) {
      if (exists(i, j)) {
        delete E[i][j];
        V[j].inDegree--;
      }
    }
    //ɾ����i��
    E.erase(E.begin() + i);
    //�ڶ��㼯��ɾ���Ǹ�����
    Tv eraseV = V[i].data;
    V.erase(V.begin() + i);
    n--;
    //ɾ�����������������
    for (int j = 0; j < n; j++) {
      if (E[j][i]) {
        delete E[j][i];
        V[j].outDegree--;
        E[j].erase(E[j].begin() + i);
      }
    }
    //���ر�ɾ���Ķ�����Ϣ
    return eraseV;
  }

  // �ߵ�ȷ�ϲ���
  virtual bool exists(int i, int j)  //��(i, j)�Ƿ����
  {
    return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL;
  }
  // �ߵĻ�����������ѯ����i��j֮������ߣ�0 <= i, j < n��exists(i, j)��
  virtual EType& type(int i, int j) { return E[i][j]->type; }    //��(i, j)������
  virtual Te& edge(int i, int j) { return E[i][j]->data; }       //��(i, j)������
  virtual int& weight(int i, int j) { return E[i][j]->weight; }  //��(i, j)��Ȩ��
  //�ߵĶ�̬����
  //���һ��Ȩ��Ϊw,e(i,j)�ı�
  virtual void insert(Te const& edge, int w, int i, int j) {
    if (exists(i, j)) return;
    E[i][j] = new Edge<Te>(edge, w);
    V[i].outDegree++;
    V[j].inDegree++;
    e++;
  }
  //ɾ��e(i,j)
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
