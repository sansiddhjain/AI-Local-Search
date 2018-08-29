	/*
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 *
 */

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

#include "SessionOrganizer.h"
#include "Util.h"
#include "Paper.h"
#include "Paper1.h"


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
	vector<Paper> p;
	int t = conference->getSessionsInTrack();
	int par = conference->getParallelTracks();
	int k_conf = conference->getPapersInSession();
	int n = k_conf*par*t;
	for (int i=0; i<n; i++)
	{
		for (int j=i+1; j<n; j++)
		{
			Paper paper (distanceMatrix[i][j], i, j);
			p.push_back(paper);
			// cout<<distanceMatrix[i][j] << " " <<paper.getSimilarity()<<endl;
		}

	}

	// for(int i=0; i<p.size(); i++)
	// {
	// 	cout<<p[i].getFirstPaper()<<" "<<p[i].getSecondPaper()<<" "<<p[i].getSimilarity()<<endl;
	// }

	sort(p.begin(), p.end(), [](Paper & one, Paper & two){return one.getSimilarity() > two.getSimilarity();});

	// printing the sorted vector
	// for(int i=0; i<p.size(); i++)
	// {
	// 	cout<<p[i].getFirstPaper()<<" "<<p[i].getSecondPaper()<<" "<<p[i].getSimilarity()<<endl;
	// }



	// cout<<"sorted"<<endl;
	// cout<<p.size()<<endl;
	//keep track of visited papers: paperID and track number
	vector< Paper1 > visited_paper;

	//making temp data structure for the allotment
	vector<int> schedule[t][par];

	for(int i=0; i<p.size(); i++)
	{
		//debugging
		// cout<<i<<" "<<visited_paper.size()<<endl;
		// cout<<p[i].getFirstPaper()<<" "<<p[i].getSecondPaper()<<" "<<p[i].getSimilarity()<<endl;
		// cout<<"##"<<endl;

		// if(i>10)
		// 	break;
		//insert the paper
		bool p1 = false;
		bool p2 = false;
		Paper1 paper1;
		Paper1 paper2;

		//checking if the paper have already been visited
		for(int id=0; id<visited_paper.size(); id++)
		{
			if(visited_paper[id].getPaperID() == p[i].getFirstPaper() )
			{
				p1 = true;
				paper1 = visited_paper[id];
			}
			if(visited_paper[id].getPaperID() == p[i].getSecondPaper() )
			{
				p2 = true;
				paper2 = visited_paper[id];
			}
			if(p1 && p2)
				break;
		}
		// if(find_if(visited_paper.begin(), visited_paper.end(), p[i].getFirstPaper()) != v.end())
		//     p1 = true;
		// if(find_if(visited_paper.begin(), visited_paper.end(), p[i].getSecondPaper()) != v.end())
		//     p2 = true;

		//trying some heuristic
		// if(i > 30000)
		// {
		// 	p1 = false;
		// 	p2 = false;
		// }
		if(p1 && p2)
		{
			// cout<<"continued"<<endl;
			continue;
		}
		else if (!p1 && !p2)
		{
			//insert both p1 and p2 in a new track
			//search for the element with the lowest size in the matrix
			int min = k_conf;
			int min_k = 0;
			int min_j = 0;
			bool esc = false;
			for(int k=0; k<par; k++)
			{
				for(int j=0; j<t; j++)
				{
					if(schedule[j][k].size() == 0)
					{
						schedule[j][k].push_back(p[i].getFirstPaper());
						schedule[j][k].push_back(p[i].getSecondPaper());
						esc = true;
						visited_paper.push_back( Paper1( p[i].getFirstPaper(), k, j ) );
						visited_paper.push_back( Paper1( p[i].getSecondPaper(), k, j ) );
						break;
					}
					else if (schedule[j][k].size() < min)
					{
						min = schedule[j][k].size();
						min_j = j;
						min_k = k;
					}
				}
				if(esc)
					break;
			}
			if(!esc)
			{
				if(min != (k_conf-1))
				{
					schedule[min_j][min_k].push_back(p[i].getFirstPaper());
					schedule[min_j][min_k].push_back(p[i].getSecondPaper());
					visited_paper.push_back( Paper1( p[i].getFirstPaper(), min_k, min_j ) );
					visited_paper.push_back( Paper1( p[i].getSecondPaper(), min_k, min_j ) );
				}
				else
				{
					schedule[min_j][min_k].push_back(p[i].getFirstPaper());
					visited_paper.push_back( Paper1( p[i].getFirstPaper(), min_k, min_j ) );

					bool esc1 = false;
					for(int k=0; k<par; k++)
					{
						for(int j=0; j<t; j++)
						{
							if(schedule[j][k].size()<k_conf)
							{
								schedule[j][k].push_back(p[i].getSecondPaper());
								visited_paper.push_back( Paper1( p[i].getSecondPaper(), k, j ) );
								esc1 = true;
								break;
							}
						}
						if(esc1)
							break;
					}
				}
			}

		}
		else if(!p1)
		{
			//insert p1
			if(schedule[paper2.getSession()][paper2.getTrack()].size() < k_conf)
			{
				schedule[paper2.getSession()][paper2.getTrack()].push_back(p[i].getFirstPaper());
				visited_paper.push_back( Paper1( p[i].getFirstPaper(), paper2.getTrack(), paper2.getSession() ) );
			}
			else
			{
				int min = k_conf;
				int min_k = 0;
				int min_j = 0;
				bool esc = false;
				for(int k=0; k<par; k++)
				{
					for(int j=0; j<t; j++)
					{
						if(schedule[j][k].size() == 0)
						{
							schedule[j][k].push_back(p[i].getFirstPaper());
							esc = true;
							visited_paper.push_back( Paper1( p[i].getFirstPaper(), k, j ) );
							break;
						}
						else if (schedule[j][k].size() < min)
						{
							min = schedule[j][k].size();
							min_j = j;
							min_k = k;
						}
					}
					if(esc)
						break;
				}
				if(!esc)
				{
					schedule[min_j][min_k].push_back(p[i].getFirstPaper());
					visited_paper.push_back( Paper1( p[i].getFirstPaper(), min_k, min_j ) );
				}
			}
		}
		else if(!p2)
		{
			//insert p2
			if(schedule[paper1.getSession()][paper1.getTrack()].size() < k_conf)
			{
				schedule[paper1.getSession()][paper1.getTrack()].push_back(p[i].getFirstPaper());
				visited_paper.push_back( Paper1( p[i].getFirstPaper(), paper1.getTrack(), paper1.getSession() ) );
			}
			else
			{
				int min = k_conf;
				int min_k = 0;
				int min_j = 0;
				bool esc = false;
				for(int k=0; k<par; k++)
				{
					for(int j=0; j<t; j++)
					{
						if(schedule[j][k].size() == 0)
						{
							schedule[j][k].push_back(p[i].getSecondPaper());
							esc = true;
							visited_paper.push_back( Paper1( p[i].getSecondPaper(), k, j ) );
							break;
						}
						else if (schedule[j][k].size() < min)
						{
							min = schedule[j][k].size();
							min_j = j;
							min_k = k;
						}
					}
					if(esc)
						break;
				}
				if(!esc)
				{
					schedule[min_j][min_k].push_back(p[i].getSecondPaper());
					visited_paper.push_back( Paper1( p[i].getSecondPaper(), min_k, min_j ) );
				}
			}
		}
	}

	//entering the schedule in the main datastructure
	vector<int> inserted_paper;
	vector<int> domain;

	for(int i=0; i<n; i++)
	{
		domain.push_back(i);
	}

	//removing redundant paper and adding if less
	for(int i=0; i<t; i++)
	{
		for(int j=0; j<par; j++)
		{
			vector<int> temp_schedule = schedule[i][j];
			for(int k=0; k<temp_schedule.size(); k++)
			{
				if(find(inserted_paper.begin(), inserted_paper.end(), temp_schedule[k]) != inserted_paper.end() )
				{
					int rem = temp_schedule[k];
					temp_schedule.erase(remove(temp_schedule.begin(), temp_schedule.end(), rem), temp_schedule.end());
					k--;
				}
				else
				{
					int rem = temp_schedule[k];
					domain.erase(remove(domain.begin(), domain.end(), rem), domain.end());
				}
			}
			if(temp_schedule.size() < k_conf)
			{
				while(temp_schedule.size() < k_conf)
				{
					int add_element = domain[0];
					domain.erase(domain.begin());
					temp_schedule.push_back(add_element);
				}
			}
			else if(temp_schedule.size() > k_conf)
			{
				while(temp_schedule.size() > k_conf)
				{
					domain.push_back(temp_schedule[temp_schedule.size()-1]);
					temp_schedule.erase(temp_schedule.end()-1);
				}
			}
			schedule[i][j] = temp_schedule;
		}
	}

	for(int i=0; i<t; i++)
	{
		for(int j=0; j<par; j++)
		{
			vector<int> temp_schedule = schedule[i][j];
			// cout<<temp_schedule.size()<<endl;
			for(int k=0; k<temp_schedule.size(); k++)
			{
				inserted_paper.push_back(temp_schedule[k]);
				conference->setPaper ( j, i, k, temp_schedule[k] );
			}
		}
	}



	// cout<<inserted_paper.size()<<endl;
	// sort(inserted_paper.begin(), inserted_paper.end());
	// for(int i=0; i<inserted_paper.size(); i++)
	// {
	// 	cout<<"#"<<inserted_paper[i]<<endl;
	// }
}

void SessionOrganizer::hillClimbing (time_t startTime, char* output_filename )
{
  time_t currTime = time(NULL);
  double elapsedTime = difftime(currTime, startTime);
  while (elapsedTime < processingTimeInMinutes*60)
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

    while ((!localOptima) & (elapsedTime < processingTimeInMinutes*60))
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
      printSessionOrganiser( output_filename);
      currTime = time(NULL);
      elapsedTime = difftime(currTime, startTime);
    }
  }
}

void SessionOrganizer::hillClimbing1 (time_t startTime, char* output_filename )
{
  time_t currTime = time(NULL);
  double elapsedTime = difftime(currTime, startTime);
  while (elapsedTime < processingTimeInMinutes*60)
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

    while ((!localOptima) & (elapsedTime < processingTimeInMinutes*60))
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
        localOptima = true;
      else
        conference->swapPapers(p1_p, p1_t, p1_k, p_opt, t_opt, k_opt);
      loopCounter++;
      cout << loopCounter << '\n';
      cout << scoreOrganization() << '\n';
      if (loopCounter % 5 == 0)
        printSessionOrganiser( output_filename);
      currTime = time(NULL);
      elapsedTime = difftime(currTime, startTime);
    }
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
