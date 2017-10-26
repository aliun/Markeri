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
#include <string>

#include "localization.h"

using namespace std;
using namespace cv;

void ajmo_video(string lokacija, int broj_markera, vector<vector<vector<int> > > markeri_niz, Mat cameraMatrix, Mat distCoeffs)
{

	VideoCapture cap(lokacija);

	double fps = cap.get(CV_CAP_PROP_FPS);

	if (!cap.isOpened()) 
	{
			cout << "Error opening video stream or file" << endl;
			return;
	}
	
	int e1, e2;
	Mat frame;



	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH), height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	vector<vector<vector<unsigned char> > > image(width, vector<vector<unsigned char> > (height, vector<unsigned char>(3)));
	Mat undistortedimage;
	
	vector<vector<double> > results;


	int k1;
	k1 = getTickCount();
	while (1)
	{
		e1 = getTickCount();


		cap >> frame;

		if (frame.empty())
			break;

		///////////////////////////////////////////////////////////////////

		undistort(frame, undistortedimage, cameraMatrix, distCoeffs);

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					//cout << i << " " << j << " " << k << endl;
					image[i][j][k] = undistortedimage.at<Vec3b>(j, i)[k];
				}
			}
		}

		int kolikomarkera = 0;

		if (findcamera(results, kolikomarkera, image, width, height, broj_markera, markeri_niz, cameraMatrix, distCoeffs))
		{
			cout << "naso " << endl;
		}
		else
		{
			cout << "ne naso " << endl;
		}

		
		e2 = getTickCount();

		//cout << (double)(e2 - e1) / getTickFrequency();

		char c = waitKey(10);
	}

	cap.release();
	destroyAllWindows();
}