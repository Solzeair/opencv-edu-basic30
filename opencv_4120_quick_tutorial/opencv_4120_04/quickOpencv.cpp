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
	//for (int row = 0; row < h; row++) {
	//	for (int col = 0; col < w; col++) {
	//		if (dims == 1) {
	//			int pv = image.at<uchar>(row, col); // 灰度图像素值单字节
	//			image.at<uchar>(row, col) = 255 - pv;
	//		}
	//		else if (dims == 3) {
	//			// at 访问该行列像素点
	//			// 结构体：Vec3b Vec3f 分别存放 B G R 的亮度值
	//			Vec3b bgr = image.at<Vec3b>(row, col);
	//			image.at<Vec3b>(row, col)[0] = 255 - bgr[0];
	//			image.at<Vec3b>(row, col)[1] = 255 - bgr[1];
	//			image.at<Vec3b>(row, col)[2] = 255 - bgr[2];
	//		}
	//	}
	//}
	// 这样时间复杂度更好，不用每次计算二维地址
	for (int row = 0; row < h; row++) {
		// 直接返回图像第 row 行的行首内存地址（指针）
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



