#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "helpers.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

namespace Padding {
	typedef enum {
		ZERO,
		REPLICA,
	} PaddingStyle_t;
}

vector<vector<unsigned char>> medianBlur(const vector<vector<unsigned char>>& img, vector<int> kernel, Padding::PaddingStyle_t padding_way)
{
	int h = img.size();
	assert(h > 0);
	int w = img[0].size();

	int m = kernel[0];
	int n = kernel[1];
	assert(m > 0 && n > 0);
	assert(m < w && n < h);

	int padLeft = (int)((m-1)/2);
	int padRight = (m-1)-padLeft;

	int padTop = (int)((n-1)/2);
	int padBottom = (n-1)-padTop;

	vector<unsigned char> row_img(h+n-1, 0);
	vector<vector<unsigned char>> dstImage(w+m-1, row_img);

	switch (padding_way)
	{
	case Padding::ZERO:
	{
		for (int i = 0; i < w + m - 1; i++)
		{
			for (int j = 0; j < h + n - 1; j++)
			{
				if (i >= padLeft && i < (padLeft + w))
					if (j >= padTop && j < (padTop + h))
						dstImage[i][j] = img[i - padLeft][j - padTop];
			}
		}
	}
	break;
	case Padding::REPLICA:
		break;
	default:
		break;
	}

	return dstImage;
}

int main()
{
	string imgfile = "lena_std.tif";
	cv::Mat img = cv::imread(imgfile, cv::IMREAD_GRAYSCALE);
	//cv::Mat img = cv::imread(imgfile);
	display_image("lena", img);

	cv::Mat saltImg = addSaltNoise(img, 10000);
	display_image("salt_image", saltImg);

	auto && imgVec = cvMatToVecImage(img);
	debugMemPrint(&imgVec[0][0], 1, 0, 100);

	auto && padZeroImage = medianBlur(imgVec, {100, 100}, Padding::ZERO);

	cv::Mat padImg = vecImageToCvMat(padZeroImage);

	//cv::waitKey(0);
	//cv::destroyAllWindows();

	return 0;
}
