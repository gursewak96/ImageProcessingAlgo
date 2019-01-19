#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void Sharpen(const Mat &, Mat&);
void EdgeDetection(const Mat& I, Mat & R);

static void help() {
	cout << endl
		<< "This program shows how much time is taked by edge detection implementation."
		<< endl;
}

int main(int argc, char* argv[]){

	help();
	//initialising pictures at specific path
	Mat N = imread("../../images/img.jpg");
	Mat M,J;
	Size size(500, 500);

	//resizing the image
	resize(N,M, size);

	//Original picture
	namedWindow("Original image", WINDOW_AUTOSIZE);
	imshow("Original image", M);

	
	const int times = 100;
	double t;

	//creating kernel
	Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 8, -1, 0, -1, 0);
	 
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	//Measuring time for Sharpen function
	Mat _I = M.clone();

	
	Mat R;
	
	 t = (double)getTickCount();
	// Using one of three defined function to check the time it takes to reduce the image color
	 EdgeDetection(_I, R);
	//Averaging time in milliseconds
	//t = 1000000*((double)getTickCount() - t)/getTickFrequency();
	 t = 1000*(double)((getTickCount() - t) / getTickFrequency());

	cout << "Average Time taken to sharpening the Image " << t <<" milliseconds."<< endl;

	namedWindow("Processed Image");

	//resulted image
	imshow("Processed Image", R);
	waitKey(0);
	return 0;
}





void Sharpen(const Mat& I, Mat & R) {
	CV_Assert(I.depth() == CV_8U);

	const int nChannels = I.channels();
	R.create(I.size(), I.type());

	for (int i = 1; i < I.rows - 1; i++) {
		const uchar * prev = I.ptr<uchar>(i - 1);
		const uchar * current = I.ptr<uchar>(i);
		const uchar * next = I.ptr<uchar>(i + 1);

		uchar * output = R.ptr<uchar>(i);
		for (int j = nChannels; j < nChannels*(I.cols - 1); j++) {
			*output++ = saturate_cast<uchar>(5 * current[j] - current[j - nChannels] - current[j + nChannels] - prev[j] - next[j]);
		}
	}

	R.row(0).setTo(Scalar(0));
	R.row(R.rows - 1).setTo(Scalar(0));
	R.col(0).setTo(Scalar(0));
	R.col(R.cols - 1).setTo(Scalar(0));
}

void EdgeDetection(const Mat& I, Mat & R) {
	CV_Assert(I.depth() == CV_8U);

	const int nChannels = I.channels();
	R.create(I.size(), I.type());

	for (int i = 1; i < I.rows - 1; i++) {
		const uchar * prev = I.ptr<uchar>(i - 1);
		const uchar * current = I.ptr<uchar>(i);
		const uchar * next = I.ptr<uchar>(i + 1);

		uchar * output = R.ptr<uchar>(i);
		for (int j = nChannels; j < nChannels*(I.cols - 1); j++) {
			*output++ = saturate_cast<uchar>(-4 * current[j] + current[j - nChannels] + current[j + nChannels] + prev[j] + next[j]);
		}
	}

	R.row(0).setTo(Scalar(0));
	R.row(R.rows - 1).setTo(Scalar(0));
	R.col(0).setTo(Scalar(0));
	R.col(R.cols - 1).setTo(Scalar(0));
}