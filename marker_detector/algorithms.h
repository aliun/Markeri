#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

#include <vector>

using namespace std;

struct coord
{
    int x,y;
};


void binarization(vector<vector<int> >& gimage, int width, int height, int dim, int c);

void suzukiabe(vector<vector<int> >& gimage, int width, int height, int connected_case);

vector<list<coord> > douglaspeucker(vector<vector<int> >& gimage, int width, int height);

vector<vector<int> > bacimrezu(vector<vector<int> >& gimage, int minx, int miny, int maxx, int maxy, list<coord> rect);

#endif // ALGORITHMS_H_INCLUDED
