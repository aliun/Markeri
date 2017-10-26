#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

#include <vector>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

struct coord
{
    int x,y;
};


Vec3f rotationMatrixToEulerAngles(Mat &R);

void binarization(vector<vector<int> >& gimage, int width, int height, int dim, int c);

void suzukiabe(vector<vector<int> >& gimage, int width, int height, int connected_case);

vector<list<coord> > douglaspeucker(vector<vector<int> >& gimage, int width, int height, double eps);

vector<vector<int> > bacimrezu(vector<vector<int> >& gimage, int minx, int miny, int maxx, int maxy, list<coord> rect);

//vector<vector<double> > uradi_matu(double aa, double bb, double cosuv, double cosuw, double cosvw, double ab, double bc, double ac);


#endif // ALGORITHMS_H_INCLUDED
