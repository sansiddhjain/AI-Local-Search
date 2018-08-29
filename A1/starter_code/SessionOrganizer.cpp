/*
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 *
 */

#include <cstdlib>
#include <ctime>

#include "SessionOrganizer.h"
#include "Util.h"

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
}

void SessionOrganizer::initialiseRandom ( )
{
    int paperCounter = 0;
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                conference->setPaper ( j, i, k, paperCounter );
                paperCounter++;
            }
        }
    }
}

void SessionOrganizer::initialiseSensible ( )
{

}

void SessionOrganizer::hillClimbing ( )
{
    int t = conference->getSessionsInTrack();
    int p = conference->getParallelTracks();
    int k = conference->getPapersInSession();
    int n = k*p*t;

    srand(time(NULL));

    bool localOptima = false;
    int loopCounter = 0;
    int p1_p, p1_t, p1_k;
    double score, maxScore;
    int p_opt, t_opt, k_opt;

    while (!localOptima)
    {
      p1_p = rand() % p;
      p1_t = rand() % t;
      p1_k = rand() % k;
      maxScore = scoreOrganization();
      // cout << "loop bp" << '\n';
      if (loopCounter % 2 == 0)
      {
        // cout << "if" << '\n';
        for (int p2_p = 0; p2_p < p; p2_p++)
        {
          if (p2_p != p1_p)
          {
            for (int p2_k = 0; p2_k < k; p2_k++)
            {
              conference->swapPapers(p1_p, p1_t, p1_k, p2_p, p1_t, p2_k);
              score = scoreOrganization();
              conference->swapPapers(p1_p, p1_t, p1_k, p2_p, p1_t, p2_k);
              if (score > maxScore)
              {
                maxScore = score;
                p_opt = p2_p; t_opt = p1_t; k_opt = p2_k;
              }
            }
          }
        }
      }
      else
      {
        // cout << "else" << '\n';
        for (int p2_t = 0; p2_t < p; p2_t++)
        {
          // cout << "outer for" << '\n';
          if (p2_t != p1_t)
          {
            for (int p2_k = 0; p2_k < k; p2_k++)
            {
              // cout << "inner for" << '\n';
              conference->swapPapers(p1_p, p1_t, p1_k, p1_p, p2_t, p2_k);
              // cout << "first swap" << '\n';
              score = scoreOrganization();
              // cout << "scoring" << '\n';
              conference->swapPapers(p1_p, p1_t, p1_k, p1_p, p2_t, p2_k);
              // cout << "after swapping papers" << '\n';
              if (score > maxScore)
              {
                maxScore = score;
                p_opt = p1_p; t_opt = p2_t; k_opt = p2_k;
              }
            }
          }
        }
      }
      if (maxScore == scoreOrganization())
      {
        for (int p2_p = 0; p2_p < p; p2_p++)
        {
          if (p2_p != p1_p)
          {
            for (int p2_t = 0; p2_t < t; p2_t++)
            {
              if (p2_t != p1_t)
              {
                for (int p2_k = 0; p2_k < k; p2_k++)
                {
                  conference->swapPapers(p1_p, p1_t, p1_k, p2_p, p2_t, p2_k);
                  score = scoreOrganization();
                  conference->swapPapers(p1_p, p1_t, p1_k, p2_p, p2_t, p2_k);
                  if (score > maxScore)
                  {
                    maxScore = score;
                    p_opt = p2_p; t_opt = p2_t; k_opt = p2_k;
                  }
                }
              }
            }
          }
        }
        if (maxScore == scoreOrganization())
          localOptima = true;
        else
          conference->swapPapers(p1_p, p1_t, p1_k, p_opt, t_opt, k_opt);
      }
      else
        conference->swapPapers(p1_p, p1_t, p1_k, p_opt, t_opt, k_opt);
      loopCounter++;
      cout << loopCounter << '\n';
      cout << scoreOrganization() << '\n';
    }
}


void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            //cout<<"Line read:"<<line<<endl;
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInMinutes = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string elements[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( char * filename)
{
    conference->printConference ( filename);
}

double SessionOrganizer::scoreOrganization ( )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}
