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
// #include "Util.h"

using namespace std;


double** createRandomMat(int n)
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

void createInputFile(int p, int t, int k, char* filename)
{
	ofstream ofile(filename);
	int n = p*t*k;
	double ** mat = createRandomMat(n);
	ofile << 2 << "\n";
	ofile << k<< "\n";
	ofile << p << "\n";
	ofile << t << "\n";
	ofile << 2 << "\n";
	for ( int i = 0; i < n; i++ )
	{
			for ( int j = 0; j < n; j++ )
			{
				if (j == n-1)
					ofile << mat[i][j] << "\n";
				else
					ofile << mat[i][j] << " ";
			}
	}
	ofile.close();
}

/*
 *
 */
int main ( int argc, char** argv )
{
    time_t startTime = time(NULL);
		// Parse the input.
		if ( argc < 3 )
		{
				cout<<"Missing arguments\n";
				cout<<"Correct format : \n";
				cout << "./main <input_filename> <output_filename>";
				exit ( 0 );
		}
		string inputfilename ( argv[1] );
		// char *inp = inputfilename.c_str();

		// int k = stoi(argv[3]);
		// int p = stoi(argv[4]);
		// int t = stoi(argv[5]);
		// createInputFile(p, t, k, argv[1]);

		// Initialize the conference organizer.
		SessionOrganizer *organizer  = new SessionOrganizer( inputfilename );
		// cout << "yay" << '\n';
		// Organize the papers into tracks based on similarity.
    if (organizer->processingTimeInMinutes<=0.6)
    {
      organizer->initialiseRandom ( );
    }
    else if(organizer->processingTimeInMinutes>=1.5)
    {
      organizer->initialiseSensible ( );
    }
    else
    {
      organizer->initialiseGreedySensible ( );
    }
		// organizer->initialiseRandom ( );
		// organizer->initialiseSensible ( );
		// organizer->initialiseGreedySensible ( );
		// // organizer->initialiseSensible1( );
		// cout << "yay" << '\n';
		organizer->hillClimbing (startTime, argv[2]);
		// organizer->hillClimbing ( );
		// organizer->hillClimbing ( );
		// organizer->hillClimbing ( );
		// organizer->hillClimbing ( );

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
