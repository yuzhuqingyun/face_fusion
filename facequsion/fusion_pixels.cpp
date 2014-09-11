/******************************************************
[内容]	
	多聚焦融合
	1、基于单个像素点的融合算法
	   对八邻域计算梯度
	   1  1  1
	   1 -8  1
	   1  1  1
	   得到结果grand_1,grand_2
	   method1:
	     得到比较结果，进行形态学开闭运算，使得较为平坦
		 然后采用邻域梯度取大的方法产生结果
	   method2:
		 得到比较结果，超过一定阈值的进行邻域梯度取大法，
		 在阈值范围内的，取加权平均

*******************************************************/


//-------------------文件包含----------------------
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include "fusion_pixels.h"
//------------------命名空间---------------------
using namespace std;

Mat FusionGreaterPixel(Mat src_img_1, Mat src_img_2)
{
	// load source image
	//Mat src_img_1 = imread("cola1.bmp", IMREAD_GRAYSCALE);
	CV_Assert(src_img_1.data != NULL);
	//Mat src_img_2 = imread("cola2.bmp", IMREAD_GRAYSCALE);
	CV_Assert(src_img_2.data != NULL);
	
	// define
	int ranges = src_img_1.cols * src_img_1.rows;

	Mat grade_img_1;    //边缘图
	Mat grade_img_2;
	Mat dst_img_2(src_img_1.size(), CV_8UC1);

	// 对图像进行滤波
	float kernel_value[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	Mat kernel(3 , 3, CV_32F, kernel_value);
	filter2D(src_img_1, grade_img_1, CV_32F, kernel);
	filter2D(src_img_2, grade_img_2, CV_32F, kernel);
	
	// method 1: compare and Mathematical Morphology
	// 效果不佳
	//取边缘值最大
	Mat compare_result = abs(grade_img_1) > abs(grade_img_2);   //grade_img_1边缘较大为1，否则为0
	Mat compare_result_temp;
	uchar element_data[9] = {0,1,0,1,1,1,0,1,0};
	Mat element = Mat(3,3,CV_8UC1, element_data);
	morphologyEx(compare_result, compare_result_temp, MORPH_CLOSE, element, Point(-1,-1), 1);   //闭运算
	
	morphologyEx(compare_result_temp, compare_result_temp, MORPH_OPEN, element, Point(-1,-1), 1);   //开运算
	//imshow("result", compare_result);
	//imshow("result_temp", compare_result_temp);
	//waitKey();
	//destroyAllWindows();
	unsigned char *ptr_dst_2 = dst_img_2.ptr<uchar>(0);
	unsigned char *ptr_grand = compare_result_temp.ptr<uchar>(0);
	unsigned char *ptrSrc1 = src_img_1.ptr<uchar>(0);
	unsigned char *ptrSrc2 = src_img_2.ptr<uchar>(0);
	for (int i = 0; i < ranges; i++)    //将dst_img_2结果设置为边缘值较大的值
	{
		if (*ptr_grand != 0)	//grade_img_1大
		{
			*ptr_dst_2 = *ptrSrc1;
		}
		else
			*ptr_dst_2 = *ptrSrc2;	//grade_img_2大
		ptr_dst_2++;
		ptr_grand++;
		ptrSrc1++;
		ptrSrc2++;
	}
	imshow("src1", src_img_1);
	imshow("src2", src_img_2);
	imshow("dst2", dst_img_2);	//取较大值
	imshow("result", compare_result);
	waitKey();
	destroyAllWindows();

	return dst_img_2;

}

Mat FusionAveragePixel(Mat src_img_1, Mat src_img_2)
{
	// load source image
	//Mat src_img_1 = imread("cola1.bmp", IMREAD_GRAYSCALE);
	CV_Assert(src_img_1.data != NULL);
	//Mat src_img_2 = imread("cola2.bmp", IMREAD_GRAYSCALE);
	CV_Assert(src_img_2.data != NULL);

	// define
	int ranges = src_img_1.cols * src_img_1.rows;

	Mat grade_img_1;    //边缘图
	Mat grade_img_2;
	Mat dst_img_1(src_img_1.size(), CV_8UC1);

	// 对图像进行滤波
	float kernel_value[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	Mat kernel(3 , 3, CV_32F, kernel_value);
	filter2D(src_img_1, grade_img_1, CV_32F, kernel);
	filter2D(src_img_2, grade_img_2, CV_32F, kernel);

	Mat compare_result = abs(grade_img_1) > abs(grade_img_2);   //grade_img_1边缘较大为1，否则为0
	Mat compare_result_temp;
	uchar element_data[9] = {0,1,0,1,1,1,0,1,0};
	Mat element = Mat(3,3,CV_8UC1, element_data);
	morphologyEx(compare_result, compare_result_temp, MORPH_CLOSE, element, Point(-1,-1), 1);   //闭运算

	morphologyEx(compare_result_temp, compare_result_temp, MORPH_OPEN, element, Point(-1,-1), 1);   //开运算
	//imshow("result", compare_result);
	//imshow("result_temp", compare_result_temp);
	//waitKey();
	//destroyAllWindows();

	// method 2:
	// 效果比method 1好，略模糊
	//边缘差值大过阈值时，取大，否则取平均

	float *pd1 = grade_img_1.ptr<float>(0);
	float *pd2 = grade_img_1.ptr<float>(0);
	unsigned char *ptr1 = src_img_1.ptr<uchar>(0);
	unsigned char *ptr2 = src_img_2.ptr<uchar>(0);
	unsigned char *ptr = dst_img_1.ptr<uchar>(0);
	int diff_threshold = 3;
	for (int i = 0; i < ranges; i++)
	{
		if ( abs(*pd1)-abs(*pd2) > diff_threshold)
		{
			*ptr = *ptr1;
		}
		else if ( abs(*pd2)-abs(*pd1) > diff_threshold)
		{
			*ptr = *ptr2;
		}
		else
			*ptr = (*ptr1+*ptr2)/2;
		pd1++;
		pd2++;
		ptr1++;
		ptr2++;
		ptr++;
	}
	imshow("src1", src_img_1);
	imshow("src2", src_img_2);
	imshow("dst1", dst_img_1);	//取平均
	imshow("result", compare_result);
	waitKey();
	destroyAllWindows();

	return dst_img_1;
}

