#ifndef FILE_MANAGEMENT_H_INCLUDED
#define FILE_MANAGEMENT_H_INCLUDED

using namespace std;


void readBMP(const char* filename, int& width, int& height, vector<vector<vector<unsigned char> > >& image);

void writeBMP(const char* filename, int width, int height, const vector<vector<vector<unsigned char> > >& image);

#endif // FILE_MANAGEMENT_H_INCLUDED
