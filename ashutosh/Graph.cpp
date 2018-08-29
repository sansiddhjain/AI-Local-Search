#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#include "Graph.h"

Graph::Graph(int V)
{
  this->V = V;
  this->adj_list = new list<int>[V];
}

void Graph::addDirectedEdge(int v, int w)
{
  adj_list[v].push_back(w);
}

void Graph::addEdge(int v, int w)
{
  adj_list[v].push_back(w);
  adj_list[w].push_back(v);
}

void Graph::createGraph(double ** simMatrix, double threshold)
{
  for (int i = 0; i < this->V; i++)
  {
    for (int j = i+1; j < this->V; j++)
    {
      if (simMatrix[i][j] <= threshold)
        addEdge(i, j);
    }
  }
}


void Graph::RecursiveDFSStack(int v, bool visited[], stack<int> &Stack)
{
  visited[v] = true;

  list<int>::iterator i;
  for (i = adj_list[v].begin(); i != adj_list[v].end() ; i++)
  {
    if (!visited[*i])
      RecursiveDFSStack(*i, visited, Stack);
  }
  Stack.push(v);
}

void Graph::RecursiveDFSSet(int v, bool visited[], vector<int> &Set)
{
  visited[v] = true;

  list<int>::iterator i;
  for (i = adj_list[v].begin(); i != adj_list[v].end() ; i++)
  {
    if (!visited[*i])
      RecursiveDFSSet(*i, visited, Set);
  }
  Set.push_back(v);
  // cout << " test" << v;
  // fprintf(stderr, "%s\n", );
}

void Graph::calculateSCC(vector< vector<int> > &SCCs)
{
  stack<int> Stack;

  // Initialise visited array to false for all nodes
  bool *visited = new bool[V];
  for (int i = 0; i < V; i++)
    visited[i] = false;

  // Create stack of visited node as given by DFS
  for (int i = 0; i < V; i++)
  {
    if (visited[i] == false)
      RecursiveDFSStack(i, visited, Stack);
  }

  // Re-initialise visited array to false for second DFS sweep
  for (int i = 0; i < V; i++)
    visited[i] = false;

  // std::cout << "Stack elements after 1 round of DFS - " << '\n';
  // for (int i = 0; i < V; i++)
  // {
  //   std::cout << " " << Stack.top();
  //   Stack.pop();
  // }
  // std::cout << "" << '\n';

  while (Stack.empty() == false)
  {
    int v = Stack.top();
    Stack.pop();

    if (visited[v] == false)
    {
      vector<int> scc_set;
      RecursiveDFSSet(v, visited, scc_set);
      cout << '\n';
      SCCs.push_back(scc_set);
    }
  }
}
