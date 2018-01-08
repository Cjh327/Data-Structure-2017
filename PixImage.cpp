#include <opencv2/opencv.hpp>
#include <iostream>
#include "PixImage.h"

using namespace std;
using namespace cv;

//初始化
PixImage::PixImage(string filePath)
{
	image = imread(filePath);
	name = filePath;
	height = image.rows;
	width = image.cols;
	size = height * width;
	imgdata = new ImageData[size];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			imgdata[i * width + j].blue = image.at<Vec3b>(i, j)[0];
			imgdata[i * width + j].green = image.at<Vec3b>(i, j)[1];
			imgdata[i * width + j].red = image.at<Vec3b>(i, j)[2];
		}
	}
}

PixImage::~PixImage()
{
	delete[]imgdata;
}

//图是否为空
bool PixImage::Empty()
{
	return image.empty();
}

//得到某位置处的像素值
int PixImage::GetBGR(int row, int col, int bgr)
{
	if (row < 0 || row >= height || col < 0 || col >= width)
		return 0;
	switch (bgr) {
	case 0: return imgdata[row * width + col].blue;
	case 1: return imgdata[row * width + col].green;
	case 2: return imgdata[row * width + col].red;
	case 3: return greydata[row * width + col];
	default: return 0;
	}
}

//模糊化
void PixImage::Blurring(int filter, int n)
{
	Mat blurredImg = image;
	if (filter == 1)
		MeanFilter(n, blurredImg);
	else
		GaussFilter(n, blurredImg);
	imshow("平滑降噪", blurredImg);
	imwrite("blurred.jpg", blurredImg);
	waitKey(0);
}

//均值滤波器
void PixImage::MeanFilter(int n, Mat &blurredImg)
{
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if ((i - n / 2 >= 0) && (i + (n - 1) / 2 < image.rows) && (j - n / 2 >= 0) && (j + (n - 1) / 2) < image.cols) {
				int sum[3] = { 0,0,0 };
				for (int k1 = 0; k1 < n; k1++)
					for (int k2 = 0; k2 < n; k2++)
						for (int bgr = 0; bgr < 3; bgr++)
							sum[bgr] += GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
				for (int bgr = 0; bgr < 3; bgr++)
					blurredImg.at<Vec3b>(i, j)[bgr] = sum[bgr] / (n * n);
			}
			else {
				for (int bgr = 0; bgr < 3; bgr++)
					blurredImg.at<Vec3b>(i, j)[bgr] = GetBGR(i, j, bgr);
			}
		}
	}
}

//高斯滤波器
void PixImage::GaussFilter(int n, Mat &blurredImg)
{
	if (n != 3 && n != 5)
		n = 3;
	if (n == 3) {
		for (int i = 0; i < blurredImg.rows; i++) {
			for (int j = 0; j < blurredImg.cols; j++) {
				int sum[3] = { 0,0,0 };
				for (int k1 = 0; k1 < n; k1++)
					for (int k2 = 0; k2 < n; k2++) {
						int dist = (k1 - n / 2) * (k1 - n / 2) + (k2 - n / 2) * (k2 - n / 2);
						if (dist == 0)
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += 4 * GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
						else if (dist == 2)
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += 2 * GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
						else
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
					}
				for (int bgr = 0; bgr < 3; bgr++)
					blurredImg.at<Vec3b>(i, j)[bgr] = sum[bgr] / 16;
			}
		}
	}
	else if (n == 5) {
		for (int i = 0; i < blurredImg.rows; i++) {
			for (int j = 0; j < blurredImg.cols; j++) {
				int sum[3] = { 0,0,0 };
				for (int k1 = 0; k1 < n; k1++)
					for (int k2 = 0; k2 < n; k2++) {
						int dist = (k1 - n / 2) * (k1 - n / 2) + (k2 - n / 2) * (k2 - n / 2);
						if (dist == 0)
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += 41 * GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
						else if (dist == 1)
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += 26 * GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
						else if (dist == 2)
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += 16 * GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
						else if (dist == 4)
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += 7 * GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
						else if (dist == 5)
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += 4 * GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
						else
							for (int bgr = 0; bgr < 3; bgr++)
								sum[bgr] += GetBGR(i - n / 2 + k1, j - n / 2 + k2, bgr);
					}
				for (int bgr = 0; bgr < 3; bgr++)
					blurredImg.at<Vec3b>(i, j)[bgr] = sum[bgr] / 273;


			}
		}
	}
}

//边缘检测
void PixImage::Sobel(int scale)
{
	Blurring(2, 5);
	Mat sobelImg = imread("blurred.jpg", 0);
//	Mat sobelImg = imread(name, 0);
	greydata = new uchar[size];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			greydata[i * width + j] = sobelImg.at<uchar>(i, j);

	double *Gray = new double[sobelImg.rows*sobelImg.cols];
	double sum = 0;

	for (int i = 0; i < sobelImg.rows; i++)
		for (int j = 0; j < sobelImg.cols; j++)
			sum += (double)sobelImg.at<uchar>(i, j);

	double mean = sum / (sobelImg.rows * sobelImg.cols);
	int thresh = 3 * sqrt(scale * mean);
	//	cout << (int)thresh << endl;

	for (int i = 0; i < sobelImg.rows; i++) {
		for (int j = 0; j < sobelImg.cols; j++) {
			int GrayX = -GetBGR(i - 1, j - 1, 3) + GetBGR(i + 1, j - 1, 3)
				- 2 * GetBGR(i - 1, j, 3) + 2 * GetBGR(i + 1, j, 3)
				- GetBGR(i - 1, j + 1, 3) + GetBGR(i + 1, j + 1, 3);
			int GrayY = GetBGR(i - 1, j - 1, 3) + 2 * GetBGR(i, j - 1, 3)
				+ GetBGR(i + 1, j - 1, 3) - GetBGR(i - 1, j + 1, 3)
				- 2 * GetBGR(i, j + 1, 3) - GetBGR(i + 1, j + 1, 3);
			Gray[i*sobelImg.cols + j] = sqrt(GrayX * GrayX + GrayY * GrayY);
		}
	}
	for (int i = 0; i < sobelImg.rows; i++) {
		for (int j = 0; j < sobelImg.cols; j++) {
			if (Gray[i*sobelImg.cols + j] >= thresh) sobelImg.at<uchar>(i, j) = 255;
			else sobelImg.at<uchar>(i, j) = 0;
		}
	}


	imshow("边缘检测", sobelImg);
	imwrite("sobel.jpg", sobelImg);
	waitKey(0);
}
