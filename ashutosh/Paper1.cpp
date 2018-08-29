#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>
#include <stack>

using namespace std;

#include "Paper.h"

Paper::Paper(int i, int t, int s);
{
	this->id = i;
	this->track = t;
	this->session = s;
}

int Paper::getPaperID()
{
	return id;
}

int Paper::getTrack()
{
	return track;
}

int Paper::getSession()
{
	return session;
}