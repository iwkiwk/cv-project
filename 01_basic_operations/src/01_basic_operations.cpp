#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "helpers.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// Test macros
#define TEST_SECTION_FILTERS        1
#define TEST_SECTION_MATOPERATIONS  1
#define TEST_SECTION_CAMERA         1
#define TEST_GAMMA_CORRECTION       1
#define TEST_HISTOGRAM_EQUALIZE     1
#define TEST_TRANSFORM              1

int main()
{
	//cv::Mat img = cv::imread("lena_std.tif");
	cv::Mat img = cv::imread("bird.png");

#if TEST_SECTION_FILTERS
	// colored image
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
	int rows = A.rows;
	int col = A.cols;
	auto B = A;
	
	auto C = B.row(3);
	auto D = C.clone();
	B.row(3).copyTo(C);
	A = D;
	B.release();
	C = C.clone();

	cout << img.step << endl; // bytes num
	cout << img.step.buf[0] << endl;
	cout << img.step.buf[1] << endl;
	cout << img.dims << endl;
	cout << img.size[0] << endl;
	cout << img.size[1] << endl;
	cout << img.size.dims() << endl;
	cout << img.channels() << endl;
	cout << img.depth() << endl;
	cout << img.type() << endl;
	cout << GetMatType(img) << endl;
	cout << img.size << endl;
	cout << img.elemSize() << endl;
	cout << img.elemSize1() << endl;

	cv::uint8_t *data = img.data;
	cout << (int)data[0] << endl;
	cout << (int)data[3] << endl;
	cout << (int)data[6] << endl;
	cout << (int)data[img.step] << endl;

	cout << (int)img.at<cv::Vec3b>(0,0)[0] << endl;
	cout << (int)img.at<cv::Vec3b>(0,1)[0] << endl;
	cout << (int)img.at<cv::Vec3b>(0,2)[0] << endl;
	cout << (int)img.at<cv::Vec3b>(1,0)[0] << endl;
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

#if TEST_GAMMA_CORRECTION
	auto adjust_gamma = [](const cv::Mat &img, double gamma = 1.0) {
		double invGamma = 1.0 / gamma;
		vector<cv::uint8_t> table;
		for (int i = 0; i < 256; i++) {
			table.push_back((cv::uint8_t)(pow(i / 255.0, invGamma) * 255));
		}
		cv::Mat img_out;
		cv::LUT(img, table, img_out);
		return img_out;
	};
	cv::namedWindow("color_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("color_image", img);
	auto img_gamma = adjust_gamma(img, 2.2);
	cv::namedWindow("gamma_img", cv::WINDOW_AUTOSIZE);
	cv::imshow("gamma_img", img_gamma);
	cv::waitKey(0);
#endif

#if TEST_HISTOGRAM_EQUALIZE
	auto equalize_histogram = [](const cv::Mat &img) {
		cv::Mat img_yuv;
		cv::cvtColor(img, img_yuv, cv::COLOR_BGR2YUV);

		vector<cv::uint8_t> yc;
		cv::uint8_t *data = img_yuv.data;
		for (int i = 0; i < img_yuv.size[0]; i++) {
			for (int j = 0; j < img_yuv.size[1]; j++) {
				int idx = i * img_yuv.size[1] + j;
				yc.push_back(data[3 * idx]);
			}
		}
		cv::equalizeHist(yc, yc);
		for (int i = 0; i < img_yuv.size[0]; i++) {
			for (int j = 0; j < img_yuv.size[1]; j++) {
				int idx = i * img_yuv.size[1] + j;
				data[3 * idx] = yc[idx];
			}
		}
		cv::cvtColor(img_yuv, img_yuv, cv::COLOR_YUV2BGR);
		return img_yuv;
	};

	auto &&img_yuv = equalize_histogram(img);
	cv::namedWindow("color_image", cv::WINDOW_AUTOSIZE);
	cv::imshow("color_image", img);
	cv::namedWindow("histogram_img", cv::WINDOW_AUTOSIZE);
	cv::imshow("histogram_img", img_yuv);
	cv::waitKey(0);
#endif

#if TEST_TRANSFORM
	cv::Mat mat = cv::getRotationMatrix2D(cv::Point(img.cols / 2, img.rows / 2), 90, 1);
	cv::Mat img_rot;
	cv::warpAffine(img, img_rot, mat, cv::Size2d(img.cols, img.rows), 1, cv::BORDER_REPLICATE);
	display_image("rotate_image", img_rot);

	auto random_warp = [](const cv::Mat &img, auto randint) {
		int height = img.rows;
		int width = img.cols;
		int channels = img.channels();

		int random_margin = 60;
		float x1 = randint(-random_margin, random_margin);
		float y1 = randint(-random_margin, random_margin);
		float x2 = randint(width - random_margin - 1, width - 1);
		float y2 = randint(-random_margin, random_margin);
		float x3 = randint(width - random_margin - 1, width - 1);
		float y3 = randint(height - random_margin - 1, height - 1);
		float x4 = randint(-random_margin, random_margin);
		float y4 = randint(height - random_margin - 1, height - 1);

		float dx1 = randint(-random_margin, random_margin);
		float dy1 = randint(-random_margin, random_margin);
		float dx2 = randint(width - random_margin - 1, width - 1);
		float dy2 = randint(-random_margin, random_margin);
		float dx3 = randint(width - random_margin - 1, width - 1);
		float dy3 = randint(height - random_margin - 1, height - 1);
		float dx4 = randint(-random_margin, random_margin);
		float dy4 = randint(height - random_margin - 1, height - 1);

		cv::Mat pts1 = (cv::Mat_<float>(4, 2) << x1, y1, x2, y2, x3, y3, x4, y4);
		cv::Mat pts2 = (cv::Mat_<float>(4, 2) << dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4);
		auto M_warp = cv::getPerspectiveTransform(pts1, pts2);
		// or
		//cv::Point2f pt1[4] = {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}};
		//cv::Point2f pt2[4] = {{dx1, dy1}, {dx2, dy2}, {dx3, dy3}, {dx4, dy4}};
		//auto M_warp = cv::getPerspectiveTransform(pt1, pt2);

		cv::Mat img_warp;
		cv::warpPerspective(img, img_warp, M_warp, cv::Size(width, height));

		return img_warp;
	};

	auto &&img_warp = random_warp(img, randint);
	display_image("warp_image", img_warp);

#endif
	cv::destroyAllWindows();

	return 0;
}