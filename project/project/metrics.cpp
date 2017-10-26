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
#include "localization.h"
#include "metrics.h"

void uradimetriku(string lokacija, int broj_slika, int broj_markera, vector<vector<vector<int> > > markeri_niz, Mat cameraMatrix, Mat distCoeffs)
{

	int width, height;
	vector<vector<vector<unsigned char> > > image;
	


	int kolikomarkera = 0;
	double sum1 = 0, sum2 = 0, sum3 = 0;
	int koliko1 = 0, koliko2 = 0, koliko3 = 0;
	double maxvar = 0, minvar = INT32_MAX, indmax;


	ofstream varijanset(lokacija + "varijanset.txt");
	ofstream varijanser(lokacija + "varijanser.txt");
	ofstream varijansete(lokacija + "varijansete.txt");
	ofstream varijansere(lokacija + "varijansere.txt");

	for (int br = 1; br <= broj_slika; br++)
	{

		cout << endl << " Slika broj " << br << ": " << endl;

		stringstream lokacijaa;
		lokacijaa << lokacija << br << ".bmp";


		readBMP(lokacijaa.str().c_str(), width, height, image);

		lokacijaa.clear();
		lokacijaa.str(string());


		Mat imagemat(width, height, CV_8UC3);


		Mat undistortedimage;


		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				imagemat.at<Vec3b>(i, j)[0] = image[i][j][0];
				imagemat.at<Vec3b>(i, j)[1] = image[i][j][1];
				imagemat.at<Vec3b>(i, j)[2] = image[i][j][2];
			}



		//writeBMP(locationn, width, height, image);


		undistort(imagemat, undistortedimage, cameraMatrix, distCoeffs);


		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
				for (int k = 0; k < 3; k++)
					image[i][j][k] = undistortedimage.at<Vec3b>(i, j)[k];

		vector<vector<double> > results;
		if (findcamera(results, kolikomarkera, image, width, height, broj_markera, markeri_niz, cameraMatrix, distCoeffs))
		{
			stringstream ss;
			ss << lokacija << br << ".txt";

			ifstream gt(ss.str());

			ss.clear();
			ss.str(string());

			double x, y, z;
			double anglex, angley, anglez;

			gt >> x >> y >> z;

			double micic_angle;
			gt >> micic_angle;

			anglex = -90+360;
			angley = 0;
			anglez = -90 + micic_angle;
			if (anglez < 0) anglez += 360;


			cout << " RESULTS:" << endl << "(" << results[0][0] << ", " << results[0][1] << ", " << results[0][2] << ")"
				<< endl << "(" << results[1][0] << ", " << results[1][1] << ", " << results[1][2] << ")" << endl;

			cout << " GT:" << endl << "(" << x << ", " << y << ", " << z << ")"
				<< endl << "(" << anglex << ", " << angley << ", " << anglez << ")" << endl;

			gt.close();

			
			varijanset << sqrt(pow(x - 43.2, 2) + pow(y-100, 2) + pow(z - 11.9, 2)) << '\t';
			varijanset << x << '\t' << y << '\t' << abs(x - results[0][0]) << '\t'
				<< abs(y - results[0][1]) << '\t'
				<< abs(z - results[0][2]) << endl;

			double varx, vary, varz;

			if (abs(anglex - results[1][0])<180) varx = abs(anglex - results[1][0]);
			else varx = 360 - abs(anglex - results[1][0]);

			if (abs(angley - results[1][1])<180) vary = abs(angley - results[1][1]);
			else vary = 360 - abs(angley - results[1][1]);

			if (abs(anglez - results[1][2])<180) varz = abs(anglez - results[1][2]);
			else varz = 360 - abs(anglez - results[1][2]);


			varijanser << x << '\t' << y << '\t' << varx << '\t' << vary << '\t' << varz << endl;


			varijansete << x << '\t' << y << '\t' << sqrt(pow((x - results[0][0]), 2) + pow((y - results[0][1]), 2) + pow((z - results[0][2]), 2)) << endl;

			varijansere << x << '\t' << y << '\t' << sqrt(pow(varx, 2) + pow(vary, 2) + pow(varz, 2)) << endl;



			if (kolikomarkera == 1)
			{
				sum1 += sqrt(varx*varx + vary*vary + varz*varz);//sqrt(pow((x - res.at<double>(0, 0)), 2) + pow((y - res.at<double>(1, 0)), 2) + pow((z - res.at<double>(2, 0)), 2));
				koliko1++;
			}
			if (kolikomarkera == 2)
			{
				sum2 += sqrt(varx*varx + vary*vary + varz*varz);//sqrt(pow((x - res.at<double>(0, 0)), 2) + pow((y - res.at<double>(1, 0)), 2) + pow((z - res.at<double>(2, 0)), 2));
				koliko2++;
			}
			if (kolikomarkera == 3)
			{
				sum3 += sqrt(varx*varx + vary*vary + varz*varz);//sqrt(pow((x - res.at<double>(0, 0)), 2) + pow((y - res.at<double>(1, 0)), 2) + pow((z - res.at<double>(2, 0)), 2));
				koliko3++;
			}

			/*
			if (maxvar < sqrt(varx*varx + vary*vary + varz*varz))
			{
				maxvar = sqrt(varx*varx + vary*vary + varz*varz);
				indmax = br;
			}
			if (minvar > sqrt(varx*varx + vary*vary + varz*varz))
			{
				minvar = sqrt(varx*varx + vary*vary + varz*varz);
			}
			*/


			/*
			for (int x = 0; x<width; x++)
			{
			for (int y = 0; y<height; y++)
			{
			//if ((gimage[x][y]!=1)&&(gimage[x][y]!=0)) {image[x][y][0]=255; image[x][y][1]=255; image[x][y][2]=255;}
			//else {image[x][y][0]=0; image[x][y][1]=0; image[x][y][2]=0;}
			if (gimage[x][y] == -2) { image[x][y][0] = 0; image[x][y][1] = 255; image[x][y][2] = 0; }
			else {
			//image[x][y][0] = gimage2[x][y]*255 ; image[x][y][1] = gimage2[x][y] * 255; image[x][y][2] = gimage2[x][y] *255;
			}
			//else if (gimage[x][y]>1) {image[x][y][0]=255; image[x][y][1]=255; image[x][y][2]=255;}
			//image[x][y][0]=gimage2[x][y]; image[x][y][1]=gimage2[x][y]; image[x][y][2]=gimage2[x][y];

			}
			}


			writeBMP(locationn, width, height, image);


			*/
		}
		else
		{
			cout << "fail" << endl;
			varijanset << "fail" << endl;
			varijanser << "fail" << endl;
			varijansete << "fail" << endl;
			varijansere << "fail" << endl;
		}
	}

	//cout << " Slika sa jednim markerom ima " << koliko1 << " i tu se gresi u proseku " << sum1 / koliko1 << endl;
	//cout << " Slika sa dva markera ima " << koliko2 << " i tu se gresi u proseku " << sum2 / koliko2 << endl;
	//cout << " Slika sa tri markera ima " << koliko3 << " i tu se gresi u proseku " << sum3 / koliko3 << endl;

	//cout << maxvar <<" " <<minvar <<" " <<indmax;

	varijanset.close();
	varijanser.close();
	varijansete.close();
	varijansere.close();
}