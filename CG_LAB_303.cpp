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


int task_1()
{
    Mat src = chooseimg();
    resize(src, src, Size(512, 512));
    src.convertTo(src, CV_32F, 1.0 / 255);
    Mat srcDCT;
    dct(src, srcDCT);

    imshow("src", src);
    imshow("dct", srcDCT);
    waitKey();
    return 0;
    main();
}


int task_2(IplImage* original, float accuracy = 0.1)
{
	assert(original != 0);

	IplImage* src = 0, * rgb = 0;
	IplImage* bin = 0;
	IplImage* phase = 0;

	src = cvCloneImage(original);

	// заведём цветную картинку
	rgb = CreateImage(GetSize(src), 8, 3);
	cvConvertImage(src, rgb, GRAY2BGR);

	// бинарная картинка - для контуров
	bin = CreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCanny(src, bin, 50, 200);

	cvNamedWindow("bin", 1);
	cvShowImage("bin", bin);

	// максимальное расстояние от начала координат - это длина диагонали
	int RMax = cvRound(sqrt((double)(src->width * src->width + src->height * src->height)));

	// картинка для хранения фазового пространства Хафа (r, f)
	// 0 < r < RMax
	// 0 < f < 2*PI
	phase = cvCreateImage(cvSize(RMax, 180), IPL_DEPTH_16U, 1);
	cvZero(phase);

	int x = 0, y = 0, r = 0, f = 0;
	float theta = 0;

	// пробегаемся по пикселям изображения контуров
	for (y = 0; y < bin->height; y++) {
		uchar* ptr = (uchar*)(bin->imageData + y * bin->widthStep);
		for (x = 0; x < bin->width; x++) {
			if (ptr[x] > 0) { // это пиксель контура?
				// рассмотрим все возможные прямые, которые могут 
				// проходить через эту точку
				for (f = 0; f < 180; f++) { //перебираем все возможные углы наклона
					short* ptrP = (short*)(phase->imageData + f * phase->widthStep);
					for (r = 0; r < RMax; r++) { // перебираем все возможные расстояния от начала координат
						theta = f * CV_PI / 180.0; // переводим градусы в радианы

						// Если решение уравнения достаточно хорошее (точность больше заданой)
						if (abs(((y)*sin(theta) + (x)*cos(theta)) - r) < accuracy) {
							ptrP[r]++; // увеличиваем счетчик для этой точки фазового пространства.
						}
					}
				}
			}
		}
	}

	NamedWindow("phase", 1);
	ShowImage("phase", phase);

	// увеличим фазовую картинку
	IplImage* phaseImage = CreateImage(cvSize(phase->width * 3, phase->height * 3), IPL_DEPTH_16U, 1);
	cvResize(phase, phaseImage);

	cvNamedWindow("phaseImage", 1);
	cvShowImage("phaseImage", phaseImage);

	// Выбираем точку фазового пространства которая набрала наибольшее число попаданий
	unsigned int MaxPhaseValue = 0;
	float Theta = 0;
	int R = 0;
	for (f = 0; f < 180; f++) { //перебираем все возможные углы наклона
		short* ptrP = (short*)(phase->imageData + f * phase->widthStep);
		for (r = 0; r < RMax; r++) { // перебираем все возможные расстояния от начала координат
			if (ptrP[r] > MaxPhaseValue) {
				MaxPhaseValue = ptrP[r];
				Theta = f;
				R = r;
			}
		}
	}
	printf("[M] %d\n", MaxPhaseValue);

	// нормировка
	float scaler = 0xFFFFFFFF / (float)MaxPhaseValue;
	for (y = 0; y < phaseImage->height; y++) {
		short* ptr = (short*)(phaseImage->imageData + y * phaseImage->widthStep);
		for (x = 0; x < phaseImage->width; x++) {
			ptr[x] *= scaler;
		}
	}
	cvNamedWindow("phaseImage2", 1);
	cvShowImage("phaseImage2", phaseImage);


	// Рисуем линию по точкам для  R, Teta которые получили в результате преобразования
	Theta = Theta * CV_PI / 180.0;
	for (y = 0; y < rgb->height; y++) {
		uchar* ptr = (uchar*)(rgb->imageData + y * rgb->widthStep);
		for (x = 0; x < rgb->width; x++) {
			if (cvRound(((y)*sin(Theta) + (x)*cos(Theta))) == R) {
				ptr[3 * x] = 0;
				ptr[3 * x + 1] = 0;
				ptr[3 * x + 2] = 255;
			}
		}
	}

	NamedWindow("line", 1);
	ShowImage("line", rgb);

	// освобождаем ресурсы
	ReleaseImage(&src);
	ReleaseImage(&rgb);

	ReleaseImage(&bin);
	ReleaseImage(&phase);
	ReleaseImage(&phaseImage);
}


int task_3() 
{
    return 0;
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
	IplImage * original = 0;
		char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
		original = cvLoadImage(filename, 0);
    task_2(IplImage * original = 0, float accuracy);
    break;
    case 3:
    task_3();
    break;
    case 0:
    exit(1);
    default:
    break;
    }


    return 0;
}





