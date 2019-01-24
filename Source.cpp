#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"

#include<iostream>
using namespace std;
using namespace cv;

//function to find distance between two pixels
int dist(int b1, int g1, int r1, int b2, int g2, int r2);

//gives the pixel value where the mouse is clicked
void mouseEvent(int event, int x, int y, int flags, void *param);

//flag to handle color assignment during mouse event
bool flag = false;

int main()
{
	//variable to stores 3 channels of a pixel
	int r1, g1, b1, r2, g2, b2;

	//point object to pass during mouse event
	Point point;

	//variable to store mouse position
	int x, y;

	namedWindow("Cam");
	
	//Getting feed from camera
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cout << "Cant open the file." << endl;
	}
	
	//desired color blue
	r1 = 0;
	b1 = 255;
	g1 = 0;
	
	while (true) {
		Mat frame;
		cap.read(frame);
		
		//Maximum possible distance between pixels
		int worldRecord = 10000;
		int d = worldRecord;

		setMouseCallback("Cam", mouseEvent, &point);

		if (flag) {
			b1 = frame.at<Vec3b>(point.y, point.x)[0];
			g1 = frame.at<Vec3b>(point.y, point.x)[1];
			r1 = frame.at<Vec3b>(point.y, point.x)[2];
			flag = false;
		}
		

		
		//Magic comes here
		for (int i = 0; i < frame.rows; ++i) {
			for (int j = 0; j < frame.cols; j++) {
				b2 = frame.at<Vec3b>(i, j)[0];
				g2 = frame.at<Vec3b>(i, j)[1];
				r2 = frame.at<Vec3b>(i, j)[2];

				worldRecord = dist(b1, g1, r1, b2, g2, r2);
				
				if (d > worldRecord){
					d = worldRecord;
					y = i;
					x = j;
					}
			}
		}
		//if distance between pixel is less than 200 then draw the circle at that pixel
		if (worldRecord < 300) {
				circle(frame, Point(x, y), 15, Scalar(0, 0, 255), -1, 8);
		}

		imshow("Cam", frame);

		//if key is pressed within 30millisecond then exit the loop
		if (waitKey(30) >= 0) 
			break;
	}

	return 0;
}

int dist(int b1,int g1, int r1, int b2, int g2, int r2) {
	int inter =(int)( pow((abs(b2 - b1)), 2) + pow((abs(g2 - g1)), 2) + pow((abs(r2 - r1)), 2));
	return (int)sqrt(inter);
}

void mouseEvent(int event, int x, int y, int flags, void *param) {
	Point *ptr = (Point *)param;
	if (event == EVENT_LBUTTONDBLCLK) {
		ptr->x = x;
		ptr->y = y;
		flag = true;
	}
	
	

}