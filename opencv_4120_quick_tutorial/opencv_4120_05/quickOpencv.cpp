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
	namedWindow("output image2", cv::WINDOW_FREERATIO);
	imshow("output image2", image);
}

void QuickDemo::operate_demo(Mat& image)
{
	int h = image.rows;
	int w = image.cols;
	int dims = image.channels();
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			Vec3b p1 = image.at<Vec3b>(row, col);
			Vec3b p2 = image.at<Vec3b>(row, col);
			// image.at<Vec3b>(row, col)[0] = p1[0] + p2[0]; 溢出
			// saturate_cast 类型转换函数 采用的是饱和（Saturation）策略：
			// 如果计算结果 > 255：强制截断为 255（保持最亮）; < 0： 0
			image.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(p1[0] + p2[0]);
			image.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(p1[1] + p2[1]);
			image.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(p1[2] + p2[2]);
		}
	}

	// Scalar 是 OpenCV 用来存放颜色或向量数值的类 4通道
	// OpenCV 内部针对这种像素级的数学运算做了饱和度保护，强制截断不会溢出
	// 重载运算符 乘法不行，不知道点乘还是叉乘
	image /= Scalar(5, 5, 5);
	image += Scalar(5, 5, 5);
	image -= Scalar(5, 5, 5);

	// 计算函数
	Mat dst = Mat::zeros(image.size(), image.type());;
	Mat m = Mat::zeros(image.size(), image.type());
	multiply(image, m, dst);
	divide(image, m, dst);
	add(image, m, dst);
	subtract(image, m, dst);

	namedWindow("output2", cv::WINDOW_FREERATIO);
	imshow("output2", image);
}



