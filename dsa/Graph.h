#ifndef GRAPH_H_
#define GRAPH_H_
#pragma once
#include <limits.h>
#include <stack>
using std::stack;
//�����״̬
typedef enum {
  UNDISCOVERED,
  DISCOVERED,
  VISITED  //ֻ�иõ�������ڽӵ㶼��DISCOVERED,�õ����VISITED
} VStatus;

//�ߵ�����
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWORD } EType;

//ͼģ���ܵ�һЩ�ӿ�,Tv��������,Te������
//ע���ǳ������
template <typename Tv,typename Te>
class Graph {
 private:
  void reset() {                   //���ж��㡢�ߵĸ�����Ϣ��λ
    for (int i = 0; i < n; i++) {  //���ж����
      status(i) = UNDISCOVERED;
      dTime(i) = fTime(i) = -1;  //״̬��ʱ���ǩ
      parent(i) = -1;
      priority(i) = INT_MAX;  //���ڱ������еģ����ڵ㣬���ȼ���
      for (int j = 0; j < n; j++)                     //���бߵ�
        if (exists(i, j)) type(i, j) = UNDETERMINED;  //����
    }
  }
  void BFS(int, int&);               //����ͨ�򣩹�����������㷨
  void DFS(int, int&);               //����ͨ��������������㷨
  void BCC(int, int&, stack<int>&);  //����ͨ�򣩻���DFS��˫��ͨ�����ֽ��㷨
  bool TSort(int, int&, stack<Tv>*);  //����ͨ�򣩻���DFS�����������㷨
  template <typename PU>
  void PFS(int, PU);  //����ͨ�����ȼ��������
  //���麯��
 public:
  // ����
  int n;                              //��������
  virtual int insert(Tv const&) = 0;  //���붥�㣬���ر��
  virtual Tv remove(int) = 0;  //ɾ�����㼰������ߣ����ظö�����Ϣ
  virtual Tv& vertex(int) = 0;     //����v�����ݣ��ö����ȷ���ڣ�
  virtual int inDegree(int) = 0;   //����v����ȣ��ö����ȷ���ڣ�
  virtual int outDegree(int) = 0;  //����v�ĳ��ȣ��ö����ȷ���ڣ�
  virtual int firstNbr(int) = 0;   //����v���׸��ڽӶ���
  virtual int nextNbr(int, int) = 0;  //����v�ģ�����ڶ���j�ģ���һ�ڽӶ���
  virtual VStatus& status(int) = 0;  //����v��״̬
  virtual int& dTime(int) = 0;       //����v��ʱ���ǩdTime
  virtual int& fTime(int) = 0;       //����v��ʱ���ǩfTime
  virtual int& parent(int) = 0;      //����v�ڱ������еĸ���
  virtual int& priority(int) = 0;    //����v�ڱ������е����ȼ���

  // �ߣ�����Լ��������߾�ͳһת��Ϊ�������һ������ߣ��Ӷ�������ͼ��������ͼ������
  int e;                              //������
  virtual bool exists(int, int) = 0;  //��(v, u)�Ƿ����
  virtual void insert(Te const&, int, int,
                      int) = 0;  //�ڶ���v��u֮�����Ȩ��Ϊw�ı�e
  virtual Te remove(int, int) = 0;  //ɾ������v��u֮��ı�e�����ظñ���Ϣ
  virtual EType& type(int, int) = 0;  //��(v, u)������
  virtual Te& edge(int, int) = 0;  //��(v, u)�����ݣ��ñߵ�ȷ���ڣ�
  virtual int& weight(int, int) = 0;  //��(v, u)��Ȩ��

  // �㷨
  void bfs(int);                      //������������㷨
  void dfs(int);                      //������������㷨
  void bcc(int);                      //����DFS��˫��ͨ�����ֽ��㷨
  stack<Tv>* tSort(int);              //����DFS�����������㷨
  //�����������ǻ���PFS���ȼ��������
  //��ʵ���õ�����̰�Ĳ���
  void prim(int);                     //��С֧����Prim�㷨
  void dijkstra(int);                 //���·��Dijkstra�㷨
  template <typename PU>
  void pfs(int, PU);  //���ȼ��������
};
#endif
