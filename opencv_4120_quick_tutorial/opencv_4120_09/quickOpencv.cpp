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
	imshow("键盘输出", image);
	int c = waitKey(1);
	cout << c << endl; // 没有键盘读入输出-1
	while (true) {
		c = waitKey(0);
		switch (c) {
		case 27: {
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
			dst = Mat::zeros(image.size(), image.type());
			dst = Scalar(50, 50, 50);
			add(image, dst, dst);
		} break;
		}
		imshow("键盘输出", dst);
	}
}

void QuickDemo::color_style_demo(Mat& image)
{
	int colormap[] = {
		COLORMAP_AUTUMN,
		COLORMAP_BONE,
		COLORMAP_JET,
		COLORMAP_WINTER,
		COLORMAP_RAINBOW,
		COLORMAP_OCEAN,
		COLORMAP_SUMMER,
		COLORMAP_SPRING,
		COLORMAP_COOL,
		COLORMAP_HSV,
		COLORMAP_PINK,
		COLORMAP_HOT,
		COLORMAP_PARULA,
		COLORMAP_MAGMA,
		COLORMAP_INFERNO,
		COLORMAP_PLASMA,
		COLORMAP_VIRIDIS,
		COLORMAP_CIVIDIS,
		COLORMAP_TWILIGHT,
		COLORMAP_TWILIGHT_SHIFTED,
		COLORMAP_TURBO,
		COLORMAP_DEEPGREEN
	};
	namedWindow("颜色表输出", WINDOW_FREERATIO);
	imshow("颜色表输出", image);
	int c = waitKey(1);
	cout << c << endl; // 没有键盘读入输出-1
	int index = 0;
	while (true) {
		c = waitKey(500);
		if (c == 27) {
			break;
		}
		index %= 22;
		cout << index << endl;
		applyColorMap(image, dst, colormap[index]);
		index++;
		imshow("颜色表输出", dst);
	}
}

void QuickDemo::bitwise_demo(Mat& image)
{
	Mat m1 = Mat::zeros(Size(256, 256), CV_8UC3);
	Mat m2 = Mat::zeros(Size(256, 256), CV_8UC3);
	// rectangle(m1, Point(100, 100), Point(180, 180), Scalar(255, 255, 0), -1, LINE_8, 0);
	/*
	CV_EXPORTS_W void rectangle(
		InputOutputArray img,	// 目标图像
		Rect rec,				// 矩形位置 左上角x,y，宽，高
        const Scalar& color,	// 颜色
		int thickness = 1,		// 矩形边框的粗细，单位是像素，-1：表示填满矩形内部
        int lineType = LINE_8,  // 线条类型 LINE_4：画线往上下左右 4 个方向走一格 LINE_8：包括斜方向走一格子 LINE_AA：在线条边缘用半透明的像素做过渡（抗锯齿）
		int shift = 0			// 坐标精度 表示坐标值的小数位数，实际坐标 = 你传入的值 ÷ 2^shift
	);
	*/
	rectangle(m1, Rect(100, 100, 80, 80), Scalar(255, 255, 0), -1, LINE_8, 0);
	rectangle(m2, Rect(150, 150, 80, 80), Scalar(0, 255, 255), -1, LINE_8, 0);
	imshow("m1", m1);
	imshow("m2", m2);
	Mat dst;
	// bitwise_and(m1, m2, dst); // 与
	// bitwise_or(m1, m2, dst);  // 或
	// bitwise_xor(m1, m2, dst); // 异或
	bitwise_not(image, dst); // 非
	imshow("像素位操作", dst);
	waitKey(0);

}


