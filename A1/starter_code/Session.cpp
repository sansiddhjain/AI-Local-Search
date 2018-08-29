/*
 * File:   Session.cpp
 * Author: Kapil Thakkar
 *
 */

#include <errno.h>
#include <vector>

#include "Session.h"

Session::Session ( )
{
	papersInSession = 0;
}

Session::Session ( int papersInSession )
{
	this->papersInSession = papersInSession;
	initPapers ( papersInSession );
}

void Session::initPapers ( int papersInSession )
{
	this->papers = ( int * ) malloc ( sizeof (int ) * papersInSession );
	for ( int i = 0; i < papersInSession; i++ )
	{
		papers[i] = -1;
	}
}

int Session::getNumberOfPapers ( )
{
	return papersInSession;
}

int Session::getPaper ( int index )
{
	if ( index < papersInSession )
	{
		// std::cout << "entered into getPaper function of Session" << '\n';
		// std::cout << index << '\n';
		// for (int i = papersInSession; i > 0; i--) {
		//   std::cout << papers[i-1] << '\n';
		// }
		// std::cout << papers[index] << '\n';
		return papers[index];
	}
	else
	{
		cout << "Index out of bound - Session::getPaper" << endl;
		exit ( 0 );
	}
}

void Session::setPaper ( int index, int paperId )
{
	if ( index < papersInSession )
	{
		papers[index] = paperId;
	}
	else
	{
		cout << "Index out of bound - Session::setPaper" << endl;
		exit ( 0 );
	}
}

void Session::printSession ( )
{
	for ( int i = 0; i < papersInSession ; i++ )
	{
		cout<<papers[i]<<" ";
	}
	cout<<endl;
}
