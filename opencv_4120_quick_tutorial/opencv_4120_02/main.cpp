#include <opencv2/opencv.hpp>
#include "quickOpencv.h"
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	// RGB 通道顺序 B G R 共2^24种颜色
	Mat src = imread("E:/opencv/image/123.png"); 
	if (src.empty()) {
		printf("could not load image...");
		return -1;
	}
	namedWindow("output image", cv::WINDOW_FREERATIO);
	// 只支持8位的显示或者浮点数的显示
	// 类型是 CV_8U 或者数据范围在 0~1 的浮点数
	imshow("output image", src);

	QuickDemo qd;
	qd.colotSpaceDemo(src);

	waitKey(0);
	destroyAllWindows();

	return 0;
}