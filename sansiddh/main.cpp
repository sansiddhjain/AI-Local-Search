/*
 * File:   main.cpp
 * Author: Kapil Thakkar
 *
 */

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>

#include "SessionOrganizer.h"
#include "Graph.h"

using namespace std;

double** genRandomMat(int n)
{
  srand(time(NULL));
  double** mat = new double*[n];
  for ( int i = 0; i < n; ++i )
    mat[i] = new double[n];

  for (int i = 0; i < n; i++)
  {
    for (int j = i; j < n; j++)
    {
      if (i == j)
        mat[i][j] = 0;
      else
      {
        mat[i][j] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        mat[j][i] = mat[i][j];
      }
    }
  }
  return mat;
}

/*
 *
 */
int main ( int argc, char** argv )
{
    // Parse the input.
    if ( argc < 3 )
    {
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    // string inputfilename ( argv[1] );
    //
    // // Initialize the conference organizer.
    // SessionOrganizer *organizer  = new SessionOrganizer( inputfilename );
    //
    // // Organize the papers into tracks based on similarity.
    // organizer->organizePapers ( );
    //
    // organizer->printSessionOrganiser ( argv[2]);
    //
    // // Score the organization against the gold standard.
    // double score = organizer->scoreOrganization ( );
    // cout<< "score:"<<score<<endl;

    int k = stoi(argv[3]);
    int p = stoi(argv[4]);
    int t = stoi(argv[5]);
    int n = k*p*t;

    double** mat = getRandomMat(n);
    organizer->distanceMatrix = mat;
    organizer->initialise(mat);
    organizer->

    Graph *graph  = new Graph(n);
    graph->createGraph(mat, 0.1);
    vector< vector<int> > SCCs;
    graph->calculateSCC(SCCs);
    vector< vector<int> >::iterator i;
    vector<int>::iterator j;
    for (i = SCCs.begin(); i != SCCs.end(); i++)
    {
      for (j = i->begin(); j != i->end(); j++)
        cout << *j << ' ';
      cout << "\n";
    }

    return 0;
}
