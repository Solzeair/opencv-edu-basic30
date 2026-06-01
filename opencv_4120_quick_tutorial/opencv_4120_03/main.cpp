// Mat 包含头部(Header)和数据体两部分
// 头部：记录矩阵属性（行数 | 列数 | 通道数 | 数据类型 | 指针）
// 数据体：存储实际的像素数据（像素的坐标 | 像素值）
// 直接赋值：新建并复制头部，共享内存（不同指针指向同一块像素数据）
// 克隆/拷贝操作：执行深拷贝，创建独立数据副本

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
	qd.mat_creation_demo(src);

	waitKey(0);
	destroyAllWindows();

	return 0;
}