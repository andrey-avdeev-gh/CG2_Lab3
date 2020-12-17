#include <iostream>
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
}


void task_2()
{
	Mat src = chooseimg();
	Mat image = src;

	GaussianBlur(image, image, 5.0f, 5);

	CvSeq* results = cvHoughCircles(image,storage,CV_HOUGH_GRADIENT,10,	image->width / 5);
	for (int i = 0; i < results->total; i++) {
		float* p = (float*)GetSeqElem(results, i);
		Point pt = Point(cvRound(p[0]), cvRound(p[1]));
		circle(src, pt, cvRound(p[2]), CV_RGB(0xff, 0, 0));
	}

	namedWindow("cvHoughCircles", 1);
	imshow("cvHoughCircles", src);

	waitKey(0);
	destroyAllWindows();
	main();
}


void task_2_var2()
{
	Mat image = chooseimg(), src, dst, color_dst;

	src = Mat::zeros(image.rows, image.cols, CV_8UC1);
	dst = Mat::zeros(image.rows, image.cols, CV_8UC1);

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i = 0;


	dst = cvCreateImage(cvGetSize(src), 8, 1);
	color_dst = cvCreateImage(cvGetSize(src), 8, 3);

	Canny(src, dst, 50, 200, 3);

	cvtColor(dst, color_dst, COLOR_GRAY2BGR);
	lines = houghLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);

	for (i = 0; i < lines->total; i++) {
		Point* line = (Point*)cvGetSeqElem(lines, i);
		Line(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 3, AA, 0);
	}

	// показываем
	namedWindow("Source", 1);
	imshow("Source", src);

	namedWindow("Hough", 1);
	imshow("Hough", color_dst);

	waitKey(0);
	destroyAllWindows();
	main();
}

void task_3() 
{
	Mat image = chooseimg(), dst, gray, bin;

	// создаём одноканальные картинки
	gray = Mat::zeros(image.rows, image.cols, CV_8UC1);
	bin = Mat::zeros(image.rows, image.cols, CV_8UC1);
	// клонируем
	dst = image;
	// окно для отображения картинки
	namedWindow("original", WINDOW_AUTOSIZE);
	namedWindow("binary", WINDOW_AUTOSIZE);
	namedWindow("contours", WINDOW_AUTOSIZE);

	// преобразуем в градации серого
	cvtColor(image, gray, COLOR_RGB2GRAY);

	// преобразуем в двоичное
	cvInRangeS(gray, Scalar(40), Scalar(150), bin); // atoi(argv[2])

	CvSeq* contours = 0;

	// находим контуры
	int contoursCont = findContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// нарисуем контуры
	for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {
		drawContours(dst, seq0, 255, CV_RGB(0, 0, 250), 0, 1, 8); // рисуем контур
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
		task_2_var2();
		break;
	case 4:
		task_3();
		break;
	case 0:
		exit(1);
	default:
		break;
	}
}