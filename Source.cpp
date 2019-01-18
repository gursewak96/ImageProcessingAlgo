#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>
using namespace cv;
using namespace std;

void ScanImageAndReduceC(Mat& I, const uchar* const table);
void ScanImageAndReduceI(Mat& I, const uchar* const table);
void ScanImageAndReduceR(Mat& I, const uchar * const table);

int main(int argc, char* argv[]){
	//initialising pictures at specific path
	Mat N = imread("../../images/img.jpg");
	Mat M,J;
	Size size(500, 500);
	resize(N,M, size);
	namedWindow("Colored image", WINDOW_AUTOSIZE);
	
	//Original picture
	imshow("Colored image", M);

	//converting command line string argument to integer
	stringstream ss;
	int divideWidth;
	ss << argv[1];
	ss >> divideWidth;

	//creating a lookup table for image color reduction
	uchar table[256];
	for (int i = 0; i < 256; i++) {
		table[i] = (uchar)(divideWidth*(i / divideWidth));
	}

	const int times = 100;
	double t;

	t = (double)getTickCount();

	for (int i = 0; i < times; i++) {
		Mat _I = M.clone();

		// Using one of three defined function to check the time it takes to reduce the image color
		ScanImageAndReduceC(_I, table);
	}
	//Averaging time in milliseconds
	t = 1000*((double)getTickCount() - 1) / getTickFrequency();
	t /= times;

	cout << "Average Time taken to reduce Image with pointer (Averaged for " << times << " runs): " << t << endl;
	namedWindow("Colored image1", WINDOW_AUTOSIZE);

	//resulted image
	imshow("Colored image1", J);
	waitKey(0);
	return 0;
}

void ScanImageAndReduceC(Mat& I, const uchar* const table) {
	CV_Assert(I.depth() == CV_8U);

	int channels = I.channels();
	int nRows = I.rows;
	int nCols = I.cols*channels;
	if (I.isContinuous()) {
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;

	for (i = 0; i < nRows; i++) {
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; j++) {
			p[j] = table[p[j]];
		}
	}
}

void ScanImageAndReduceI(Mat& I, const uchar* const table) {

	CV_Assert(I.depth() == CV_8U);

	const int channels = I.channels();

	switch (channels) {
		case 1:
		{
			MatIterator_<uchar> it, end;
			for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it) {
				*it = table[*it];
			}
			break;
		}
		case 3:
		{
			MatIterator_<Vec3b> it, end;
			for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it){
				(*it)[0] = table[(*it)[0]];
				(*it)[1] = table[(*it)[0]];
				(*it)[2] = table[(*it)[0]];
			}
		}
	}
}

void ScanImageAndReduceR(Mat& I, const uchar * const table) {
	CV_Assert(I.depth() == CV_8U);
	const int channels = I.channels();
	cout << channels << endl;
	
	switch (channels) {
	case 1:
	{
		for (int i = 0; i < I.rows; i++) {
			for (int j = 0; j < I.cols; j++) {
				I.at<uchar>(i,j) = table[I.at<uchar>(i,j)];
			}
		}
		break;
	}
	case 3:
	{
		Mat_<Vec3b> _I = I;
		for (int i = 0; i < I.rows; i++) {
			for (int j = 0; j < I.cols; ++j) {
				_I(i, j)[0] = table[_I(i, j)[0]];
				_I(i, j)[1] = table[_I(i, j)[1]];
				_I(i, j)[2] = table[_I(i, j)[2]];
			}
		}
		break;
	}
	}
}