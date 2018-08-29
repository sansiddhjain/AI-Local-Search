#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#include "Paper1.h"

Paper1::Paper1(int i, int t, int s)
{
	this->id = i;
	this->track = t;
	this->session = s;
}

int Paper1::getPaperID()
{
	return id;
}

int Paper1::getTrack()
{
	return track;
}

int Paper1::getSession()
{
	return session;
}
