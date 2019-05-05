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

	vector<unsigned char> row_img(w+m-1, 0);
	vector<vector<unsigned char>> dstImage(h+n-1, row_img);
	vector<vector<unsigned char>> retImage(img);

	switch (padding_way)
	{
	case Padding::ZERO:
		{
		for (int j = 0; j < h + n - 1; j++)
		{
			for (int i = 0; i < w + m - 1; i++)
			{
				if (i >= padLeft && i < (padLeft + w))
					if (j >= padTop && j < (padTop + h))
						dstImage[j][i] = img[j - padTop][i - padLeft];
			}
		}
		}
		break;
	case Padding::REPLICA:
		{
		for (int j = 0; j < h + n - 1; j++)
			for (int i = 0; i < w + m - 1; i++)
				if (i < padLeft)
					if (j < padTop)
						dstImage[j][i] = img[0][0];
					else if (j >= padTop + h)
						dstImage[j][i] = img[h - 1][0];
					else
						dstImage[j][i] = img[j - padTop][0];
				else if (i >= (padLeft + w))
					if (j < padTop)
						dstImage[j][i] = img[0][w - 1];
					else if (j >= padTop + h)
						dstImage[j][i] = img[h - 1][w - 1];
					else
						dstImage[j][i] = img[j - padTop][w - 1];
				else
					if (j < padTop)
						dstImage[j][i] = img[0][i - padLeft];
					else if (j >= padTop + h)
						dstImage[j][i] = img[h - 1][i - padLeft];
					else
						dstImage[j][i] = img[j - padTop][i - padLeft];
		}
		break;
	default:
		break;
	}

	int *vec = new int[m*n];

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int l = 0; l < n; l++)
				for (int k = 0; k < m; k++)
					vec[l*m + k] = dstImage[j + l][i + k];
			double median = findMedian(vec, m*n);
			assert(median < 256);
			retImage[j][i] = (unsigned char)median;
		}
	}

	delete[] vec;

	return retImage;
}

int main()
{
	string imgfile = "lena_std.tif";
	cv::Mat img = cv::imread(imgfile, cv::IMREAD_GRAYSCALE);
	//cv::Mat img = cv::imread(imgfile);
	display_image("lena", img);

	cv::Mat saltImg = addSaltNoise(img, 10000);
	display_image("salt_image", saltImg);

	auto && imgVec = cvMatToVecImage(saltImg);
	debugMemPrint(&imgVec[0][0], 1, 0, 100);

	auto && padZeroImage = medianBlur(imgVec, {3, 3}, Padding::ZERO);
	auto && padReplicaImage = medianBlur(imgVec, {3, 3}, Padding::REPLICA);

	cv::Mat padImgZ = vecImageToCvMat(padZeroImage);
	cv::Mat padImgR = vecImageToCvMat(padReplicaImage);

	//cv::waitKey(0);
	//cv::destroyAllWindows();

	return 0;
}
