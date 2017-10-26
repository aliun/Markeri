#ifndef LOCALIZATION_H_INCLUDED
#define LOCALIZATION_H_INCLUDED


#include <vector>
#include <iostream>
#include "algorithms.h"

using namespace std;

bool findcamera(vector<vector<double> >& results, int& kolikomarkera, vector<vector<vector<unsigned char> > >& image, int width, int height, int broj_markera, const vector<vector<vector<int> > >& markeri_niz, Mat cameraMatrix, Mat distCoeffs);



#endif