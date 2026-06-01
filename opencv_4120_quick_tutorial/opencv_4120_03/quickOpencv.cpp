#include "quickOpencv.h"

void QuickDemo::colotSpaceDemo(Mat& image){
	Mat gray, hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	// HSV色彩空间 色调0-180、饱和度0-255 和亮度0-255
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("HSV", hsv);
	imshow("GRAY", gray);
	// 有三种参数
	imwrite("E:/opencv/image/hsv_result.png", hsv);
	imwrite("E:/opencv/image/gray_result.png", gray);
}

void QuickDemo::mat_creation_demo(Mat& image)
{
	Mat m1, m2;
	m1 = image.clone();
	image.copyTo(m2);

	// 创建空白图像
	// Mat m3 = Mat::ones(Size(8, 8), CV_8UC3);
	// 8U 8位无符号整数 C3 3通道
	// ones初始话多通道只会给第一个通道赋值，zeros全为0
	Mat m3 = Mat::zeros(Size(8, 8), CV_8UC3);
	// m3 = 127; // 给第一个通道赋值为127
	m3 = Scalar(0, 0, 255); // 给任意通道赋值
	// 输出矩阵 行长度为通道数乘以列数
	cout << m3 << endl;
	cout << "column=" << m3.cols << " row=" << m3.rows << " channels=" << m3.channels() << endl;
	namedWindow("output image", WINDOW_FREERATIO);
	imshow("output image", m3);

	// Mat m4 = m3; // 赋值 指针指向同一内存，都改变
	// Mat m4 = m3.clone(); // 克隆和拷贝 只改变当前内存的值
	Mat m4;
	m3.copyTo(m4);
	m4 = Scalar(0, 255, 255); 
	cout << m4 << endl;
	namedWindow("output image", WINDOW_FREERATIO);
	imshow("output image", m4);
	namedWindow("output image2", WINDOW_FREERATIO);
	imshow("output image2", m3);

	// 模板 创建小尺寸矩阵写法（尤其是卷积核）
	Mat kernel = (Mat_<char>(3, 3) <<
		0, -1, 0,
		-1, 5, -1,
		0, -1, 0
	);
	cout << kernel << endl;
}



