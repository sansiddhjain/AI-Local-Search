#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#include "Paper.h"

Paper::Paper(double s, int p1, int paper2)
{
	this->sim = s;
	this->p1 = p1;
	this->p2 = paper2;
}

double Paper::getSimilarity()
{
	return sim;
}

int Paper::getFirstPaper()
{
	return p1;
}

int Paper::getSecondPaper()
{
	return p2;
}
