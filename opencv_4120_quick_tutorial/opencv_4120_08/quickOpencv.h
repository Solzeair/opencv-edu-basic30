#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class QuickDemo {
public:
	void colotSpaceDemo(Mat& image);      // 图像色彩空间转换
	void mat_creation_demo(Mat& image);   // 图像对象的创建与赋值
	void pixel_visit_demo(Mat& image);	  // 图像像素的读写操作
	void operators_demo(Mat& image);	  // 图像像素的算术操作
	void tracking_bat_demo1(Mat& image);  // 滚动条操作演示亮度
	void tracking_bat_demo2(Mat& image);  // 滚动条操作演示对比度
	void key_demo(Mat& image);			  // 键盘响应操作
	void color_style_demo(Mat& image);    // 自带颜色表操作
};