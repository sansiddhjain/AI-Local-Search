/*
 * File:   Util.h
 * Author: Kapil Thakkar

 */

#ifndef UTIL_H
#define	UTIL_H

/**
 * Utility Function to split string message, using give delimiter. The result is stored in result[] array.
 *
 * @param message string to be splitted
 * @param delimter delimiter used to split string
 * @param result result will be stored in this array
 * @param expcted length of the result
 */
void splitString(string message, string delimiter, string result[], int n) {
    int i = 0, pos = 0, length = 0, temp;
    temp = message.find ( delimiter.c_str ( ), pos );
    while ( temp != -1 )
    {
        length = temp - pos;
        result[i] = message.substr ( pos, length );
        pos = temp + delimiter.size ( );
        temp = message.find ( delimiter.c_str ( ), pos );
        i++;
    }
    result[i] = message.substr ( pos );
    i++;
    if ( i != n )
    {
        cout << "The similarity matrix does not have the correct format.";
        exit ( 0 );
    }
}

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

void createInputFile(int p, int t, int k, char * filename)
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
          ofile << mat[i][j] << " ";
      ofile<<"\n";
  }
  ofile.close();
}

#endif	/* UTIL_H */
