#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <Afxtempl.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;
int thresh = 100;
int max_thresh = 255;
void thresh_callback(int, void*);
RNG rng(12345);
Mat src_gray;
Mat edge;
Mat dst;
int main(int argc, char** argv) {
	string filename;
	cout << "please input path and filename: ";
	cin >> filename;
	Mat src = imread(filename,CV_LOAD_IMAGE_COLOR);
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	dst=src.clone();
	blur(src_gray,src_gray,Size(3,3));

	namedWindow("source", 0);
	imshow("source", src);

	createTrackbar(" Canny thresh:", "source", &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);

	waitKey(0);
	return(0);
}

//void thresh_callback(int, void*)
//{
//	vector<vector<Point> > contours;
//	vector<Vec4i> hierarchy;
//	/// Detect edges using canny
//	Canny(src_gray, edge, 150, 200, 3);
//	/// Find contours
//	findContours(edge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
//	/// Draw contours
//	for (size_t i = 0; i< contours.size(); i++)
//	{
//		drawContours(dst, contours, (int)i, Scalar(255, 255, 255), 0.1, 8, hierarchy, 0, Point());
//	}
//	/// Show in a window
//	namedWindow("background", 0);
//	imshow("background", dst);
//}

void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours(10000);
	vector<Vec4i> hierarchy(10000);

	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i< contours.size(); i++)
	{
		/*Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));*/
		drawContours(drawing, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
	}
	//test
	/// Show in a window
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	imwrite("contours.jpg",drawing);
}