#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <random>
#include <iostream>
#include <cassert>
#include <algorithm>

// Helper functions

// Show image
auto display_image = [](const std::string &name, const cv::Mat &img) {
	cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
	cv::imshow(name, img);
};

// Generate random integers in range [a, b]
auto randint = [](int a, int b) {
	std::random_device r;
	std::default_random_engine el(r());
	std::uniform_int_distribution<int> uniform_dist(a, b);
	return uniform_dist(el);
};

// Copy opencv Mat struct to c++ vector struct
std::vector<std::vector<unsigned char>> cvMatToVecImage(const cv::Mat &img)
{
	assert(img.channels() == 1);

	int rows = img.rows;
	int cols = img.cols;

	std::vector<unsigned char> row_image(cols, 0);
	std::vector<std::vector<unsigned char>> dstImage(rows, row_image);

	const unsigned char *data = img.data;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			dstImage[i][j] = data[i*cols+j];

	return dstImage;
}

// Convert c++ vector struct to opencv Mat
cv::Mat vecImageToCvMat(const std::vector<std::vector<unsigned char>> &imgVec)
{
	int rows = imgVec.size();
	int cols = imgVec[0].size();

	cv::Mat dstImg = cv::Mat(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			dstImg.data[i*cols+j] = imgVec[i][j];

	return dstImg;
}

// Add salt noises to image
cv::Mat addSaltNoise(const cv::Mat & srcImage, int n)
{
	auto dstImage = srcImage.clone();
	int  rows     = dstImage.rows;
	int  cols     = dstImage.cols;

	for (int k = 0; k < n; k++)
	{
		int i = rand() % rows;
		int j = rand() % cols;

		// Image channels
		cv::uint8_t *data = dstImage.data;
		if (dstImage.channels() == 1)
		{
			data[i*cols+j]    = 255;
			// or
			// dstImage.at<cv::uint8_t>(i, j) = 255;
		}
		else
		{
			data[i*cols*3+j*3+0] = 255;
			data[i*cols*3+j*3+1] = 255;
			data[i*cols*3+j*3+2] = 255;
		}
	}

	for (int k = 0; k < n; k++)
	{
		int i = rand() % rows;
		int j = rand() % cols;

		// Image channels
		cv::uint8_t *data = dstImage.data;
		if (dstImage.channels() == 1)
		{
			data[i*cols+j]    = 0;
			// or
			// dstImage.at<cv::uint8_t>(i, j) = 255;
		}
		else
		{
			data[i*cols*3+j*3+0] = 0;
			data[i*cols*3+j*3+1] = 0;
			data[i*cols*3+j*3+2] = 0;
		}
	}

	return dstImage;
}

// Get Matrix type
std::string GetMatType(const cv::Mat& mat)
{
    const int mtype = mat.type();

    switch (mtype)
    {
    case CV_8UC1:  return "CV_8UC1";
    case CV_8UC2:  return "CV_8UC2";
    case CV_8UC3:  return "CV_8UC3";
    case CV_8UC4:  return "CV_8UC4";

    case CV_8SC1:  return "CV_8SC1";
    case CV_8SC2:  return "CV_8SC2";
    case CV_8SC3:  return "CV_8SC3";
    case CV_8SC4:  return "CV_8SC4";

    case CV_16UC1: return "CV_16UC1";
    case CV_16UC2: return "CV_16UC2";
    case CV_16UC3: return "CV_16UC3";
    case CV_16UC4: return "CV_16UC4";

    case CV_16SC1: return "CV_16SC1";
    case CV_16SC2: return "CV_16SC2";
    case CV_16SC3: return "CV_16SC3";
    case CV_16SC4: return "CV_16SC4";

    case CV_32SC1: return "CV_32SC1";
    case CV_32SC2: return "CV_32SC2";
    case CV_32SC3: return "CV_32SC3";
    case CV_32SC4: return "CV_32SC4";

    case CV_32FC1: return "CV_32FC1";
    case CV_32FC2: return "CV_32FC2";
    case CV_32FC3: return "CV_32FC3";
    case CV_32FC4: return "CV_32FC4";

    case CV_64FC1: return "CV_64FC1";
    case CV_64FC2: return "CV_64FC2";
    case CV_64FC3: return "CV_64FC3";
    case CV_64FC4: return "CV_64FC4";

    default:
        return "Invalid type of matrix!";
    }
}

// Print memory values of integers
void debugMemPrint(const void *data, unsigned eleByte, unsigned startIdx, unsigned endIdx, bool isSigned = false)
{
	assert(startIdx < endIdx);

	if (isSigned) {
		switch (eleByte) {
		case 1:
		{
			const char *mem = (char*)data;
			for (unsigned i = startIdx; i < endIdx; i++)
				std::cout << (int)mem[i] << "\t";
		}
		break;
		case 2:
		{
			const short *mem = (short*)data;
			for (unsigned i = startIdx; i < endIdx; i++)
				std::cout << (int)mem[i] << "\t";
		}
		break;
		case 4:
		{
			const int *mem = (int*)data;
			for (unsigned i = startIdx; i < endIdx; i++)
				std::cout << (int)mem[i] << "\t";
		}
		break;
		default:
			break;
		}
	}
	else {
		switch (eleByte) {
		case 1:
		{
			const unsigned char *mem = (unsigned char*)data;
			for (unsigned i = startIdx; i < endIdx; i++)
				std::cout << (unsigned int)mem[i] << "\t";
		}
		break;
		case 2:
		{
			const unsigned short *mem = (unsigned short*)data;
			for (unsigned i = startIdx; i < endIdx; i++)
				std::cout << (unsigned int)mem[i] << "\t";
		}
		break;
		case 4:
		{
			const unsigned int *mem = (unsigned int*)data;
			for (unsigned i = startIdx; i < endIdx; i++)
				std::cout << (unsigned int)mem[i] << "\t";
		}
		break;
		default:
			break;
		}
	}

	std::cout << "\n";
}

double findMedian(int vec[], int n)
{
	std::sort(vec, vec+n);

	if (n % 2 == 0)
		return (double)vec[n/2];

	return (double)(vec[(n-1)/2] + vec[n/2])/2.0;
}