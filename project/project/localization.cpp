#include <vector>
#include <iostream>
#include "algorithms.h"


using namespace std;

bool findcamera(vector<vector<double> >& results, int& kolikomarkera, vector<vector<vector<unsigned char> > >& image, int width, int height, int broj_markera, const vector<vector<vector<int> > >& markeri_niz, Mat cameraMatrix, Mat distCoeffs)
{
	bool marker_found = false;

	int dim, kvad, maks, minim, localtres;
	vector<vector<int> > gimage, gimage2;


	int trashholdivice = 10;
	int trashholdrast = 5;
	int binc = 15;
	int bintrash = 40;
	double epsdaglas = 5;

	vector<Point2d> imagePoints;
	vector<Point3d> objectPoints;

	gimage.resize(width);
	gimage2.resize(width);
	for (int x = 0; x < width; x++)
	{
		gimage[x].resize(height);
		gimage2[x].resize(height);
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			gimage[x][y] = (image[x][y][0] + image[x][y][1] + image[x][y][2]) / 3;
			// image[x][y][0]=gimage[x][y]; image[x][y][1]=gimage[x][y]; image[x][y][2]=gimage[x][y];
		}
	}



	binarization(gimage, width, height, bintrash, binc);



	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
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
	rects = douglaspeucker(gimage, width, height, epsdaglas);




	vector<vector<vector<unsigned char> > > crpimage;


	int minx, miny, maxx, maxy;

	coord koord;



	int min_index;

	bool indikator;

	//cout << rects.size();
	for (int i = 0; i < rects.size(); i++)
	{
		indikator = true;
		for (int j = 0; j < 4; j++)
		{

			koord = rects[i].front();

			//cout << koord.x << " " << koord.y <<endl;

			if (sqrt((rects[i].front().x - rects[i].back().x)*(rects[i].front().x - rects[i].back().x) +
				(rects[i].front().y - rects[i].back().y)*(rects[i].front().y - rects[i].back().y)) < trashholdivice)
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
		for (int k = 0; k < 4; k++)
		{
			koord = rects[i].front();



			if (minx > koord.x) minx = koord.x;
			if (miny > koord.y) miny = koord.y;
			if (maxx < koord.x) maxx = koord.x;
			if (maxy < koord.y) maxy = koord.y;


			rects[i].pop_front();
			rects[i].push_back(koord);

			//gimage2[koord.x][koord.y]=-1;
		}


		crpimage.resize(maxx - minx + 1);
		for (int x = 0; x < maxx - minx + 1; x++)
		{
			crpimage.at(x).resize(maxy - miny + 1);



			for (int y = 0; y < maxy - miny + 1; y++)
			{
				crpimage.at(x).at(y).resize(3);
				crpimage[x][y][0] = crpimage[x][y][1] = crpimage[x][y][2] = gimage2[minx + x][miny + y] * 255;
			}
		}


		vector<vector<int> > marker;//(8, vector<int> (8));

		marker = bacimrezu(gimage2, minx, miny, maxx, maxy, rects[i]);

		int rast_min = 64;

		for (int k = 0; k < broj_markera; k++)
		{
			int rast = 0;


			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (markeri_niz[k][i][j] != marker[j][i]) rast++;
				}
			}

			//cout <<endl <<rast <<endl;

			if (rast <= rast_min)
			{
				rast_min = rast;
				min_index = k;

				//cout <<" " <<rast_min;
			}

		}

		if (rast_min < trashholdrast)
		{
			marker_found = true;

			Point2d point2;
			Point3d point3;

			//prepisi image points

			for (int j = 0; j < 4; j++)
			{
				koord = rects[i].front();


				cout << koord.x << " " << koord.y << endl;

				point2.x = koord.x;// -cameraMatrix.at<double>(0, 2);
				point2.y = koord.y;// -cameraMatrix.at<double>(1, 2);


				imagePoints.push_back(point2);


				rects[i].pop_front();
				rects[i].push_back(koord);
			}

			/*
			imagePoints[0].x = 1;
			imagePoints[0].y = 0;

			imagePoints[1].x = 2;
			imagePoints[1].y = 0;

			imagePoints[2].x = 2;
			imagePoints[2].y = 1;

			imagePoints[3].x = 1;
			imagePoints[3].y = 10;
			*/

			//ucitaj object points


			stringstream ss;
			ss << "F:\\Projects\\Unfinished\\markeri\\Markeri\\kodovi\\txt\\" << min_index << min_index << ".txt";

			ifstream recnik(ss.str());

			cout << min_index << endl;


			double x1, x2, x3;

			for (int j = 0; j < 4; j++)
			{
				recnik >> x1 >> x2 >> x3;

				cout << x1 << " " << x2 << " " << x3 << endl;

				point3.x = x1;
				point3.y = x2;
				point3.z = x3;

				objectPoints.push_back(point3);


				//cout << objectPoints[j].x << " " << objectPoints[j].y << " " << objectPoints[j].z << endl;

			}

			ss.clear();
			ss.str(string());
			recnik.close();
		}
	}


	/*
	objectPoints[0].x = 0;
	objectPoints[0].y = -10;
	objectPoints[0].z = 10;

	objectPoints[1].x = 10;
	objectPoints[1].y = -10;
	objectPoints[1].z = 10;

	objectPoints[2].x = 10;
	objectPoints[2].y = 0;
	objectPoints[2].z = 10;

	objectPoints[3].x = 0;
	objectPoints[3].y = 0;
	objectPoints[3].z = 10;

	*/

	/*

	cameraMatrix.at<double>(0, 0) = 1;
	cameraMatrix.at<double>(1, 1) = 1;

	cameraMatrix.at<double>(0, 2) = 10;// width / 2;
	cameraMatrix.at<double>(1, 2) = 10; // height / 2;


	//cout << cameraMatrix <<endl;

	std::cout << "TEST" << std::endl;

	Mat imagepoints2(3, 1, DataType<double>::type);
	for (int k = 0; k < 4; k++)
	{
	cv::Mat tmp = cv::Mat(objectPoints[k]);
	std::cout << "type of tmp " << tmp.type() << std::endl;

	std::cout << "type of camera matrix " << cameraMatrix.type() << std::endl;
	std::cout << "type of image points 2 " << imagepoints2.type() << std::endl;


	imagepoints2 = cameraMatrix * tmp;
	cout << endl << imagepoints2 << endl << objectPoints[k];

	imagePoints[k].x = imagepoints2.at<double>(0, 0) / imagepoints2.at<double>(2, 0);
	imagePoints[k].y = imagepoints2.at<double>(1, 0) / imagepoints2.at<double>(2, 0);

	cout <<" moji image pointsi : "<< imagePoints[k].x << " " << imagePoints[k].y <<endl;

	}

	objectPoints[0].x = 0;
	objectPoints[0].y = 0;
	objectPoints[0].z = 10;

	objectPoints[1].x = 0;
	objectPoints[1].y = 10;
	objectPoints[1].z = 10;

	objectPoints[2].x = 0;
	objectPoints[2].y = 10;
	objectPoints[2].z = 0;

	objectPoints[3].x = 0;
	objectPoints[3].y = 0;
	objectPoints[3].z = 0;

	*/

	//cout << cameraMatrix << endl;





	if (marker_found)
	{

		Mat rvec(3, 1, DataType<double>::type);
		Mat tvec(3, 1, DataType<double>::type);

		Mat dstCoeffs(5, 1, DataType<double>::type);
		for (int i = 0; i < 5; i++) dstCoeffs.at<double>(i) = 0;

		kolikomarkera = objectPoints.size() / 4;
		if (solvePnP(objectPoints, imagePoints, cameraMatrix, dstCoeffs, rvec, tvec, false, CV_ITERATIVE))
		{
			//cout << "ROTACIJA: " << rvec << endl;
			//cout << "TRANSLACIJA: " << tvec << endl;

			Mat matrica(3, 3, DataType<double>::type);
			Rodrigues(rvec, matrica);


			Mat vec = Mat(rotationMatrixToEulerAngles(matrica));
			//cout << vec << endl;
			//cout << matrica << endl;

			Mat matrica2(4, 4, DataType<double>::type);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					matrica2.at<double>(i, j) = matrica.at<double>(i, j);
			for (int i = 0; i < 3; i++) matrica2.at<double>(i, 3) = tvec.at<double>(i, 0);
			matrica2.at<double>(3, 3) = 1;
			for (int i = 0; i < 3; i++) matrica2.at<double>(3, i) = 0;

			//cout << matrica2 << endl;

			Mat res(4, 1, DataType<double>::type);
			Mat zamnoziti(4, 1, DataType<double>::type);
			zamnoziti.at<double>(0, 0) = 0;
			zamnoziti.at<double>(1, 0) = 0;
			zamnoziti.at<double>(2, 0) = 0;
			zamnoziti.at<double>(3, 0) = 1;



			Mat matrica3(4, 4, DataType<double>::type);
			matrica3 = matrica2.inv();

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					matrica.at<double>(i, j) = matrica3.at<double>(i, j);

			Rodrigues(matrica, rvec);

			res = matrica3 * zamnoziti;


			//cout << matrica3 << endl;

			//cout << matrica << endl;

			rvec = Mat(rotationMatrixToEulerAngles(matrica));

			rvec = (rvec / CV_PI) * 180.0;


			//cout << res << endl << rvec << endl;


			

			//cout << res << endl << rvec << endl;

			objectPoints.erase(objectPoints.begin(), objectPoints.end());
			imagePoints.erase(imagePoints.begin(), imagePoints.end());

			vector<vector<double> > result(2, vector<double> (3));
			result[0][0] = res.at<double>(0, 0);
			result[0][1] = res.at<double>(1, 0);
			result[0][2] = res.at<double>(2, 0);
		
			result[1][0] = rvec.at<float>(0, 0);
			result[1][1] = rvec.at<float>(1, 0);
			result[1][2] = rvec.at<float>(2, 0);

			for (int gg = 0; gg < 3; gg++) if (result[1][gg] < 0) result[1][gg] += 360;

			results = result;

			return true;

		}
		//else cout << "PnP fejlovao";

	
	}

	return false;
}