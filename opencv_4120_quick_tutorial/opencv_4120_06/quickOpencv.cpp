#include "quickOpencv.h"

void QuickDemo::colotSpaceDemo(Mat& image) {
	Mat gray, hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("HSV", hsv);
	imshow("GRAY", gray);
	imwrite("E:/opencv/image/hsv_result.png", hsv);
	imwrite("E:/opencv/image/gray_result.png", gray);
}

void QuickDemo::mat_creation_demo(Mat& image)
{
	Mat m1, m2;
	m1 = image.clone();
	image.copyTo(m2);
	Mat m3 = Mat::zeros(Size(8, 8), CV_8UC3);
	m3 = Scalar(0, 0, 255);
	cout << m3 << endl;
	cout << "column=" << m3.cols << " row=" << m3.rows << " channels=" << m3.channels() << endl;
	namedWindow("output image", WINDOW_FREERATIO);
	imshow("output image", m3);

	Mat m4;
	m3.copyTo(m4);
	m4 = Scalar(0, 255, 255);
	cout << m4 << endl;
	namedWindow("output image", WINDOW_FREERATIO);
	imshow("output image", m4);
	namedWindow("output image2", WINDOW_FREERATIO);
	imshow("output image2", m3);

	Mat kernel = (Mat_<char>(3, 3) <<
		0, -1, 0,
		-1, 5, -1,
		0, -1, 0
		);
	cout << kernel << endl;
}

void QuickDemo::pixel_visit_demo(Mat& image)
{
	int h = image.rows;
	int w = image.cols;
	int dims = image.channels();
	for (int row = 0; row < h; row++) {
		uchar* curr = image.ptr<uchar>(row);
		for (int col = 0; col < w; col++) {
			if (dims == 1) {
				int pv = *curr;
				*curr++ = 255 - pv;
			}
			else if (dims == 3) {
				*curr = 255 - *curr; curr++;
				*curr = 255 - *curr; curr++;
				*curr = 255 - *curr; curr++;
			}
		}
	}
	namedWindow("output image2", WINDOW_FREERATIO);
	imshow("output image2", image);
}

void QuickDemo::operators_demo(Mat& image)
{
	int h = image.rows;
	int w = image.cols;
	int dims = image.channels();
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			Vec3b p1 = image.at<Vec3b>(row, col);
			Vec3b p2 = image.at<Vec3b>(row, col);
			image.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(p1[0] + p2[0]);
			image.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(p1[1] + p2[1]);
			image.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(p1[2] + p2[2]);
		}
	}
	image /= Scalar(5, 5, 5);
	image += Scalar(5, 5, 5);
	image -= Scalar(5, 5, 5);

	Mat dst = Mat::zeros(image.size(), image.type());;
	Mat m = Mat::zeros(image.size(), image.type());
	multiply(image, m, dst);
	divide(image, m, dst);
	add(image, m, dst);
	subtract(image, m, dst);

	namedWindow("output2", WINDOW_FREERATIO);
	imshow("output2", image);
}

Mat dst, m, src;
int lightvalue = 50, mx = 100;
// OpenCV 回调函数只接受严格固定参数的普通函数指针 写在类里需要加static
// 而 C++ 普通成员函数自带隐藏的 this 指针导致参数不匹配，
// 故必须用 static 将其降级为不含 this 指针的静态（全局）函数。
static void onTrack(int , void* ) { // 默认static 养成习惯 不写没影响
	m = Scalar(lightvalue, lightvalue, lightvalue);
	add(src, m, dst); // 在原图的亮度上再加50
	imshow("调整亮度", dst);
}
void QuickDemo::tracking_bat_demo1(Mat& image)
{
	namedWindow("调整亮度", WINDOW_FREERATIO);
	// 中间被操作对象 即使外部声明变量也需要初始化分配对应内存
	src = image;
	m = Mat::zeros(image.size(), image.type());
	// dst = Mat::zeros(image.size(), image.type());

	// 在 winname 窗口上才会响应
	// 滚动条注册事件 自动触发一次回调
	// trackbarname, winname, int* value, int count, TrackbarCallback, void* userdata = 0
	createTrackbar("亮度条", "调整亮度", &lightvalue, mx, onTrack);
	// 滚动条响应事件
	// 激活时刻：当你的鼠标在窗口上拖动了滑块
	// 触发机制：立刻暂停并调用，把当前所在的刻度值传给 onTrack 的第一个参数
	onTrack(0, 0); // 传入的值没用， 因为之前有一次回调所以不写也没问题
}


// 亮度 -> 增减像素偏移量 整张图整体变亮或变暗
// dst = src + b ： b > 0 -> 变亮
static void onlight(int b, void* userdata) {
	// image 始终只作为输入参数 作为初始值不会被修改
	// 滑块的每一个位置，对应的都是相对于原图的计算结果
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());;
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(b, b, b);
	// 对比度和亮度的联合调整 自带安全截断
	// addWeighted(src1, alpha, src2, beta, gamma, dst)
	/*  dst = src1 * alpha + src2 * beta + gamma
		src1：第一张图，或者第一组数据
		alpha：第一张图的权重
		src2：第二张图
		beta：第二张图的权重
		gamma：最后额外加的常数
		dst：输出结果
	*/
	addWeighted(image, 1.0, m, 0, b, dst); // 纯亮度调整 m矩阵此时作为没被用到的占位矩阵
	imshow("对比度与亮度调整", dst);
}
// 对比度 -> 图像中不同亮度区域之间差异的大小
// dst = src * c ： c > 1 -> 像素差距拉大 亮的更亮、暗的更暗
static void onContrast(int b, void* userdata) {
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());;
	Mat m = Mat::zeros(image.size(), image.type());
	double contrast = b / 100.0;
	addWeighted(image, contrast, m, 0.0, 0, dst); // 纯对比度调整
	imshow("对比度与亮度调整", dst);
}
// 注册事件使用引用符号，函数运行完销毁该变量，故需要全局变量避免内存泄露
int contrastvalue = 100;
// 鼠标点击滚动条时，系统自动捕捉并调用绑定的 onChange 方法
void QuickDemo::tracking_bat_demo2(Mat& image)
{
	namedWindow("对比度与亮度调整", WINDOW_FREERATIO);
	// 当设定的标度左右边界超过临界值时，再改变标度像素也不会再改变
	// 普通函数调用：你的代码调用 OpenCV 的函数。
	// 回调函数：你写好一个函数，注册绑定给 OpenCV。当特定事件（如点击）发生，操作系统和 OpenCV 反过来调用你写的函数
	createTrackbar("亮度条", "对比度与亮度调整", &lightvalue, mx, onlight, (void*)(&image));
	createTrackbar("对比度条", "对比度与亮度调整", &contrastvalue, 200, onContrast, (void*)(&image));
}

// 消息响应机制
/*
1. 注册绑定
开发者调用 createTrackbar，在特定窗口上注册滑块，并将回调函数指针和数据地址（userdata）提交给 OpenCV 内部的映射表。
2. 硬件捕获（打包信件）
用户拖动滑块，操作系统驱动捕捉到鼠标物理事件，计算出对应的窗口和刻度值，打包成一条 UI 消息 丢进该程序的消息队列（信箱）中。
3. 消息泵送（取信派发）
主程序运行到 waitKey 时，暂停当前主线计算，去系统信箱中捞取消息。发现滑块消息后，由 OpenCV 内部调度器依据映射表进行精准派发。
4. 回调响应（拆信执行）
控制权临时移交给回调函数。函数接收到系统传过来的最新刻度值和原图地址，基于原图计算出新图层，通过 imshow 刷新窗口屏幕
*/