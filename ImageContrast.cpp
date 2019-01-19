#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void Sharpen(const Mat &, Mat&);

int main(int argc, char* argv[]){
	//initialising pictures at specific path
	Mat N = imread("../../images/img.jpg");
	Mat M,J;
	Size size(500, 500);
	resize(N,M, size);
	namedWindow("Original image", WINDOW_AUTOSIZE);
	
	//Original picture
	imshow("Original image", M);

	
	const int times = 100;
	double t;
	 
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	//Measuring time for Sharpen function

	t = (double)getTickCount();
	Mat R;
	for (int i = 0; i < times; i++) {
		Mat _I = M.clone();

		// Using one of three defined function to check the time it takes to reduce the image color
		Sharpen(_I, R);
	}
	//Averaging time in milliseconds
	t = ((double)getTickCount() - 1) / getTickFrequency();
	t /= times;

	cout << "Average Time taken to sharpening the Image " << times << " runs): " << t << endl;

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
