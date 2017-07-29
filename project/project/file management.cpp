#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <cmath>
#include <cstring>
#include <vector>
#include <fstream>


using namespace std;


void readBMP(const char* filename, int& width, int& height, vector<vector<vector<unsigned char> > >& image)
{

    unsigned char info[100];
    unsigned char* data;
    FILE* f = fopen(filename, "rb");

    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    width = *(int*)&info[18];
    height = *(int*)&info[22];
    int stride = (width*3);
    if(stride%4 != 0) stride += 4-stride%4;
    int offset = *(int*)&info[10]-54+1;
    fread(info, sizeof(unsigned char), offset, f); // move read by offset bytes
    data = (unsigned char*)malloc(stride*height*sizeof(unsigned char));
    fread(data, sizeof(unsigned char), stride*height, f);

    image.resize(width);
    for (int i=0; i<width; i++)
    {
        image.at(i).resize(height);

        for (int j=0; j<height; j++)
            image.at(i).at(j).resize(3);
    }

    for(int x = 0; x<width; x++)
        for(int y = 0; y<height; y++)
        {
            image[x][height-y-1][0] = data[y*stride+x*3+0];
            image[x][height-y-1][1] = data[y*stride+x*3+1];
            image[x][height-y-1][2] = data[y*stride+x*3+2];
        }

    fclose(f);
}

void writeBMP(const char* filename, int width, int height, const vector<vector<vector<unsigned char> > >& image)
{
    FILE *f;
    unsigned char *img = NULL;
    int filesize = 54 + 3*width*height;
    if( img )
        free( img );
    img = (unsigned char *)malloc(3*width*height);
    memset(img,0,sizeof(img));

    int x,y,r,g,b;

    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
    {
        x=i; y=(height-1)-j;
        r = image[i][j][0];
        g = image[i][j][1];
        b = image[i][j][2];

        img[(x+y*width)*3+2] = (unsigned char)(r);
        img[(x+y*width)*3+1] = (unsigned char)(g);
        img[(x+y*width)*3+0] = (unsigned char)(b);
    }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       width    );
    bmpinfoheader[ 5] = (unsigned char)(       width>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       width>>16);
    bmpinfoheader[ 7] = (unsigned char)(       width>>24);
    bmpinfoheader[ 8] = (unsigned char)(       height    );
    bmpinfoheader[ 9] = (unsigned char)(       height>> 8);
    bmpinfoheader[10] = (unsigned char)(       height>>16);
    bmpinfoheader[11] = (unsigned char)(       height>>24);

    f = fopen(filename,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for(int i=0; i<height; i++)
    {
        fwrite(img+(width*(i)*3),3,width,f);
        fwrite(bmppad,1,(4-(width*3)%4)%4,f);
    }
    fclose(f);

}
