#include "opencv2\opencv.hpp"
#include "kalman.h"


kalman::kalman(int _n, int _m) {
	n = _n;
	m = _m;
}

void  kalman::setFixed(cv::Mat _A, cv::Mat _H, cv::Mat _Q, cv::Mat _R) {
	A = _A;
	H = _H;
	Q = _Q;
	R = _R;
	I = cv::Mat(n, n, cv::DataType<float>::type, 0);
	for (int i = 0; i < n; i++)
		I.at<float>(i, i) = 1;
}

void  kalman::setFixed(cv::Mat _A, cv::Mat _H, cv::Mat _Q, cv::Mat _R, cv::Mat _B) {
	A = _A;
	B = _B;
	H = _H;
	Q = _Q;
	R = _R;
	I = cv::Mat(n, n, cv::DataType<float>::type, 0);
	for (int i = 0; i < n; i++)
		I.at<float>(i, i) = 1;
}


void  kalman::setInitial(cv::Mat _X0, cv::Mat _P0) {
	X0 = _X0;
	P0 = _P0;
}

void  kalman::predict(void) {
	X = (A * X0);
	static cv::Mat At;
	transpose(A, At);
	P = (A * P0 * At) + Q;
}

void  kalman::predict(cv::Mat U) {
	X = (A * X0) + (B * U);
	static cv::Mat At;
	transpose(A, At);
	P = (A * P0 * At) + Q;
}

void  kalman::correct(cv::Mat Z) {

	static cv::Mat Ht;
	transpose(H, Ht);

	K = (P * Ht) * (H * P * Ht + R).inv();

	X = X + K*(Z - H * X);

	P = (I - K * H) * P;

	X0 = X;
	P0 = P;
}