#include <iostream>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#ifndef PAPER_H
#define PAPER_H

class Paper
{
  double sim;
  int p1;
  int p2;

public:		
  Paper(double s, int p1, int paper2);

  //get similarity
  double getSimilarity();

  //get the first paper
  int getFirstPaper();

  //get the second paper
  int getSecondPaper();
};

#endif
