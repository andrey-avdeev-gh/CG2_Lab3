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
	IplImage* image = 0;
	IplImage* gray = 0;
	IplImage* dst = 0;

		// имя картинки задаётся первым параметром
		char* filename = argc == 2 ? argv[1] : "Image0.jpg";
		// получаем картинку
		image = cvLoadImage(filename, 1);

		printf("[i] image: %s\n", filename);
		assert(image != 0);

		// создаём одноканальные картинки
		gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
		dst = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);

		// окно для отображения картинки
		cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("gray", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("cvCanny", CV_WINDOW_AUTOSIZE);

		// преобразуем в градации серого
		cvCvtColor(image, gray, CV_RGB2GRAY);

		// получаем границы
		cvCanny(gray, dst, 10, 100, 3);

		// показываем картинки
		cvShowImage("original", image);
		cvShowImage("gray", gray);
		cvShowImage("cvCanny", dst);

		// ждём нажатия клавиши
		cvWaitKey(0);

		// освобождаем ресурсы
		cvReleaseImage(&image);
		cvReleaseImage(&gray);
		cvReleaseImage(&dst);
		// удаляем окна
		cvDestroyAllWindows();
}


void task_2()
{
	IplImage* image = 0;
	// имя картинки задаётся первым параметром
	char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
	// получаем картинку (в градациях серого)
	image = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);

	printf("[i] image: %s\n", filename);
	assert(image != 0);

	// загрузим оригинальное изображении
	IplImage* src = cvLoadImage(filename);

	// хранилище памяти для кругов
	CvMemStorage* storage = cvCreateMemStorage(0);
	// сглаживаем изображение
	cvSmooth(image, image, CV_GAUSSIAN, 5, 5);
	// поиск кругов
	CvSeq* results = cvHoughCircles(
		image,
		storage,
		CV_HOUGH_GRADIENT,
		10,
		image->width / 5
	);
	// пробегаемся по кругам и рисуем их на оригинальном изображении
	for (int i = 0; i < results->total; i++) {
		float* p = (float*)cvGetSeqElem(results, i);
		CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
		cvCircle(src, pt, cvRound(p[2]), CV_RGB(0xff, 0, 0));
	}
	// показываем
	cvNamedWindow("cvHoughCircles", 1);
	cvShowImage("cvHoughCircles", src);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&image);
	cvReleaseImage(&src);
	cvDestroyAllWindows();
}


void task_2_var2()
{
	IplImage* src = 0;
	IplImage* dst = 0;
	IplImage* color_dst = 0;

	// имя картинки задаётся первым параметром
	char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
	// получаем картинку (в градациях серого)
	src = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);

	if (!src) {
		printf("[!] Error: cant load image: %s \n", filename);
		return -1;
	}

	printf("[i] image: %s\n", filename);


	// хранилище памяти для хранения найденных линий
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i = 0;


	dst = cvCreateImage(cvGetSize(src), 8, 1);
	color_dst = cvCreateImage(cvGetSize(src), 8, 3);

	// детектирование границ
	cvCanny(src, dst, 50, 200, 3);

	// конвертируем в цветное изображение
	cvCvtColor(dst, color_dst, CV_GRAY2BGR);

	// нахождение линий
	lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);

	// нарисуем найденные линии
	for (i = 0; i < lines->total; i++) {
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 3, CV_AA, 0);
	}

	// показываем
	cvNamedWindow("Source", 1);
	cvShowImage("Source", src);

	cvNamedWindow("Hough", 1);
	cvShowImage("Hough", color_dst);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseImage(&color_dst);
	cvDestroyAllWindows();
	main();
}

void task_3() 
{
	// имя картинки задаётся первым параметром
	char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
	// получаем картинку
	image = cvLoadImage(filename, 1);

	printf("[i] image: %s\n", filename);
	assert(image != 0);

	// создаём одноканальные картинки
	gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	bin = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	// клонируем
	dst = cvCloneImage(image);
	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("binary", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("contours", CV_WINDOW_AUTOSIZE);

	// преобразуем в градации серого
	cvCvtColor(image, gray, CV_RGB2GRAY);

	// преобразуем в двоичное
	cvInRangeS(gray, cvScalar(40), cvScalar(150), bin); // atoi(argv[2])

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	// находим контуры
	int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// нарисуем контуры
	for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {
		cvDrawContours(dst, seq0, CV_RGB(255, 216, 0), CV_RGB(0, 0, 250), 0, 1, 8); // рисуем контур
	}

	// показываем картинки
	cvShowImage("original", image);
	cvShowImage("binary", bin);
	cvShowImage("contours", dst);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&gray);
	cvReleaseImage(&bin);
	cvReleaseImage(&dst);
	// удаляем окна
	cvDestroyAllWindows();
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