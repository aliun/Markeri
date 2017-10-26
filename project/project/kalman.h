#include "opencv2\opencv.hpp"

/*
*	Kalman Filter Class Definition.
*
*	by Fabio Carbone, 23/12/2016
*   www.fabiocarbone.org
*
*	Matrix Dimension must be:
*
*	A: n x n
*	B: n x m
*	H: n x n
*	Q: n x n
*	R: n x n
*	I: n x n
*	X: n x 1
*	U: m x 1
*	Z: n x 1
*	P: n x n
*	K: n x n
*
*/

class kalman{

public:

	/* Problem Dimension */
	int n; //State vector dimension
	int m; //Control vector (input) dimension (if there is not input, set to zero)

		   /* Fixed Matrix */

	cv::Mat A; //System dynamics Matrix
	cv::Mat B; //Control Matrix 
	cv::Mat H; //Mesaurement Adaptation Matrix
	cv::Mat Q; //Process Noise Covariance Matrix
	cv::Mat R; //Measurement Noise Covariance Matrix
	cv::Mat I; //Identity Matrix

			/* Variable Matrix */
	cv::Mat X; //(Current) State vector             
	cv::Mat P; //State Covariance
	cv::Mat K; //Kalman Gain Matrix

				/* Inizial Value */
	cv::Mat X0; //Initial State vector                    
	cv::Mat P0; //Initial State Covariance Matrix

				 
	kalman(int _n, int _m);

	void setFixed(cv::Mat _A, cv::Mat _H, cv::Mat _Q, cv::Mat _R);

	void setFixed(cv::Mat _A, cv::Mat _B, cv::Mat _H, cv::Mat _Q, cv::Mat _R);

	void setInitial(cv::Mat _X0, cv::Mat _P0);

	void predict(void);

	void predict(cv::Mat U);

	void correct(cv::Mat Z);

};
