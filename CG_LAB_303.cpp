﻿#include <iostream>
#include <string>
#include <iomanip>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int main();

Mat chooseimg()
{
	cout << endl << "Enter full name of the file: " << endl;
	string imagename = "";
	cin >> imagename;

	try
	{
		Mat img = imread(samples::findFile(imagename), IMREAD_COLOR);
		if (img.empty())
		{
			cout << "Image cannot be loaded..!!" << endl;
			exit(0);
		}

		return img;
	}
	catch (cv::Exception)
	{
		cout << "Image cannot be loaded..!!" << endl;
		exit(0);
	}


}


void task_1()
{
	Mat image = chooseimg(), gray, dst;


		gray = Mat::zeros(image.rows, image.cols, CV_8UC1);
		dst = Mat::zeros(image.rows, image.cols, CV_8UC1);

		namedWindow("original", WINDOW_AUTOSIZE);
		namedWindow("gray", WINDOW_AUTOSIZE);
		namedWindow("cvCanny", WINDOW_AUTOSIZE);


		cvtColor(image, gray, COLOR_RGB2GRAY);
		Canny(gray, dst, 10, 100, 3);

		imshow("original", image);
		imshow("gray", gray);
		imshow("cvCanny", dst);

		waitKey(0);
		destroyAllWindows();
		main();
}


void task_2()
{
	Mat src = chooseimg();
	Mat gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	medianBlur(gray, gray, 5);
	vector<Vec3f> circles;
	HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
		gray.rows / 16,
		100, 30, 1, 30
	);
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);
		// circle center
		circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
		// circle outline
		int radius = c[2];
		circle(src, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
	}
	imshow("detected circles", src);
	waitKey();
	destroyAllWindows();
	main();
}

void task_3() 
{
	Mat image = chooseimg(), dst, gray, bin;
	gray = Mat::zeros(image.rows, image.cols, CV_8UC1);
	bin = Mat::zeros(image.rows, image.cols, CV_8UC1);
	dst = image;
	namedWindow("original", WINDOW_AUTOSIZE);
	namedWindow("binary", WINDOW_AUTOSIZE);
	namedWindow("contours", WINDOW_AUTOSIZE);

	cvtColor(image, gray, COLOR_RGB2GRAY);

	inRange(gray, Scalar(40), Scalar(150), bin);
	int contoursCont = findContours(bin, storage, &contours, sizeof(CvContour), RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {/		drawContours(dst, seq0, 255, CV_RGB(0, 0, 250), 0, 1, 8);
	}
	imshow("original", image);
	imshow("binary", bin);
	imshow("contours", dst);

	waitKey(0);
	destroyAllWindows();
	main();
}

int main()
{
	int opt;

	cout
		<< endl
		<< "   Main Menu    " << endl
		<< endl << "Write 1 for task_1" << endl
		<< endl << "Write 2 for task_2" << endl
		<< endl << "Write 6 for task_3" << endl
		<< endl
		<< endl << "Write 0 to exit" << endl
		<< endl
		;

	cin >> opt;
	cout << "\n" << endl;

	switch (opt)
	{
	case 1:
		task_1();
		break;
	case 2:
		task_2();
		break;
	case 3:
		task_3();
		break;
	case 0:
		exit(1);
	default:
		break;
	}
}