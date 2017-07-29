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
#include <sstream>

using namespace cv;
using namespace std;


#include "file management.h"
#include "algorithms.h"


int width, height, dim, kvad, maks, minim, localtres;
vector<vector<vector<unsigned char> > > image;
vector<vector<int> > gimage, gimage2;

const char *location = "F:\\Projects\\Unfinished\\markeri\\Markeri\\markeri\\markerilabelirano\\5brt2.bmp";
const char *locationn = "F:\\Projects\\Unfinished\\markeri\\Markeri\\img.bmp";

//const char *location = "E:\\bmpmarkweri\\s.bmp";
//const char *locationn = "E:\\bmpmarkweri\\img.bmp";



int main(int argv, char** argc)
{
	ifstream markeri;
	int broj_markera = 8;

	ifstream matricakamere("F:\\Projects\\Unfinished\\markeri\\Markeri\\matcal.txt");
	ifstream distkoef("F:\\Projects\\Unfinished\\markeri\\Markeri\\cofdis.txt");

	Mat cameraMatrix(3, 3, DataType<double>::type);
	setIdentity(cameraMatrix);
	for (int i=0; i<3; i++)
		for (int j = 0; j < 3; j++)
		{
			matricakamere >> cameraMatrix.at<double>(i,j);
		}



	Mat distCoeffs(5, 1, DataType<double>::type);
	for (int i=0; i<5; i++) distkoef >> distCoeffs.at<double>(i);
	
	readBMP(location, width, height, image);

	Mat imagemat(width, height, CV_8UC3);


	Mat undistortedimage;


	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			{
				imagemat.at<Vec3b>(i, j)[0] = image[i][j][0];
				imagemat.at<Vec3b>(i, j)[1] = image[i][j][1];
				imagemat.at<Vec3b>(i, j)[2] = image[i][j][2];
			}
		

	undistort(imagemat, undistortedimage, cameraMatrix, distCoeffs);


	for (int i = 0; i<width; i++)
		for (int j = 0; j <height; j++)
			for (int k = 0; k<3; k++)
				image[i][j][k] = undistortedimage.at<Vec3b>(i, j)[k];
		


	gimage.resize(width);
	gimage2.resize(width);
	for (int x = 0; x<width; x++)
	{
		gimage[x].resize(height);
		gimage2[x].resize(height);
	}

	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
		{
			gimage[x][y] = (image[x][y][0] + image[x][y][1] + image[x][y][2]) / 3;
			// image[x][y][0]=gimage[x][y]; image[x][y][1]=gimage[x][y]; image[x][y][2]=gimage[x][y];
		}
	}



	binarization(gimage, width, height, 40, 45);


	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
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

	int trashhold = 10;


	int min_index;

	bool indikator;
	for (int i = 0; i<rects.size(); i++)
	{
		indikator = true;
		for (int j = 0; j<4; j++)
		{

			koord = rects[i].front();

			if (sqrt((rects[i].front().x - rects[i].back().x)*(rects[i].front().x - rects[i].back().x) +
				(rects[i].front().y - rects[i].back().y)*(rects[i].front().y - rects[i].back().y))<trashhold)
			{
				indikator = false;
				break;
			}

			gimage2[koord.x][koord.y] = -1;

			rects[i].pop_front();
			rects[i].push_back(koord);

		}
		if (!indikator) continue;

		minx = width; miny = height; maxx = -1; maxy = -1;
		for (int k = 0; k<4; k++)
		{
			koord = rects[i].front();



			if (minx>koord.x) minx = koord.x;
			if (miny>koord.y) miny = koord.y;
			if (maxx<koord.x) maxx = koord.x;
			if (maxy<koord.y) maxy = koord.y;


			rects[i].pop_front();
			rects[i].push_back(koord);

			//gimage2[koord.x][koord.y]=-1;
		}


		crpimage.resize(maxx - minx + 1);
		for (int x = 0; x<maxx - minx + 1; x++)
		{
			crpimage.at(x).resize(maxy - miny + 1);



			for (int y = 0; y<maxy - miny + 1; y++)
			{
				crpimage.at(x).at(y).resize(3);
				crpimage[x][y][0] = crpimage[x][y][1] = crpimage[x][y][2] = gimage2[minx + x][miny + y] * 255;
			}
		}

		vector<vector<int> > marker;//(8, vector<int> (8));

		marker = bacimrezu(gimage2, minx, miny, maxx, maxy, rects[i]);

		int rast_min = 64;

		for (int k = 0; k<broj_markera; k++)
		{
			int rast = 0;
			int bit;

			stringstream ss;
			ss << "F:\\Projects\\Unfinished\\markeri\\Markeri\\kodovi\\txt\\" << k << ".txt";
			

			markeri.open(ss.str());

			ss.clear();
			ss.str(string());



			for (int i = 0; i<8; i++)
			{
				for (int j = 0; j<8; j++)
				{
					markeri >> bit;
					if (bit != marker[j][i]) rast++;
					//cout <<marker[j][i] <<" ";
					//cout << bit << " ";
				}

				//cout <<endl;
			}

			//cout <<endl <<rast;

			if (rast <= rast_min)
			{
				rast_min = rast;
				min_index = k;

				//cout <<" " <<rast_min;
			}

			markeri.close();
		}

		if (rast_min < 5)
		{

			vector<Point2f> imagePoints(4);
			vector<Point3f> objectPoints(4);

			//prepisi image points

			for (int j = 0; j < 4; j++)
			{
				koord = rects[i].front();

				imagePoints[j].x = koord.x;
				imagePoints[j].y = koord.y;

				rects[i].pop_front();
				rects[i].push_back(koord);
			}


			/*
			koord = rects[i].front();

			imagePoints[1][0].x = koord.x;
			imagePoints[1][0].y = koord.y;

			imagePoints[2][0].x = koord.x;
			imagePoints[2][0].y = koord.y;

			imagePoints[3][0].x = koord.x;
			imagePoints[3][0].y = koord.y;

			rects[i].pop_front();
			rects[i].push_back(koord);

			koord = rects[i].front();

			imagePoints[1][1].x = koord.x;
			imagePoints[1][1].y = koord.y;

			imagePoints[2][1].x = koord.x;
			imagePoints[2][1].y = koord.y;

			imagePoints[4][0].x = koord.x;
			imagePoints[4][0].y = koord.y;

			rects[i].pop_front();
			rects[i].push_back(koord);

			koord = rects[i].front();

			imagePoints[1][2].x = koord.x;
			imagePoints[1][2].y = koord.y;

			imagePoints[3][1].x = koord.x;
			imagePoints[3][1].y = koord.y;

			imagePoints[4][1].x = koord.x;
			imagePoints[4][1].y = koord.y;

			rects[i].pop_front();
			rects[i].push_back(koord);

			koord = rects[i].front();

			imagePoints[4][2].x = koord.x;
			imagePoints[4][0].y = koord.y;

			imagePoints[2][2].x = koord.x;
			imagePoints[2][2].y = koord.y;

			imagePoints[3][2].x = koord.x;
			imagePoints[3][2].y = koord.y;

			rects[i].pop_front();
			rects[i].push_back(koord);
			*/

			//ucitaj object points


			stringstream ss;
			ss << "F:\\Projects\\Unfinished\\markeri\\Markeri\\kodovi\\txt\\" << min_index << min_index << ".txt";

			ifstream recnik(ss.str());

			ss.clear();
			ss.str(string());

			double x1, x2, x3;

			for (int k = 0; k < 4; k++)
			{
				recnik >> x1 >> x2 >> x3;

				objectPoints[k].x = x1;
				objectPoints[k].y = x2;
				objectPoints[k].z = x3;

			}

			/*
			objectPoints[1][0].x = points[0].x;
			objectPoints[1][0].y = points[0].y;

			objectPoints[2][0].x = points[0].x;
			objectPoints[2][0].y = points[0].y;

			objectPoints[3][0].x = points[0].x;
			objectPoints[3][0].y = points[0].y;


			objectPoints[1][1].x = points[1].x;
			objectPoints[1][1].y = points[1].y;

			objectPoints[2][1].x = points[1].x;
			objectPoints[2][1].y = points[1].y;

			objectPoints[4][0].x = points[1].x;
			objectPoints[4][0].y = points[1].y;


			objectPoints[1][2].x = points[2].x;
			objectPoints[1][2].y = points[2].y;

			objectPoints[3][1].x = points[2].x;
			objectPoints[3][1].y = points[2].y;

			objectPoints[4][1].x = points[2].x;
			objectPoints[4][1].y = points[2].y;


			objectPoints[4][2].x = points[3].x;
			objectPoints[4][0].y = points[3].y;

			objectPoints[2][2].x = points[3].x;
			objectPoints[2][2].y = points[3].y;

			objectPoints[3][2].x = points[3].x;
			objectPoints[3][2].y = points[3].y;
			*/

			Mat rvec(3, 1, DataType<double>::type);
			Mat tvec(3, 1, DataType<double>::type);

			Mat dstCoeffs(5, 1, DataType<double>::type);
			for (int i = 0; i < 5; i++) dstCoeffs.at<double>(i) = 0;

			if (solvePnP(objectPoints, imagePoints, cameraMatrix, dstCoeffs, rvec, tvec))
			{
				cout << rvec << endl;
				cout << tvec << endl;
			}
			else cout << "PnP fejlovao";
		}

	}





	/*
	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
		{
			//if ((gimage[x][y]!=1)&&(gimage[x][y]!=0)) {image[x][y][0]=255; image[x][y][1]=255; image[x][y][2]=255;}
			//else {image[x][y][0]=0; image[x][y][1]=0; image[x][y][2]=0;}
			if (gimage2[x][y] == -1) { image[x][y][0] = 0; image[x][y][1] = 255; image[x][y][2] = 0; }
			//else if (gimage[x][y]>1) {image[x][y][0]=255; image[x][y][1]=255; image[x][y][2]=255;}
			//image[x][y][0]=gimage2[x][y]; image[x][y][1]=gimage2[x][y]; image[x][y][2]=gimage2[x][y];

		}
	}


	writeBMP(locationn, width, height, image);

	*/




	waitKey();
}