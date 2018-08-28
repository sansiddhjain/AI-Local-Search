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
#include "Util.h"

using namespace std;


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
    string inputfilename ( argv[1] );

    int k = stoi(argv[3]);
    int p = stoi(argv[4]);
    int t = stoi(argv[5]);
    createInputFile(p, t, k, inputfilename);

    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer( inputfilename );

    // Organize the papers into tracks based on similarity.
    organizer->initialiseRandom ( );
    organizer->hillClimbing ( );

    organizer->printSessionOrganiser ( argv[2]);

    // Score the organization against the gold standard.
    double score = organizer->scoreOrganization ( );
    cout<< "score:"<<score<<endl;

    // Graph *graph  = new Graph(n);
    // graph->createGraph(mat, 0.1);
    // vector< vector<int> > SCCs;
    // graph->calculateSCC(SCCs);
    // vector< vector<int> >::iterator i;
    // vector<int>::iterator j;
    // for (i = SCCs.begin(); i != SCCs.end(); i++)
    // {
    //   for (j = i->begin(); j != i->end(); j++)
    //     cout << *j << ' ';
    //   cout << "\n";
    // }

    return 0;
}
