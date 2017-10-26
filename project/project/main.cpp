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

using namespace cv;
using namespace std;

#include "metrics.h"
#include "video.h"

int broj_slika = 288;
string lokacijan = "F:\\Projects\\Unfinished\\markeri\\Markeri\\baza\\pravabaza\\klasa1\\";
string lokacijavideo = "F:\\Projects\\Unfinished\\markeri\\Markeri\\baza\\temp.mp4";
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

	cout << cameraMatrix << endl;

	Mat distCoeffs(5, 1, DataType<double>::type);
	for (int i=0; i<5; i++) distkoef >> distCoeffs.at<double>(i);

	vector<vector<vector<int> > > markeri_niz(broj_markera, vector<vector<int> >(8, vector<int>(8)));
	for (int k = 0; k < broj_markera; k++)
	{
		
		stringstream ss;
		ss << "F:\\Projects\\Unfinished\\markeri\\Markeri\\kodovi\\txt\\" << k << ".txt";


		markeri.open(ss.str());

		ss.clear();
		ss.str(string());

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				markeri >> markeri_niz[k][i][j];
			}
		}


		markeri.close();
	}

	uradimetriku(lokacijan, broj_slika, broj_markera, markeri_niz, cameraMatrix, distCoeffs);
	//ajmo_video(lokacijavideo, broj_markera, markeri_niz, cameraMatrix, distCoeffs);

	waitKey();
}