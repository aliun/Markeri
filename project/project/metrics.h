#ifndef METRICS_H_INCLUDED
#define METRICS_H_INCLUDED


#include "opencv2\opencv.hpp"
#include <iostream>
#include <list>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <cmath>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;



#include "file management.h"
#include "algorithms.h"



void uradimetriku(string lokacija, int broj_slika, int broj_markera, vector<vector<vector<int> > > markeri_niz, Mat cameraMatrix, Mat distCoeffs);


#endif // METRICS_H_INCLUDED

