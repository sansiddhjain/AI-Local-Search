#include <iostream>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#ifndef GRAPH_H
#define GRAPH_H

class Graph
{
  int V;
  list<int> *adj_list;

public:
  Graph(int V);
  void addDirectedEdge(int v, int w);
  void addEdge(int v, int w);
  void createGraph(double ** simMatrix, double threshold);
  void RecursiveDFSStack(int v, bool visited[], stack<int> &Stack);
  void RecursiveDFSSet(int v, bool visited[], vector<int> &Set);
  void calculateSCC(vector< vector<int> > &SCCs);
};

#endif
