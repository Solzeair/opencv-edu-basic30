#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

// 通道，是指图像中包含的独立颜色信息的层数。
// 位深，是指每一个像素（或者每个通道的像素）用多少个二进制位（Bits）来存储颜色数值。它决定了色彩的渐变过渡有多平滑。
int main(int argc, char** argv) {
	// 读取图像作为二位数组矩阵 两参数 图像路径 flag
	// flag 默认IMREAD_COLOR彩色三通道 1 IMREAD_GRAYSCALE灰色一通道 0 
	// 三通道 一个通道8位 位图深度24 opencv中用枚举类型表示
	// IMREAD_UNCHANGED不处理-1 常用于png3+1透明通道 IMREAD_ANYCOLOR浮点数据hsv色彩空间
	cv::Mat src = cv::imread("C:/Users/30845/Pictures/Screenshots/1.png", IMREAD_GRAYSCALE);
	if (src.empty()) {
		printf("could not load image...");
		return -1;
	}
	// 返回通道数
	cout << src.channels() << endl;
	// 返回每个通道的数据类型深度 0	CV_8U 8位无符号整数
	cout << src.depth() << endl;
	// OpenCV窗口管理机制：窗口名称就是 OpenCV 内部管理窗口的 “唯一标识符，相同的名字使用同一个窗口
	// 创建一个用于显示图像的窗口 窗口名称 flag
	// flag               可缩放 宽高比锁定
	// WINDOW_AUTOSIZE		否	是(锁定)	默认模式，最稳妥，图片多大窗口就多大。
	// WINDOW_NORMAL		是	是(锁定)	屏幕放不下大图时，通过鼠标缩小查看。
	// WINDOW_FREERATIO	    是	否(解锁)	需要拉伸变形来挤压窗口空间，或特殊比例显示。
	// 由于 win GUI 机制 NORMAL 和 FREERATIO 效果相同，但是在qt中不同
	cv::namedWindow("output image", cv::WINDOW_FREERATIO);
	// 输出显示图像大小保持一致 窗口名称 矩阵对象
	// 将内存里 Mat 对象的像素数据渲染到屏幕的窗口中显示出来
	cv::imshow("output image", src);
	// 0表示阻塞 停止 1表示停顿1ms继续执行 
	cv::waitKey(0);
	// 之前创建的所有窗口全部销毁掉
	cv::destroyAllWindows();

	return 0;
}