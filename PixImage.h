#include <string>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

struct ImageData {
	uchar red;
	uchar green;
	uchar blue;
};

class PixImage {
public:
	PixImage(string filePath);	//构造函数
	void Blurring(int filter = 1, int n = 3);	//模糊处理
	void Sobel(int scale = 4);	//边缘检测
	bool Empty();				//图是否为空
	~PixImage();				//析构函数

private:
	Mat image;			//图片
	string name;		//图片名称
	int height, width;	//高度，宽度
	int size;			//大小
	ImageData *imgdata;	//像素
	uchar *greydata;	//灰度图像素
	int GetBGR(int row, int col, int bgr);	// 得到某位置处的像素值	rgb = 0: b   rgb = 1: g   rgb = 2: r
	void MeanFilter(int n, Mat &blurredImg);	//均值
	void GaussFilter(int n, Mat &blurredImg);	//高斯核
};
