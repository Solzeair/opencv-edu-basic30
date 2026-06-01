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
static void onTrack(int, void*) { // 默认static 养成习惯 不写没影响
	m = Scalar(lightvalue, lightvalue, lightvalue);
	add(src, m, dst); // 在原图的亮度上再加50
	imshow("调整亮度", dst);
}
void QuickDemo::tracking_bat_demo1(Mat& image)
{
	namedWindow("调整亮度", WINDOW_FREERATIO);
	src = image;
	m = Mat::zeros(image.size(), image.type());
	createTrackbar("亮度条", "调整亮度", &lightvalue, mx, onTrack);
	onTrack(0, 0); // 传入的值没用， 因为之前有一次回调所以不写也没问题
}

static void onlight(int b, void* userdata) {
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());;
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(b, b, b);
	addWeighted(image, 1.0, m, 0, b, dst); // 纯亮度调整 m矩阵此时作为没被用到的占位矩阵
	imshow("对比度与亮度调整", dst);
}
static void onContrast(int b, void* userdata) {
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());;
	Mat m = Mat::zeros(image.size(), image.type());
	double contrast = b / 100.0;
	addWeighted(image, contrast, m, 0.0, 0, dst); // 纯对比度调整
	imshow("对比度与亮度调整", dst);
}
int contrastvalue = 100;
void QuickDemo::tracking_bat_demo2(Mat& image)
{
	namedWindow("对比度与亮度调整", WINDOW_FREERATIO);
	createTrackbar("亮度条", "对比度与亮度调整", &lightvalue, mx, onlight, (void*)(&image));
	createTrackbar("对比度条", "对比度与亮度调整", &contrastvalue, 200, onContrast, (void*)(&image));
}

void QuickDemo::key_demo(Mat& image)
{
	Mat dst = Mat::zeros(image.size(), image.type());
	namedWindow("键盘输出", WINDOW_FREERATIO);
	// imshow 只是把图像数据提交到了内存的显示缓冲区，并向操作系统消息队列发送了一个“请求重绘”的通知。
	// OpenCV 的图形界面是单线程的。 如果没有后续指令，CPU 会继续执行下一行代码，不会理会操作系统的图形刷新请求
	imshow("键盘输出", image);
	// 代码运行到 waitKey 时，它会在等待期间偷偷做一件事：去操作系统的消息队列里捞取所有的 UI 事件, 此时图像才会刷新
	int c = waitKey(1);
	cout << c << endl; // 没有键盘读入输出-1
	while (true) {
		// waitKey 没有特殊处理作 1，特别是视频分析，这里为了方便测试取 0 无限期阻塞等待
		// 延迟等待：在指定的时间 ms 暂停程序的执行，等待用户按下键盘按键， 并处理窗口消息队列里的事件。
		// 捕获按键：如果用户在此期间按下了键盘，它会立刻捕捉到这个按键，并返回该按键的 ASCII 码值。
		c = waitKey(0);
		switch (c) {
			case 27: {
				// “窗口对象”并不是一个真正放在函数栈里的 C++ 局部对象，而是“创建窗口的请求和控制代码”；
				// 真正的窗口活在 OpenCV/操作系统那里，不会随着函数返回自动消失，故 return 并不能销毁窗口对象。
				// 所以当函数 return 后，窗口并不会关闭，必须手动关闭
				destroyWindow("键盘输出");
				return; 
			}
			case 49: {
				cout << "operator #1" << endl;
				cvtColor(image, dst, COLOR_BGR2GRAY);
			} break;
			case 50: {
				cout << "operator #2" << endl;
				cvtColor(image, dst, COLOR_BGR2HSV);
			} break;
			case 51: {
				cout << "operator #3" << endl;
				dst = Mat::zeros(image.size(), image.type()); // 避免上一步转换成灰度导致错误
				dst = Scalar(50, 50, 50);
				add(image, dst, dst);
			} break;
		}
		imshow("键盘输出", dst);
	}
}


