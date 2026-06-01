#include <opencv2/opencv.hpp>
#include "quickOpencv.h"
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	Mat src = imread("E:/Course/opencv/image/123.png");
	if (src.empty()) {
		printf("could not load image...");
		return -1;
	}
	namedWindow("output image", cv::WINDOW_FREERATIO);
	imshow("output image", src);

	QuickDemo qd;
	// qd.colotSpaceDemo(src);
	// qd.mat_creation_demo(src);
	qd.pixel_visit_demo(src);

	waitKey(0);
	destroyAllWindows();

	return 0;
}