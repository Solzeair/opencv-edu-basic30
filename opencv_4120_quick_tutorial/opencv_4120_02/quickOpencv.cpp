#include "quickOpencv.h"

void QuickDemo::colotSpaceDemo(Mat& image){
	Mat gray, hsv;
	// Convert Color（颜色空间转换）。
	// 把一张图片的颜色信息，从一种“语言”（颜色空间）翻译成另一种“语言”
	cvtColor(image, gray, COLOR_BGR2GRAY);
	// HSV色彩空间 色调0-180、饱和度0-255 和亮度0-255
	cvtColor(image, hsv, COLOR_BGR2HSV);
	imshow("HSV", hsv);
	imshow("GRAY", gray);
	// 将 Mat 对象（内存中的矩阵数据）按照指定的格式存入硬盘。
	// 有三种参数
	imwrite("E:/opencv/image/hsv_result.png", hsv);
	imwrite("E:/opencv/image/gray_result.png", gray);
}
