#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

#define TEST_SECTION_FILTERS        1
#define TEST_SECTION_CAMERA         0
#define TEST_SECTION_MATOPERATIONS  0

int main()
{
#if TEST_SECTION_FILTERS
	// colored image
	cv::Mat img = cv::imread("lena_std.tif");
	if (!img.data)
	{
		cout << "Could not open image." << endl;
		return -1;
	}
	cv::namedWindow("color_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("color_image", img);

	// grey image
	cv::Mat img_grey = cv::imread("lena_std.tif", 0);
	cv::namedWindow("grey_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("grey_image", img_grey);

	// gaussian blur
	cv::Mat img_gaus;
	cv::GaussianBlur(img, img_gaus, cv::Size(11, 11), 8, 8);
	cv::namedWindow("gaus_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("gaus_image", img_gaus);

	// blur
	cv::Mat img_blur;
	cv::blur(img, img_blur, cv::Size(11, 11));
	cv::namedWindow("blur_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("blur_image", img_blur);

	// median blur
	cv::Mat img_median;
	cv::medianBlur(img, img_median, 11);
	cv::namedWindow("median_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("median_image", img_median);

	// bilateral blur
	cv::Mat img_bil;
	cv::bilateralFilter(img, img_bil, 7, 14, 3);
	cv::namedWindow("bil_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("bil_image", img_bil);

	// canny edges
	cv::Mat img_edge;
	cv::bilateralFilter(img, img_edge, 7, 7, 3);
	cv::cvtColor(img_edge, img_edge, cv::COLOR_BGR2GRAY);
	cv::Canny(img_edge, img_edge, 50, 150);
	cv::namedWindow("bil_edge", cv::WINDOW_AUTOSIZE);
	cv::imshow("bil_edge", img_edge);

	cv::waitKey(0);
#endif

#if TEST_SECTION_MATOPERATIONS
	// cv::Mat operations
	cv::Mat A(4,3,CV_64F);
	auto B = A;
	auto C = B.row(3);
	auto D = C.clone();
	B.row(3).copyTo(C);
	A = D;
	B.release();
	C = C.clone();

#endif

#if TEST_SECTION_CAMERA
	// Video operation
	cv::VideoCapture cap(0);
	if(!cap.isOpened()) { cout << "Cannot open camera.\n"; return -1; }
	cv::Mat frame, edges;
	cv::namedWindow("edges", cv::WINDOW_AUTOSIZE);
	while(1)
	{
		cap >> frame;
		cv::bilateralFilter(frame, edges, 7, 7, 3);
		cv::cvtColor(edges, edges, cv::COLOR_BGR2GRAY);
		//cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
		//cv::blur(edges, edges, cv::Size(4,4));
		cv::Canny(edges, edges, 20, 150, 3);
		cv::imshow("edges", edges);
		if(cv::waitKey(10) >= 0) break;
	}
#endif
	return 0;
}