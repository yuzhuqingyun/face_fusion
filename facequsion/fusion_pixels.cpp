/******************************************************
[����]	
	��۽��ں�
	1�����ڵ������ص���ں��㷨
	   �԰���������ݶ�
	   1  1  1
	   1 -8  1
	   1  1  1
	   �õ����grand_1,grand_2
	   method1:
	     �õ��ȽϽ����������̬ѧ�������㣬ʹ�ý�Ϊƽ̹
		 Ȼ����������ݶ�ȡ��ķ����������
	   method2:
		 �õ��ȽϽ��������һ����ֵ�Ľ��������ݶ�ȡ�󷨣�
		 ����ֵ��Χ�ڵģ�ȡ��Ȩƽ��

*******************************************************/


//-------------------�ļ�����----------------------
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include "fusion_pixels.h"
//------------------�����ռ�---------------------
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

	Mat grade_img_1;    //��Եͼ
	Mat grade_img_2;
	Mat dst_img_2(src_img_1.size(), CV_8UC1);

	// ��ͼ������˲�
	float kernel_value[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	Mat kernel(3 , 3, CV_32F, kernel_value);
	filter2D(src_img_1, grade_img_1, CV_32F, kernel);
	filter2D(src_img_2, grade_img_2, CV_32F, kernel);
	
	// method 1: compare and Mathematical Morphology
	// Ч������
	//ȡ��Եֵ���
	Mat compare_result = abs(grade_img_1) > abs(grade_img_2);   //grade_img_1��Ե�ϴ�Ϊ1������Ϊ0
	Mat compare_result_temp;
	uchar element_data[9] = {0,1,0,1,1,1,0,1,0};
	Mat element = Mat(3,3,CV_8UC1, element_data);
	morphologyEx(compare_result, compare_result_temp, MORPH_CLOSE, element, Point(-1,-1), 1);   //������
	
	morphologyEx(compare_result_temp, compare_result_temp, MORPH_OPEN, element, Point(-1,-1), 1);   //������
	//imshow("result", compare_result);
	//imshow("result_temp", compare_result_temp);
	//waitKey();
	//destroyAllWindows();
	unsigned char *ptr_dst_2 = dst_img_2.ptr<uchar>(0);
	unsigned char *ptr_grand = compare_result_temp.ptr<uchar>(0);
	unsigned char *ptrSrc1 = src_img_1.ptr<uchar>(0);
	unsigned char *ptrSrc2 = src_img_2.ptr<uchar>(0);
	for (int i = 0; i < ranges; i++)    //��dst_img_2�������Ϊ��Եֵ�ϴ��ֵ
	{
		if (*ptr_grand != 0)	//grade_img_1��
		{
			*ptr_dst_2 = *ptrSrc1;
		}
		else
			*ptr_dst_2 = *ptrSrc2;	//grade_img_2��
		ptr_dst_2++;
		ptr_grand++;
		ptrSrc1++;
		ptrSrc2++;
	}
	imshow("src1", src_img_1);
	imshow("src2", src_img_2);
	imshow("dst2", dst_img_2);	//ȡ�ϴ�ֵ
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

	Mat grade_img_1;    //��Եͼ
	Mat grade_img_2;
	Mat dst_img_1(src_img_1.size(), CV_8UC1);

	// ��ͼ������˲�
	float kernel_value[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	Mat kernel(3 , 3, CV_32F, kernel_value);
	filter2D(src_img_1, grade_img_1, CV_32F, kernel);
	filter2D(src_img_2, grade_img_2, CV_32F, kernel);

	Mat compare_result = abs(grade_img_1) > abs(grade_img_2);   //grade_img_1��Ե�ϴ�Ϊ1������Ϊ0
	Mat compare_result_temp;
	uchar element_data[9] = {0,1,0,1,1,1,0,1,0};
	Mat element = Mat(3,3,CV_8UC1, element_data);
	morphologyEx(compare_result, compare_result_temp, MORPH_CLOSE, element, Point(-1,-1), 1);   //������

	morphologyEx(compare_result_temp, compare_result_temp, MORPH_OPEN, element, Point(-1,-1), 1);   //������
	//imshow("result", compare_result);
	//imshow("result_temp", compare_result_temp);
	//waitKey();
	//destroyAllWindows();

	// method 2:
	// Ч����method 1�ã���ģ��
	//��Ե��ֵ�����ֵʱ��ȡ�󣬷���ȡƽ��

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
	imshow("dst1", dst_img_1);	//ȡƽ��
	imshow("result", compare_result);
	waitKey();
	destroyAllWindows();

	return dst_img_1;
}

