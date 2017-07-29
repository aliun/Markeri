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
#include <sstream>

#include "file management.h"
#include "algorithms.h"

using namespace std;


int width, height, dim, kvad, maks, minim, localtres ;
vector<vector<vector<unsigned char> > > image;
vector<vector<int> > gimage, gimage2;

const char *location = "F:\\Projects\\Unfinished\\markeri\\Markeri\\markeri\\markerilabelirano\\5brt1.bmp";
const char *locationn = "F:\\Projects\\Unfinished\\markeri\\Markeri\\img.bmp" ;

//const char *location = "E:\\bmpmarkweri\\s.bmp";
//const char *locationn = "E:\\bmpmarkweri\\img.bmp";


int main()
{
    ifstream markeri;
    int broj_markera = 8;

    readBMP(location, width, height, image);

    gimage.resize(width);
    gimage2.resize(width);
    for(int x=0;x<width;x++)
    {
        gimage[x].resize(height);
        gimage2[x].resize(height);
    }

    for(int x=0;x<width;x++)
    {
       for(int y=0;y<height;y++)
       {
           gimage[x][y] = (image[x][y][0]+ image[x][y][1] + image[x][y][2])/3;
           // image[x][y][0]=gimage[x][y]; image[x][y][1]=gimage[x][y]; image[x][y][2]=gimage[x][y];
       }
    }



    binarization(gimage, width, height, 40, 45);


    for(int x=0;x<width;x++)
    {
       for(int y=0;y<height;y++)
       {

           gimage2[x][y] = gimage[x][y];
           // image[x][y][0]=gimage[x][y]; image[x][y][1]=gimage[x][y]; image[x][y][2]=gimage[x][y];
       }
    }

    //sinteticki podaci

    /*
    int a;

    width = 13;
    height = 8;


    for (int j=0; j<height; j++)
        for (int i=0; i<width; i++)

            {
                cin >> a;

                gimage[i][j]= (unsigned char) a;
            }

    */


    suzukiabe(gimage, width, height, 8);

    vector<list<coord> > rects;
    rects = douglaspeucker(gimage, width, height);


    vector<vector<vector<unsigned char> > > crpimage;


    int minx, miny, maxx, maxy;

    coord koord;

    int trashhold=10;


    int min_index;

    bool indikator;
    for (int i=0; i<rects.size(); i++)
    {
        indikator=true;
        for (int j=0; j<4; j++)
        {

            koord=rects[i].front();

            if (sqrt((rects[i].front().x-rects[i].back().x)*(rects[i].front().x-rects[i].back().x)+
                     (rects[i].front().y-rects[i].back().y)*(rects[i].front().y-rects[i].back().y))<trashhold)
            {
                indikator=false;
                break;
            }

            gimage2[koord.x][koord.y]=-1;

            rects[i].pop_front();
            rects[i].push_back(koord);

        }
        if (!indikator) continue;

        minx=width; miny=height; maxx=-1; maxy=-1;
        for (int k=0; k<4; k++)
                    {
                        koord=rects[i].front();



                        if (minx>koord.x) minx=koord.x;
                        if (miny>koord.y) miny=koord.y;
                        if (maxx<koord.x) maxx=koord.x;
                        if (maxy<koord.y) maxy=koord.y;


                        rects[i].pop_front();
                        rects[i].push_back(koord);

                        //gimage2[koord.x][koord.y]=-1;
                    }


        crpimage.resize(maxx-minx+1);
        for (int x=0; x<maxx-minx+1; x++)
        {
            crpimage.at(x).resize(maxy-miny+1);



            for (int y=0; y<maxy-miny+1; y++)
            {
                crpimage.at(x).at(y).resize(3);
                crpimage[x][y][0]=crpimage[x][y][1]=crpimage[x][y][2]=gimage2[minx+x][miny+y]*255;
            }
        }

        vector<vector<int> > marker;//(8, vector<int> (8));

        marker = bacimrezu(gimage2, minx, miny, maxx, maxy, rects[i]);

        int rast_min=64;

        for (int k=0; k<broj_markera; k++)
        {
            int rast=0;
            int bit;

            stringstream ss;
            ss <<"F:\\Projects\\Unfinished\\markeri\\Markeri\\kodovi\\txt\\" <<k <<".txt";
            const char* str = ss.str().c_str();


            ss.clear();
            ss.str(string());

            markeri.open (str);

            for (int i=0; i<8; i++)
            {
                for (int j=0; j<8; j++)
                {
                    markeri >>bit;
                    if (bit!=marker[j][i]) rast++;
                    //cout <<marker[j][i] <<" ";
                }

                //cout <<endl;
            }

            //cout <<endl <<rast;

            if (rast<=rast_min)
            {
                rast_min=rast;
                min_index=k;

                //cout <<" " <<rast_min;
            }

            markeri.close();
        }

        //cout <<endl;

        if (rast_min<10)
        cout <<min_index <<endl;

        //////////////////////////////////////////////////////////////

        vector<vector<double> > dist;

        //double a=(bc/ab), b=(ac/ab);
        dist = uradi_matu(1, 1, 0.995, 0.995, 0.995, 1, 1, 1);//(a,b,cosuv,cosuw,cosvw,ab,bc,ac);





    }







    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            //if ((gimage[x][y]!=1)&&(gimage[x][y]!=0)) {image[x][y][0]=255; image[x][y][1]=255; image[x][y][2]=255;}
            //else {image[x][y][0]=0; image[x][y][1]=0; image[x][y][2]=0;}
            if (gimage2[x][y]==-1) {image[x][y][0]=0; image[x][y][1]=255; image[x][y][2]=0;}
            //else if (gimage[x][y]>1) {image[x][y][0]=255; image[x][y][1]=255; image[x][y][2]=255;}
            //image[x][y][0]=gimage2[x][y]; image[x][y][1]=gimage2[x][y]; image[x][y][2]=gimage2[x][y];

        }
    }


    writeBMP(locationn, width, height, image);

    return 0;
}
