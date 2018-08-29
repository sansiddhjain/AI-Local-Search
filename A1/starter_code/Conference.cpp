/*
 * File:   Conference.cpp
 * Author: Kapil Thakkar
 *
 */

#include "Conference.h"

Conference::Conference ( )
{
    this->parallelTracks = 0;
    this->sessionsInTrack = 0;
    this->papersInSession = 0;
}

Conference::Conference ( int parallelTracks, int sessionsInTrack, int papersInSession )
{
    this->parallelTracks = parallelTracks;
    this->sessionsInTrack = sessionsInTrack;
    this->papersInSession = papersInSession;
    initTracks ( parallelTracks, sessionsInTrack, papersInSession );
}

void Conference::initTracks ( int parallelTracks, int sessionsInTrack, int papersInSession )
{
    tracks = ( Track * ) malloc ( sizeof (Track ) * parallelTracks );
    for ( int i = 0; i < parallelTracks; i++ )
    {
        Track tempTrack ( sessionsInTrack );
        for ( int j = 0; j < sessionsInTrack; j++ )
        {
            Session tempSession ( papersInSession );
            tempTrack.setSession ( j, tempSession );
        }
        tracks[i] = tempTrack;
    }
}

int Conference::getParallelTracks ( )
{
    return parallelTracks;
}

int Conference::getSessionsInTrack ( )
{
    return sessionsInTrack;
}

int Conference::getPapersInSession ( )
{
    return papersInSession;
}

int Conference::getPaper(int p, int t, int k)
{
  if (p >= parallelTracks) {
    // cout << "error - p > parallelTracks" << '\n';
  }
  Track tr = tracks[p];
  // cout << "got track" << '\n';
  return tr.getPaper(t, k);
  // cout << "post return statement" << '\n';
}

void Conference::swapPapers(int p1, int t1, int k1, int p2, int t2, int k2)
{
  int paper1 = getPaper(p1, t1, k1);
  // cout << "got paper 1" << '\n';
  int paper2 = getPaper(p2, t2, k2);
  // cout << "got paper 2" << '\n';
  setPaper(p1, t1, k1, paper2);
  // cout << "setting paper 2 to paper 1's slot" << '\n';
  setPaper(p2, t2, k2, paper1);
  // cout << "setting paper 1 to paper 2's slot" << '\n';
}

Track Conference::getTrack ( int index )
{
    if ( index < parallelTracks )
    {
        return tracks[index];
    }
    else
    {
        cout << "Index out of bound - Conference::getTrack" << endl;
        exit ( 0 );
    }
}

void Conference::setPaper ( int trackIndex, int sessionIndex, int paperIndex, int paperId )
{
    if ( this->parallelTracks > trackIndex )
    {
        Track curTrack = tracks[trackIndex];
        curTrack.setPaper ( sessionIndex, paperIndex, paperId );
    }
    else
    {
        cout << "Index out of bound - Conference::setPaper" << endl;
        exit ( 0 );
    }
}

void Conference::printConference (char * filename )
{
    ofstream ofile(filename);

    for ( int i = 0; i < sessionsInTrack; i++ )
    {
        for ( int j = 0; j < parallelTracks; j++ )
        {
            for ( int k = 0; k < papersInSession; k++ )
            {
                ofile<< tracks[j].getSession ( i ).getPaper ( k ) << " ";
            }
            if ( j != parallelTracks - 1 )
            {
                ofile<<"| ";
            }
        }
        ofile<<"\n";
    }
    ofile.close();
    cout<<"Organization written to ";
    printf("%s :)\n",filename);

}
