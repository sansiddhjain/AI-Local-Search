#include <iostream>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#ifndef PAPER1_H
#define PAPER1_H

class Paper1
{
  int id;
  int track;
  int session;

public:
  Paper1(int i, int t, int s);

  int getPaperID();
  
  int getTrack();
  
  int getSession();
};

#endif
