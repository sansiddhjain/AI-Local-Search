#include <iostream>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#ifndef PAPER_H
#define PAPER_H

class Paper
{
  int sim;
  int p1;
  int p2;

public:
	Paper();
			
  Paper(int s, int p1, int paper2);

  //get similarity
  int getSimilarity();

  //get the first paper
  int getFirstPaper();

  //get the second paper
  int getSecondPaper();
};

#endif
