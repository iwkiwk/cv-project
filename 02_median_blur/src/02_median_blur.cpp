#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "helpers.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int main()
{
	string imgfile = "lena_std.tif";
	cv::Mat img = cv::imread(imgfile, cv::IMREAD_GRAYSCALE);
	//cv::Mat img = cv::imread(imgfile);
	display_image("lena", img);

	cv::Mat saltImg = addSaltNoise(img, 10000);
	display_image("salt_image", saltImg);

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}