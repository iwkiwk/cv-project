#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("lena_std.tif");
	if (!img.data)
	{
		cout << "Could not open image." << endl;
		return -1;
	}

	namedWindow("CvTest", WINDOW_AUTOSIZE);
	imshow("CvTest", img);

	waitKey(0);

	return 0;
}