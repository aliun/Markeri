#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace cv;

const float calibrationSquareDimension = 0.0243f;
const float arcuoSquareDimension = 1;
const Size chessBoardDimension = Size(9, 6);

void createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners)
{
	for (int i = 0; i<boardSize.height; i++)
	{
		for (int j = 0; j<boardSize.width; j++)
		{
			corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
		}
	}
}

void getChessBoardCorners(vector<Mat> images, vector<vector<Point2f> >& allFoundCorners, bool showResult = false)
{
	for (vector<Mat>::iterator i = images.begin(); i != images.end(); i++)
	{
		vector<Point2f> pointBuf;
		bool found = findChessboardCorners(*i, Size(9, 6), pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

		if (found)
		{
			allFoundCorners.push_back(pointBuf);
		}

		if (showResult)
		{
			drawChessboardCorners(*i, Size(9, 6), pointBuf, found);
			imshow("lepo", *i);
			waitKey(0);
		}
	}
}

double computeReprojectionErrors(const vector<vector<Point3f> >& objectPoints,
	const vector<vector<Point2f> >& imagePoints,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const Mat& cameraMatrix, const Mat& distCoeffs,
	vector<float>& perViewErrors)
{
	vector<Point2f> imagePoints2;
	int i, totalPoints = 0;
	double totalErr = 0, err;
	perViewErrors.resize(objectPoints.size());

	for (i = 0; i < (int)objectPoints.size(); ++i)
	{
		projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,  // project
			distCoeffs, imagePoints2);
		err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);              // difference

		int n = (int)objectPoints[i].size();
		perViewErrors[i] = (float)std::sqrt(err*err / n);                        // save for this view
		totalErr += err*err;                                             // sum it up
		totalPoints += n;
	}

	return std::sqrt(totalErr / totalPoints);              // calculate the arithmetical mean
}

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distortionCoeff, double& calibError)
{
	vector<vector<Point2f> > checkerboardImageSpacePoints;
	getChessBoardCorners(calibrationImages, checkerboardImageSpacePoints, false);

	vector<vector<Point3f> > worldSpaceCornerPoints(1);


	createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);

	worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

	vector<Mat> rVectors, tVectors;
	distortionCoeff = Mat::zeros(8, 1, CV_64F);


	calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize, cameraMatrix, distortionCoeff, rVectors, tVectors);
	
	vector<float> perViewError;
	calibError = computeReprojectionErrors(worldSpaceCornerPoints, checkerboardImageSpacePoints, rVectors, tVectors, cameraMatrix, distortionCoeff, perViewError);

	

	//printf("%f", calibError);
}

bool saveCameraCalibration(Mat cameraMatrix, Mat distortionCoeff, double calibError)
{
//	FileStorage fs("file.yml", FileStorage::WRITE);
//	fs << "cameraMatrix" << cameraMatrix;
	////fs << "distortionCoeff" << distortionCoeff;
	//fs << "calibError" << calibError;
	//fs.release();


	//_______________________________________________________________________________________________________________________________________________
	ofstream outStream("mama.txt");
	if (outStream)
	{
		uint16_t rows = cameraMatrix.rows;
		uint16_t columns = cameraMatrix.cols;

		for (int r = 0; r<rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				double value = cameraMatrix.at<double>(r, c);
				outStream << value << endl;
			}
		}

		rows = distortionCoeff.rows;
		columns = distortionCoeff.cols;

		for (int r = 0; r<rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				double value = distortionCoeff.at<double>(r, c);
				outStream << value << endl;
			}
		}
		outStream << calibError << endl;
		outStream.close();
		return true;
	}
	return false;
}






int main(int argc, char** argv)
{
	Mat frame;
	Mat drawToFrame;

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
	
	Mat distortionCoeff;

	double calibError;

	vector<Mat> savedImages;

	vector<vector<Point2f> > markerCorners, rejectedCandidates;

	bool ind;

	VideoCapture vid(0);

	ind = vid.set(CV_CAP_PROP_FRAME_WIDTH, 640);

	ind = vid.set(CV_CAP_PROP_FRAME_HEIGHT, 480);




	if (!vid.isOpened())
	{
		return 0; //////000000 po tutu
	}

	int framesPerSecond = 20; 


	namedWindow("Webcam", CV_WINDOW_AUTOSIZE);


	stringstream ss;
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);


	while (true)
	{
		if (!vid.read(frame))
			{
			break;
			}

		vector<Vec2f> foundPoints;
		bool found = false;

		

		found = findChessboardCorners(frame, chessBoardDimension, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
		frame.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame, chessBoardDimension, foundPoints, found);

		if (found)
			{
			imshow("Webcam", drawToFrame);
			}
		else
			{
			imshow("Webcam", frame);
			}

		
		char character = waitKey(100 / framesPerSecond);

		//cout << "pizdo";
		switch (character)
			{
		case ' ':
			//saving
			
			//cout << "cuvam_pizdo";
			//printf("Filecreadddddted");

			if (true)//(found)
				{
				ss << savedImages.size()+1 << ".png";
				//frame = imread(ss.str());

				Mat temp;
				frame.copyTo(temp);
				savedImages.push_back(temp);
				cout << "cuvam_pizdo";
				
				
				cout << savedImages.size() ;
				cout << endl;
				imwrite(ss.str(), temp, compression_params);

				ss.clear();
				ss.str(string());
				}

			break;

		case 'f':
			//calibration
			
			if (savedImages.size()>15)
				{
				cameraCalibration(savedImages, chessBoardDimension, calibrationSquareDimension, cameraMatrix, distortionCoeff, calibError);
				saveCameraCalibration(cameraMatrix, distortionCoeff, calibError);
				cout<< "File created";
				}
			break;
		
		case 27:
			//exit
			return 0;
			break;
			}

		
	}

}
